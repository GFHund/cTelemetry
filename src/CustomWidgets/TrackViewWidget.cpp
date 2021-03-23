#include "TrackViewWidget.h"

#include <wx/dcbuffer.h>

BEGIN_EVENT_TABLE(TrackViewWidget, wxPanel)
    EVT_PAINT(TrackViewWidget::paintEvent)
END_EVENT_TABLE()

TrackViewWidget::TrackViewWidget(wxFrame* parent,const wxSize& size)
: wxWindow(parent,wxID_ANY)
{
    mTrackMinWidth = size.GetWidth()- 10;
    mTrackMinHeight = size.GetHeight() - 10;
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
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        TrackDataSet dataSet = *i;
        for(auto j = dataSet.getIterator();!j.isEnd();j.next()){
            if(!j.hasNext()){
                break;
            }
            float x0 = j.getVector().getX() * trackScaling + translateX;
            float y0 = j.getVector().getY() * trackScaling + translateY;
            float x1 = j.getNextVector().getX() * trackScaling + translateX;
            float y1 = j.getNextVector().getY() * trackScaling + translateY;

            dc.DrawLine(x0,y0,x1,y1);
        }
        
    }
    
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
    //mOverallMinY -= 10;
    //mOverallMinX -= 10;

}