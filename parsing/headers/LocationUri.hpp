#ifndef LOCATIONURI_HPP
#define LOCATIONURI_HPP

#include "./Uri.hpp"
#include "./utils.hpp"

//https://www.f5.com/pdf/white-papers/http-fundamentals-wp.pdf
class LocationUri {
	private:
		Uri _Uri;
		bool _IsDir;

	public:
		LocationUri();
		LocationUri(std::string str);

		~LocationUri() ;
		LocationUri(const LocationUri &obj);
		LocationUri &operator=(LocationUri const &obj) ;
		
		
		std::string GetUri()const ;
		Uri GetUriOBJ() const ;

		std::string GetInputURI() const;
		bool IsDir() const ;

};



#endif
