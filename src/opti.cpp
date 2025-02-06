// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Determines if ALT optimization is beneficial based on graph density.
 *
 * @detailed
 * This function evaluates the density of a graph by mapping nodes to unique numeric 
 * identifiers and counting the number of edges. The purpose is to determine whether 
 * the ALT (A* Landmarks and Triangle Inequality) heuristic should be applied for 
 * optimization.
 *
 * Functionality:
 * - Assigns a unique numeric ID to each node encountered.
 * - Counts the total number of edges.
 * - Computes the graph density to assess ALT heuristic effectiveness.
 * - Decides whether ALT preprocessing should be applied based on the density threshold.
 *
 * Mathematical Analysis:
 * - The graph density is computed as:
 *
 *     density = (actual_edges) / (maximum_possible_edges)
 *
 *   where:
 *   - `actual_edges = edge_count`
 *   - `maximum_possible_edges = (node_count * (node_count - 1)) / 2`
 *   - This assumes an undirected graph without duplicate edges.
 *
 * - If the computed density is below the provided ALT threshold, ALT preprocessing is 
 *   deemed beneficial.
 *
 * Steps of Execution:
 * - Initializes a node mapper to assign unique IDs to nodes.
 * - Reads edges, extracting node identifiers and counting them.
 * - Computes graph density based on the formula above.
 * - Returns an `optimization_flags` structure indicating if ALT optimization is beneficial.
 *
 * Edge Cases Handled:
 * - If the graph has one or zero nodes, density is set to `0.0` to prevent division errors.
 *
 * @param alt_density_threshold Density threshold for ALT optimization.
 * @return `optimization_flags` structure indicating if ALT optimization is beneficial.
 *
 * @complexity
 * - Time Complexity: O(E) (Processes each edge once).
 * - Space Complexity: O(N) (Stores unique node mappings).
 */
optimization_flags checkGraphOptimization(const std::string& map_path, double alt_density_threshold)
{
    struct node_mapper {
        std::unordered_map<std::string, size_t> node_to_id;
        size_t current_id = 0;

        size_t get_id(const std::string& node) {
            auto it = node_to_id.find(node);
            if (it != node_to_id.end()) return it->second;
            node_to_id[node] = current_id;
            return current_id++;
        }

        size_t total_nodes() const {
            return current_id;
        }
    };

    std::ifstream file(map_path, std::ios::binary);
    if (!file.is_open()) {
        console("error", "could not open file : " + map_path);
        logger("error: could not open file : " + map_path);
        return {};
    }

    node_mapper mapper;
    mapper.node_to_id.reserve(1024);

    size_t edge_count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t c1 = line.find(',');
        if (c1 == std::string::npos) continue;
        size_t c2 = line.find(',', c1 + 1);
        if (c2 == std::string::npos) continue;

        std::string landmark_a = line.substr(0, c1);
        std::string landmark_b = line.substr(c1 + 1, c2 - (c1 + 1));

        mapper.get_id(landmark_a);
        mapper.get_id(landmark_b);
        ++edge_count;
    }
    file.close();

    size_t node_count = mapper.total_nodes();
    double density = 0.0;
    if (node_count > 1) {
        double max_edges = static_cast<double>(node_count) * (node_count - 1) / 2.0;
        density = edge_count / max_edges;
    }

    double avg_edges_per_node = (node_count > 0) ? (2.0 * edge_count / node_count) : 0.0;
    
    optimization_flags flags;
    flags.alt_optimized = (density < alt_density_threshold);
    if (avg_edges_per_node < 4) {
        flags.search_engine_recommanded = 1;
    }
    else if (avg_edges_per_node >= 4 && avg_edges_per_node <= 5) {
        flags.search_engine_recommanded = 2;
    }
    else if (avg_edges_per_node > 5) {
        flags.search_engine_recommanded = 3;
    }
    return flags;
}
