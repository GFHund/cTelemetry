#include "FileManager.h"

#include "../vendor/sqlite3/sqlite3.h"
//#include "FileNotFoundException.h"
#include "Exceptions/FileNotFoundException.h"
#include "Exceptions/SQLErrorException.h"
#include "Exceptions/FileOpenErrorException.h"
#include "Exceptions/NotFoundException.h"
#include "F1_2020_Converter.h"
#include <fstream>

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

        sqlite3* inMemoryDb;
        int rc3 = sqlite3_open(":memory:",&inMemoryDb);
        if(rc3){
            //std::cout << "Could Not In Memory Database" << std::endl;
            //throw FileNotFoundException("memory");
            mFiles.push_back(DbFile(db,path));
            return;
        }
        sqlite3_backup *pBackup;
        pBackup = sqlite3_backup_init(inMemoryDb, "main", db, "main");
        if( pBackup ){
            (void)sqlite3_backup_step(pBackup, -1);
            (void)sqlite3_backup_finish(pBackup);
            sqlite3_close(db);
            mFiles.push_back(DbFile(inMemoryDb,path));
        }
        else {
            mFiles.push_back(DbFile(db,path));
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
std::vector<std::string> FileManager::getOpenFiles(){
    std::vector<std::string> ret;
    for(auto i = mFiles.begin();i!= mFiles.end();i++){
        ret.push_back(i->getName());
    }
    return ret;
}
bool FileManager::closeFile(std::string name){
    for(auto i = mFiles.begin();i!= mFiles.end();i++){
        if(name.compare(i->getName()) == 0){
            mFiles.erase(i);
            return true;
        }
    }
    return false;
}
DbFile& FileManager::getOpenDbFileByName(std::string name){
    for(auto i = mFiles.begin();i!= mFiles.end();i++){
        if(name.compare(i->getName()) == 0){
            return *i;
        }
    }
    throw NotFoundException();
}
bool FileManager::addActiveLap(AnalyseData lapMetaData){
    if(this->mActiveLaps.size() >= MAX_ACTIVE_LAPS){
        return false;
    }
    this->mActiveLaps.push_back(lapMetaData);
    return true;
}
bool FileManager::removeActiveLap(AnalyseData lapMetaData){
    for(auto i = mActiveLaps.begin();i != mActiveLaps.end();i++){
        if(i->compare(lapMetaData)){
            mActiveLaps.erase(i);
            return true;
        }
    }
    return false;
}
AnalyseData& FileManager::getActiveLap(int index){
    if(index >= mActiveLaps.size()){
        throw NotFoundException();
    }
    return mActiveLaps[index];
}
int FileManager::getNumberOfActiveLaps(){
    return this->mActiveLaps.size();
}
int FileManager::getMaxNumberOfActiveLaps(){
    return MAX_ACTIVE_LAPS;
}
unsigned int FileManager::getNextColor(){
    int index = mActiveLaps.size();
    return getColor(index);
}
unsigned int FileManager::getColor(int index){
    unsigned int color[] = {0xffff0000,0xff00ff00,0xff0000ff,0xffffff00};
    if(index >= MAX_ACTIVE_LAPS){
        return 0;
    }
    return color[index];
}
DbFile& FileManager::getFirstDbFile(){
    if(mFiles.size() < 1){
        throw NotFoundException();
    }
    return mFiles[0];
}