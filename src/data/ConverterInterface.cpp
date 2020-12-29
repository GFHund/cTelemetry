#include "ConverterInterface.h"

#include "Exceptions/SQLErrorException.h"
#include "Exceptions/SQLErrorException.h"
#include "Exceptions/FileNotFoundException.h"

void ConverterInterface::createFileInfoTable(sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS file_info(file_property TEXT,file_value TEXT)";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
    }
    int ret_code = 0;
    ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
    }
}

void ConverterInterface::insertFileInfoTable(sqlite3* convertedDb){
    sqlite3_stmt* stmt;
    std::string insertSql = "INSERT INTO file_info(file_property,file_value) VALUES (\"VERSION\",\"1.0.0\")";
    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
    }
    int ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
    }
}

void ConverterInterface::createDriversTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS driver(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT,number INTEGER)";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
    }
    int ret_code = 0;
    ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
    }   
}

void ConverterInterface::createLapTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS lap(id INTEGER PRIMARY KEY AUTOINCREMENT, driver INTEGER,lap_number INTEGER,lap_time REAL)";
	sqlite3_stmt* stmt;
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}

void ConverterInterface::createLegendTable(sqlite3* convertedDb){
    //SQL CREATE TABLE
	sqlite3_stmt* stmt;
    std::string createSql = "CREATE TABLE IF NOT EXISTS legend(key INTEGER, propertyname TEXT)";
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}

void ConverterInterface::insertLegendTable(sqlite3* convertedDb){
	
	std::string insertSql = "INSERT INTO legend (key,propertyname) VALUES (1,\"Speed\"),(2,\"throttle\"),(3,\"break\"),(4,\"steer\"),(5,\"rpm\"),(6,\"gear\"),(7,\"lap Distance\"),(8,\"lap time\"),(9,\"pos\")";
	sqlite3_stmt* stmt;
	
	//SQL Insert
	if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	int ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}

void ConverterInterface::createReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS reference_unit(id INTEGER PRIMARY KEY AUTOINCREMENT,lap_id INTEGER, session_time REAL,frame_identifier INTEGER,lap_distance REAL,lap_time REAL,lap_invalid INTEGER)";
	sqlite3_stmt* stmt;
	//CREATE Reference Table(for the x Axis);
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}

void ConverterInterface::createFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS float_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, float_val REAL)";
	sqlite3_stmt* stmt;
	//CREATE float_data Table(for the y Axis);
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}

void ConverterInterface::createIntDataTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS int_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, int_val INTEGER)";
	sqlite3_stmt* stmt;
	//CREATE float_data Table(for the y Axis);
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}

void ConverterInterface::createVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string createSql = "CREATE TABLE IF NOT EXISTS vec_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, x_val REAL,y_val REAL,z_val REAL)";
	sqlite3_stmt* stmt;
	//CREATE float_data Table(for the y Axis);
	if(sqlite3_prepare_v2(convertedDb,createSql.c_str(),createSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
	int ret_code = 0;
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),createSql);
	}
}