#ifndef CLIENTMAXBODYSIZE_HPP
#define CLIENTMAXBODYSIZE_HPP

#include <string>
#include <stdexcept>
#include <cctype> // std::isdigit
#include "./utils.hpp"

// https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
// if client_max_body_size is smaller than the file size trying to be uploaded it gives a:
// 413 error (Request Entity too large)

class ClientMaxBodySize {
	private:
		size_t _val;
	public:
		ClientMaxBodySize();
		ClientMaxBodySize(std::string val);
		~ClientMaxBodySize();
		size_t GetVal() const;
};

#endif
