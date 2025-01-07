#include "incl.h"

graph_data load_graph_data(const std::string& file_path) /* () load graph data inside memory. */
{
    _logger("Starting to load graph data from: " + file_path);

    std::ifstream file_stream(file_path);
    if (!file_stream.is_open()) { // return an empty graph if the file cannot be open.
        _logger("Error: Could not open file: " + file_path);
        return {};
    }

    /* 
        > estimate file size & reserve it.
    */

    std::streamsize file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    // estimate file size with a supposition of the average size of a line.
    const size_t average_line_size = 50;
    size_t estimated_lines = file_size / average_line_size;

    graph_data gdata;
    
    // reserve the estimated size file.
    gdata.node_to_index.reserve(estimated_lines);
    gdata.edges.reserve(estimated_lines);

    /*
        < estimate file size & reserve it.
    */

    /*
        > data traitment.    
    */

    std::string line_data;

    while (std::getline(file_stream, line_data)) 
    {
        gdata.line_count++;
        if (line_data.empty()) {
            continue;
        }

        std::stringstream line_stream(line_data);
        std::string val_a, val_b, val_time;
        
        // reading all value inside .csv file & checking if any value is missing.
        if (!std::getline(line_stream, val_a, ',')) {
            std::cout << "[WARNING] Missing Landmark_A_ID at line " + std::to_string(gdata.line_count) << std::endl;
            _logger("Warning: Missing Landmark_A_ID at line " + std::to_string(gdata.line_count));
            continue;
        }
        if (!std::getline(line_stream, val_b, ',')) {
            std::cout << "[WARNING] Missing Landmark_B_ID at line " + std::to_string(gdata.line_count) << std::endl;
            _logger("Warning: Missing Landmark_B_ID at line " + std::to_string(gdata.line_count));
            continue;
        }

        if (!std::getline(line_stream, val_time, ',')) {
            std::cout << "[WARNING] Missing val_time at line " + std::to_string(gdata.line_count) << std::endl;
            _logger("Warning: Missing val_time at line " + std::to_string(gdata.line_count));
            continue;
        }

        int node_a, node_b, time_cost;

        try {
            node_a = std::stoi(val_a);
            node_b = std::stoi(val_b);
            time_cost = std::stoi(val_time);
        }
        catch (const std::exception& e) { // catch any formatting problem of retreived values.
            _logger("Warning: Invalid node/time ID at line "
                + std::to_string(gdata.line_count) + ": " + e.what());
            continue;
        }

        // assign an index if they never been met.
        if (gdata.node_to_index.find(node_a) == gdata.node_to_index.end()) {
            gdata.node_to_index[node_a] = gdata.index_count++;
        }
        if (gdata.node_to_index.find(node_b) == gdata.node_to_index.end()) {
            gdata.node_to_index[node_b] = gdata.index_count++;
        }

        // stock the decree inside a triplet (A, B, time_cost).
        gdata.edges.emplace_back(node_a, node_b, time_cost);

        // adjacence construction
        gdata.adjacency[node_a].push_back({node_b, time_cost});
        gdata.adjacency[node_b].push_back({node_a, time_cost});
    }

    /*
        < data traitment.
    */

    _logger("Finished loading graph data.");
    _logger("Total lines processed: " + std::to_string(gdata.line_count));
    _logger("Total unique nodes: " + std::to_string(gdata.node_to_index.size()));
    _logger("Total edges collected: " + std::to_string(gdata.edges.size()));

    // initialize union-find at the right size.
    gdata.uf.resize(gdata.node_to_index.size());
    return gdata;
}
