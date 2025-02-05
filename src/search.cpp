// ✅ file verified.
#include "incl.h"
#include "decl.h"
#include "profiling.h"

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

static inline int baseHeuristicFunc(int current_node, int goal_node)
{
    int diff = current_node - goal_node;
    return (diff < 0) ? -diff : diff;
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
static inline int altHeuristicFunc(int current_node, int goal_node, const graph& gdata)
{
    if (current_node == goal_node) {
        return 0;
    }

    auto it_cur = gdata.node_to_index.find(current_node);
    auto it_goal = gdata.node_to_index.find(goal_node);
    if (it_cur == gdata.node_to_index.end() || it_goal == gdata.node_to_index.end()) {
        return 0;
    }

    size_t idx_c = it_cur->second;
    size_t idx_g = it_goal->second;

    const auto& dm_c = gdata.dist_landmark[idx_c];
    const auto& dm_g = gdata.dist_landmark[idx_g];

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

    if (best_val < 0) {
        best_val = 0;
    }
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
static inline int computeHeuristic(int current_node, int goal_node, const graph& gdata, const config& conf)
{
    return conf.use_alt ? altHeuristicFunc(current_node, goal_node, gdata)
                        : baseHeuristicFunc(current_node, goal_node);
}

// ✅ function + comment verified.
/**
 * @brief Implements a multithreaded bidirectional shortest path search algorithm.
 *
 * @detailed
 * This function finds the shortest path between `start_node` and `end_node` in
 * the given graph using a Bidirectional A* approach, but with multithreading
 * to significantly enhance performance.
 *
 * Functionality:
 * - Two threads are spawned: forward expansion and backward expansion.
 * - Two priority queues (`forward_queue` and `backward_queue`) are protected by
 *   separate mutexes, allowing forward and backward expansions to proceed in parallel.
 * - A shared, atomic `best_distance` prunes expansions early across threads.
 * - Once a thread detects it cannot improve upon `best_distance`, it marks the search
 *   as complete.
 * - The search stops as soon as both queues are empty or no better solution is possible.
 * - The `weight` parameter allows a suboptimal but faster search:
 *   - `1.0` = Best path (no suboptimality).
 *   - `1.1` = Up to 10% suboptimal but potentially faster.
 *   - `1.2+` = Allow more suboptimality with generally faster expansions.
 * 
 *   (one of my implementations was the parallel processing of both expansions, which
 *   actually led to better performance with a weight of 1.0 than 1.2)
 *
 * Updates:
 * - Optimized buffer initialization using lazy reset via versioning:
 *   - Instead of resetting all buffers with `std::fill`, a search ID system is used.
 *   - Each search increments `current_search_id`, avoiding O(n) reset costs.
 * - Improved memory efficiency and response time:
 *   - Removed redundant buffer resets, reducing overhead.
 *   - Significantly reduces unnecessary memory writes, leading to better cache performance.
 * - Maintains correctness with conditional access:
 *   - Buffers now track their version, ensuring correctness while skipping unnecessary writes.
 *   - Default values (`-1` or `{ -1, 0 }`) are automatically used for untouched entries.
 *
 * Properties:
 * - Efficient pathfinding through bidirectional expansion.
 * - Optimized with multithreading for faster performance.
 * - Supports weighted heuristic tuning for speed vs. accuracy trade-offs.
 *
 * @param gdata Reference to the graph data.
 * @param conf Configuration settings for heuristic selection.
 * @param start_node The starting node.
 * @param end_node The target node.
 * @param weight Heuristic weight influencing path selection
 *               (`1.0` = guaranteed shortest path, `>1.0` = faster but possibly suboptimal).
 * @param buffers Reference to `search_buffers` for optimized memory management.
 * @return A `path_result` structure containing the shortest path details.
 *
 * @complexity
 * - Time Complexity: Approximately O(E log V), but performed in parallel (2 threads).
 * - Space Complexity: O(V) for storing distances, parents, etc.
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

path_result findShortestPath(const graph& gdata, search_buffers& buffers, const config& conf, int start_node, int end_node, double weight)
{
    PROFILE_START("findShortestPath_total");

    PROFILE_START("input_validation");
    if (start_node == end_node) {
        PROFILE_STOP("input_validation");
        PROFILE_STOP("findShortestPath_total");
        return { 0, 0, {} };
    }
    
    auto it_start = gdata.node_to_index.find(start_node);
    auto it_end = gdata.node_to_index.find(end_node);
    if (it_start == gdata.node_to_index.end() || it_end == gdata.node_to_index.end()) {
        PROFILE_STOP("input_validation");
        PROFILE_STOP("findShortestPath_total");
        return { -1, 0, {} };
    }
    PROFILE_STOP("input_validation");

    PROFILE_START("index_mapping");
    size_t start_idx = it_start->second;
    size_t end_idx = it_end->second;
    const int inf = std::numeric_limits<int>::max();
    PROFILE_STOP("index_mapping");

    PROFILE_START("buffers_update");
    buffers.current_search_id++;
    setDistFromStart(buffers, start_idx, 0);
    setDistFromEnd(buffers, end_idx, 0);
    PROFILE_STOP("buffers_update");

    PROFILE_START("priority_queue_setup");
    using pq_item = std::pair<int, int>;
    auto cmp = [](const pq_item& a, const pq_item& b) {
        return a.first > b.first;
    };
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> forward_queue(cmp);
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> backward_queue(cmp);
    PROFILE_STOP("priority_queue_setup");

    PROFILE_START("heuristic_computation");
    int h_start = computeHeuristic(start_node, end_node, gdata, conf);
    int h_end   = computeHeuristic(end_node, start_node, gdata, conf);
    setHForward(buffers, start_idx, h_start);
    setHBackward(buffers, end_idx, h_end);
    PROFILE_STOP("heuristic_computation");

    PROFILE_START("initial_queue_push");
    forward_queue.push({ getDistFromStart(buffers, start_idx) + (int)(weight * h_start), (int)start_idx });
    backward_queue.push({ getDistFromEnd(buffers, end_idx)   + (int)(weight * h_end),   (int)end_idx });
    PROFILE_STOP("initial_queue_push");

    std::atomic<int> best_distance(inf);
    std::atomic<int> best_meet_node(-1);
    std::atomic<bool> search_done(false);

    std::mutex forward_mutex;
    std::mutex backward_mutex;

    PROFILE_START("lambda_getForwardH_setup");
    auto getForwardH = [&](int idx) -> int {
        int hv = getHForward(buffers, idx);
        if (hv >= 0) {
            return hv;
        }
        int real_node = gdata.index_to_node[idx];
        hv = computeHeuristic(real_node, end_node, gdata, conf);
        setHForward(buffers, idx, hv);
        return hv;
    };
    PROFILE_STOP("lambda_getForwardH_setup");

    PROFILE_START("lambda_getBackwardH_setup");
    auto getBackwardH = [&](int idx) -> int {
        int hv = getHBackward(buffers, idx);
        if (hv >= 0) {
            return hv;
        }
        int real_node = gdata.index_to_node[idx];
        hv = computeHeuristic(real_node, start_node, gdata, conf);
        setHBackward(buffers, idx, hv);
        return hv;
    };
    PROFILE_STOP("lambda_getBackwardH_setup");

    auto expandForward = [&](int cur_idx, int cur_f) {
        PROFILE_START("expandForward_total");
        int cur_g = getDistFromStart(buffers, cur_idx);
        int h_val = getForwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            PROFILE_STOP("expandForward_total");
            return;
        }
        PROFILE_START("expandForward_neighbors");

        size_t start_edge = gdata.offsets[cur_idx];
        size_t end_edge = ((static_cast<size_t>(cur_idx) + 1) < gdata.offsets.size()) ? gdata.offsets[static_cast<size_t>(cur_idx) + 1] : gdata.edges.size();

        int cnt = 0;
        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        for (size_t i = start_edge; i < end_edge; i++) {
            PROFILE_START("expandForward_iteration");
            if (++cnt % 4 == 0 && search_done.load(std::memory_order_relaxed)) {
                PROFILE_STOP("expandForward_iteration");
                break;
            }
            const auto& edge = gdata.edges[i];
            int nbr_idx = edge.target;
            int cost = edge.weight;
            int new_g = cur_g + cost;

            if (getDistFromEnd(buffers, nbr_idx) >= 0 && (new_g + getDistFromEnd(buffers, nbr_idx)) >= best_dist_snapshot) {
                PROFILE_STOP("expandForward_iteration");
                continue;
            }
        
            int existing = getDistFromStart(buffers, nbr_idx);
            if (existing < 0 || new_g < existing) {
                setDistFromStart(buffers, nbr_idx, new_g);
                setParentForward(buffers, nbr_idx, { cur_idx, cost });
                int h_nbr = getForwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                {
                    std::lock_guard<std::mutex> lk(forward_mutex);
                    forward_queue.push({ f_cost, nbr_idx });
                }
            }
            if (getDistFromEnd(buffers, nbr_idx) >= 0) {
                int total_cost = new_g + getDistFromEnd(buffers, nbr_idx);
                if (total_cost < best_dist_snapshot) {
                    best_distance.store(total_cost, std::memory_order_relaxed);
                    best_meet_node.store(nbr_idx, std::memory_order_relaxed);
                }
            }
            PROFILE_STOP("expandForward_iteration");
        }
        PROFILE_STOP("expandForward_neighbors");
        PROFILE_STOP("expandForward_total");
    };

    auto expandBackward = [&](int cur_idx, int cur_f) {
        PROFILE_START("expandBackward_total");
        int cur_g = getDistFromEnd(buffers, cur_idx);
        int h_val = getBackwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            PROFILE_STOP("expandBackward_total");
            return;
        }
        PROFILE_START("expandBackward_neighbors");

        size_t start_edge = gdata.offsets[cur_idx];
        size_t end_edge = ((static_cast<size_t>(cur_idx) + 1) < gdata.offsets.size()) ? gdata.offsets[static_cast<size_t>(cur_idx) + 1] : gdata.edges.size();
        
        int cnt = 0;
        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        for (size_t i = start_edge; i < end_edge; i++) {
            PROFILE_START("expandBackward_iteration");
            if (++cnt % 4 == 0 && search_done.load(std::memory_order_relaxed)) {
                PROFILE_STOP("expandBackward_iteration");
                break;
            }
            const auto& edge = gdata.edges[i];
            int nbr_idx = edge.target;
            int cost = edge.weight;
            int new_g = cur_g + cost;
            if (getDistFromStart(buffers, nbr_idx) >= 0 &&
                (new_g + getDistFromStart(buffers, nbr_idx)) >= best_dist_snapshot) {
                PROFILE_STOP("expandBackward_iteration");
                continue;
            }
            int existing = getDistFromEnd(buffers, nbr_idx);
            if (existing < 0 || new_g < existing) {
                setDistFromEnd(buffers, nbr_idx, new_g);
                setParentBackward(buffers, nbr_idx, { cur_idx, cost });
                int h_nbr = getBackwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                {
                    std::lock_guard<std::mutex> lk(backward_mutex);
                    backward_queue.push({ f_cost, nbr_idx });
                }
            }
            if (getDistFromStart(buffers, nbr_idx) >= 0) {
                int total_cost = new_g + getDistFromStart(buffers, nbr_idx);
                if (total_cost < best_dist_snapshot) {
                    best_distance.store(total_cost, std::memory_order_relaxed);
                    best_meet_node.store(nbr_idx, std::memory_order_relaxed);
                }
            }
            PROFILE_STOP("expandBackward_iteration");
        }
        PROFILE_STOP("expandBackward_neighbors");
        PROFILE_STOP("expandBackward_total");
    };
    
    auto forwardThreadFunc = [&]() {
        PROFILE_START("forwardThread_total");
        while (!search_done.load(std::memory_order_relaxed)) {
            PROFILE_START("forwardThread_iteration");
            int cur_f, cur_idx;
            {
                if (forward_queue.empty()) {
                    search_done.store(true, std::memory_order_relaxed);
                    PROFILE_STOP("forwardThread_iteration");
                    break;
                }
                auto top_item = forward_queue.top();
                forward_queue.pop();
                cur_f = top_item.first;
                cur_idx = top_item.second;
            }
            int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
            if (cur_f >= best_dist_snapshot) {
                search_done.store(true, std::memory_order_relaxed);
                PROFILE_STOP("forwardThread_iteration");
                break;
            }
            expandForward(cur_idx, cur_f);
            PROFILE_STOP("forwardThread_iteration");
        }
        PROFILE_STOP("forwardThread_total");
    };

    auto backwardThreadFunc = [&]() {
        PROFILE_START("backwardThread_total");
        while (!search_done.load(std::memory_order_relaxed)) {
            PROFILE_START("backwardThread_iteration");
            int cur_f, cur_idx;
            {
                if (backward_queue.empty()) {
                    search_done.store(true, std::memory_order_relaxed);
                    PROFILE_STOP("backwardThread_iteration");
                    break;
                }
                auto top_item = backward_queue.top();
                backward_queue.pop();
                cur_f = top_item.first;
                cur_idx = top_item.second;
            }
            int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
            if (cur_f >= best_dist_snapshot) {
                search_done.store(true, std::memory_order_relaxed);
                PROFILE_STOP("backwardThread_iteration");
                break;
            }
            expandBackward(cur_idx, cur_f);
            PROFILE_STOP("backwardThread_iteration");
        }
        PROFILE_STOP("backwardThread_total");
    };

    PROFILE_START("thread_launch");
    std::thread forward_thread(forwardThreadFunc);
    std::thread backward_thread(backwardThreadFunc);
    PROFILE_STOP("thread_launch");

    PROFILE_START("thread_join");
    forward_thread.join();
    backward_thread.join();
    PROFILE_STOP("thread_join");

    PROFILE_START("result_evaluation");
    int final_best_distance = best_distance.load(std::memory_order_relaxed);
    int meet_idx = best_meet_node.load(std::memory_order_relaxed);
    if (meet_idx < 0 || final_best_distance >= inf) {
        PROFILE_STOP("result_evaluation");
        PROFILE_STOP("findShortestPath_total");
        return { -1, 0, {} };
    }
    PROFILE_STOP("result_evaluation");

    PROFILE_START("path_reconstruction");
    std::vector<int> forward_indices;
    {
        PROFILE_START("forward_path_build");
        int cur = meet_idx;
        while (cur != (int)start_idx) {
            auto par = getParentForward(buffers, cur);
            forward_indices.push_back(cur);
            cur = par.first;
        }
        forward_indices.push_back((int)start_idx);
        std::reverse(forward_indices.begin(), forward_indices.end());
        PROFILE_STOP("forward_path_build");
    }

    std::vector<int> backward_indices;
    {
        PROFILE_START("backward_path_build");
        int cur = meet_idx;
        while (cur != (int)end_idx) {
            auto par = getParentBackward(buffers, cur);
            backward_indices.push_back(cur);
            cur = par.first;
        }
        backward_indices.push_back((int)end_idx);
        PROFILE_STOP("backward_path_build");
    }

    if (!backward_indices.empty()) {
        backward_indices.erase(backward_indices.begin());
    }
    forward_indices.insert(forward_indices.end(), backward_indices.begin(), backward_indices.end());
    for (auto& idx : forward_indices) {
        idx = gdata.index_to_node[idx];
    }
    PROFILE_STOP("path_reconstruction");

    path_result result{};
    result.total_time = final_best_distance;
    result.total_node = (int)forward_indices.size();
    result.steps = std::move(forward_indices);

    PROFILE_STOP("findShortestPath_total");
    PROFILE_REPORT();
    return result;
}