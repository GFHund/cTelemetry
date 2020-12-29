#ifndef __NOT_FOUND_EXCEPTION__
#define __NOT_FOUND_EXCEPTION__

#include <exception>
#include <string>

class NotFoundException: public std::exception{
    private:

    public:
    NotFoundException();
    virtual const char* what() const throw();
};

#endif
