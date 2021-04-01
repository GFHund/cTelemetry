#include "TrackView.h"
#include "EventSystem/EventManager.h"
#include "CustomWidgets/TrackViewWidget.h"
#include "data/FileManager.h"
#include "wx/xrc/xmlres.h"
#include "data/Exceptions/NotFoundException.h"
#include "data/Exceptions/SQLErrorException.h"
#include "data/Exceptions/FileOpenErrorException.h"
#include <fstream>

BEGIN_EVENT_TABLE(TrackView, wxFrame)
    //EVT_LISTBOX(XRCID("mPropertiesSelector"), TrackView::OnProperiesListClicked)
    //EVT_LISTBOX(XRCID("mDriverSelector"), TrackView::OnDriverListClicked)
END_EVENT_TABLE()

TrackView::TrackView(wxWindow* parent,std::string propertyName){
    wxXmlResource::Get()->LoadFrame(this, parent, "TrackView");
    TrackViewWidget* trackViewWidget = new TrackViewWidget(this,wxSize(500,300));
    wxXmlResource::Get()->AttachUnknownControl(wxT("mTrack"),
		trackViewWidget
	);
    

    //EventManager::getInstance()->subscribe("updateDiagramm",this);
    EventManager::getInstance()->subscribe("DiagramChanged",this);
    EventManager::getInstance()->subscribe("ChangeProperty",this);

    mPropertyName = propertyName;
    //trackViewWidget->paintNow();
}
TrackView::~TrackView(){
    //EventManager::getInstance()->unsubscribe("updateDiagramm",this);
    EventManager::getInstance()->unsubscribe("DiagramChanged",this);
    EventManager::getInstance()->unsubscribe("ChangeProperty",this);
}
void TrackView::event(std::string eventName,EventParam* param){
    
    if(eventName.compare("ChangeProperty") == 0){
        std::string propertyName = param->getString("propertyName");
        mPropertyName = propertyName;
        addDataToTrackView();
    }
    else if(eventName.compare("DiagramChanged") == 0){
        float vecX = param->getFloat("xPos");
        float vecY = param->getFloat("yPos");
        float vecZ = param->getFloat("zPos");
        dogEngine::CVector3 vec(vecX,vecY,vecZ);
        setPointInTrackView(vec);
    }
}
void TrackView::updateListBoxes(){

}
void TrackView::setPointInTrackView(dogEngine::CVector3 pos){
    TrackViewWidget* trackViewWidget = XRCCTRL(*this, "mTrack", TrackViewWidget);
    trackViewWidget->setPoint(dogEngine::CVector2(pos.getX(),pos.getZ()));
}
void TrackView::addDataToTrackView(){
    
    if(mPropertyName.length() == 0){
        return;
    }
    TrackViewWidget* trackViewWidget = XRCCTRL(*this, "mTrack", TrackViewWidget);
    trackViewWidget->clearDataSet();
    
    for(int i=0;i < FileManager::getInstance()->getNumberOfActiveLaps();i++){
        try{
            AnalyseData& metaData = FileManager::getInstance()->getActiveLap(i);
            
            TrackDataSet data = FileManager::getInstance()
                ->getOpenDbFileByName(metaData.getFilename()).getTrackValues(metaData,mPropertyName);
            
            trackViewWidget->addTrackDataset(data);
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
        catch(FileOpenErrorException e){
            std::string message = e.what();
            wxString wxMessage = wxString(message);
            wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"File Open Error",wxOK|wxCENTRE|wxICON_ERROR);
            msg->ShowModal();
            break;
        }
        catch(std::exception e){
            std::string message = e.what();
            wxString wxMessage = wxString(message);
            wxMessageDialog* msg = new wxMessageDialog(this,wxMessage,"Error",wxOK|wxCENTRE|wxICON_ERROR);
            msg->ShowModal();
            break;
        }
    }
}
void TrackView::OnProperiesListClicked(wxCommandEvent& event){
    
}
void TrackView::OnDriverListClicked(wxCommandEvent& event){
    
}