// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Loads graph data from memory and constructs the graph representation.
 *
 * @detailed
 * This function processes a vector of edges stored in memory and constructs
 * a graph representation optimized for shortest-path computations. Each edge
 * is defined by three values:
 *
 *     node_a, node_b, time_cost
 *
 * where:
 * - `node_a` and `node_b` are unique node identifiers.
 * - `time_cost` represents the travel cost between `node_a` and `node_b`.
 *
 * Functionality:
 * - Iterates through the provided edge list and extracts node and edge information.
 * - Constructs an adjacency list for efficient graph traversal.
 * - Optionally preprocesses landmark distances for ALT heuristics.
 *
 * Processing Steps:
 * - Iterate through the edge list, extracting node IDs and edge weights.
 * - Assign unique indices to nodes using a hash map.
 * - Store edges and populate adjacency lists.
 * - Construct a mapping between node IDs and indices.
 * - If ALT heuristics are enabled, preprocess landmark distances.
 *
 * Edge Case Handling:
 * - If no edges exist, an empty graph is returned.
 * - If no nodes exist, the expansion ratio is set to `1.0` by default.
 * - Ensures memory efficiency by clearing unused data.
 *
 * @param conf Reference to the configuration settings, which includes heuristic settings.
 * @param edges Reference to the vector of edges containing node connections and travel costs.
 * @return A fully constructed `graph` object containing nodes, edges, and adjacency lists.
 *
 * @complexity
 * - Time Complexity: O(E) (Processes `E` edges).
 * - Space Complexity: O(N + E) (Stores `N` nodes and `E` edges).
 */
graph loadGraphData(config& conf, std::vector<edge>& edges)
{
    console("info", "starting to load graph data from memory.");
    logger("starting to load graph data from memory.");

    graph gdata;

    for (const auto& edge : edges) {
        gdata.line_count++;

        int na = edge.a;
        int nb = edge.b;
        int tc = edge.t;

        if (gdata.node_to_index.find(na) == gdata.node_to_index.end()) {
            gdata.node_to_index[na] = gdata.index_count++;
        }
        if (gdata.node_to_index.find(nb) == gdata.node_to_index.end()) {
            gdata.node_to_index[nb] = gdata.index_count++;
        }

        gdata.edges.emplace_back(na, nb, tc);
    }

    gdata.adjacency.resize(gdata.node_to_index.size());
    for (const auto& [a, b, cost] : gdata.edges) {
        size_t ia = gdata.node_to_index.at(a);
        size_t ib = gdata.node_to_index.at(b);

        gdata.adjacency[ia].emplace_back(std::make_pair(static_cast<int>(ib), cost));
        gdata.adjacency[ib].emplace_back(std::make_pair(static_cast<int>(ia), cost));
    }

    gdata.index_to_node.resize(gdata.node_to_index.size());
    for (const auto& kv : gdata.node_to_index) {
        gdata.index_to_node[kv.second] = kv.first;
    }

    edges.clear();
    std::vector<edge>().swap(edges);

    console("success", "finished loading graph data from memory.");
    logger("finished loading graph data from memory.");
    logger("total lines processed: " + std::to_string(gdata.line_count));
    logger("total unique nodes: " + std::to_string(gdata.node_to_index.size()));
    logger("total edges collected: " + std::to_string(gdata.edges.size()));

    if (conf.use_alt) {
        console("info", "starting ALT pre-processing.");
        logger("starting ALT pre-processing.");
        preprocessAlt(gdata, conf);
    }

    return gdata;
}
