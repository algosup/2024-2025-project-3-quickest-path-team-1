// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Reads graph edge data from a CSV file into memory.
 *
 * @detailed
 * This function processes a CSV file containing graph edge data and extracts
 * valid edges for graph construction. The CSV file must contain three columns:
 *
 *     node_a, node_b, time_cost
 *
 * where:
 * - `node_a` and `node_b` are unique node identifiers (strictly positive integers).
 * - `time_cost` represents the travel cost between `node_a` and `node_b` (must be ≥ 1).
 *
 * Functionality:
 * - Validates that the input file has a `.csv` extension.
 * - Opens and reads the file line by line.
 * - Splits each line into three columns (nodes and cost).
 * - Trims whitespace and validates that values are strictly positive integers.
 * - Logs and skips malformed or invalid lines.
 * - Converts valid data into edge structures and stores them in memory.
 *
 * Processing Steps:
 * - Validate the file extension and attempt to open the file.
 * - Read and parse each line, ensuring it contains three valid columns.
 * - Trim and verify integer format for each column.
 * - Convert valid data into edges and store them in the provided vector.
 * - Log processing statistics including total lines read and valid edges loaded.
 *
 * Edge Case Handling:
 * - If the file cannot be opened, logs an error and returns `false`.
 * - If a line contains non-integer values or is malformed, it is logged and skipped.
 * - If an edge contains invalid values (negative or zero nodes, non-positive cost), it is skipped.
 * - Ensures memory efficiency by processing data in-place.
 *
 * @param map_path Path to the CSV file containing graph edge data.
 * @param edges Reference to the vector where extracted edges will be stored.
 * @return `true` if the CSV file was successfully read and processed, otherwise `false`.
 *
 * @complexity
 * - Time Complexity: O(L) (Processes `L` lines in the file).
 * - Space Complexity: O(E) (Stores `E` valid edges in memory).
 */
bool readCSV(const std::string& map_path, std::vector<edge>& edges)
{
    if (map_path.size() < 4 || map_path.compare(map_path.size() - 4, 4, ".csv") != 0) {
        console("error", "file is not a .csv file: " + map_path);
        logger("error: file is not a .csv file: " + map_path);
        return false;
    }

    console("info", "reading CSV data into memory.");
    logger("reading CSV data into memory.");

    std::ifstream file(map_path, std::ios::binary);
    if (!file.is_open()) {
        console("error", "could not open file: " + map_path);
        logger("error: could not open file: " + map_path);
        return false;
    }

    std::string line;
    size_t line_num = 0;
    size_t valid_lines = 0;

    std::vector<std::string> columns;
    columns.reserve(3);

    auto trim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
        };

    auto isStrictPositiveIntegerString = [&](const std::string& s) -> bool {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
        };

    while (std::getline(file, line)) {
        line_num++;

        if (line.empty()) {
            continue;
        }

        columns.clear();
        {
            size_t start = 0;
            for (int i = 0; i < 2; ++i) {
                size_t pos = line.find(',', start);
                if (pos == std::string::npos) {
                    break;
                }
                columns.emplace_back(line.substr(start, pos - start));
                start = pos + 1;
            }
            if (!line.empty()) {
                columns.emplace_back(line.substr(start));
            }
        }

        if (columns.size() != 3) {
            console("warning",
                "invalid CSV format at line " + std::to_string(line_num)
                + ": expected 3 columns, got " + std::to_string(columns.size()) + " - skipping line.");
            logger("warning: invalid CSV format at line " + std::to_string(line_num)
                + ": expected 3 columns, got " + std::to_string(columns.size()) + " - skipping line.");
            continue;
        }

        for (auto& col : columns) {
            trim(col);
        }

        if (!isStrictPositiveIntegerString(columns[0]) ||
            !isStrictPositiveIntegerString(columns[1]) ||
            !isStrictPositiveIntegerString(columns[2]))
        {
            console("warning",
                "invalid integer data at line " + std::to_string(line_num)
                + " - skipping line.");
            logger("warning: invalid integer data at line " + std::to_string(line_num)
                + " - non-integer or negative values in columns: [ "
                + columns[0] + ", " + columns[1] + ", " + columns[2] + " ] - skipping line.");
            continue;
        }

        int a, b, cost;
        try {
            a = std::stoi(columns[0]);
            b = std::stoi(columns[1]);
            cost = std::stoi(columns[2]);
        }
        catch (const std::exception& e) {
            console("warning",
                "invalid integer data at line " + std::to_string(line_num)
                + ": " + e.what() + " - skipping line.");
            logger("warning: invalid integer data at line " + std::to_string(line_num)
                + ": " + e.what() + " - skipping line.");
            continue;
        }

        if (a <= 0 || b <= 0 || cost < 1) {
            console("warning",
                "invalid values at line " + std::to_string(line_num)
                + " (node_a and node_b must be > 0, cost must be >= 1) - skipping line.");
            logger("warning: invalid values at line " + std::to_string(line_num)
                + " - node_a=" + std::to_string(a)
                + ", node_b=" + std::to_string(b)
                + ", cost=" + std::to_string(cost)
                + " (must be strictly positive) - skipping line.");
            continue;
        }

        edges.emplace_back(edge{ a, b, cost });
        valid_lines++;
    }

    file.close();

    console("success", "finished reading CSV data into memory.");
    logger("finished reading CSV data into memory.");
    logger("total lines read: " + std::to_string(line_num));
    logger("total valid edges loaded: " + std::to_string(valid_lines));

    return true;
}

