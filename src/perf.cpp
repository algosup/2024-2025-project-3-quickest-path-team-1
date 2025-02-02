﻿// ✅ file verified.

/**
 * @brief Performance monitoring utilities 
 *
 * This file provides platform-specific system monitoring functions to retrieve
 * performance metrics such as memory usage and system resource utilization.
 *
 * Why use platform-specific APIs?
 * - The STL does not provide built-in system monitoring.
 * - We rely on system SDKs to access low-level OS functionalities.
 * - To maintain cross-platform compatibility, we use conditional compilation
 *   (`#ifdef _WIN32`, `#ifdef __linux__`, etc.), ensuring that each OS uses
 *   its native API.
 *
 * Platform Implementations:
 * - Windows (`windows.h`, `psapi.h`) → Process memory and performance data.
 * - Linux (`sys/sysinfo.h`, `sys/resource.h`, `unistd.h`) → RAM, CPU load, uptime.
 * - macOS (`sys/sysctl.h`, `mach/mach.h`) → Mach kernel APIs for resource tracking.
 *
 * Note: This file is independent of the main codebase and serves only for debugging.
 * It can be removed at any time without affecting core functionality.
 */

#include "incl.h"
#include "decl.h"

size_t estimateUnorderedMapIntSizeT(const std::unordered_map<int, size_t>& m) {
    size_t total = sizeof(m);
    total += m.bucket_count() * sizeof(void*);
    total += m.size() * (sizeof(std::pair<const int, size_t>) + sizeof(void*));
    return total;
}

template <typename T>
size_t estimateVector1d(const std::vector<T>& v) {
    return sizeof(v) + (v.capacity() * sizeof(T));
}

template <typename T>
size_t estimateVector2d(const std::vector<std::vector<T>>& vec2d) {
    size_t total = sizeof(vec2d)
        + (vec2d.capacity() * sizeof(std::vector<T>));
    for (auto& sub : vec2d) {
        total += sizeof(sub) + (sub.capacity() * sizeof(T));
    }
    return total;
}

size_t estimateVector2dPairIntInt(const std::vector<std::vector<std::pair<int, int>>>& vec2d) {
    size_t total = sizeof(vec2d)
        + (vec2d.capacity() * sizeof(std::vector<std::pair<int, int>>));
    for (auto& sub : vec2d) {
        total += sizeof(sub) + (sub.capacity() * sizeof(std::pair<int, int>));
    }
    return total;
}

size_t estimateTotalGraphMemory(const graph& g) {
    size_t total = 0;
    total += sizeof(g.line_count);
    total += sizeof(g.index_count);
    total += estimateUnorderedMapIntSizeT(g.node_to_index);
    total += estimateVector1d(g.index_to_node);
    total += estimateVector2dPairIntInt(g.adjacency);
    total += estimateVector2d(g.dist_landmark_to);
    total += estimateVector2d(g.dist_landmark_from);
    return total;
}

#if defined(IS_WINDOWS)

void getSystemMemory(double& total_ram_mb, double& free_ram_mb) {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        total_ram_mb = static_cast<double>(statex.ullTotalPhys) / 1048576.0;
        free_ram_mb = static_cast<double>(statex.ullAvailPhys) / 1048576.0;
    }
    else {
        total_ram_mb = 0.0;
        free_ram_mb = 0.0;
    }
}

double getProcessMemory() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
        reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
        sizeof(pmc))) {
        return static_cast<double>(pmc.WorkingSetSize) / 1048576.0;
    }
    return 0.0;
}

#elif defined(IS_MACOS)

#include <mach/mach.h>

void getSystemMemory(double& total_ram_mb, double& free_ram_mb) {
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    int64_t total_ram = 0;
    size_t length = sizeof(total_ram);
    if (sysctl(mib, 2, &total_ram, &length, NULL, 0) == 0) {
        total_ram_mb = static_cast<double>(total_ram) / 1048576.0;
    }
    else {
        total_ram_mb = 0.0;
    }

    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
#if defined(HOST_VM_INFO64_COUNT)
    vm_statistics64_data_t vmstat64;
    if (host_statistics64(mach_host_self(),
        HOST_VM_INFO64,
        reinterpret_cast<host_info_t>(&vmstat64),
        &count) == KERN_SUCCESS) {
        int64_t free_memory = static_cast<int64_t>(vmstat64.free_count)
            * static_cast<int64_t>(sysconf(_SC_PAGESIZE));
        free_ram_mb = static_cast<double>(free_memory) / 1048576.0;
    }
    else {
        free_ram_mb = 0.0;
    }
#else
    vm_statistics_data_t vmstat;
    if (host_statistics(mach_host_self(),
        HOST_VM_INFO,
        reinterpret_cast<host_info_t>(&vmstat),
        &count) == KERN_SUCCESS) {
        int64_t free_memory = static_cast<int64_t>(vmstat.free_count)
            * static_cast<int64_t>(sysconf(_SC_PAGESIZE));
        free_ram_mb = static_cast<double>(free_memory) / 1048576.0;
    }
    else {
        free_ram_mb = 0.0;
    }
#endif
}

double getProcessMemory() {
    mach_task_basic_info info;
    mach_msg_type_number_t info_count = MACH_TASK_BASIC_INFO_COUNT;
#if defined(MACH_TASK_BASIC_INFO_64)
    if (task_info(mach_task_self(),
        MACH_TASK_BASIC_INFO_64,
        reinterpret_cast<task_info_t>(&info),
        &info_count) == KERN_SUCCESS) {
        return static_cast<double>(info.resident_size) / 1048576.0;
    }
#else
    if (task_info(mach_task_self(),
        MACH_TASK_BASIC_INFO,
        reinterpret_cast<task_info_t>(&info),
        &info_count) == KERN_SUCCESS) {
        return static_cast<double>(info.resident_size) / 1048576.0;
    }
#endif
    return 0.0;
}

#elif defined(IS_LINUX)

void getSystemMemory(double& total_ram_mb, double& free_ram_mb) {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) == 0) {
        total_ram_mb =
            static_cast<double>(sys_info.totalram) * sys_info.mem_unit / 1048576.0;
        free_ram_mb =
            static_cast<double>(sys_info.freeram) * sys_info.mem_unit / 1048576.0;
    }
    else {
        total_ram_mb = 0.0;
        free_ram_mb = 0.0;
    }
}

double getProcessMemory() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return static_cast<double>(usage.ru_maxrss) / 1024.0;
    }
    return 0.0;
}

#endif

// ✅ function + comment verified.
/**
 * @brief Logs memory usage statistics for a given graph structure.
 *
 * @detailed
 * This function analyzes and logs the memory consumption of various components
 * of the `graph` structure. It helps in understanding the memory footprint of
 * the graph and its effect on system resources.
 *
 * Functionality:
 * - Estimates memory usage for different graph components.
 * - Retrieves system memory statistics.
 * - Logs detailed memory consumption metrics.
 *
 * Memory Estimation Functions:
 * - Graph Components:
 *   - `estimateUnorderedMapIntSizeT(g.node_to_index)`: Estimates memory for the node-to-index mapping.
 *   - `estimateVector1d(g.index_to_node)`: Estimates memory for the index-to-node vector.
 *   - `estimateVector2d(g.dist_landmark_to)`: Computes memory for landmark-to-node distances.
 *   - `estimateVector2d(g.dist_landmark_from)`: Computes memory for node-to-landmark distances.
 *   - `estimateVector2dPairIntInt(g.adjacency)`: Estimates memory for the adjacency list.
 *   - `estimateTotalGraphMemory(g)`: Computes the total memory used by the graph.
 *
 * - System Memory Analysis:
 *   - `getSystemMemory(total_ram_mb, free_ram_mb)`: Retrieves total and free system RAM.
 *   - `getProcessMemory()`: Retrieves the process's memory usage.
 *
 * Logged Memory Statistics:
 * - Memory Usage by Graph Components (in MB):
 *   - `node_to_index` (Hash map for node indexing).
 *   - `index_to_node` (Reverse lookup vector).
 *   - `adjacency` (Adjacency list representation).
 *   - `dist_landmark_to` (Precomputed distances to landmarks).
 *   - `dist_landmark_from` (Precomputed distances from landmarks).
 *   - Total Estimated Graph Memory.
 *
 * - System Memory Statistics (in MB):
 *   - Total System RAM.
 *   - Free System RAM.
 *   - Process Memory Usage.
 *
 * - Comparison of Graph Memory vs. System Memory:
 *   - Graph Memory Usage as a Percentage of Total System RAM:
 *
 *     `graph_memory_percentage = (Total Graph Memory / Total System RAM) * 100`
 *
 *   - Process Memory Usage as a Percentage of Available RAM:
 *
 *     `process_memory_percentage = (Process Memory / Remaining RAM) * 100`
 *
 * Use Cases:
 * - Helps profile memory consumption for large graphs.
 * - Assists in optimizing graph data structures.
 * - Identifies potential memory bottlenecks.
 *
 * Edge Cases Handled:
 * - If system memory retrieval fails, it logs `0.0 MB` values.
 * - Prevents division by zero when computing percentages.
 *
 * @param g Reference to the `graph` structure.
 *
 * @complexity
 * - Time Complexity: O(N + E) (Processes each graph component once).
 * - Space Complexity: O(1) (Only temporary calculations, no additional memory allocations).
 */

void storePerf(const graph& g) {
    std::ostringstream output_stream;
    output_stream << std::fixed << std::setprecision(6);

    double mem_node_to_index_mb =
        static_cast<double>(estimateUnorderedMapIntSizeT(g.node_to_index)) / 1048576.0;

    double mem_index_to_node_mb =
        static_cast<double>(estimateVector1d(g.index_to_node)) / 1048576.0;

    double mem_adjacency_mb =
        static_cast<double>(estimateVector2dPairIntInt(g.adjacency)) / 1048576.0;

    double mem_dist_landmark_to_mb =
        static_cast<double>(estimateVector2d(g.dist_landmark_to)) / 1048576.0;

    double mem_dist_landmark_from_mb =
        static_cast<double>(estimateVector2d(g.dist_landmark_from)) / 1048576.0;

    output_stream.str("");
    output_stream.clear();
    output_stream << "memory usage by members (approx):";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  node_to_index: " << mem_node_to_index_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  index_to_node: " << mem_index_to_node_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  adjacency: " << mem_adjacency_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  dist_landmark_to: " << mem_dist_landmark_to_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  dist_landmark_from: " << mem_dist_landmark_from_mb << " MB";
    logger(output_stream.str());

    double total_graph_mem_mb =
        static_cast<double>(estimateTotalGraphMemory(g)) / 1048576.0;

    output_stream.str("");
    output_stream.clear();
    output_stream << "total estimated graph memory: " << total_graph_mem_mb << " MB";
    logger(output_stream.str());

    double total_system_ram_mb = 0.0, free_system_ram_mb = 0.0;
    double process_mem_mb = 0.0;

#if defined(IS_WINDOWS) || defined(IS_MACOS) || defined(IS_LINUX)
    getSystemMemory(total_system_ram_mb, free_system_ram_mb);
    process_mem_mb = getProcessMemory();
#endif

    output_stream.str("");
    output_stream.clear();
    output_stream << "system memory info:";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  total system RAM: " << total_system_ram_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  free system RAM: " << free_system_ram_mb << " MB";
    logger(output_stream.str());

    output_stream.str("");
    output_stream.clear();
    output_stream << "  process memory usage: " << process_mem_mb << " MB";
    logger(output_stream.str());

    double graph_vs_system_perc =
        (total_system_ram_mb > 0.0)
        ? (total_graph_mem_mb / total_system_ram_mb) * 100.0
        : 0.0;

    output_stream.str("");
    output_stream.clear();
    output_stream << "the graph uses approximately " << graph_vs_system_perc
        << "% of the total system RAM.";
    logger(output_stream.str());

    double process_vs_remaining_perc = 0.0;
    double other_mem_mb = total_system_ram_mb - process_mem_mb;
    if (other_mem_mb > 0.0) {
        process_vs_remaining_perc = (process_mem_mb / other_mem_mb) * 100.0;
    }

    output_stream.str("");
    output_stream.clear();
    output_stream << "the process uses approximately " << process_vs_remaining_perc
        << "% of the RAM not used by other processes.";
    logger(output_stream.str());
}
