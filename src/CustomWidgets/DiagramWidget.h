#include <wx/wx.h>
#include <wx/sizer.h>

#include "../data/DiagramDataSet.h"
class DiagramWidget: public wxWindow{
	private:
	const int PADDING_X = 20;
	const int PADDING_Y = 20;

	int mDiagramMinWidth;
    int mDiagramMinHeight;

	std::vector<std::pair<DiagramDataSet,int>> mDataSets;
	
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
	
	DECLARE_EVENT_TABLE();
};

