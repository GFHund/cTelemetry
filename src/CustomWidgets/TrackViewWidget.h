#ifndef __TRACK_VIEW_WIDGET__
#define __TRACK_VIEW_WIDGET__

#include <wx/wx.h>
#include <wx/sizer.h>

#include "../data/TrackDataSet.h"
#include <vector>

class TrackViewWidget: public wxWindow{
    private:
    /*
    struct colorLegend{
		int from;
		int to;
		int color;
	};
    */

	std::vector<TrackDataSet> mDataSets;
	//std::vector<colorLegend> mColors;
    std::vector< std::pair<int,int> > mColors;
    float mOverallMinX;
	float mOverallMaxX;
	float mOverallMinY;
	float mOverallMaxY;
    int mTrackMinWidth;
    int mTrackMinHeight;

    void calculateOverallMinMax();
    DECLARE_EVENT_TABLE();
    public:
    TrackViewWidget(wxFrame* parent,const wxSize& size);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();
	void render(wxDC& dc);

    void addTrackDataset(TrackDataSet dataset);
    void clearDataSet();

    
};

#endif