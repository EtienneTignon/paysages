#include <iostream>
#include <vector>
#include <valarray>
#include <string>
#include <array>
#include <fstream>
#include "Solution.h"
#include "Instance.h"
#include "Fitness.h"
#include "Calc_feature.h"

using namespace std;

//Structures

struct feature_landscape
{
    string fonc_voisinage;
    string fonc_fitness;
    float rugosite;
    float rugosite_sans_neutres;
    float rugosite_neutres_ignores;
    float taux_neutre;
};

struct feature_instance
{
    string nom;
    int nbr_machine;
    int nbr_job;
    int l_bound;
    float ecart_type;
    bool glob_known;
    int opt_glob;
};

struct feature_run
{
    int nbr_iteration;
    int resultat;
    bool l_bound_atteind;
    bool glob_atteind;
    vector<pair<pair<int, int>, int>> schedule;
};

struct feature_amalgams
{
    float moyenne_iteration;
    int iteration_max;
    int iteration_min;
    float moyenne_resultat;
    int resultat_max;
    int resultat_min;

    float freq_l_bound;
    float freq_glob_atteind;

    string descente;
};

//Fonctions

feature_instance Retenir_instance(Instance inst)
{
    feature_instance i;
    i.nom = inst.get_nom();
    i.ecart_type = inst.get_ecart_type();
    i.glob_known = inst.opt_glob_known();
    i.l_bound = inst.get_l_bound();
    i.nbr_job = inst.get_nbr_job();
    i.nbr_machine = inst.get_nbr_machine();
    i.opt_glob = inst.get_opt_glob();
    return i;
}

void afficher_feature_instance(feature_instance fi)
{
    cout << "Affichage de l'instance : " << endl;
    cout << " | Nom : " << fi.nom << endl;
    cout << " | Taille : " << fi.nbr_machine << "x" << fi.nbr_job << endl;
    cout << " | Ecart type : " << fi.ecart_type << endl;
    cout << " | Borne inférieur : " << fi.l_bound << endl;
    cout << " | Optimum global : ";
    if (fi.glob_known)
        cout << fi.opt_glob << endl;
    else
        cout << "inconnu" << endl;
}

feature_landscape Retenir_paysage(Instance inst, string voisinage, string fitness)
{
    feature_landscape fl;

    fl.fonc_fitness = fitness;
    fl.fonc_voisinage = voisinage;
    fl.rugosite = Calc_feature::rugosite(inst, voisinage, fitness, "simple");
    fl.rugosite_sans_neutres = Calc_feature::rugosite(inst, voisinage, fitness, "sans_neutre");
    fl.rugosite_neutres_ignores = Calc_feature::rugosite(inst, voisinage, fitness, "sans_plateau");
    fl.taux_neutre = Calc_feature::taux_neutre(inst, voisinage, fitness);

    return fl;
}

void Afficher_feature_landscape(feature_landscape fl)
{
    cout << "Affichage du paysage : " << endl;
    cout << " | Fitness : " << fl.fonc_fitness << endl;
    cout << " | Voisinage : " << fl.fonc_voisinage << endl;
    cout << " | Rugosité : " << fl.rugosite << endl;
    cout << " | Taux de neutre : " << fl.taux_neutre << endl;
}

feature_run Retenir_run(Instance inst, string voisinage, string fitness)
{
    Solution run = Calc_feature::Hill_climbing(inst, fitness, voisinage, (fitness.find("crit") != std::string::npos));
    feature_run fr;

    int temps_total = run.get_temps_total();
    fr.glob_atteind = (temps_total == inst.get_opt_glob());
    fr.l_bound_atteind = (temps_total == inst.get_l_bound());
    fr.nbr_iteration = run.get_run().size();
    fr.resultat = temps_total;
    fr.schedule = run.get_temps();

    return fr;
}

void Afficher_feature_run(feature_run fr)
{
    cout << "Affichage de la run" << endl;
    cout << " | Résultat : " << fr.resultat << endl;
    cout << " | Nombre d'itération : " << fr.nbr_iteration << endl;
    cout << " | Optimum global atteind ? : " << fr.glob_atteind << endl;
    cout << " | Borne inférieur atteind ? : " << fr.l_bound_atteind << endl;
}

feature_amalgams Retenir_paquet_de_run(Instance inst, string voisinage, string fitness, string descente)
{
    vector<Solution> runs = {};
    int taille_paquet = 100;
    if (descente == "SHC_with_budget")
    {
        int budget = inst.get_nbr_machine() * inst.get_nbr_job() * 100;
        for (int i = 0; i < taille_paquet; i++)
            runs.push_back(Calc_feature::Hill_climbing_with_neutral(inst, fitness, voisinage, budget, (voisinage.find("crit") != std::string::npos)));
    }
    else if(descente == "SHC")
    {
        for (int i = 0; i < taille_paquet; i++)
            runs.push_back(Calc_feature::Hill_climbing(inst, fitness, voisinage, (voisinage.find("crit") != std::string::npos)));
    }
    else
    {
        cout << "ERREUR : MAUVAISE DESCENTE" << endl;
    }
    feature_amalgams fa;
    float freq_glob = 0.0;
    float freq_l_b = 0.0;
    int iter_min = runs[0].get_run().size();
    int iter_max = runs[0].get_run().size();
    int result_min = runs[0].get_temps_total();
    int result_max = runs[0].get_temps_total();
    float iter_moy = 0.0;
    float result_moy = 0.0;
    for (int i = 0; i < taille_paquet; i++)
    {
        int resultat = runs[i].get_temps_total();
        int taille_descente = runs[i].get_run().size();
        if (inst.is_opt_glob(resultat))
            freq_glob++;
        if (inst.is_lower_bound(resultat))
            freq_l_b++;
        if (resultat < result_min)
            result_min = resultat;
        else if (resultat > result_max)
            result_max = resultat;
        if (taille_descente < iter_min)
            iter_min = taille_descente;
        else if (taille_descente > iter_max)
            iter_max = taille_descente;
        iter_moy += taille_descente;
        result_moy += resultat;
    }

    fa.freq_glob_atteind = freq_glob / taille_paquet;
    fa.freq_l_bound = freq_l_b / taille_paquet;
    fa.iteration_max = iter_max;
    fa.iteration_min = iter_min;
    fa.moyenne_iteration = iter_moy / taille_paquet;
    fa.moyenne_resultat = result_moy / taille_paquet;
    fa.resultat_max = result_max;
    fa.resultat_min = result_min;
    fa.descente = descente;
    return fa;
}

void Enrengistrer_feature(feature_instance fi, feature_landscape fl, feature_amalgams fa, string filepath)
{
    ofstream fichier(filepath, ios::app);
    fichier << fi.nom << ",";
    fichier << fi.nbr_machine << "," << fi.nbr_job << "," << fi.l_bound << "," << fi.ecart_type << "," << fi.glob_known << "," << fi.opt_glob << ",";
    fichier << fl.fonc_voisinage << "," << fl.fonc_fitness << "," << fl.rugosite << "," << fl.rugosite_sans_neutres << "," << fl.rugosite_neutres_ignores << "," << fl.taux_neutre << ",";
    fichier << fa.descente << ",";
    fichier << fa.resultat_min << "," << fa.moyenne_resultat << "," << fa.resultat_max << ",";
    fichier << fa.iteration_min << "," << fa.moyenne_iteration << "," << fa.iteration_max << ",";
    fichier << fa.freq_glob_atteind << "," << fa.freq_l_bound << endl;
}

// Main()
int main(int argc, char *argv[])
{
    //Initialisation du fichier dans lequel écrire.
    string filepath = "data/test.csv";

    //Initialisation des instances
    vector<Instance> instances = {};

    if (argc >= 2)
    {
        int p = stoi(argv[1]);
        int i1 = p / 10;
        i1 += 3;
        int i2 = p % 10;
        if (i2 == 0)
            i2 = 10;
        string p1 = to_string(i1);
        string p2 = to_string(i2);

        cout << p1 << ":" << p2 << endl;

        if (p1.size() < 2)
            p1 = "0" + p1;
        if (p2.size() < 2)
            p2 = "0" + p2;

        cout << p1 << ":" << p2 << endl;
        //Instance fic = Instance("Gp03-01");

        filepath = "data/Ta" + p1 + "-" + p2 + ".csv";
        Instance fic = Instance("Ta" + p1 + "-" + p2);
        instances.push_back(fic);
    }
    else
    {
        /*
        vector<vector<int>> in2_1 =
            {{1, 3},
             {3, 7}};
        Instance inst_2_1 = Instance(2, 2, in2_1, false, 0, "2x2");
        instances.push_back(inst_2_1);
        */
        vector<vector<int>> in3_1 =
            {{661, 168, 171},
             {6, 489, 505},
             {333, 343, 324}};
        Instance inst_3_1 = Instance(3, 3, in3_1, true, 1168, "GP03-01");
        instances.push_back(inst_3_1);
        vector<vector<int>> in3_2 =
            {{497, 170, 333},
             {7, 659, 334},
             {496, 171, 333}};
        Instance inst_3_2 = Instance(3, 3, in3_2, true, 1170, "GP03-02");
        instances.push_back(inst_3_2);
        vector<vector<int>> in4_1 =
            {{638, 3, 42, 317},
             {7, 638, 69, 286},
             {4, 1, 605, 390},
             {351, 358, 284, 7}};
        Instance inst_4_1 = Instance(4, 4, in4_1, true, 1281, "GP04-01");
        instances.push_back(inst_4_1);
        vector<vector<int>> in4_2 =
            {{707, 1, 279, 13},
             {2, 41, 324, 633},
             {290, 283, 80, 347},
             {1, 675, 317, 7}};
        Instance inst_4_2 = Instance(4, 4, in4_2, true, 1270, "GP04-02");
        instances.push_back(inst_4_2);
        vector<vector<int>> in5_1 =
            {{289, 26, 35, 649, 1},
             {1, 316, 366, 254, 63},
             {392, 1, 570, 34, 3},
             {21, 1, 1, 45, 932},
             {297, 656, 28, 18, 1}};
        Instance inst_5_1 = Instance(5, 5, in5_1, true, 1245, "GP05-01");
        instances.push_back(inst_5_1);
        vector<vector<int>> in6_1 =
            {{19, 301, 1, 24, 10, 645},
             {1, 30, 320, 433, 199, 17},
             {12, 318, 9, 1, 659, 1},
             {17, 329, 642, 1, 1, 10},
             {301, 12, 27, 204, 130, 326},
             {650, 10, 1, 337, 1, 1}};
        Instance inst_6_1 = Instance(6, 6, in6_1, true, 1264, "GP06-01");
        instances.push_back(inst_6_1);
        vector<vector<int>> in10_1 =
            {{69, 458, 1, 142, 15, 2, 98, 45, 169, 1},
             {175, 1, 464, 1, 1, 14, 232, 1, 1, 110},
             {3, 66, 120, 290, 3, 15, 60, 435, 1, 7},
             {1, 7, 35, 1, 2, 365, 1, 4, 318, 266},
             {1, 117, 1, 172, 28, 41, 438, 28, 173, 1},
             {18, 1, 26, 13, 195, 1, 1, 412, 4, 329},
             {367, 1, 2, 378, 134, 54, 1, 2, 1, 60},
             {364, 22, 9, 1, 13, 1, 161, 47, 328, 54},
             {1, 2, 277, 1, 9, 506, 7, 25, 1, 171},
             {1, 325, 65, 1, 600, 1, 1, 1, 4, 1}};
        Instance inst_10_1 = Instance(10, 10, in10_1, true, 1093, "GP10-01");
        instances.push_back(inst_10_1);
    }

    //Création du fichier
    ofstream fichier(filepath);
    fichier << "nom_instance,nbr_machine,nbr_job,borne_inférieur,ecart_type,optimum_connu,optimum_global,";
    fichier << "voisinage,fitness,rugosité,rugosité_sans_neutre,rugosité_sans_plateau,taux_neutre,";
    fichier << "descente,";
    fichier << "resultat_min,resultat_moyen,resultat_max,";
    fichier << "nbr_pas_min,nbr_pas_moyen,nbr_pas_max,";
    fichier << "freq_optimum_glob,freq_borne_inf" << endl;
    fichier.close();

    //Initialisation des fitness et voisinage exploitable
    vector<string> voisinages = {"swap_critic", "insert_critic", "swap+insert_critic"};
    //vector<string> fitnesses = {"nd", "fast", "giffler", "FIFO-simple", "best"};
    //vector<string> fitnesses = {"nd", "giffler", "FIFO-simple", "FIFO-critique", "best"};
    vector<string> fitnesses = {"FIFO-simple"};

    //Features et enrengistrement des landscapes
    for (unsigned int inst = 0; inst < instances.size(); inst++)
    {
        cout << "Instance " << inst << " : " << endl;
        feature_instance fi = Retenir_instance(instances[inst]);
        for (unsigned int fit = 0; fit < fitnesses.size(); fit++)
        {
            for (unsigned int vois = 0; vois < voisinages.size(); vois++)
            {
                cout << " | Fitness : " << fitnesses[fit] << " / Voisinage : " << voisinages[vois] << endl;
                feature_landscape fl = Retenir_paysage(instances[inst], voisinages[vois], fitnesses[fit]);
                feature_amalgams fa1 = Retenir_paquet_de_run(instances[inst], voisinages[vois], fitnesses[fit], "SHC");
                //feature_amalgams fa1;
                Enrengistrer_feature(fi, fl, fa1, filepath);
                feature_amalgams fa2 = Retenir_paquet_de_run(instances[inst], voisinages[vois], fitnesses[fit], "SHC_with_budget");
                Enrengistrer_feature(fi, fl, fa2, filepath);
                vector<pair<int,float>> courbe = Calc_feature::courbe_de_rugosite(instances[inst],voisinages[vois],fitnesses[fit],"simple");
                ofstream fc("CourbeCorrelation/courbe_" + instances[inst].get_nom() + voisinages[vois] + fitnesses[fit] + ".csv");
                for (unsigned int lg = 0; lg < courbe.size(); lg++)
                    fc << courbe[lg].first << "," << courbe[lg].second << endl;
                fc.close();
            }
        }
    }

    return 0;
}

vector<pair<vector<pair<pair<int, int>, int>>, int>> permute(Instance inst, int l, int r, vector<pair<vector<pair<pair<int, int>, int>>, int>> paysage, string methode)
{
    // Base case
    if (l == r)
    {
        Fitness f;
        Solution s = f.calcul_solution(inst, methode);
        auto result = s.get_temps();
        vector<pair<pair<int, int>, int>> real_result = {};
        for (int x1 = 0; x1 < 3; x1++)
        {
            for (int x2 = 0; x2 < 3; x2++)
            {
                for (unsigned int i = 0; i < result.size(); i++)
                {
                    if (result[i].first.first == x1 && result[i].first.second == x2)
                        real_result.push_back(result[i]);
                }
            }
        }
        result = real_result;
        bool present = false;
        for (unsigned int i = 0; i < paysage.size(); i++)
        {
            if (paysage[i].first == result)
            {
                present = true;
                paysage[i].second++;
            }
        }
        if (!present)
        {
            paysage.push_back({result, 1});
            //cout << result << endl;
        }
        //s.afficher_temps();
    }
    else
    {
        // Permutations made
        for (int i = l; i <= r; i++)
        {
            // Swapping done
            inst.swap_perm(l, i);
            // Recursion called
            paysage = permute(inst, l + 1, r, paysage, methode);
            // Backtrack
            inst.swap_perm(l, i);
        }
    }
    return paysage;
}
/*
int main()
{
    //Mon instance
/*
    vector<vector<int>> in1 =
        {{1,3},
         {3,7}};
    Instance inst_1 = Instance(2, 2, in1, true, 10);
    inst_1.set_permutation({{0,1},{1,1},{0,0},{1,0}});
    */
/*
    Fitness f;
    f.calcul_solution(inst_1,"FIFO-simple");
    */
/*
    inst_1.set_permutation({{0, 1}, {0, 0}, {1, 2}, {2, 2}, {0, 2}, {1, 0}, {1, 1}, {2, 1}, {2, 0}});
    Fitness f;
    Solution sog = f.calcul_solution(inst_1, "f");
    vector<pair<pair<int, int>, int>> sg = sog.get_temps();
    for (unsigned int i = 0; i < sg.size(); i++)
    {
        cout << sg[i].first.first << "-" << sg[i].first.second << ":" << sg[i].second << "/ ";
    }
    cout << "!" << sog.get_temps_total() << "!" << endl;
    Solution sof = f.calcul_solution(inst_1, "FIFO-simple");
    vector<pair<pair<int, int>, int>> sf = sof.get_temps();
    for (unsigned int i = 0; i < sf.size(); i++)
    {
        cout << sf[i].first.first << "-" << sf[i].first.second << ":" << sf[i].second << "/ ";
    }
    cout << "!" << sof.get_temps_total() << "!" << endl;
    */
/*
    ofstream fichier("data/paysage.txt");
    int n = 4;
    vector<pair<vector<pair<pair<int, int>, int>>, int>> paysage_vide = {};
    vector<pair<vector<pair<pair<int, int>, int>>, int>> paysage = permute(inst_1, 0, n - 1, paysage_vide,"giffler");
    cout << paysage.size() << endl;
    for (unsigned int i = 0; i < paysage.size(); i++)
    {
        fichier << i << "    ";
        for (unsigned int j = 0; j < paysage[i].first.size(); j++)
        {
            fichier << "{" << paysage[i].first[j].first.first << " " << paysage[i].first[j].first.second << "}:" << paysage[i].first[j].second << " ";
        }
        fichier << "    " << paysage[i].second << endl;
    }
    */
/*
    int result_g = 0;
    int result_f = 0;
    Fitness f;
    while (result_f == result_g)
    {
        inst_1.initialisation_permutation();
        result_g = f.calcul_solution(inst_1, "giffler").get_temps_total();
        result_f = f.calcul_solution(inst_1, "FIFO-simple").get_temps_total();
    }
    inst_1.show_permutation();
    */
/*
    return 0;
}
    */

//dans le doute : [machine][job]
//remplacer vector par valarray ?

// >make
// >/.main
