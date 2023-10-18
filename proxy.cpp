#include <iostream>
#include <cstring>
#include <unistd.h> // for read()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex>

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
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cout << "Scoket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    inet_pton(AF_INET, "136.159.2.17", &serv_addr.sin_addr);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    std::cout << "Client Connected" << std::endl;
    return sock;
}

int acceptSocket(struct sockaddr_in address, int server_fd, int addrlen){
    int new_socket; 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket Accepted!" << std::endl;
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
    //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    server_fd = initServerSock(address);

    return_sock = acceptSocket(address, server_fd, addrlen);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    inet_pton(AF_INET, "136.159.2.17", &serv_addr.sin_addr);

    client_sock = initClientSock(serv_addr);

    while(1){
    // Reading the message from the client and printing it to the console
    ssize_t bytesRead = read(return_sock, buffer, 4096);
		if (bytesRead < 0) {
        //perror("read");
        //exit(EXIT_FAILURE);
    }


    // std::string s = buffer;

    // std::regex hostReg("Host: ");
    // auto words_begin = std::sregex_iterator(s.begin(), s.end(), hostReg);
    // std::regex endHostReg(".ca\r\n");
    // auto end = std::sregex_iterator(s.begin(), s.end(), endHostReg);
    // std::cout << std::distance(words_begin, end) << std::endl;
    // for(std::sregex_iterator i = words_begin; i != end; ++i){
    //     std::smatch match = *i;
    //     std::string match_str = match.str();
    //     std::cout << match_str << std::endl;
    // }
    



    std::cout << "Message received: " << buffer << std::endl;

    if(buffer[0] != 'G' || buffer[11] != 'p'){
        std::cout << "Post skipped" << std::endl;
        continue;
    }

    send(client_sock, buffer, 4096,0);
    std::cout << "Request to website sent!" << std::endl;
    std::string s = "";
    while(read(client_sock, buffer1, 4096) != 0){
        s += buffer1;
    }
    std::cout << "Message Recieved from website!" << std::endl;

    std::cout << buffer1 << std::endl;
    // Sending a response to the client
    //std::string s = buffer1;
    std::regex frog("frog\\s|Frog\\s|FRog\\s|FROg\\s|FROG\\s|fROG\\s|frOG\\s|froG\\s|fRog\\s|frOg\\s|FrOG\\s|FRoG\\s|fRoG\\s|FrOg\\s|fRog\\s|frOg\\s");
    s = std::regex_replace(s, frog, "fred ");
    std::cout << s << std::endl;

    send(return_sock, s.c_str(), 4096, 0);
    //send(return_sock, "\r\n\r\n", strlen("\r\n\r\n"),0);
    std::cout << "Hello message sent" << std::endl;
    }
    std::cout << "End of main" << std::endl;
    return 0;
}

    // // Sending a message to the server
    // send(sock, "Hello from client", strlen("Hello from client"), 0);
    // std::cout << "Hello message sent" << std::endl;

    // // Reading the response from the server and printing it to the console
    // read(sock, buffer1, 1024);
    // std::cout << "Message received: " << buffer1 << std::endl;
    
    // return 0;
