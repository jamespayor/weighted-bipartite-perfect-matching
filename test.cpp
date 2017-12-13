#include "stdlib.h"
#include <iostream>
#include <vector>

#include "hungarian.h"

const std::pair<int, std::vector<int> > bruteForceInternal(const int n, const std::vector<WeightedBipartiteEdge> edges, std::vector<bool>& leftMatched, std::vector<bool>& rightMatched, const int edgeUpTo = 0, const int matchCount = 0) {
	if (matchCount == n) {
		return std::make_pair(0, std::vector<int>());
	}

	int bestCost = 1 << 20;
	std::vector<int> bestEdges;
	for (int edgeIndex = edgeUpTo; edgeIndex < edges.size(); ++edgeIndex) {
		const WeightedBipartiteEdge& edge = edges[edgeIndex];
		if (!leftMatched[edge.left] && !rightMatched[edge.right]) {
			leftMatched[edge.left] = true;
			rightMatched[edge.right] = true;
			std::pair<int, std::vector<int> > remainder = bruteForceInternal(n, edges, leftMatched, rightMatched, edgeIndex + 1, matchCount + 1);
			leftMatched[edge.left] = false;
			rightMatched[edge.right] = false;

			if (remainder.first + edge.cost < bestCost) {
				bestCost = remainder.first + edge.cost;
				bestEdges = remainder.second;
				bestEdges.push_back(edgeIndex);
			}
		}
	}

	return std::make_pair(bestCost, bestEdges);
}

const std::vector<int> bruteForce(const int n, const std::vector<WeightedBipartiteEdge> edges) {
	std::vector<bool> leftMatched(n), rightMatched(n);
	std::vector<int> edgeIndices = bruteForceInternal(n, edges, leftMatched, rightMatched).second;
	std::vector<int> matching(n);
	for (int i = 0; i < edgeIndices.size(); ++i) {
		const WeightedBipartiteEdge& edge = edges[edgeIndices[i]];
		matching[edge.left] = edge.right;
	}
	return matching;
}

int main() {
	int n = 10;
	std::vector<WeightedBipartiteEdge> edges;
	for (int i = 0; i < n; ++i) {
		for (int j = i - 2; j < i + 2; ++j) {
			edges.push_back(WeightedBipartiteEdge(i, (j + n) % n, rand() % 100));
		}
	}

	std::vector<int> groundTruthMatching = bruteForce(n, edges);
	for (int i = 0; i < n; ++i) {
		std::cout << i << " -> " << groundTruthMatching[i] << '\n';
	}
	std::cout << std::endl;

	std::vector<int> matching = hungarianMinimumWeightPerfectMatching(n, edges);
	if (matching == groundTruthMatching) {
		std::cout << "(Hungarian algorithm output matches brute force.)" << std::endl;
	} else {
		std::cout << std::endl << "Hungarian algorithm output doesn't agree with brute force:\n";
		if (matching.empty()) {
			std::cout << "=> No matching was found.\n";
		} else {
			for (int i = 0; i < n; ++i) {
				std::cout << i << " -> " << matching[i] << '\n';
			}
		}
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}
