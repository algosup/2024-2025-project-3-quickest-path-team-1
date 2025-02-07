#ifndef INCL_H
#define INCL_H

/*------------------------------------------------------------------------------------
							definition.
------------------------------------------------------------------------------------*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

/*------------------------------------------------------------------------------------
							all include.
------------------------------------------------------------------------------------*/

 // ===============================
 // Standard Input/Output & Strings
 // ===============================

#include <iostream>      // Standard I/O (std::cout, std::cin)
#include <string>        // String manipulation (std::string)
#include <iomanip>       // I/O formatting (std::setw, std::setprecision)
#include <stdexcept>     // Exception handling (std::runtime_error, std::invalid_argument)
#include <utility>       // Utility functions (std::pair, std::move)

// ===============================
// File and String Streams
// ===============================

#include <fstream>       // File stream operations (std::ifstream, std::ofstream)
#include <sstream>       // String stream operations (std::stringstream)
#include <filesystem>    // File system utilities (std::filesystem::path, directory operations)

// ===============================
// Numerical Operations & Limits
// ===============================

#include <limits>        // Numeric limits (std::numeric_limits<T>)

// ===============================
// Multi-threading & Synchronization
// ===============================

#include <thread>             // Thread management (std::thread)
#include <atomic>             // Atomic operations (std::atomic)
#include <mutex>              // Mutex for synchronization (std::mutex, std::lock_guard)
#include <condition_variable> // Thread condition synchronization (std::condition_variable)

// ===============================
// Data Structures & Algorithms
// ===============================

#include <vector>          // Dynamic arrays (std::vector)
#include <unordered_set>   // Hash-based set (std::unordered_set)
#include <unordered_map>   // Hash-based map (std::unordered_map)
#include <queue>           // Queue and priority queue (std::queue, std::priority_queue)
#include <set>             // Ordered set (std::set)
#include <stack>           // Stack (std::stack)
#include <tuple>           // Tuples (std::tuple)
#include <algorithm>       // Standard algorithms (std::sort, std::find, etc.)
#include <map>			   // Ordered associative containers (std::map, std::multimap)
#include <memory>          // Smart pointers (std::unique_ptr, std::shared_ptr)

// ===============================
// Networking (Cross-Platform)
// ===============================

#ifdef _WIN32
#include <winsock2.h>      // Windows Sockets API
#include <ws2tcpip.h>      // Additional network functions
#ifdef _MSC_VER
    #pragma comment(lib, "ws2_32.lib")
#endif
#else
#include <sys/types.h>      // Type definitions for system calls
#include <sys/socket.h>     // Socket programming (socket(), bind(), listen())
#include <netinet/in.h>     // Internet address family (struct sockaddr_in)
#include <arpa/inet.h>      // Address conversion functions (inet_pton, inet_ntop)
#include <unistd.h>         // POSIX API (read(), write(), close())
#include <cstring>          // C-string functions (memset, strcpy, etc.)
#include <cerrno>           // Error handling (errno, strerror)

#define INVALID_SOCKET -1   // Define invalid socket for UNIX
#define SOCKET_ERROR -1     // Define socket error for UNIX
#define closesocket close   // Map Windows closesocket to UNIX close()
typedef int SOCKET;         // Define socket type for UNIX
#endif

// ===============================
// Platform-Specific System Info 
// (not from STL librairy.)
// (Used only in perf.cpp for debugging purposes; independent of the main code and can be removed at any time without impact.)
// ===============================

#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS
#include <windows.h>   // Windows API functions
#include <psapi.h>     // Process status API for memory usage

#elif defined(__APPLE__) && defined(__MACH__)
#define IS_MACOS
#include <sys/types.h>   // Data type definitions
#include <sys/sysctl.h>  // System control for hardware info
#include <mach/mach.h>   // Mach API for memory usage

#elif defined(__linux__)
#define IS_LINUX
#include <sys/sysinfo.h>  // System information (RAM, uptime, etc.)
#include <sys/resource.h> // Process resource usage
#include <unistd.h>       // Standard symbolic constants and types
#endif


#endif //INCL_H