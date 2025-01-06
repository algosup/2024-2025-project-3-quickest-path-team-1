#include "incl.h"

bool connectivity_validator(const std::string& file_path) {
    _logger("Starting connectivity validation.");

    std::ifstream file_stream(file_path);
    if (!file_stream.is_open()) {
        _logger("Error: Could not open file: " + file_path);
        return false;
    }
    _logger("File opened successfully: " + file_path);

    // Maps original node ID -> compressed index (0..n-1)
    std::unordered_map<int, int> node_to_index;
    node_to_index.reserve(24000000); // Reserve space for 24 million nodes

    std::vector<std::pair<int, int>> edges;
    edges.reserve(24000000); // Adjust based on expected edges

    std::string line_data;
    int index_count = 0;
    size_t line_number = 0;

    while (std::getline(file_stream, line_data)) {
        line_number++;
        if (line_data.empty()) {
            continue;
        }
        std::stringstream line_stream(line_data);
        std::string val_a, val_b, val_time;
        if (!std::getline(line_stream, val_a, ',')) {
            _logger("Warning: Missing Landmark_A_ID at line " + std::to_string(line_number));
            continue;
        }
        if (!std::getline(line_stream, val_b, ',')) {
            _logger("Warning: Missing Landmark_B_ID at line " + std::to_string(line_number));
            continue;
        }
        if (!std::getline(line_stream, val_time, ',')) {
            // Time is optional for connectivity; log if desired
        }

        int node_a, node_b;
        try {
            node_a = std::stoi(val_a);
            node_b = std::stoi(val_b);
        }
        catch (const std::exception& e) {
            _logger("Warning: Invalid node ID at line " + std::to_string(line_number) + ": " + e.what());
            continue;
        }

        // Assign indices if not already assigned
        if (node_to_index.find(node_a) == node_to_index.end()) {
            node_to_index[node_a] = index_count++;
            if (index_count % 1000000 == 0) {
                _logger("Assigned indices to " + std::to_string(index_count) + " nodes.");
            }
        }
        if (node_to_index.find(node_b) == node_to_index.end()) {
            node_to_index[node_b] = index_count++;
            if (index_count % 1000000 == 0) {
                _logger("Assigned indices to " + std::to_string(index_count) + " nodes.");
            }
        }

        edges.emplace_back(node_a, node_b);

        if (line_number % 1000000 == 0) {
            _logger("Processed " + std::to_string(line_number) + " lines.");
        }
    }

    _logger("Total lines processed: " + std::to_string(line_number));
    _logger("Total unique nodes: " + std::to_string(node_to_index.size()));
    _logger("Total edges collected: " + std::to_string(edges.size()));

    if (node_to_index.empty()) {
        _logger("No nodes found in the file.");
        return false;
    }

    UnionFind uf;
    uf.resize(node_to_index.size());
    _logger("Union-Find structure initialized.");

    size_t edge_count = 0;
    for (const auto& e : edges) {
        uf.union_set(node_to_index[e.first], node_to_index[e.second]);
        edge_count++;
        if (edge_count % 1000000 == 0) {
            _logger("Processed " + std::to_string(edge_count) + " edges.");
        }
    }
    _logger("All edges processed.");

    // Check if all nodes belong to the same connected component
    int representative = uf.find_set(0);
    bool fully_connected = true;
    for (size_t i = 1; i < node_to_index.size(); ++i) {
        if (uf.find_set(i) != representative) {
            fully_connected = false;
            _logger("Disconnection found between nodes at index 0 and " + std::to_string(i));
            break;
        }
        if (i % 1000000 == 0) {
            _logger("Checked connectivity for " + std::to_string(i) + " nodes.");
        }
    }

    if (fully_connected) {
        _logger("The graph is fully connected.");
    }
    else {
        _logger("The graph is NOT fully connected.");
    }

    return fully_connected;
}