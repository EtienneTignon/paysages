#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <iterator>
#include "Instance.h"

using namespace std;

Instance::Instance(int _nbr_job, int _nbr_machine, vector<vector<int>> _matrice_tache, bool _opt_glob_connu, int _opt_glob, string nom)
{
    this->nom = nom;
    this->nbr_job = _nbr_job;
    this->nbr_machine = _nbr_machine;
    this->opt_glob_connu = _opt_glob_connu;
    this->matrice_tache = _matrice_tache;
    if (_opt_glob_connu)
        this->opt_glob = _opt_glob;
    else
    {
        this->opt_glob = 0;
    }

    //Calcul de l'écart-type
    this->ecart_type = this->calcul_ecart_type(_matrice_tache);
    //Fabrication de la permutation de base
    this->initialisation_permutation();
    //calcul de la borne inférieur
    this->calcul_l_bound();
}

Instance::Instance(string nom_de_fichier)
{
    this->nom = nom_de_fichier;
    string filepath = "Instances/" + nom_de_fichier + ".txt";
    cout << filepath << endl;
    ifstream fichier(filepath);
    fichier >> this->nbr_machine;
    fichier >> this->nbr_job;
    this->matrice_tache = {};
    for(int m = 0; m < this->nbr_machine; m++)
    {
        this->matrice_tache.push_back({});
        for(int j = 0; j < this->nbr_job; j++)
        {
            int t;
            fichier >> t;
            this->matrice_tache[m].push_back(t);
        }
    }

    int o_glob;
    fichier >> o_glob;
    if(o_glob < 0)
    {
        this->opt_glob_connu = false;
        this->opt_glob = 0;
    }else
    {
        this->opt_glob_connu = true;
        this->opt_glob = o_glob;
    }

    //Calcul de l'écart-type
    this->ecart_type = this->calcul_ecart_type(this->matrice_tache);
    //Fabrication de la permutation de base
    this->initialisation_permutation();
    //calcul de la borne inférieur
    this->calcul_l_bound();
    
}

float Instance::calcul_ecart_type(vector<vector<int>> matrice)
{
    int sum = 0;
    int taille = this->nbr_job * this->nbr_machine;
    for (int j = 0; j < this->nbr_job; j++)
    {
        for (int m = 0; m < this->nbr_machine; m++)
        {
            sum += matrice[m][j];
        }
    }
    float moyenne = sum / (taille);
    float var = 0;
    for (int j = 0; j < this->nbr_job; j++)
    {
        for (int m = 0; m < this->nbr_machine; m++)
        {
            var += pow(matrice[m][j], 2);
        }
    }
    var = var / taille;
    var = var - pow(moyenne, 2);
    float ecart = sqrt(var);

    return ecart;
}

float Instance::get_ecart_type()
{
    return this->ecart_type;
}

void Instance::initialisation_permutation()
{
    this->permutation = {};
    for (int j = 0; j < this->nbr_job; j++)
    {
        for (int m = 0; m < this->nbr_machine; m++)
        {
            this->permutation.push_back(make_pair(m, j));
        }
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(begin(this->permutation), end(this->permutation), g);
}
void Instance::swap_perm(int indice_1, int indice_2)
{
    iter_swap(this->permutation.begin() + indice_1, this->permutation.begin() + indice_2);
}

int Instance::get_nbr_job()
{
    return this->nbr_job;
}

int Instance::get_nbr_machine()
{
    return this->nbr_machine;
}

vector<pair<int, int>> Instance::get_permutation()
{
    return this->permutation;
}

int Instance::get_time(int machine, int job)
{
    return this->matrice_tache[machine][job];
}

int Instance::get_l_bound()
{
    return this->l_bound;
}

int Instance::get_opt_glob()
{
    return this->opt_glob;
}

void Instance::show_permutation()
{
    cout << "Permutation";
    for (unsigned int i = 0; i < this->permutation.size(); i++)
    {
        cout << " : " << this->permutation[i].first << "-" << this->permutation[i].second;
    }
    cout << endl;
}

void Instance::show_schedule(Solution s)
{
    cout << "Schedule" << endl;
    for (unsigned int i = 0; i < s.get_temps().size(); i++)
    {
        int machine = s.get_temps()[i].first.first;
        int job = s.get_temps()[i].first.second;
        int h_debut = s.get_temps()[i].second - (this->matrice_tache[machine][job]);
        cout << "(m" << machine << "-j" << job << ") : [" << h_debut << "-" << s.get_temps()[i].second << "]" << endl;
    }
}

void Instance::calcul_l_bound()
{
    vector<int> machine_bound = {};
    vector<int> job_bound = {};

    for (int m = 0; m < this->nbr_machine; m++)
        machine_bound.push_back(0);
    for (int j = 0; j < this->nbr_job; j++)
        job_bound.push_back(0);

    for (int m = 0; m < this->nbr_machine; m++)
    {
        for (int j = 0; j < this->nbr_job; j++)
        {
            machine_bound[m] += this->matrice_tache[m][j];
            job_bound[j] += this->matrice_tache[m][j];
        }
    }

    this->l_bound = 0;
    for (int m = 0; m < this->nbr_machine; m++)
        this->l_bound = max(this->l_bound, machine_bound[m]);
    for (int j = 0; j < this->nbr_job; j++)
        this->l_bound = max(this->l_bound, job_bound[j]);
}

bool Instance::opt_glob_known()
{
    return this->opt_glob_connu;
}

bool Instance::is_opt_glob(int result)
{
    return ((this->opt_glob == result) && this->opt_glob_connu);
}

bool Instance::is_lower_bound(int result)
{
    return (this->l_bound == result);
}

void Instance::set_permutation(vector<pair<int, int>> perm)
{
    this->permutation = perm;
}

string Instance::get_nom()
{
    return this->nom;
}