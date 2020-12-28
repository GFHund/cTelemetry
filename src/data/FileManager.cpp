#include "FileManager.h"

#include "../vendor/sqlite3/sqlite3.h"
//#include "FileNotFoundException.h"
#include "Exceptions/FileNotFoundException.h"
#include "Exceptions/SQLErrorException.h"
#include "Exceptions/FileOpenErrorException.h"
#include "F1_2020_Converter.h"

FileManager* FileManager::mInstance = 0;

FileManager::FileManager(){
    this->mConverters.push_back(new F1_2020_Converter());
}
FileManager::FileManager(FileManager& fm){}
FileManager::~FileManager(){
    delete mInstance;
    for(auto index = mConverters.begin();index != mConverters.end();index++){
        delete *index;
    }
}

FileManager* FileManager::getInstance(){
    if(!mInstance){
        mInstance = new FileManager();
    }
    return mInstance;
}
void FileManager::openFile(std::string path){
    sqlite3* db;
    int rc = sqlite3_open(path.c_str(),&db);
    //Should never happen but if it happend it should be safe to break up the function
    if(rc){
        
        sqlite3_close(db);
        throw FileNotFoundException(path);
    }
    try{
        if(!this->checkFile(db)){
            for(auto index = mConverters.begin();index != mConverters.end();index++){
            	ConverterInterface* converter = *index;
                if(converter->isFile(db)){
                    sqlite3* convertedDb = converter->convert(path,db);
                    sqlite3_close(db);
                    db = convertedDb;
                    break;
                }
            }
        }
    }catch(SQLErrorException e){
    	std::string errorMsg = std::string(e.what());
    	sqlite3_close(db);
    	throw FileOpenErrorException(errorMsg);
    }
}
bool FileManager::checkFile(sqlite3* db){
    std::string sql = std::string("SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%';");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
        throw SQLErrorException(sqlite3_errmsg(db));
    }
    int ret_code = 0;
    std::vector<std::string> tables;
    bool hasFileInfoTable = false;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
    	const char* name = (const char*)sqlite3_column_text(stmt,0);
        std::string table = std::string(name);
        if(table.compare("file_info") == 0){
            hasFileInfoTable = true;
        }
        //tables.push_back(  );
    }
    sqlite3_finalize(stmt);
    return hasFileInfoTable;
}
