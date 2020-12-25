#include "DiagramWidget.h"

BEGIN_EVENT_TABLE(DiagramWidget, wxPanel)

    EVT_MOTION(DiagramWidget::mouseMoved)
    EVT_LEFT_DOWN(DiagramWidget::mouseDown)
    EVT_LEFT_UP(DiagramWidget::mouseReleased)
    EVT_RIGHT_DOWN(DiagramWidget::rightClick)
    EVT_LEAVE_WINDOW(DiagramWidget::mouseLeftWindow)
    EVT_KEY_DOWN(DiagramWidget::keyPressed)
    EVT_KEY_UP(DiagramWidget::keyReleased)
    EVT_MOUSEWHEEL(DiagramWidget::mouseWheelMoved)

    // catch paint events
    EVT_PAINT(DiagramWidget::paintEvent)

END_EVENT_TABLE()

DiagramWidget::DiagramWidget(wxFrame* parent)
	: wxWindow(parent,wxID_ANY)
{
		SetMinSize(wxSize(diagramMinWidth,diagramMinHeight));
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void DiagramWidget::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void DiagramWidget::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void DiagramWidget::render(wxDC&  dc)
{
	bool pressedDown = false;
    if (pressedDown)
        dc.SetBrush( *wxRED_BRUSH );
    else
        dc.SetBrush( *wxGREY_BRUSH );
    
    dc.DrawRectangle( 0, 0, diagramMinWidth, diagramMinHeight );
    dc.DrawText( "hallo Welt", 20, 15 );
}

void DiagramWidget::mouseDown(wxMouseEvent& event)
{
    //pressedDown = true;
    paintNow();
}
void DiagramWidget::mouseReleased(wxMouseEvent& event)
{
    //pressedDown = false;
    paintNow();
    
    wxMessageBox( wxT("You pressed a custom button") );
}
void DiagramWidget::mouseLeftWindow(wxMouseEvent& event)
{
	/*
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
	*/
}

// currently unused events
void DiagramWidget::mouseMoved(wxMouseEvent& event) {}
void DiagramWidget::mouseWheelMoved(wxMouseEvent& event) {}
void DiagramWidget::rightClick(wxMouseEvent& event) {}
void DiagramWidget::keyPressed(wxKeyEvent& event) {}
void DiagramWidget::keyReleased(wxKeyEvent& event) {}

