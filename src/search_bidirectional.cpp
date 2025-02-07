// ✅ file verified.
#include "incl.h"
#include "decl.h"
#include "search.h"

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
path_result findShortestPathBidirectional(const graph& gdata, search_buffers& buffers, const config& conf, int start_node, int end_node, double weight)
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

    buffers.current_search_id++;
    setDistFromStart(buffers, start_idx, 0);
    setDistFromEnd(buffers, end_idx, 0);

    using pq_item = std::pair<int, size_t>;
    auto cmp = [](const pq_item& a, const pq_item& b) {
        return a.first > b.first;
    };
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> forward_queue(cmp);
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> backward_queue(cmp);

    int h_start = computeHeuristic(start_idx, end_idx, gdata, conf);
    int h_end = computeHeuristic(end_idx, start_idx, gdata, conf);
    setHForward(buffers, start_idx, h_start);
    setHBackward(buffers, end_idx, h_end);

    forward_queue.push({ getDistFromStart(buffers, start_idx) + (int)(weight * h_start), (int)start_idx });
    backward_queue.push({ getDistFromEnd(buffers, end_idx)   + (int)(weight * h_end),   (int)end_idx });

    std::atomic<int> best_distance(inf);
    std::atomic<int> best_meet_node(-1);
    std::atomic<bool> search_done(false);

    std::mutex forward_mutex;
    std::mutex backward_mutex;

    auto getForwardH = [&](int idx) -> int {
        int hv = getHForward(buffers, idx);
        if (hv >= 0) {
            return hv;
        }
        hv = computeHeuristic(idx, end_idx, gdata, conf);
        setHForward(buffers, idx, hv);
        return hv;
    };

    auto getBackwardH = [&](int idx) -> int {
        int hv = getHBackward(buffers, idx);
        if (hv >= 0) {
            return hv;
        }
        hv = computeHeuristic(idx, start_idx, gdata, conf);
        setHBackward(buffers, idx, hv);
        return hv;
    };

    auto expandForward = [&](int cur_idx_int, int cur_f) {
        size_t cur_idx = (size_t)cur_idx_int;

        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        int local_best_dist = best_dist_snapshot;
        int local_best_node = best_meet_node.load(std::memory_order_relaxed);

        int cur_g = getDistFromStart(buffers, cur_idx);
        int h_val = getForwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            return;
        }

        size_t start_edge = gdata.offsets[cur_idx];
        size_t end_edge = ((cur_idx + 1) < gdata.offsets.size()) ? gdata.offsets[cur_idx + 1] : gdata.edges.size();

        std::vector<pq_item> local_queue_insert;
        local_queue_insert.reserve(end_edge - start_edge);

        for (size_t i = start_edge; i < end_edge; i++) {
            if (search_done.load(std::memory_order_relaxed)) {
                break;
            }
            const auto& edge = gdata.edges[i];
            int nbr_idx = edge.target;
            int cost = edge.weight;
            int new_g = cur_g + cost;

            if (getDistFromEnd(buffers, nbr_idx) >= 0) {
                int possible_dist = new_g + getDistFromEnd(buffers, nbr_idx);
                if (possible_dist >= local_best_dist) {
                    continue;
                }
            }

            int existing = getDistFromStart(buffers, nbr_idx);
            if (existing < 0 || new_g < existing) {
                setDistFromStart(buffers, nbr_idx, new_g);
                setParentForward(buffers, nbr_idx, { (int)cur_idx, cost });
                int h_nbr = getForwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                local_queue_insert.push_back({ f_cost, nbr_idx });
            }

            int dist_from_end = getDistFromEnd(buffers, nbr_idx);
            if (dist_from_end >= 0) {
                int total_cost = new_g + dist_from_end;
                if (total_cost < local_best_dist) {
                    local_best_dist = total_cost;
                    local_best_node = nbr_idx;
                }
            }
        }

        if (!local_queue_insert.empty()) {
            std::lock_guard<std::mutex> lk(forward_mutex);
            for (auto &ins : local_queue_insert) {
                forward_queue.push(ins);
            }
        }

        if (local_best_dist < best_dist_snapshot) {
            int global_best = best_distance.load(std::memory_order_relaxed);
            if (local_best_dist < global_best) {
                best_distance.store(local_best_dist, std::memory_order_relaxed);
                best_meet_node.store(local_best_node, std::memory_order_relaxed);
            }
        }
    };

    auto expandBackward = [&](int cur_idx_int, int cur_f) {
        size_t cur_idx = (size_t)cur_idx_int;

        int best_dist_snapshot = best_distance.load(std::memory_order_relaxed);
        int local_best_dist = best_dist_snapshot;
        int local_best_node = best_meet_node.load(std::memory_order_relaxed);

        int cur_g = getDistFromEnd(buffers, cur_idx);
        int h_val = getBackwardH(cur_idx);
        if (cur_g + (int)(weight * h_val) < cur_f) {
            return;
        }

        size_t start_edge = gdata.offsets[cur_idx];
        size_t end_edge = ((cur_idx + 1) < gdata.offsets.size()) ? gdata.offsets[cur_idx + 1] : gdata.edges.size();

        std::vector<pq_item> local_queue_insert;
        local_queue_insert.reserve(end_edge - start_edge);

        for (size_t i = start_edge; i < end_edge; i++) {
            if (search_done.load(std::memory_order_relaxed)) {
                break;
            }
            const auto& edge = gdata.edges[i];
            int nbr_idx = edge.target;
            int cost = edge.weight;
            int new_g = cur_g + cost;

            if (getDistFromStart(buffers, nbr_idx) >= 0) {
                int possible_dist = new_g + getDistFromStart(buffers, nbr_idx);
                if (possible_dist >= local_best_dist) {
                    continue;
                }
            }

            int existing = getDistFromEnd(buffers, nbr_idx);
            if (existing < 0 || new_g < existing) {
                setDistFromEnd(buffers, nbr_idx, new_g);
                setParentBackward(buffers, nbr_idx, { (int)cur_idx, cost });
                int h_nbr = getBackwardH(nbr_idx);
                int f_cost = new_g + (int)(weight * h_nbr);
                local_queue_insert.push_back({ f_cost, nbr_idx });
            }

            if (getDistFromStart(buffers, nbr_idx) >= 0) {
                int total_cost = new_g + getDistFromStart(buffers, nbr_idx);
                if (total_cost < local_best_dist) {
                    local_best_dist = total_cost;
                    local_best_node = nbr_idx;
                }
            }
        }

        if (!local_queue_insert.empty()) {
            std::lock_guard<std::mutex> lk(backward_mutex);
            for (auto &ins : local_queue_insert) {
                backward_queue.push(ins);
            }
        }

        if (local_best_dist < best_dist_snapshot) {
            int global_best = best_distance.load(std::memory_order_relaxed);
            if (local_best_dist < global_best) {
                best_distance.store(local_best_dist, std::memory_order_relaxed);
                best_meet_node.store(local_best_node, std::memory_order_relaxed);
            }
        }
    };

    auto forwardThreadFunc = [&]() {
        while (!search_done.load(std::memory_order_relaxed)) {
            int cur_f, cur_idx;
            {
                std::lock_guard<std::mutex> lk(forward_mutex);
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
                std::lock_guard<std::mutex> lk(backward_mutex);
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
            auto par = getParentForward(buffers, cur);
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
            auto par = getParentBackward(buffers, cur);
            backward_indices.push_back(cur);
            cur = par.first;
        }
        backward_indices.push_back((int)end_idx);
    }

    if (!backward_indices.empty()) {
        backward_indices.erase(backward_indices.begin());
    }
    forward_indices.insert(forward_indices.end(), backward_indices.begin(), backward_indices.end());
    for (auto& idx : forward_indices) {
        idx = gdata.index_to_node[idx];
    }

    path_result result{};
    result.total_time = final_best_distance;
    result.total_node = (int)forward_indices.size();
    result.steps = std::move(forward_indices);

    return result;
}