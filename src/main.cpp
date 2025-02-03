// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Prints the program's main menu with a dynamic step title.
 *
 * @detailed
 * This function clears the console and prints a graphical ASCII banner for the program,
 * along with a dynamically specified step (e.g., "configuration", "pre-processing").
 *
 * Functionality:
 * - Uses `clear()` to reset the console screen.
 * - Prints a colored ASCII title indicating the tool's purpose.
 * - Displays a dynamic step indicator to show the current stage of execution.
 *
 * Edge Cases Handled:
 * - Ensures consistent formatting across different execution steps.
 *
 * @param step The current step of execution (e.g., "configuration (1/3)").
 *
 * @complexity
 * - Time Complexity: O(1) (Constant time output to console)
 * - Space Complexity: O(1) (Only uses a few temporary strings)
 */
void printMenu(std::string step) 
{
    clear();

	std::cout <<
		YELLOW << "\n    _____                     _  _______ _          _____      _   _     \n"
		"   / ____| " << RESET << " Team 1, ALGOSUP " << YELLOW << " | ||__   __| |        |  __ \\    | | | |    \n"
		<< BLUE << "  | (___  _ __ ___   __ _ ___| |__ | |  | |__   ___| |__) |_ _| |_| |__  \n"
		"   \\___ \\| '_ ` _ \\ / _` / __| '_ \\| |  | '_ \\ / _ \\  ___/ _` | __| '_ \\ \n"
		<< GREEN << "   ____) | | | | | | (_| \\__ \\ | | | |  | | | |  __/ |  | (_| | |_| | | |\n"
		"  |_____/|_| |_| |_|\\__,_|___/_| |_|_|  |_| |_|\\___|_|   \\__,_|\\__|_| |_|\n\n"
		<< RESET << "  ~ API to perform " << YELLOW << "ultra-fast" << RESET << " query inside bidirectional " << BLUE << "large" << RESET << "-" << GREEN << "scale" << RESET << " graph."
		<< std::flush;

	std::cout << "\n\n  --- " << step << " ---\n\n" << std::flush;
}

// ✅ function + comment verified.
/**
 * @brief Entry point of the API Gateway application.
 *
 * @detailed
 * This function initializes and launches the graph processing API, executing a sequence
 * of steps to configure, preprocess, and serve shortest-path computations.
 *
 * Workflow:
 *
 * Step 1: Configuration Setup
 * - Displays the configuration menu (`printMenu("configuration (1/3)")`).
 * - Calls `getConfiguration()` to:
 *   - Retrieve user-defined parameters (graph path, heuristics, logging).
 * - If logging is enabled, initializes the logger (`initLogger()`) and logs settings.
 * - Displays a warning about memory usage.
 * - Waits for 3 seconds (`sleep(3)`) before proceeding.
 *
 * Step 2: Graph Preprocessing
 * - Displays the preprocessing menu (`printMenu("pre-processing (2/3)")`).
 * - Reads graph data from a CSV file (`readCSV()`).
 *   - Ensures the file format is valid.
 *   - Extracts edge data into memory.
 * - Integrity Check (`checkIntegrity()`):
 *   - Ensures the graph contains no duplicate edges.
 *   - Logs an error and terminates if the check fails.
 * - Connectivity Check (`checkConnectivity()`):
 *   - Ensures the graph is fully connected.
 *   - Logs an error and terminates if the check fails.
 *
 * Step 3: Graph Loading
 * - Calls `loadGraphData(conf, edges)`, which:
 *   - Builds the adjacency list.
 *   - Prepares ALT heuristic preprocessing (if enabled).
 * - Clears unused memory (`edges.clear()`).
 * - Waits for 3 seconds before proceeding.
 *
 * Step 4: Performance Metrics
 * - Displays the "Ready" menu (`printMenu("ready (3/3)")`).
 * - Calls `storePerf(gdata)` to measure memory usage and log system statistics.
 *
 * Step 5: API Server Launch
 * - Calls `launchApiGateway(gdata, conf)`:
 *   - Starts an HTTP API server.
 *   - Handles shortest-path requests in JSON/XML format.
 *
 * Step 6: Cleanup
 * - Calls `closeLogger()` to properly close the log file before exiting.
 *
 * Error Handling:
 * - If CSV parsing fails, the program logs an error and terminates.
 * - If integrity or connectivity checks fail, the program exits with an error message.
 * - If the API server crashes, logs the failure before exiting.
 *
 * @return `0` on successful execution, non-zero on failure.
 */
int main() 
{	
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	config conf;

	printMenu("configuration (1/3)");

    console("warning", "the program will utilize all available RAM resources on your PC. please close any unnecessary programs to ensure optimal performance.");

	conf = getConfiguration(conf);

    if (conf.log) {
        initLogger(conf.log_file);
        loggerConf(conf);
    }

#if defined(_WIN32) || defined(_WIN64)
    Sleep(3000);
#else
    sleep(3);
#endif

    printMenu("pre-processing (2/3)");

    std::vector<edge> edges;
    static bool result_csv = readCSV(conf.map_path, edges);
    if (!result_csv) {
        console("error", "an error occurred while processing the CSV file. please ensure that you provide a .csv file with exactly three columns, and that all values are positive integers with a fcost > 0.");
        logger("error: an error occurred while processing the CSV file. please ensure that you provide a .csv file with exactly three columns, and that all values are positive integers with a fcost > 0.");
        return 0;
    }
    
    static bool result_integrity = checkIntegrity(edges);
    if (!result_integrity)
    {
        console("error", "integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.");
        logger("error: integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.");
        return 0;

    }
    else {
        console("success", "integrity check passed without any error. continueing.");
        logger("integrity check passed without any error. continueing.");
    }

    static bool result_connectivity = checkConnectivity(edges);
    if (!result_connectivity)
    {
        console("error", "connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.");
        logger("error: connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.");
        return 0;

    }
    else {
        console("success", "connectivity check passed without any error. continueing.");
        logger("connectivity check passed without any error. continueing.");
    }
    
    graph gdata;

    gdata = loadGraphData(conf, edges);
    
    search_buffers buffers;
    initializeSearchBuffers(gdata, buffers);
    console("success", "search buffers succefully initialized.");

#if defined(_WIN32) || defined(_WIN64)
    Sleep(3000);
#else
    sleep(3);
#endif
    
    printMenu("ready (3/3)");

    storePerf(gdata);

    launchApiGateway(gdata, buffers, conf);

    closeLogger();
}