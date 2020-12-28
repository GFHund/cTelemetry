#include "FileOpenErrorException.h"

FileOpenErrorException::FileOpenErrorException(std::string errorMsg){
    mErrorMsg = errorMsg;
}
const char* FileOpenErrorException::what() const throw(){
    std::string error = std::string("Error in the SQL Statement: ");
    error += mErrorMsg;
    return error.c_str();
}