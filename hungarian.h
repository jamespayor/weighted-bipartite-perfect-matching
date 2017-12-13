#ifndef HUNGARIAN_H
#define HUNGARIAN_H

#include <vector>
#include <utility>

struct WeightedBipartiteEdge {
    const int left;
    const int right;
    const int cost;

    WeightedBipartiteEdge() : left(), right(), cost() {}
    WeightedBipartiteEdge(int left, int right, int cost) : left(left), right(right), cost(cost) {}
};

const std::vector<int> hungarianMinimumWeightPerfectMatching(int n, const std::vector<WeightedBipartiteEdge> allEdges);


#endif // HUNGARIAN_H
