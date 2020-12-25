#include <wx/wx.h>
#include <wx/sizer.h>

class DiagramWidget: public wxWindow{
	private:
	
	static const int diagramMinWidth = 200;
    static const int diagramMinHeight = 50;
	
	public:
	DiagramWidget(wxFrame* parent);
	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void render(wxDC& dc);
	
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	
	DECLARE_EVENT_TABLE();
};

