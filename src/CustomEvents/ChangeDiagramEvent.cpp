#include "ChangeDiagramEvent.h"

ChangeDiagramEvent::ChangeDiagramEvent(wxEventType eventType, int winid, float xAxis)
:wxCommandEvent(eventType,winid){
    this->mXAxis = xAxis;
}

wxEvent* ChangeDiagramEvent::Clone() const{
    return new ChangeDiagramEvent(*this);
}
float ChangeDiagramEvent::getAxisX(){
    return this->mXAxis;
}