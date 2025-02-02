// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Loads graph data from memory and constructs the graph representation.
 *
 * @detailed
 * This function processes a vector of edges stored in memory and constructs
 * an undirected graph optimized for shortest-path computations. Each edge in the input
 * is defined by three values:
 *
 *     node_a, node_b, time_cost
 *
 * where:
 * - `node_a` and `node_b` are unique node identifiers.
 * - `time_cost` represents the travel cost between `node_a` and `node_b`.
 *
 * Functionality:
 * - Iterates through the edge list to identify and assign unique indices to nodes.
 * - Constructs an adjacency list for efficient graph traversal by adding bidirectional edges.
 * - Builds a reverse mapping (index_to_node) for quick lookups of node identifiers by index.
 * - Clears the original edge vector after processing to conserve memory.
 * - Logs the progress and key statistics (total lines processed, unique nodes) via console and logger.
 * - Optionally invokes ALT (A*, Landmarks, and Triangle inequality) pre-processing if enabled in the configuration.
 *
 * Processing Steps:
 * - First pass: Iterate through edges to assign unique indices to nodes using a hash map.
 * - Resize the adjacency list based on the number of unique nodes.
 * - Second pass: Populate the adjacency list by adding bidirectional connections.
 * - Construct a reverse mapping vector (`index_to_node`) for node lookups.
 * - Clear the original edge list to free memory.
 * - Log progress messages and summary statistics.
 * - If ALT heuristics are enabled in the configuration, call `preprocessAlt` for further pre-processing.
 *
 * Edge Case Handling:
 * - If no edges exist, an empty graph is returned.
 * - Ensures memory efficiency by clearing unused data.
 *
 * @param conf Reference to the configuration settings, including heuristic and ALT pre-processing flags.
 * @param edges Reference to the vector of edges containing node connections and travel costs.
 * @return A fully constructed `graph` object containing nodes, bidirectional edges, and adjacency lists.
 *
 * @complexity
 * - Time Complexity: O(E) (Processes each of the `E` edges).
 * - Space Complexity: O(N + E) (Stores `N` unique nodes and `E` edges in the graph representation).
 */
graph loadGraphData(config& conf, std::vector<edge>& edges)
{
    console("info", "starting to load graph data from memory.");
    logger("starting to load graph data from memory.");

    graph gdata;

    for (const auto& e : edges) {
        gdata.line_count++;

        if (gdata.node_to_index.find(e.a) == gdata.node_to_index.end()) {
            gdata.node_to_index[e.a] = gdata.index_count++;
        }
        if (gdata.node_to_index.find(e.b) == gdata.node_to_index.end()) {
            gdata.node_to_index[e.b] = gdata.index_count++;
        }
    }

    gdata.adjacency.resize(gdata.node_to_index.size());

    for (const auto& e : edges) {
        size_t ia = gdata.node_to_index[e.a];
        size_t ib = gdata.node_to_index[e.b];

        gdata.adjacency[ia].emplace_back(ib, e.t);
        gdata.adjacency[ib].emplace_back(ia, e.t);
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

    if (conf.use_alt) {
        console("info", "starting ALT pre-processing.");
        logger("starting ALT pre-processing.");
        preprocessAlt(gdata, conf);
    }

    return gdata;
}

