#ifndef __F1_2020_CONVERTER__
#define __F1_2020_CONVERTER__
#include "ConverterInterface.h"
#include <string>

class F1_2020_Converter: public ConverterInterface{
    protected:
	struct lapArea{
		int lap;
		float sessionTimeMin;
		float sessionTimeMax;
	};

    void insertDriversTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertLapTable(sqlite3* f1Db,sqlite3* convertedDb);

    void createDataTables(sqlite3*f1Db,sqlite3* convertedDb);

    void insertReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    
    void insertIntDataTable(sqlite3* f1Db,sqlite3* convertedDb);

    void insertVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb);

    public:
    bool isFile(sqlite3* db);
	sqlite3* convert(std::string path,sqlite3* db);
};
#endif
