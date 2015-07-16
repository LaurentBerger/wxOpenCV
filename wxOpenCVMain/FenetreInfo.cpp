#include "FenetreInfo.h"
#include "wx/graphics.h"
#include "FenetrePrincipale.h"


// frame constructor
ShapedFrame::ShapedFrame(wxFrame *parent,wxPoint &pSrc)
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
    //m_bmp = wxBitmap(_("star.png"), wxBITMAP_TYPE_PNG);

	cv::Mat im=((FenetrePrincipale*)parent)->ImAcq()->getMat(cv::ACCESS_READ);
	wxColor c=((FenetrePrincipale*)parent)->LitCouleurPixel(pSrc);
	wxMemoryDC memDC;

	wxString s1("Region");
	wxString s2("Region");
	wxSize	taille1=memDC.GetTextExtent(s1);
	wxSize	taille2=memDC.GetTextExtent(s2);
	taille1.SetWidth(std::max(taille1.GetWidth(),taille2.GetWidth()));
	taille1.SetHeight(taille1.GetHeight()+taille2.GetHeight());

    m_bmp = wxBitmap(im.cols, im.rows);
    memDC.SelectObject(m_bmp);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
    memDC.SetPen(c);
    memDC.SetBrush(wxBrush(c));


	cv::Vec3b x;
	cv::Vec3f xx;
	cv::Vec6f xxx;
	std::complex<float> zz[3];
	int val;
	double dVal;
	switch(im.type()){
	case CV_32FC1:
		{
		float ref=im.at<float>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				float x=im.at<float>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_64FC1:
		{
		double ref=im.at<double>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				float x=im.at<double>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_32FC3 :
		{
		cv::Vec3f ref=im.at<cv::Vec3f>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				cv::Vec3f x=im.at<cv::Vec3f>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_32FC(6) :
		break;
	case CV_64FC3 :
		{
		cv::Vec3d ref=im.at<cv::Vec3d>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				cv::Vec3d x=im.at<cv::Vec3d>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_32SC1:
		{
		int ref=im.at<int>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				int x=im.at<int>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_32SC3:
		{
		cv::Vec3i ref=im.at<cv::Vec3i>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				cv::Vec3i x=im.at<cv::Vec3i>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_8UC1:
		{
		unsigned char ref=im.at<unsigned char>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				unsigned char x=im.at<unsigned char>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_8UC3 :
		{
		cv::Vec3b ref=im.at<cv::Vec3b>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				cv::Vec3b x=im.at<cv::Vec3b>(i,j);
				//if (x==ref)
					{
                    memDC.SetBrush(wxBrush(wxColour(x[2], x[1], x[0])));
                    memDC.SetPen((wxColour(x[2], x[1], x[0])));

					memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_16UC1 :
		{
		unsigned short ref=im.at<unsigned short>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				unsigned short x=im.at<unsigned short>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_16SC1 :
		{
		short ref=im.at<short>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				short x=im.at<short>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
	case CV_16SC3 :
		{
		cv::Vec3s ref=im.at<cv::Vec3s>(pSrc.y,pSrc.x);
		for (int i=0;i<im.rows;i++)
			for (int j=0;j<im.cols;j++)
				{
				cv::Vec3s x=im.at<cv::Vec3s>(i,j);
				if (x==ref)
					{
					 memDC.DrawRectangle(wxRect(j, i, 2, 2));

					}


				}
		}
		break;
		}


		
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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// frame constructor
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
PointFrame::PointFrame(wxFrame *parent,wxPoint &pSrc)
       : wxFrame(parent, wxID_ANY, wxEmptyString,
                  pSrc, wxSize(100, 100),
                  0
                  | wxFRAME_SHAPED
                  | wxSIMPLE_BORDER
                  | wxFRAME_NO_TASKBAR
                  | wxSTAY_ON_TOP
            )
{
	wxMemoryDC memDC;

	wxColor c(*wxRED);

    m_bmp = wxBitmap(16, 16);
    memDC.SelectObject(m_bmp);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
    memDC.SetPen(c);
    memDC.SetBrush(wxBrush(c));

	memDC.DrawCircle(4,4,4);

    memDC.SelectObject(wxNullBitmap);

    SetSize(wxSize(m_bmp.GetWidth(), m_bmp.GetHeight()));
    SetToolTip(_("Right-click to close, double click to cycle shape"));
    SetWindowShape();
	Bind(wxEVT_MOTION, &PointFrame::OnMouseMove,this);
	Bind(wxEVT_PAINT,&PointFrame::OnPaint,this);
	Bind(wxEVT_LEFT_UP, &PointFrame::OnLeftUp,this);
	Bind(wxEVT_LEFT_DOWN, &PointFrame::OnLeftDown,this);
	Bind(wxEVT_RIGHT_UP, &PointFrame::OnExit,this);
	Bind(wxEVT_LEFT_DCLICK, &PointFrame::OnDoubleClick,this);
}


void PointFrame::SetWindowShape()
{
    SetShape(wxRegion(m_bmp, *wxWHITE));
}

void PointFrame::OnDoubleClick(wxMouseEvent& WXUNUSED(evt))
{
    SetWindowShape();
}

void PointFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    wxPoint pos = (evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx =  pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void PointFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void PointFrame::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    if (evt.Dragging() && evt.LeftIsDown())
    {
        wxPoint pos = (pt);
        Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void PointFrame::OnExit(wxMouseEvent& WXUNUSED(evt))
{
    Close();
}

void PointFrame::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(m_bmp, 0, 0, true);
}
