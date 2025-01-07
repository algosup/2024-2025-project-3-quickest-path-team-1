#include "incl.h"

bool integrity_validator(const graph_data& gdata) /* () check if their is no duplicate inside the memory */
{
    _logger("Starting integrity check (checking for duplicated edges A,B and B,A).");

    // hash table to stock all edges in form (A, B).
    std::unordered_set<uint64_t> edge_hash_set;
    edge_hash_set.reserve(gdata.edges.size());

    // encoding (a,b) in uint64_t (32 bits for a, 32 bits for b).
    auto encode_edge = [&](int a, int b) {
        uint64_t A = static_cast<uint64_t>(static_cast<uint32_t>(a));
        uint64_t B = static_cast<uint64_t>(static_cast<uint32_t>(b));
        return (A << 32) | B;
        };

    size_t count = 0;
    for (const auto& edge : gdata.edges) 
    {
        count++;
        // edge is now a triplet (A, B, time_cost).
        int A, B, time_cost;
        std::tie(A, B, time_cost) = edge;

        // verification : if (B,A) is already here => conflict.
        uint64_t reversed_code = encode_edge(B, A);
        if (edge_hash_set.find(reversed_code) != edge_hash_set.end()) {
            std::cout << "[ERROR] integrity conflict : edge (" + std::to_string(A) + "," + std::to_string(B)
                + ") conflicts with (" + std::to_string(B) + "," + std::to_string(A) + ")." << std::endl;
            _logger("Integrity error: edge (" + std::to_string(A) + "," + std::to_string(B)
                + ") conflicts with (" + std::to_string(B) + "," + std::to_string(A) + ").");
            return false;
        }

        // else we insert (A,B).
        uint64_t edge_code = encode_edge(A, B);
        edge_hash_set.insert(edge_code);

        // progression log.
        if (count % 1'000'000 == 0) {
            _logger("Checked " + std::to_string(count) + " edges for integrity...");
        }
    }

    _logger("Integrity check completed. No duplicate edges found.");
    return true;
}
