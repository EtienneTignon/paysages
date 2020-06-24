#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED
#include <vector>

using namespace std;

class Solution
{

private:
    vector<pair<pair<int, int>, int>> temps_fin;
    vector<int> run;
    bool chemin_critique_inscrit;
    vector<pair<int,int>> chemin_critique;
    float temps_total;
public:
    Solution(vector<pair<pair<int, int>, int>> temps, vector<int> run);
    Solution(vector<pair<pair<int, int>, int>> temps);
    int get_temps_total();
    vector<pair<pair<int, int>, int>> get_temps();
    vector<int> get_run();
    void set_run(vector<int> run);
    void afficher_temps();
    void set_chemin_critique(vector<pair<int,int>> nouveau_chemin_critique);
    bool est_dans_chemin_critique(pair<int,int> tache);
};

#endif // SOLUTION_H_INCLUDED