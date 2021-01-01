#include "FileOpenErrorException.h"

FileOpenErrorException::FileOpenErrorException(std::string errorMsg){
    mErrorMsg = errorMsg;
}
const char* FileOpenErrorException::what() const throw(){
    std::string error = std::string("File Open Error Exception: ");
    error += mErrorMsg;
    return error.c_str();
}