#include <iostream>
#include <cstring>
#include <unistd.h> // for read()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex>

//initializes socket that client will attempt to connect to
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

//initializes socket that will connect to website server
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

//accepts incoming connection requests from web browser
int acceptSocket(struct sockaddr_in address, int server_fd, int addrlen){
    int new_socket; 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket Accepted!" << std::endl;
    return new_socket;
}

int main(int argc, char** argv) {
    int server_fd, return_sock, client_sock; // File descriptors for the server and the new client socket
    struct sockaddr_in address, serv_addr; // Structure containing an internet address
    int addrlen = sizeof(address);
    char buffer[4096] = {0}; // Buffer to store messages
    //char buffer1[4096] = {0}; // Buffer to store messages
    char previous[4096] = {0};

    address.sin_family = AF_INET; // Address family (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    address.sin_port = htons(8080); // Port number, converted to network byte order
    //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    server_fd = initServerSock(address);

    //return_sock = acceptSocket(address, server_fd, addrlen);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    inet_pton(AF_INET, "136.159.2.17", &serv_addr.sin_addr);

    //client_sock = initClientSock(serv_addr);

    while(1){
        char buffer1[4096] = {0};
    // Reading the message from the client and printing it to the console
    return_sock = acceptSocket(address, server_fd, addrlen);
    client_sock = initClientSock(serv_addr);
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

//skips if page tries to request for the same one twice
    if(strcmp(previous, buffer) == 0){
        std::cout << "Repeat Skipped" << std::endl;
        continue;
    }

//skips anything other than get request for websites
    if(buffer[0] != 'G' || buffer[11] != 'p' || buffer[34] != '~'){
        std::cout << "Post skipped" << std::endl;
        char buf[4096];
        strcpy(buf, buffer);
        send(client_sock, buf, 4096, 0);
        bytesRead = read(client_sock, buffer1, 4096);
        send(return_sock, buffer1, 4096, 0);

        continue;
    }

//sends get request to website server
    send(client_sock, buffer, 4096,0);
    std::cout << "Request to website sent!" << std::endl;
    std::string s = "";
    
    // recv(client_sock, buffer1, 4096, 0);
    // std::string header(buffer1);
    // std::regex length("Content-Length: ");
    // auto content_begin = std::sregex_iterator(header.begin(), header.end(), length);
    // std::smatch m = *(((((((((++(content_begin))))))))));
    // std::string matchstr = m.str();
    // std::cout << std::endl << matchstr << std::endl << std::endl;
    // s.append(buffer1);

//fills s with all the data incoming from website
    while(recv(client_sock, buffer1, 4096, 0) > 0){
        s.append(buffer1);
    }
    std::cout << "Message Recieved from website!" << std::endl;

    //std::cout << buffer1 << std::endl;
    //std::string s = buffer1;
    if(strcmp(argv[1], "frog") == 0){
    //regex for replacing frog with fred
        std::regex frog("frog\\s|Frog\\s|FRog\\s|FROg\\s|FROG\\s|fROG\\s|frOG\\s|froG\\s|fRog\\s|frOg\\s|FrOG\\s|FRoG\\s|fRoG\\s|FrOg\\s|fRog\\s|frOg\\s");
        s = std::regex_replace(s, frog, "fred ");
        std::regex frogdash("\\sfrog-");
        s = std::regex_replace(s, frogdash, " fred-");
        std::regex frogs("frogs\\s|Frogs\\s|FRogs\\s|FROgs\\s|FROGs\\s|fROGs\\s|frOGs\\s|froGs\\s|fRogs\\s|frOgs\\s|FrOGs\\s|FRoGs\\s|fRoGs\\s|FrOgs\\s|fRogs\\s|frOgs\\s");
        s = std::regex_replace(s, frogs, "freds ");
        std::regex froggy("\\sfrogg");
        s = std::regex_replace(s, froggy, " fredg");
        std::cout << s << std::endl;
    }

    //sends data back to web browser
    send(return_sock, s.c_str(), s.length(), 0);
    std::cout << "Hello message sent" << std::endl;
    strcpy(previous, buffer);
    }
    std::cout << "End of main" << std::endl;
    return 0;
}
