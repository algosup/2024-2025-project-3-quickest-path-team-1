// ✅ file verified.
#include "incl.h"
#include "decl.h"

// ✅ function + comment verified.
/**
 * @brief Converts a `sockaddr_in` structure to a human-readable IPv4 address string.
 *
 * @detailed
 * This function takes a `sockaddr_in` structure (typically from a network connection)
 * and converts the IP address into a readable string format using `inet_ntop`
 * (or `InetNtopA` on Windows).
 *
 * Functionality:
 * - Uses `inet_ntop()` (or `InetNtopA()` on Windows) to convert the binary IPv4 address
 *   (`sin_addr`) to a string.
 * - If conversion fails, returns `"Unknown"`.
 *
 * Platform-Specific Handling:
 * - On Windows, uses `InetNtopA()` from `Ws2tcpip.h`.
 * - On Linux/macOS, uses `inet_ntop()` from `arpa/inet.h`.
 *
 * Edge Cases Handled:
 * - If the conversion function fails, returns `"Unknown"`.
 *
 * @param addr A `sockaddr_in` structure containing the IP address.
 * @return A `std::string` representing the human-readable IP address.
 *
 * @complexity
 * - Time Complexity: O(1) (Single system call).
 * - Space Complexity: O(1) (Fixed-size buffer for IP conversion).
 */
std::string ipToString(const sockaddr_in& addr) 
{
    char ip_str[INET_ADDRSTRLEN];
#ifdef _WIN32
    if (InetNtopA(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL) {
        return "Unknown";
    }
#else
    if (inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL) {
        return "Unknown";
    }
#endif
    return std::string(ip_str);
}

// ✅ function + comment verified.
/**
 * @brief Starts an HTTP API server to process shortest path queries.
 *
 * @detailed
 * This function initializes a socket-based API gateway that listens for HTTP requests
 * on port 80. The server processes GET requests with start and end node parameters
 * and returns the shortest path result in JSON or XML format.
 *
 * Steps of Execution:
 * - Step 1: Initialize Network Socket
 *   - On Windows, initializes Winsock (`WSAStartup`).
 *   - Creates a listening socket (`socket()`) for accepting connections.
 *   - Binds the socket to port 80 (`bind()`) and starts listening (`listen()`).
 *
 * - Step 2: Handle Incoming Requests
 *   - Accepts incoming client connections (`accept()`).
 *   - Reads the HTTP GET request to extract:
 *     - `start` (source node)
 *     - `end` (destination node)
 *     - `weight` (optional heuristic weight factor).
 *   - Parses request headers to determine response format (`JSON` or `XML`).
 *
 * - Step 3: Compute Shortest Path
 *   - Calls `findShortestPath()` using the extracted parameters.
 *   - If the path is found, constructs a valid JSON/XML response.
 *   - If the path does not exist, returns a `404 Not Found` error.
 *
 * - Step 4: Send HTTP Response
 *   - Constructs an HTTP response header.
 *   - Generates a JSON or XML response body.
 *   - Sends the response back to the client (`send()`).
 *
 * - Step 5: Clean Up
 *   - Closes the client socket (`closesocket()`).
 *   - Logs the request and response details.
 *
 * Edge Cases Handled:
 * - Winsock Initialization Failure (Windows) → Logs and exits.
 * - Socket Creation/Binding Errors → Logs and exits.
 * - Invalid HTTP Requests → Returns `400 Bad Request`.
 * - Malformed Query Parameters → Returns `400 Bad Request`.
 * - No Path Found → Returns `404 Not Found`.
 * - Unsupported Accept Headers → Returns `400 Bad Request`.
 *
 * @param gdata The graph data structure used for shortest path calculations.
 * @param conf The configuration settings for the API.
 * @return `0` on success, `1` on failure.
 *
 * @complexity
 * - Time Complexity:
 *   - Socket Setup: O(1)
 *   - Request Parsing: O(N) (where N is request length)
 *   - Path Calculation: O(E log V) (Dijkstra/A* search)
 *   - Response Construction: O(M) (where M is response size)
 *   - Overall Complexity: O(E log V) (depends on graph traversal).
 * - Space Complexity:
 *   - Request Buffer: O(1)
 *   - Response Buffer: O(1)
 *   - Graph Memory Usage: O(V + E).
 */
int launchApiGateway(const graph& gdata, search_buffers& buffers, const config& conf)
{
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        console("error", "failed to initialize winsock.");
        logger("error: failed to initialize winsock.");
        return 1;
    }
#endif

    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == INVALID_SOCKET) {
        console("error", "socket creation failed.");
        logger("error: socket creation failed.");
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port = htons(80);

    if (bind(listening_socket, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == SOCKET_ERROR) {
        console("error", "bind failed.");
        logger("error: bind failed.");
        closesocket(listening_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(listening_socket, SOMAXCONN) == SOCKET_ERROR) {
        console("error", "listen failed.");
        logger("error: listen failed.");
        closesocket(listening_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    console("success", "server is listening on port 80. press any touch to close it.");
    logger("server is listening on port 80.");

    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        SOCKET client_socket = accept(listening_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

        if (client_socket == INVALID_SOCKET) {
            console("error", "failed to accept connection.");
            logger("error: failed to accept connection.");
            closesocket(listening_socket);
#ifdef _WIN32
            WSACleanup();
#endif
            return 1;
        }

        char recv_buf[2048];
        memset(recv_buf, 0, sizeof(recv_buf));
        int bytes_received = recv(client_socket, recv_buf, sizeof(recv_buf) - 1, 0);

        auto start_time = std::chrono::high_resolution_clock::now();
        bool success = true;
        int status_code = 200;
        double used_weight = conf.weight;
        int start_val = -1;
        int end_val = -1;
        std::string message_response = "OK";
        std::string user_ip = ipToString(client_addr);

        enum class ResponseType { JSON, XML, UNKNOWN };
        ResponseType response_type = ResponseType::JSON;
        bool accept_invalid = false;

        if (bytes_received <= 0) {
            console("warning", "empty request or recv error.");
            logger("warning: empty request or recv error.");
            closesocket(client_socket);
            continue;
        }

        std::string request(recv_buf, bytes_received);
        size_t pos_get = request.find("GET /");
        if (pos_get == std::string::npos) {
            success = false;
            message_response = "INVALID REQUEST";
            status_code = 400;
        }
        else {
            size_t pos_http = request.find(" HTTP/1.1", pos_get);
            if (pos_http == std::string::npos) {
                success = false;
                message_response = "INVALID REQUEST";
                status_code = 400;
            }
            else {
                std::string query_part = request.substr(pos_get + 5, pos_http - (pos_get + 5));
                size_t st_pos = query_part.find("start=");
                size_t en_pos = query_part.find("end=");
                size_t wt_pos = query_part.find("weight=");

                bool start_ok = false;
                bool end_ok = false;
                bool weight_ok = true;

                if (st_pos != std::string::npos) {
                    try {
                        size_t amp_pos = query_part.find("&", st_pos);
                        std::string val = query_part.substr(st_pos + 6, amp_pos - (st_pos + 6));
                        start_val = std::stoi(val);
                        start_ok = true;
                    }
                    catch (...) {
                        success = false;
                        start_ok = false;
                        message_response = "INVALID PARAMS";
                        status_code = 400;
                    }
                }
                if (en_pos != std::string::npos) {
                    try {
                        size_t amp_pos = query_part.find("&", en_pos);
                        std::string val = query_part.substr(en_pos + 4, amp_pos - (en_pos + 4));
                        end_val = std::stoi(val);
                        end_ok = true;
                    }
                    catch (...) {
                        success = false;
                        end_ok = false;
                        message_response = "INVALID PARAMS";
                        status_code = 400;
                    }
                }
                if (conf.personalized_weight && wt_pos != std::string::npos) {
                    try {
                        size_t amp_pos = query_part.find("&", wt_pos);
                        std::string val = query_part.substr(wt_pos + 7, amp_pos - (wt_pos + 7));
                        double tmp_weight = std::stod(val);
                        bool format_ok = true;
                        size_t dot_pos = val.find(".");
                        if (dot_pos != std::string::npos) {
                            if (val.size() - dot_pos - 1 > 2) {
                                format_ok = false;
                            }
                        }
                        if (tmp_weight < 1.0 || tmp_weight > 2.0 || !format_ok) {
                            weight_ok = false;
                        }
                        else {
                            used_weight = tmp_weight;
                        }
                    }
                    catch (...) {
                        weight_ok = false;
                    }
                    if (!weight_ok) {
                        success = false;
                        message_response = "INVALID WEIGHT";
                        status_code = 400;
                    }
                }
                if (!start_ok || !end_ok) {
                    success = false;
                    message_response = "INVALID PARAMS";
                    status_code = 400;
                }
                if (success && (start_val < 0 || end_val < 0)) {
                    success = false;
                    message_response = "INVALID PARAMS";
                    status_code = 400;
                }
            }
        }

        if (success) {
            size_t accept_pos = request.find("Accept:");
            if (accept_pos == std::string::npos) {
                response_type = ResponseType::JSON;
            }
            else {
                size_t line_end = request.find("\r\n", accept_pos);
                if (line_end == std::string::npos) {
                    line_end = request.find("\n", accept_pos);
                }
                std::string accept_line = request.substr(accept_pos, (line_end == std::string::npos ? line_end : line_end - accept_pos));
                std::string accept_line_lower;
                accept_line_lower.resize(accept_line.size());
                std::transform(accept_line.begin(), accept_line.end(), accept_line_lower.begin(), ::tolower);

                if (accept_line_lower.find("application/xml") != std::string::npos) {
                    response_type = ResponseType::XML;
                }
                else if (accept_line_lower.find("application/json") != std::string::npos) {
                    response_type = ResponseType::JSON;
                }
                else if (accept_line_lower.find("*/*") != std::string::npos) {
                    response_type = ResponseType::JSON;
                }
                else {
                    success = false;
                    accept_invalid = true;
                    message_response = "INVALID ACCEPT HEADER";
                    status_code = 400;
                }
            }
        }

        path_result pres;
        if (success) {
            if(conf.search_engine == 1) {
                pres = findShortestPathUnidirectional(gdata, buffers, conf, start_val, end_val, used_weight); 
            }
            else {
                pres = findShortestPathBidirectional(gdata, buffers, conf, start_val, end_val, used_weight);
            }
            if (pres.total_time < 0) {
                success = false;
                message_response = "NO PATH FOUND";
                status_code = 404;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::string response_str;
        std::string content_type;
        std::string status_line = (status_code == 200) ? "200 OK" : std::to_string(status_code) + " ";

        switch (status_code) {
        case 200: status_line += "OK"; break;
        case 400: status_line += "Bad Request"; break;
        case 404: status_line += "Not Found"; break;
        default:  status_line += "Error"; break;
        }

        if (accept_invalid) {
            content_type = "application/json";
            std::stringstream ss;
            ss << R"({"status":{"message":")" << message_response << R"(","code":)" << status_code << "}}";
            response_str = ss.str();
        }
        else if (response_type == ResponseType::XML) {
            content_type = "application/xml";
            std::stringstream ss;
            ss << "<?xml version=\"1.0\"?>\n";
            ss << "<response>\n";
            ss << "  <status>\n";
            ss << "    <message>" << message_response << "</message>\n";
            ss << "    <code>" << status_code << "</code>\n";
            ss << "  </status>\n";
            ss << "  <response_time>" << elapsed_ms << "</response_time>\n";
            ss << "  <req>\n";
            ss << "    <start>" << start_val << "</start>\n";
            ss << "    <end>" << end_val << "</end>\n";
            ss << "    <weight>" << used_weight << "</weight>\n";
            ss << "  </req>\n";
            ss << "  <res>\n";
            if (success) {
                ss << "    <total_time>" << pres.total_time << "</total_time>\n";
                ss << "    <total_node>" << pres.total_node << "</total_node>\n";
                ss << "    <itinary>";
                for (size_t i = 0; i < pres.steps.size(); ++i) {
                    ss << pres.steps[i];
                    if (i + 1 < pres.steps.size()) ss << ",";
                }
                ss << "</itinary>\n";
            }
            else {
                ss << "    <total_time>0</total_time>\n";
                ss << "    <total_node>0</total_node>\n";
                ss << "    <itinary></itinary>\n";
            }
            ss << "  </res>\n";
            ss << "</response>\n";
            response_str = ss.str();
        }
        else {
            content_type = "application/json";
            std::stringstream ss;
            ss << R"({"status":{"message":")" << message_response << R"(","code":)" << status_code
                << R"(},"response_time":)" << elapsed_ms
                << R"(,"req":{"start":)" << start_val
                << R"(,"end":)" << end_val
                << R"(,"weight":)" << used_weight
                << R"(},"res":{)";

            if (success) {
                ss << R"("total_time":)" << pres.total_time
                    << R"(,"total_node":)" << pres.total_node
                    << R"(,"itinary":[)";
                for (size_t i = 0; i < pres.steps.size(); ++i) {
                    ss << pres.steps[i];
                    if (i + 1 < pres.steps.size()) ss << ",";
                }
                ss << "]}}";
            }
            else {
                ss << R"("total_time":0,"total_node":0,"itinary":[]}})";
            }
            response_str = ss.str();
        }

        std::string header =
            "HTTP/1.1 " + status_line + "\r\n"
            "Content-Type: " + content_type + "\r\n"
            "Content-Length: " + std::to_string(response_str.size()) + "\r\n"
            "Connection: close\r\n\r\n";

        std::string full_response = header + response_str;
        send(client_socket, full_response.c_str(), static_cast<int>(full_response.size()), 0);
        closesocket(client_socket);

        if (pos_get != std::string::npos && (start_val != -1 || end_val != -1)) {
            std::string color_time = (elapsed_ms > 1000 ? RED : GREEN);
            std::string color_success = (success ? GREEN : RED);
            std::string log_message = user_ip
                + " > start node: " + std::to_string(start_val)
                + ", end node: " + std::to_string(end_val)
                + ", weight: " + std::to_string(used_weight)
                + " > elapsed time: "
                + color_time + std::to_string(elapsed_ms) + RESET
                + " ms / success: "
                + color_success + (success ? "true" : "false") + RESET + ".";

            console("info", log_message);
            logger(user_ip + " > start node: " + std::to_string(start_val) + ", end node: " + std::to_string(end_val) + ", weight: " + std::to_string(used_weight) + " > elapsed time: " + std::to_string(elapsed_ms) + " ms / success: " + (success ? "true" : "false") + ".");
        }
    }

    closesocket(listening_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
