#include "DataView.h"

#include "wx/xrc/xmlres.h"
#include <string>

BEGIN_EVENT_TABLE(DataView, wxFrame)
	EVT_BUTTON(XRCID("mFileOpenButton"), DataView::OnFileOpenButton)
END_EVENT_TABLE()

DataView::DataView(wxWindow* parent){
	wxXmlResource::Get()->LoadFrame(this, parent, "DataView");
}
DataView::~DataView(){}

void DataView::OnFileOpenButton(wxCommandEvent& event){
	wxFileDialog openFileDialog(this,_("Open Telemetry File"),"","","Telemetry files(*.sqlite3)|*.sqlite3",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL){
		return;
	}
	std::string path = openFileDialog.GetPath().ToStdString();

}