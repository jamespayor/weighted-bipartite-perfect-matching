
#include <algorithm>
#include <vector>
#include <deque>
#include <limits>

#include "hungarian.h"


// Macros!
//
// fo(i, n):       foreach i in [0, n)
// range(i, a, b): foreach i in [a, b)
//
// (Note: end value is cached, so fo(i, function()) will only have function called once.)

#define fo(i, n) for(int i = 0, _n = (n); i < _n; ++i)
#define range(i, a, b) for(int i = (a), _n = (b); i < _n; ++i)

static const int oo = std::numeric_limits<int>::max();
static const int UNMATCHED = -1;

struct LeftEdge {
	const int right;
	const int cost;

	LeftEdge(int right, int cost) : right(right), cost(cost) {}

	const bool operator < (const LeftEdge& otherEdge) {
		return right < otherEdge.right || (right == otherEdge.right && cost < otherEdge.cost);
	}
};

const std::vector<int> hungarianMinimumWeightPerfectMatching(const int n, const std::vector<WeightedBipartiteEdge> allEdges) {

    // Edge lists for each left node.
	std::vector<LeftEdge> *leftEdges = new std::vector<LeftEdge>[n];

    //region Edge list initialization

	// Initialize edge lists for each left node, based on the incoming set of edges.
    // While we're at it, we check that every node has at least one associated edge.
	{
		int leftEdgeCounts[n];
        int rightEdgeCounts[n];
		fo(edgeIndex, allEdges.size()) {
			const WeightedBipartiteEdge& edge = allEdges[edgeIndex];
			++leftEdgeCounts[edge.left];
            ++rightEdgeCounts[edge.right];
		}

        fo(i, n) {
            if (leftEdgeCounts[i] == 0 || rightEdgeCounts[i] == 0) {
                // No matching will be possible.
                return std::vector<int>();
            }
        }

        // Probably unnecessary, but reserve the required space for each node, just because?
		fo(i, n) {
			leftEdges[i].reserve(leftEdgeCounts[i]);
		}
	}
	// Actually add to the edge lists now.
	fo(edgeIndex, allEdges.size()) {
		const WeightedBipartiteEdge& edge = allEdges[edgeIndex];
		leftEdges[edge.left].push_back(LeftEdge(edge.right, edge.cost));
	}

	// Sort the edge lists, and remove duplicate edges (keep the edge with smallest cost).
	fo(i, n) {
		std::vector<LeftEdge>& edges = leftEdges[i];
		std::sort(edges.begin(), edges.end());

		int edgeCount = 0;
        int lastRight = -1;
        fo(edgeIndex, edges.size()) {
            const LeftEdge& edge = edges[edgeIndex];
            if (edge.right == lastRight) {
                continue;
            }
            if (edgeIndex != edgeCount) {
                edges[edgeCount] = edge;
            }
            ++edgeCount;
        }

        edges.resize(edgeCount);
	}

    //endregion Edge list initialization

    // These hold "potentials" for nodes on the left and nodes on the right, which reduce the costs of attached edges.
    // We maintain that every reduced cost, cost[i][j] - leftPotential[i] - leftPotential[j], is greater than zero.
    int leftPotential[n], rightPotential[n];

    //region Node potential initialization

    // Here, we seek good initial values for the node potentials.
    // Note: We're guaranteed by the above code that at every node on the left and right has at least one edge.

    // First, we raise the potentials on the left as high as we can for each node.
    // This guarantees each node on the left has at least one "tight" edge.

    fo(i, n) {
        const std::vector<LeftEdge>& edges = leftEdges[i];
        int smallestEdgeCost = edges[0].cost;
        range(edgeIndex, 1, edges.size()) {
            if (edges[edgeIndex].cost < smallestEdgeCost) {
                smallestEdgeCost = edges[edgeIndex].cost;
            }
        }

        // Set node potential to the smallest incident edge cost.
        // This is as high as we can take it without creating an edge with zero reduced cost.
        leftPotential[i] = smallestEdgeCost;
    }

    // Second, we raise the potentials on the right as high as we can for each node.
    // We do the same as with the left, but this time take into account that costs are reduced
    // by the left potentials.
    // This guarantees that each node on the right has at least one "tight" edge.

    fo(j, n) {
        rightPotential[j] = oo;
    }

    fo(edgeIndex, allEdges.size()) {
        const WeightedBipartiteEdge& edge = allEdges[edgeIndex];
        int reducedCost = edge.cost - leftPotential[edge.left];
        if (rightPotential[edge.right] > reducedCost) {
            rightPotential[edge.right] = reducedCost;
        }
    }

    //endregion Node potential initialization

    // Tracks how many edges for each left node are "tight".
    // Following initialization, we maintain the invariant that these are at the start of the node's edge list.
    int leftTightEdgesCount[n];

    //region Tight edge initialization

    // Here we find all tight edges, defined as edges that have zero reduced cost.
    // We will be interested in the subgraph induced by the tight edges, so we partition the edge lists for
    // each left node accordingly, moving the tight edges to the start.

    fo(i, n) {
        std::vector<LeftEdge>& edges = leftEdges[i];
        int tightEdgeCount = 0;
        fo(edgeIndex, edges.size()) {
            const LeftEdge& edge = edges[edgeIndex];
            int reducedCost = edge.cost - leftPotential[i] - rightPotential[edge.right];
            if (reducedCost == 0) {
                if (edgeIndex != tightEdgeCount) {
                    std::swap(edges[tightEdgeCount], edges[edgeIndex]);
                }
                ++tightEdgeCount;
            }
        }
        leftTightEdgesCount[i] = tightEdgeCount;
    }

    //endregion Tight edge initialization


    // Now we're ready to begin the inner loop.

    // We maintain an (initially empty) partial matching, in the subgraph of tight edges.
    int leftMatchedTo[n];
    int rightMatchedTo[n];
    std::fill(leftMatchedTo, leftMatchedTo + n, UNMATCHED);
    std::fill(rightMatchedTo, rightMatchedTo + n, UNMATCHED);

    // While an augmenting path exists, we add it to the matching.
    // When an augmenting path doesn't exist, we update the potentials so that an edge between the area
    // we can reach and the unreachable nodes on the right becomes tight, giving us another edge to explore.
    //
    // We proceed in this fashion until we can't find more augmenting paths or add edges.
    // At that point, we either have a min-weight perfect matching, or no matching exists.



    int rightSlack[n];

    // Now we're ready to begin the main algorithm.
    // We


	return std::vector<int>();
}

