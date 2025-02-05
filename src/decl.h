#ifndef DECL_H
#define DECL_H

#include "incl.h"

/*------------------------------------------------------------------------------------
							color definition.
------------------------------------------------------------------------------------*/

inline std::string RED = "\033[1;31m"; 
inline std::string BLUE = "\033[1;34m";
inline std::string YELLOW = "\033[38;5;226m";
inline std::string GREEN = "\033[1;32m";
inline std::string RESET = "\033[0m";

/*------------------------------------------------------------------------------------
							weight definition.
------------------------------------------------------------------------------------*/

/**
 * @brief Stores general configuration settings.
 *
 * Holds file paths, algorithm settings, and logging preferences.
 */
struct config
{
    std::string map_path{};
    std::string config_file{};
    std::string log_file{};
    std::string alt_prep_file{};

    bool use_alt = false;
    bool save_alt = false;
    int nb_alt = 0;

    double weight = 1;
    bool personalized_weight = false;

    bool log = false;

    config() = default;
};

/**
 * @brief Represents a graph structure with edges lists and landmarks.
 *
 * Stores graph topology, edges representation, and precomputed landmark distances.
 */
struct graph
{
    std::unordered_map<int, size_t> node_to_index;
    std::vector<int> index_to_node;

    size_t line_count = 0;
    size_t index_count = 0;

    struct edge_repr {
        int target;
        int weight;
    };

    std::vector<size_t> offsets;
    std::vector<edge_repr> edges;

    std::vector<std::vector<int>> dist_landmark;
};

/**
 * @brief Stores optimization flags.
 *
 * Tracks whether graph optimizations (e.g., ALT preprocessing) have been applied.
 * (for now, we use only ALT preprocessing, but, if we want to implement other algorithm like CH (contraption hierarchies), we can add it simply inside the opti flag.)
 */
struct optimization_flags
{
    bool alt_optimized = false;
};

/**
 * @brief Represents a state in Dijkstra's algorithm.
 *
 * Stores the current node and its distance, with comparison for priority queue usage.
 */
struct dijkstra_state
{
    int node;
    int distance;

    bool operator>(const dijkstra_state& other) const {
        return distance > other.distance;
    }
};

/**
 * @brief Hash function for std::pair<int, int>.
 *
 * Enables unordered_map usage for edge-based lookups.
 */
struct pair_hash
{
    std::size_t operator()(const std::pair<int, int>& p) const noexcept {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

/**
 * @brief Represents the result of a shortest path computation.
 *
 * Stores total travel time, number of nodes visited, and the path sequence.
 */
struct path_result
{
    int total_time;
    int total_node;
    std::vector<int> steps;
};

/**
 * @brief Represents an edge in the graph.
 *
 * Defines a directed connection between two nodes with a travel time.
 * (only used for loading .csv file inside memory and make some verifications.)
 */
struct edge
{
    int a;
    int b;
    int t;
};

/**
 * @brief Holds reusable buffers for the shortest path search.
 *
 * This structure stores preallocated buffers for distance tracking, parent 
 * pointers, and heuristic values to optimize repeated searches. Instead of 
 * reinitializing buffers for each search, a versioning system is used to track 
 * valid entries, enabling efficient reuse without unnecessary memory operations.
 * 
 * Each buffer has an associated version vector that ensures only relevant values 
 * are accessed or updated in a given search. The `current_search_id` increments 
 * with each new search, allowing old values to be automatically ignored.
 */
struct search_buffers {
    std::vector<int> dist_from_start;
    std::vector<int> dist_from_end;
    std::vector<std::pair<int, int>> parent_forward;
    std::vector<std::pair<int, int>> parent_backward;
    std::vector<int> h_forward;
    std::vector<int> h_backward;
    
    int current_search_id = 1;

    std::vector<int> version_dist_from_start;
    std::vector<int> version_dist_from_end;
    std::vector<int> version_parent_forward;
    std::vector<int> version_parent_backward;
    std::vector<int> version_h_forward;
    std::vector<int> version_h_backward;
};

/*------------------------------------------------------------------------------------
							function declaration.
------------------------------------------------------------------------------------*/

//  helper.cpp (General utilities)
bool fileExists(const std::string& path);
void ensureDirectory(const std::string& path);
std::string computeFileHash(const std::string& file_path);

void clear();

int getInteger(const std::string& prompt);
double getPercentage(const std::string& prompt);
bool getYesNo(const std::string& prompt);

void initializeSearchBuffers(const graph& gdata, search_buffers& buffers);

//  log.cpp (Logging)
bool initLogger(const std::string& log_file_path);
void closeLogger();
void logger(const std::string& message);

void console(const std::string& type, const std::string& message);

//  conf.cpp (Configuration)
config getConfiguration(config conf);
void loggerConf(config& conf);

bool updateNbAlt(config& conf, int new_nb_alt);

//  loader.cpp (Graph loading)
graph loadGraphData(config& conf, std::vector<edge>& edges);
bool readCSV(const std::string& map_path, std::vector<edge>& edges);

//  connectivity.cpp (Graph verification)
bool checkConnectivity(const std::vector<edge>& edges);
bool checkIntegrity(const std::vector<edge>& edges);

//  opti.cpp (Graph optimization)
optimization_flags checkGraphOptimization(const std::string& map_path, double alt_density_threshold = 1e-4);

//  alt.cpp (ALT preprocessing)
void preprocessAlt(graph& gdata, config& conf);

//  bu_alt.cpp (ALT data storage/loading)
void saveAltData(const graph& gdata, const config& conf);
bool loadAltData(graph& gdata, config& conf);

//  perf.cpp (Performance tracking)
void storePerf(const graph& g);

//  search.cpp (Pathfinding)
path_result findShortestPath(const graph& gdata, search_buffers& buffers, const config& conf, int start_node, int end_node, double weight);

std::vector<int> dijkstraSingleSource(const graph& gdata, int source, size_t node_count);

//  api.cpp (API management)
int launchApiGateway(const graph& gdata, search_buffers& buffers, const config& conf);

#endif //DECL_H
