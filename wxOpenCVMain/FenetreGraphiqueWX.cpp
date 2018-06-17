#include "FenetreGraphiqueWX.h"
#include "Fenetre3D.h"
//#include "wxOSGApp.h" 

#ifdef __WWWWWWW__ 


BEGIN_EVENT_TABLE(FenetreGraphiqueWX, wxGLCanvas)
    EVT_KEY_DOWN        (FenetreGraphiqueWX::OnKeyDown)
    EVT_KEY_UP            (FenetreGraphiqueWX::OnKeyUp)
    EVT_MOUSE_EVENTS    (FenetreGraphiqueWX::OnMouse)
 	EVT_TIMER(1, FenetreGraphiqueWX::OnTimer)
 	EVT_TIMER(2, FenetreGraphiqueWX::OnTimerCopy)
 	
   EVT_JOYSTICK_EVENTS(FenetreGraphiqueWX::OnJoystickEvent)
END_EVENT_TABLE()
#endif

FenetreGraphiqueWX::FenetreGraphiqueWX(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes)
    : wxGLCanvas(parent, id,attributes,  pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{

    // Make the new context current (activate it for use) with this canvas.
glContexte = new wxGLContext(this);
    // default cursor to standard
    _oldCursor = *wxSTANDARD_CURSOR;
	basculeCopyAvance=0;
	horlogeSeq=NULL;
	horlogeCopy=NULL;
	fenParent=parent;
Bind(wxEVT_MOTION, &FenetreGraphiqueWX::OnMouse,this);
Bind(wxEVT_LEFT_DOWN, &FenetreGraphiqueWX::OnMouse,this);
Bind(wxEVT_LEFT_UP, &FenetreGraphiqueWX::OnMouse,this);
Bind(wxEVT_RIGHT_DOWN, &FenetreGraphiqueWX::OnMouse,this);
Bind(wxEVT_RIGHT_UP, &FenetreGraphiqueWX::OnMouse,this);

Bind(wxEVT_ERASE_BACKGROUND, &FenetreGraphiqueWX::OnEraseBackground,this);
Bind(wxEVT_PAINT,&FenetreGraphiqueWX::OnPaint,this);
Bind(wxEVT_SIZE, &FenetreGraphiqueWX::OnSize,this);
Bind(wxEVT_KEY_DOWN ,&FenetreGraphiqueWX::OnKeyDown,this,wxID_ANY);
Bind(wxEVT_CHAR ,&FenetreGraphiqueWX::OnChar,this,wxID_ANY);
Bind(wxEVT_KEY_UP,&FenetreGraphiqueWX::OnKeyUp,this,wxID_ANY);
Bind(wxEVT_TIMER, &FenetreGraphiqueWX::OnTimer,this,1);
Bind(wxEVT_TIMER, &FenetreGraphiqueWX::OnTimerCopy,this,2);
Bind(wxEVT_ENTER_WINDOW,&FenetreGraphiqueWX::OnMouseEnter,this);
Bind(wxEVT_MOUSEWHEEL,&FenetreGraphiqueWX::OnMouseWheel,this);


}

void FenetreGraphiqueWX::OnMouseWheel(wxMouseEvent &event)
{
    int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();

    if (_graphics_window.valid()) {
        _graphics_window->getEventQueue()->mouseScroll(
            delta>0 ? 
            osgGA::GUIEventAdapter::SCROLL_UP : 
            osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}


void FenetreGraphiqueWX::UseCursor(bool value)
{
    if (value)
    {
        // show the old cursor
        SetCursor(_oldCursor);
    }
    else
    {
        // remember the old cursor
        _oldCursor = GetCursor();

        // hide the cursor
        //    - can't find a way to do this neatly, so create a 1x1, transparent image
        wxImage image(1,1);
        image.SetMask(true);
        image.SetMaskColour(0, 0, 0);
        wxCursor cursor(image);
        SetCursor(cursor);

        // On wxGTK, only works as of version 2.7.0
        // (http://trac.wxwidgets.org/ticket/2946)
        // SetCursor( wxStockCursor( wxCURSOR_BLANK ) );
    }
}

void FenetreGraphiqueWX::init()
{
    wxJoystick *m_stick = new wxJoystick(wxJOYSTICK1);
    int nButtons;
    if (!m_stick->IsOk())
    {
		//wxMessageBox(_T("No joystick detected!"));
    }
	else
		nButtons = m_stick->GetNumberButtons();

#if wxUSE_SOUND

    m_fire.Create(_T("buttonpress.wav"));
#endif // wxUSE_SOUND
    m_stick->SetCapture(this, 10);
    if (_graphics_window.valid())
    {
/*        setState( new osg::State );
        getState()->setGraphicsContext(this);


        if (_traits.valid() && _traits->sharedContext)
        {
            getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( getState()->getContextID() );
        }
        else
        {
            getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }*/
    }

}

FenetreGraphiqueWX::~FenetreGraphiqueWX()
{
}

void FenetreGraphiqueWX::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    /* must always be here */
    wxPaintDC dc(this);
}

void FenetreGraphiqueWX::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
//    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int width, height;
    GetClientSize(&width, &height);

    // update the window dimensions, in case the window has been resized.
    if (_graphics_window.valid())
    {
        // update the window dimensions, in case the window has been resized.
        _graphics_window->getEventQueue()->windowResize(0, 0, width, height);
        _graphics_window->resized(0,0,width,height);
    }
}

void FenetreGraphiqueWX::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    /* Do nothing, to avoid flashing on MSW */
}

void FenetreGraphiqueWX::OnKeyDown(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
	if (key==WXK_NONE )
		key = event.GetKeyCode();
#else
    int key = event.GetKeyCode();
#endif
int d=0;
if (key==WXK_NUMPAD_ADD)
	{
	((Fenetre3D*)fenParent)->Surface()->DefEchZ(((Fenetre3D*)fenParent)->Surface()->EchZ()*1.1);
	((Fenetre3D*)fenParent)->Surface()->Maj(0);
	((Fenetre3D*)fenParent)->Surface()->MajSurface();
	}
if (key==    WXK_NUMPAD_SUBTRACT)
	{
	((Fenetre3D*)fenParent)->Surface()->DefEchZ(((Fenetre3D*)fenParent)->Surface()->EchZ()/1.1);
	((Fenetre3D*)fenParent)->Surface()->Maj(0);
	((Fenetre3D*)fenParent)->Surface()->MajSurface();
	}
if (key==WXK_F1)
	((Fenetre3D*)fenParent)->ActiveGeode(0);
if (key==WXK_F2)
	((Fenetre3D*)fenParent)->ActiveGeode(1);
if (key==WXK_F3)
	((Fenetre3D*)fenParent)->ActiveGeode(2);
if (key==WXK_F4)
	((Fenetre3D*)fenParent)->ActiveGeode(3);
if (key==WXK_F5)
	((Fenetre3D*)fenParent)->ActiveGeode(4);
if (key==WXK_F6)
	((Fenetre3D*)fenParent)->ActiveGeode(5);
if (key==WXK_F7)
	((Fenetre3D*)fenParent)->ActiveGeode(6);
if (key==WXK_F8)
	((Fenetre3D*)fenParent)->ActiveGeode(7);
if (key==WXK_F9)
	((Fenetre3D*)fenParent)->ActiveGeode(8);
if (key==WXK_F10)
	((Fenetre3D*)fenParent)->ActiveGeode(9);
if (key==WXK_F11)
	((Fenetre3D*)fenParent)->ActiveGeode(10);
if (key==WXK_F12)
	((Fenetre3D*)fenParent)->ActiveGeode(11);
if (key==WXK_LEFT)
	((Fenetre3D*)fenParent)->DeplaceCurseur(-1,0);
if (key==WXK_RIGHT)
	((Fenetre3D*)fenParent)->DeplaceCurseur(1,0);
if (key==WXK_UP)
	((Fenetre3D*)fenParent)->DeplaceCurseur(0,1);
if (key==WXK_DOWN)
	((Fenetre3D*)fenParent)->DeplaceCurseur(0,-1);


if (d!=0)
	{
	MAJ(d);
	}
    if (_graphics_window.valid())
    {
        // update the window dimensions, in case the window has been resized.
        _graphics_window->getEventQueue()->keyPress(key);
    }

    // propagate event
event.Skip();
}

void FenetreGraphiqueWX::OnChar(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif
int d=0;
if (key>='1' && key<='4')
	{
	((Fenetre3D*)fenParent)->MAJNoeud(key);
	}
if (key=='7')
	d=+1;
else if (key=='6')	
	d=-1;
if (key=='V'&& event.ShiftDown() )
	{
	if (horlogeSeq==NULL)
		{
		horlogeSeq= new wxTimer(this,1);
		}
	if (horlogeCopy==NULL)
		{
		horlogeCopy= new wxTimer(this,2);
		}
	horlogeSeq->Start(3000,false);
	horlogeCopy->Start(300,false);
//	((wxOpencvApp*)osgApp)->AutoriseVideo();
	}
if (key=='V'&& !event.ShiftDown() )
	{
//	((wxOpencvApp*)osgApp)->InterditVideo();
	if (horlogeSeq && horlogeSeq->IsRunning())
		{
		horlogeSeq->Stop();
		horlogeCopy->Stop();
		}
	}
	
if (key=='T')
	{
	surfaceReference->Transparence();
	surfaceReference->Maj();
	}
if (key=='W' && !event.ShiftDown() )
	{
	surfaceReference->Descendre(1);
	surfaceReference->Maj();
	}
if (key=='W' && event.ShiftDown() )
	{
	surfaceReference->Descendre(100);
	surfaceReference->Maj();
	}
if (key=='X' && !event.ShiftDown())
	{
	surfaceReference->Monter(1);
	surfaceReference->Maj();
	}
if (key=='X' && event.ShiftDown())
	{
	surfaceReference->Monter(100);
	surfaceReference->Maj();
	} // if (key=='X' && event.ShiftDown())

if (key=='R')
	{
	((Fenetre3D*)fenParent)->Composante('R');
	}
if (key=='G')
	{
	((Fenetre3D*)fenParent)->Composante('G');
	}
if (key=='B')
	{
	((Fenetre3D*)fenParent)->Composante('B');
	}

if (d!=0)
	{
	MAJ(d);
	}
    if (_graphics_window.valid())
    {
        // update the window dimensions, in case the window has been resized.
        _graphics_window->getEventQueue()->keyPress(key);
    }

    // propagate event
event.Skip();
}


void FenetreGraphiqueWX::MAJ(int d)
{
	surface->Maj(d);
	surface->LitShapeDrawable()->setUseDisplayList(false);
	surface->LitShapeDrawable()->dirtyBound(); 
	surface->LitShapeDrawable()->setUseDisplayList(true);
}

void FenetreGraphiqueWX::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif
        if (_graphics_window.valid())
    {
        // update the window dimensions, in case the window has been resized.
        _graphics_window->getEventQueue()->keyRelease(key);
    }


    // propagate event
    event.Skip();
}


void FenetreGraphiqueWX::useCursor(bool cursorOn)
{
    if (cursorOn) {

        // show the old cursor
        SetCursor(_oldCursor);
    }
    else {

        // remember the old cursor
        _oldCursor = GetCursor();

        // hide the cursor
        //    - can't find a way to do this neatly, so create a 1x1, transparent image
        wxImage image(1,1);
        image.SetMask(true);
        image.SetMaskColour(0, 0, 0);
        wxCursor cursor(image);
        SetCursor(cursor);
    }
}


void FenetreGraphiqueWX::OnMouse(wxMouseEvent& event)
{
    if (event.ButtonDown()) 
		{
        int button = event.GetButton();
        if (_graphics_window.valid())
			{
        // update the window dimensions, in case the window has been resized.
			_graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(), button);
			}

		}
    else if (event.ButtonUp()) 
		{
        int button = event.GetButton();
        if (_graphics_window.valid())
			{
        // update the window dimensions, in case the window has been resized.
			_graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(), button);
			}
        
		}
    else if (event.Dragging()) 
        {
        if (_graphics_window.valid())
			_graphics_window->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
        
		}
}

void FenetreGraphiqueWX::OnTimer(wxTimerEvent& event)
{
MAJ(1);

}


void FenetreGraphiqueWX::OnTimerCopy(wxTimerEvent& event)
{
static int numImage=0;
if (numImage==0)
	 wxInitAllImageHandlers();

wxClientDC dc(this);
wxSize dc_size = dc.GetSize();
int width = dc_size.GetWidth();
int height = dc_size.GetHeight();
wxBitmap test_bitmap(width , height );
wxMemoryDC memDC;
memDC.SelectObject(test_bitmap);

// Copy from Screen DC to mem DC.
memDC.Blit(0, 0, width, height, &dc, 0, 0);
memDC.SelectObject( wxNullBitmap );
wxImage im=test_bitmap.ConvertToImage();
wxString s;
s.Printf(_T("im3d%d.jpg"),numImage);
numImage++;
im.SaveFile(s.c_str());
return ; 
}


void FenetreGraphiqueWX::OnJoystickEvent(wxJoystickEvent& event)
{
    wxClientDC dc(this);
	wxJoystick * joy=(wxJoystick *)event.GetEventObject();
    wxPoint pt(joy->GetPosition());
    int		w,h;
    static int bouton=joy->GetButtonState();
	dc.GetSize(&w,&h);
    int button = joy->GetButtonState();
	pt.x=w*(pt.x - joy->GetXMin())/double(joy->GetXMax()-joy->GetXMin());
	pt.y=h*(pt.y - joy->GetYMin())/double(joy->GetYMax()-joy->GetYMin());
	switch(button){
	case 2:
		button=3;
		break;
	case 4:
		button=3;
		break;
	case 8:
		return;
		break;
		}

	if (event.ButtonDown())
		{
//		m_fire.Play();
//		button = event.GetButtonChange();
        if (_graphics_window.valid())
			_graphics_window->getEventQueue()->mouseButtonPress(pt.x, pt.y, button);
		}
    else if (event.ButtonUp()) 
		{
//		button = event.GetButtonChange();
        if (_graphics_window.valid())
			_graphics_window->getEventQueue()->mouseButtonRelease(pt.x, pt.y, button);
//		m_fire.Play();
		}
    else if (joy->GetButtonState() && event.IsMove()) 
        if (_graphics_window.valid())
			_graphics_window->getEventQueue()->mouseMotion(pt.x, pt.y);

/*
	if (event.ButtonIsDown() && !bouton)
		{
		m_fire.Play();
        getEventQueue()->mouseButtonPress(pt.x, pt.y, button);
		bouton=button;
		}
    else if (event.ButtonUp()) 
		{
        getEventQueue()->mouseButtonRelease(pt.x, pt.y, button);
		bouton=button;
		m_fire.Play();
		}
    else if (bouton) 
        getEventQueue()->mouseMotion(pt.x, pt.y);
*/

#if wxUSE_SOUND
    if (joy->GetButtonState())
    {
//        m_fire.Play();
    }
#endif // wxUSE_SOUND
}

void FenetreGraphiqueWX::OnMouseEnter(wxMouseEvent &event)
{
    // Set focus to ourselves, so keyboard events get directed to us
    SetFocus();
}

GraphicsOSGWX::GraphicsOSGWX(FenetreGraphiqueWX *canvas)
{
    _canvas = canvas;

    _traits = new GraphicsContext::Traits;

    wxPoint pos = _canvas->GetPosition();
    wxSize  size = _canvas->GetSize();

    _traits->x = pos.x;
    _traits->y = pos.y;
    _traits->width = size.x;
    _traits->height = size.y;

    init();
}

GraphicsOSGWX::~GraphicsOSGWX()
{
}

void GraphicsOSGWX::init()
{
    if (valid())
    {
        setState( new osg::State );
        getState()->setGraphicsContext(this);

 /*       if (_traits.valid() /*&& _traits->sharedContext.valid())
        {
            getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( getState()->getContextID() );
        }
        else*/
        {
            getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }
    }
}

void GraphicsOSGWX::grabFocus()
{
    // focus the canvas
    _canvas->SetFocus();
}

void GraphicsOSGWX::grabFocusIfPointerInWindow()
{
    // focus this window, if the pointer is in the window
    wxPoint pos = wxGetMousePosition();
    if (wxFindWindowAtPoint(pos) == _canvas)
        _canvas->SetFocus();
}

void GraphicsOSGWX::useCursor(bool cursorOn)
{
    _canvas->UseCursor(cursorOn);
}

bool GraphicsOSGWX::makeCurrentImplementation()
{
_canvas->SetCurrent(*_canvas->glContexte);
    return true;
}

void GraphicsOSGWX::swapBuffersImplementation()
{
    _canvas->SwapBuffers();
}


