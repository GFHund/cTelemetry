#include "SQLErrorException.h"

SQLErrorException::SQLErrorException(std::string errorMsg,std::string sql){
    mErrorMsg = errorMsg;
    mSql = sql;
}
const char* SQLErrorException::what() const throw()
{
    std::string error = std::string("Error in the SQL Statement: ");
    error += mErrorMsg;
    error += " SQL: ";
    error += mSql;
    return error.c_str();
}
