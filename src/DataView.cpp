#include "DataView.h"

#include "wx/xrc/xmlres.h"

BEGIN_EVENT_TABLE(DataView, wxFrame)
END_EVENT_TABLE()

DataView::DataView(wxWindow* parent){
	wxXmlResource::Get()->LoadFrame(this, parent, "DataView");
}
DataView::~DataView(){}