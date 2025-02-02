﻿// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Clears the terminal screen using ANSI escape codes.
 *
 * Supports Windows (10+) by enabling Virtual Terminal Processing and Unix-like systems directly.
 * (I try to never implement system("") to ensure that that the code is safe, and can't be exploited.)
 */
void clear() {
#ifdef _WIN32
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_out == INVALID_HANDLE_VALUE) return;

    DWORD dw_mode = 0;
    if (!GetConsoleMode(h_out, &dw_mode)) return;

    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_out, dw_mode);
#endif
    std::cout << "\033[2J\033[H" << std::flush;
}

// ✅ function + comment verified.
/**
 * @brief Checks if a file exists at the specified path.
 *
 * Attempts to open the file in read mode to determine its existence.
 *
 * @param path The file path to check.
 * @return `true` if the file exists and is accessible, otherwise `false`.
 */
bool fileExists(const std::string& path)
{
    std::ifstream ifs(path);
    return ifs.good();
}

// ✅ function + comment verified.
/**
 * @brief Ensures that a directory exists, creating it if necessary.
 *
 * Creates the specified directory and any missing parent directories.
 *
 * @param path The directory path to ensure exists.
 */
void ensureDirectory(const std::string& path)
{
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
}

// ✅ function + comment verified.
/**
 * @brief Computes a hash of a file's contents.
 *
 * Reads the entire file and returns a hexadecimal hash string.
 *
 * @param file_path The path to the file.
 * @return A hexadecimal string representing the file's hash, or `"0"` if unreadable.
 */
std::string computeFileHash(const std::string& file_path)
{
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs.is_open()) {
        return "0";
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string content = oss.str();
    std::hash<std::string> hasher;
    size_t h = hasher(content);
    std::stringstream ss;
    ss << std::hex << h;
    return ss.str();
}


// ✅ function + comment verified.
/**
 * @brief Prompts the user for a yes/no response.
 *
 * Continuously asks until the user enters 'y' or 'n'.
 *
 * @param prompt The message displayed to the user.
 * @return `true` if the user selects 'y', otherwise `false`.
 */
bool getYesNo(const std::string& prompt)
{
    std::string response;
    while (true) {
        std::cout << prompt << std::flush;
        std::cin >> response;
        if (response == "y" || response == "n") {
            return response == "y";
        }
        console("error", "invalid input. please enter 'y' or 'n'.");
    }
}

// ✅ function + comment verified.
/**
 * @brief Prompts the user for an integer input.
 *
 * Continues prompting until a valid integer is entered.
 *
 * @param prompt The message displayed to the user.
 * @return The validated integer input.
 */
int getInteger(const std::string& prompt)
{
    std::string response;
    int value;
    while (true) {
        std::cout << prompt << std::flush;
        std::cin >> response;
        try {
            size_t pos;
            value = std::stoi(response, &pos);
            if (pos == response.length()) {
                return value;
            }
            else {
                throw std::invalid_argument("Invalid characters found");
            }
        }
        catch (std::exception&) {
            console("error", "invalid input. please enter a valid integer.");
        }
    }
}

// ✅ function + comment verified.
/**
 * @brief Prompts the user for a percentage and returns it as a multiplier.
 *
 * Accepts an integer between 0 and 100 and converts it to a multiplier (1.0 to 2.0).
 *
 * @param prompt The message displayed to the user.
 * @return A multiplier representing the entered percentage.
 */
double getPercentage(const std::string& prompt)
{
    int percent;
    while (true) {
        percent = getInteger(prompt);
        if (percent >= 0 && percent <= 100) {
            break;
        }
        console("error", "invalid input. please enter an integer between 0 and 100.");
    }
    return 1.0 + (static_cast<double>(percent) / 100.0);
}



// ✅ function + comment verified.
/**
 * @brief Computes shortest paths from a single source using Dijkstra’s algorithm.
 *
 * @detailed
 * This function implements Dijkstra’s algorithm to compute the shortest paths from
 * the source node to all other nodes in a graph with non-negative edge weights.
 *
 * Functionality:
 * - Uses a priority queue to select the node with the smallest distance.
 * - Updates distances to neighboring nodes as shorter paths are found.
 * - Iterates until all reachable nodes are processed.
 *
 * Mathematical Properties:
 * - The time complexity of Dijkstra's algorithm (with a binary heap) is:
 *
 *     O((V + E) log V)
 *
 *   where:
 *   - `V` is the number of vertices (nodes).
 *   - `E` is the number of edges.
 *
 * Edge Cases:
 * - If the source node is invalid, the function returns a distance array filled with `-1`.
 *
 * @param adjacency Adjacency list representation of the graph.
 * @param source The starting node for the shortest path computation.
 * @param node_count The total number of nodes in the graph.
 * @param node_to_index Mapping of node IDs to array indices.
 * @return A vector containing the shortest path distances from the source to all nodes.
 *
 * @complexity
 * - Time Complexity: O((V + E) log V) (Dijkstra's algorithm with a binary heap).
 * - Space Complexity: O(V) (Stores shortest path distances).
 */
std::vector<int> dijkstraSingleSource(const std::vector<std::vector<std::pair<int, int>>>& adjacency, int source, size_t node_count, const std::unordered_map<int, size_t>& node_to_index)
{
    std::vector<int> distances(node_count, -1);

    auto it = node_to_index.find(source);
    if (it == node_to_index.end()) return distances;

    distances[it->second] = 0;
    std::priority_queue<dijkstra_state, std::vector<dijkstra_state>, std::greater<dijkstra_state>> pq;
    pq.push({ static_cast<int>(it->second), 0 });

    while (!pq.empty()) {
        auto [cur_idx, cur_dist] = pq.top();
        pq.pop();

        if (distances[cur_idx] != cur_dist) continue;

        for (auto& [nbr_idx, cost] : adjacency[cur_idx]) {
            int nd = cur_dist + cost;
            if (distances[nbr_idx] < 0 || nd < distances[nbr_idx]) {
                distances[nbr_idx] = nd;
                pq.push({ nbr_idx, nd });
            }
        }
    }

    return distances;
}