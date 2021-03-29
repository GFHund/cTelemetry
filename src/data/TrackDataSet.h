#ifndef __TRACK_DATA_SET__
#define __TRACK_DATA_SET__

#include <vector>
#include <string>
#include "vector.h"

class TrackDataSet{
    private:
    std::vector< std::pair<dogEngine::CVector2,float> > mData;
    float mMaxXValue;
    float mMinXValue;
    float mMaxYValue;
    float mMinYValue;
    float mMaxValue;
    float mMinValue;
    std::string mName;

    class Iterator{
        private:
        std::vector< std::pair<dogEngine::CVector2,float> >::iterator mStart;
        std::vector< std::pair<dogEngine::CVector2,float> >::iterator mEnd;
        public: 
        Iterator(std::vector< std::pair<dogEngine::CVector2,float> >::iterator start, std::vector< std::pair<dogEngine::CVector2,float> >::iterator end);

        bool isEnd();
        void next();

        dogEngine::CVector2 getVector();
        float getValue();

        bool hasNext();
        dogEngine::CVector2 getNextVector();
        float getNextValue();
    }; 
    public:
    TrackDataSet(std::string name, std::vector< std::pair<dogEngine::CVector2,float> > data);

    Iterator getIterator();
    float getMaxXValue();
    float getMinXValue();
    float getMaxYValue();
    float getMinYValue();
    float getMaxValue();
    float getMinValue();
    std::string getName();
};

#endif