#ifndef CALC_FEATURE_H_INCLUDED
#define CALC_FEATURE_H_INCLUDED
#include <vector>
#include "Instance.h"
#include "Solution.h"

class Calc_feature
{
public:
    static vector<int> Random_walk(Instance inst, string fitness, string voisinage, int nbr_iteration);
    static vector<int> Random_walk_without_neutral_neighbour(Instance inst, string fitness, string voisinage, int nbr_iteration);
    static vector<int> Random_walk_with_neutral_shortcut(Instance inst, string fitness, string voisinage, int nbr_iteration);
    static Solution Hill_climbing(Instance inst, string fitness, string voisinage, bool critic);
    static Solution Hill_climbing_with_neutral(Instance inst, string fitness, string voisinage, int budget, bool critic);
    static Solution Hill_climbing_and_best(Instance inst, string fitness, string voisinage, bool critic);
    static Solution Hill_climbing_with_neutral_and_best(Instance inst, string fitness, string voisinage, int budget, bool critic);
    static Solution Hill_climbing_with_budget(Instance inst, string fitness, string voisinage, int budget, bool critic);
    static Solution Descente_ILS(Instance inst, string fitness, string voisinage, int budget, int ln_saut, bool critic);
    static float rugosite(Instance inst, string voisinage, string fitness, string rugosite);
    static vector<pair<int,float>> courbe_de_rugosite(Instance inst, string voisinage, string fitness, string rugosite);
    static float taux_neutre(Instance inst, string voisinage, string fitness);
};

#endif // CALC_FEATURE_H_INCLUDED