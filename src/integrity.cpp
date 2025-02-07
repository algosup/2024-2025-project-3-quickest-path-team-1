// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Verifies the integrity of a graph by checking for duplicate edges.
 *
 * @detailed
 * This function ensures that all edges in the graph are unique by tracking them
 * using a hash set. If a duplicate edge is detected, an error is logged, and
 * the function returns `false`.
 *
 * Functionality:
 * - Iterates through the provided edge list.
 * - Encodes each edge `(A, B)` into a unique identifier using a 64-bit integer:
 *
 *   edge_id = (min(A, B) << 32) | max(A, B)
 *
 *   - This ensures that `(A, B)` and `(B, A)` are treated as the same edge.
 * - Uses an unordered set (`std::unordered_set<uint64_t>`) to track unique edges.
 * - Detects duplicates:
 *   - If an encoded edge is already present in the set, returns `false`.
 * - Returns `true` if all edges are unique.
 *
 * Edge Case Handling:
 * - If the graph has no edges, it is considered valid, returns `true`.
 *
 * Data Structure:
 * - The unordered set provides constant-time lookups (`O(1)`) for edge uniqueness checking.
 *
 * @param edges Reference to the vector of edges representing the graph.
 * @return `true` if no duplicate edges are found, otherwise `false`.
 *
 * @complexity
 * - Time Complexity: O(E) (Each edge is processed once and stored in a hash set).
 * - Space Complexity: O(E) (All unique edges are stored in memory).
 */
bool checkIntegrity(const std::vector<edge>& edges)
{
    console("info", "integrity check launched.");
    logger("integrity check launched.");

    std::unordered_set<uint64_t> unique_edges;
    unique_edges.reserve(edges.size());

    for (const auto& e : edges) {
        int u = e.a;
        int v = e.b;

        if (u > v) {
            std::swap(u, v);
        }

        uint64_t encoded = (static_cast<uint64_t>(u) << 32) | static_cast<uint64_t>(v);

        if (!unique_edges.insert(encoded).second) {
            logger("error: duplicate edge detected between nodes: " + std::to_string(u) + " and " + std::to_string(v));
            return false;
        }
    }

    return true;
}

