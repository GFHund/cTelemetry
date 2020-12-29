#include "vendor/cxxopts/cxxopts.hpp"
#include "data/SampleGenerator.h"
#include "data/F1_2020_Converter.h"
#include "data/Exceptions/FileNotFoundException.h"
#include "data/Exceptions/SQLErrorException.h"
#include <string>
int main(int argc, char** argv){
    cxxopts::Options options("cTelemetryConverter","Converts a foreign source file into the internal format of cTelemetry");
    options.add_options()
        .add("g,generator","Type of the generator. Possible Values are: 'SampleGenerator','F1_2020'", cxxopts::value<std::string>())
        .add("i,input","Input File for the generator",cxxopts::value<std::string>())
        .add("h,help","get this Help Text");

    auto result = options.parse(argc,argv);
    if(result.count("help")){
        std::cout << options.help() << std::endl;
        exit(0);
    }
    try{
        std::string generator = result["generator"].as<std::string>();
        //The Sample Generator should only generate a test file and not convert a file
        if(generator.compare("SampleGenerator") != 0){
            std::string input = result["input"].as<std::string>();
            sqlite3* inputDb;
            int rc = sqlite3_open(input.c_str(),&inputDb);
            if(rc){
                std::cout << "could not open Database" << std::endl;
            }
            if(generator.compare("F1_2020") == 0){
                
                F1_2020_Converter f1_2020;
                if(f1_2020.isFile(inputDb)){
                    f1_2020.convert(input,inputDb);
                }else{
                    std::cout << "Database does not fit to f1 2020 specification" << std::endl;
                    return 1;    
                }
            }else{
                std::cout << "could not find Generator Please use follow generators: 'SampleGenerator','F1_2020'" << std::endl;
                return 1;
            }
        }
        else{
            SampleGenerator sampleGen;
            sampleGen.convert("",nullptr);
        }
    }
    catch(cxxopts::OptionParseException e){
        std::cout << e.what() << std::endl;
        return 1;
    }
    catch(FileNotFoundException e){
        std::cout << e.what() << std::endl;
        return 1;
    }
    catch(SQLErrorException e){
        std::cout << e.what() << std::endl;
        return 1;
    }
}