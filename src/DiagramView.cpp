#include "DiagramView.h"

#include "CustomWidgets/DiagramWidget.h"

#include "wx/xrc/xmlres.h"
#include "data/FileManager.h"
#include "EventSystem/EventManager.h"
#include "data/Exceptions/NotFoundException.h"
#include "data/Exceptions/SQLErrorException.h"
#include "TrackView.h"

BEGIN_EVENT_TABLE(DiagramView, wxFrame)
    EVT_LISTBOX(XRCID("mPropertiesSelector"), DiagramView::OnProperiesListClicked)
    EVT_MENU(XRCID("mTrackDistance"),DiagramView::OnTrackDistanceSelected)
    EVT_MENU(XRCID("mTime"),DiagramView::OnTimeSelected)
    EVT_MENU(XRCID("mTrackView"),DiagramView::OnTrackView)
    CHANGE_DIAGRAM_EVENT(XRCID("mDiagram"),DiagramView::OnDiagramChange)
END_EVENT_TABLE()

DiagramView::DiagramView(wxWindow* parent){
	
	
	// Load up this frame from XRC. [Note, instead of making a class's
    // constructor take a wxWindow* parent with a default value of NULL,
    // we could have just had designed MyFrame class with an empty
    // constructor and then written here:
    // wxXmlResource::Get()->LoadFrame(this, (wxWindow* )NULL, "DiagramView");
    // since this frame will always be the top window, and thus parentless.
    // However, the current approach has source code that can be recycled
    // for other frames that aren't the top level window.]
    wxXmlResource::Get()->LoadFrame(this, parent, "DiagramView");
	
	// Load the menubar from XRC and set this frame's menubar to it.
    SetMenuBar(wxXmlResource::Get()->LoadMenuBar("mMenu"));
	
	//wxBoxSizer* horizontalBox = XRCCTRL (this,"mHorizontalBox",wxBoxSizer);
	
	DiagramWidget* diagramWidget = new DiagramWidget(this,wxSize(1280,768));
	wxXmlResource::Get()->AttachUnknownControl(wxT("mDiagram"),
		diagramWidget
	);
	
	// Give the frame an optional statusbar. The '1' just means one field.
    // A gripsizer will automatically get put on into the corner, if that
    // is the normal OS behaviour for frames on that platform. Helptext
    // for menu items and toolbar tools will automatically get displayed
    // here.
    //CreateStatusBar( 1 );

    EventManager::getInstance()->subscribe("updateDiagramm",this);
    mPropertyName = "";
}

DiagramView::~DiagramView(){}

void DiagramView::event(std::string eventName,EventParam* param){
    if(eventName.compare("updateDiagramm") == 0){
        updateProperties();
    }
}
void DiagramView::updateProperties(){
    std::map<int,std::string> finalList;
    wxListBox* propertiesListCtrl = XRCCTRL(*this, "mPropertiesSelector", wxListBox);
    for(int i=0;i < FileManager::getInstance()->getNumberOfActiveLaps();i++){
        AnalyseData& data = FileManager::getInstance()->getActiveLap(i);
        std::map<int,std::string> propertiesName = FileManager::getInstance()
            ->getOpenDbFileByName(data.getFilename()).getYProperties();
        for(auto j = propertiesName.begin();j != propertiesName.end();j++){
            finalList.insert(std::pair<int,std::string>(j->first,j->second));
        }
    }
    propertiesListCtrl->Clear();
    for(auto i = finalList.begin();i != finalList.end();i++){
        propertiesListCtrl->Append(wxString(i->second));
    }
}
void DiagramView::OnProperiesListClicked(wxCommandEvent& event){
    updateDiagramm();

    wxListBox* propertiesListCtrl = XRCCTRL(*this, "mPropertiesSelector", wxListBox);
    int selection = propertiesListCtrl->GetSelection();
    wxString selectionString = propertiesListCtrl->GetString(selection);
    EventParam* param = new EventParam();
    param->setString("propertyName",selectionString.ToStdString());
    EventManager::getInstance()->fireEvent("ChangeProperty",param);
}

void DiagramView::OnTrackDistanceSelected(wxCommandEvent& event){
    updateDiagramm();
}
void DiagramView::OnTimeSelected(wxCommandEvent& event){
    updateDiagramm();
}
void DiagramView::updateDiagramm(){
    wxListBox* propertiesListCtrl = XRCCTRL(*this, "mPropertiesSelector", wxListBox);
    wxMenuItem* trackDistanceMenuItem = GetMenuBar()->FindItem(XRCID("mTrackDistance"));//XRCCTRL(*this,"mTrackDistance",wxMenuItem);
    //wxMenuItem* timeDistanceMenuItem = XRCCTRL(*this,"mTime",wxMenuItem);
    wxMenuItem* timeDistanceMenuItem = GetMenuBar()->FindItem(XRCID("mTime"));
    
    int xProperties = 0;
    
    if(trackDistanceMenuItem->IsChecked()){
        xProperties = 0;
    } else if(timeDistanceMenuItem->IsChecked()) {
        xProperties = 1;
    }
    DiagramWidget* diagramWidget = XRCCTRL (*this,"mDiagram",DiagramWidget);
    diagramWidget->clearXyDataset();

    int selection = propertiesListCtrl->GetSelection();
    wxString selectionString = propertiesListCtrl->GetString(selection);
    mPropertyName = selectionString.ToStdString();
    for(int i=0;i < FileManager::getInstance()->getNumberOfActiveLaps();i++){
        try{
            AnalyseData& metaData = FileManager::getInstance()->getActiveLap(i);
            DiagramDataSet data = FileManager::getInstance()
                ->getOpenDbFileByName(metaData.getFilename()).getValues(metaData,xProperties,selectionString.ToStdString());

            diagramWidget->addXyDataset(data,metaData.getColor());
        }catch(NotFoundException e){
            std::string message = "Not Found Exception";
            wxString wxMessage = wxString(message);
            wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"Not Found error",wxOK|wxCENTRE|wxICON_ERROR);
            msg->ShowModal();
            break;
        }
        catch(SQLErrorException e){
            std::string message = e.what();
            wxString wxMessage = wxString(message);
            wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_ERROR);
            msg->ShowModal();
            break;
        }
        catch(std::exception e){
            std::string message = e.what();
            wxString wxMessage = wxString(message);
            wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"SQL error",wxOK|wxCENTRE|wxICON_ERROR);
            msg->ShowModal();
            break;
        }
    }
}

void DiagramView::OnDiagramChange(ChangeDiagramEvent& event){
    float axisX = event.getAxisX();
    EventParam* param = new EventParam();
    param->setFloat("xAxis",axisX);

    EventManager::getInstance()->fireEvent("DiagramChanged",param);
    
    //EventManager::getInstance()->subscribe("updateDiagramm",this);
}
void DiagramView::OnTrackView(wxCommandEvent& event){
    TrackView* trackView = new TrackView(this,mPropertyName);
    trackView->Show();
}