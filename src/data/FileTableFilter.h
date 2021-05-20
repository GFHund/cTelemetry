#ifndef __FILE_TABLE_FILTER__
#define __FILE_TABLE_FILTER__
#include <string>
#include "../vendor/sqlite3/sqlite3.h"
class FileTableFilter{
    public:
    std::string mSearchString;
    int mRound;

    FileTableFilter(){
        mSearchString = "";
        mRound = -1;
    }

    std::string getWhere(){
        if(mSearchString.length() <= 0  || mRound <= -1){
            return "";
        }
        std::string ret = " WHERE";
        
        if(mSearchString.length() > 0){
            ret += " driver.name LIKE :search";
        }
        if(mRound > -1){
            ret += " lap.lap_number = :lapNumber";
        }
        return ret;
    }
    void bindStmt(sqlite3_stmt* stmt){
        int index = 1;
        if(mSearchString.length() > 0){
            sqlite3_bind_text(stmt,index,mSearchString.c_str(),mSearchString.length(),SQLITE_TRANSIENT);
            index++;
        }
        if(mRound > -1){
            sqlite3_bind_int(stmt,index,mRound);
            index++;
        }
        
    }
};
#endif