#include "incl.h"

int api_start() 
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listening_socket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

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

    if (listen(listening_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << "\n";
        closesocket(listening_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "server is listening on port 80.";

    while (true) 
    {
        SOCKET client_socket = accept(listening_socket, nullptr, nullptr);
        
        if (client_socket == INVALID_SOCKET) 
        {
            std::cerr << "accept() failed: " << WSAGetLastError() << "\n";
            closesocket(listening_socket);
            WSACleanup();
            return 1;
        }

        char recv_buf[1024];
        ZeroMemory(recv_buf, sizeof(recv_buf));
        int bytes_received = recv(client_socket, recv_buf, sizeof(recv_buf) - 1, 0);

        if (bytes_received > 0) 
        {
            std::string request(recv_buf, bytes_received);
            std::cout << "received request:\n" << request << "\n";

            std::string body = "<html><body><h1>Salam mon frere jespere tu vas bien rrohya lebess trkl woula</h1></body></html>";
            std::string header =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n";

            std::string full_response = header + body;

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

    closesocket(listening_socket);
    WSACleanup();
    return 0;
}