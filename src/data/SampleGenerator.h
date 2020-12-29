#ifndef __SAMPLE_GENERATOR__
#define __SAMPLE_GENERATOR__
#include "ConverterInterface.h"

class SampleGenerator: public ConverterInterface{
    protected:
    void insertDriversTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertLapTable(sqlite3* f1Db,sqlite3* convertedDb);


    void insertReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    
    void insertIntDataTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb);

    public:
    bool isFile(sqlite3* db);
	sqlite3* convert(std::string path,sqlite3* db);
};

#endif