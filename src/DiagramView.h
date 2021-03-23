// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "EventSystem/AbstractEvent.h"
#include "CustomEvents/ChangeDiagramEvent.h"

class DiagramView:public wxFrame, public AbstractEvent{
	private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
    std::string mPropertyName;
    public:
    // ctor(s)
    DiagramView(wxWindow* parent=(wxWindow *)NULL);
	~DiagramView();
    void event(std::string eventName,EventParam* param = nullptr);
    void updateProperties();
    void OnProperiesListClicked(wxCommandEvent& event);
    void OnTrackDistanceSelected(wxCommandEvent& event);
    void OnTimeSelected(wxCommandEvent& event);
    void updateDiagramm();
    void OnDiagramChange(ChangeDiagramEvent& event);
    void OnTrackView(wxCommandEvent& event);

/*
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	*/

	
};
