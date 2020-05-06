#include <algorithm>
#include <iterator>
#include <time.h>
#include <iostream>
#include <random>
#include <vector>
#include "Voisinage.h"
#include "Solution.h"

using namespace std;

Voisinage::Voisinage(string voisinage, Instance inst)
{
    this->voisinage_choisi = voisinage;
    if (voisinage.find("swap") != std::string::npos)
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin - 1; i++)
        {
            for (unsigned int j = i + 1; j < nbr_voisin; j++)
            {
                this->permutation_swap.push_back({i, j});
            }
        }
    }
    if (voisinage.find("insert") != std::string::npos)
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin; i++)
        {
            for (unsigned int j = 0; j < nbr_voisin; j++)
            {
                if (i != j)
                    this->permutation_trans.push_back({i, j});
            }
        }
    }
    this->Reinitialiser_voisin();
}

Voisinage::Voisinage(string voisinage, Instance inst, Solution s)
{
    this->voisinage_choisi = voisinage;
    if (voisinage.find("swap") != std::string::npos)
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin - 1; i++)
        {
            for (unsigned int j = i + 1; j < nbr_voisin; j++)
            {
                bool i_est_critique = s.est_dans_chemin_critique(inst.get_permutation()[i]);
                bool j_est_critique = s.est_dans_chemin_critique(inst.get_permutation()[j]);
                if (i_est_critique && j_est_critique)
                    this->permutation_swap.push_back({i, j});
                else if(i_est_critique || j_est_critique)
                {
                    int k = i+1;
                    bool k_est_critique = false;
                    while(k<j && !k_est_critique)
                    {
                        k_est_critique = s.est_dans_chemin_critique(inst.get_permutation()[k]);
                        if(k_est_critique)
                        {
                            this->permutation_swap.push_back({i, j});
                        }
                        else
                        {
                            k++;
                        }
                        
                    }
                }
            }
        }
    }
    if (voisinage.find("insert") != std::string::npos)
    {
        unsigned int nbr_voisin = inst.get_permutation().size();
        for (unsigned int i = 0; i < nbr_voisin; i++)
        {
            for (unsigned int j = 0; j < nbr_voisin; j++)
            {
                if (i != j)
                {
                    bool accepter = false;
                    bool flag = false;
                    int a = min(i,j);
                    while (!flag)
                    {
                        if(a != i)
                        {
                            if(a > max(i,j))
                            {
                                flag = true;
                            }
                            else if((s.est_dans_chemin_critique(inst.get_permutation()[i]) && s.est_dans_chemin_critique(inst.get_permutation()[a])))
                            {
                                flag = true;
                                accepter = true;
                            }
                        }
                        a++;
                    }
                    if(accepter)
                        this->permutation_trans.push_back({i, j});
                }
            }
        }
    }
    this->Reinitialiser_voisin();
}

vector<pair<int, int>> Voisinage::swap(vector<pair<int, int>> permutation)
{
    int indice1 = this->permutation_swap[this->prochain_voisin_swap].first;
    int indice2 = this->permutation_swap[this->prochain_voisin_swap].second;
    vector<pair<int, int>> perm = permutation;
    iter_swap(perm.begin() + indice1, perm.begin() + indice2);
    this->prochain_voisin_swap++;
    return perm;
}

vector<pair<int, int>> Voisinage::trans(vector<pair<int, int>> permutation)
{
    int indice1 = this->permutation_trans[this->prochain_voisin_trans].first;
    int indice2 = this->permutation_trans[this->prochain_voisin_trans].second;
    vector<pair<int, int>> perm = permutation;
    if (indice1 < indice2)
    {
        for (int dif = 0; dif < indice2 - indice1; dif++)
        {
            int sw1 = indice1 + dif;
            int sw2 = indice1 + dif + 1;
            iter_swap(perm.begin() + sw1, perm.begin() + sw2);
        }
    }
    else
    {
        for (int dif = 0; dif < indice1 - indice2; dif++)
        {
            int sw1 = indice1 - dif;
            int sw2 = (indice1 - dif) - 1;
            iter_swap(perm.begin() + sw1, perm.begin() + sw2);
        }
    }
    this->prochain_voisin_trans++;
    return perm;
}

vector<pair<int, int>> Voisinage::Creer_voisin(vector<pair<int, int>> permutation)
{
    srand(time(NULL));
    int nbr_voisin = 0;
    int nbr_voisin_swap = 0;
    int nbr_voisin_trans = 0;
    if (this->voisinage_choisi.find("swap") != std::string::npos)
    {
        nbr_voisin_swap = this->permutation_swap.size() - this->prochain_voisin_swap;
        nbr_voisin += nbr_voisin_swap;
    }
    if (this->voisinage_choisi.find("insert") != std::string::npos)
    {

        nbr_voisin_trans = this->permutation_trans.size() - this->prochain_voisin_trans;
        nbr_voisin += nbr_voisin_trans;
    }
    int voisin_choisi = (rand() % nbr_voisin) + 1;
    if (this->voisinage_choisi.find("swap") != std::string::npos)
    {
        voisin_choisi -= nbr_voisin_swap;
        if (voisin_choisi <= 0)
            return this->swap(permutation);
    }
    if (this->voisinage_choisi.find("insert") != std::string::npos)
    {
        voisin_choisi -= nbr_voisin_trans;
        if (voisin_choisi <= 0)
            return this->trans(permutation);
    }
    cout << "ERROR Voisinage::Creer_voisin" << endl;
    return {};
}

bool Voisinage::Voisinage_parcouru()
{
    if (this->voisinage_choisi.find("swap") != std::string::npos)
    {
        if (this->prochain_voisin_swap < this->permutation_swap.size())
            return false;
    }
    if (this->voisinage_choisi.find("insert") != std::string::npos)
    {
        if (this->prochain_voisin_trans < this->permutation_trans.size())
            return false;
    }
    return true;
}

void Voisinage::Reinitialiser_voisin()
{
    mt19937 g(this->rd());
    shuffle(begin(this->permutation_swap), end(this->permutation_swap), g);
    shuffle(begin(this->permutation_trans), end(this->permutation_trans), g);
    this->prochain_voisin_swap = 0;
    this->prochain_voisin_trans = 0;
}