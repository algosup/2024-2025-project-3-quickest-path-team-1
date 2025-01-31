// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Saves precomputed ALT heuristic data to a binary file.
 *
 * @detailed
 * This function writes the ALT (A* Landmarks and Triangle Inequality) heuristic
 * data to a binary file for fast I/O.
 *
 * Data Stored:
 * - dist_landmark_to:
 *   - First writes the size of the outer vector (number of nodes).
 *   - For each element (node), writes the number of landmarks, then the array of `int` distances.
 *
 * - dist_landmark_from:
 *   - Same pattern as dist_landmark_to.
 *
 * Steps of Execution:
 * 1. Open the output file in binary mode, truncating it if it exists.
 * 2. Write the data sizes and then the distance values in binary.
 * 3. Handle exceptions for file I/O operations.
 *
 * Edge Cases Handled:
 * - If the file cannot be opened, log an error and return.
 * - If an exception occurs during writing, log an error and return.
 *
 * Complexity:
 * - Time Complexity: O(V × nb_alt), where `V` is the number of nodes and `nb_alt` is the
 *   number of landmarks. Each distance is written once.
 * - Space Complexity: O(1). No additional memory besides small local variables is used.
 *
 * @param gdata Reference to the graph data structure containing precomputed ALT data.
 * @param conf Reference to configuration settings containing the output file path.
 */
void saveAltData(const graph& gdata, const config& conf)
{
    console("info", "starting to save ALT data to file " + conf.alt_prep_file);
    console("warning", "DO NOT CLOSE THE PROGRAM UNTIL THIS PROCESS IS FINISHED!");
    logger("starting to save ALT data to file " + conf.alt_prep_file);

    try {
        std::ofstream ofs(conf.alt_prep_file, std::ios::binary | std::ios::out | std::ios::trunc);
        if (!ofs.is_open()) {
            console("error", "unable to open file for writing ALT data:" + conf.alt_prep_file + ". skipping saving part.");
            logger("error: unable to open file for writing ALT data:" + conf.alt_prep_file + ". skipping saving part.");
            return;
        }

        {
            uint64_t outer_size = static_cast<uint64_t>(gdata.dist_landmark_to.size());
            ofs.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));

            for (const auto& row : gdata.dist_landmark_to) {
                uint64_t inner_size = static_cast<uint64_t>(row.size());
                ofs.write(reinterpret_cast<const char*>(&inner_size), sizeof(inner_size));

                ofs.write(reinterpret_cast<const char*>(row.data()), inner_size * sizeof(int));
            }
        }
        {
            uint64_t outer_size = static_cast<uint64_t>(gdata.dist_landmark_from.size());
            ofs.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));

            for (const auto& row : gdata.dist_landmark_from) {
                uint64_t inner_size = static_cast<uint64_t>(row.size());
                ofs.write(reinterpret_cast<const char*>(&inner_size), sizeof(inner_size));

                ofs.write(reinterpret_cast<const char*>(row.data()), inner_size * sizeof(int));
            }
        }

        ofs.close();
    }
    catch (const std::exception& e) {
        console("warning", "error while saving ALT data: " + std::string(e.what()));
        logger("error: cannot save ALT data: " + std::string(e.what()));
        return;
    }

    console("success", "finished saving ALT data to file " + conf.alt_prep_file);
    logger("finished saving ALT data to file " + conf.alt_prep_file);
}


// ✅ function + comment verified.
/**
 * @brief Loads precomputed ALT heuristic data from a binary file.
 *
 * @detailed
 * This function reads the ALT (A* Landmarks and Triangle Inequality) heuristic
 * data from a binary file. The format must match the one used by `saveAltData`.
 *
 * Data Loaded:
 * - dist_landmark_to:
 *   - Reads the number of nodes (outer size).
 *   - For each node, reads the number of landmarks, then reads `int` distances.
 *
 * - dist_landmark_from:
 *   - Same pattern as dist_landmark_to.
 *
 * Steps of Execution:
 * 1. Open the input file in binary mode.
 * 2. Check if the file is empty (size == 0); if empty, return `false` to force preprocessing.
 * 3. Read the data in the same order it was written.
 * 4. Populate the graph structure’s ALT distance vectors.
 * 5. Handle exceptions for file I/O operations.
 *
 * Edge Cases Handled:
 * - If the file does not exist or is empty, returns `false` (indicating we need to recompute ALT data).
 * - If an exception occurs (e.g., incomplete or corrupted file), returns `false`.
 *
 * Complexity:
 * - Time Complexity: O(V × nb_alt), reading each node’s landmark distances.
 * - Space Complexity: O(V × nb_alt), storing all loaded distances.
 *
 * @param gdata Reference to the graph data structure to be populated.
 * @param conf Reference to configuration settings containing the input file path.
 * @return `true` if ALT data was successfully loaded, otherwise `false`.
 */
bool loadAltData(graph& gdata, config& conf)
{
    std::ifstream ifs(conf.alt_prep_file, std::ios::binary | std::ios::in);
    if (!ifs.is_open()) {
        return false;
    }

    console("info", "loading ALT data from " + conf.alt_prep_file);
    logger("loading ALT data from " + conf.alt_prep_file);

    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    if (fileSize == 0) {
        console("info", "ALT data file is empty, proceeding with preprocessing");
        logger("ALT data file is empty, proceeding with preprocessing");
        return false;
    }
    ifs.seekg(0, std::ios::beg);

    try {
        {
            uint64_t outer_size = 0;
            ifs.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));
            if (!ifs.good()) {
                console("error", "failed to read the size of dist_landmark_to, processing landmarks again");
                logger("error: failed to read the size of dist_landmark_to, processing landmarks again.");
                return false;
            }

            gdata.dist_landmark_to.resize(static_cast<size_t>(outer_size));
            for (size_t i = 0; i < outer_size; ++i) {
                uint64_t inner_size = 0;
                ifs.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));
                if (!ifs.good()) {
                    console("error", "failed to read the size of a dist_landmark_to row, processing landmarks again");
                    logger("error: failed to read the size of a dist_landmark_to row, processing landmarks again.");
                    return false;
                }

                gdata.dist_landmark_to[i].resize(static_cast<size_t>(inner_size));
                ifs.read(reinterpret_cast<char*>(gdata.dist_landmark_to[i].data()),
                    static_cast<std::streamsize>(inner_size * sizeof(int)));
                if (!ifs.good()) {
                    console("error", "failed to read dist_landmark_to row data, processing landmarks again");
                    logger("error: failed to read dist_landmark_to row data, processing landmarks again.");
                    return false;
                }
            }
        }
        {
            uint64_t outer_size = 0;
            ifs.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));
            if (!ifs.good()) {
                console("error", "failed to read the size of dist_landmark_from, processing landmarks again");
                logger("error: failed to read the size of dist_landmark_from, processing landmarks again.");
                return false;
            }

            gdata.dist_landmark_from.resize(static_cast<size_t>(outer_size));
            for (size_t i = 0; i < outer_size; ++i) {
                uint64_t inner_size = 0;
                ifs.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));
                if (!ifs.good()) {
                    console("error", "failed to read the size of a dist_landmark_from row, processing landmarks again");
                    logger("error: failed to read the size of a dist_landmark_from row, processing landmarks again.");
                    return false;
                }

                gdata.dist_landmark_from[i].resize(static_cast<size_t>(inner_size));
                ifs.read(reinterpret_cast<char*>(gdata.dist_landmark_from[i].data()),
                    static_cast<std::streamsize>(inner_size * sizeof(int)));
                if (!ifs.good()) {
                    console("error", "failed to read dist_landmark_from row data, processing landmarks again");
                    logger("error: failed to read dist_landmark_from row data, processing landmarks again");
                    return false;
                }
            }
        }

        ifs.close();
        console("success", "ALT data loaded successfully from " + conf.alt_prep_file);
        logger("ALT data loaded successfully from " + conf.alt_prep_file);
        return true;
    }
    catch (const std::exception& e) {
        console("warning", "failed to parse ALT data: " + std::string(e.what()));
        logger("error: failed to parse ALT data: " + std::string(e.what()));
        return false;
    }
}
