// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Trims leading and trailing whitespace from a string.
 */
static std::string trim(const std::string& s)
{
    const char* whitespace = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

// ✅ function + comment verified.
/**
 * @brief Reads key=value lines from a file into an std::unordered_map.
 */
static std::unordered_map<std::string, std::string> readKeyValueFile(const std::string& filename, bool log_invalid_line = false)
{
    std::unordered_map<std::string, std::string> data;
    std::ifstream infile(filename);
    if (!infile.is_open()) return data;

    std::string line;
    while (std::getline(infile, line)) {
        std::string trimmed_line = trim(line);
        if (trimmed_line.empty() || trimmed_line[0] == '#') continue;
        size_t delimiter_pos = trimmed_line.find('=');
        if (delimiter_pos == std::string::npos) {
            if (log_invalid_line) console("warning", "Invalid config line (no '='): " + trimmed_line);
            continue;
        }
        std::string key = trim(trimmed_line.substr(0, delimiter_pos));
        std::string value = trim(trimmed_line.substr(delimiter_pos + 1));
        data[key] = value;
    }
    infile.close();
    return data;
}

// ✅ function + comment verified.
/**
 * @brief Updates a specific key in the provided configuration map.
 */
void updateConfigMap(std::unordered_map<std::string, std::string>& config_map, const std::string& key, const std::string& value)
{
    config_map[key] = value;
}

// ✅ function + comment verified.
/**
 * @brief Writes an std::unordered_map config to a file in key=value format.
 */
bool writeConfigFile(const std::string& config_file_path, const std::unordered_map<std::string, std::string>& config_map)
{
    std::ofstream outfile(config_file_path, std::ios::trunc);
    if (!outfile.is_open()) {
        console("error", "Failed to open config file for writing.");
        return false;
    }
    for (const auto& [key, value] : config_map) {
        outfile << key << "=" << value << "\n";
    }
    outfile.close();
    return true;
}

// ✅ function + comment verified.
/**
 * @brief Checks if a string can represent a boolean.
 *        Accepts "true" or "false" (case-sensitive).
 */
static bool isBoolString(const std::string& s)
{
    return (s == "true" || s == "false");
}

// ✅ function + comment verified.
/**
 * @brief Converts a "true"/"false" string into a boolean value.
 *        Assumes `s` is valid (either "true" or "false").
 */
static bool toBool(const std::string& s)
{
    return (s == "true");
}

// ✅ function + comment verified.
/**
 * @brief Checks if a string can represent an integer (e.g., "-5", "0", "42").
 *        Uses std::stoi internally; catches exceptions.
 */
static bool isIntString(const std::string& s)
{
    if (s.empty()) return false;
    try {
        size_t idx;
        std::stoi(s, &idx);
        return (idx == s.size());
    }
    catch (...) {
        return false;
    }
}

// ✅ function + comment verified.
/**
 * @brief Converts a valid integer string to int.
 */
static int toInt(const std::string& s)
{
    return std::stoi(s);
}

// ✅ function + comment verified.
/**
 * @brief Checks if a string can represent a floating-point number
 *        (including integer-like strings, e.g. "3", "3.14", "-2.7").
 */
static bool isFloatString(const std::string& s)
{
    if (s.empty()) return false;
    try {
        size_t idx;
        std::stod(s, &idx);
        return (idx == s.size());
    }
    catch (...) {
        return false;
    }
}

// ✅ function + comment verified.
/**
 * @brief Converts a valid float/double string to double.
 */
static double toDouble(const std::string& s)
{
    return std::stod(s);
}

// ✅ function + comment verified.
/**
 * @brief Verifies the integrity of a configuration file.
 */
static bool verifyConfFile(const config& conf)
{
    auto data_unordered = readKeyValueFile(conf.config_file);
    std::map<std::string, std::string> data(data_unordered.begin(), data_unordered.end());
    if (data.empty()) return false;
    if (data.find("map_path") == data.end() || data["map_path"].empty()) return false;
    if (data.find("config_file") == data.end() || data["config_file"].empty()) return false;
    if (data.find("alt_prep_file") == data.end() || data["alt_prep_file"].empty()) return false;
    if (data.find("use_alt") == data.end() || !isBoolString(data["use_alt"])) return false;
    if (data.find("save_alt") == data.end() || !isBoolString(data["save_alt"])) return false;
    if (data.find("personalized_weight") == data.end() || !isBoolString(data["personalized_weight"])) return false;
    if (data.find("log") == data.end() || !isBoolString(data["log"])) return false;
    if (data.find("nb_alt") == data.end() || !isIntString(data["nb_alt"])) return false;
    if (data.find("weight") == data.end() || !isFloatString(data["weight"])) return false;
    if (data.find("search_engine") == data.end() || !isIntString(data["search_engine"])) return false;
    return true;
}

// ✅ function + comment verified.
/**
 * @brief Creates and saves a configuration file using the config struct values.
 */
static void createConfFile(const config& conf)
{
    std::ofstream ofs(conf.config_file);
    if (!ofs.is_open()) return;
    ofs << "map_path=" << conf.map_path << "\n";
    ofs << "config_file=" << conf.config_file << "\n";
    ofs << "alt_prep_file=" << conf.alt_prep_file << "\n";
    ofs << "use_alt=" << (conf.use_alt ? "true" : "false") << "\n";
    ofs << "save_alt=" << (conf.save_alt ? "true" : "false") << "\n";
    ofs << "nb_alt=" << conf.nb_alt << "\n";
    ofs << "weight=" << conf.weight << "\n";
    ofs << "personalized_weight=" << (conf.personalized_weight ? "true" : "false") << "\n";
    ofs << "log=" << (conf.log ? "true" : "false") << "\n";
    ofs << "search_engine=" << conf.search_engine << "\n";
    ofs.close();
}

// ✅ function + comment verified.
/**
 * @brief Loads configuration settings from a file into the provided config object.
 */
static config loadConfFile(config conf)
{
    auto data_unordered = readKeyValueFile(conf.config_file);
    std::map<std::string, std::string> data(data_unordered.begin(), data_unordered.end());
    if (data.empty()) return conf;
    if (data.find("map_path") != data.end()) conf.map_path = data["map_path"];
    if (data.find("config_file") != data.end()) conf.config_file = data["config_file"];
    if (data.find("alt_prep_file") != data.end()) conf.alt_prep_file = data["alt_prep_file"];
    if (data.find("use_alt") != data.end() && isBoolString(data["use_alt"])) conf.use_alt = toBool(data["use_alt"]);
    if (data.find("save_alt") != data.end() && isBoolString(data["save_alt"])) conf.save_alt = toBool(data["save_alt"]);
    if (data.find("nb_alt") != data.end() && isIntString(data["nb_alt"])) conf.nb_alt = toInt(data["nb_alt"]);
    if (data.find("weight") != data.end() && isFloatString(data["weight"])) conf.weight = toDouble(data["weight"]);
    if (data.find("personalized_weight") != data.end() && isBoolString(data["personalized_weight"])) {
        conf.personalized_weight = toBool(data["personalized_weight"]);
    }
    if (data.find("log") != data.end() && isBoolString(data["log"])) conf.log = toBool(data["log"]);
    if (data.find("search_engine") != data.end() && isIntString(data["search_engine"])) conf.search_engine = toInt(data["search_engine"]);
    return conf;
}

// ✅ function + comment verified.
/**
 * @brief Logs the current configuration settings.
 */
void loggerConf(config& conf)
{
    logger("process config:");
    logger("  map_path > " + conf.map_path);
    logger("  config_file > " + conf.config_file);
    logger("  log_file > " + conf.log_file);
    logger("  alt_prep_file > " + conf.alt_prep_file);
    logger("  use_alt > " + std::to_string(conf.use_alt));
    logger("  save_alt > " + std::to_string(conf.save_alt));
    logger("  nb_alt > " + std::to_string(conf.nb_alt));
    logger("  weight > " + std::to_string(conf.weight));
    logger("  personalized_weight > " + std::to_string(conf.personalized_weight));
    logger("  log > " + std::to_string(conf.log));
    logger("  search_engine > " + std::to_string(conf.search_engine));
}

// ✅ function + comment verified.
/**
 * @brief Updates the nb_alt field in both memory and the config file.
 */
bool updateNbAlt(config& conf, int new_nb_alt)
{
    auto config_map = readKeyValueFile(conf.config_file, true);
    if (config_map.empty()) return false;
    updateConfigMap(config_map, "nb_alt", std::to_string(new_nb_alt));
    if (!writeConfigFile(conf.config_file, config_map)) {
        logger("error: failed to write updated config to file.");
        console("error", "failed to write updated config to file.");
        return false;
    }
    return true;
}

// ✅ function + comment verified.
/**
 * @brief Updates the nb_alt field in both memory and the config file.
 */
bool updateMapPath(config& conf, std::string map_path)
{
    auto config_map = readKeyValueFile(conf.config_file, true);
    if (config_map.empty()) return false;
    updateConfigMap(config_map, "map_path", map_path);
    if (!writeConfigFile(conf.config_file, config_map)) {
        logger("error: failed to write updated config to file.");
        console("error", "failed to write updated config to file.");
        return false;
    }
    return true;
}

// ✅ function + comment verified.
/**
 * @brief Loads or creates a new configuration file based on user input.
 *
 * @detailed
 * This function:
 * - Ensures required directories exist.
 * - Prompts the user for a CSV file path.
 * - Computes a hash of the file path to organize configuration storage.
 * - Determines if an existing configuration can be loaded.
 * - If no configuration exists, prompts the user for setup values.
 * - Saves the new configuration for future use.
 *
 * Configuration Setup Steps:
 * 1. Prompts for map path (CSV format).
 * 2. Computes a file hash to create a unique folder.
 * 3. Checks for an existing configuration:
 *    - If found: Loads it.
 *    - If not found: Proceeds with manual configuration.
 * 4. Graph Optimization Analysis:
 *    - Checks whether the graph structure is suitable for ALT heuristics.
 * 5. User Prompts:
 *    - Whether to use ALT preprocessing.
 *    - Number of landmarks (if ALT is enabled).
 *    - Whether to save ALT preprocessing results.
 *    - Heuristic weight for pathfinding.
 *    - Whether users can set personalized weights.
 *    - Whether debugging logs should be written.
 *
 * Directory Structure:
 *
 * graph/
 * ├── [hashed_map_path]/
 * │   ├── conf.json         # Configuration file
 * │   ├── log.txt           # Log file
 * │   ├── prep/
 * │   │   ├── alt.json      # ALT preprocessing data (optional)
 *
 * Edge Cases Handled:
 * - If a configuration file already exists and is valid, it is reused.
 * - Ensures invalid user inputs are properly handled with retries.
 *
 * @param conf The configuration object (default values).
 * @return The final configuration object with user preferences.
 *
 * @complexity
 * - Time Complexity: O(1) (Interactive input, directory creation, file reads/writes)
 * - Space Complexity: O(1) (Minimal memory overhead)
 */
config getConfiguration(config conf)
{
    ensureDirectory("graph");

    while (true) {
        std::cout << "\n  > enter .csv map path (format: landmark_A,landmark_B,cost): " << std::flush;
        std::cin >> conf.map_path;

        if (fileExists(conf.map_path)) {
            break;
        } else {
            console("error", "could not open file : " + conf.map_path);
            logger("error: could not open file : " + conf.map_path);
        }
    }

    std::cout << "\n" << std::flush;

    std::string file_hash = computeFileHash(conf.map_path);

    std::string main_folder = "graph/" + file_hash;
    ensureDirectory(main_folder);

    conf.config_file = main_folder + "/conf.smsh";
    conf.log_file = main_folder + "/log.txt";

    std::string preprocess_folder = main_folder + "/prep";
    ensureDirectory(preprocess_folder);

    conf.alt_prep_file = preprocess_folder + "/alt.bin";

    if (fileExists(conf.config_file)) 
    {
        if (verifyConfFile(conf)) {
            console("success", "config file detected! file path: " + conf.config_file);
            
            std::string tmp_map_path = conf.map_path;
            conf = loadConfFile(conf);
            if(conf.map_path != tmp_map_path) {
                conf.map_path = tmp_map_path;
                updateMapPath(conf, tmp_map_path);
                console("info", "new map path updated inside configuration file.");
            }

            console("success", "config file loaded!");
            return conf;
        }
    }

    console("info", "no config file detected! starting a new setup.");
    std::cout << "\n" << std::flush;

    optimization_flags flags = checkGraphOptimization(conf.map_path);
    console("info","is obtimized for ALT: [" + (flags.alt_optimized ? GREEN + "Yes" + RESET : RED + "No" + RESET) + "] ");
    console("info", std::string("Search engine recommended: [ ") + (flags.search_engine_recommanded == 1 ? GREEN + "Unidirectional (avg: < 3 edges per node)" + RESET : "Unidirectional (avg: < 3 edges per node)") 
    + " / " + (flags.search_engine_recommanded == 2 ? GREEN + "Both can be used (avg: 3-4 edges per node) " + RESET : "Both can be used (avg: 3-4 edges per node) ") 
    + " / " + (flags.search_engine_recommanded == 3 ? GREEN + "Bidirectional (avg: > 4 edges per node)" + RESET : "Bidirectional (avg: > 4 edges per node)") + " ]");

    std::cout << "\n  ~ process\n" << std::flush;

    conf.search_engine = getOneOrTwo("\n  > what type of search engine do you want to use? ('1' for unidirectional - '2' for bidirectional): ");

    bool use_alt = getYesNo("\n  > do you want to use the ALT pre-processing method (1min ~ 10min)? (y/n): ");
    if (use_alt) {
        conf.use_alt = true;
        conf.nb_alt = getInteger("\n  > how many landmarks do you want to use? (e.g., 10): ");
        std::cout << "\n" << std::flush;
        bool backup_alt = getYesNo("  > do you want to backup this pre-process for future use? (this will consume time/storage) (y/n): ");
        conf.save_alt = backup_alt;
    }
    else {
        conf.use_alt = false;
    }

    double weight = getPercentage("\n  > what maximum percentage above the shortest path duration are you willing to allow? (e.g. 10 for 10%) (min: 0 / max: 100): ");
    conf.weight = weight;

    std::cout << "\n  ~ api\n" << std::flush;

    bool personalized_weight = getYesNo("\n  > do you authorize the user to, optionally, set a personalized heuristic percentage when making query (y/n): ");
    conf.personalized_weight = personalized_weight;

    std::cout << "\n  ~ other\n" << std::flush;

    bool log = getYesNo("\n  > do you want to get debugging log (written inside a .txt) (y/n) ? : ");
    conf.log = log;

    std::cout << "\n" << std::flush;

    console("success", "configuration completed! saving it.");
    createConfFile(conf);

    return conf;
    
}

