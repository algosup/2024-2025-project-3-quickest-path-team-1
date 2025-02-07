// ✅ file verified.
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
 * @brief Prompts the user for a 1/2 response.
 *
 * Continuously asks until the user enters '1' or '2'.
 *
 * @param prompt The message displayed to the user.
 * @return '1' or '2'.
 */
int getOneOrTwo(const std::string& prompt)
{
    int one_or_two;
    while (true) {
        one_or_two = getInteger(prompt);
        if (one_or_two == 1 || one_or_two == 2) {
            break;
        }
        console("error", "invalid input. please enter 1 or 2.");
    }
    return one_or_two;
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
 * @param gdata Graph representation containing adjacency information.
 * @param source The starting node for the shortest path computation.
 * @param node_count The total number of nodes in the graph.
 * @return A vector containing the shortest path distances from the source to all nodes.
 *
 * @complexity
 * - Time Complexity: O((V + E) log V) (Dijkstra's algorithm with a binary heap).
 * - Space Complexity: O(V) (Stores shortest path distances).
 */

std::vector<int> dijkstraSingleSource(const graph& gdata, int source, size_t node_count)
{
    std::vector<int> distances(node_count, -1);

    auto it = gdata.node_to_index.find(source);
    if (it == gdata.node_to_index.end())
        return distances;

    size_t source_idx = it->second;
    distances[source_idx] = 0;

    using state = std::pair<int, size_t>;
    std::priority_queue<state, std::vector<state>, std::greater<state>> pq;
    pq.push({0, source_idx});

    while (!pq.empty()) {
        auto [cur_dist, cur_idx] = pq.top();
        pq.pop();

        if (distances[cur_idx] != cur_dist)
            continue;

        size_t start_edge = gdata.offsets[cur_idx];
        size_t end_edge = (cur_idx + 1 < gdata.offsets.size()) ? gdata.offsets[cur_idx + 1] : gdata.edges.size();

        for (size_t i = start_edge; i < end_edge; ++i) {
            const auto& edge = gdata.edges[i];
            size_t nbr_idx = edge.target;
            int nd = cur_dist + edge.weight;
            if (distances[nbr_idx] < 0 || nd < distances[nbr_idx]) {
                distances[nbr_idx] = nd;
                pq.push({nd, nbr_idx});
            }
        }
    }

    return distances;
}

// ✅ function + comment verified.
/**
 * @brief Initializes the search buffers for the graph.
 *
 * Allocates and prepares the necessary buffers (distance vectors, 
 * parent pointers, and heuristic caches) based on the graph's size. It also 
 * initializes version tracking for each buffer to enable efficient reuse across 
 * multiple searches without requiring full reallocation.
 * 
 * The versioning system ensures that buffer values remain valid for the 
 * duration of a search while allowing automatic reset when a new search begins. 
 * This approach improves performance by reducing redundant operations.
 *
 * This function should be called once during initialization or whenever the 
 * graph structure changes, ensuring that the buffers are properly sized 
 * and ready for subsequent searches.
 *
 * @param gdata The graph data structure.
 * @param buffers The search_buffers structure to initialize.
 */
void initializeSearchBuffers(const graph& gdata, search_buffers& buffers) {
    size_t n = gdata.node_to_index.size();
    buffers.dist_from_start.resize(n);
    buffers.dist_from_end.resize(n);
    buffers.parent_forward.resize(n);
    buffers.parent_backward.resize(n);
    buffers.h_forward.resize(n);
    buffers.h_backward.resize(n);
    
    buffers.version_dist_from_start.assign(n, 0);
    buffers.version_dist_from_end.assign(n, 0);
    buffers.version_parent_forward.assign(n, 0);
    buffers.version_parent_backward.assign(n, 0);
    buffers.version_h_forward.assign(n, 0);
    buffers.version_h_backward.assign(n, 0);
    
    buffers.current_search_id = 1; 
}