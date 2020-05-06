#include <vector>
#include <algorithm>
#include <iostream>
#include "Solution.h"

using namespace std;

Solution::Solution(vector<pair<pair<int, int>, int>> temps_fin, vector<int> run)
{
    this->temps_fin = temps_fin;
    this->run = run;
    this->chemin_critique_inscrit = false;
}

Solution::Solution(vector<pair<pair<int, int>, int>> temps_fin)
{
    this->temps_fin = temps_fin;
    this->run = {};
    this->chemin_critique_inscrit = false;
}

int Solution::get_temps_total()
{
    int temps_total = this->temps_fin[0].second;
    for (unsigned int i = 1; i < this->temps_fin.size(); i++)
    {
        temps_total = max(temps_total, this->temps_fin[i].second);
    }
    return temps_total;
}

vector<pair<pair<int, int>, int>> Solution::get_temps()
{
    return this->temps_fin;
}

vector<int> Solution::get_run()
{
    return this->run;
}

void Solution::set_run(vector<int> run)
{
    this->run = run;
}

void Solution::afficher_temps()
{
    for (unsigned int i = 0; i < this->temps_fin.size(); i++)
        cout << this->temps_fin[i].first.first << "," << this->temps_fin[i].first.second << ":" << this->temps_fin[i].second << " / ";
    cout << endl;
}

void Solution::set_chemin_critique(vector<pair<int,int>> nouveau_chemin_critique)
{
    this->chemin_critique_inscrit = true;
    this->chemin_critique = nouveau_chemin_critique;
}

bool Solution::est_dans_chemin_critique(pair<int,int> tache)
{
    if(this->chemin_critique_inscrit)
    {
        for(unsigned int i = 0; i < this->chemin_critique.size(); i++)
        {
            if(this->chemin_critique[i]==tache)
                return true;
        }
    }
    return false;
}