#include <iostream>
#include <cstring>
#include <unistd.h> // for read()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int initServerSock(struct sockaddr_in address){
    int serv;
    if ((serv = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (bind(serv, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for client connections, with a maximum of 3 pending connections
    if (listen(serv, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
		std::cout << "Server is listening on port 8080..." << std::endl;

    return serv;
}

int initClientSock(struct sockaddr_in serv_addr){
    int sock;
    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    return sock;
}

int acceptSocket(struct sockaddr_in address, int server_fd, int addrlen){
    int new_socket; 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

int main() {
    int server_fd, return_sock, client_sock; // File descriptors for the server and the new client socket
    struct sockaddr_in address, serv_addr; // Structure containing an internet address
    int addrlen = sizeof(address);
    char buffer[4096] = {0}; // Buffer to store messages
    char buffer1[4096] = {0}; // Buffer to store messages

    address.sin_family = AF_INET; // Address family (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    address.sin_port = htons(8080); // Port number, converted to network byte order

    server_fd = initServerSock(address);

    return_sock = acceptSocket(address, server_fd, addrlen);

    serv_addr.sin_family = AF_INET; // Address family (IPv4)
    serv_addr.sin_port = htons(80); // Port number to connect to, converted to network byte order
	inet_pton(AF_INET, "136.159.5.25", &serv_addr.sin_addr); // Converting IPv4 and IPv6 addresses from text to binary form

    client_sock = initClientSock(serv_addr);

    while(1){

    // Reading the message from the client and printing it to the console
    ssize_t bytesRead = read(return_sock, buffer, 4096);
		if (bytesRead < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    std::cout << "Message received: " << buffer << std::endl;

    send(client_sock, buffer, 4096,0);

    read(client_sock, buffer1, 4096);

    // Sending a response to the client
    send(return_sock, buffer1, 4096, 0);
    std::cout << "Hello message sent" << std::endl;
    }
    
    return 0;
}

    // // Sending a message to the server
    // send(sock, "Hello from client", strlen("Hello from client"), 0);
    // std::cout << "Hello message sent" << std::endl;

    // // Reading the response from the server and printing it to the console
    // read(sock, buffer1, 1024);
    // std::cout << "Message received: " << buffer1 << std::endl;
    
    // return 0;
