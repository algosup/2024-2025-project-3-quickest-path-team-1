#include "incl.h"

bool connectivity_validator(graph_data& gdata) /* () validate the connectivity between all node of the map. */
{
    _logger("Starting connectivity check.");

    size_t edge_count = 0;
    for (const auto& edge : gdata.edges) 
    {
        // now edge is a triplet.
        int node_a, node_b, time_cost;
        std::tie(node_a, node_b, time_cost) = edge;

        // getting union-find index.
        int idx_a = gdata.node_to_index[node_a];
        int idx_b = gdata.node_to_index[node_b];
        
        // union.
        gdata.uf.union_set(idx_a, idx_b);

        edge_count++;
        if (edge_count % 1'000'000 == 0) {
            _logger("Processed " + std::to_string(edge_count) + " edges for connectivity...");
        }
    }
    _logger("All edges processed for connectivity.");

    // checking connectivity by verifying if all node have the same representative.
    if (gdata.node_to_index.empty()) {
        _logger("No nodes found in graph_data. Connectivity check failed.");
        return false;
    }

    // we take the representative of index 0 (or any other) as a reference.
    int first_rep = gdata.uf.find_set(0);

    size_t i = 0;
    for (; i < gdata.node_to_index.size(); ++i) {
        if (gdata.uf.find_set(static_cast<int>(i)) != first_rep) {
            _logger("Graph is NOT fully connected. Disconnection found at index " 
                + std::to_string(i));
            return false;
        }
        if (i % 1'000'000 == 0 && i > 0) {
            _logger("Checked connectivity for " + std::to_string(i) + " nodes...");
        }
    }

    _logger("Connectivity check passed. The graph is fully connected.");
    return true;
}
