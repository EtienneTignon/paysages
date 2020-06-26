#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include "Fitness.h"
#include "Solution.h"
#include "Instance.h"

using namespace std;

Solution Fitness::calcul_solution(Instance inst, string methode)
{
    if (methode == "nd")
        return this->nd_engine(inst);
    else if (methode == "giffler")
        return this->giffler_engine(inst);
    else if (methode == "fast")
        return this->fast_engine(inst);
    else if (methode == "FIFO-simple")
        return this->fifo_engine(inst);
    else if (methode == "FIFO-critique")
        return this->critic_engine(inst);
    else if (methode == "best")
    {
        Solution nd = this->nd_engine(inst);
        Solution g = this->giffler_engine(inst);
        Solution fs = this->fifo_engine(inst);
        if ((nd.get_temps_total() > fs.get_temps_total()) && (g.get_temps_total() > fs.get_temps_total()))
            return fs;
        else if (g.get_temps_total() > nd.get_temps_total())
            return nd;
        else
            return g;
    }
    else
        cout << "Méthode inconnu" << endl;

    vector<pair<pair<int, int>, int>> sol = {};
    return Solution(sol);
}

vector<Solution> Fitness::calcul_fitness(Instance inst, string methode)
{
    vector<Solution> fit = {};
    if (methode == "nd" || methode == "giffler" || methode == "fast" || methode == "FIFO-simple" || methode == "FIFO-critique" || methode == "best")
    {
        fit.push_back(this->calcul_solution(inst, methode));
    }
    else if (methode == "prio")
    {
        Solution snd = this->calcul_solution(inst, "nd");
        Solution sgf = this->calcul_solution(inst, "giffler");
        Solution ssp = this->calcul_solution(inst, "FIFO-simple");
        Solution scr = this->calcul_solution(inst, "FIFO-critique");

        int sndt = snd.get_temps_total();
        int sgft = sgf.get_temps_total();
        int sspt = ssp.get_temps_total();
        int scrt = scr.get_temps_total();

        if (sndt < sgft)
        {
            if (sgft < sspt)
            {
                if (sspt < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                    fit.push_back(scr);
                }
                else if (sgft < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(sgf);
                    fit.push_back(scr);
                    fit.push_back(ssp);
                }
                else if (sgft < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(scr);
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(snd);
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                }
            }
            else if (sndt < sspt)
            {
                if (sgft < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                    fit.push_back(scr);
                }
                else if (sspt < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(ssp);
                    fit.push_back(scr);
                    fit.push_back(sgf);
                }
                else if (sndt < scrt)
                {
                    fit.push_back(snd);
                    fit.push_back(scr);
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(snd);
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                }
            }
            else
            {
                if (sgft < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(snd);
                    fit.push_back(sgf);
                    fit.push_back(scr);
                }
                else if (sndt < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(snd);
                    fit.push_back(scr);
                    fit.push_back(sgf);
                }
                else if (sspt < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(scr);
                    fit.push_back(snd);
                    fit.push_back(sgf);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(ssp);
                    fit.push_back(snd);
                    fit.push_back(sgf);
                }
            }
        }
        else
        {
            if (sndt < sspt)
            {
                if (sspt < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(snd);
                    fit.push_back(ssp);
                    fit.push_back(scr);
                }
                else if (sndt < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(snd);
                    fit.push_back(scr);
                    fit.push_back(ssp);
                }
                else if (sgft < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(scr);
                    fit.push_back(snd);
                    fit.push_back(ssp);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(sgf);
                    fit.push_back(snd);
                    fit.push_back(ssp);
                }
            }
            else if (sgft < sspt)
            {
                if (sndt < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                    fit.push_back(snd);
                    fit.push_back(scr);
                }
                else if (sspt < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                    fit.push_back(scr);
                    fit.push_back(snd);
                }
                else if (sgft < scrt)
                {
                    fit.push_back(sgf);
                    fit.push_back(scr);
                    fit.push_back(ssp);
                    fit.push_back(snd);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(sgf);
                    fit.push_back(ssp);
                    fit.push_back(snd);
                }
            }
            else
            {
                if (sndt < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                    fit.push_back(snd);
                    fit.push_back(scr);
                }
                else if (sgft < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                    fit.push_back(scr);
                    fit.push_back(snd);
                }
                else if (sspt < scrt)
                {
                    fit.push_back(ssp);
                    fit.push_back(scr);
                    fit.push_back(sgf);
                    fit.push_back(snd);
                }
                else
                {
                    fit.push_back(scr);
                    fit.push_back(ssp);
                    fit.push_back(sgf);
                    fit.push_back(snd);
                }
            }
        }
    }
    else
        cout << "Méthode inconnu" << endl;
    return fit;
}

Solution Fitness::nd_engine(Instance inst)
{
    //Initialize solution.
    vector<pair<pair<int, int>, int>> sol = {};
    //Initialize EOM and EOJ to 0.
    vector<int> EOM;
    for (int m = 0; m < inst.get_nbr_machine(); m++)
        EOM.push_back(0);
    vector<int> EOJ;
    for (int j = 0; j < inst.get_nbr_job(); j++)
        EOJ.push_back(0);

    //Initialize phi.
    vector<pair<int, int>> phi = inst.get_permutation();

    //While phi not empty.
    while (!phi.empty())
    {
        //Find the first task to do.
        int indice = 0;
        pair<int, int> task_to_chose = phi[0];
        int t = max(EOM[task_to_chose.first], EOJ[task_to_chose.second]);
        for (unsigned int i = 1; i < phi.size(); i++)
        {
            int t_prime = max(EOM[phi[i].first], EOJ[phi[i].second]);
            if (t_prime < t)
            {
                indice = i;
                task_to_chose = phi[i];
                t = t_prime;
            }
        }

        //Write the time it take.
        int end_time = t + inst.get_time(task_to_chose.first, task_to_chose.second);
        sol.push_back({task_to_chose, end_time});
        EOM[task_to_chose.first] = end_time;
        EOJ[task_to_chose.second] = end_time;

        //Remove the task from phi
        phi.erase(phi.begin() + indice);
    }

    Solution s = Solution(sol);
    s.set_chemin_critique(this->calcul_chemin_critique(s, inst));
    return s;
}

Solution Fitness::fast_engine(Instance inst)
{
    //Initialize solution.
    vector<pair<pair<int, int>, int>> sol = {};
    //Initialize EOM and EOJ to 0.
    vector<int> EOM;
    for (int m = 0; m < inst.get_nbr_machine(); m++)
        EOM.push_back(0);
    vector<int> EOJ;
    for (int j = 0; j < inst.get_nbr_job(); j++)
        EOJ.push_back(0);

    //Initialize phi.
    vector<pair<int, int>> phi = inst.get_permutation();

    //While phi not empty.
    while (!phi.empty())
    {
        //Find the first task to do.
        int indice = 0;
        pair<int, int> task_to_chose = phi[0];
        int t_fin = max(EOM[task_to_chose.first], EOJ[task_to_chose.second]) + inst.get_time(task_to_chose.first, task_to_chose.second);
        for (unsigned int i = 1; i < phi.size(); i++)
        {
            int t_prime = max(EOM[phi[i].first], EOJ[phi[i].second]) + inst.get_time(phi[i].first, phi[i].second);
            if (t_prime < t_fin)
            {
                indice = i;
                task_to_chose = phi[i];
                t_fin = t_prime;
            }
        }

        //Write the time it take.
        sol.push_back({task_to_chose, t_fin});
        EOM[task_to_chose.first] = t_fin;
        EOJ[task_to_chose.second] = t_fin;

        //Remove the task from phi
        phi.erase(phi.begin() + indice);
    }

    Solution s = Solution(sol);
    s.set_chemin_critique(this->calcul_chemin_critique(s, inst));
    return s;
}

Solution Fitness::giffler_engine(Instance inst)
{
    //Initialize solution.
    vector<pair<pair<int, int>, int>> sol = {};
    //Initialize EOM and EOJ to 0.
    vector<int> EOM;
    for (int m = 0; m < inst.get_nbr_machine(); m++)
        EOM.push_back(0);
    vector<int> EOJ;
    for (int j = 0; j < inst.get_nbr_job(); j++)
        EOJ.push_back(0);

    //Initialize phi.
    vector<pair<int, int>> phi = inst.get_permutation();

    //While phi not empty.
    while (!phi.empty())
    {
        //Find the earlyest theorical endtime for the next task.
        int t_fin = max(EOM[phi[0].first], EOJ[phi[0].second]) + inst.get_time(phi[0].first, phi[0].second);
        vector<int> Mok = {phi[0].first};
        vector<int> Jok = {phi[0].second};
        for (unsigned int i = 1; i < phi.size(); i++)
        {
            int t_fin_prime = max(EOM[phi[i].first], EOJ[phi[i].second]) + inst.get_time(phi[i].first, phi[i].second);
            if (t_fin == t_fin_prime)
            {
                Mok.push_back(phi[i].first);
                Jok.push_back(phi[i].second);
            }
            else if (t_fin_prime < t_fin)
            {
                Mok = {phi[i].first};
                Jok = {phi[i].second};
                t_fin = t_fin_prime;
            }
        }

        //Take the first task who can start before that time.
        pair<int, int> task_to_chose = phi[0];
        int indice = -1;
        bool task_choosen = false;
        while (!task_choosen)
        {
            indice++;
            if ((this->est_dedans(phi[indice].first, Mok) || this->est_dedans(phi[indice].second, Jok)) && (max(EOM[phi[indice].first], EOJ[phi[indice].second]) < t_fin))
            {
                task_choosen = true;
                task_to_chose = phi[indice];
            }
        }

        //Write the time it take.
        t_fin = max(EOM[phi[indice].first], EOJ[phi[indice].second]) + inst.get_time(phi[indice].first, phi[indice].second);
        sol.push_back({task_to_chose, t_fin});
        EOM[task_to_chose.first] = t_fin;
        EOJ[task_to_chose.second] = t_fin;

        //Remove the task from phi
        phi.erase(phi.begin() + indice);
    }

    Solution s = Solution(sol);
    s.set_chemin_critique(this->calcul_chemin_critique(s, inst));
    return s;
}

Solution Fitness::fifo_engine(Instance inst)
{
    //Initialize solution.
    vector<pair<pair<int, int>, int>> sol = {};
    //Initialize sets of gaps.
    vector<vector<pair<int, int>>> MG = {};
    for (unsigned int mg = 0; mg < inst.get_nbr_machine(); mg++)
    {
        MG.push_back({});
        MG[mg].push_back({0, INT_MAX});
    }
    vector<vector<pair<int, int>>> JG = {};
    for (unsigned int jg = 0; jg < inst.get_nbr_job(); jg++)
    {
        JG.push_back({});
        JG[jg].push_back({0, INT_MAX});
    }

    //Initialize phi.
    vector<pair<int, int>> phi = inst.get_permutation();

    //While phi not empty.
    while (!phi.empty())
    {
        int t = 0;
        int t_fin;
        bool t_found = false;
        unsigned int i;
        unsigned int j;
        while (!t_found)
        {
            t_fin = t + inst.get_time(phi[0].first, phi[0].second);
            bool m_found = false;
            bool no_m_possible = false;
            bool j_found = false;
            bool no_j_possible = false;
            i = 0;
            j = 0;
            //Find the rignt gap in MG.
            while (!m_found && !no_m_possible)
            {
                if (i >= MG[phi[0].first].size())
                    no_m_possible = true;
                else if (MG[phi[0].first][i].first <= t && MG[phi[0].first][i].second >= t)
                {
                    if (MG[phi[0].first][i].second < t_fin)
                        no_m_possible = true;
                    else
                    {
                        m_found = true;
                    }
                }
                else
                {
                    i++;
                }
            }
            //Find the right gap in JG.
            if (m_found)
            {
                while (!j_found && !no_j_possible)
                {
                    if (j >= JG[phi[0].second].size())
                        no_j_possible = true;
                    else if (JG[phi[0].second][j].first <= t && JG[phi[0].second][j].second >= t)
                    {
                        if (JG[phi[0].second][j].second < t_fin)
                            no_j_possible = true;
                        else
                        {
                            j_found = true;
                        }
                    }
                    else
                    {
                        j++;
                    }
                }
            }
            if (!j_found || !m_found)
            {
                t++;
            }
            else
            {
                t_found = true;
            }
        }
        //Put the task in the schedule.
        sol.push_back({phi[0], t_fin});

        //Change the sets.
        if (t != MG[phi[0].first][i].first)
            MG[phi[0].first].push_back({MG[phi[0].first][i].first, t});
        if (t != JG[phi[0].second][j].first)
            JG[phi[0].second].push_back({JG[phi[0].second][j].first, t});
        if (t_fin != MG[phi[0].first][i].second)
            MG[phi[0].first].push_back({t_fin, MG[phi[0].first][i].second});
        if (t_fin != JG[phi[0].second][j].second)
            JG[phi[0].second].push_back({t_fin, JG[phi[0].second][j].second});
        MG[phi[0].first].erase(MG[phi[0].first].begin() + i);
        JG[phi[0].second].erase(JG[phi[0].second].begin() + j);

        //Remove the task from phi
        phi.erase(phi.begin());
    }

    Solution s = Solution(sol);
    s.set_chemin_critique(this->calcul_chemin_critique(s, inst));
    return s;
}

Solution Fitness::critic_engine(Instance inst)
{
    pair<int, int> vide = {-1, -1};

    //Initialize solution.
    vector<pair<pair<int, int>, int>> sol = {};

    //Initialize critical path.
    int longueur_chemin_critique = 0;
    pair<int, int> derniere_operation = {-1, -1};

    //Initialize EOM and EOJ to 0.
    vector<int> EOM = {};
    vector<pair<int, int>> derniere_operation_machine = {};
    for (int m = 0; m < inst.get_nbr_machine(); m++)
    {
        EOM.push_back(0);
        derniere_operation_machine.push_back(vide);
    }
    vector<int> EOJ = {};
    vector<pair<int, int>> derniere_operation_job = {};
    for (int j = 0; j < inst.get_nbr_job(); j++)
    {
        EOJ.push_back(0);
        derniere_operation_job.push_back(vide);
    }

    //initialize list of predecesor
    vector<vector<pair<int, int>>> predecesseur(inst.get_nbr_machine(), vector<pair<int, int>>(inst.get_nbr_job(), vide));

    //Initialize phi.
    vector<pair<int, int>> phi = inst.get_permutation();

    //While phi not empty.
    while (!phi.empty())
    {
        pair<int, int> tache = phi[0];
        int machine = tache.first;
        int job = tache.second;
        if (EOM[machine] > EOJ[job])
        {
            predecesseur[machine][job] = derniere_operation_machine[machine];
        }
        else
        {
            predecesseur[machine][job] = derniere_operation_job[job];
        }

        int t_fin = max(EOM[machine], EOJ[job]) + inst.get_time(machine, job);
        sol.push_back({tache, t_fin});

        EOM[machine] = t_fin;
        EOJ[job] = t_fin;
        derniere_operation_machine[machine] = tache;
        derniere_operation_job[job] = tache;
        if (t_fin > longueur_chemin_critique)
        {
            longueur_chemin_critique = t_fin;
            derniere_operation = tache;
        }

        phi.erase(phi.begin());
    }

    // Reconstruction du chemin critique

    vector<pair<int, int>> Chemin_critique = {derniere_operation}; // tableau contenant la liste des opérations du chemin critique à l’envers
    int taille_chemin = 1;

    pair<int, int> tache_precedente = predecesseur[derniere_operation.first][derniere_operation.second];

    while (tache_precedente != vide)
    {
        Chemin_critique.push_back(tache_precedente);
        tache_precedente = predecesseur[tache_precedente.first][tache_precedente.second];
    }

    Solution s = Solution(sol);
    s.set_chemin_critique(Chemin_critique);
    return s;
}

bool Fitness::est_dedans(int i, vector<int> list)
{
    for (unsigned int j = 0; j < list.size(); j++)
    {
        if (i == list[j])
            return true;
    }
    return false;
}

vector<pair<int, int>> Fitness::calcul_chemin_critique(Solution s, Instance inst)
{
    vector<pair<int, int>> Chemin_critique = {};
    int t = s.get_temps_total();
    while (t > 0)
    {
        int i = 0;
        bool flag = false;
        while (!flag)
        {
            if (s.get_temps()[i].second == t)
            {
                flag = true;
                pair<int, int> task = s.get_temps()[i].first;
                Chemin_critique.push_back(task);
                t -= inst.get_time(task.first, task.second);
            }
            else
            {
                i++;
            }
        }
    }
    return Chemin_critique;
}

string Fitness::comparaison_fitness(vector<Solution> a, vector<Solution> b)
{
    int s1 = a.size();
    int s2 = b.size();
    if (s1 != s2)
    {
        cout << "error Fitness::comparaison_fitness deux liste de taille différentes incomparable.";
        return "error";
    }
    for (int i = 0; i < s1; i++)
    {
        if (a[i].get_temps_total() > b[i].get_temps_total())
            return "greater";
        if (a[i].get_temps_total() < b[i].get_temps_total())
            return "lower";
    }
    return "equal";
}