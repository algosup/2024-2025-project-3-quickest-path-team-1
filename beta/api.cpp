#include "incl.h"

int api_start(const graph_data& gdata)
{
    // initialize winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    // create listening socket
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listening_socket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    // bind socket to port 80
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(80);

    if (bind(listening_socket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << "\n";
        closesocket(listening_socket);
        WSACleanup();
        return 1;
    }

    // start listening for connections
    if (listen(listening_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << "\n";
        closesocket(listening_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "server is listening on port 80.\n";

    while (true)
    {
        // accept client connection
        SOCKET client_socket = accept(listening_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "accept() failed: " << WSAGetLastError() << "\n";
            closesocket(listening_socket);
            WSACleanup();
            return 1;
        }

        // receive HTTP request
        char recv_buf[1024];
        ZeroMemory(recv_buf, sizeof(recv_buf));
        int bytes_received = recv(client_socket, recv_buf, sizeof(recv_buf) - 1, 0);

        if (bytes_received > 0)
        {
            std::string request(recv_buf, bytes_received);
            std::cout << "received request:\n" << request << "\n";

            // extract start and end from URL
            int start_val = -1, end_val = -1;
            {
                size_t pos_get = request.find("GET /");
                if (pos_get != std::string::npos) {
                    size_t pos_http = request.find(" HTTP/1.1", pos_get);
                    if (pos_http != std::string::npos) {
                        std::string query_part = request.substr(pos_get + 5, pos_http - (pos_get + 5));
                        size_t st_pos = query_part.find("start=");
                        if (st_pos != std::string::npos) {
                            size_t amp_pos = query_part.find("&", st_pos);
                            start_val = std::stoi(query_part.substr(st_pos + 6, amp_pos - (st_pos + 6)));
                        }
                        size_t en_pos = query_part.find("end=");
                        if (en_pos != std::string::npos) {
                            size_t amp_pos = query_part.find("&", en_pos);
                            end_val = std::stoi(query_part.substr(en_pos + 4, amp_pos - (en_pos + 4)));
                        }
                    }
                }
            }

            // run pathfinding if valid parameters
            std::string response_body;
            auto start_time = std::chrono::high_resolution_clock::now();
            if (start_val >= 0 && end_val >= 0)
            {
                path_result pres = find_shortest_path_a_star(gdata, start_val, end_val);
                auto end_time = std::chrono::high_resolution_clock::now();
                auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                if (pres.total_time < 0) {
                    std::stringstream ss;
                    ss << R"({"status":{"message":"NO PATH"},"response_time":)"
                        << elapsed_ms << R"(,"req":{"start":)"
                        << start_val << R"(,"end":)"
                        << end_val << R"(},"res":{"total_time":0,"itinary":[]}})";
                    response_body = ss.str();
                }
                else {
                    std::stringstream ss;
                    ss << R"({"status":{"message":"OK"},"response_time":)"
                        << elapsed_ms
                        << R"(,"req":{"start":)"
                        << start_val << R"(,"end":)"
                        << end_val << R"(},"res":{"total_time":)"
                        << pres.total_time
                        << R"(,"itinary":[)";
                    for (size_t i = 0; i < pres.steps.size(); i++) {
                        ss << R"({"node_a":)"
                            << pres.steps[i].node_a
                            << R"(,"node_b":)"
                            << pres.steps[i].node_b
                            << R"(,"time":)"
                            << pres.steps[i].time
                            << "}";
                        if (i + 1 < pres.steps.size()) ss << ",";
                    }
                    ss << "]}}";
                    response_body = ss.str();
                }
            }
            else {
                // invalid parameters
                auto end_time = std::chrono::high_resolution_clock::now();
                auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                std::stringstream ss;
                ss << R"({"status":{"message":"INVALID PARAMS"},"response_time":)"
                    << elapsed_ms
                    << R"(,"req":{"start":)"
                    << start_val << R"(,"end":)"
                    << end_val << R"(},"res":{"total_time":0,"itinary":[]}})";
                response_body = ss.str();
            }

            // send HTTP response
            std::string header =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n";
            std::string full_response = header + response_body;
            int bytes_sent = send(client_socket, full_response.c_str(), full_response.size(), 0);
            if (bytes_sent == SOCKET_ERROR) {
                std::cerr << "send() failed: " << WSAGetLastError() << "\n";
            }
        }
        else if (bytes_received == 0) {
            std::cout << "connection closing...\n";
        }
        else {
            std::cerr << "recv() failed: " << WSAGetLastError() << "\n";
        }

        closesocket(client_socket);
    }

    // clean up
    closesocket(listening_socket);
    WSACleanup();
    return 0;
}
