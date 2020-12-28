#ifndef __FILE_NOT_FOUND_EXCEPTION__
#define __FILE_NOT_FOUND_EXCEPTION__
#include <exception>
#include <string>
class FileNotFoundException: public std::exception{
    private:
    std::string file;
    public:
    FileNotFoundException(std::string file);
    virtual const char* what() const throw();
};
#endif