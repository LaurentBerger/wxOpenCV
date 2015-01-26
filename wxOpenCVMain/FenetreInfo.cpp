#include "FenetreInfo.h"
#include "wx/graphics.h"



// frame constructor
ShapedFrame::ShapedFrame(wxFrame *parent,wxString s1,wxString s2)
       : wxFrame(parent, wxID_ANY, wxEmptyString,
                  wxDefaultPosition, wxSize(100, 100),
                  0
                  | wxFRAME_SHAPED
                  | wxSIMPLE_BORDER
                  | wxFRAME_NO_TASKBAR
                  | wxSTAY_ON_TOP
            )
{
    m_shapeKind = Shape_Star;
    //m_bmp = wxBitmap(wxT("star.png"), wxBITMAP_TYPE_PNG);

	

	wxMemoryDC memDC;


	wxSize	taille1=memDC.GetTextExtent(s1);
	wxSize	taille2=memDC.GetTextExtent(s2);
	taille1.SetWidth(std::max(taille1.GetWidth(),taille2.GetWidth()));
	taille1.SetHeight(taille1.GetHeight()+taille2.GetHeight());

    m_bmp = wxBitmap(taille1.GetWidth()+200, taille1.GetHeight()+200);
    memDC.SelectObject(m_bmp);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
    memDC.SetPen(*wxRED_PEN);
    memDC.SetBrush(*wxRED_BRUSH);
    memDC.DrawRectangle(wxRect(00, 00, taille1.GetWidth(), taille1.GetHeight()));
    memDC.DrawRectangle(wxRect(150, 150, 30, 30));
	memDC.DrawText(s1,0,0);
	memDC.DrawText(s2,0,taille2.GetHeight());
    memDC.SelectObject(wxNullBitmap);



    SetSize(wxSize(m_bmp.GetWidth(), m_bmp.GetHeight()));
    SetToolTip(_("Right-click to close, double click to cycle shape"));
    SetWindowShape();
	Bind(wxEVT_MOTION, &ShapedFrame::OnMouseMove,this);
	Bind(wxEVT_PAINT,&ShapedFrame::OnPaint,this);
//	Bind(wxEVT_ACTIVATE, &ShapedFrame::OnActivate,this);
	Bind(wxEVT_LEFT_UP, &ShapedFrame::OnLeftUp,this);
	Bind(wxEVT_LEFT_DOWN, &ShapedFrame::OnLeftDown,this);
	Bind(wxEVT_RIGHT_UP, &ShapedFrame::OnExit,this);
	Bind(wxEVT_LEFT_DCLICK, &ShapedFrame::OnDoubleClick,this);
}


void ShapedFrame::SetWindowShape()
{
    switch ( m_shapeKind )
    {
        case Shape_None:
            SetShape(wxRegion());
            break;

        case Shape_Star:
            SetShape(wxRegion(m_bmp, *wxWHITE));
            break;

#if wxUSE_GRAPHICS_CONTEXT
        case Shape_Circle:
            {
                wxGraphicsPath
                    path = wxGraphicsRenderer::GetDefaultRenderer()->CreatePath();
                path.AddCircle(m_bmp.GetWidth()/2, m_bmp.GetHeight()/2, 30);
                SetShape(path);
            }
            break;
#endif // wxUSE_GRAPHICS_CONTEXT

        case Shape_Max:
            wxFAIL_MSG( "invalid shape kind" );
            break;
    }
}

void ShapedFrame::OnDoubleClick(wxMouseEvent& WXUNUSED(evt))
{
    m_shapeKind = static_cast<ShapeKind>((m_shapeKind + 1) % Shape_Max);
    SetWindowShape();
}

void ShapedFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx =  pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void ShapedFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void ShapedFrame::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    if (evt.Dragging() && evt.LeftIsDown())
    {
        wxPoint pos = ClientToScreen(pt);
        Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void ShapedFrame::OnExit(wxMouseEvent& WXUNUSED(evt))
{
    Close();
}

void ShapedFrame::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(m_bmp, 0, 0, true);
}
