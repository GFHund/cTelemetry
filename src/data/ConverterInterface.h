#ifndef __CONVERTER_INTERFACE__
#define __CONVERTER_INTERFACE__
#include "../vendor/sqlite3/sqlite3.h"
#include <string>
class ConverterInterface{
	protected:
	void createFileInfoTable(sqlite3* convertedDb);
    void insertFileInfoTable(sqlite3* convertedDb);

    void createDriversTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertDriversTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

    void createLapTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertLapTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

    void createLegendTable(sqlite3* convertedDb);
    void insertLegendTable(sqlite3* convertedDb);

    void createReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

    void createFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

    void createIntDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertIntDataTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

    void createVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb);
    virtual void insertVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb) = 0;

	public:
	virtual bool isFile(sqlite3* db) = 0;
	virtual sqlite3* convert(std::string path,sqlite3* db) = 0;
};

#endif
