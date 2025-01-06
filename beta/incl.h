#pragma once

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

#include <functional>

#include <ctime>

#include <iomanip>

// web include

#include <winsock2.h>
#include <ws2tcpip.h>

// librairy 

#pragma comment(lib, "Ws2_32.lib")

// src include

//	api.cpp

int api_start();

//	connectivity.cpp

bool connectivity_validator(const std::string& file_path);

//	integrity.cpp

bool integrity_validator(const std::string& file_path);

// log.cpp

bool init_logger(const std::string& log_file_path);
void close_logger();

inline void _logger(const std::string& message);

// common class

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    UnionFind() = default;
    UnionFind(size_t n) : parent(n), rank(n, 0) {
        for (size_t i = 0; i < n; ++i) {
            parent[i] = static_cast<int>(i);
        }
    }
    // Find with path compression
    int find_set(int v) {
        if (parent[v] != v) {
            parent[v] = find_set(parent[v]);
        }
        return parent[v];
    }
    // Union by rank
    bool union_set(int a, int b) {
        int rootA = find_set(a);
        int rootB = find_set(b);
        if (rootA == rootB) {
            return false;  // They are already in the same set
        }
        if (rank[rootA] < rank[rootB]) {
            parent[rootA] = rootB;
        }
        else if (rank[rootA] > rank[rootB]) {
            parent[rootB] = rootA;
        }
        else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }
        return true;
    }
    // Size of underlying storage
    size_t size() const {
        return parent.size();
    }
    // Initialize or resize
    void resize(size_t n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            parent[i] = static_cast<int>(i);
        }
    }
};