#ifndef __DB_FILE__
#define __DB_FILE__
#include "../vendor/sqlite3/sqlite3.h"
#include "FileTableData.h"
#include "AnalyseData.h"
#include <string>
#include <vector>
#include <map>
#include <thread>
#include "DiagramDataSet.h"
#include "TrackDataSet.h"
#include "vector.h"
#include "FileTableFilter.h"

class DbFile{
	private:
        sqlite3* mDb;
        std::string mName;
        std::map<int,std::string> mYProperties;

        int getLapId(AnalyseData metaData);
        int getDriverId(AnalyseData metaData);
        std::string getKeyType(int key);
    public:
        DbFile(sqlite3* db,std::string name);
        ~DbFile();

        std::string getName();
        std::vector<FileTableData> getFileTable(FileTableFilter* filter = nullptr);
        std::map<int,std::string> getYProperties();
        int getKeyFromPropertiesName(std::string PropertiesName);
        std::string getPropertiesNameFromKey(int key);
        DiagramDataSet getValues(AnalyseData metaData, int xProperties, std::string propertiesName);
        DiagramDataSet getValues(AnalyseData metaData, int xProperties, int key);
        TrackDataSet getTrackValues(AnalyseData metaData, std::string propertiesName);
        TrackDataSet getTrackValues(AnalyseData metaData, int key);
        dogEngine::CVector3 get3DPosFromDistance(float distance);
        
};

#endif
