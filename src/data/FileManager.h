#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__
#include <string>
#include <vector>
#include "DbFile.h"
#include "ConverterInterface.h"

class FileManager{
	private:
	std::vector<DbFile> mFiles;
	std::vector<ConverterInterface*> mConverters;
	
	protected:
	static FileManager* mInstance;

	FileManager();
	FileManager(FileManager& fm);
	~FileManager();
	
	bool checkFile(sqlite3* db);
	public:
	static FileManager* getInstance();
	void openFile(std::string path);
	
};

#endif
