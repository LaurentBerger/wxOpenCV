#include "wx/wxprec.h"

#include "ControleCamera.h"
#include "FenetrePrincipale.h"
#include "imagestat.h"
#include "EvenementCamera.h"
#include <wx/string.h>
#include "CameraAndor.h"
#include "CameraOpenCV.h"


#define ID_CORRECTION_GAIN 201 // Estimation Bruit additif
#define ID_DEB_ESTIM_FOND 204 // Estimation Bruit additif
#define ID_FIN_ESTIM_FOND 205
#define ID_DEB_ESTIM_GAIN 208 // Estimation Gain
#define ID_FIN_ESTIM_GAIN 209


BEGIN_EVENT_TABLE(ControleCamera, wxFrame)
    EVT_CLOSE(ControleCamera::OnClose)
    EVT_SCROLL(ControleCamera::OnSlider)
    EVT_CHOICE(wxID_ANY, ControleCamera::OnChoice)  
    EVT_TEXT_ENTER(wxID_ANY, ControleCamera::OnTextValider) 
    EVT_BUTTON(220, ControleCamera::ExpositionAutomatique)
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

SetIcon(wxIcon(_T("seec64.bmp"),wxBITMAP_TYPE_ICO ));
panneau = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);
m_sizerFrame = new wxBoxSizer(wxVERTICAL);

//Centre(wxBOTH);
listeFenetreOnglet = new wxNotebook(panneau, -1,wxDefaultPosition, wxDefaultSize,wxNB_TOP);
/*Tableur *infoSequence = new Tableur((wxFrame*)listeFenetreOnglet,200,15); 
listeFenetreOnglet->AddPage(infoSequence, "Cloud Image");            */

panneau->SetSizer(m_sizerFrame);
OuvertureOngletStatus();
if (cam && !strcmp(cam->NomCamera(),"ANDOR"))
	{
	OuvertureOngletEMCCD();
	}
//OuvertureOngletParametresGeometries();
OuvertureOngletParametresTemporels();
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
if (x==cam->ModeGainEMCCD())
	{
	wxWindowList&w=ongletEMCCD->GetChildren();
	((wxChoice*) w[0])->SetSelection(x);
	}

}	

void ControleCamera::OnSlider(wxScrollEvent &w)
{
if (!cam || !parent)
	return;

wxSlider *cb=(wxSlider*)wxWindow::FindWindowById(w.GetId(),this);

if(w.GetId()==ID_GAIN_CAM)
	{
	int x=w.GetInt();
	cam->DefEMCCDGain(x);
	if (cam->Erreur())
		wxMessageBox(_T("not a valid value"));
	} // if(w.GetEventObject()==slZFar)
if(w.GetId()==ID_GLI_BUTTER)
	{
	int x=w.GetInt();
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
	cam->DefIndFiltreMoyenne(x);

	} // if(w.GetEventObject()==slZFar)

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
if ( event.GetEventType() == wxEVT_COMMAND_COMBOBOX_SELECTED )
	return;
int i = ((wxChoice*)event.GetEventObject())->GetCurrentSelection();
switch(event.GetId()){
case 100: // Taille sous image
	{
// Si la taiile de l'image est custom alors les boites de positions s'affichent
	wxWindowList&w=ongletGeometries->GetChildren();
	if (i==4)
		{
		for (int j=4;j<=11;j++)
			w[j]->Show();
		}
	else if (w[5]->IsShown())
		{
		for (int j=4;j<=11;j++)
			w[j]->Hide();
		}
	if (cam)
		cam->DefTailleImage(i);
	}	
	break;
case 101: // Binning
	if (cam)
		cam->DefBinning(i);
	break;
case 200: // Mode d'acquisitions (single accumulate kinetic)
	{
	wxWindowList&w=ongletTemporels->GetChildren();
	switch(i){
	case 0:
		for (int j=8;j<=15;j++)
			w[j]->Hide();
		break;
	case 1:
		for (int j=6;j<=11;j++)
			w[j]->Show();
		for (int j=12;j<=15;j++)
			w[j]->Hide();
		break;
	case 2:
		for (int j=6;j<=15;j++)
			w[j]->Show();
		break;
		}
	if (cam)
		cam->DefModeAcquisition(i);
	}	
	break;
case 201: // Mode de déclenchement 
	if (cam)
		cam->DefModeDeclenchement(i);
	break;
case 202: // Type de zone de lecture
	if (cam)
		cam->DefZoneLecture(i);
	break;
case 300:
	if (cam->IsEMCCD())
		{
		wxString slc=((wxChoice*)event.GetEventObject())->GetStringSelection();
		if (!slc.Cmp(_T("EM Gain 0-255")))
			i=0;
		if (!slc.Cmp(_T("EM Gain 0-4095")))
			i=1;
		if (!slc.Cmp(_T("Linear Mode")))
			i=2;
		if (!slc.Cmp(_T("Real EM gain")))
			i=3;
		cam->DefModeGain(i);
		if (cam->Erreur())
			wxMessageBox(_T("Mode Not Available"));
		else
			{
			int	minGain=0,maxGain=255;
			cam->LireIntervalGain(&minGain,&maxGain);
			if (cam->Erreur())
				{
				minGain=0;
				maxGain=0;
				}
					
			slEMGain->SetMax(maxGain);
			slEMGain->SetMin(minGain);
			slEMGain->Refresh();
			}

		}
	else
		wxMessageBox(_T("Not Available"));
	
default :
	break;
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
wxSlider *cs=(wxSlider*)wxWindow::FindWindowById(ID_GLI_BUTTER,ongletMoyenne);	
cs->SetValue(cam->IndFiltreMoyenne());
};


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
ongletTemporels = new wxWindow(listeFenetreOnglet,-1); 

wxPoint	position[]={
wxPoint(10,10),wxPoint(110,10),wxPoint(210,10),
wxPoint(10,40),wxPoint(110,40),wxPoint(210,40),
wxPoint(10,80),wxPoint(110,80),
wxPoint(10,110),wxPoint(110,110),
wxPoint(10,140),wxPoint(110,140),
wxPoint(10,170),wxPoint(110,170),
wxPoint(10,200),wxPoint(110,200),
wxPoint(210,80)
};
wxSize	taille[]={
// Texte		Réglette
wxSize(100,30),wxSize(100,30),wxSize(100,30),
wxSize(100,30),wxSize(100,30),wxSize(100,30),
wxSize(100,30),wxSize(100,20),
wxSize(100,30),wxSize(100,20),
wxSize(100,30),wxSize(100,20),
wxSize(100,30),wxSize(100,20),
wxSize(100,30),wxSize(100,20),
wxSize(100,30)};
wxString	legende[]={_T("Mode"),_T("Triggering"),_T("Read out"),wxEmptyString,wxEmptyString,wxEmptyString,
_T("Exposure Time (s)"),_T("0.1 "),_T("No of Accumulations"),_T("1"),_T("Accumulation Cycle frequency (Hz)"),_T("0.5"),
_T("No. of images in Kinetic"),_T("1"),_T("Kinetic Duration (s)"),_T("10"),_T("Automatic Exposure"),_T("10")};
int i=0;
new wxStaticText(ongletTemporels,-1,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletTemporels,-1,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletTemporels,-1,legende[i],position[i], taille[i]);
i++;
wxArrayString mode;
mode.Add(_T("Single"));
mode.Add(_T("Accumulate"));
mode.Add(_T("Kinetic"));
mode.Add(_T("Fast Kinetic"));
wxChoice *t=new wxChoice(ongletTemporels,200,position[i],taille[i], mode);
i++;
t->SetSelection(0);
wxArrayString triggering;
triggering.Add(_T("Internal"));
triggering.Add(_T("External"));
triggering.Add(_T("Fast external"));
triggering.Add(_T("External start"));
t = new wxChoice(ongletTemporels,201,position[i],taille[i], triggering);
i++;
t->SetSelection(0);

wxArrayString lecture;
lecture.Add(_T("Image"));
lecture.Add(_T("Multi-track"));
t = new wxChoice(ongletTemporels,202,position[i],taille[i], lecture);
i++;
t->SetSelection(0);

new wxStaticText(ongletTemporels,203,legende[i],position[i], taille[i]);
i++;
wxTextCtrl *te=new wxTextCtrl(ongletTemporels,204,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletTemporels,205,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletTemporels,206,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletTemporels,207,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletTemporels,208,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletTemporels,209,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletTemporels,210,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletTemporels,211,legende[i],position[i], taille[i]);
i++;
te=new wxTextCtrl(ongletTemporels,212,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
wxWindowList&w=ongletTemporels->GetChildren();
for (int j=8;j<=15;j++)
	w[j]->Hide();
autoAdjust=new wxButton(ongletTemporels,220,legende[i],position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
autoAdjust->Disable();
i++;



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

