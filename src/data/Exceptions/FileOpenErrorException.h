#ifndef __FILE_OPEN_EXCEPTION__
#define __FILE_OPEN_EXCEPTION__

#include <exception>
#include <string>

class FileOpenErrorException: public std::exception{
    private:
    std::string mErrorMsg;

    public:
    FileOpenErrorException(std::string errorMsg);
    virtual const char* what() const throw();
};

#endif