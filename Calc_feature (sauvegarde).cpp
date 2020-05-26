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

using namespace std;

vector<int> Calc_feature::Random_walk(Instance inst, string fitness, string voisinage, int nbr_iteration)
{
    srand(time(NULL));
    inst.initialisation_permutation();
    pair<int, int> voisin;
    vector<int> resultat;
    Fitness fit;
    resultat.push_back(fit.calcul_solution(inst, fitness).get_temps_total());
    for (int iter = 1; iter <= nbr_iteration; iter++)
    {
        if (voisinage == "swap")
        {
            int v1 = rand() % (inst.get_permutation().size() - 1);
            int v2 = (rand() % (inst.get_permutation().size() - v1)) + v1;
            inst.swap_perm(v1, v2);
            resultat.push_back(fit.calcul_solution(inst, fitness).get_temps_total());
        }
        else
        {
            int v1 = rand() % inst.get_permutation().size();
            int v2 = v1;
            while (v2 == v1)
            {
                v2 = rand() % inst.get_permutation().size();
            }
            if (v1 < v2)
            {
                for (int dif = 0; dif < v2 - v1; dif++)
                {
                    int sw1 = v1 + dif;
                    int sw2 = v1 + dif + 1;
                    inst.swap_perm(sw1, sw2);
                }
            }
            else
            {
                for (int dif = 0; dif < v1 - v2; dif++)
                {
                    int sw1 = v1 - dif;
                    int sw2 = (v1 - dif) - 1;
                    inst.swap_perm(sw1, sw2);
                }
            }
            resultat.push_back(fit.calcul_solution(inst, fitness).get_temps_total());
        }
    }
    return resultat;
}

Solution Calc_feature::Hill_climbing(Instance inst, string fitness, string voisinage)
{
    //Initialisation de la liste des voisins et autres variables de la recherche local
    inst.initialisation_permutation();
    vector<pair<int, int>> liste_voisin = {};
    vector<int> parcours = {};
    if (voisinage == "swap")
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin - 1; i++)
        {
            for (unsigned int j = i + 1; j < nbr_voisin; j++)
            {
                liste_voisin.push_back({i, j});
            }
        }
    }
    else
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin; i++)
        {
            for (unsigned int j = 0; j < nbr_voisin; j++)
            {
                if (i != j)
                    liste_voisin.push_back({i, j});
            }
        }
    }
    bool termine = false;
    random_device rd;
    //Calcul du fitness initial
    Fitness fit;
    Solution s = fit.calcul_solution(inst, fitness);
    Solution s2 = s;
    parcours.push_back(s.get_temps_total());
    int iter = 0;
    while (!termine)
    {
        //Randomisation de la liste des voisins.
        mt19937 g(rd());
        shuffle(begin(liste_voisin), end(liste_voisin), g);
        bool meilleur_voisin_trouve = false;
        unsigned int p = 0;
        vector<pair<int, int>> save = inst.get_permutation();
        //Parcours des voisins
        while (!meilleur_voisin_trouve && p < liste_voisin.size())
        {
            if (voisinage == "swap")
            {
                inst.swap_perm(liste_voisin[p].first, liste_voisin[p].second);
            }
            else
            {
                if (liste_voisin[p].first < liste_voisin[p].second)
                {
                    for (int dif = 0; dif < liste_voisin[p].second - liste_voisin[p].first; dif++)
                    {
                        int sw1 = liste_voisin[p].first + dif;
                        int sw2 = liste_voisin[p].first + dif + 1;
                        inst.swap_perm(sw1, sw2);
                    }
                }
                else
                {
                    for (int dif = 0; dif < liste_voisin[p].first - liste_voisin[p].second; dif++)
                    {
                        int sw1 = liste_voisin[p].first - dif;
                        int sw2 = (liste_voisin[p].first - dif) - 1;
                        inst.swap_perm(sw1, sw2);
                    }
                }
            }
            s2 = fit.calcul_solution(inst, fitness);
            //cout << "Voisin n°" << p+1 << " : " << s2.get_temps_total() << endl;
            if (s2.get_temps_total() < s.get_temps_total())
            {
                meilleur_voisin_trouve = true;
                s = s2;
                parcours.push_back(s.get_temps_total());
            }
            else
            {
                inst.set_permutation(save);
                p++;
            }
        }
        if (!meilleur_voisin_trouve)
        {
            termine = true;
        }
        else
        {
            iter++;
        }
    }
    s.set_run(parcours);
    return s;
}

float Calc_feature::rugosite(Instance inst, string voisinage, string fitness)
{
    //Obtenir les résultats
    int longueur_run = 1001;
    vector<int> result = Calc_feature::Random_walk(inst, fitness, voisinage, longueur_run-1);
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
    for (int i = 0; i < longueur_run-1; i++)
    {
        diff_1.push_back(result_normalise[i] * result_normalise[i + 1]);
    }
    //Faire la somme des produits des différences (et en profiter pour calculer la variance)
    float variance = pow(result[0]-moyenne, 2);
    float s_p_diff = diff_1[0];
    for (int i = 1; i < longueur_run-1; i++)
    {
        //Variance
        variance += pow(result[i]-moyenne, 2);
        //S des P des différences
        s_p_diff += diff_1[i];
    }
    variance = variance / result.size();
    //Calculer tau
    float tau = variance / s_p_diff;
    return tau;
}

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

float Calc_feature::long_moy_descente(Instance inst, string voisinage, string fitness)
{
    valarray<float> res(1000);
#pragma omp parallel for
    for (int i = 0; i < 1000; i++)
    {
        res[i] = (float)Calc_feature::Hill_climbing(inst, fitness, voisinage).get_run().size();
    }
    float moy = res.sum() / 1000;
    return moy;
}