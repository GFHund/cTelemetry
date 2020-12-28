#include "FileNotFoundException.h"
FileNotFoundException::FileNotFoundException(std::string file){
    this->file = file;
}
const char* FileNotFoundException::what() const throw()
{
    std::string ret = std::string("File Not found:");
    ret += this->file;
    return ret.c_str();
}