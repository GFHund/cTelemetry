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
    void createFileInfoTable(sqlite3* convertedDb);
    void createDriversTable(sqlite3* f1Db,sqlite3* convertedDb);
    void createLapTable(sqlite3* f1Db,sqlite3* convertedDb);
    void createLegendTable(sqlite3* convertedDb);
    void createDataTables(sqlite3*f1Db,sqlite3* convertedDb);
    void createReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    void createFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    void createIntDataTable(sqlite3* f1Db,sqlite3* convertedDb);
    void createVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb);

    public:
    bool isFile(sqlite3* db);
	sqlite3* convert(std::string path,sqlite3* db);
};
#endif
