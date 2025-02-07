// ✅ file verified.
#include "incl.h"
#include "decl.h"

static std::ofstream log_stream;  

// ✅ function + comment verified.
/**
 * @brief Initializes the logging system by opening a log file.
 *
 * @detailed
 * This function opens a log file for writing, enabling persistent logging of system messages.
 * If the file cannot be opened, an error is logged and the function returns `false`.
 *
 * Functionality:
 * - Opens the log file in append mode (`std::ios::out | std::ios::app`).
 * - Logs a success message if the file is successfully opened.
 * - Logs an error if the file cannot be accessed.
 *
 * Edge Cases Handled:
 * - If the log file is inaccessible (e.g., due to permission issues), logging is disabled.
 *
 * @param log_file_path Path to the log file.
 * @return `true` if logging was successfully initialized, `false` otherwise.
 *
 * @complexity
 * - Time Complexity: O(1) (Single file I/O operation)
 * - Space Complexity: O(1) (Only maintains a file stream)
 */
bool initLogger(const std::string& log_file_path) 
{
    log_stream.open(log_file_path, std::ios::out | std::ios::app);
    if (!log_stream.is_open()) {
        console("error", "could not open log file : " + log_file_path);
        logger("error: could not open log file : " + log_file_path);
        return false;
    }

    console("success", "logger initialized, log.txt : " + log_file_path);
    logger("---------- logger initialized ----------");
    return true;
}

// ✅ function + comment verified.
/**
 * @brief Closes the log file and stops logging.
 *
 * @detailed
 * This function ensures that the log file is properly closed before program termination.
 * It prevents resource leaks by closing the file stream when logging is no longer needed.
 *
 * Functionality:
 * - Checks if the log file is open.
 * - Logs a closing message before closing the file.
 *
 * Edge Cases Handled:
 * - If the log file is already closed, no action is taken.
 *
 * @complexity
 * - Time Complexity: O(1) (Single file stream operation)
 * - Space Complexity: O(1) (No extra memory allocated)
 */
void closeLogger() 
{
    if (log_stream.is_open()) {
        console("success", "stopping logging...");
        logger("---------- logger closing ----------");
        log_stream.close();
    }
}

// ✅ function + comment verified.
/**
 * @brief Logs a message to the log file with a timestamp.
 *
 * @detailed
 * This function writes a timestamped log entry to the log file, ensuring that messages
 * are recorded with their respective date and time.
 *
 * Functionality:
 * - Retrieves the current system time.
 * - Formats the timestamp as `[MM/DD/YY] [HH:MM:SS] > message`.
 * - Writes the formatted log entry to the log file.
 * - Flushes the file stream to ensure immediate write.
 *
 * Platform-Specific Handling:
 * - Uses `localtime_s()` on Windows.
 * - Uses `localtime_r()` on Linux/macOS.
 *
 * Edge Cases Handled:
 * - If the log file is not open, the function does nothing.
 * - If time retrieval fails, the function does nothing.
 *
 * @param message The log message to be written.
 *
 * @complexity
 * - Time Complexity: O(1) (Single file I/O operation)
 * - Space Complexity: O(1) (Minimal memory usage)
 */
void logger(const std::string& message) 
{
    if (!log_stream.is_open()) {
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm local_tm;

#if defined(_WIN32) || defined(_MSC_VER)
    if (localtime_s(&local_tm, &now) != 0) {
        return;
    }
#else
    if (!localtime_r(&now, &local_tm)) {
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

// ✅ function + comment verified.
/**
 * @brief Displays a message in the console with colored output based on message type.
 *
 * @detailed
 * This function formats and displays messages in the terminal using ANSI color codes
 * (if supported). It categorizes messages into different types: `success`, `warning`,
 * `error`, and `info`, assigning distinct colors to each.
 *
 * Functionality:
 * - Assigns ANSI color codes based on the message type.
 * - Converts the type string to uppercase for display consistency.
 * - Prints the formatted message in the format `[TYPE] message`.
 *
 * Message Type Colors:
 * - `success` → GREEN
 * - `warning` → YELLOW
 * - `error` → RED
 * - `info` → BLUE
 * - Unknown types default to a generic format.
 *
 * Edge Cases Handled:
 * - If the message type is unknown, it prints `[UNKNOW]` without color formatting.
 * - Works on terminals that support ANSI color sequences.
 *
 * @param type The message type (`success`, `warning`, `error`, `info`).
 * @param message The message to display.
 *
 * @complexity
 * - Time Complexity: O(1) (Basic string manipulation and console output)
 * - Space Complexity: O(1) (Minimal memory usage)
 */
void console(const std::string& type, const std::string& message) 
{
    std::string color;

    if (type == "success") { color = GREEN; }
    else if (type == "warning") { color = YELLOW; }
    else if (type == "error") { color = RED; }
    else if (type == "info") { color = BLUE; }
    else {
        std::cout << "  [UNKNOW] " << message << std::endl;
        return;
    }

    std::string type_upper = type;
    std::transform(type_upper.begin(), type_upper.end(), type_upper.begin(), ::toupper);

    std::cout << "  [" << color << type_upper << RESET << "] " << message << std::endl;

    return;
}
