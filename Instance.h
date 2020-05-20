#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED
#include <string>
#include <vector>
#include "Solution.h"

using namespace std;

class Instance
{

private:
    int nbr_job;
    int nbr_machine;
    vector<vector<int>> matrice_tache;
    bool opt_glob_connu;
    int opt_glob;
    float ecart_type;
    vector<pair<int, int>> permutation;
    int l_bound;
    string nom;

    void calcul_l_bound();
    float calcul_ecart_type(vector<vector<int>> matrice);

public:
    Instance(int _nbr_job, int _nbr_machine, vector<vector<int>> matrice_tache, bool _opt_glob_connu, int _opt_glob, string nom);
    Instance(string nom_de_fichier);
    void initialisation_permutation();
    float get_ecart_type();
    int get_nbr_job();
    int get_nbr_machine();
    int get_time(int machine, int job);
    int get_l_bound();
    int get_opt_glob();
    vector<pair<int, int>> get_permutation();
    void swap_perm(int indice_1, int indice_2);
    void show_permutation();
    void show_schedule(Solution s);
    bool opt_glob_known();
    bool is_opt_glob(int result);
    bool is_lower_bound(int result);
    void set_permutation(vector<pair<int, int>> perm);
    string get_nom();
    void recreer_instance_depuis_solution(Solution s);
};

#endif // INSTANCE_H_INCLUDED