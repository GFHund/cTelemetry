#include <wx/wx.h>
#include <wx/sizer.h>

#include "../data/DiagramDataSet.h"
#include "../CustomEvents/ChangeDiagramEvent.h"
class DiagramWidget: public wxWindow{
	private:
	const int PADDING_X = 20;
	const int PADDING_Y = 20;

	int mDiagramMinWidth;
    int mDiagramMinHeight;
	bool mShowContextWindow;
	int mMouseX;
	int mMouseY;
	float mValueX;

	float mOverallMinX;
	float mOverallMaxX;
	float mOverallMinY;
	float mOverallMaxY;
	float mXValuePerPixel;
	float mYValuePerPixel;
	int mOriginalWidth;
	int mOriginalHeight;

	std::vector<std::pair<DiagramDataSet,int>> mDataSets;

	void calculateOverallMinMax();
	
	public:
	DiagramWidget(wxFrame* parent,const wxSize& size);
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

	/*my Diagramm Functions*/
	void addXyDataset(DiagramDataSet dataset, int color);
	void removeXyDataset(int color);

	void clearXyDataset();
	
	DECLARE_EVENT_TABLE();
};

wxDEFINE_EVENT(DIAGRAM_CHANGE,ChangeDiagramEvent);

#define ChangeDiagramEventHandler(func) (&func)

#define CHANGE_DIAGRAM_EVENT(id, func) wx__DECLARE_EVT1(DIAGRAM_CHANGE, id, ChangeDiagramEventHandler(func))