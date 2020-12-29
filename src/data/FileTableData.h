#ifndef __TABLE_DATA__
#define __TABLE_DATA__

#include <string>

class FileTableData{
    protected:
    std::string mPlayer;
    int mLap;
    float mLapTime;

    public:
    FileTableData(std::string player,int lap,float lapTime){
        mPlayer = player;
        mLap = lap;
        mLapTime = lapTime;
    }

    std::string getPlayer(){
        return mPlayer;
    }
    int getLap(){
        return mLap;
    }
    float getLapTime(){
        return mLapTime;
    }
};

#endif