#include "../vendor/sqlite3/sqlite3.h"
#include "../vendor/cxxopts/cxxopts.hpp"
#include <string>
#include <iostream>
#include <vector>

void executeSql(sqlite3* db, std::string sql);
void copyDriver(sqlite3* inputDb,sqlite3* outputDb, int driverId);
void copyTelemetry(sqlite3* inputDb,sqlite3* outputDb, int driverId,int lapId);
template<typename T>
void copyTelemetrySimpleDataTypes(sqlite3* inputDb,sqlite3* outputDb, int lapId,std::string type);
void copyTelemetryVecType(sqlite3* inputDb,sqlite3* outputDb, int lapId);

int main(int argc, char** argv){
    cxxopts::Options options("SplitOneRound","Extracts a Lap and save it into a new file. Furthermore if two rounds are saved as on it will be splitted");
    options.add_options()
        .add("d,driverId","Driver id of the input file", cxxopts::value<std::string>())
        .add("l,lapId","Lap which should be splitted",cxxopts::value<std::string>())
        .add("i,input","Input File for the generator",cxxopts::value<std::string>())
        .add("o,output","output File",cxxopts::value<std::string>())
        .add("h,help","get this Help Text");

    auto result = options.parse(argc,argv);
    if(result.count("help")){
        std::cout << options.help() << std::endl;
        exit(0);
    }
    std::string driverId = result["driverId"].as<std::string>();
    int iDriverId = std::atoi(driverId.c_str());
    std::string lapId = result["lapId"].as<std::string>();
    int iLapId = std::atoi(lapId.c_str());
    std::string inputFile = result["input"].as<std::string>();
    std::string outputFile = result["output"].as<std::string>();
    sqlite3* inputDb;
    sqlite3* outputDb;
    sqlite3* inMemoryDb;
	

    int rc = sqlite3_open(inputFile.c_str(),&inputDb);
    int rc2 = sqlite3_open(outputFile.c_str(),&outputDb);
    int rc3 = sqlite3_open(":memory:",&inMemoryDb);

    if(rc){
        std::cout << "Could Not Open file: " << inputFile << std::endl;
        return 1;
    }
    if(rc2){
        std::cout << "Could Not Open File: " << outputFile << std::endl;
        return 1;
    }
    if(rc3){
        std::cout << "Could Not In Memory Database" << std::endl;
        return 1;
    }

    std::string fileInfoCreateSql = "CREATE TABLE IF NOT EXISTS file_info(file_property TEXT,file_value TEXT)";
    std::string fileInfoInsertSql = "INSERT INTO file_info(file_property,file_value) VALUES (\"VERSION\",\"1.0.0\")";
    std::string driverCreateSql = "CREATE TABLE IF NOT EXISTS driver(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT,number INTEGER)";
    std::string lapCreateSql = "CREATE TABLE IF NOT EXISTS lap(id INTEGER PRIMARY KEY AUTOINCREMENT, driver INTEGER,lap_number INTEGER,lap_time REAL)";
    std::string lapIndexSql = "CREATE INDEX IF NOT EXISTS lap_index ON lap(driver)";
    std::string legendCreateSql = "CREATE TABLE IF NOT EXISTS legend(key INTEGER, propertyname TEXT, type TEXT)";
    std::string legendInsertSql = "INSERT INTO legend (key,propertyname,type) VALUES (1,\"Speed\",\"int\"),(2,\"throttle\",\"float\"),(3,\"break\",\"float\"),(4,\"steer\",\"float\"),(5,\"rpm\",\"int\"),(6,\"gear\",\"int\"),(7,\"lap Distance\",\"float\"),(8,\"lap time\",\"float\"),(9,\"pos\",\"vec\")";
    std::string referenceUnitCreateSql = "CREATE TABLE IF NOT EXISTS reference_unit(id INTEGER PRIMARY KEY AUTOINCREMENT,lap_id INTEGER, session_time REAL,frame_identifier INTEGER,lap_distance REAL,lap_time REAL,lap_invalid INTEGER)";
    std::string referenceUnitIndexSql = "CREATE INDEX IF NOT EXISTS ref_unit_index ON reference_unit(lap_id,frame_identifier)";
    std::string floatDataCreateSql = "CREATE TABLE IF NOT EXISTS float_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, float_val REAL)";
    std::string intDataCreateSql = "CREATE TABLE IF NOT EXISTS int_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, int_val INTEGER)";
    std::string vecDataCreateSql = "CREATE TABLE IF NOT EXISTS vec_data(id INTEGER PRIMARY KEY AUTOINCREMENT,reference_unit_id INTEGER, property_id INTEGER, x_val REAL,y_val REAL,z_val REAL)";
    
    std::cout << "Create Tables" << std::endl;
    executeSql(inMemoryDb,fileInfoCreateSql);
    executeSql(inMemoryDb,fileInfoInsertSql);
    executeSql(inMemoryDb,driverCreateSql);
    executeSql(inMemoryDb,lapCreateSql);
    executeSql(inMemoryDb,lapIndexSql);
    executeSql(inMemoryDb,legendCreateSql);
    executeSql(inMemoryDb,legendInsertSql);
    executeSql(inMemoryDb,referenceUnitCreateSql);
    executeSql(inMemoryDb,referenceUnitIndexSql);
    executeSql(inMemoryDb,floatDataCreateSql);
    executeSql(inMemoryDb,intDataCreateSql);
    executeSql(inMemoryDb,vecDataCreateSql);
    std::cout << "Copy Driver" << std::endl;
    copyDriver(inputDb,inMemoryDb,iDriverId);
    std::cout << "Copy Telemetry" << std::endl;
    copyTelemetry(inputDb,inMemoryDb,iDriverId,iLapId);

    std::cout << "Copy Telemetry Int" << std::endl;
    copyTelemetrySimpleDataTypes<int>(inputDb,inMemoryDb,iLapId,"int");
    std::cout << "Copy Telemetry Float" << std::endl;
    copyTelemetrySimpleDataTypes<float>(inputDb,inMemoryDb,iLapId,"float");
    std::cout << "Copy Telemetry Vec" << std::endl;
    copyTelemetryVecType(inputDb,inMemoryDb,iLapId);

    sqlite3_backup *pBackup;
    pBackup = sqlite3_backup_init(outputDb, "main", inMemoryDb, "main");
    if( pBackup ){
        (void)sqlite3_backup_step(pBackup, -1);
        (void)sqlite3_backup_finish(pBackup);
        sqlite3_close(inMemoryDb);
    }
    else{
        rc = sqlite3_errcode(outputDb);
        if(rc != SQLITE_DONE){
            std::cout << "Error at writing memory db down " << sqlite3_errmsg(outputDb) << std::endl;
        }
    }

    sqlite3_close(inputDb);
    sqlite3_close(outputDb);
}
void executeSql(sqlite3* db, std::string sql){
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK ){
        std::string errorMsg = sqlite3_errmsg(db);
        std::cout << "SQL Error: "<< errorMsg << " " << sql << std::endl;
        return;
    }
    int retCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(retCode != SQLITE_DONE){
        std::string errorMsg = sqlite3_errmsg(db);
        std::cout << "SQL Error: " << errorMsg << " " << sql << std::endl;
        return;
    }
}
void copyDriver(sqlite3* inputDb,sqlite3* outputDb, int driverId){
    sqlite3_stmt* stmt;
    std::string sql = "SELECT name,number FROM driver WHERE id = :driver";
    if(sqlite3_prepare_v2(inputDb,sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK ){
        std::string errorMsg = sqlite3_errmsg(inputDb);
        std::cout << "SQL Error: " << errorMsg << " " << sql << std::endl;
        return;
    }
    sqlite3_bind_int(stmt,1,driverId);
    int retCode = sqlite3_step(stmt);
    std::string name = "";
    int number = 0;
    if(retCode != SQLITE_ROW){
        std::string errorMsg = sqlite3_errmsg(inputDb);
        std::cout << "SQL Error: " << errorMsg << " " << sql << std::endl;
        return;
    }
    else {
        name += (const char*)sqlite3_column_text(stmt,0);
        number = sqlite3_column_int(stmt,1);
    }
    std::cout << "Driver Name: " << name << std::endl;
    std::cout << "Driver Number: " << number << std::endl;
    sqlite3_finalize(stmt);
    std::string insertSql = "INSERT INTO driver(name,number) VALUES(:name,:number)";
    if(sqlite3_prepare_v2(outputDb,insertSql.c_str(),insertSql.length(),&stmt,NULL) != SQLITE_OK ){
        std::string errorMsg = sqlite3_errmsg(outputDb);
        std::cout << "SQL Error: " << errorMsg << " " << insertSql << std::endl;
        return;
    }
    sqlite3_bind_text(stmt,1,name.c_str(),name.length(),SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,2,number);
    retCode = sqlite3_step(stmt); 
    sqlite3_finalize(stmt);
    if(retCode != SQLITE_DONE){
        std::string errorMsg = sqlite3_errmsg(outputDb);
        std::cout << "SQL Error: " << errorMsg << " " << sql << std::endl;
        return;
    }
}
struct referenceUnit{
    int lapId;
    float sessionTime;
    int frameIdentifier;
    float lapDistance;
    float lapTime;
    int lapInvalid;
};
void copyTelemetry(sqlite3* inputDb,sqlite3* outputDb, int driverId,int lapId){
    sqlite3_stmt* stmt;
    std::string sql = "SELECT lap_id,session_time,frame_identifier,lap_distance,lap_time,lap_invalid FROM reference_unit WHERE lap_id = :lapId AND lap_distance > 0 ORDER BY session_time";
    if(sqlite3_prepare_v2(inputDb,sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << sql << std::endl;
        return;
    }
    sqlite3_bind_int(stmt,1,lapId);
    int ret_code;
    std::vector<referenceUnit> rows;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int lapId = sqlite3_column_int(stmt,0);
        float sessionTime = sqlite3_column_double(stmt,1);
        int frameIdentifier = sqlite3_column_int(stmt,2);
        float lapDistance = sqlite3_column_double(stmt,3);
        float lapTime = sqlite3_column_double(stmt,4);
        int lapInvalid = sqlite3_column_int(stmt,5);
        referenceUnit ref;
        ref.lapId = lapId;
        ref.sessionTime = sessionTime;
        ref.frameIdentifier = frameIdentifier;
        ref.lapDistance = lapDistance;
        ref.lapTime = lapTime;
        ref.lapInvalid = lapInvalid;
        rows.push_back(ref);
    }
    sqlite3_finalize(stmt);

    sqlite3_stmt* stmt2;
    std::string insertSql = "INSERT INTO reference_unit(lap_id,session_time,frame_identifier,lap_distance,lap_time,lap_invalid) VALUES (:lapId,:sessionTime,:frameIdentifier,:lapDistance,:lapTime,:lapInvalid)";
    if(sqlite3_prepare_v2(outputDb,insertSql.c_str(),insertSql.length(),&stmt2,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << insertSql << std::endl;
        return;
    }

    float lastTime = 0.0;
    float lastLapDistance = 0.0;
    int lap = 1;
    bool bFirst = true;
    for(auto i = rows.begin();i!= rows.end();i++){
        if(bFirst){
            bFirst = false;
        } else {
            if(lastLapDistance >= i->lapDistance && abs(lastLapDistance - i->lapDistance) > 1.0f){
                std::cout << lastLapDistance << " " << i->lapDistance << std::endl;
                sqlite3_stmt* stmt3;
                std::string newLapSql = "INSERT INTO lap(driver, lap_number, lap_time) VALUES (:driver,:lapNumber,:lapTime)";
                if(sqlite3_prepare_v2(outputDb,newLapSql.c_str(),newLapSql.length(),&stmt3,NULL) != SQLITE_OK ){
                    std::cout << "SQL Error: " << newLapSql << std::endl;
                }
                sqlite3_bind_int(stmt3,1,1);
                sqlite3_bind_int(stmt3,2,lap);
                sqlite3_bind_double(stmt3,3,lastTime);
                sqlite3_step(stmt3);
                sqlite3_finalize(stmt3);
                lap++;
            }
        }
        sqlite3_bind_int(stmt2,1,lap);
        sqlite3_bind_double(stmt2,2,i->sessionTime);
        sqlite3_bind_int(stmt2,3,i->frameIdentifier);
        sqlite3_bind_double(stmt2,4,i->lapDistance);
        sqlite3_bind_double(stmt2,5,i->lapTime);
        sqlite3_bind_int(stmt2,6,i->lapInvalid);

        sqlite3_step(stmt2);

        sqlite3_clear_bindings(stmt2);
		sqlite3_reset(stmt2);
        
        lastTime = i->lapTime;
        lastLapDistance = i->lapDistance;
    }
    sqlite3_finalize(stmt2);
    sqlite3_stmt* stmt3;
    std::string newLapSql = "INSERT INTO lap(driver, lap_number, lap_time) VALUES (:driver,:lapNumber,:lapTime)";
    if(sqlite3_prepare_v2(outputDb,newLapSql.c_str(),newLapSql.length(),&stmt3,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << newLapSql << std::endl;
    }
    sqlite3_bind_int(stmt3,1,1);
    sqlite3_bind_int(stmt3,2,lap);
    sqlite3_bind_double(stmt3,3,lastTime);
    sqlite3_step(stmt3);
    sqlite3_finalize(stmt3);
} 
template<typename T>
struct simpleTypeTable{
    int frameIdentifier;
    int referenceUnitId;
    int propertyId;
    T value;
};
template<typename T>
void copyTelemetrySimpleDataTypes(sqlite3* inputDb,sqlite3* outputDb, int lapId,std::string type){
    sqlite3_stmt* stmt;
    std::string sql = "SELECT frame_identifier,reference_unit_id,property_id,";
    sql += type;
    sql += "_val FROM ";
    sql += type;
    sql += "_data LEFT JOIN reference_unit ON reference_unit_id = reference_unit.id WHERE lap_id = :lapId AND lap_distance > 0  ORDER BY session_time";
    if(sqlite3_prepare_v2(inputDb,sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << sql << std::endl;
        return;
    }
    sqlite3_bind_int(stmt,1,lapId);
    int ret_code;

    std::vector< simpleTypeTable<T> > rows;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int frame_identifier = sqlite3_column_int(stmt,0);
        int reference_unit_id = sqlite3_column_int(stmt,1);
        int property_id = sqlite3_column_int(stmt,2);
        
        
        simpleTypeTable<T> ref;
        ref.frameIdentifier = frame_identifier;
        ref.referenceUnitId = reference_unit_id;
        ref.propertyId = property_id;
        if(type.compare("int") == 0){
            int value = sqlite3_column_int(stmt,3);
            ref.value = value;
        }else if(type.compare("float") == 0){
            float value = sqlite3_column_double(stmt,3);
            ref.value = value;
        }
        
        
        rows.push_back(ref);
    }
    sqlite3_finalize(stmt);

    std::string insertSql = "INSERT INTO ";
    insertSql += type;
    insertSql += "_data(reference_unit_id,property_id,";
    insertSql += type;
    insertSql += "_val) VALUES(:referenceUnitId,:propertyId,:value)";

    if(sqlite3_prepare_v2(outputDb,insertSql.c_str(),insertSql.length(),&stmt,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << insertSql << std::endl;
        return;
    }
    std::string selectSql = "SELECT id FROM reference_unit WHERE frame_identifier = :frameIdentifier";
    sqlite3_stmt* stmt2;
    if(sqlite3_prepare_v2(outputDb,selectSql.c_str(),selectSql.length(),&stmt2,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << selectSql << std::endl;
        return;
    }

    for(auto i = rows.begin(); i!=rows.end();i++){
        sqlite3_bind_int(stmt2,1,i->frameIdentifier);
        ret_code = sqlite3_step(stmt2);
        if(ret_code != SQLITE_ROW){
            sqlite3_clear_bindings(stmt2);
		    sqlite3_reset(stmt2);
            std::cout << "Skippe simple Type " << ret_code << " " << i->frameIdentifier <<std::endl;
            continue;
        }
        int id = sqlite3_column_int(stmt2,0);

        sqlite3_bind_int(stmt,1,id);
        sqlite3_bind_int(stmt,2,i->propertyId);
        if(type.compare("int") == 0){
            sqlite3_bind_int(stmt,3,i->value);
        }
        else if(type.compare("float")==0){
            sqlite3_bind_double(stmt,3,i->value);
        }
        sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
		sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt2);
		sqlite3_reset(stmt2);
    }
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    
}
struct vecTable{
    int frameIdentifier;
    int referenceUnitId;
    int propertyId;
    float xValue;
    float yValue;
    float zValue;
};
void copyTelemetryVecType(sqlite3* inputDb,sqlite3* outputDb, int lapId){
    sqlite3_stmt* stmt;
    std::string sql = "SELECT frame_identifier,reference_unit_id,property_id,x_val,y_val,z_val FROM vec_data LEFT JOIN reference_unit ON reference_unit_id = reference_unit.id WHERE lap_id = :lapId AND lap_distance > 0 ORDER BY session_time";
    if(sqlite3_prepare_v2(inputDb,sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << sql << std::endl;
        return;
    }
    sqlite3_bind_int(stmt,1,lapId);
    int ret_code;

    std::vector< vecTable > rows;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int frame_identifier = sqlite3_column_int(stmt,0);
        int reference_unit_id = sqlite3_column_int(stmt,1);
        int property_id = sqlite3_column_int(stmt,2);
        int xValue = sqlite3_column_double(stmt,3);
        int yValue = sqlite3_column_double(stmt,4);
        int zValue = sqlite3_column_double(stmt,5);
        
        vecTable ref;
        ref.frameIdentifier = frame_identifier;
        ref.referenceUnitId = reference_unit_id;
        ref.propertyId = property_id;
        ref.xValue = xValue;
        ref.yValue = yValue;
        ref.zValue = zValue;
         
        rows.push_back(ref);
    }
    sqlite3_finalize(stmt);

    std::string insertSql = "INSERT INTO vec_data(reference_unit_id,property_id,x_val,y_val,z_val) VALUES(:referenceUnitId,:propertyId,:xValue,:yValue,:zValue)";

    if(sqlite3_prepare_v2(outputDb,insertSql.c_str(),insertSql.length(),&stmt,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << insertSql << std::endl;
        return;
    }
    std::string selectSql = "SELECT id FROM reference_unit WHERE frame_identifier = :frameIdentifier";
    sqlite3_stmt* stmt2;
    if(sqlite3_prepare_v2(outputDb,selectSql.c_str(),selectSql.length(),&stmt2,NULL) != SQLITE_OK ){
        std::cout << "SQL Error: " << selectSql << std::endl;
        return;
    }

    for(auto i = rows.begin(); i!=rows.end();i++){
        sqlite3_bind_int(stmt2,1,i->frameIdentifier);
        ret_code = sqlite3_step(stmt2);
        if(ret_code != SQLITE_ROW){
            std::cout << "Skippe vec Type " << ret_code << " " << i->frameIdentifier <<std::endl;
            sqlite3_clear_bindings(stmt2);
		    sqlite3_reset(stmt2);
            continue;
        }
        int id = sqlite3_column_int(stmt2,0);

        sqlite3_bind_int(stmt,1,id);
        sqlite3_bind_int(stmt,2,i->propertyId);
        sqlite3_bind_double(stmt,3,i->xValue);
        sqlite3_bind_double(stmt,4,i->yValue);
        sqlite3_bind_double(stmt,5,i->zValue);
        
        sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
		sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt2);
		sqlite3_reset(stmt2);
    }
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    
}