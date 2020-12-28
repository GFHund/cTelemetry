#ifndef __SQL_ERROR_EXCEPTION__
#define __SQL_ERROR_EXCEPTION__

#include <exception>
#include <string>

class SQLErrorException: public std::exception{
    private:
    std::string mErrorMsg;
    std::string mSql;

    public:
    SQLErrorException(std::string errorMsg, std::string sql = "");
    virtual const char* what() const throw();
};

#endif
