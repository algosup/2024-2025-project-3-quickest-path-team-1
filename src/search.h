// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Inline helper functions for lazy-access to search buffers.
 *
 * These functions provide getters and setters for distances, heuristics, and parent pointers
 * in the `search_buffers` structure. They utilize versioning to ensure that only data from the
 * current search (indicated by `current_search_id`) is considered valid, thereby avoiding
 * unnecessary full-buffer resets.
 */
namespace {
    inline int getDistFromStart(const search_buffers &buffers, size_t idx) {
        return (buffers.version_dist_from_start[idx] == buffers.current_search_id) ? buffers.dist_from_start[idx] : -1;
    }
    inline void setDistFromStart(search_buffers &buffers, size_t idx, int value) {
        buffers.dist_from_start[idx] = value;
        buffers.version_dist_from_start[idx] = buffers.current_search_id;
    }
    
    inline int getDistFromEnd(const search_buffers &buffers, size_t idx) {
        return (buffers.version_dist_from_end[idx] == buffers.current_search_id) ? buffers.dist_from_end[idx] : -1;
    }
    inline void setDistFromEnd(search_buffers &buffers, size_t idx, int value) {
        buffers.dist_from_end[idx] = value;
        buffers.version_dist_from_end[idx] = buffers.current_search_id;
    }
    
    inline int getHForward(const search_buffers &buffers, size_t idx) {
        return (buffers.version_h_forward[idx] == buffers.current_search_id) ? buffers.h_forward[idx] : -1;
    }
    inline void setHForward(search_buffers &buffers, size_t idx, int value) {
        buffers.h_forward[idx] = value;
        buffers.version_h_forward[idx] = buffers.current_search_id;
    }
    
    inline int getHBackward(const search_buffers &buffers, size_t idx) {
        return (buffers.version_h_backward[idx] == buffers.current_search_id) ? buffers.h_backward[idx] : -1;
    }
    inline void setHBackward(search_buffers &buffers, size_t idx, int value) {
        buffers.h_backward[idx] = value;
        buffers.version_h_backward[idx] = buffers.current_search_id;
    }
    
    inline std::pair<int,int> getParentForward(const search_buffers &buffers, size_t idx) {
        return (buffers.version_parent_forward[idx] == buffers.current_search_id) ? buffers.parent_forward[idx] : std::make_pair(-1, 0);
    }
    inline void setParentForward(search_buffers &buffers, size_t idx, std::pair<int,int> value) {
        buffers.parent_forward[idx] = value;
        buffers.version_parent_forward[idx] = buffers.current_search_id;
    }
    
    inline std::pair<int,int> getParentBackward(const search_buffers &buffers, size_t idx) {
        return (buffers.version_parent_backward[idx] == buffers.current_search_id) ? buffers.parent_backward[idx] : std::make_pair(-1, 0);
    }
    inline void setParentBackward(search_buffers &buffers, size_t idx, std::pair<int,int> value) {
        buffers.parent_backward[idx] = value;
        buffers.version_parent_backward[idx] = buffers.current_search_id;
    }
}

// ✅ function + comment verified.
/**
 * @brief Computes a simple heuristic distance between two nodes.
 *
 * @detailed
 * This function calculates the heuristic distance between the `current_node`
 * and the `goal_node` using the absolute difference:
 *
 *     h(n) = |current_node - goal_node|
 *
 * This heuristic is commonly referred to as the Manhattan distance.
 *
 * Functionality:
 * - Computes the absolute difference between `current_node` and `goal_node`.
 * - Used in heuristic-based search algorithms.
 *
 * Properties:
 * - It is admissible if the nodes represent positions on a linear path.
 * - It is consistent since moving from `n` to `n+1` always increases the
 *   cost by at most `1`.
 *
 * @param current_node The starting node.
 * @param goal_node The target node.
 * @param gdata Reference to the graph data.
 * @return The absolute difference between the two nodes.
 *
 * @complexity
 * - Time Complexity: O(1) (Performs a single subtraction and absolute operation).
 * - Space Complexity: O(1) (No additional memory allocated).
 */

static inline int baseHeuristicFunc(size_t current_idx, size_t goal_idx, const graph& gdata)
{
    int c_node = gdata.index_to_node[current_idx];
    int g_node = gdata.index_to_node[goal_idx];
    int diff = c_node - g_node;
    return diff < 0 ? -diff : diff;
}

// ✅ function + comment verified.
/**
 * @brief Computes a heuristic estimate between two nodes using landmark-based heuristics.
 *
 * @detailed
 * This function implements a more advanced heuristic based on precomputed distances
 * from landmarks. It estimates the heuristic cost using the triangle inequality:
 *
 *      h(n) = max{ |d_L(goal) - d_L(n)| }
 *
 * where:
 * - `d_L(n)` represents the precomputed distance from landmark L to node n.
 *
 * Functionality:
 * - Computes heuristic estimates based on landmark distances.
 * - Uses the triangle inequality to derive a tighter lower bound.
 * - Processes multiple landmarks in an optimized loop.
 *
 * @note
 * - Caching has been removed to enhance performance by eliminating synchronization overhead.
 * - Loop unrolling is employed to reduce iteration overhead and boost performance.
 *
 * @param current_node The starting node.
 * @param goal_node The target node.
 * @param gdata Reference to the graph data.
 * @return The heuristic estimate based on landmark distances.
 *
 * @complexity
 * - Time Complexity: O(L) (L is the number of landmarks used; loop unrolling helps reduce overhead but remains linear).
 * - Space Complexity: O(1) (No caching is used).
 */
static inline int altHeuristicFunc(size_t current_idx, size_t goal_idx, const graph& gdata)
{
    if (current_idx == goal_idx) {
        return 0;
    }
    const auto &dm_c = gdata.dist_landmark[current_idx];
    const auto &dm_g = gdata.dist_landmark[goal_idx];

    int best_val = 0;
    int count_l = (int)dm_c.size();
    const int* dm_c_ptr = dm_c.data();
    const int* dm_g_ptr = dm_g.data();

#define ALT_HEURISTIC_STEP(I)                                \
    {                                                        \
        int d_c = dm_c_ptr[(I)];                             \
        int d_g = dm_g_ptr[(I)];                             \
        if (d_c < 0 || d_g < 0)                              \
            continue;                                        \
        int diff = d_g - d_c;                                \
        if (diff < 0) diff = -diff;                          \
        if (diff > best_val) best_val = diff;                \
    }

    int i = 0;
    for (; i + 3 < count_l; i += 4) {
        ALT_HEURISTIC_STEP(i);
        ALT_HEURISTIC_STEP(i + 1);
        ALT_HEURISTIC_STEP(i + 2);
        ALT_HEURISTIC_STEP(i + 3);
    }
    for (; i < count_l; i++) {
        ALT_HEURISTIC_STEP(i);
    }

#undef ALT_HEURISTIC_STEP
    return best_val;
}

// ✅ function + comment verified.
/**
 * @brief Computes the heuristic value based on the configuration settings.
 *
 * @detailed
 * This function determines whether to use the base heuristic or an alternative
 * heuristic based on the provided configuration.
 *
 * Functionality:
 * - If `conf.use_alt` is `true`, it calls `altHeuristicFunc()`.
 * - Otherwise, it falls back to `baseHeuristicFunc()`.
 * - Allows dynamic selection of heuristic strategies based on runtime requirements.
 *
 * Properties:
 * - Ensures flexibility in heuristic computation.
 * - Supports both simple and advanced heuristic techniques.
 *
 * @param current_node The starting node.
 * @param goal_node The target node.
 * @param gdata Reference to the graph data.
 * @param conf Configuration settings for heuristic selection.
 * @return The computed heuristic value.
 *
 * @complexity
 * - Time Complexity: O(L) (if `altHeuristicFunc` is used), O(1) (for `baseHeuristicFunc`).
 * - Space Complexity: O(1) (No extra allocations performed).
 */
static inline int computeHeuristic(size_t current_idx, size_t goal_idx, const graph& gdata, const config& conf)
{
    return conf.use_alt ? altHeuristicFunc(current_idx, goal_idx, gdata)
                        : baseHeuristicFunc(current_idx, goal_idx, gdata);
}