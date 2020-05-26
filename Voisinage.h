#ifndef VOISINAGE_H_INCLUDED
#define VOISINAGE_H_INCLUDED
#include <vector>
#include <string>
#include "Instance.h"
#include "Solution.h"

using namespace std;

class Voisinage
{
private:
    Voisinage(string voisinage, Instance inst);
    vector<pair<int, int>> permutation_swap;
    int prochain_voisin_swap;
    vector<pair<int, int>> swap(vector<pair<int, int>> permutation);
    vector<pair<int, int>> permutation_trans;
    int prochain_voisin_trans;
    string voisinage_choisi;
    vector<pair<int, int>> trans(vector<pair<int, int>> permutation);
    random_device rd;
    bool changement(int i,int j,Instance inst);
    bool changement_insert(int i,int j,Instance inst);

public:
    Voisinage(string voisinage, Instance inst, Solution s);
    Voisinage(string voisinage, Instance inst, bool garantir_change);
    vector<pair<int, int>> Creer_voisin(vector<pair<int, int>> permutation);
    bool Voisinage_parcouru();
    void Reinitialiser_voisin();
};

#endif // VOISINAGE_H_INCLUDED