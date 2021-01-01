#include "DiagramDataSet.h"

#include <cfloat>

DiagramDataSet::DiagramDataSet(std::string name,std::vector< std::pair<float,float> > data){
    mName = name;
    mData = data;
    mMaxXValue = FLT_MIN;
    mMinXValue = FLT_MAX;
    mMaxYValue = FLT_MIN;
    mMinYValue = FLT_MAX;
    for(auto i = mData.begin();i!= mData.end();i++){
        if(i->first > mMaxXValue){
            mMaxXValue = i->first;
        }
        if(i->first < mMinXValue){
            mMinXValue = i->first;
        }
        if(i->second > mMaxYValue){
            mMaxYValue = i->second;
        }
        if(i->second < mMinYValue){
            mMinYValue = i->second;
        }
    }
}

DiagramDataSet::Iterator DiagramDataSet::getIterator(){
    return Iterator(mData.begin(),mData.end());
}

float DiagramDataSet::getMaxXValue(){
    return mMaxXValue;
}
float DiagramDataSet::getMinXValue(){
    return mMinXValue;
}
float DiagramDataSet::getMaxYValue(){
    return mMaxYValue;
}
float DiagramDataSet::getMinYValue(){
    return mMinYValue;
}
std::string DiagramDataSet::getName(){
    return mName;
}

DiagramDataSet::Iterator::Iterator(std::vector< std::pair<float,float> >::iterator start, std::vector< std::pair<float,float> >::iterator end){
    mStart = start;
    mEnd = end;
}
bool DiagramDataSet::Iterator::isEnd(){
    return mStart == mEnd;
}
void DiagramDataSet::Iterator::next(){
    mStart++;
}
float DiagramDataSet::Iterator::getX(){
    return mStart->first;
}
float DiagramDataSet::Iterator::getY(){
    return mStart->second;
}
bool DiagramDataSet::Iterator::hasNext(){
    mStart++;
    bool ret = mStart != mEnd;
    mStart--;
    return ret;
}
float DiagramDataSet::Iterator::getNextX(){
    mStart++;
    float x = mStart->first;
    mStart--;
    return x;
}
float DiagramDataSet::Iterator::getNextY(){
    mStart++;
    float y = mStart->second;
    mStart--;
    return y;
}