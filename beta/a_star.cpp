#include "incl.h"

path_result find_shortest_path_a_star(const graph_data& gdata,
    int start_node,
    int end_node)
{
    // handle trivial cases.
    if (start_node == end_node)
        return { 0, {} };

    if (gdata.adjacency.find(start_node) == gdata.adjacency.end() ||
        gdata.adjacency.find(end_node) == gdata.adjacency.end())
        return { -1, {} };

    // initialize structures.
    std::unordered_map<int, int> dist_from_start, dist_from_end;
    std::unordered_map<int, std::pair<int, int>> parent_start, parent_end;

    const int INF = std::numeric_limits<int>::max();

    // setup initial distances and parents.
    dist_from_start[start_node] = 0;
    dist_from_end[end_node] = 0;
    parent_start[start_node] = { -1, 0 };
    parent_end[end_node] = { -1, 0 };

    // priority queues for bidirectional search.
    typedef std::pair<int, int> pq_item;
    auto cmp = [](const pq_item& a, const pq_item& b) {
        return a.first > b.first;
        };
    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> forward_queue(cmp), backward_queue(cmp);

    forward_queue.push({ 0, start_node });
    backward_queue.push({ 0, end_node });

    int best_meet_node = -1;
    int best_distance = INF;

    // alternate search steps until paths meet.
    while (!forward_queue.empty() || !backward_queue.empty()) 
    {
        // forward step.
        if (!forward_queue.empty()) {
            pq_item topF = forward_queue.top();
            forward_queue.pop();

            int cur_dist = topF.first;
            int cur_node = topF.second;

            if (dist_from_start[cur_node] < cur_dist) continue;

            if (gdata.adjacency.find(cur_node) != gdata.adjacency.end()) {
                const auto& edges = gdata.adjacency.at(cur_node);
                for (auto& e : edges) {
                    int neigh = e.first;
                    int cost = e.second;
                    int ndist = cur_dist + cost;

                    if (!dist_from_start.count(neigh) || ndist < dist_from_start[neigh]) {
                        dist_from_start[neigh] = ndist;
                        parent_start[neigh] = { cur_node, cost };
                        forward_queue.push({ ndist, neigh });
                    }

                    if (dist_from_end.count(neigh)) {
                        int total_potential = ndist + dist_from_end[neigh];
                        if (total_potential < best_distance) {
                            best_distance = total_potential;
                            best_meet_node = neigh;
                        }
                    }
                }
            }
        }

        // terminate early if queues cannot improve the best distance.
        if (best_distance < INF) {
            if (!forward_queue.empty() && forward_queue.top().first > best_distance) break;
            if (!backward_queue.empty() && backward_queue.top().first > best_distance) break;
        }

        // backward step.
        if (!backward_queue.empty()) {
            pq_item topB = backward_queue.top();
            backward_queue.pop();

            int cur_dist = topB.first;
            int cur_node = topB.second;

            if (dist_from_end[cur_node] < cur_dist) continue;

            if (gdata.adjacency.find(cur_node) != gdata.adjacency.end()) {
                const auto& edges = gdata.adjacency.at(cur_node);
                for (auto& e : edges) {
                    int neigh = e.first;
                    int cost = e.second;
                    int ndist = cur_dist + cost;

                    if (!dist_from_end.count(neigh) || ndist < dist_from_end[neigh]) {
                        dist_from_end[neigh] = ndist;
                        parent_end[neigh] = { cur_node, cost };
                        backward_queue.push({ ndist, neigh });
                    }

                    if (dist_from_start.count(neigh)) {
                        int total_potential = ndist + dist_from_start[neigh];
                        if (total_potential < best_distance) {
                            best_distance = total_potential;
                            best_meet_node = neigh;
                        }
                    }
                }
            }
        }

        if (best_distance < INF) {
            if (!forward_queue.empty() && forward_queue.top().first > best_distance) break;
            if (!backward_queue.empty() && backward_queue.top().first > best_distance) break;
        }
    }

    // no path found.
    if (best_meet_node < 0)
        return { -1, {} };

    // reconstruct the path.
    std::vector<path_step> path_forward, path_backward;

    for (int cur = best_meet_node; cur != start_node; cur = parent_start[cur].first) {
        path_forward.push_back({ parent_start[cur].first, cur, parent_start[cur].second });
    }
    std::reverse(path_forward.begin(), path_forward.end());

    for (int cur = best_meet_node; cur != end_node; cur = parent_end[cur].first) {
        path_backward.push_back({ cur, parent_end[cur].first, parent_end[cur].second });
    }

    path_forward.insert(path_forward.end(), path_backward.begin(), path_backward.end());

    return { best_distance, path_forward };
}
