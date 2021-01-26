#include "DbFile.h"

#include "Exceptions/SQLErrorException.h"
#include <utility>
#include "../data/Exceptions/FileOpenErrorException.h"
#include <fstream>
#include <thread>

DbFile::DbFile(sqlite3* db,std::string name){
    mDb = db;
    mName = name;
    //std::thread generateYProperties;//getYProperties
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
std::map<int,std::string> DbFile::getYProperties(){
    if(this->mYProperties.size() > 0){
        return mYProperties;
    }
    std::string sql = "SELECT legend.key, legend.propertyname FROM legend INNER JOIN float_data ON float_data.property_id = legend.key GROUP BY float_data.property_id";
    std::string sql2 = "SELECT legend.key, legend.propertyname FROM legend INNER JOIN int_data ON int_data.property_id = legend.key GROUP BY int_data.property_id";
    sqlite3_stmt* stmt;
	int ret_code;
    std::map<int,std::string> yValues;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int key = sqlite3_column_int(stmt,0);
        std::string name = (const char*)sqlite3_column_text(stmt,1);
        yValues.insert(std::pair<int,std::string>(key,name));
    }
    sqlite3_finalize(stmt);
    if(sqlite3_prepare_v2(this->mDb,sql2.c_str(),sql2.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql2);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int key = sqlite3_column_int(stmt,0);
        std::string name = (const char*)sqlite3_column_text(stmt,1);
        yValues.insert(std::pair<int,std::string>(key,name));
    }
    sqlite3_finalize(stmt);
    mYProperties = yValues;
    return yValues;
}
int DbFile::getKeyFromPropertiesName(std::string propertiesName){
    std::string sql = "SELECT key FROM legend WHERE propertyname LIKE \"";
    sql += propertiesName;
    sql += "\"";
    sqlite3_stmt* stmt;
	int ret_code;
    int ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        ret = sqlite3_column_int(stmt,0);
        
    }
    sqlite3_finalize(stmt);
    return ret;
}
std::string DbFile::getPropertiesNameFromKey(int key){
    std::string sql = "SELECT propertyname FROM legend WHERE key = ";
    sql += key;
    sqlite3_stmt* stmt;
	int ret_code;
    std::string ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        ret = (const char*)sqlite3_column_text(stmt,0);
        
    }
    sqlite3_finalize(stmt);
    return ret;
}
int DbFile::getLapId(AnalyseData metaData){
    int driverNumber = getDriverId(metaData);
    std::string sql = "SELECT id FROM lap WHERE lap_number =  ";
    sql += std::to_string(metaData.getLap());
    sql += " AND driver = ";
    sql += std::to_string(driverNumber);
    sqlite3_stmt* stmt;
	int ret_code;
    int ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        ret = sqlite3_column_int(stmt,0);
        
    }
    sqlite3_finalize(stmt);
    return ret;
}
int DbFile::getDriverId(AnalyseData metaData){
    int pos = metaData.getPlayer().find('|');
    std::string playerName; 
    if(pos == std::string::npos){
        playerName = metaData.getPlayer();
    }else{
        playerName = metaData.getPlayer().substr(pos+1);
    }
    std::string sql = "SELECT id FROM driver WHERE name LIKE \"";
    sql += playerName;
    sql += "\"";
    sqlite3_stmt* stmt;
	int ret_code;
    int ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        ret = sqlite3_column_int(stmt,0);
        
    }
    sqlite3_finalize(stmt);
    return ret;
}
std::string DbFile::getKeyType(int key){
    std::string sql = "SELECT type FROM legend WHERE key = ";
    sql += std::to_string(key);
    sqlite3_stmt* stmt;
	int ret_code;
    std::string ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        ret = (const char*)sqlite3_column_text(stmt,0);
        
    }
    sqlite3_finalize(stmt);
    return ret;
}
DiagramDataSet DbFile::getValues(AnalyseData metaData, int xProperties, std::string propertiesName){
    try{
        int key = getKeyFromPropertiesName(propertiesName);
        return getValues(metaData,xProperties,key);    
    } catch(std::exception e){
        throw FileOpenErrorException("push_back has thrown an error");
    }
    
}
DiagramDataSet DbFile::getValues(AnalyseData metaData, int xProperties, int key){
    int lapId = getLapId(metaData);
    std::string typeTable = getKeyType(key);
    std::string sql = "SELECT ";
    switch(xProperties){
        case 0:
            sql += "lap_distance,";
            break;
        case 1:
            sql += "lap_time,";
    }
    sql += typeTable;
    sql += "_val";
    sql += " FROM reference_unit INNER JOIN ";
    sql += typeTable;
    sql += "_data as data_table ON data_table.reference_unit_id = reference_unit.id WHERE property_id = ";
    sql += std::to_string(key);
    sql += " AND lap_id = ";
    sql += std::to_string(lapId);

    sqlite3_stmt* stmt;
	int ret_code;
    std::vector< std::pair<float,float> > ret;
    if(sqlite3_prepare_v2(this->mDb,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(this->mDb),sql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        float xAxis = sqlite3_column_double(stmt,0);
        float yAxis;
        if(typeTable.compare("int") == 0){
            yAxis = sqlite3_column_int(stmt,1);
        } else if(typeTable.compare("float") == 0){
            yAxis = sqlite3_column_double(stmt,1);
        }
        ret.push_back(std::pair<float,float>(xAxis,yAxis));        
    }
    sqlite3_finalize(stmt);
    DiagramDataSet dataset = DiagramDataSet(metaData.getPlayer(),ret);
    return dataset;
}
