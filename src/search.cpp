﻿// ✅ file verified.
#include "incl.h"
#include "decl.h"

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
            continue;                                      \
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
    if (conf.use_alt) {
        return altHeuristicFunc(current_node, goal_node, gdata);
    }
    return baseHeuristicFunc(current_node, goal_node);
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
 * - Refactored to reuse preallocated search buffers (`search_buffers`):
 *   - Removed per-call vector allocations.
 *   - Buffers are initialized once and reset per search, reducing overhead.
 * - Improved memory efficiency and response time:
 *   - Faster initialization through `std::fill` instead of full reallocations.
 *   - Reduced overall heap usage, leading to better performance.
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

path_result findShortestPath(const graph& gdata, search_buffers& buffers, const config& conf, int start_node, int end_node, double weight)
{
    if (start_node == end_node) {
        return { 0, 0, {} };
    }

    auto it_start = gdata.node_to_index.find(start_node);
    auto it_end = gdata.node_to_index.find(end_node);
    if (it_start == gdata.node_to_index.end() || it_end == gdata.node_to_index.end()) {
        return { -1, 0, {} };
    }

    size_t start_idx = it_start->second;
    size_t end_idx = it_end->second;

    const int inf = std::numeric_limits<int>::max();

    std::fill(buffers.dist_from_start.begin(), buffers.dist_from_start.end(), -1);
    std::fill(buffers.dist_from_end.begin(), buffers.dist_from_end.end(), -1);
    std::fill(buffers.h_forward.begin(),  buffers.h_forward.end(), -1);
    std::fill(buffers.h_backward.begin(), buffers.h_backward.end(), -1);
    std::fill(buffers.parent_forward.begin(), buffers.parent_forward.end(), std::make_pair(-1, 0));
    std::fill(buffers.parent_backward.begin(), buffers.parent_backward.end(), std::make_pair(-1, 0));

    buffers.dist_from_start[start_idx] = 0;
    buffers.dist_from_end[end_idx] = 0;

    using pq_item = std::pair<int, int>;
    auto cmp = [](const pq_item& a, const pq_item& b) {
        return a.first > b.first;
    };
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> forward_queue(cmp);
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> backward_queue(cmp);

    int h_start = computeHeuristic(start_node, end_node, gdata, conf);
    int h_end   = computeHeuristic(end_node, start_node, gdata, conf);
    buffers.h_forward[start_idx] = h_start;
    buffers.h_backward[end_idx] = h_end;

    forward_queue.push({ buffers.dist_from_start[start_idx] + (int)(weight * h_start), (int)start_idx });
    backward_queue.push({ buffers.dist_from_end[end_idx]   + (int)(weight * h_end),   (int)end_idx });

    std::atomic<int> best_distance(inf);
    std::atomic<int> best_meet_node(-1);
    std::atomic<bool> search_done(false);

    std::mutex forward_mutex;
    std::mutex backward_mutex;

    const auto& adjacency = gdata.adjacency;

    auto getForwardH = [&](int idx) -> int {
        if (buffers.h_forward[idx] >= 0) {
            return buffers.h_forward[idx];
        }
        int real_node = gdata.index_to_node[idx];
        int hv = computeHeuristic(real_node, end_node, gdata, conf);
        buffers.h_forward[idx] = hv;
        return hv;
    };

    auto getBackwardH = [&](int idx) -> int {
        if (buffers.h_backward[idx] >= 0) {
            return buffers.h_backward[idx];
        }
        int real_node = gdata.index_to_node[idx];
        int hv = computeHeuristic(real_node, start_node, gdata, conf);
        buffers.h_backward[idx] = hv;
        return hv;
    };

    auto expandForward = [&](int cur_idx, int cur_f) {
        int cur_g = buffers.dist_from_start[cur_idx];
        int h_val = getForwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            return;
        }
        const auto& neighbors = adjacency[cur_idx];
        int cnt = 0;
        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        for (auto& edge : neighbors) {
            if (++cnt % 4 == 0 && search_done.load(std::memory_order_relaxed)) {
                return;
            }
            int nbr_idx = edge.first;
            int cost = edge.second;
            int new_g = cur_g + cost;
            if (buffers.dist_from_end[nbr_idx] >= 0 && (new_g + buffers.dist_from_end[nbr_idx]) >= best_dist_snapshot) {
                continue;
            }
            if (buffers.dist_from_start[nbr_idx] < 0 || new_g < buffers.dist_from_start[nbr_idx]) {
                buffers.dist_from_start[nbr_idx] = new_g;
                buffers.parent_forward[nbr_idx] = { cur_idx, cost };
                int h_nbr = getForwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                {
                    std::lock_guard<std::mutex> lk(forward_mutex);
                    forward_queue.push({ f_cost, nbr_idx });
                }
            }
            if (buffers.dist_from_end[nbr_idx] >= 0) {
                int total_cost = new_g + buffers.dist_from_end[nbr_idx];
                if (total_cost < best_dist_snapshot) {
                    best_distance.store(total_cost, std::memory_order_relaxed);
                    best_meet_node.store(nbr_idx, std::memory_order_relaxed);
                }
            }
        }
    };

    auto expandBackward = [&](int cur_idx, int cur_f) {
        int cur_g = buffers.dist_from_end[cur_idx];
        int h_val = getBackwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            return;
        }
        const auto& neighbors = adjacency[cur_idx];
        int cnt = 0;
        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        for (auto& edge : neighbors) {
            if (++cnt % 4 == 0 && search_done.load(std::memory_order_relaxed)) {
                return;
            }
            int nbr_idx = edge.first;
            int cost = edge.second;
            int new_g = cur_g + cost;
            if (buffers.dist_from_start[nbr_idx] >= 0 && (new_g + buffers.dist_from_start[nbr_idx]) >= best_dist_snapshot) {
                continue;
            }
            if (buffers.dist_from_end[nbr_idx] < 0 || new_g < buffers.dist_from_end[nbr_idx]) {
                buffers.dist_from_end[nbr_idx] = new_g;
                buffers.parent_backward[nbr_idx] = { cur_idx, cost };
                int h_nbr = getBackwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                {
                    std::lock_guard<std::mutex> lk(backward_mutex);
                    backward_queue.push({ f_cost, nbr_idx });
                }
            }
            if (buffers.dist_from_start[nbr_idx] >= 0) {
                int total_cost = new_g + buffers.dist_from_start[nbr_idx];
                if (total_cost < best_dist_snapshot) {
                    best_distance.store(total_cost, std::memory_order_relaxed);
                    best_meet_node.store(nbr_idx, std::memory_order_relaxed);
                }
            }
        }
    };

    auto forwardThreadFunc = [&]() {
        while (!search_done.load(std::memory_order_relaxed)) {
            int cur_f, cur_idx;
            {
                if (forward_queue.empty()) {
                    search_done.store(true, std::memory_order_relaxed);
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
                break;
            }
            expandForward(cur_idx, cur_f);
        }
    };

    auto backwardThreadFunc = [&]() {
        while (!search_done.load(std::memory_order_relaxed)) {
            int cur_f, cur_idx;
            {
                if (backward_queue.empty()) {
                    search_done.store(true, std::memory_order_relaxed);
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
                break;
            }
            expandBackward(cur_idx, cur_f);
        }
    };

    std::thread forward_thread(forwardThreadFunc);
    std::thread backward_thread(backwardThreadFunc);

    forward_thread.join();
    backward_thread.join();

    int final_best_distance = best_distance.load(std::memory_order_relaxed);
    int meet_idx = best_meet_node.load(std::memory_order_relaxed);

    if (meet_idx < 0 || final_best_distance >= inf) {
        return { -1, 0, {} };
    }

    std::vector<int> forward_indices;
    {
        int cur = meet_idx;
        while (cur != (int)start_idx) {
            auto& par = buffers.parent_forward[cur];
            forward_indices.push_back(cur);
            cur = par.first;
        }
        forward_indices.push_back((int)start_idx);
        std::reverse(forward_indices.begin(), forward_indices.end());
    }

    std::vector<int> backward_indices;
    {
        int cur = meet_idx;
        while (cur != (int)end_idx) {
            auto& par = buffers.parent_backward[cur];
            backward_indices.push_back(cur);
            cur = par.first;
        }
        backward_indices.push_back((int)end_idx);
    }

    if (!backward_indices.empty()) {
        backward_indices.erase(backward_indices.begin());
    }
    forward_indices.insert(
        forward_indices.end(),
        backward_indices.begin(),
        backward_indices.end()
    );
    for (auto& idx : forward_indices) {
        idx = gdata.index_to_node[idx];
    }

    path_result result{};
    result.total_time = final_best_distance;
    result.total_node = (int)forward_indices.size();
    result.steps = std::move(forward_indices);
    return result;
}