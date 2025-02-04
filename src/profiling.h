// ✅ file verified.
#ifndef PROFILING_H
#define PROFILING_H

#include "incl.h"


#ifdef ENABLE_PROFILING

class profiler {
public:
    using clock = std::chrono::high_resolution_clock;

    static profiler& getInstance() {
        static profiler instance;
        return instance;
    }

    void start(const std::string& section) {
        auto now = clock::now();
        std::lock_guard<std::mutex> lock(mutex_);
        start_times_[section] = now;
    }

    void stop(const std::string& section) {
        auto now = clock::now();
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = start_times_.find(section);
        if (it != start_times_.end()) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - it->second).count();
            durations_[section] += duration;
            counts_[section] += 1;
            start_times_.erase(it);
        }
    }

    void report() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "\n\n  --- profiling report ---\n" << std::flush;
        for (const auto& [section, duration] : durations_) {
            std::cout << "  " << section << ": " << duration << " micro seconds over "
                << counts_.at(section) << " calls.\n" << std::flush;
        }
        std::cout << "  ------------------------\n" << std::flush;

        durations_.clear();
        counts_.clear();
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, clock::time_point> start_times_;
    std::unordered_map<std::string, long long> durations_;
    std::unordered_map<std::string, int> counts_;

    profiler() {}
    ~profiler() {}
    profiler(const profiler&) = delete;
    profiler& operator=(const profiler&) = delete;
};

#define PROFILE_START(section) profiler::getInstance().start(section)
#define PROFILE_STOP(section)  profiler::getInstance().stop(section)
#define PROFILE_REPORT()      profiler::getInstance().report()

#else

#define PROFILE_START(section)
#define PROFILE_STOP(section)
#define PROFILE_REPORT()

#endif 

#endif