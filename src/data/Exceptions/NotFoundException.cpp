#include "NotFoundException.h"

NotFoundException::NotFoundException(){
}
const char* NotFoundException::what() const throw()
{
    std::string error = std::string("Error Not Found Exception");
    return error.c_str();
}
