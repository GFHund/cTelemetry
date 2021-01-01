// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "EventSystem/AbstractEvent.h"

class DiagramView:public wxFrame, public AbstractEvent{
	public:
    // ctor(s)
    DiagramView(wxWindow* parent=(wxWindow *)NULL);
	~DiagramView();
    void event(std::string eventName,EventParam* param = nullptr);
    void updateDiagramm();
    void OnProperiesListClicked(wxCommandEvent& event);

/*
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	*/

	private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};
