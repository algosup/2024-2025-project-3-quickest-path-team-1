// ✅ file verified.
#include "incl.h"
#include "decl.h"
#include "search.h"

// ✅ function + comment verified.
/**
 * @brief Implements a unidirectional A* shortest path search algorithm.
 *
 * @detailed
 * This function finds the shortest path between `start_node` and `end_node` in the given graph
 * using a standard unidirectional A* search approach. It leverages a priority queue to explore nodes
 * based on their estimated total cost (the sum of the cost from the start node and a heuristic estimate
 * to the target node). The heuristic weight (`weight`) allows for tuning between optimality and speed:
 * - A weight of `1.0` guarantees the shortest path.
 * - Weights greater than `1.0` allow for faster computations at the potential expense of optimality.
 *
 * Functionality:
 * - Initializes search buffers with lazy resets using a search ID system to avoid full buffer reinitializations.
 * - Uses a priority queue (`open_list`) to select the most promising node to expand based on its f-cost.
 * - Expands nodes by relaxing edges and updating cost estimates if a better path is found.
 * - Terminates once the target node is expanded, reconstructing the path by tracing back through parent pointers.
 *
 * Optimizations:
 * - Avoids revisiting already closed nodes using a `closed` vector.
 * - Computes and caches heuristic values to reduce redundant calculations.
 * - Uses lazy updates in the search buffers for efficient memory management.
 *
 * @param gdata Reference to the graph data.
 * @param buffers Reference to `search_buffers` for optimized memory management.
 * @param conf Configuration settings for heuristic selection.
 * @param start_node The starting node.
 * @param end_node The target node.
 * @param weight Heuristic weight influencing path selection
 *               (`1.0` = guaranteed shortest path, `>1.0` = faster but possibly suboptimal).
 * @return A `path_result` structure containing the shortest path details.
 *
 * @complexity
 * - Time Complexity: Approximately O(E log V), where E is the number of edges and V is the number of vertices.
 * - Space Complexity: O(V) for storing distances, parents, and other search-related data.
 */
path_result findShortestPathUnidirectional(const graph& gdata, search_buffers& buffers, const config& conf, int start_node, int end_node, double weight)
{
    if (start_node == end_node) {
        return { 0, 0, {} };
    }

    auto it_start = gdata.node_to_index.find(start_node);
    auto it_end = gdata.node_to_index.find(end_node);
    if (it_start == gdata.node_to_index.end() || it_end == gdata.node_to_index.end()) {
        return { -1, 0, {} };
    }

    size_t start_idx = it_start->second;
    size_t end_idx = it_end->second;
    const int INF = std::numeric_limits<int>::max();

    buffers.current_search_id++;
    setDistFromStart(buffers, start_idx, 0);
    setDistFromEnd(buffers, end_idx, INF);

    using pq_item = std::pair<double, size_t>;
    auto cmp = [](const pq_item &a, const pq_item &b) {
        return a.first > b.first;
    };
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> open_list(cmp);

    int h_start = computeHeuristic(start_idx, end_idx, gdata, conf);
    setHForward(buffers, start_idx, h_start);
    double f_start = (double)getDistFromStart(buffers, start_idx) + weight * (double)h_start;
    open_list.push({ f_start, start_idx });

    const auto& offsets = gdata.offsets;
    const auto& edges = gdata.edges;
    size_t node_count = gdata.index_to_node.size();

    std::vector<bool> closed(node_count, false);

    while (!open_list.empty())
    {
        auto top_item = open_list.top();
        open_list.pop();

        int cur_idx = (int)top_item.second;
        if (closed[cur_idx]) {
            continue;
        }
        closed[cur_idx] = true;

        if (cur_idx == (int)end_idx)
        {
            std::vector<int> path;
            path.reserve(64); 
            int cur = end_idx;
            while (cur != (int)start_idx)
            {
                path.push_back(gdata.index_to_node[cur]);
                cur = getParentForward(buffers, cur).first;
            }
            path.push_back(gdata.index_to_node[start_idx]);
            std::reverse(path.begin(), path.end());
            int final_cost = getDistFromStart(buffers, end_idx);
            return { final_cost, (int)path.size(), path };
        }

        int cur_cost = getDistFromStart(buffers, cur_idx);
        size_t start_edge = offsets[cur_idx];
        size_t end_edge = ((static_cast<size_t>(cur_idx) + 1) < gdata.offsets.size()) ? gdata.offsets[static_cast<size_t>(cur_idx) + 1] : gdata.edges.size();

        for (size_t i = start_edge; i < end_edge; i++)
        {
            int nbr_idx = edges[i].target;
            if (closed[nbr_idx]) {
                continue;
            }
            int edge_cost = edges[i].weight;
            int new_cost = cur_cost + edge_cost;
            int old_cost = getDistFromStart(buffers, nbr_idx);

            if (old_cost < 0 || new_cost < old_cost)
            {
                setDistFromStart(buffers, nbr_idx, new_cost);
                setParentForward(buffers, nbr_idx, { cur_idx, edge_cost });

                int h_val = getHForward(buffers, nbr_idx);
                if (h_val < 0)
                {
                    h_val = computeHeuristic(nbr_idx, end_idx, gdata, conf);
                    setHForward(buffers, nbr_idx, h_val);
                }
                double f_cost = (double)new_cost + weight * (double)h_val;
                open_list.push({ f_cost, (size_t)nbr_idx });
            }
        }
    }
    return { -1, 0, {} };
}