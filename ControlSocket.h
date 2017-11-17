/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ControlSocket.h
 * Author: daan
 *
 * Created on October 30, 2017, 10:30 AM
 */

#ifndef CONTROLSOCKET_H
#define CONTROLSOCKET_H

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <signal.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <array>

#include <functional>

typedef std::function<void(std::string * command)> handleString_function;
typedef std::function<void(std::string * command, handleString_function)> handleCommand_function;

class ControlSocket {
public:
    ControlSocket(std::string controlPipeName);
    ControlSocket(const ControlSocket& orig);
    void tick();
    virtual ~ControlSocket();
    void setCommandHandler(handleCommand_function handler);
private:
    
    bool makeSocketNonblocking(int socketfd);
    void listen(std::string controlPipeName);
    void handleControlCommand(std::string * command);
    
    void handleServerEvent(epoll_event event);
    void handleClientEvent(epoll_event event);
            
    int socketFd;
    int serverSocketFd;
    int epollFd;

    /*
    const char * controlPipeName;
    bool controlPipeCreated;
    */

    static constexpr int max_events = 32;
    
    handleCommand_function commandHandler;

};

#endif /* CONTROLSOCKET_H */

