#include "TrackDataSet.h"

TrackDataSet::TrackDataSet(std::string name, std::vector< std::pair<dogEngine::CVector2,float> > data){
    this->mData = data;
    this->mName = name;

    mMaxXValue = FLT_MIN;
    mMinXValue = FLT_MAX;
    mMaxYValue = FLT_MIN;
    mMinYValue = FLT_MAX;
    mMinValue = FLT_MAX;
    mMaxValue = FLT_MIN;
    for(auto i = mData.begin();i!= mData.end();i++){
        if(i->first.getX() > mMaxXValue){
            mMaxXValue = i->first.getX();
        }
        if(i->first.getX() < mMinXValue){
            mMinXValue = i->first.getX();
        }
        if(i->first.getY() > mMaxYValue){
            mMaxYValue = i->first.getY();
        }
        if(i->first.getY() < mMinYValue){
            mMinYValue = i->first.getY();
        }
        if(i->second < mMinValue){
            mMinValue = i->second;
        }
        if(i->second > mMaxValue){
            mMaxValue = i->second;
        }
    }
}

TrackDataSet::Iterator TrackDataSet::getIterator(){
    return Iterator(mData.begin(),mData.end());
}

float TrackDataSet::getMaxXValue(){
    return this->mMaxXValue;
}
float TrackDataSet::getMinXValue(){
    return this->mMinXValue;
}
float TrackDataSet::getMaxYValue(){
    return this->mMaxYValue;
}
float TrackDataSet::getMinYValue(){
    return this->mMinYValue;
}
std::string TrackDataSet::getName(){
    return this->mName;
}

float TrackDataSet::getMaxValue(){
    return this->mMaxValue;
}
float TrackDataSet::getMinValue(){
    return this->mMinValue;
}

TrackDataSet::Iterator::Iterator(std::vector< std::pair<dogEngine::CVector2,float> >::iterator start, std::vector< std::pair<dogEngine::CVector2,float> >::iterator end){
    mStart = start;
    mEnd = end;
}
bool TrackDataSet::Iterator::isEnd(){
    return mStart == mEnd;
}
void TrackDataSet::Iterator::next(){
    mStart++;
}
dogEngine::CVector2 TrackDataSet::Iterator::getVector(){
    return mStart->first;
}
float TrackDataSet::Iterator::getValue(){
    return mStart->second;
}
bool TrackDataSet::Iterator::hasNext(){
    mStart++;
    bool ret = mStart != mEnd;
    mStart--;
    return ret;
}
dogEngine::CVector2 TrackDataSet::Iterator::getNextVector(){
    mStart++;
    dogEngine::CVector2 vec = mStart->first;
    mStart--;
    return vec;
}
float TrackDataSet::Iterator::getNextValue(){
    mStart++;
    float value = mStart->second;
    mStart--;
    return value;
}
