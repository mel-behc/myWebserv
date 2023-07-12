#include "server.hpp"

server::server() {
    if (!_getaddrinfo())
        exit(1);

    if (!_socket())
        exit(1);

    if (!_setsocket()) 
        exit(1);

    if (!_bind()) // in case of error must close the listner
        exit(1);

    freeaddrinfo(result); 

    if(!_listen())
        exit(1);

    _add_descriptor(socketFd);
    
    // pos_read = 0;
    bytesCounter = 0;
    while (1) {
        if (_poll() == -1)
            break ; // poll call fialed
        for (size_t i = 0; i < pfds.size(); i++) {
            if (pfds[i].revents & POLLIN) {
                if (pfds[i].fd == socketFd)
                    _accept();   // Listening descriptor is readable. 
                else
                    _receive(i); // check the receiving of data
            }
            else if (pfds[i].events & POLLOUT) {
                _body = _response.headers_generator(_request._status_code);
                bytesSend = send(pfds[i].fd, _body.c_str(), _body.size(), 0);
                if (bytesSend < 0) {
                    std::cout << strerror(errno) << '\n';
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                }
                // if (_request._connexion != "keep-alive") {
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                    _body.erase();
                // }
            }
        }
    }
}


bool server::_getaddrinfo(void) {
    // void for the moment after that i must pass the result of the parser <multimap>
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    if ((getaddrinfo("localhost", PORT, &hints, &result)) < 0) {
        std::cout <<  gai_strerror(errno);
        return (false);
    }
    return (true);
}

int server::_socket(void) {
    socketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socketFd < 0) {
        if ((fcntl(socketFd, F_SETFL, O_NONBLOCK) < 0)) //  to check the non blocking after tests
            std::cout << strerror(errno);
        std::cout << strerror(errno);
        exit (1);
    }
    return (socketFd);
}

bool server::_setsocket(void) {
    int setsock = 1;
    if ((setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&setsock, sizeof(setsock))) < 0) {
        std::cout << strerror(errno);
        return (false);
    }
    return (true);
}

bool server::_bind(void) {
    if ((bind(socketFd, result->ai_addr, result->ai_addrlen)) < 0) {
        std::cout << strerror(errno);
        return (false);
    }
    return (true);
}

bool server::_listen(void) {
    if ((listen(socketFd, SOMAXCONN)) < 0) {
        std::cout << strerror(errno);
        return (false);
    }
    return (true);
}

int server::_poll(void) {
    int total_fds;
    total_fds = 0;
    total_fds = poll(pfds.data(), pfds.size(), -1);
    if (total_fds < 0) {
        std::cout << strerror(errno);
        return (0);
    }
    return (total_fds);
}

int server::_accept(void) {
    clientaddrln = sizeof(client);
    clinetFd = accept(socketFd, (struct sockaddr*)&client, &clientaddrln);
    if (clinetFd < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            std::cout << "after cheking the errno \n";
        else
            std::cout << strerror(errno);
    }
    else
        _add_descriptor(clinetFd); // fill the pfds vector
    return (clinetFd);
}

void server::_add_descriptor(int fd) {
    pollfds.fd = fd;
    pollfds.events = POLLIN;
    pfds.push_back(pollfds);
}

void server::_receive(int index) {
    memset(buffer, 0, BUFFSIZE);
    bytesRecv = recv(pfds[index].fd, buffer, (BUFFSIZE - 1), 0);
    if (bytesRecv < 0) {
        std::cout << strerror(errno) << '\n';
        close(pfds[index].fd);
        pfds.erase(pfds.begin() + index);
        exit(0) ;
    }
    _tmpBody.append(buffer, bytesRecv);
    if (bytesCounter == 0) { // stor headers in a multi-map and erase them from the body
        _request._ParseRequestHeaders(_tmpBody);
        _path = _request._uri;
    }
    bytesCounter += bytesRecv; // to accelerate the receive the data by not checking the body every time
    if (_request._method == "POST") {
        position1 = _tmpBody.find(_request._boundary);
        if (position1 != -1) {
            if(_request._chunkedTransfer)
                _request._parseChunkedRequestBody(_tmpBody);
            else
                _request._parseNormalRequestBody(_tmpBody, position1);
            _tmpBody.erase(0, position1);
            _request._nbrFiles = 1;
            _request._ParseRequestHeaders(_tmpBody);
        }
        // FINALE STAPE
        position2 = _tmpBody.find(_request._finaleBoundary); // find the last boundary
        if (position2  != -1) { // End of receiving
            while ((position1 = _tmpBody.find(_request._boundary)) != -1) {
                if(_request._chunkedTransfer)
                    _request._parseChunkedRequestBody(_tmpBody);
                else
                    _request._parseNormalRequestBody(_tmpBody, position1);
                _tmpBody.erase(0, position1);
                _request._nbrFiles = 1;
                _request._ParseRequestHeaders(_tmpBody);
            }
            // SEND THE LAST DATA 
            if (_request._chunkedTransfer)
                _request._parseChunkedRequestBody(_tmpBody);
            else
                _request._parseNormalRequestBody(_tmpBody, position2);
            bytesCounter = 0;
            _tmpBody.clear();
            _request._msgrequest.clear();
            pfds[index].events = POLLOUT;
        }
    }
}

// void    server::_requestParsing(int position, std::string &_tmpBody) {
//     if(_request._chunkedTransfer)
//         _request._parseChunkedRequestBody(_tmpBody);
//     else
//         _request._parseNormalRequestBody(_tmpBody, position);
//     _tmpBody.erase(0, position);
//     _request._nbrFiles = 1;
//     _request._ParseRequestHeaders(_tmpBody);
// }

server::~server() {}

// GET METHOD
// if (_request._method == "GET") {
//     std::cout << "GET method \n";
//     if (pos_read == 0) {
//         _response.get_content_type(_path);
//         _body = _response.headers_generator(_request._status_code);
//         _body += readFile();
//         pos_read++;
//     }
//     if (_body.size() > 0) {
//         bytesCounter = _body.size() / 10;
//         if (_body.size() < BUFFSIZE)
//             bytesCounter = _body.size();
//         bytesSend = send(pfds[i].fd, _body.c_str(), bytesCounter, 0);
//         if (bytesSend < 0)
//             std::cout << strerror(errno) << '\n';
//         _body.erase(0, bytesSend);
//     }
//     else {
//         close(pfds[i].fd);
//         pfds.erase(pfds.begin() + i);
//         _body.erase();
//         pos_read = 0;
//     }
// }

// std::string	server::readFile(void) { // RETURN THE CONTENT OF A FILE AS A STD::STRING
// 	std::ifstream 	file;
// 	std::string text;
// 	std::ostringstream streambuff;
// 	file.open(_path);
// 	if (file.is_open()) {
// 		streambuff << file.rdbuf();
// 		text = streambuff.str();
// 	}
// 	file.close();
// 	return text;
// }

// int server::get_file_size(void) {
//     std::ifstream _file(_path, std::ios::in | std::ios::binary | std::ios::ate);
//     if (!_file.is_open())
//         std::cout << "error kabiiiiiir \n";
//     std::streampos _fileSize = _file.tellg();
//     int contentLength = static_cast<int>(_fileSize);
//     _file.close();
//     return (contentLength);
// }