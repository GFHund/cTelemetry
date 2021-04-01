#include "TrackViewWidget.h"

#include <wx/dcbuffer.h>
#include "../data/vector.h"

BEGIN_EVENT_TABLE(TrackViewWidget, wxPanel)
    EVT_PAINT(TrackViewWidget::paintEvent)
END_EVENT_TABLE()

TrackViewWidget::TrackViewWidget(wxFrame* parent,const wxSize& size)
: wxWindow(parent,wxID_ANY)
{
    mTrackMinWidth = size.GetWidth()- 10;
    mTrackMinHeight = size.GetHeight() - 10;
    
    mColors.push_back(std::make_pair<int,int>(0x0000000A,0x000000ff));
    mColors.push_back(std::make_pair<int,int>(0x00000A00,0x0000ff00));
    mColors.push_back(std::make_pair<int,int>(0x000A0000,0x00ff0000));
    mColors.push_back(std::make_pair<int,int>(0x000A0A00,0x00ffff00));
    
}

void TrackViewWidget::paintEvent(wxPaintEvent& evt){
    wxBufferedPaintDC bufferDc(this);
    render(bufferDc);
}
void TrackViewWidget::paintNow(){
    wxClientDC dc(this);
    wxBufferedDC bufferDc(&dc,wxSize(mTrackMinWidth,mTrackMinHeight));
    render(bufferDc);
}
void TrackViewWidget::render(wxDC& dc){
    dc.Clear();
    float trackWidth = mOverallMaxX - mOverallMinX;
    float trackHeight = mOverallMaxY - mOverallMinY;
    float trackScalingX = this->mTrackMinWidth / trackWidth;
    float trackScalingY = this->mTrackMinHeight / trackHeight;
    //Map should be distorion-free
    float trackScaling = trackScalingY;
    if(trackScalingX < trackScalingY){
        trackScaling = trackScalingX;
    }
    float translateX = 0;
    float translateY = 0;
    if(mOverallMinX < 0){
        translateX = (mOverallMinX * trackScaling) * -1 + 10;
    }
    if(mOverallMinY < 0){
        translateY = (mOverallMinY * trackScaling) * -1 + 10;
    }
    int dataSetNumber = 0;
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        TrackDataSet dataSet = *i;
        int iMinColor = mColors[dataSetNumber].first;
        int iMaxColor = mColors[dataSetNumber].second;
        unsigned char* cMinColor = (unsigned char*)&iMinColor;
        unsigned char* cMaxColor = (unsigned char*)&iMaxColor;
        dogEngine::CVector3 maxColor = dogEngine::CVector3(cMaxColor[0],cMaxColor[1],cMaxColor[2]);
        dogEngine::CVector3 minColor = dogEngine::CVector3(cMinColor[0],cMinColor[1],cMinColor[2]);
        float valueMax = dataSet.getMaxValue();
        float valueMin = dataSet.getMinValue();
        for(auto j = dataSet.getIterator();!j.isEnd();j.next()){
            if(!j.hasNext()){
                break;
            }
            float value = j.getValue();
            float valuePercent = (value - valueMin) / (valueMax - valueMin);
            dogEngine::CVector3 valueColor = (minColor - maxColor) * valuePercent + maxColor;

            float x0 = j.getVector().getX() * trackScaling + translateX;
            float y0 = j.getVector().getY() * trackScaling + translateY;
            float x1 = j.getNextVector().getX() * trackScaling + translateX;
            float y1 = j.getNextVector().getY() * trackScaling + translateY;

            dc.SetPen(wxPen(wxColour(valueColor.getX(),valueColor.getY(),valueColor.getZ())));
            dc.DrawLine(x0,y0,x1,y1);
        }
        dataSetNumber++;
    }
    dogEngine::CVector2 point = this->mPoint * trackScaling;
    point = point + dogEngine::CVector2(translateX,translateY);
    dc.SetPen(wxPen(wxColour(0,0,0)));
    dc.DrawLine(point.getX()-5,point.getY()-5,point.getX()+5,point.getY()+5);
    dc.DrawLine(point.getX()-5,point.getY()+5,point.getX()+5,point.getY()-5);
    
}
void TrackViewWidget::addTrackDataset(TrackDataSet dataset){
    this->mDataSets.push_back(dataset);
    calculateOverallMinMax();
    paintNow();
}
void TrackViewWidget::clearDataSet(){
    this->mDataSets.clear();
    paintNow();
    calculateOverallMinMax();
}
void TrackViewWidget::calculateOverallMinMax(){
    mOverallMinX = FLT_MAX;
    mOverallMaxX = FLT_MIN;
    mOverallMinY = FLT_MAX;
    mOverallMaxY = FLT_MIN;
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        float minX = i->getMinXValue();
        float maxX = i->getMaxXValue();
        float minY = i->getMinYValue();
        float maxY = i->getMaxYValue();
        if(mOverallMaxX < maxX){
            mOverallMaxX = maxX;
        }
        if(mOverallMinX > minX){
            mOverallMinX = minX;
        }
        if(mOverallMaxY < maxY){
            mOverallMaxY = maxY;
        }
        if(mOverallMinY > minY){
            mOverallMinY = minY;
        }
    }
}

void TrackViewWidget::setPoint(dogEngine::CVector2 vec){
    this->mPoint = vec;
    this->paintNow();
}