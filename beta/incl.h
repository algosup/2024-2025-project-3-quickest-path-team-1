#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// main include

#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include <unordered_set>
#include <unordered_map>

#include <vector>
#include <queue>
#include <tuple>

#include <functional>
#include <algorithm>

#include <ctime>
#include <chrono>

#include <iomanip>

#include <limits>

// web include

#include <winsock2.h>
#include <ws2tcpip.h>

// librairy 

#pragma comment(lib, "Ws2_32.lib")

// structure class

struct union_find {
    std::vector<int> parent;
    std::vector<int> rank;

    void resize(size_t n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            parent[i] = static_cast<int>(i);
        }
    }

    int find_set(int v) {
        if (parent[v] == v) {
            return v;
        }
        parent[v] = find_set(parent[v]);
        return parent[v];
    }

    bool union_set(int a, int b) {
        int root_a = find_set(a);
        int root_b = find_set(b);
        if (root_a == root_b) {
            return false; // Ils sont déjà dans le même ensemble => potentiel "conflit" (dans un autre contexte)
        }
        if (rank[root_a] < rank[root_b]) {
            parent[root_a] = root_b;
        }
        else if (rank[root_a] > rank[root_b]) {
            parent[root_b] = root_a;
        }
        else {
            parent[root_b] = root_a;
            rank[root_a]++;
        }
        return true;
    }
};

struct graph_data {
    // Associe un ID de noeud à un index (pour le union_find)
    std::unordered_map<int, size_t> node_to_index;

    // On remplace std::vector<std::pair<int,int>> par un vecteur de triplets
    // (node_a, node_b, time_cost)
    std::vector<std::tuple<int, int, int>> edges;

    // Compteurs
    size_t line_count = 0;
    size_t index_count = 0;

    // Union-Find déjà existant
    union_find uf;

    // Nouvelle structure d’adjacence pour préparer des algos A* etc.
    // adjacency[node] = liste de (voisin, time_cost)
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjacency;
};

/**
 * @brief Représente une étape dans le chemin trouvé
 */
struct path_step {
    int node_a;
    int node_b;
    int time;
};

/**
 * @brief Représente le résultat final :
 *        - total_time: temps total du trajet
 *        - steps: liste des arêtes (node_a, node_b, time)
 */
struct path_result {
    int total_time;
    std::vector<path_step> steps;
};

/**
 * @brief Calcule le plus court chemin (ou approximé) entre start et end
 *        en utilisant A* ou un algo rapide (Dijkstra si heuristique=0).
 * @param gdata Le graphe (adjacency etc.)
 * @param start_node L’ID du noeud de départ
 * @param end_node   L’ID du noeud d’arrivée
 * @return path_result contenant le coût total et la liste des (A->B,time)
 */
path_result find_shortest_path_a_star(const graph_data& gdata, int start_node, int end_node);


// src include

//	api.cpp

int api_start(const graph_data& gdata);

//  loader.cpp

graph_data load_graph_data(const std::string& file_path);

//	connectivity.cpp

bool connectivity_validator(graph_data& gdata);

//	integrity.cpp


bool integrity_validator(const graph_data& gdata);

// log.cpp

bool init_logger(const std::string& log_file_path);
void close_logger();

inline void _logger(const std::string& message);