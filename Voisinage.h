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
    vector<pair<int, int>> permutation_swap;
    int prochain_voisin_swap;
    vector<pair<int, int>> swap(vector<pair<int, int>> permutation);
    vector<pair<int, int>> permutation_trans;
    int prochain_voisin_trans;
    string voisinage_choisi;
    vector<pair<int, int>> trans(vector<pair<int, int>> permutation);
    random_device rd;

public:
    Voisinage(string voisinage, Instance inst);
    Voisinage(string voisinage, Instance inst, Solution s);
    vector<pair<int, int>> Creer_voisin(vector<pair<int, int>> permutation);
    bool Voisinage_parcouru();
    void Reinitialiser_voisin();
};

#endif // VOISINAGE_H_INCLUDED