#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../header.hpp"
#include "../request/request.hpp"
#include "../response/response.hpp"


#define PORT "9430"
#define BUFFSIZE 16000


class request;

class server {
    

    private:
        std::vector<struct pollfd>  pfds;
        struct addrinfo             hints;
        struct addrinfo             *result;
        struct sockaddr_in          client;
        struct pollfd               pollfds;
        socklen_t                   clientaddrln;
        char                        buffer[16000];
        int                         sockfd;
        int                         cltfd;
        int                         bytes_rcv;
        int                         bytes_send;
        request                     reqobj;
        response                    respoo;
        std::string                 reqmsg;
        std::string                 _msg;
        std::string                 _path;
        std::string                 _headers;
        int                         pos_read;
        // int                         retsend;
        int                         bytToSend;
        std::string                 _test;
        std::ofstream               file;
        int                         pos;
        std::string                 _delimiter;
        // int                         totalpos;


    // SERVER METHODS
        bool    _getaddrinfo(void);
        int     _socket(void);
        bool    _setsocket(void);
        bool    _bind(void);
        bool    _listen(void);
        int     _accept(void);
        int     _poll(void);
        void    _receive(int index);
        void    _add_descriptor(int fd);

    
        // int     get_file_size(void);
        // std::string	readFile(void);

    public:
        server();
        ~server();
};

#endif