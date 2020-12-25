#ifndef __CONVERTER_INTERFACE__
#define __CONVERTER_INTERFACE__

class ConverterInterface{
	public:
	virtual bool isFile(std::string filename) = 0;
	virtual std::string convert(std::string filename) = 0;
}

#endif