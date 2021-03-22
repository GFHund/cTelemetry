#ifndef __CHANGE_DIAGRAM_EVENT__
#define __CHANGE_DIAGRAM_EVENT__
#include <wx/wx.h>
#include <wx/sizer.h>
class ChangeDiagramEvent: public wxEvent{
    private:
        float mXAxis;
    public:
        ChangeDiagramEvent(wxEventType eventType, int winid, float xAxis);
        float getAxisX();
        virtual wxEvent* Clone() const;
};

#endif