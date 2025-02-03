// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Precomputes landmark-based shortest path estimates using the ALT heuristic.
 *
 * @detailed
 * This function implements ALT (A* Landmarks and Triangle Inequality) preprocessing, which enhances
 * heuristic pathfinding by precomputing distances from selected landmark nodes.
 *
 * Functionality:
 * - Uses the farthest-node method to select optimal landmarks.
 * - Computes shortest-path distances using Dijkstra’s algorithm.
 * - Optionally loads or stores precomputed landmark data for reuse.
 *
 * Steps of the Algorithm:
 * 1. If `save_alt` is enabled, attempt to load precomputed landmark data from storage.
 * 2. Initialize storage structures for distance calculations.
 * 3. Select the first landmark as an arbitrary node (first node in the dataset).
 * 4. Compute shortest-path distances using Dijkstra’s algorithm:
 *    - `dist_landmark[i][j]` stores the shortest path distance from landmark `j` to node `i`.
 * 5. Iteratively select `nb_alt` landmarks using the farthest-node heuristic:
 *    - Find the node that has the maximum minimum distance from previous landmarks.
 *    - Recompute shortest-path distances using Dijkstra’s algorithm.
 *    - If no more landmarks can be choosen, break the loop and modify the configuration.
 * 6. If `save_alt` is enabled, store the precomputed landmark data.
 *
 * Mathematical Background:
 * - Landmark heuristics use the triangle inequality:
 *
 *     h(n) = max{ |d_L(goal) - d_L(n)| }
 *
 *   where:
 *   - `d_L(n)` is the precomputed distance from landmark `L` to node `n`.
 *
 * - The farthest-node landmark selection ensures a wide spread of landmarks,
 *   improving heuristic accuracy in large graphs.
 *
 * Edge Cases:
 * - If no valid landmarks are found, preprocessing stops early.
 * - If loading precomputed landmarks fails, it falls back to computation.
 *
 * @param gdata Reference to the graph data structure.
 * @param conf Reference to the configuration settings, which determine landmark count and storage options.
 *
 * @complexity
 * - Time Complexity: O(nb_alt * (E log V))
 *   - Each landmark selection involves a Dijkstra search → O(E log V).
 *   - `nb_alt` landmark selections are performed.
 * - Space Complexity: O(V * nb_alt)
 *   - Each node stores distances to all selected landmarks.
 */
void preprocessAlt(graph& gdata, config& conf)
{
    if (conf.save_alt)
    {
        if (loadAltData(gdata, conf)) {
            return;
        }
    }

    logger("selecting landmarks using farthest-node strategy");

    size_t n = gdata.node_to_index.size();

    std::vector<int> landmarks;
    landmarks.reserve(conf.nb_alt);
    std::vector<int> md(n, std::numeric_limits<int>::max());
    std::vector<int> d(n, -1);

    gdata.dist_landmark.resize(n, std::vector<int>(conf.nb_alt, -1));

    int fl = gdata.node_to_index.begin()->first;
    landmarks.push_back(fl);
    console("info", "processing landmark " + std::to_string(fl) + " (1/" + std::to_string(conf.nb_alt) + ")");
    logger("processing landmark " + std::to_string(fl) + " (1/" + std::to_string(conf.nb_alt) + ")");

    d = dijkstraSingleSource(gdata.adjacency, fl, n, gdata.node_to_index);
    {
        int i = 0;
        for (auto& kv : gdata.node_to_index) {
            size_t idx = kv.second;
            gdata.dist_landmark[idx][i] = d[idx];
            if (d[idx] >= 0 && d[idx] < md[idx])
                md[idx] = d[idx];
        }
    }

    for (int i = 1; i < conf.nb_alt; ++i) {
        int nl = -1;
        int mx = -1;

        for (auto& kv : gdata.node_to_index) {
            size_t idx = kv.second;
            if (md[idx] > mx) {
                mx = md[idx];
                nl = kv.first;
            }
        }

        if (nl == -1) {
            logger("warning: only selected " + std::to_string(landmarks.size()) + " landmarks as the number of requested landmarks exceeds the number of available nodes. setting new value inside the config.");
            console("warning", "only selected " + std::to_string(landmarks.size()) + " landmarks as the number of requested landmarks exceeds the number of available nodes. setting new value inside the config.");
            
            conf.nb_alt = landmarks.size();
            updateNbAlt(conf, conf.nb_alt);

            break;
        }

        landmarks.push_back(nl);
        console("info", "processing landmark " + std::to_string(nl) +
            " (" + std::to_string(i + 1) + "/" + std::to_string(conf.nb_alt) + ")");
        logger("processing landmark " + std::to_string(nl) +
            " (" + std::to_string(i + 1) + "/" + std::to_string(conf.nb_alt) + ")");

        d = dijkstraSingleSource(gdata.adjacency, nl, n, gdata.node_to_index);
        for (auto& kv : gdata.node_to_index) {
            size_t idx = kv.second;
            gdata.dist_landmark[idx][i] = d[idx];
            if (d[idx] >= 0 && d[idx] < md[idx])
                md[idx] = d[idx];
        }

        size_t selected_idx = gdata.node_to_index[nl];
        md[selected_idx] = -1;
    }

    console("success", "ALT pre-processing complete.");
    logger("ALT pre-processing complete.");

    if (conf.save_alt) {
        saveAltData(gdata, conf);
    }
}