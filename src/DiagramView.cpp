#include "DiagramView.h"

#include "CustomWidgets/DiagramWidget.h"
#include "wx/xrc/xmlres.h"

BEGIN_EVENT_TABLE(DiagramView, wxFrame)
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
	
	DiagramWidget* diagramWidget = new DiagramWidget(this);
	wxXmlResource::Get()->AttachUnknownControl(wxT("mDiagram"),
		diagramWidget
	);
	
	// Give the frame an optional statusbar. The '1' just means one field.
    // A gripsizer will automatically get put on into the corner, if that
    // is the normal OS behaviour for frames on that platform. Helptext
    // for menu items and toolbar tools will automatically get displayed
    // here.
    //CreateStatusBar( 1 );
}

DiagramView::~DiagramView(){}