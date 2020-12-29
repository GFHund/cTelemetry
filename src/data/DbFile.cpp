#include "DbFile.h"

#include "Exceptions/SQLErrorException.h"
DbFile::DbFile(sqlite3* db,std::string name){
    mDb = db;
    mName = name;
}
DbFile::~DbFile(){
    //sqlite3_close(this->mDb);
}
std::string DbFile::getName(){
    return this->mName;
}
std::vector<FileTableData> DbFile::getFileTable(){
    std::string sql = "SELECT driver.name,driver.number,lap.lap_number,lap.lap_time FROM driver INNER JOIN lap ON lap.driver = driver.id";
    sqlite3_stmt* stmt;
	int ret_code;
    std::vector<FileTableData> data;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        std::string name = (const char*)sqlite3_column_text(stmt,0);
        int driverNumber = sqlite3_column_int(stmt,1);
        int lapNumber = sqlite3_column_int(stmt,2);
        float lapTime = sqlite3_column_double(stmt,3);
        std::string finalName = std::to_string(driverNumber) + "|" + name;
        data.push_back(FileTableData(finalName,lapNumber,lapTime));
    }
    sqlite3_finalize(stmt);
    return data;
}