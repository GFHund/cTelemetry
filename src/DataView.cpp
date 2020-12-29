#include "DataView.h"

#include <wx/xrc/xmlres.h>
#include <wx/listbox.h>
#include <wx/dataview.h>
#include <string>
#include "data/FileManager.h"
#include "data/Exceptions/FileNotFoundException.h"
#include "data/Exceptions/FileOpenErrorException.h"
#include "data/Exceptions/NotFoundException.h"
#include "data/Exceptions/SQLErrorException.h"

BEGIN_EVENT_TABLE(DataView, wxFrame)
	EVT_BUTTON(XRCID("mFileOpenButton"), DataView::OnFileOpenButton)
	EVT_BUTTON(XRCID("mCloseFile"), DataView::OnFileCloseButton)
	EVT_LISTBOX(XRCID("mOpenFiles"),DataView::OnFileOpenListSelectItem)
END_EVENT_TABLE()

DataView::DataView(wxWindow* parent){
	wxXmlResource::Get()->LoadFrame(this, parent, "DataView");

	wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);
	fileDataList->AppendTextColumn(wxT("Spieler"));
	fileDataList->AppendTextColumn(wxT("Runde"));
	fileDataList->AppendTextColumn(wxT("Rundenzeit"));
	
	wxDataViewListCtrl* analyseList = XRCCTRL(*this, "mAnalyseList", wxDataViewListCtrl);
	analyseList->AppendTextColumn(wxT("Datei"));
	analyseList->AppendTextColumn(wxT("Spieler"));
	analyseList->AppendTextColumn(wxT("Runde"));
	analyseList->AppendTextColumn(wxT("Rundenzeit"));
	analyseList->AppendTextColumn(wxT("Farbe"));
	
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

	std::vector<std::string> fileNames = FileManager::getInstance()->getOpenFiles();
	std::vector<wxString> wxFileNames;
	for(auto i=fileNames.begin();i!= fileNames.end();i++){
		wxFileNames.push_back(wxString(*i));
	}

	wxListBox* pFileOpenList = XRCCTRL(*this, "mOpenFiles", wxListBox);
	pFileOpenList->InsertItems(wxFileNames.size(),wxFileNames.data(),0);
}
void DataView::OnFileCloseButton(wxCommandEvent& event){
	wxListBox* pFileOpenList = XRCCTRL(*this, "mOpenFiles", wxListBox);
	int selectionIndex = pFileOpenList->GetSelection();
	wxString selectionString = pFileOpenList->GetString(selectionIndex);
	if(FileManager::getInstance()->closeFile(selectionString.ToStdString())){
		pFileOpenList->Delete(selectionIndex);
		wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);
		fileDataList->DeleteAllItems();
	}
	
}

void DataView::OnFileOpenListSelectItem(wxCommandEvent& event){
	wxListBox* pFileOpenList = XRCCTRL(*this, "mOpenFiles", wxListBox);
	wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);

	int selectionIndex = pFileOpenList->GetSelection();
	wxString selectionString = pFileOpenList->GetString(selectionIndex);

	try{
		std::vector<FileTableData> fileDataTable = FileManager::getInstance()->getOpenDbFileByName(selectionString.ToStdString()).getFileTable();
		fileDataList->DeleteAllItems();
		for(auto i = fileDataTable.begin();i!= fileDataTable.end();i++){
			wxVector<wxVariant> data;
			std::string lap = std::to_string(i->getLap());
			std::string lapTime = std::to_string(i->getLapTime());
			data.push_back( wxVariant(i->getPlayer()) );
			data.push_back( wxVariant(lap) );
			data.push_back( wxVariant(lapTime) );
			fileDataList->AppendItem(data);
		}
	} catch(SQLErrorException e){
		std::string message = "Error: ";
		message += e.what();
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_ERROR);
		msg->ShowModal();
	}
}