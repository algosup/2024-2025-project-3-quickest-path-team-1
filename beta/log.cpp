#include "incl.h"

static std::ofstream log_stream;

bool init_logger(const std::string& log_file_path) {
    log_stream.open(log_file_path, std::ios::out | std::ios::app);
    if (!log_stream.is_open()) {
        std::cerr << "Error: Could not open log file: " << log_file_path << std::endl;
        return false;
    }
    
    _logger("Logging started.");
    
    return true;
}

void close_logger() {
    if (log_stream.is_open()) {
        _logger("Logging finished.");
        log_stream.close();
    }
}

void _logger(const std::string& message) {
    if (!log_stream.is_open()) {
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm local_tm;

#if defined(_WIN32) || defined(_MSC_VER)
    if (localtime_s(&local_tm, &now) != 0) {
        log_stream << "[Error] Failed to get local time." << std::endl;
        return;
    }
#else
    if (!localtime_r(&now, &local_tm)) {
        log_stream << "[Error] Failed to get local time." << std::endl;
        return;
    }
#endif

    log_stream << "["
        << std::put_time(&local_tm, "%m/%d/%y")
        << "] ["
        << std::put_time(&local_tm, "%H:%M:%S")
        << "] > "
        << message
        << std::endl;

    log_stream.flush();
}
