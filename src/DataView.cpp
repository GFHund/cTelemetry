#include "DataView.h"

#include "wx/xrc/xmlres.h"
#include <string>
#include "data/FileManager.h"
#include "data/Exceptions/FileNotFoundException.h"
#include "data/Exceptions/FileOpenErrorException.h"

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

	try{
		std::string path = openFileDialog.GetPath().ToStdString();
		FileManager::getInstance()->openFile(path);

	}catch(FileNotFoundException &e){
		std::string message = "Error: ";
		message += e.what();
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"File Not Found Error",wxOK|wxCENTRE|wxICON_ERROR);
		msg->ShowModal();
	}catch(FileOpenErrorException &e){
		std::string message = "Error: ";
		message += e.what();
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"File Not Found Error",wxOK|wxCENTRE|wxICON_ERROR);
		msg->ShowModal();
	}


}
