#ifndef __TRACK_VIEW__
#define __TRACK_VIEW__

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <string>
#include "EventSystem/AbstractEvent.h"
#include "data/vector.h"

class TrackView: public wxFrame, public AbstractEvent{
    
    private:
    wxDECLARE_EVENT_TABLE();
    std::string mPropertyName;

    public:
    TrackView(wxWindow* parent=(wxWindow *)NULL,std::string propertyName = "");
	~TrackView();
    void event(std::string eventName,EventParam* param = nullptr);
    void updateListBoxes();
    void setPointInTrackView(dogEngine::CVector3 pos);
    void addDataToTrackView();

    void OnProperiesListClicked(wxCommandEvent& event);
    void OnDriverListClicked(wxCommandEvent& event);  
};

#endif