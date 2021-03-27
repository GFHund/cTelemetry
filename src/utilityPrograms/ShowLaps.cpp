#include "../vendor/sqlite3/sqlite3.h"
#include "../vendor/cxxopts/cxxopts.hpp"
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char** argv){
    cxxopts::Options options("ShowLaps","Shows laps of the file");
    options.add_options()
        .add("i,input","Input File for the generator",cxxopts::value<std::string>())
        .add("h,help","get this Help Text");

    auto result = options.parse(argc,argv);
    if(result.count("help")){
        std::cout << options.help() << std::endl;
        exit(0);
    }
    std::string inputFile = result["input"].as<std::string>();
    sqlite3* inputDb;
    int rc = sqlite3_open(inputFile.c_str(),&inputDb);
    if(rc){
        std::cout << "Could Not Open file: " << inputFile << std::endl;
        return 1;
    }
    std::string lapListSql = "SELECT lap.id,lap.lap_number,lap.lap_time,driver.id,driver.name,driver.number FROM lap LEFT JOIN driver ON lap.driver = driver.id";

    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(inputDb,lapListSql.c_str(),lapListSql.length(),&stmt,NULL) != SQLITE_OK ){
        std::string errorMsg = sqlite3_errmsg(inputDb);
        std::cout << "SQL Error: "<< errorMsg << " " << lapListSql << std::endl;
        return 1;
    }
    int ret_code;
    std::cout << "driver Id | driver Name | driver number | lap id | lap number | lap Time" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int lapId = sqlite3_column_int(stmt,0);
        int lapNumber = sqlite3_column_int(stmt,1);
        float lapTime = sqlite3_column_double(stmt,2);
        int driverId = sqlite3_column_int(stmt,3);
        std::string driverName = (const char*) sqlite3_column_text(stmt,4);
        int driverNumber = sqlite3_column_int(stmt,5);
        std::cout << driverId << " | " << driverName << " | " << driverNumber << " | " << lapId << " | " << lapNumber << " | " << lapTime << std::endl;
    }
    sqlite3_finalize(stmt);
    return 0;
}