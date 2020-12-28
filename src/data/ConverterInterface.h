#ifndef __CONVERTER_INTERFACE__
#define __CONVERTER_INTERFACE__
#include "../vendor/sqlite3/sqlite3.h"
#include <string>
class ConverterInterface{
	public:
	virtual bool isFile(sqlite3* db) = 0;
	virtual sqlite3* convert(std::string path,sqlite3* db) = 0;
};

#endif
