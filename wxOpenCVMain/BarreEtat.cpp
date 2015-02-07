#include "FenetrePrincipale.h"
enum
{
    StatusBar_Checkbox = 1000,
};

BEGIN_EVENT_TABLE(BarreInfo, wxStatusBar)
    EVT_SIZE(BarreInfo::OnSize)
    EVT_KEY_DOWN(BarreInfo::OnKeyDown)
    EVT_CHECKBOX(StatusBar_Checkbox, BarreInfo::BasculeCurseur)
    EVT_BUTTON(3789, BarreInfo::OnButton)
    EVT_BUTTON(3790, BarreInfo::OnButton)
    EVT_BUTTON(3791, BarreInfo::OnButton)
    EVT_BUTTON(3792, BarreInfo::OnButton) // Plan rouge
    EVT_BUTTON(3793, BarreInfo::OnButton) // Plan vert
    EVT_BUTTON(3794, BarreInfo::OnButton) // plan bleu
    EVT_TIMER(wxID_ANY, BarreInfo::OnTimer)
END_EVENT_TABLE()


static const int BITMAP_SIZE_X = 50;
static const int BITMAP_SIZE_Y = 15;


void BarreInfo::OnKeyDown(wxKeyEvent &event)
{
fen->TraitementClavier(event);
event.Skip();
}


BarreInfo::BarreInfo(wxWindow *parent)
           : wxStatusBar(parent, wxID_ANY), m_timer(this), m_checkbox(NULL)
{
static const int widths[9] = { -1,20,20,20,BITMAP_SIZE_X,BITMAP_SIZE_X, 80, 30,100 };

SetFieldsCount(9);
SetStatusWidths(9, widths);
fen=(FenetrePrincipale	*)parent; 
curseur=1;
#if wxUSE_CHECKBOX
m_checkbox = new wxCheckBox(this, StatusBar_Checkbox, _T("&Cursor"));
m_checkbox->SetValue(true);
#endif

                               
wxSize		size;
wxRect		rect;
GetFieldRect(Champ_Rouge, rect);
wxPoint p=rect.GetBottomLeft();
m_planRouge = new wxButton(this, 3792,_("R"),p, wxDefaultSize,wxBU_EXACTFIT);
size = m_planRouge->GetSize();

m_planRouge->Move(rect.x + (rect.width - size.x) / 2,
						rect.y + (rect.height - size.y) / 2);
m_planRouge->SetLabel(_("R"));

GetFieldRect(Champ_Vert, rect);
p=rect.GetBottomLeft();
m_planVert = new wxButton(this, 3793,_("G"),p, wxDefaultSize,wxBU_EXACTFIT);
size = m_planVert->GetSize();
m_planVert->SetLabel(_("G"));

m_planVert->Move(rect.x + (rect.width - size.x) / 2,
						rect.y + (rect.height - size.y) / 2);
m_planBleu = new wxButton(this, 3794,_("B"),p, wxDefaultSize,wxBU_EXACTFIT);
m_planBleu->SetLabel(_("B"));
size = m_planBleu->GetSize();
GetFieldRect(Champ_Bleu, rect);

m_planBleu->Move(rect.x + (rect.width - size.x) / 2,
						rect.y + (rect.height - size.y) / 2);
videoRouge = CreateBitmapForButton();
videoVert = CreateBitmapForButton(true);
m_videoBmp = new wxBitmapButton(this, 3789,videoVert ,
                               wxDefaultPosition, wxDefaultSize,
                               wxBU_EXACTFIT);
m_videoBmp->Show(false);

m_statBmp = new wxButton(this, 3790,"Stat",
                               wxDefaultPosition, wxDefaultSize,
                               wxBU_EXACTFIT);
m_statBmp->SetLabel("Stat");
m_zoomBmp = new wxButton(this, 3791,"Zoom",
                               wxDefaultPosition, wxDefaultSize,
                               wxBU_EXACTFIT);
m_zoomBmp->SetLabel("Zoom");
m_planBleu->SetBackgroundColour(*wxBLUE);
m_planVert->SetBackgroundColour(*wxGREEN);
m_planVert->Show(true);
m_planRouge->SetBackgroundColour(*wxRED);
m_planVert->Show(false);
m_planRouge->Show(false);
m_planBleu->Show(false);


#if wxUSE_TIMER
m_timer.Start(1000);
#endif

SetMinHeight(BITMAP_SIZE_Y);

UpdateClock();
}

#ifdef __VISUALC__
    #pragma warning(default: 4355)
#endif

BarreInfo::~BarreInfo()
{
#if wxUSE_TIMER
    if ( m_timer.IsRunning() )
    {
        m_timer.Stop();
    }
#endif
}

wxBitmap BarreInfo::CreateBitmapForButton(bool on)
{
    static const int BMP_BUTTON_SIZE_X = 10;
    static const int BMP_BUTTON_SIZE_Y = 9;

    wxBitmap bitmap(BMP_BUTTON_SIZE_X, BMP_BUTTON_SIZE_Y);
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(on ? *wxGREEN_BRUSH : *wxRED_BRUSH);
    dc.SetBackground(*wxLIGHT_GREY_BRUSH);
    dc.Clear();
    dc.DrawEllipse(0, 0, BMP_BUTTON_SIZE_X, BMP_BUTTON_SIZE_Y);
    dc.SelectObject(wxNullBitmap);

    return bitmap;
}

void BarreInfo::OnSize(wxSizeEvent& event)
{
#if wxUSE_CHECKBOX
    if ( !m_checkbox )
        return;
#endif

    wxRect rect;
    GetFieldRect(Champ_Checkbox, rect);

#if wxUSE_CHECKBOX
    m_checkbox->SetSize(rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4);
#endif

    GetFieldRect(Champ_BitmapVideo, rect);
    wxSize size = m_videoBmp->GetSize();

    m_videoBmp->Move(rect.x + (rect.width - size.x) / 2,
                    rect.y + (rect.height - size.y) / 2);
    GetFieldRect(Champ_BitmapStat, rect);
    size = m_statBmp->GetSize();

    m_statBmp->Move(rect.x + (rect.width - size.x) / 2,
                    rect.y + (rect.height - size.y) / 2);
if (fen->ImAcq())
		{
		GetFieldRect(Champ_Rouge, rect);
		size = m_planRouge->GetSize();

		m_planRouge->Move(rect.x + (rect.width - size.x) / 2,
						rect.y + (rect.height - size.y) / 2);
		m_planRouge->Show(true);
		int nbCanaux=fen->ImAcq()->channels();
		if (nbCanaux>=3)
			{
			GetFieldRect(Champ_Vert, rect);
			size = m_planVert->GetSize();

			m_planVert->Move(rect.x + (rect.width - size.x) / 2,
							rect.y + (rect.height - size.y) / 2);
			GetFieldRect(Champ_Bleu, rect);
			size = m_planBleu->GetSize();
			m_planBleu->Move(rect.x + (rect.width - size.x) / 2,
							rect.y + (rect.height - size.y) / 2);
			}
		if (fen && fen->ImAcq() && fen->ImAcq()->channels()>=3)
			{
			m_planRouge->Show(true);
			m_planVert->Show(true);
			m_planBleu->Show(true);
			}
		else
			{
			m_planRouge->Show(true);
			m_planVert->Show(false);
			m_planBleu->Show(false);
			}

		}
GetFieldRect(Champ_BitmapZoom, rect);
size = m_zoomBmp->GetSize();

m_zoomBmp->Move(rect.x + (rect.width - size.x) / 2,
                rect.y + (rect.height - size.y) / 2);


    event.Skip();
}

void BarreInfo::MAJButton()
{
if (fen->ModeCamera())
	{
	m_timer.Start(1000);
	UpdateClock();
	m_videoBmp->SetBitmapLabel(videoRouge);
    m_videoBmp->Refresh();	
	}
else
	{
	m_videoBmp->SetBitmapLabel(videoVert);
    m_videoBmp->Refresh();	
	m_timer.Stop();
//	m_statbmp->SetBitmapLabel(CreateBitmapForButton(true));
	SetStatusText(wxEmptyString, Champ_Clock);
    }

}

void BarreInfo::OnButton(wxCommandEvent &event)
{
wxCommandEvent evt;
if (event.GetId()==3789)
	{
	if (fen->ModeCamera())
		{
		if (wxMessageBox("Do you want really to stop grabbing?", "Warning", wxYES_NO)==wxYES)
			fen->ModeCamera(evt);
		}
	else
		{
		if (wxMessageBox("Do you want really to start grabbing?", "Warning", wxYES_NO)==wxYES)
			fen->ModeCamera(evt);
		}
	}
if (event.GetId()==3791)
	{
	fen->BasculeZoom();

	}
if (event.GetId()==3790)
	{
	fen->BasculeStat();

	}
if (event.GetId()==3792)
	{
	fen->BasculePlan(2);
	fen->NouvelleImage();
	fen->MAJNouvelleImage();
	if (fen->PlanActif(2))
		m_planRouge->SetBackgroundColour(*wxRED);
	else
		m_planRouge->SetBackgroundColour(*wxWHITE);
	}
if (event.GetId()==3793)
	{
	fen->BasculePlan(1);
	fen->NouvelleImage();
	fen->MAJNouvelleImage();
	if (fen->PlanActif(1))
		m_planVert->SetBackgroundColour(*wxGREEN);
	else
		m_planVert->SetBackgroundColour(*wxWHITE);

	}
if (event.GetId()==3794)
	{
	fen->BasculePlan(0);
	fen->NouvelleImage();
	fen->MAJNouvelleImage();
	if (fen->PlanActif(0))
		m_planBleu->SetBackgroundColour(*wxBLUE);
	else
		m_planBleu->SetBackgroundColour(*wxWHITE);

	}
event.Skip();

}

void BarreInfo::BasculeCurseur(wxCommandEvent& event)
{
curseur=!curseur;
if (!curseur)
	SetStatusText(wxEmptyString, Champ_Text);
wxCommandEvent evt;
event.Skip();
}

void BarreInfo::DoToggle()
{

}

void BarreInfo::UpdateClock()
{
static int tempsRestant=0;
char w=fen->UtilisateurAbsent();
if (w)
	return;
wxString s(wxDateTime::Now().FormatTime());
if (fen->SequenceEnCours())
	{
	if (tempsRestant==0)
		tempsRestant=fen->TempsRestantHorlogeSeq()/1000;
	tempsRestant--;
	wxString ss;
	ss.Printf(_T("**%d**"),tempsRestant);
	s=s+_T(" ")+ss;
	}
SetStatusText(s, Champ_Clock);
int nbCanaux;
if (fen && fen->ImAcq() ) //Invalid read of size 4 cv::Mat::channels() const (mat.inl.hpp:709)
	 nbCanaux=fen->ImAcq()->channels();
if (fen && fen->ImAcq() && fen->ImAcq()->channels()>=3) //Invalid read of size 4 cv::Mat::channels() const (mat.inl.hpp:709)
	{
	 nbCanaux=fen->ImAcq()->channels();
	m_planRouge->Show(true);
	m_planVert->Show(true);
	m_planBleu->Show(true);
	}
else
	{
	m_planRouge->Show(true);
	m_planVert->Show(false);
	m_planBleu->Show(false);
	}
}

void BarreInfo::UpdateCurseur(int x,int y,int val)
{
wxString	s;
s.Printf(_T("(%d,%d)=%d"),x,y,val);
    SetStatusText(s, Champ_Text);

}

void BarreInfo::UpdateCurseur(int x,int y,double val)
{
wxString	s;
s.Printf(_T("(%d,%d)=%lf"),x,y,val);
    SetStatusText(s, Champ_Text);

}

void BarreInfo::UpdateCurseur(int x,int y,int r,int g,int b)
{
wxString	s;
s.Printf(_T("(%d,%d)=(%d,%d,%d)"),x,y,r,g,b);
    SetStatusText(s, Champ_Text);

}
void BarreInfo::UpdateCurseur(int x,int y,float r,float g,float b)
{
wxString	s;
s.Printf(_T("(%d,%d)=(%f,%f,%f)"),x,y,r,g,b);
    SetStatusText(s, Champ_Text);

}
void BarreInfo::UpdateCurseur(int x,int y,std::complex<float> r,std::complex<float> g,std::complex<float> b)
{
wxString	s;
s.Printf(_T("(%d,%d)=(%lf,%lf,%lf)"),x,y,abs(r),abs(g),abs(b));
    SetStatusText(s, Champ_Text);

}
