#ifndef HUNGARIAN_H
#define HUNGARIAN_H

#include <vector>

struct WeightedBipartiteEdge {
    const int left;
    const int right;
    const int cost;

    WeightedBipartiteEdge() : left(), right(), cost() {}
    WeightedBipartiteEdge(const int left, const int right, const int cost) : left(left), right(right), cost(cost) {}
};

const std::vector<int> hungarianMinimumWeightPerfectMatching(int n, const std::vector<WeightedBipartiteEdge> allEdges);


#endif // HUNGARIAN_H
