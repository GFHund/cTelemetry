#ifndef __DB_FILE__
#define __DB_FILE__
#include "../vendor/sqlite3/sqlite3.h"
class DbFile{
	private:
        sqlite3* mDb;
    public:
        DbFile(sqlite3* db);
};

#endif
