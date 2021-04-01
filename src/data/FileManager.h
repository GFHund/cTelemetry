#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__
#include <string>
#include <vector>
#include "DbFile.h"
#include "ConverterInterface.h"
#include "AnalyseData.h"

class FileManager{
	private:
	std::vector<DbFile> mFiles;
	std::vector<ConverterInterface*> mConverters;
	std::vector<AnalyseData> mActiveLaps;
	const int MAX_ACTIVE_LAPS = 4;
	
	protected:
	static FileManager* mInstance;

	FileManager();
	FileManager(FileManager& fm);
	~FileManager();
	
	bool checkFile(sqlite3* db);
	public:
	static FileManager* getInstance();
	void openFile(std::string path);

	std::vector<std::string> getOpenFiles();
	bool closeFile(std::string name);
	DbFile& getOpenDbFileByName(std::string name);
	DbFile& getFirstDbFile();

	bool addActiveLap(AnalyseData lapMetaData);
	bool removeActiveLap(AnalyseData lapMetaData);
	AnalyseData& getActiveLap(int index);
	int getNumberOfActiveLaps();
	int getMaxNumberOfActiveLaps();

	unsigned int getNextColor();
	unsigned int getColor(int index);
	
};

#endif
