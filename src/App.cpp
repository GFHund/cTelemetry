#include "App.h"

#include "DiagramView.h"
#include "DataView.h"
#include "wx/xrc/xmlres.h"
#if wxUSE_AUI
    #include "wx/xrc/xh_aui.h"
    #include "wx/xrc/xh_auitoolb.h"
#endif // wxUSE_AUI

bool App::OnInit(){
	// call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;
		
	#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
	#endif
	#if wxUSE_GIF
		wxImage::AddHandler(new wxGIFHandler);
	#endif
	wxImage::AddHandler(new wxPNGHandler );
	
	// Initialize all the XRC handlers. Always required (unless you feel like
    // going through and initializing a handler of each control type you will
    // be using (ie initialize the spinctrl handler, initialize the textctrl
    // handler). However, if you are only using a few control types, it will
    // save some space to only initialize the ones you will be using. See
    // wxXRC docs for details.
    wxXmlResource::Get()->InitAllHandlers();
	
	// Allow using environment variables in the file paths in the resources,
    // while keeping the default wxXRC_USE_LOCALE flag.
    wxXmlResource::Get()->SetFlags(wxXRC_USE_LOCALE | wxXRC_USE_ENVVARS);
	
	#if wxUSE_AUI
		wxXmlResource::Get()->AddHandler(new wxAuiXmlHandler);
		wxXmlResource::Get()->AddHandler(new wxAuiToolBarXmlHandler);
	#endif
	// Load all of the XRC files that will be used. You can put everything
    // into one giant XRC file if you wanted, but then they become more
    // diffcult to manage, and harder to reuse in later projects.
    if ( !wxXmlResource::Get()->LoadAllFiles("rc") )
        return false;
		
	DiagramView * dView = new DiagramView();
	dView->Show();
	
	DataView* dataView = new DataView();
	dataView->Show();
		
	return true;
}

wxIMPLEMENT_APP(App);