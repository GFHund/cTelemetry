#include "DataView.h"

#include <wx/xrc/xmlres.h>
#include <wx/listbox.h>

#include <string>
#include "data/FileManager.h"
#include "data/Exceptions/FileNotFoundException.h"
#include "data/Exceptions/FileOpenErrorException.h"
#include "data/Exceptions/NotFoundException.h"
#include "data/Exceptions/SQLErrorException.h"
#include "EventSystem/EventManager.h"

BEGIN_EVENT_TABLE(DataView, wxFrame)
	EVT_BUTTON(XRCID("mFileOpenButton"), DataView::OnFileOpenButton)
	EVT_BUTTON(XRCID("mCloseFile"), DataView::OnFileCloseButton)
	EVT_LISTBOX(XRCID("mOpenFiles"),DataView::OnFileOpenListSelectItem)
	EVT_DATAVIEW_ITEM_ACTIVATED(XRCID("mFileDataList"), DataView::OnFileDataListDClick)
	EVT_BUTTON(XRCID("mAddLap"),DataView::OnAddAnalyseButton)

	EVT_DATAVIEW_ITEM_ACTIVATED(XRCID("mAnalyseList"), DataView::OnAnalyseDataListDClick)
	EVT_BUTTON(XRCID("mDeleteLap"),DataView::OnDeleteAnalyseButton)
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
	wxFileNames.clear();
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

void DataView::OnFileDataListDClick(wxDataViewEvent& event){
	wxDataViewItem item = event.GetItem();
	wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);
	
	int row = fileDataList->ItemToRow(item);
	addToAnalyseTable(row);
}
void DataView::OnAddAnalyseButton(wxCommandEvent& event){
	wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);
	int selectedRow = fileDataList->GetSelectedRow(); 
	if(selectedRow != wxNOT_FOUND){
		addToAnalyseTable(selectedRow);
	}
	else {
		std::string message = "Please select a Row in the upper table";
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_INFORMATION);
		msg->ShowModal();
	}
}
void DataView::addToAnalyseTable(int row){
	wxDataViewListCtrl* fileDataList = XRCCTRL(*this, "mFileDataList", wxDataViewListCtrl);
	wxString player = fileDataList->GetTextValue(row,0);
	wxString sLap = fileDataList->GetTextValue(row,1);
	wxString sLapTime = fileDataList->GetTextValue(row,2);
	int iLap = std::stoi(sLap.ToStdString());
	float fLapTime = std::stof(sLapTime.ToStdString());
	
	wxListBox* pFileOpenList = XRCCTRL(*this, "mOpenFiles", wxListBox);
	int selectionIndex = pFileOpenList->GetSelection();
	wxString filename = pFileOpenList->GetString(selectionIndex);
	AnalyseData metaData = AnalyseData(filename.ToStdString(),player.ToStdString(),iLap,fLapTime,0);
	
	if(FileManager::getInstance()->addActiveLap(metaData)){
		wxDataViewListCtrl* analyseDataList = XRCCTRL(*this, "mAnalyseList", wxDataViewListCtrl);
		wxVector<wxVariant> data;
		data.push_back(wxVariant(filename));
		data.push_back(wxVariant(player));
		data.push_back(wxVariant(sLap));
		data.push_back(wxVariant(sLapTime));
		data.push_back(wxVariant("0"));
		analyseDataList->AppendItem(data);
		fileDataList->UnselectRow(row);
		EventManager::getInstance()->fireEvent("updateDiagramm");
	}
	else {
		std::string message = "Could not push the lap to the analyse because it is only ";
		message += std::to_string(FileManager::getInstance()->getMaxNumberOfActiveLaps());
		message += " Data allowed";
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_INFORMATION);
		msg->ShowModal();
	}
	
}

void DataView::OnAnalyseDataListDClick(wxDataViewEvent& event){
	wxDataViewItem item = event.GetItem();
	wxDataViewListCtrl* AnalyseDataList = XRCCTRL(*this, "mAnalyseList", wxDataViewListCtrl);
	
	int row = AnalyseDataList->ItemToRow(item);
	removeFromAnalyseTable(row);
}
void DataView::OnDeleteAnalyseButton(wxCommandEvent& event){
	wxDataViewListCtrl* AnalyseDataList = XRCCTRL(*this, "mAnalyseList", wxDataViewListCtrl);
	int selectedRow = AnalyseDataList->GetSelectedRow(); 
	if(selectedRow != wxNOT_FOUND){
		removeFromAnalyseTable(selectedRow);
	}
	else {
		std::string message = "Please select a Row in the lower table";
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_INFORMATION);
		msg->ShowModal();
	}
}

void DataView::removeFromAnalyseTable(int row){
	wxDataViewListCtrl* AnalyseDataList = XRCCTRL(*this, "mAnalyseList", wxDataViewListCtrl);
	wxString filename = AnalyseDataList->GetTextValue(row,0);
	wxString player = AnalyseDataList->GetTextValue(row,1);
	wxString sLap = AnalyseDataList->GetTextValue(row,2);
	wxString sLapTime = AnalyseDataList->GetTextValue(row,3);
	int iLap = std::stoi(sLap.ToStdString());
	float fLapTime = std::stof(sLapTime.ToStdString());
	AnalyseData metaData = AnalyseData(filename.ToStdString(),player.ToStdString(),iLap,fLapTime,0);
	if(FileManager::getInstance()->removeActiveLap(metaData)){
		AnalyseDataList->DeleteItem(row);
		EventManager::getInstance()->fireEvent("updateDiagramm");
	}
	else {
		std::string message = "Could not delete row";
		wxString wxMessage = wxString(message);
		wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_INFORMATION);
		msg->ShowModal();
	}
}