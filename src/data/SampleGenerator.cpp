#include "SampleGenerator.h"

#include "Exceptions/SQLErrorException.h"
#include "Exceptions/SQLErrorException.h"
#include "Exceptions/FileNotFoundException.h"
#include <math.h>
#include <iostream>

void SampleGenerator::insertDriversTable(sqlite3* f1Db,sqlite3* convertedDb){
    sqlite3_stmt* stmt;
    std::string insertSql = "INSERT INTO driver(name,number) VALUES (\"Sin func\",1),(\"Cos Func\",2)";
    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
    int ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}

void SampleGenerator::insertLapTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string insertSql = "INSERT INTO lap(driver,lap_number,lap_time) VALUES (1,1,51.00),(1,2,51.300),(2,1,51.150),(2,2,51.251)";
    sqlite3_stmt* stmt;
	int ret_code;
    std::cout << "insertSql: " << insertSql << std::endl;

    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
        std::string errorMsg = sqlite3_errmsg(convertedDb);
        std::cout << "Error: " << errorMsg << std::endl;
		throw SQLErrorException(errorMsg,insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}

void SampleGenerator::insertReferenceDataTable(sqlite3* f1Db,sqlite3* convertedDb){
    std::string insertSql = "INSERT INTO reference_unit(lap_id, lap_distance, lap_time, session_time, frame_identifier,lap_invalid) VALUES ";
    sqlite3_stmt* stmt;
	int ret_code;

    const float pi = 3.1415;
    for(float i = 0; i < 2*pi;i+=(pi/18) ){
        if(i != 0){
            insertSql += ",";
        }
        insertSql += "(1,";
        insertSql += std::to_string(i);
        insertSql += ",";
        insertSql += std::to_string(i);
        insertSql += ",0.0,0,0),";
        insertSql += "(2,";
        insertSql += std::to_string(i);
        insertSql += ",";
        insertSql += std::to_string(i);
        insertSql += ",0.0,0,0),";
        insertSql += "(3,";
        insertSql += std::to_string(i);
        insertSql += ",";
        insertSql += std::to_string(i);
        insertSql += ",0.0,0,0),";
        insertSql += "(4,";
        insertSql += std::to_string(i);
        insertSql += ",";
        insertSql += std::to_string(i);
        insertSql += ",0.0,0,0)";
    }

    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
        std::string errorMsg = std::string(sqlite3_errmsg(convertedDb));
        throw SQLErrorException(errorMsg,insertSql);
    }
    ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
        throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
    }
}

void SampleGenerator::insertFloatDataTable(sqlite3* f1Db,sqlite3* convertedDb){
    sqlite3_stmt* stmt;
	int ret_code;
	std::string insertSql = "INSERT INTO float_data(reference_unit_id, property_id, float_val) VALUES ";

    std::string convertedSelect = "SELECT ref.id,ref.lap_distance FROM reference_unit ref";

    if(sqlite3_prepare_v2(convertedDb,convertedSelect.c_str(),convertedSelect.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),convertedSelect);
	}
    bool bFirst = true;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int id = sqlite3_column_int(stmt,0);
        float value = sqlite3_column_double(stmt,1);
        if(bFirst){
            bFirst = false;
        }
        else{
            insertSql += ",";
        }
        float targetValue = sin(value);
        insertSql += "(";
        insertSql += std::to_string(id);
        insertSql += ",2,";
        insertSql += std::to_string(targetValue);
        insertSql += ")";
    }   

    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}

void SampleGenerator::insertIntDataTable(sqlite3* f1Db,sqlite3* convertedDb){}

void SampleGenerator::insertVec3DataTable(sqlite3* f1Db,sqlite3* convertedDb){}


bool SampleGenerator::isFile(sqlite3* db){
    return true;
}
sqlite3* SampleGenerator::convert(std::string path,sqlite3* db){
    sqlite3* convertedDb;
    int rc = sqlite3_open("Sample_Generator.sqlite3",&convertedDb);
    if(rc){
        //sqlite3_close(db);
        throw FileNotFoundException(path);
    }
    try{
		
        this->createFileInfoTable(convertedDb);
		this->insertFileInfoTable(convertedDb);

        this->createDriversTable(db, convertedDb);
		this->insertDriversTable(db, convertedDb);

        this->createLapTable(db, convertedDb);
		this->insertLapTable(db, convertedDb);

        this->createLegendTable(convertedDb);
		this->insertLegendTable(convertedDb);

        createReferenceDataTable(db, convertedDb);
        insertReferenceDataTable(db, convertedDb);
        
        createFloatDataTable(db, convertedDb);
        insertFloatDataTable(db, convertedDb);

        createIntDataTable(db, convertedDb);
        insertIntDataTable(db, convertedDb);

        createVec3DataTable(db, convertedDb);
        insertVec3DataTable(db, convertedDb);
        
        return convertedDb;
    }catch(SQLErrorException e){
        throw e;
    }
}