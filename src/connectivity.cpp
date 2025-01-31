// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Determines whether the graph is fully connected.
 *
 * @detailed
 * This function checks if all nodes in the graph belong to a single connected component
 * using the Union-Find (Disjoint Set Union - DSU) algorithm. A graph is considered fully
 * connected if there exists a path between every pair of nodes.
 *
 * Functionality:
 * - Iterates through the edge list to determine the highest node ID (`max_node_id`).
 * - Initializes the Union-Find structure for efficient connectivity checks.
 * - Processes each edge, merging connected nodes using path compression and rank-based merging.
 * - Tracks used nodes to ensure all relevant nodes are considered.
 * - Determines connectivity:
 *   - Finds the root node of the first encountered component.
 *   - Checks if all used nodes share the same root.
 *   - If any node belongs to a different component, the graph is disconnected.
 *
 * Edge Case Handling:
 * - If there are no edges, considers the graph trivially connected returns `true`.
 * - If there are no nodes, the graph is also considered connected, returns `true`.
 * - If multiple disconnected components exist, the graph is considers unconnected, returns `false`.
 *
 * Algorithm: Union-Find (Disjoint Set)
 * - Uses Path Compression for efficient root retrieval (`findp(x)`).
 * - Implements Union by Rank to optimize merging (`unite(x, y)`).
 *
 * @param edges Reference to the vector of edges representing the graph.
 * @return `true` if the graph is fully connected, otherwise `false`.
 *
 * @complexity
 * - Time Complexity: O(E log V) (Union-Find with path compression runs in nearly constant time).
 * - Space Complexity: O(V) (Stores node parents, ranks, and connectivity status).
 */
bool checkConnectivity(const std::vector<edge>& edges)
{
    console("info", "connectivity check launched.");
    logger("connectivity check launched.");

    if (edges.empty()) {
        logger("no edges found in the data. graph is considered connected.");
        return true;
    }

    int max_node_id = -1;
    for (const auto& edge : edges) {
        if (edge.a > max_node_id) max_node_id = edge.a;
        if (edge.b > max_node_id) max_node_id = edge.b;
    }

    if (max_node_id < 0) {
        logger("no edges found in the data. graph is considered connected.");
        return true;
    }

    std::vector<int> parent(max_node_id + 1);
    std::vector<int> rank(max_node_id + 1, 0);
    std::vector<bool> used(max_node_id + 1, false);

    for (int i = 0; i <= max_node_id; ++i) parent[i] = i;

    auto findp = [&](int x) -> int {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
        };

    auto unite = [&](int x, int y) {
        x = findp(x);
        y = findp(y);
        if (x == y) return;
        if (rank[x] < rank[y]) std::swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) ++rank[x];
        };

    for (const auto& edge : edges) {
        int a = edge.a;
        int b = edge.b;
        used[a] = true;
        used[b] = true;
        unite(a, b);
    }

    int root = -1;
    size_t count_used = 0;
    for (int i = 0; i <= max_node_id; ++i) {
        if (used[i]) {
            if (root < 0) {
                root = findp(i);
            }
            else if (findp(i) != root) {
                return false;
            }
            ++count_used;
        }
    }

    if (count_used == 0) {
        logger("no edges found in the data. graph is considered connected.");
        return true;
    }

    return true;
}
