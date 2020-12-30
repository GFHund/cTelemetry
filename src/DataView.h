// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include <wx/dataview.h>
#endif
class DataView: public wxFrame{
	private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();

    void addToAnalyseTable(int row);
    void removeFromAnalyseTable(int row);
	public:
    // ctor(s)
    DataView(wxWindow* parent=(wxWindow *)NULL);
    ~DataView();

    void OnFileOpenButton(wxCommandEvent& event);
    void OnFileCloseButton(wxCommandEvent& event);
    void OnFileOpenListSelectItem(wxCommandEvent& event);

    void OnFileDataListDClick(wxDataViewEvent& event);
    void OnAddAnalyseButton(wxCommandEvent& event);

    void OnAnalyseDataListDClick(wxDataViewEvent& event);
    void OnDeleteAnalyseButton(wxCommandEvent& event);
};