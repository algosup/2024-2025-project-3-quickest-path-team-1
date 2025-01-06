#include "incl.h"

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    system("cls");

    bool enable_log = false;
    std::string log_file_path;
    std::string file_path;

    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];
        
        if (arg == "-log" && i + 1 < argc) 
        {
            enable_log = true;
            log_file_path = argv[++i];
        }
        else 
        {
            if (file_path.empty()) {
                file_path = arg;
            }
        }
    }

    if (enable_log) {
        if (!init_logger(log_file_path)) {
            return 1;
        }
    }

    std::string map_path;

    std::cout << "Enter the map path (press 'ENTER' if you want to download it from GitHub): ";

    std::getline(std::cin, map_path);

    std::cout << "---" << std::endl;

    if (map_path.empty()) 
    {
        std::cout << "No path entered. Downloading from GitHub..." << std::endl;
    }

    /* PTN DE PROBLEME DE MERDE
    static bool result_integrity = integrity_validator(map_path);
    if (!result_integrity)
    {
        std::cout << "[FAILED] integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops." << std::endl;
        return 0;
    }
    else {
        std::cout << "[SUCCESS] integrity check passed without any error. continueing." << std::endl;
    }
    */
    static bool result_connectivity = connectivity_validator(map_path); 
    if (!result_connectivity)
    {
        std::cout << "[FAILED] connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks. " << std::endl;
        return 0;
    }
    else {
        std::cout << "[SUCCESS] connectivity check passed without any error. continueing." << std::endl;
    }

    api_start();
}
