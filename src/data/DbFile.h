#ifndef __DB_FILE__
#define __DB_FILE__
#include "../vendor/sqlite3/sqlite3.h"
#include "FileTableData.h"
#include <string>
#include <vector>
#include <map>

class DbFile{
	private:
        sqlite3* mDb;
        std::string mName;
    public:
        DbFile(sqlite3* db,std::string name);
        ~DbFile();

        std::string getName();
        std::vector<FileTableData> getFileTable();
        std::map<int,std::string> getYValues();
};

#endif
