Fast C++ implementation of the Hungarian algorithm
--------------------------------------------------

This is an open-source implementation of the O(N^3) dynamic-programming version of the
Hungarian algorithm, for weighted perfect bipartite matching.
It's written with speed in mind, whilst trying to remain readable.

When limited to M edges, I'd expect this to run closer to O(NM), although I haven't verified
this with analysis.

The best description of the algorithm I'm aware of is
[this TopCoder post](https://www.topcoder.com/community/data-science/data-science-tutorials/assignment-problem-and-hungarian-algorithm/),
which I highly recommend should you attempt to understand what's going on.


### Usage

Choose your `n` (the number of nodes on each side of the bipartite graph),
build a `vector` of `WeightedBipartiteEdge`s, then call `hungarianMinimumWeightPerfectMatching(n, edges)`.

You'll receive back either an empty `vector` (indicating that no perfect matching is possible),
or a `vector` of size `n` giving the matching.

If `matching` is the result, then `matching[i]` gives the node on the right that the left node is matched to.


### Sketch of what the algorithm does

Seriously consider reading the TopCoder post. It might help you be slightly less confused.

##### What are node "potentials" even?

The underlying theory is best understood viewing the node potentials in the algorithm
as _pricing functions_, as seen in min-cost max-flow. I'll focus on what goes on in the Hungarian algorithm though.

First, note that adding _X_ to the cost of all edges for a node on the left doesn't affect
the final matching. It just increases the cost of every possible matching by _X_.
Similarly, shifting the cost of all edges to a node on the right just changes the cost of every matching.

We've simply called "the amount we decrease the cost of edges connected to node _i_" the _potential_ of node _i_.
As we build a matching, we can freely move these potentials up and down, adjusting the edge costs,
without changing the result. 

##### Why is it useful to adjust costs in this way?

Let's call the costs adjusted by potential the _reduced costs_.
The algorithm likes to proceed by maintaining a property:

- In the residual graph (in which edges in the matching go from right to left), there is no cycle with _negative reduced cost_. 

We maintain this property from the beginning to the end.
If at the end we have a perfect matching (i.e. a flow) with no negative residual cycles,
then it's impossible to improve the matching (i.e. the flow is a min-cost flow).

(To see this, suppose we have a matching that costs more than optimal.
Treat this as a flow, and subtract away any optimal flow.
The difference must be a _circulation_, i.e. a set of cycles.
One of these cycles must be negative-cost in our matching, otherwise it would have the same cost as the optimal one.)  

##### What the algorithm does.

So, at the beginning, we have an empty matching, which has no residual cycles.
We maintain always that edges have positive reduced cost. Then, we iterate:

- Adjust node potentials so that some of the edges have _zero_ reduced cost.
- Attempt to augment our matching only using these zero-cost edges.

The algorithm can keep adding at least one new edge to the section of the graph it's exploring
(with edges that have zero reduced cost), and so it must inexorably succeed.

##### Caveats

I've probably gotten something wrong in the above explanation, because linear programming isn't my thing,
and I don't actually understand the algorithm well enough to have derived it. I hope this helps someone though!