#include "wx/wxprec.h"

#include "ControleCamera.h"
#include "FenetrePrincipale.h"
#include "imagestat.h"
#include "EvenementCamera.h"
#include <wx/string.h>
#include "CameraAndor.h"
#include "CameraOpenCV.h"

#ifdef __WXGTK__
#include "bitmaps/Pause.xpm"
#include "bitmaps/Play.xpm"
#include "bitmaps/Record.xpm"
#include "bitmaps/Avance.xpm"
#include "bitmaps/AvanceFin.xpm"
#include "bitmaps/Retour.xpm"
#include "bitmaps/RetourDebut.xpm"
#else
#include "bitmaps\Pause.xpm"
#include "bitmaps\Play.xpm"
#include "bitmaps\Record.xpm"
#include "bitmaps\Avance.xpm"
#include "bitmaps\AvanceFin.xpm"
#include "bitmaps\Retour.xpm"
#include "bitmaps\RetourDebut.xpm"

#endif



#define ID_CORRECTION_GAIN 201 // Estimation Bruit additif
#define ID_DEB_ESTIM_FOND 204 // Estimation Bruit additif
#define ID_FIN_ESTIM_FOND 205
#define ID_DEB_ESTIM_GAIN 208 // Estimation Gain
#define ID_FIN_ESTIM_GAIN 209
#define ID_ASC_CONTRAST 100
#define ID_VAL_ASC_CONTRAST 200
#define ID_TAILLE_VIDEO 300
#define ID_BOUTON_MAGNETO 320

BEGIN_EVENT_TABLE(ControleCamera, wxFrame)
    EVT_CLOSE(ControleCamera::OnClose)
    EVT_SCROLL(ControleCamera::OnSlider)
    EVT_CHOICE(wxID_ANY, ControleCamera::OnChoice)
    EVT_TEXT_ENTER(wxID_ANY, ControleCamera::OnTextValider)
    EVT_BUTTON(220, ControleCamera::ExpositionAutomatique)
    EVT_BUTTON(320, ControleCamera::Play)
    EVT_BUTTON(321, ControleCamera::Record)
    EVT_BUTTON(322, ControleCamera::Pause)
    EVT_BUTTON(323, ControleCamera::Debut)
    EVT_BUTTON(324, ControleCamera::Fin)
    EVT_BUTTON(325, ControleCamera::Suivante)
    EVT_BUTTON(326, ControleCamera::Precedente)
    EVT_SLIDER(327, ControleCamera::AllerA)
    EVT_BUTTON(ID_DEB_ESTIM_GAIN, ControleCamera::EstimationGain)
    EVT_BUTTON(ID_FIN_ESTIM_GAIN, ControleCamera::EstimationGain)
	EVT_CHECKBOX(211,ControleCamera::ModeMoyenne)
	EVT_CHECKBOX(ID_CORRECTION_GAIN,ControleCamera::OnCaseCocher)

END_EVENT_TABLE()

ProcessGestionCamera *pThread=NULL;

#define ID_GAIN_CAM 2500
#define ID_GLI_BUTTER 2501


void	ControleCamera::PThread(void *t)
{
if (pThread && !t)
	{
	wxStaticText *s=(wxStaticText*)FindWindowById(ID_DEB_ESTIM_GAIN,ongletFond);
	s->SetLabelText(_("Start"));
	}

pThread=(ProcessGestionCamera *)t;
}

ControleCamera::ControleCamera(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
commEvt=1;
imagePrete=0;
cam =NULL;
osgApp=NULL;
parent=NULL;

SetIcon(wxIcon("wxocv.bmp",wxBITMAP_TYPE_ICO ));
panneau = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);
m_sizerFrame = new wxBoxSizer(wxVERTICAL);

//Centre(wxBOTH);
listeFenetreOnglet = new wxNotebook(panneau, -1,wxDefaultPosition, wxDefaultSize,wxNB_TOP);
/*Tableur *infoSequence = new Tableur((wxFrame*)listeFenetreOnglet,200,15);
listeFenetreOnglet->AddPage(infoSequence, "Cloud Image");            */

panneau->SetSizer(m_sizerFrame);
OuvertureOngletStatus();
ongletTemporels=NULL;
if (cam && !strcmp(cam->NomCamera(),"ANDOR"))
	{
	//OuvertureOngletEMCCD();
	}
OuvertureOngletMagneto();
OuvertureOngletFond();
OuvertureOngletMoyenne();

//m_sizerFrame->Fit(this);
//m_sizerFrame->SetSizeHints(this);
m_sizerFrame->Insert(0, listeFenetreOnglet, 5, wxEXPAND | wxALL, 4);
m_sizerFrame->Hide(listeFenetreOnglet);
m_sizerFrame->Show(listeFenetreOnglet);
Show(false);

}

void ControleCamera::DebutAcquisition(void)
{
pThread=new ProcessGestionCamera(this,cam);
pThread->Create();
pThread->Run();
}

void ControleCamera::FinAcquisition(void)
{
pThread->Kill();
}

void ControleCamera::NouvelleImage(wxCommandEvent& event)
{
if (!cam || !parent)
	return;
if (cam->IsRunning())
	{
	wxMessageBox(_("Camera must be paused first!"));
	return;
	}
if (event.GetId()==ID_DEB_ESTIM_FOND)
	{
	}

}

void ControleCamera::EstimationGain(wxCommandEvent& event)
{
if (!cam || !parent)
	return;
if (cam->IsRunning())
	{
	wxMessageBox(_("Camera must be paused first!"));
	return;
	}
if (event.GetId()==ID_DEB_ESTIM_GAIN)
	{
	if (pThread==NULL)
		{
		wxStaticText *s=(wxStaticText*)FindWindowById(ID_DEB_ESTIM_GAIN,ongletFond);
		s->SetLabelText(_("Stop"));
		pThread= new ProcessGestionCamera(this,cam);
		pThread->Run();
		}
	else if (pThread!=NULL && pThread->IsRunning())
		{
		if (pThread->Delete()!= wxTHREAD_NO_ERROR)
			{
			wxMessageBox(_("Process is stopped!"));
			pThread=NULL;
			}
		else;
			wxMessageBox(_("Cannot stop Process!"));
		}
	}
}


void ControleCamera::OnClose(wxCloseEvent& event)
{
}


void ControleCamera::ModeMoyenne(wxCommandEvent &w)
{
if (!cam || !parent)
	return;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (cam->ModeMoyenne())
	cam->DesActiveModeMoyenne();
else
	cam->ActiveModeMoyenne();
}

void ControleCamera::DefTempsExposition(float x)
{
if (!cam)
	return;
	cam->DefTempsExposition(x);
wxString s;
s.Printf(_T("%f"),x);
wxWindowList&w=ongletTemporels->GetChildren();
((wxTextCtrl*)w[7])->ChangeValue(s);

}

void ControleCamera::DefGainEMCCD(float x)
{
if (!cam)
	return;
slEMGain->SetValue(int(x));
cam->DefEMCCDGain(int(x));
}


void ControleCamera::DefModeGainEMCCD(int x)
{
if (!cam)
	return;
cam->DefModeGain(x);

}

void ControleCamera::AllerA(wxCommandEvent& event)
{
    DefPositionGlissiere();
}



void ControleCamera::OnSlider(wxScrollEvent &w)
{
if (!cam || !parent)
	return;

wxSlider *cbContrast=(wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels);
wxSlider *cbGain=(wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels);
wxSlider *cbLum=(wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels);
wxSlider *cbSat=(wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels);
wxSlider *cbTpsExpo = (wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST + 4, ongletTemporels);
wxSlider *cbButter = (wxSlider*)wxWindow::FindWindowById(ID_GLI_BUTTER , ongletMoyenne);
bool b;
if (w.GetEventObject()==cbContrast)
	{
	if (cam->Contraste(w.GetInt()))
		{
		cbContrast->SetValue(w.GetInt());
		wxString s;
		s.Printf("%d",w.GetInt());
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST,ongletTemporels))->SetLabelText(s);
		}
	else
		wxMessageBox(_("Value out of range"));
	}
else if (w.GetEventObject()==cbGain)
	{
	if (cam->Gain(w.GetInt()))
		{
		cbGain->SetValue(w.GetInt());
		wxString s;
		s.Printf("%d",w.GetInt());
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+1,ongletTemporels))->SetLabelText(s);
		}
	else
		wxMessageBox(_("Value out of range"));
}
else if (w.GetEventObject()==cbLum)
	{
	if (cam->Luminosite(w.GetInt()))
		{
		cbLum->SetValue(w.GetInt());
		wxString s;
		s.Printf("%d",w.GetInt());
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+2,ongletTemporels))->SetLabelText(s);
		}
	else
		wxMessageBox(_("Value out of range"));
}
else if (w.GetEventObject()==cbSat)
	{
	if (cam->Saturation(w.GetInt()))
		{
		cbSat->SetValue(w.GetInt());
		wxString s;
		s.Printf("%d",w.GetInt());
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+3,ongletTemporels))->SetLabelText(s);
		}
	else
		wxMessageBox(_("Value out of range"));
}
else if (w.GetEventObject() == cbTpsExpo)
	{
	if (cam->TempsExposition(w.GetInt()))
		{
		cbTpsExpo->SetValue(w.GetInt());
		wxString s;
		s.Printf("%d", w.GetInt());
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST + 4, ongletTemporels))->SetLabelText(s);
		}
	else
		wxMessageBox(_("Value out of range"));
	}
else if (w.GetEventObject() == cbButter)
	{
	cam->DefIndFiltreMoyenne(w.GetInt());
		{
		cbButter->SetValue(w.GetInt());
		wxString s;
		}
	}

}

void ControleCamera::OnCaseCocher( wxCommandEvent& event )
{
if  (!parent || !cam)
	return;
FenetrePrincipale *p=((FenetrePrincipale*)parent);
switch(event.GetId()){
case ID_CORRECTION_GAIN:
	p->DefCorrectionGain(!p->CorrectionGain());
	break;
default:
	break;
	}
}




void ControleCamera::OnSpinUp(wxScrollEvent &w)
{
}
		/*!< Gestion des boutons spins */
void ControleCamera::OnSpinDown(wxScrollEvent &w)
{

}

void ControleCamera::OnTextValider(wxCommandEvent &w)
{
if (!cam)
	return;
wxString s=((wxTextCtrl*)w.GetEventObject())->GetValue();
long	valEntiere;
double	valFlottante;

s.ToLong(&valEntiere);
s.ToDouble(&valFlottante);
switch (w.GetId()){
case 104 :
	cam->DefCoinGauche(valEntiere);
	break;
case 106 :
	cam->DefCoinDroit(valEntiere);
	break;
case 108 :
	cam->DefCoinHaut(valEntiere);
	break;
case 110 :
	cam->DefCoinBas(valEntiere);
	break;
case 204 :
	cam->DefTempsExposition(valFlottante);
	((wxOsgApp*)osgApp)->SauverConfiguration();
	break;
case 206 :
	cam->DefNombreAccumulation(valEntiere);
	break;
case 208 :
	cam->DefFrequenceAccumulation(valFlottante);
	break;
case 210 :
	cam->DefNombreCinetique(valEntiere);
	break;
case 212 :
	cam->DefFrequenceCinetique(valFlottante);
	break;
	}
}


void ControleCamera::OnChoice( wxCommandEvent& event )
{
if (!cam)
	return;
if ( event.GetEventType() == wxEVT_COMMAND_COMBOBOX_SELECTED )
	return;
int i = ((wxChoice*)event.GetEventObject())->GetCurrentSelection();
wxSize tailleCapteur[20];
tailleCapteur[0]=wxSize(160,120);tailleCapteur[1]=wxSize(176,144);tailleCapteur[2]=wxSize(320,176);
tailleCapteur[3]=wxSize(320,240);tailleCapteur[4]=wxSize(352,288);tailleCapteur[5]=wxSize(432,240);
tailleCapteur[6]=wxSize(544,288);tailleCapteur[7]=wxSize(640,360);tailleCapteur[8]=wxSize(640,480);
tailleCapteur[9]=wxSize(752,416);tailleCapteur[10]=wxSize(800,448);tailleCapteur[11]=wxSize(800,600);
tailleCapteur[12]=wxSize(864,480);tailleCapteur[13]=wxSize(960,544);tailleCapteur[14]=wxSize(960,720);
tailleCapteur[15]=wxSize(1024,576);tailleCapteur[16]=wxSize(1184,656);tailleCapteur[17]=wxSize(1280,720);
tailleCapteur[18]=wxSize(1280,960);tailleCapteur[19]=wxSize(-1,-1);
if (i>=0 &&i<=18 && cam->ReglageTaille())
	{
	cam->DefTailleImage(tailleCapteur[i].x,tailleCapteur[i].y);
	}

}


void ControleCamera::OuvertureOngletFond()
{
wxPoint	position[]={
// Texte		Réglette
wxPoint(10,10),wxPoint(170,10),wxPoint(280,10),
wxPoint(10,60),wxPoint(170,60),wxPoint(280,60),wxPoint(390,60),
wxPoint(10,100),wxPoint(170,100),wxPoint(280,100),wxPoint(390,100)};
wxSize	taille[]={
// Texte		Réglette
wxSize(160,30),wxSize(110,30),wxSize(110,30),
wxSize(160,30),wxSize(110,30),wxSize(110,30),wxSize(110,30),
wxSize(160,30),wxSize(110,30),wxSize(110,30),wxSize(110,30)};
long style=wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS ;

wxString	legende[]={_T("Enable bias correction"),_T("Enable background correction "),_T("Enable function correction"),
_T("Cumulate Bias Image"),_T("Start"),_T("Reset Dark Image"),_T("Load Dark Image"),
_T("Cumulate Background Image (gain)"),_T("Start"),_T("Reset Background"),_T("Load Background"),
_T("Cumulate Zero level Function "),_T("Start"),_T("Reset Function")
};
ongletFond = new wxWindow(listeFenetreOnglet,-1);
int i=0;
wxCheckBox *t=new wxCheckBox(ongletFond,200+i,legende[i],position[i], taille[i]); //bias 200
i++;
if (osgApp)
	t->SetValue(((wxOsgApp*)osgApp)->Graphique()->CorrectionBiais());
t=new wxCheckBox(ongletFond,ID_CORRECTION_GAIN,legende[i],position[i], taille[i]); // background 201
if (osgApp)
	t->SetValue(((wxOsgApp*)osgApp)->Graphique()->CorrectionFond());
i++;
new wxCheckBox(ongletFond,200+i,legende[i],position[i], taille[i]);// function 202
i++;
new wxCheckBox(ongletFond,200+i,legende[i],position[i], taille[i]);// cumul biais 203
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);//start 204
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);// reset 205
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);// load 206
i++;
new wxCheckBox(ongletFond,200+i,legende[i],position[i], taille[i]);// cumul fond 207
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);// start 208
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);// reset 209
i++;
new wxButton(ongletFond,200+i,legende[i],position[i], taille[i]);// load 210
i++;
listeFenetreOnglet->AddPage(ongletFond, _T("Background Settings"));
ongletFond->Refresh();
}

void ControleCamera::OuvertureOngletMoyenne()
{
wxPoint	position[]={
// Texte		Réglette
wxPoint(10,10),wxPoint(110,10),
wxPoint(10,40),wxPoint(110,40)};
wxSize	taille[]={
// Texte		Réglette
wxSize(100,30),wxSize(100,30),wxSize(100,30),wxSize(150,50)};
long style=wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS ;

wxString	legende[]={_("Mean Mode"),wxEmptyString,_("Frequency cut off ([0. 1.0] "),wxEmptyString};
ongletMoyenne = new wxWindow(listeFenetreOnglet,-1);
int i=0;
new wxStaticText(ongletMoyenne,-1,legende[i],position[i], taille[i]);
i++;
new wxCheckBox(ongletMoyenne,211,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletMoyenne,-1,legende[i],position[i], taille[i]);
i++;
wxSlider *slIndFiltre=new wxSlider(ongletMoyenne,ID_GLI_BUTTER,0, 0,10 ,position[i], taille[i],style);
slIndFiltre->SetLabel(legende[i]);
i++;
listeFenetreOnglet->AddPage(ongletMoyenne, _("Time Filtering"));
ongletMoyenne->Refresh();
}



void ControleCamera::OuvertureOngletStatus()
{
ongletStatus = new wxWindow(listeFenetreOnglet,-1);

// Texte		Réglette
wxString	legende[]={_T("Camera model"),_T(" "),_T("CCD size"),_T(" ")};
wxPoint	position[]={
wxPoint(10,10),wxPoint(100,10),
wxPoint(10,60),wxPoint(100,60)};
wxSize	taille[]={
// Texte		Réglette
wxSize(90,40),wxSize(200,40),
wxSize(90,40),wxSize(200,40)};
for (int i=0;i<4;i++)
	new wxStaticText(ongletStatus,i,legende[i],position[i], taille[i]);

listeFenetreOnglet->AddPage(ongletStatus, _T("Informations"));
DrawOngletStatus();
ongletStatus->Refresh();
}

void	ControleCamera::DefCamera(CameraVirtuelle *c)
{
if (c==NULL)
	{
	Show(false);
	cam=c;
	return;
	}
if (cam==NULL && c)
	Show(true);
cam=c;

DrawOngletStatus();
wxCheckBox *cb=(wxCheckBox*)wxWindow::FindWindowById(211,ongletMoyenne);
if (cam->ModeMoyenne())
	cb->SetValue(true);
else
	cb->SetValue(false);
wxSlider *cs = (wxSlider*)wxWindow::FindWindowById(ID_GLI_BUTTER, ongletMoyenne);
cs->SetValue(cam->IndFiltreMoyenne());
if (!ongletTemporels || !wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels))
	return;

if (cam->ReglageContraste() )
	{
	double v,min=0,max=100;
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels))->Enable();
	if (cam && cam->ReglageContraste())
		{
		v=cam->Contraste();
		if (v>0 && v<256)
			{
			max=256;
			min=-256;
			}
		else  if (v!=DBL_MAX)
			{
			min = -20;
			max = 20;
		}
		wxString s;
		if (v!=DBL_MAX)
			s.Printf("%d",int(v));
		else
			s.Printf("Error");
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST,ongletTemporels))->SetLabelText(s);
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels))->SetMax(int(max));
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels))->SetMin(int(min));
		if (v!=DBL_MAX)
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST, ongletTemporels))->SetValue(int(v));
		else
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST, ongletTemporels))->Disable();
	}
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST,ongletTemporels))->Enable();

	}
else
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST,ongletTemporels))->Disable();
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST,ongletTemporels))->Disable();
	}
if (cam->ReglageGain())
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels))->Enable();
	double v,min=0,max=100;
	if (cam && cam->ReglageGain())
		{
		v=cam->Gain();
		if (v>0 && v<256)
			{
			max = 256;
			min = -256;
		}
		else  if (v!=DBL_MAX)
			{
			min=-10*abs(v);
			max=10*abs(v)+1;
			}
		wxString s;
		if (v!=DBL_MAX)
			s.Printf("%d",int(v));
		else
			s.Printf("Error");
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+1,ongletTemporels))->SetLabelText(s);
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels))->SetMax(int(max));
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels))->SetMin(int(min));
		if (v!=DBL_MAX)
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels))->SetValue(int(v));
		else
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1, ongletTemporels))->Disable();
	}
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+1,ongletTemporels))->Enable();
	}
else
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+1,ongletTemporels))->Disable();
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+1,ongletTemporels))->Disable();
	}
if (cam->ReglageLuminosite())
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels))->Enable();
	double v,min=0,max=100;
	if (cam && cam->ReglageLuminosite())
		{
		v=cam->Luminosite();
		if (v>0 && v<256)
			{
			max=256;
			min=-256;
			}
		else  if (v!=DBL_MAX)
			{
			min = -20;
			max = 20;
		}
		wxString s;
		if (v!=DBL_MAX)
			s.Printf("%d",int(v));
		else
			s.Printf("Error");
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+2,ongletTemporels))->SetLabelText(s);
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels))->SetMax(int(max));
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels))->SetMin(int(min));
		if (v!=DBL_MAX)
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels))->SetValue(int(v));
		else
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2, ongletTemporels))->Disable();
	}
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+2,ongletTemporels))->Enable();
	}
else
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+2,ongletTemporels))->Disable();
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+2,ongletTemporels))->Disable();
	}
if (cam->ReglageSaturation())
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels))->Enable();
	double v,min=0,max=100;
	if (cam && cam->ReglageSaturation())
		{
		v=cam->Saturation();
		if (v>0 && v<256)
			{
			max=256;
			min=-256;
			}
		else  if (v!=DBL_MAX)
			{
			min=-10*abs(v);
			max=10*abs(v)+1;
			}
		wxString s;
		if (v!=DBL_MAX)
			s.Printf("%d",int(v));
		else
			s.Printf("Error");
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+3,ongletTemporels))->SetLabelText(s);
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels))->SetMax(int(max));
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels))->SetMin(int(min));
		if (v!=DBL_MAX)
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels))->SetValue(int(v));
		else
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3, ongletTemporels))->Disable();
	}
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+3,ongletTemporels))->Enable();
	}
else
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+3,ongletTemporels))->Disable();
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+3,ongletTemporels))->Disable();
	}
if (cam->ReglageTpsExpo())
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4,ongletTemporels))->Enable();
	double v,min=0,max=100;
	if (cam && cam->ReglageTpsExpo())
		{
		v=cam->TempsExposition();
		if (v>0 && v<256)
			{
			max=256;
			min=-256;
			}
		else  if (v!=DBL_MAX)
			{
			min = -20;
			max = 20;
		}
		wxString s;
		if (v!=DBL_MAX)
			s.Printf("%d",int(v));
		else
			s.Printf("Error");
		((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+4,ongletTemporels))->SetLabelText(s);
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4,ongletTemporels))->SetMax(int(max));
		((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4,ongletTemporels))->SetMin(int(min));
		if (v!=DBL_MAX)
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4,ongletTemporels))->SetValue(int(v));
		else
			((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4, ongletTemporels))->Disable();
	}
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+4,ongletTemporels))->Enable();
	}
else
	{
	((wxSlider*)wxWindow::FindWindowById(ID_ASC_CONTRAST+4,ongletTemporels))->Disable();
	((wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_ASC_CONTRAST+4,ongletTemporels))->Disable();
	}
if (c->FluxVideo())
    DefPositionGlissiere();
};

void ControleCamera::DefPositionGlissiere()
{
    if (cam && cam->FluxVideo())
    {
        wxSlider *w = ((wxSlider*)wxWindow::FindWindowById(ID_BOUTON_MAGNETO + 7, ongletMagneto));
        if (w)
        {
            w->SetMin(0);
            w->SetMax(cam->NbImageVideo() - 1);
            w->SetValue(cam->PositionVideo());
            w->Enable();
        }


    }

}


void ControleCamera::DrawOngletStatus()
{
if (!cam)
	return;
wxWindowList&w=ongletStatus->GetChildren();
wxString s(cam->NomCamera(), wxConvUTF8);;
int wx=cam->NbColonnePhys(),wy=cam->NbLignePhys();
w[1]->SetLabel(s);
s.Printf(_T("x = %6d pixels y = %6d pixels Fps =%f"), wx,wy,1000.0/cam->DefTpsInactif());

w[3]->SetLabel(s);

}

void ControleCamera::OuvertureOngletParametresGeometries()
{
ongletGeometries = new wxWindow(listeFenetreOnglet,-1);
wxPoint	position[]={
wxPoint(10,10),wxPoint(110,10),
wxPoint(10,140),wxPoint(110,140),
wxPoint(230,10),wxPoint(280,10),wxPoint(340,10),wxPoint(380,10),
wxPoint(230,40),wxPoint(280,40),wxPoint(340,40),wxPoint(380,40)
};
wxSize	taille[]={
// Texte		Réglette
wxSize(100,30),wxSize(100,30),
wxSize(100,30),wxSize(100,30),
wxSize(40,20),wxSize(50,20),wxSize(40,20),wxSize(50,20),
wxSize(40,20),wxSize(50,20),wxSize(40,20),wxSize(50,20)
};
wxString	legende[]={_T("Sub Image"),_T(" "),_T("Binning"),_T("CCD size")
,_T("Left"),_T(" "),_T("Right"),_T(" "),_T("top"),_T(" "),_T("Bottom"),_T(" "),_T(" ")};

int i=0;
new wxStaticText(ongletGeometries,-1,legende[i],position[i], taille[i]);
i++;
wxArrayString sousImage;
wxSize *w=cam->LitTailleCapteur();
bool *b=cam->LitTailleAutorisee();
for (int i=0;i<NB_TAILLE_VIDEO;i++)
	if (b[i])
		{
		wxString s;
		s.Printf("%dX%d",w[i].GetY(),w[i].GetX());
		sousImage.Add(s);
		}

wxChoice *t=new wxChoice(ongletGeometries,100,position[i],taille[i], sousImage);
t->SetSelection(0);
i++;
/*new wxStaticText(ongletGeometries,101,legende[i],position[i], taille[i]);
i++;
wxArrayString taillePixel;
taillePixel.Add(_T("1x1"));
taillePixel.Add(_T("2x2"));
taillePixel.Add(_T("4x4"));
taillePixel.Add(_T("8x8"));
taillePixel.Add(_T("16x16"));
t = new wxChoice(ongletGeometries,102,position[i],taille[i], taillePixel);
t->SetSelection(0);
i++;
new wxStaticText(ongletGeometries,103,legende[i],position[i], taille[i]);
i++;
wxTextCtrl *te=new wxTextCtrl(ongletGeometries,104,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
te->SetLabel(_T("0"));
i++;
new wxStaticText(ongletGeometries,105,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletGeometries,106,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
te->SetLabel(_T("1004"));
i++;
new wxStaticText(ongletGeometries,107,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletGeometries,108,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
te->SetLabel(_T("0"));
i++;
new wxStaticText(ongletGeometries,109,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletGeometries,110,legende[i],position[i], taille[i]),wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB;
te->SetLabel(_T("1002"));
i++;
wxWindowList&w=ongletGeometries->GetChildren();
for (int j=4;j<=11;j++)
	w[j]->Hide();


listeFenetreOnglet->AddPage(ongletGeometries, _T("Geometry Settings"));
*/
ongletGeometries->Refresh();
}

void ControleCamera::OuvertureOngletParametresTemporels()
{
if (ongletTemporels)
	return;
ongletTemporels = new wxWindow(listeFenetreOnglet,-1);

wxPoint	position[]={
wxPoint(10,10),wxPoint(110,10),wxPoint(260,10), // Contrast
wxPoint(10,40),wxPoint(110,40),wxPoint(260,40), // Gain
wxPoint(10,70),wxPoint(110,70),wxPoint(260,70), // Luminosité
wxPoint(10,100),wxPoint(110,100),wxPoint(260,100), // Saturation
wxPoint(10,130),wxPoint(110,130),wxPoint(260,130), // temps d'exposition
wxPoint(10,170),wxPoint(110,170) // taille de l'image
};
wxSize	taille[]={
// Texte		Réglette
wxSize(100,30),wxSize(150,30),wxSize(100,30),
wxSize(100,30),wxSize(150,30),wxSize(100,30),
wxSize(100,30),wxSize(150,30),wxSize(100,30),
wxSize(100,30),wxSize(150,30),wxSize(100,30),
wxSize(100,30),wxSize(150,30),wxSize(100,30),
wxSize(100,30),wxSize(100,20),
};
wxString	legende[]={
	_("Contrast"),"","",
	_("Gain"),"","",
	_("Brightness"),"","",
	_("Saturation"),"","",
	_("Exposure time"),"","",
	_("Size"),"",""};
int i=0,j=0;
for (;i<5;i++)
	{
	double v,min=0,max=100;
	switch(i){
	case 0://Contraste
		if (cam && cam->ReglageContraste())
			{
			v=cam->Contraste();
			if (v>0 && v<256)
				{
				max=256;
				min=1;
				}
			else  if (v!=DBL_MAX)
				{
				min=-10*abs(v);
				max=10*abs(v)+1;
				}
			}
		break;
	case 1://gain
		if (cam && cam->ReglageGain())
			{
			v=cam->Gain();
			if (v>0 && v<256)
				{
				max=256;
				min=1;
				}
			else  if (v!=DBL_MAX)
				{
				min=-10*abs(v);
				max=10*abs(v);
				}
			}
		break;
	case 2://luminosite
		if (cam && cam->ReglageLuminosite())
			{
			v=cam->Contraste();
			if (v>0 && v<256)
				{
				max=256;
				min=1;
				}
			else  if (v!=DBL_MAX)
				{
				min=-10*abs(v);
				max=10*abs(v);
				}
			}
		break;
	case 3://Saturation
		if (cam && cam->ReglageSaturation())
			{
			v=cam->Contraste();
			if (v>0 && v<256)
				{
				max=256;
				min=1;
				}
			else  if (v!=DBL_MAX)
				{
				min=-10*abs(v);
				max=10*abs(v);
				}
			}
		break;
	case 4://Exposure time
		if (cam && cam->ReglageTpsExpo())
			{
			v=cam->TempsExposition();
			if (v>0 && v<256)
				{
				max=256;
				min=1;
				}
			else  if (v!=DBL_MAX)
				{
				min=-10*abs(v);
				max=10*abs(v);
				}
			}
		break;
		}
	wxString s;
	if (v!=DBL_MAX)
		s.Printf("%d",int(v));
	else
		s.Printf("Error");
	new wxStaticText(ongletTemporels,-1,legende[j],position[j], taille[j]);
	j++;
	new wxSlider(ongletTemporels,ID_ASC_CONTRAST+i,(int)v, int(min),int(max) ,position[j], taille[j],wxSL_HORIZONTAL);
	j++;
	new wxTextCtrl(ongletTemporels,ID_VAL_ASC_CONTRAST+i,s ,position[j], taille[j],wxSL_HORIZONTAL);
	j++;
	}
new wxStaticText(ongletTemporels,-1,legende[j],position[j], taille[j]);
j++;
wxArrayString mode;
mode.Add("160X120");mode.Add("176X144");mode.Add("320X176");
mode.Add("320X240");mode.Add("352X288");mode.Add("432X240");
mode.Add("544X288"); mode.Add("640X360"); mode.Add("640X480");
mode.Add("752X416"); mode.Add("800X448"); mode.Add("800X600");
mode.Add("864X480");mode.Add("960X544");mode.Add("960X720");
mode.Add("1024X576");mode.Add("1184X656");mode.Add("1280X720");
mode.Add("1280X960");

wxChoice *t=new wxChoice(ongletTemporels,ID_TAILLE_VIDEO,position[j],taille[j], mode);
t->SetSelection(0);



listeFenetreOnglet->AddPage(ongletTemporels, _T("Time Settings"));
ongletTemporels->Refresh();
}

char			ControleCamera::ImagePrete(void)
{
if (!cam)
	return 0;
return cam->ImagePrete();
}

char ControleCamera::Image()
{
if (!cam || !parent)
	return 1;
cam->Acquisition();
std::vector<cv::Point2f> repereIni,repere;
EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
x->ptId=repereIni;
x->ptApp=repere;
x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
return 0;
}


void ControleCamera::OuvertureOngletEMCCD()
{
if (!cam)
	return;
/*if (strcmp(cam->NomCamera(),"Luca")!=0)
	return;
*/

ongletEMCCD = new wxWindow(listeFenetreOnglet,-1);
int	minGain=0,maxGain=255;
//GetEMGainRange(&minGain,&maxGain);
// Texte		Réglette
wxString	legende[]={_T("Gain Mode"),_T("Gain Value"),_T("CCD size"),_T(" ")};
wxPoint	position[]={
wxPoint(10,20),wxPoint(110,10),
wxPoint(10,60),wxPoint(100,60)};
wxSize	taille[]={
// Texte		Réglette
wxSize(90,40),wxSize(200,40),
wxSize(90,40),wxSize(200,40)};
long style=wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS ;

int	i=0;
wxArrayString modeGainEM;
int	nbMode=1;
if (cam)
	{
	if (!strcmp(cam->NomCamera(),"ANDOR"))
		{
		CameraAndor *c=(CameraAndor*)cam;
		if (c->EMGain8Bit())
			{
			nbMode++;
			modeGainEM.Add(_T("EM Gain 0-255"));
			}
		if (c->EMGain12Bit() )
			{
			nbMode++;
			modeGainEM.Add(_T("EM Gain 0-4095"));
			}
		if (c->EMGainLinear12Bit() )
			{
			nbMode++;
			modeGainEM.Add(_T("Linear Mode"));
			}
		if (c->EMGainReal12Bit() )
			{
			nbMode++;
			modeGainEM.Add(_T("Real EM gain"));
			}
		}
	}
wxChoice *t=new wxChoice(ongletEMCCD,300,position[i],taille[i], modeGainEM);
if (cam)
	t->SetSelection(cam->ModeGainEMCCD());
i++;
int	gain=(minGain+maxGain)/2;
//GetEMCCDGain(&gain);
slEMGain=new wxSlider(ongletEMCCD,ID_GAIN_CAM,gain, minGain,maxGain ,position[i], taille[i],style);
slEMGain->SetLabel(legende[i]);
i++;
if (nbMode==0)
	{
	t->Disable();
	slEMGain->Disable();
	}


listeFenetreOnglet->AddPage(ongletEMCCD, _T("EMCCD"));
DrawOngletStatus();
ongletStatus->Refresh();
}


void ControleCamera::ExpositionAutomatique(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
float tpsPause=cam->TempsExposition();
float tpsMin=tpsPause/100,tpsMax=tpsPause*20;
int		nbCycle=11;
float	rTps[11]={0.01,0.05,0.1,0.2,0.5,1,2,5,10,20,100};
float	optiTps[11];

cv::Mat	*histoImage=((wxOsgApp*)osgApp)->ImgStat()->Histogramme();
ImageInfoCV			*imAcq=((wxOsgApp*)osgApp)->Graphique()->ImAcq();
long nbLig=imAcq->LitNbLigne(),nbCol=imAcq->LitNbColonne();
long ligDeb=0,colDeb=0;

if (((wxOsgApp*)osgApp)->Graphique()->ModeRectangle())
	{
	wxRect	*r=((wxOsgApp*)osgApp)->Graphique()->RectangleSelec();
	colDeb=r->GetLeft();
	ligDeb=r->GetTop();
	nbLig=r->GetHeight();
	nbCol=r->GetWidth();
	}
wxWindowList w=(ongletTemporels->GetChildren());
float	sMax=-1;
int		cMax=0;
commEvt=0;
for (int cycle=0;cycle<nbCycle;cycle++)
	if (rTps[cycle]*tpsPause<5)
		{
		wxString ss;
		ss.Printf(_T("%.5f"),rTps[cycle]*tpsPause);
		((wxTextCtrl*)w[7])->SetValue(ss);
		w[7]->Refresh();
		cam->DefTempsExposition(rTps[cycle]*tpsPause);
		do
			{
			wxMilliSleep(tpsMin+370);
			}
		while (imagePrete==0);
		imagePrete=0;
		wxCommandEvent evt;
		NouvelleImage(evt);
//		imAcq->HistogrammeLocal(ligDeb,colDeb,&nbLig,&nbCol,histoImage);
		float	val[6]={0,1,10,20,40,80};
		int		pos[12];
//		imAcq->EstimationContraste(6,val,pos,histoImage);
		char visible=0;
		float s=0;
		for (int i=1;i<6 ;i++)
			if (i>0)
				s+=float(pos[2*i+1]-pos[2*i])*(val[i]-val[i-1]);
		optiTps[cycle]=s;
		if (s>sMax)
			{
			sMax=s;
			cMax=cycle;
			}
		}
	else
		optiTps[cycle]=-1;
float	pas;
if (cMax>1)
	pas = rTps[cMax-1];
else
	pas = rTps[cMax];
tpsMin= pas*tpsPause;
if (cMax<nbCycle)
	pas = rTps[cMax+1];
else
	pas =rTps[cMax];
tpsMax= pas*tpsPause;
pas = (tpsMax-tpsMin)/10;;
sMax=-1;
float s=0;
for (int j=0;j<10;j++,tpsMin+=pas)
	{
	wxString ss;
	ss.Printf(_T("%.5f"),tpsMin);
	((wxTextCtrl*)w[7])->SetValue(ss);
	w[7]->Refresh();
	cam->DefTempsExposition(tpsMin);
	do
		{
		wxMilliSleep(tpsMin+370);
		}
	while (imagePrete==0);
	imagePrete=0;
	wxCommandEvent evt;
	NouvelleImage(evt);
//	imAcq->HistogrammeLocal(ligDeb,colDeb,&nbLig,&nbCol,histoImage);
	float	val[6]={0,1,10,20,40,80};
	int		pos[12];
//	imAcq->EstimationContraste(6,val,pos,histoImage);
	char visible=0;
	s=0;
	for (int i=1;i<6 ;i++)
		if (i>0)
			s+=float(pos[2*i+1]-pos[2*i])*(val[i]-val[i-1]);
	if (s>sMax)
		{
		tpsMax=tpsMin;
		sMax=s;
		}
	}
tpsMin =tpsMax;
cam->DefTempsExposition(tpsMin);
		wxString ss;
		ss.Printf(_T("%.3f"),tpsMin);
		((wxTextCtrl*)w[7])->SetValue(ss);
commEvt=1;
}

void ControleCamera::Play(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
    cam->ModeAcqContinu(1);

}

void ControleCamera::Record(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
cv::VideoWriter *v = ((FenetrePrincipale*)parent)->Video();
if (v->isOpened())
    v->release();
wxDateTime	m;
wxString nom;

m=m.Now();
nom.Printf("%d",m.GetYear());
if (m.GetMonth()+1<10)
	nom.Printf("%s0%d", nom, m.GetMonth()+1);
else
	nom.Printf("%s%d", nom, m.GetMonth()+1);
if (m.GetDay()<10)
	nom.Printf("%s0%d", nom, m.GetDay());
else
	nom.Printf("%s%d", nom, m.GetDay());
if (m.GetHour()<10)
	nom.Printf("%s0%d", nom, m.GetHour());
else
	nom.Printf("%s%d", nom, m.GetHour());
if (m.GetMinute()<10)
	nom.Printf("%s0%d", nom, m.GetMinute());
else
	nom.Printf("%s%d", nom, m.GetMinute());
if (m.GetSecond()<10)
	nom.Printf("%s0%d", nom, m.GetSecond());
else
	nom.Printf("%s%d.avi", nom, m.GetSecond());
v->open(static_cast<const char*>(nom.c_str()),CV_FOURCC('P','I','M','1'),20,((FenetrePrincipale*)parent)->ImAcq()->size());
if (!v->isOpened())
    wxMessageBox(_("Error openning video for writting"));
}

void ControleCamera::Pause(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
if (cam)
{
    cam->ModeAcqContinu(0);
}
cv::VideoWriter *v = ((FenetrePrincipale*)parent)->Video();
if (v->isOpened())
    v->release();
}

void ControleCamera::Debut(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
if (cam)
{
    int x= cam->PositionVideo(0);
    cam->ModeAcqContinu(0);
    DefPositionGlissiere();
}
}

void ControleCamera::Suivante(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
if (cam)
{
	if (cam->PositionVideo()<cam->NbImageVideo()-1)
    {
        cam->PositionVideo(cam->PositionVideo() );
	    cam->ModeAcqContinu(0);
        DefPositionGlissiere();
    }
}
}

void ControleCamera::Precedente(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
if (cam)
{
    if (cam->PositionVideo() - 2 >= 0)
    {
        cam->PositionVideo(cam->PositionVideo()-2);
        cam->ModeAcqContinu(0);
        DefPositionGlissiere();

    }
}
}

void ControleCamera::Fin(wxCommandEvent& c)
{
if (!cam)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!parent)
    return;
if (cam)
{
    cam->PositionFinVideo();
    cam->ModeAcqContinu(0);
    DefPositionGlissiere();
}
}


void ControleCamera::OuvertureOngletMagneto()
{
wxPoint	position[]={
// Texte		Réglette
    wxPoint(50,10),wxPoint(100,10),wxPoint(150,10),wxPoint(200,10),wxPoint(250,10),wxPoint(300,10),wxPoint(350,10),wxPoint(50,60) };
wxSize	taille[]={
// Texte		Réglette
    wxSize(50,50),wxSize(50,50),wxSize(50,50),wxSize(50,50),wxSize(50,50),wxSize(50,50),wxSize(50,50),wxSize(350,50) };
long style=wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS ;

wxString	legende[]={_T("Play"),_T("record"),_T("pause")};
ongletMagneto = new wxWindow(listeFenetreOnglet,-1);
int i=0;

new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::Play),position[i], taille[i]);//start 204
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::Record),position[i], taille[i]);// reset 205
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::Pause),position[i], taille[i]);// load 206
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::RetourDebut),position[i], taille[i]);// load 206
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::AvanceFin),position[i], taille[i]);// load 206
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO +i,wxBitmap(::Avance),position[i], taille[i]);// load 206
i++;
new wxBitmapButton(ongletMagneto, ID_BOUTON_MAGNETO + i, wxBitmap(::Retour), position[i], taille[i]);// load 206
i++;
wxSlider *w=new wxSlider(ongletMagneto, ID_BOUTON_MAGNETO + i, 0,0,1,position[i], taille[i]);// load 206
w->Disable();
i++;

listeFenetreOnglet->AddPage(ongletMagneto, _T("Recorder"));
ongletMagneto->Refresh();
}

