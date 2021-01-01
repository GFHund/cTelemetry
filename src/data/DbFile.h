#ifndef __DB_FILE__
#define __DB_FILE__
#include "../vendor/sqlite3/sqlite3.h"
#include "FileTableData.h"
#include "AnalyseData.h"
#include <string>
#include <vector>
#include <map>
#include "DiagramDataSet.h"

class DbFile{
	private:
        sqlite3* mDb;
        std::string mName;
        int getLapId(AnalyseData metaData);
        int getDriverId(AnalyseData metaData);
        std::string getKeyType(int key);
    public:
        DbFile(sqlite3* db,std::string name);
        ~DbFile();

        std::string getName();
        std::vector<FileTableData> getFileTable();
        std::map<int,std::string> getYProperties();
        int getKeyFromPropertiesName(std::string PropertiesName);
        std::string getPropertiesNameFromKey(int key);
        DiagramDataSet getValues(AnalyseData metaData, int xProperties, std::string propertiesName);
        DiagramDataSet getValues(AnalyseData metaData, int xProperties, int key);
};

#endif
