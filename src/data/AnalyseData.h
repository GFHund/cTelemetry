#ifndef __ANALYSE_DATA__
#define __ANALYSE_DATA__
#include "FileTableData.h"
class AnalyseData: public FileTableData{
    protected:
    std::string mFilename;
    int mColor;
    public:
    AnalyseData(std::string filename,std::string player,int lap,float lapTime, int color)
    :FileTableData(player,lap,lapTime){
        this->mFilename = filename;
        this->mColor = color;
    }
    std::string getFilename(){
        return mFilename;
    }
    int getColor(){
        return mColor;
    }
    bool compare(AnalyseData otherLapMetaData){
        if(mFilename.compare(otherLapMetaData.getFilename()) != 0){
            return false;
        }
        if(mLap != otherLapMetaData.getLap()){
            return false;
        }
        if(mPlayer.compare(otherLapMetaData.getPlayer()) != 0){
            return false;
        }
        return true;
    }
};

#endif