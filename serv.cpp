#include "serv.h"

myServer::myServer(){
};
myServer::~myServer(){
    stopServer();
};

void myServer::startServer(){
    WIN(WSADATA wsaData;);
    WIN(iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0){
        std::cout << "WSAStartup failed: " << iResult << std::endl;
    }
    else
        std::cout << "WSAStartup is ok" << std::endl;);

    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ListenSocket == INVALID_SOCKET){
        std::cout << "Error at socket(): " <<  std::endl;
        WIN(WSACleanup());
    }
    else
        std::cout << "Socket() is ok" << std::endl;

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);

    iResult = bind( ListenSocket,(struct sockaddr*) &serveraddress, sizeof(serveraddress));
    if(iResult == SOCKET_ERROR){
        std::cout << "bind failed with error: " << std::endl;
        WIN(closesocket(ClientSocket))NIX(close(ClientSocket));
        WIN(WSACleanup());
    }
    else
        std::cout << "bind is ok" << std::endl;

    iResult = listen(ListenSocket,5);
    if(iResult==SOCKET_ERROR){
        std::cout << "listen failed: " << std::endl;
        WIN(closesocket(ClientSocket))NIX(close(ClientSocket));
        WIN(WSACleanup());
    }
    else
        std::cout << "listen is ok" << std::endl;

    while(true){

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ClientSocket = accept(ListenSocket, (struct sockaddr*)&client_addr, &client_len);
        if(ClientSocket == INVALID_SOCKET){
            std::cout << "accept failed: " <<std::endl;
            WIN(closesocket(ClientSocket))NIX(close(ClientSocket));
            WIN(WSACleanup());
        }
        else{
            std::cout << "accept is ok\naccepted new client: " << ClientSocket << std::endl;
            std::thread t(&myServer::threadFunc, this, std::ref(ClientSocket), std::ref(_mutex));
            t.detach();
            //_vec.emplace_back(std::thread(&myServer::threadFunc, this, std::ref(ClientSocket), std::ref(_mutex)));
        }

        std::cout << ++iterationCounter << " цикл While\n";
    }

    // for (std::thread &t : _vec) {
    //     if (t.joinable()) {
    //         t.join();
    //     }
    // }
};

void myServer::recFrom(int& recvSize, WIN(SOCKET clSocket)NIX(int clSocket)){
    int recvBuffLen = Buff;
    char recvBuff[Buff];
    std::string message;
    bzero(recvBuff, recvBuffLen);
    message.clear();
    recvSize = recv(clSocket,recvBuff,recvBuffLen,0);
    if(recvSize >= 1){
        message = recvBuff;
        std::cout << "Data received from client: " << clSocket << " " << message << std::endl;
    }
};

void myServer::sendTo(WIN(SOCKET clSocket)NIX(int clSocket)){
    int recvBuffLen = Buff;
    char recvBuff[Buff];
    std::string message;
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    std::string mystring = ss.str();
    bzero(recvBuff, recvBuffLen);
    message.clear();
    message = "response from thread ID = (" + mystring +  
                 ") says Hello to client: " + std::to_string(clSocket);
    // std::cout << "Enter your message to the client: " << std::endl;
    // std::getline(std::cin, message, '\n');
    send(clSocket,message.c_str(),message.length(),0);
};

void myServer::stopServer(){
    shutdown(ClientSocket, 1);
    WIN(closesocket(ClientSocket))NIX(close(ClientSocket));
    WIN(WSACleanup());
}
void myServer::threadFunc(WIN(SOCKET& clSocket)NIX(int& clSocket), std::mutex& mutex){
    int n;
    int x = clSocket;
    while(1){
    //mutex.lock();
    recFrom(n, x);
    if(n <= 0){
        WIN(closesocket(x);)NIX(close(x);)
        break;
    }
    sendTo(x);
    //mutex.unlock();
    };
    //WIN(closesocket(x))NIX(close(x));
};