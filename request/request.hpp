#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../header.hpp"
#include "../response/response.hpp"
// #include "../client/client.hmpp"

#define MAX_HEADER 8190
#define MEGA 1000000
class request {

    private:


        std::stringstream                   _message;
        std::string                         _http;
        std::string                         _key;
        std::string                         _value;

        bool getRequestLine(void); // get the request line
        void getHeaders(void); // get headers and store theme in multimap
        void postBodyInfos(std::string &infos); // in case of a post method must have body infos

    public:
        std::multimap<std::string, std::string>             _msgrequest;
        std::multimap<std::string, std::string>::iterator   _it;
        std::vector<std::string>                            _chunksVector;
        std::vector<std::string>                            _chunkSizeVector;
        std::string                         _method;
        std::string                         _URI; // includes file name
        std::string                         _boundary;
        std::string                         _finaleBoundary;
        std::string                         _connection;
        std::string                         _filename;
        std::string                         _contentType;
        std::string                         _body;
        std::string                         _headers;
        std::string                         _chunks;
        int                                 _contentLength;
        int                                 _statusCode;
        int                                 _bodyInfoSize;
        int                                 _chunkSize;
        bool                                _chunkedTransfer;
        int                                 _headersSize;
        bool                                _multipleFiles;
        int                                 _position;
        int                                 _position1;
        int                                 _position2;


        request();
        void                                requestHeader(std::string& buffer);

        // POST REQUEST METHODS
        void                                postMethod(client &_client);
        void                                erasePostRequestHeaders(client &_client);
        void                                normalPostRequestBody(std::string &buffer, int boundaryPosition, client &_client);
        void                                chunkedPostRequestBody(std::string &buffer, client &_client);
        std::vector<std::string>            ft_split(const std::string &str, const std::string &del);

        // GET REQUEST METHODS




        ~request();

};


#endif
