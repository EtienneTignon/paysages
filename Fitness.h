#ifndef FITNESS_H_INCLUDED
#define FITNESS_H_INCLUDED
#include <string>
#include "Solution.h"
#include "Instance.h"

using namespace std;

class Fitness
{
private:
    Solution nd_engine(Instance inst);
    Solution giffler_engine(Instance inst);
    Solution fast_engine(Instance inst);
    Solution fifo_engine(Instance inst);
    Solution critic_engine(Instance inst);
    bool est_dedans(int i, vector<int> list);
public:
    Solution calcul_solution(Instance inst, string methode);
    vector<pair<int, int>> calcul_chemin_critique(Solution s, Instance inst);
};

#endif // FITNESS_H_INCLUDED