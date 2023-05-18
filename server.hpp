#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <exception>
#include <poll.h>
#include <unistd.h> // sysconf(), _SC_OPEN_MAX
#include <cstring>
#define PORT "9430"

class server
{
    std::vector<pollfd> fds;
    // std::vector<pollfd>::iterator it;

    public:
        server();
        ~server();
};

#endif