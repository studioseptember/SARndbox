/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ControlSocket.cpp
 * Author: daan
 * 
 * Created on October 30, 2017, 10:30 AM
 */

#include "ControlSocket.h"


ControlSocket::ControlSocket(std::string controlPipeName) {

    
    this->listen(controlPipeName);
    
    if(!this->makeSocketNonblocking(this->serverSocketFd)){
        std::cout << "Error while setting non-blocking";
    }
        
    
    this->epollFd = epoll_create1(0);


    if (this->epollFd == -1){
        std::cerr << "[E] epoll_create1 failed\n";
        return;
    }

    struct epoll_event event;
    event.data.fd = this->serverSocketFd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->serverSocketFd, &event) == -1){
        if (this->epollFd == -1)
        {
            std::cerr << "[E] epoll_ctl failed\n";
        }
    }
    
    
}

bool ControlSocket::makeSocketNonblocking(int socketfd)
{
    int flags = fcntl(socketfd, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "[E] fcntl failed (F_GETFL)\n";
        return false;
    }

    flags |= O_NONBLOCK;
    int s = fcntl(socketfd, F_SETFL, flags);
    if (s == -1)
    {
        std::cerr << "[E] fcntl failed (F_SETFL)\n";
        return false;
    }

    return true;
}


void ControlSocket::listen(std::string controlPipeName){

    this->serverSocketFd = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un controlSocket;

    controlSocket.sun_family = AF_UNIX;  /* local is declared before socket() ^ */
    strcpy(controlSocket.sun_path, controlPipeName.c_str());
    unlink(controlSocket.sun_path);

    std::cout << "Opening socket: " << controlSocket.sun_path << std::endl;

    socklen_t len = strlen(controlSocket.sun_path) + sizeof(controlSocket.sun_family);
    int res = bind(this->serverSocketFd, (struct sockaddr *)&controlSocket, len);
    std::cout << "Bind result: " << res << ", errno: " << errno << std::endl;

    res = ::listen(this->serverSocketFd, 1);
    std::cout << "Listen result: " << res << ", errno: " << errno << std::endl;

    //child:
//    for(;;){
        //make it possible to accept client connections
    
    if(this->serverSocketFd > 0){

        std::cout << "Waiting for connection on " << controlSocket.sun_path << std::endl;
//
//        int res = select(controlSocketServerFd, NULL, NULL, NULL, &listenTimeout);
//        std::cout << "Have connection waiting:" << res << std::endl;
//        if(res > 0){





//        }
    }
    
//    }

}

void ControlSocket::tick(){
    
    std::array<struct epoll_event, ControlSocket::max_events> events;
    
    int nfds = epoll_wait(this->epollFd, events.data(), ControlSocket::max_events, 1 /* Timeout */);
    
    
//    std::cout << "Have events: " << nfds  << ", errno: " << errno << std::endl;
    
    for(int i = 0; i < nfds; ++i) {
        epoll_event event = events[i];
    
        if(event.data.fd == this->serverSocketFd){
             
            this->handleServerEvent(event);
            
        }else{
            
            this->handleClientEvent(event);
            /*
            int eventType = event.events;
            
            if(eventType & EPOLLHUP){ //connection closed
                shutdown(this->socketFd, SHUT_RDWR);
                this->socketFd = 0;
                return;
            }
        
            if(eventType & EPOLLIN){
                int received;
                char buffer[4096];
                if ((received = recv(event.data.fd, buffer, sizeof(buffer), 0)) < 0) {
                    perror("Error reading from socket");
                } else if(received > 0) {
                    buffer[received] = 0;
                    std::cout << "Reading: " << buffer << std::endl;
                    
                    this->handleControlCommand(new std::string(buffer));
                }
            }
            */
            
        }
    }

//    this->serverTick();
//    this->clientTick();
}

void ControlSocket::handleServerEvent(epoll_event event){
    
    std::cout << "Have server event: " << event.events << std::endl;


    if(this->socketFd > 0){
        shutdown(this->socketFd, SHUT_RDWR);
    }

    socklen_t len = sizeof(struct sockaddr_un);
    sockaddr remote;

    this->socketFd = accept(this->serverSocketFd, &remote, &len);

    std::cout << "Got a new connection" << std::endl;

    if (!this->makeSocketNonblocking(this->socketFd))
    {
        std::cerr << "[E] make_socket_nonblocking failed\n";
        return;
    }


    event.data.fd = this->socketFd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->socketFd, &event) == -1)
    {
        std::cerr << "[E] epoll_ctl failed\n";
        return;
    }
            
            
    /*
    int eventType = event.events;

    if(eventType & EPOLLHUP){ //connection closed

        std::cerr << "Server socket closed" << std::endl;

    }else{

        if(this->socketFd > 0){
            shutdown(this->socketFd, SHUT_RDWR);
        }

        socklen_t len = sizeof(struct sockaddr_un);
        sockaddr remote;

        this->socketFd = accept(this->serverSocketFd, &remote, &len);

        std::cout << "Got a new connection" << std::endl;

    }
    */
    

    
}

void ControlSocket::handleClientEvent(epoll_event event){
    
    std::cout << "Have client event: " << event.events << std::endl;
    
    if(this->socketFd <= 0){
        return;
    }

    int fd = event.data.fd;
    int eventType = event.events;

    if(eventType & EPOLLHUP){ //connection closed
        shutdown(this->socketFd, SHUT_RDWR);
        this->socketFd = 0;
        return;
    }

    if(eventType & EPOLLIN){
        int received;
        char buffer[4096];
        if ((received = recv(fd, buffer, sizeof(buffer), 0)) < 0) {
            perror("Error reading from socket");
        } else if(received > 0) {
            buffer[received] = 0;
            
            //trim a final newline
            if(buffer[received-1] == '\n'){
                buffer[received-1] = 0;
            }
            
            std::cout << "Reading: " << buffer << std::endl;
            

            this->handleControlCommand(new std::string(buffer));
        }




        /*

        std::string runningCommand = "";

        char buffer[4096];
        bool done = false;
        do {
            int n = recv(this->socketFd, buffer, sizeof(buffer), 0);
            if (n <= 0) {
                if (n < 0) perror("recv");
                done = true;
            }

            runningCommand.append(buffer, 0, n);

            std::cout << "New runningCommand: " << runningCommand.c_str() << std::endl;


            if(int newlinePos = runningCommand.at('\n')){
                std::string singleCommand = runningCommand.substr(0, newlinePos);
                runningCommand = runningCommand.substr(newlinePos + 1, runningCommand.length()  - (newlinePos + 1));
                this->handleControlCommand(singleCommand);
            }

            if (!done) 
                if (send(this->socketFd, buffer, n, 0) < 0) {
                    perror("send");
                    done = true;
                }
        } while (!done);
         */


    }
}

void ControlSocket::handleControlCommand(std::string * command){
    std::cout << "Handling command " << command->c_str() << std::endl;
    
    handleString_function replyHandler = [this] (std::string* reply) {
        if (send(this->socketFd, reply->c_str(), reply->length(), 0) < 0) {
            perror("Error in send.");
        }
    };
    
    this->commandHandler(command, replyHandler);
}

void ControlSocket::setCommandHandler(handleCommand_function handler) {
    this->commandHandler = handler;
}


ControlSocket::ControlSocket(const ControlSocket& orig) {
}

ControlSocket::~ControlSocket() {
    
    if(this->socketFd){
        int tmp = this->socketFd;
        this->socketFd = 0;
        shutdown(tmp, SHUT_RDWR);
    }
    
    if(this->serverSocketFd){
        int tmp = this->serverSocketFd;
        this->serverSocketFd = 0;
        shutdown(tmp, SHUT_RDWR);
    }

}

