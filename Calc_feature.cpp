#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>
#include <valarray>
#include <cmath>
#include "Calc_feature.h"
#include "Instance.h"
#include "Fitness.h"
#include "Voisinage.h"

using namespace std;

vector<int> Calc_feature::Random_walk(Instance inst, string fitness, string voisinage, int nbr_iteration)
{
    inst.initialisation_permutation();
    vector<int> resultat = {};
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    resultat.push_back(s[0].get_temps_total());
    for (int iter = 1; iter <= nbr_iteration; iter++)
    {
        if (voisinage.find("jump") != std::string::npos)
            inst.recreer_instance_depuis_solution(s[0]);
        bool not_redundant = (voisinage.find("nrd") != std::string::npos);
        Voisinage voi(voisinage, inst, not_redundant);
        inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
        resultat.push_back(fit.calcul_fitness(inst, fitness)[0].get_temps_total());
    }
    return resultat;
}

vector<int> Calc_feature::Random_walk_without_neutral_neighbour(Instance inst, string fitness, string voisinage, int nbr_iteration)
{
    inst.initialisation_permutation();
    vector<int> resultat = {};
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    int old_value = s[0].get_temps_total();
    int new_value = s[0].get_temps_total();
    resultat.push_back(old_value);
    for (int iter = 1; iter <= nbr_iteration; iter++)
    {
        if (voisinage.find("jump") != std::string::npos)
            inst.recreer_instance_depuis_solution(s[0]);
        bool not_redundant = (voisinage.find("nrd") != std::string::npos);
        Voisinage voi(voisinage, inst, not_redundant);
        while (old_value == new_value)
        {
            auto old_perm = inst.get_permutation();
            auto new_perm = voi.Creer_voisin(old_perm);
            inst.set_permutation(new_perm);
            vector<Solution> ns = fit.calcul_fitness(inst, fitness);
            new_value = ns[0].get_temps_total();
            if (old_value == new_value)
            {
                inst.set_permutation(old_perm);
                s = ns;
            }
        }
        resultat.push_back(new_value);
        old_value = new_value;
    }
    return resultat;
}

vector<int> Calc_feature::Random_walk_with_neutral_shortcut(Instance inst, string fitness, string voisinage, int nbr_iteration)
{
    inst.initialisation_permutation();
    vector<int> resultat = {};
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    resultat.push_back(s[0].get_temps_total());
    for (int iter = 1; iter <= nbr_iteration; iter++)
    {
        if (voisinage.find("jump") != std::string::npos)
            inst.recreer_instance_depuis_solution(s[0]);
        bool not_redundant = (voisinage.find("nrd") != std::string::npos);
        Voisinage voi(voisinage, inst, not_redundant);
        inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
        s = fit.calcul_fitness(inst, fitness);
        int res = s[0].get_temps_total();
        if (res != resultat[resultat.size() - 1])
            resultat.push_back(res);
        else
            iter--;
    }
    return resultat;
}

Solution Calc_feature::Hill_climbing(Instance inst, string fitness, string voisinage, bool critic)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    //cout << "Nouvelle descente" << endl;
    inst.initialisation_permutation();
    vector<int> parcours = {};
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s2 = s;
    parcours.push_back(s[0].get_temps_total());
    if (!critic)
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            bool not_redundant = (voisinage.find("nrd") != std::string::npos);
            Voisinage voi(voisinage, inst, not_redundant);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {

                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater")
                {
                    //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                    meilleur_voisin_trouve = true;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                }
            }
            if (!meilleur_voisin_trouve)
            {
                termine = true;
            }
        }
    }
    else
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            Voisinage voi(voisinage, inst, s[0]);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {

                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater")
                {
                    //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                    meilleur_voisin_trouve = true;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                }
            }
            if (!meilleur_voisin_trouve)
            {
                termine = true;
            }
        }
    }

    s[0].set_run(parcours);
    return s[0];
}

Solution Calc_feature::Hill_climbing_with_budget_and_restart(Instance inst, string fitness, string voisinage, int budget, bool critic, bool frein, int budget_frein)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    vector<int> parcours = {};
    random_device rd;
    bool done = false;
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s_fin = s;
    int run = 0;
    while (budget > 0)
    {
        run++;
        inst.initialisation_permutation();
        bool termine = false;
        if (run != 1)
            s = fit.calcul_fitness(inst, fitness);
        vector<Solution> s2 = s;
        parcours.push_back(s[0].get_temps_total());
        if (!critic)
        {
            while (!termine)
            {
                int voisin_parcouru = 0;
                int cc = 0;
                if (voisinage.find("jump") != std::string::npos)
                    inst.recreer_instance_depuis_solution(s[0]);
                //Randomisation de la liste des voisins.
                bool not_redundant = (voisinage.find("nrd") != std::string::npos);
                Voisinage voi(voisinage, inst, not_redundant);
                voi.Reinitialiser_voisin();
                bool meilleur_voisin_trouve = false;
                vector<pair<int, int>> save = inst.get_permutation();
                //Parcours des voisins
                while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru() && budget > 0 && (!frein || voisin_parcouru < budget_frein))
                {
                    voisin_parcouru ++;
                    inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                    s2 = fit.calcul_fitness(inst, fitness);
                    budget--;
                    cc++;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                    {
                        //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                        meilleur_voisin_trouve = true;
                        s = s2;
                        parcours.push_back(s[0].get_temps_total());
                    }
                    else
                    {
                        inst.set_permutation(save);
                    }
                }
                if (!meilleur_voisin_trouve || budget <= 0 || (frein && voisin_parcouru >= budget_frein))
                {
                    termine = true;
                }
            }
        }
        else
        {
            while (!termine)
            {
                if (voisinage.find("jump") != std::string::npos)
                    inst.recreer_instance_depuis_solution(s[0]);
                //Randomisation de la liste des voisins.
                Voisinage voi(voisinage, inst, s[0]);
                voi.Reinitialiser_voisin();
                bool meilleur_voisin_trouve = false;
                vector<pair<int, int>> save = inst.get_permutation();
                //Parcours des voisins
                while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
                {

                    inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                    s2 = fit.calcul_fitness(inst, fitness);
                    budget--;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                    {
                        //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                        meilleur_voisin_trouve = true;
                        s = s2;
                        parcours.push_back(s[0].get_temps_total());
                    }
                    else
                    {
                        inst.set_permutation(save);
                    }
                }
                if (!meilleur_voisin_trouve || budget <= 0)
                {
                    termine = true;
                }
            }
        }
        if (fit.comparaison_fitness(s, s_fin) == "lower")
        {
            s_fin = s;
        }
        //cout << "->" << s[0].get_temps_total() << ", val_min = " << s_fin[0].get_temps_total() << endl;
        //cout<< "->" << s[0].get_temps_total() << endl;
    }
    s_fin[0].set_run(parcours);
    return s_fin[0];
}

Solution Calc_feature::Hill_climbing_with_neutral(Instance inst, string fitness, string voisinage, int budget, bool critic)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    //cout << "Nouvelle descente" << endl;
    inst.initialisation_permutation();
    vector<int> parcours = {};
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s2 = s;
    parcours.push_back(s[0].get_temps_total());
    int budget_perdu = 0;
    if (!critic)
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            bool not_redundant = (voisinage.find("nrd") != std::string::npos);
            Voisinage voi(voisinage, inst, not_redundant);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                        budget_perdu = 0;
                    else
                        budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }
    else
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            Voisinage voi(voisinage, inst, s[0]);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                        budget_perdu = 0;
                    else
                        budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }

    s[0].set_run(parcours);
    return s[0];
}

Solution Calc_feature::Hill_climbing_and_best(Instance inst, string fitness, string voisinage, bool critic)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    //cout << "Nouvelle descente" << endl;
    inst.initialisation_permutation();
    vector<int> parcours = {};
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s2 = s;
    parcours.push_back(s[0].get_temps_total());
    if (!critic)
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            bool not_redundant = (voisinage.find("nrd") != std::string::npos);
            Voisinage voi(voisinage, inst, not_redundant);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {

                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater")
                {
                    //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                    meilleur_voisin_trouve = true;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                }
            }
            if (!meilleur_voisin_trouve)
            {
                termine = true;
            }
        }
    }
    else
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            Voisinage voi(voisinage, inst, s[0]);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {

                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater")
                {
                    //cout << s.get_temps_total() << "->" << s2.get_temps_total() << endl;
                    meilleur_voisin_trouve = true;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                }
            }
            if (!meilleur_voisin_trouve)
            {
                termine = true;
            }
        }
    }
    Solution s_best = fit.calcul_solution(inst, "best");
    parcours.push_back(s_best.get_temps_total());
    s[0].set_run(parcours);
    return s[0];
}

Solution Calc_feature::Hill_climbing_with_neutral_and_best(Instance inst, string fitness, string voisinage, int budget, bool critic)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    //cout << "Nouvelle descente" << endl;
    inst.initialisation_permutation();
    vector<int> parcours = {};
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s2 = s;
    parcours.push_back(s[0].get_temps_total());
    int budget_perdu = 0;
    if (!critic)
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            bool not_redundant = (voisinage.find("nrd") != std::string::npos);
            Voisinage voi(voisinage, inst, not_redundant);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                        budget_perdu = 0;
                    else
                        budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }
    else
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            Voisinage voi(voisinage, inst, s[0]);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    if (fit.comparaison_fitness(s, s2) == "greater")
                        budget_perdu = 0;
                    else
                        budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }
    Solution s_best = fit.calcul_solution(inst, "best");
    parcours.push_back(s_best.get_temps_total());
    s[0].set_run(parcours);
    return s[0];
}

Solution Calc_feature::Hill_climbing_with_budget(Instance inst, string fitness, string voisinage, int budget, bool critic)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    //cout << "Nouvelle descente" << endl;
    inst.initialisation_permutation();
    vector<int> parcours = {};
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    vector<Solution> s2 = s;
    parcours.push_back(s[0].get_temps_total());
    int budget_perdu = 0;
    if (!critic)
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            bool not_redundant = (voisinage.find("nrd") != std::string::npos);
            Voisinage voi(voisinage, inst, not_redundant);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }
    else
    {
        while (!termine)
        {
            if (voisinage.find("jump") != std::string::npos)
                inst.recreer_instance_depuis_solution(s[0]);
            //Randomisation de la liste des voisins.
            Voisinage voi(voisinage, inst, s[0]);
            voi.Reinitialiser_voisin();
            bool meilleur_voisin_trouve = false;
            vector<pair<int, int>> save = inst.get_permutation();
            //Parcours des voisins
            while (!meilleur_voisin_trouve && !voi.Voisinage_parcouru())
            {
                inst.set_permutation(voi.Creer_voisin(inst.get_permutation()));
                s2 = fit.calcul_fitness(inst, fitness);
                if (fit.comparaison_fitness(s, s2) == "greater" || fit.comparaison_fitness(s, s2) == "equal")
                {
                    meilleur_voisin_trouve = true;
                    budget_perdu++;
                    s = s2;
                    parcours.push_back(s[0].get_temps_total());
                }
                else
                {
                    inst.set_permutation(save);
                    budget_perdu++;
                }
            }
            if (!meilleur_voisin_trouve || budget_perdu >= budget)
            {
                termine = true;
            }
        }
    }

    s[0].set_run(parcours);
    return s[0];
}

Solution Calc_feature::Descente_ILS(Instance inst, string fitness, string voisinage, int budget, int ln_saut, bool critic)
{
    cout << "TODO Solution Calc_feature::Descente_ILS(Instance inst, string fitness, string voisinage, int budget, int ln_saut, bool critic)" << endl;
    Solution s = Solution({});
    return s;
}

float Calc_feature::rugosite(Instance inst, string voisinage, string fitness, string rugosite)
{
    //Obtenir les résultats
    int longueur_run = 1001;
    vector<int> result = {};
    if (rugosite == "simple")
        result = Calc_feature::Random_walk(inst, fitness, voisinage, longueur_run - 1);
    else if (rugosite == "sans_neutre")
        result = Calc_feature::Random_walk_without_neutral_neighbour(inst, fitness, voisinage, longueur_run - 1);
    else if (rugosite == "sans_plateau")
        result = Calc_feature::Random_walk_with_neutral_shortcut(inst, fitness, voisinage, longueur_run - 1);
    //Normaliser les résultats
    float moyenne = 0.0;
    for (int i = 0; i < longueur_run; i++)
    {
        moyenne += result[i];
    }
    moyenne = moyenne / longueur_run;
    vector<float> result_normalise = {};
    for (int i = 0; i < longueur_run; i++)
    {
        float r = result[i];
        result_normalise.push_back(r - moyenne);
    }
    //Calculer les différences.
    vector<float> diff_1 = {};
    for (int i = 0; i < longueur_run - 1; i++)
    {
        diff_1.push_back(result_normalise[i] * result_normalise[i + 1]);
    }
    //Faire la somme des produits des différences (et en profiter pour calculer la variance)
    float variance = pow(result[0] - moyenne, 2);
    float s_p_diff = diff_1[0];
    for (int i = 1; i < longueur_run - 1; i++)
    {
        //Variance
        variance += pow(result[i] - moyenne, 2);
        //S des P des différences
        s_p_diff += diff_1[i];
    }
    variance += pow(result[longueur_run - 1] - moyenne, 2);
    //Calculer tau
    float p1 = s_p_diff / variance;
    float tau = -1.0 / (log(p1));
    return tau;
}

vector<pair<int, float>> Calc_feature::courbe_de_rugosite(Instance inst, string voisinage, string fitness, string rugosite)
{
    vector<pair<int, float>> courbe = {};
    for (int j = 1; j < 15; j++)
    {
        //Obtenir les résultats
        int longueur_run = 10001;
        vector<int> result = {};
        if (rugosite == "simple")
            result = Calc_feature::Random_walk(inst, fitness, voisinage, longueur_run - 1);
        else if (rugosite == "sans_neutre")
            result = Calc_feature::Random_walk_without_neutral_neighbour(inst, fitness, voisinage, longueur_run - 1);
        else if (rugosite == "sans_plateau")
            result = Calc_feature::Random_walk_with_neutral_shortcut(inst, fitness, voisinage, longueur_run - 1);
        //Normaliser les résultats
        float moyenne = 0.0;
        for (int i = 0; i < longueur_run; i++)
        {
            moyenne += result[i];
        }
        moyenne = moyenne / longueur_run;
        vector<float> result_normalise = {};
        for (int i = 0; i < longueur_run; i++)
        {
            float r = result[i];
            result_normalise.push_back(r - moyenne);
        }
        //Calculer les différences.
        vector<float> diff_1 = {};
        for (int i = 0; i < longueur_run - j; i++)
        {
            diff_1.push_back(result_normalise[i] * result_normalise[i + j]);
        }
        //Faire la somme des produits des différences (et en profiter pour calculer la variance)
        float variance = pow(result[0] - moyenne, 2);
        float s_p_diff = diff_1[0];
        for (int i = 1; i < longueur_run - j; i++)
        {
            //Variance
            variance += pow(result[i] - moyenne, 2);
            //S des P des différences
            s_p_diff += diff_1[i];
        }
        variance += pow(result[longueur_run - j] - moyenne, 2);
        //Calculer tau
        float p = s_p_diff / variance;
        pair<int, float> f;
        f.first = j;
        f.second = p;
        courbe.push_back(f);
    }
    return courbe;
}

/*
float Calc_feature::taux_neutre(Instance inst, string voisinage, string fitness)
{
    vector<int> result = Calc_feature::Random_walk(inst, fitness, voisinage, 10000);
    float nbr_trans_neutre = 0;
    for (unsigned int i = 0; i < result.size() - 1; i++)
    {
        if (result[i] == result[i + 1])
            nbr_trans_neutre++;
    }
    float ratio = nbr_trans_neutre / (result.size() - 1);
    return ratio;
}
*/

float Calc_feature::taux_neutre(Instance inst, string voisinage, string fitness)
{
    int nbr_trans_neutre = 0;
    inst.initialisation_permutation();
    Fitness fit;
    vector<Solution> s = fit.calcul_fitness(inst, fitness);
    for (int iter = 1; iter <= 10000; iter++)
    {
        Instance inst2 = inst;
        if (voisinage.find("jump") != std::string::npos)
            inst2.recreer_instance_depuis_solution(s[0]);
        bool not_redundant = (voisinage.find("nrd") != std::string::npos);
        Voisinage voi(voisinage, inst2, not_redundant);
        vector<Solution> s2 = fit.calcul_fitness(inst2, fitness);
        inst2.set_permutation(voi.Creer_voisin(inst.get_permutation()));
        if (fit.comparaison_fitness(s, s2) == "equal")
            nbr_trans_neutre++;
        s = s2;
        inst = inst2;
    }
    return (((float)nbr_trans_neutre) / 9999);
}