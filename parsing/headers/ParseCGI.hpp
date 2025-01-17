#ifndef PARSECGI_HPP
#define PARSECGI_HPP

#include <string>
#include "./utils.hpp"


class ParseCGI {

	public:
		ParseCGI();
		ParseCGI(std::string str);
		~ParseCGI();
		ParseCGI(const ParseCGI &obj);

		ParseCGI &operator=(const ParseCGI &obj);
		const std::string &GetFileExtention() const ;
		const std::string &GetFilePath() const ;
		size_t GetLen() const ;


	private:

		std::string _Path;
		std::string _File;
		size_t _Len;

		void OneArg(std::string str) ;
		void TwoArgs(std::string str);

		void SetFile(std::string str);

		void SetExecutableToArg(std::string str);


};

#endif
