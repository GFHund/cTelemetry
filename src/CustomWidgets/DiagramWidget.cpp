#include "DiagramWidget.h"

#include <utility>
#include <cfloat>
#include <chrono>
#include <fstream>


BEGIN_EVENT_TABLE(DiagramWidget, wxPanel)

    EVT_MOTION(DiagramWidget::mouseMoved)
    EVT_LEFT_DOWN(DiagramWidget::mouseDown)
    EVT_LEFT_UP(DiagramWidget::mouseReleased)
    EVT_RIGHT_DOWN(DiagramWidget::rightClick)
    EVT_LEAVE_WINDOW(DiagramWidget::mouseLeftWindow)
    EVT_KEY_DOWN(DiagramWidget::keyPressed)
    EVT_KEY_UP(DiagramWidget::keyReleased)
    EVT_MOUSEWHEEL(DiagramWidget::mouseWheelMoved)

    // catch paint events
    EVT_PAINT(DiagramWidget::paintEvent)

END_EVENT_TABLE()

DiagramWidget::DiagramWidget(wxFrame* parent,const wxSize& size)
	: wxWindow(parent,wxID_ANY)
{
    mDiagramMinWidth = size.GetWidth();
    mDiagramMinHeight = size.GetHeight();
	SetMinSize(size);
    mShowContextWindow = true;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void DiagramWidget::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void DiagramWidget::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void DiagramWidget::render(wxDC&  dc)
{   
    dc.Clear();
    float overallMinX = FLT_MAX;
    float overallMaxX = FLT_MIN;
    float overallMinY = FLT_MAX;
    float overallMaxY = FLT_MIN;
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        float minX = i->first.getMinXValue();
        float maxX = i->first.getMaxXValue();
        float minY = i->first.getMinYValue();
        float maxY = i->first.getMaxYValue();
        if(overallMaxX < maxX){
            overallMaxX = maxX;
        }
        if(overallMinX > minX){
            overallMinX = minX;
        }
        if(overallMaxY < maxY){
            overallMaxY = maxY;
        }
        if(overallMinY > minY){
            overallMinY = minY;
        }
    }
    
    int originalWidth = mDiagramMinWidth - (PADDING_X * 2);
    int originalHeight = mDiagramMinHeight - (PADDING_Y * 2);
    float xValuePerPixel = originalWidth / (overallMaxX - overallMinX);
    float yValuePerPixel = originalHeight / (overallMaxY - overallMinY);
    int offsetY = PADDING_Y;
    int offsetX = PADDING_X;
    if(overallMinY < 0){
        dc.DrawLine(0,mDiagramMinHeight/2,mDiagramMinWidth,mDiagramMinHeight/2);
        offsetY = mDiagramMinHeight/2;
    }
    std::vector<std::pair<std::string,float>> yValuesAtMouse;
    float xValueAtMouse = (mMouseX - PADDING_Y) / xValuePerPixel;
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        DiagramDataSet dataSet = i->first;
        int iColor = i->second;
        unsigned char* color = (unsigned char*)&iColor;
        dc.SetBrush(wxColour(color[0],color[1],color[2],color[3]));
        yValuesAtMouse.push_back(std::pair<std::string,float>(dataSet.getName(),0));
        float xValueDiff = FLT_MAX;
        for(auto j = dataSet.getIterator();!j.isEnd();j.next()){
            if(!j.hasNext()){
                break;
            }
            
            int x0 = j.getX() * xValuePerPixel + offsetX;
            int y0 = mDiagramMinHeight - j.getY() * yValuePerPixel - offsetY;
            
            if(abs(j.getX() - xValueAtMouse) < xValueDiff){
                xValueDiff = abs(j.getX() - xValueAtMouse);
                yValuesAtMouse[yValuesAtMouse.size()-1].second = j.getY();
            }

            int x1 = j.getNextX() * xValuePerPixel + offsetX;
            int y1 = mDiagramMinHeight - j.getNextY() * yValuePerPixel - offsetY;
            dc.DrawLine(x0,y0,x1,y1);
        }
    }
    if(mShowContextWindow){
        dc.SetBrush(wxColour(255,255,255,0));
        dc.DrawLine(mMouseX,0,mMouseX,mDiagramMinHeight);
        int posX = mMouseX;
        int posY = mMouseY;
        int width = 200;
        int height = 200;
        if(posX + width > mDiagramMinWidth){
            posX -= width;
        }
        if(posY + height > mDiagramMinHeight){
            posY -= height;
        }
        dc.DrawRectangle(posX,posY,width,height);
        std::string xValue = "x Value:";
        
        dc.DrawText(wxString(xValue),posX + 10,posY + 10);
        dc.DrawText(wxString(std::to_string(xValueAtMouse)),posX + 100,posY + 10);
        int j = 1;
        for(auto i = yValuesAtMouse.begin();i!=yValuesAtMouse.end();i++){
            std::string yValue =  i->first;
            yValue += ":";
            //yValue += std::to_string(i->second);
            dc.DrawText(wxString(yValue),posX + 10, posY + 10 + j*20);
            dc.DrawText(wxString(std::to_string(i->second)),posX + 100,posY + 10 + j*20);
            j++;
        }
    }
}

void DiagramWidget::mouseDown(wxMouseEvent& event)
{
    //pressedDown = true;
    paintNow();
}
void DiagramWidget::mouseReleased(wxMouseEvent& event)
{
    //pressedDown = false;
    paintNow();
    
    //wxMessageBox( wxT("You pressed a custom button") );
}
void DiagramWidget::mouseLeftWindow(wxMouseEvent& event)
{
	/*
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
	*/
}

// currently unused events
void DiagramWidget::mouseMoved(wxMouseEvent& event) {
    if(event.Moving()){
        mMouseX = event.GetX();
        mMouseY = event.GetY();
        paintNow();
    }
}
void DiagramWidget::mouseWheelMoved(wxMouseEvent& event) {}
void DiagramWidget::rightClick(wxMouseEvent& event) {}
void DiagramWidget::keyPressed(wxKeyEvent& event) {}
void DiagramWidget::keyReleased(wxKeyEvent& event) {}

void DiagramWidget::addXyDataset(DiagramDataSet dataset, int color){
    mDataSets.push_back(std::pair<DiagramDataSet,int>(dataset,color));
    paintNow();
}
void DiagramWidget::removeXyDataset(int color){
    for(auto i = mDataSets.begin();i != mDataSets.end();i++){
        if(i->second == color){
            mDataSets.erase(i);
            return;
        }
    }
    paintNow();
}
