#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include "ImageInfo.h"
#include "wx/wxprec.h"
#include "wx/splash.h"
#include "mobile.xpm"


#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ControleCamera.h"
#include "imagestat.h"
//#include "OutilsImage.h"
#include "wxOsgApp.h"
#include "FenetrePrincipale.h"
#include "TableauBord.h"
#include "InterfaceAvance.h"
#include "GlisserForme.h"
#include "CameraAndor.h"
#include "CameraOpenCV.h"

#include <wx/image.h>
#include <wx/menu.h>
#include <wx/datetime.h>

#include <wx/filename.h>

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <strstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include "wxOpenCVConfig.h"

using namespace std;

#define TPSMISEENVEILLECOURBE	6000


// Gestion des constantes pour les langues
// language data
static const wxLanguage langIds[] =
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_FRENCH,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_GERMAN,
    wxLANGUAGE_RUSSIAN,
    wxLANGUAGE_BULGARIAN,
    wxLANGUAGE_CZECH,
    wxLANGUAGE_POLISH,
    wxLANGUAGE_SWEDISH,
#if wxUSE_UNICODE || defined(__WXMOTIF__)
    wxLANGUAGE_JAPANESE,
#endif
#if wxUSE_UNICODE
    wxLANGUAGE_GEORGIAN,
    wxLANGUAGE_ENGLISH,
    wxLANGUAGE_ENGLISH_US,
    wxLANGUAGE_ARABIC,
    wxLANGUAGE_ARABIC_EGYPT
#endif
};

// note that it makes no sense to translate these strings, they are
// shown before we set the locale anyhow
const wxString langNames[] =
{
    "System default",
    "French",
    "Italian",
    "German",
    "Russian",
    "Bulgarian",
    "Czech",
    "Polish",
    "Swedish",
#if wxUSE_UNICODE || defined(__WXMOTIF__)
    "Japanese",
#endif
#if wxUSE_UNICODE
    "Georgian",
    "English",
    "English (U.S.)",
    "Arabic",
    "Arabic (Egypt)"
#endif
};

// the arrays must be in sync
wxCOMPILE_TIME_ASSERT( WXSIZEOF(langNames) == WXSIZEOF(langIds),
                       LangArraysMismatch );



#ifndef _USRDLL
IMPLEMENT_APP(wxOsgApp)
#else

#ifdef SEEC_EXPORTS
#define SEEC_EXPORTS_API __declspec(dllexport)
#else
#define SEEC_EXPORTS_API __declspec(dllimport)
#endif

HANDLE ThreadId;
wxOsgApp *monAppli=NULL;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
 {
if (lpParameter)
	{
	Sleep(7000);
	return true;
	}
int argc=0;
char **argv=NULL;
wxApp::SetInstance(monAppli=new wxOsgApp());
wxEntryStart(argc,argv);
wxOsgApp *g=(wxOsgApp*)monAppli;
g->OnInit();
g->OnRun();
return true;
 }

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		  {
		  }
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
         wxEntryCleanup();
		break;
	}
	return TRUE;
}

extern "C" SEEC_EXPORTS_API void TestScilabRun(void)
{
ThreadProc((void*)NULL);
}

extern "C" SEEC_EXPORTS_API void TestScilabThread(void)
{
monAppli = (wxOsgApp *)NULL;
ThreadId = CreateThread(NULL,0,ThreadProc,(void*)monAppli,0,NULL);
}

extern "C" SEEC_EXPORTS_API void DLLToto(HWND handle)
{
	ThreadId = CreateThread(NULL,0,ThreadProc,(void*)monAppli,0,NULL);
}
#endif
wxDECLARE_EVENT(VAL_EVT_PTS_SUIVIS, EvtPointSuivis);
wxDEFINE_EVENT(VAL_EVT_PTS_SUIVIS, EvtPointSuivis);

#define EVT_PTS_SUIVIS(id, func) \
 	wx__DECLARE_EVT1(VAL_EVT_PTS_SUIVIS, id, GstEvtPointSuivis(func))

BEGIN_EVENT_TABLE(FenetrePrincipale, wxFrame)
    EVT_IDLE(FenetrePrincipale::OnIdle)
// EVENEMENT MENU
	EVT_MENU(OUVRIR_FICHIER_HORODATAGE,  FenetrePrincipale::OnOuvrirHorodatage)
	EVT_MENU(QUITTER_,  FenetrePrincipale::Quitter)
	EVT_MENU(REEL_ACQ,  FenetrePrincipale::ModeCamera)
	EVT_MENU(LANCER_ACQ,  FenetrePrincipale::DebutAcquisition)
	EVT_MENU(ARRET_ACQ,  FenetrePrincipale::FinAcquisition)
	EVT_MENU(IMAGE_CAMERA, FenetrePrincipale::ImageCamera)
	EVT_MENU(NOIRETBLANC_,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+1,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+2,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+3,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+4,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+5,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+6,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+7,  FenetrePrincipale::SelectPalette)
	EVT_MENU(NOIRETBLANC_+8,  FenetrePrincipale::SelectPalette)
	EVT_MENU(ARCENCIEL_,  FenetrePrincipale::SelectPalette)
	EVT_MENU(ZOOM1SUR2,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM1SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM2SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM4SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM8SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(DEFINITION_FOND_QUADRIQUE,  FenetrePrincipale::DefinitionFondQuadrique)
	EVT_MENU(Menu_Rectangle,  FenetrePrincipale::MAJRectangle)
	EVT_MENU(Menu_Coupe,  FenetrePrincipale::MAJCoupe)
	EVT_MENU(Menu_FilMax,  FenetrePrincipale::MAJFiltreMax)
	EVT_MENU(Menu_ParAlg,  FenetrePrincipale::ParamAlgo)
	EVT_MENU(MENU_OP1,  FenetrePrincipale::PrepOperation)
	EVT_MENU(MENU_OP2,  FenetrePrincipale::PrepOperation)
	EVT_MENU(MENU_EXEC_OP,  FenetrePrincipale::PrepOperation)
	EVT_MENU(RESET_OP,  FenetrePrincipale::PrepOperation)
	EVT_MENU(ENREGISTRER_FICHIER,FenetrePrincipale::Enregistrer)
	EVT_MENU(ENREGISTRERSOUS_FICHIER,FenetrePrincipale::EnregistrerSous)
	EVT_MENU(CREER_RAPPORT,FenetrePrincipale::CreerRapport)
 //   EVT_CONTEXT_MENU(FenetrePrincipale::OnMenuContext)
// EVENEMENT CLAVIER
    EVT_KEY_DOWN(FenetrePrincipale::OnKeyDown)
    EVT_KEY_UP(FenetrePrincipale::OnKeyUp)

// EVENEMENT SOURIS
// EVENEMENT FENETRE
    EVT_CLOSE(FenetrePrincipale::OnClose)
//	EVT_PAINT(FenetrePrincipale::OnPaint)
	EVT_ACTIVATE(FenetrePrincipale::OnActivate)
	EVT_SIZE(FenetrePrincipale::OnSize)
//    EVT_ERASE_BACKGROUND(FenetrePrincipale::OnEraseBackground)
// EVEMENT TIMER
	EVT_TIMER(1, FenetrePrincipale::OnTimer)
	EVT_TIMER(2, FenetrePrincipale::DetectionUtilisateur)
// EVENEMENT THREAD
   EVT_PTS_SUIVIS(wxID_ANY,FenetrePrincipale::OnThreadUpdateQueue)

// EVENEMENT RESEAU

END_EVENT_TABLE()



using namespace std;



int wxOsgApp::FilterEvent(wxEvent& event)
{
    if ( event.GetEventType()==wxEVT_KEY_DOWN)
		if (((wxKeyEvent&)event).GetKeyCode()==WXK_F1 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F2 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F3 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F4 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F5 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F6 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F7 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F8 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F9 ||
			((wxKeyEvent&)event).GetKeyCode()==WXK_F10 )
			{
				if (indFenetre>=0)
					listeFenetre[indFenetre]->fPrin->OnKeyDown( (wxKeyEvent&)event );
				return true;
			}

    return -1;
}

void	wxOsgApp::DefPointeurSouris(int mode,int type)
{
modeSouris=mode;
indPointeurSouris=type;
map<int ,EnvFenetre *>::iterator pp;
for (pp=listeFenetre.begin();pp!=listeFenetre.end();pp++)
	{
	if (pp->second && pp->second->fPrin)
		pp->second->fPrin->DefPointeurSouris(type);
	}
}


void	FenetrePrincipale::DefPointeurSouris(int type)
{
switch(type){
case 0:
	SetCursor (wxStockCursor (wxCURSOR_ARROW ));
	break;
case 1:
	SetCursor (wxStockCursor (wxCURSOR_HAND ));
	break;
	}
}


FenetrePrincipale	*wxOsgApp::Graphique(int id)
{
if (id==-1)
	if (nbFenetre>0 && indFenetre>=0)
		return listeFenetre[indFenetre]->fPrin;
	else
		return NULL;
if (listeFenetre[id])
	return listeFenetre[id]->fPrin;
else
	return NULL;
}

int wxOsgApp::RechercheFenetre(ImageInfoCV *im)
{
map<int ,EnvFenetre *>::iterator pp;
for (pp=listeFenetre.begin();pp!=listeFenetre.end();pp++)
	{
	if (pp->second && pp->second->fPrin->ImAcq()==im)
		return pp->first;
	}
return -1;
}


void wxOsgApp::Quitter(wxFrame *g)
{
quitter=true;
if (ctrlCamera)
	{
//	ctrlCamera->FinAcquisition();
//	ctrlCamera->Destroy();
	ctrlCamera->Destroy();
	ctrlCamera=NULL;
	}

map<int ,EnvFenetre *>::iterator pp;
for (pp=listeFenetre.begin();pp!=listeFenetre.end();pp++)
	if (pp->second)
		{
		wxCloseEvent evt;
		pp->second->fZoom->Close(true);
		pp->second->fStat->Close(true);
		pp->second->fPrin->Close(true);
		pp->second->fZoom->Destroy();
		pp->second->fStat->Destroy();
		pp->second->fPrin->Destroy();
		}
for (int i = 0; i < listeFenetre.size(); i++)
	delete listeFenetre[i];
delete serveur;
delete dllplplot;
delete dllWXplplotdrv;
delete dllSVGplplotdrv;
outils=NULL;
ctrlCamera=NULL;
ImageInfoCV i;
i.EffaceOp();
delete configApp;
/*ImageInfo *imAcq = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
delete imAcq;*/
/*Eve *c=new Eve;
delete c;*/
};

void wxOsgApp::Video(wxCommandEvent &w,int type)
{
static int nbFenVideo=0;
wxString s("Video");
s.Printf("Video %d",nbFenVideo++);
FenetrePrincipale *f = new FenetrePrincipale(NULL, s,
    wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
f->OuvrirVideo(type);
if (!f->Cam()->Connectee())
	{
	delete f;
	return;
	}
FenetreZoom *fenZoom = new FenetreZoom(f);
f->DefOSGApp(this);
//f->RepertoireDoc("/");
//f->NomDoc("video");
f->SetLabel(s);
ImageStatistiques *imgStatIm = new ImageStatistiques(NULL, "Image Statistic",
	wxPoint(530,0), wxSize(430,570),
	wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
imgStatIm->SetLabel("Stat : "+s);
imgStatIm->DefFenMere(f);
imgStatIm->OuvertureOngletStatus();
imgStatIm->DefOSGApp(this);
f->DefZoom(fenZoom);
f->DefImgStat(imgStatIm);
//if (dllplplot->IsLoaded() && dllWXplplotdrv->IsLoaded())
	{
	imgStatIm->OuvertureOngletHistogramme();
	imgStatIm->OuvertureOngletCoupe();
	imgStatIm->OuvertureOngletDistribRadiale();
	imgStatIm->OuvertureOngletDistribAngulaire();
	imgStatIm->OuvertureOngletFocus();
	}
imgStatIm->OuvertureOngletCouleur();
imgStatIm->OuvertureOngletPalette();
imgStatIm->OuvertureOngletRegion();
imgStatIm->OuvertureOngletCurseur();
OuvertureOutils();
listeFenetre[nbFenetre]=new EnvFenetre(f,fenZoom,imgStatIm);
indFenetre=nbFenetre;
f->DefId(nbFenetre);
f->Show(true);

f->NouvelleImage();
f->MAJNouvelleImage();

f->RecupDerniereConfig();
nbFenetre++;
f->InitIHM();
wxImage image;

for (int i = 1; i < 4; i++)
    {
    /* For some reason under wxX11, the 2nd LoadFile in this loop fails, with
       a BadMatch inside CreateFromImage (inside ConvertToBitmap). This happens even if you copy
       the first file over the second file. */
        if (image.LoadFile(wxString::Format("shape0%d.png",  i), wxBITMAP_TYPE_PNG))
        {
            DragShape* newShape = new DragShape(wxBitmap(image));
            newShape->SetPosition(wxPoint(i*50, i*50));

            if (i == 2)
                newShape->SetDragMethod(SHAPE_DRAG_TEXT);
            else if (i == 3)
                newShape->SetDragMethod(SHAPE_DRAG_ICON);
            else
                newShape->SetDragMethod(SHAPE_DRAG_BITMAP);
             f->GetDisplayList().Append(newShape);
       }
    }

f->ModeCamera(w);
}




void wxOsgApp::Ouvrir(wxCommandEvent &w)
{
wxString dossier;
configApp->Read("/dossier",&dossier,wxEmptyString);

wxFileDialog ouverture(NULL, "Ouvrir ", dossier, wxEmptyString, "*.tif;*.jpg;*.bmp;*.ymlgz");
if (ouverture.ShowModal()!=wxID_OK)
	return;
FenetrePrincipale *f = new FenetrePrincipale(NULL, "wxOpenCV",
    wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);

FenetreZoom *fenZoom = new FenetreZoom(f);
f->DefOSGApp(this);
configApp->Write("/dossier",ouverture.GetDirectory());
#ifdef __WINDOWS__
wxString s(ouverture.GetDirectory()+"\\"+ouverture.GetFilename ());
#else
wxString s(ouverture.GetDirectory()+"/"+ouverture.GetFilename ());
#endif
wxString sd(ouverture.GetDirectory());
f->RepertoireDoc(sd);
sd=ouverture.GetFilename ();
f->NomDoc(sd);
f->SetLabel(s);
f->OnOuvrir(s);
ImageStatistiques *imgStatIm = new ImageStatistiques(NULL, "Image Statistic",
	wxPoint(530,0), wxSize(430,570),
	wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
imgStatIm->SetLabel("Stat : "+s);
imgStatIm->DefFenMere(f);
imgStatIm->OuvertureOngletStatus();
imgStatIm->DefOSGApp(this);
f->DefZoom(fenZoom);
f->DefImgStat(imgStatIm);
//if (dllplplot->IsLoaded() && dllWXplplotdrv->IsLoaded())
	{
	imgStatIm->OuvertureOngletHistogramme();
	imgStatIm->OuvertureOngletCoupe();
	imgStatIm->OuvertureOngletDistribRadiale();
	imgStatIm->OuvertureOngletDistribAngulaire();
	imgStatIm->OuvertureOngletFocus();
	}
imgStatIm->OuvertureOngletCouleur();
imgStatIm->OuvertureOngletPalette();
imgStatIm->OuvertureOngletRegion();
imgStatIm->OuvertureOngletCurseur();
listeFenetre[nbFenetre]=new EnvFenetre(f,fenZoom,imgStatIm);
indFenetre=nbFenetre;
f->DefId(nbFenetre);
f->Show(true);

f->NouvelleImage();
f->MAJNouvelleImage();
f->RecupDerniereConfig();
nbFenetre++;
f->InitIHM();
wxImage image;

for (int i = 1; i < 4; i++)
    {
    /* For some reason under wxX11, the 2nd LoadFile in this loop fails, with
       a BadMatch inside CreateFromImage (inside ConvertToBitmap). This happens even if you copy
       the first file over the second file. */
        if (image.LoadFile(wxString::Format("shape0%d.png",  i), wxBITMAP_TYPE_PNG))
        {
            DragShape* newShape = new DragShape(wxBitmap(image));
            newShape->SetPosition(wxPoint(i*50, i*50));

            if (i == 2)
                newShape->SetDragMethod(SHAPE_DRAG_TEXT);
            else if (i == 3)
                newShape->SetDragMethod(SHAPE_DRAG_ICON);
            else
                newShape->SetDragMethod(SHAPE_DRAG_BITMAP);
             f->GetDisplayList().Append(newShape);
       }
    }

}

void wxOsgApp::DefOperateurImage(wxString &s)
{
pOCV.doubleParam.clear();
pOCV.intParam.clear();
pOCV.sizeParam.clear();
pOCV.pointParam.clear();

if (s.Cmp("SeparationPlan")==0)
	{
	pOCV.nomOperation=s;
	opSurjecUnaire = &ImageInfoCV::SeparationPlan;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#split";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	}
if (s.Cmp("FusionPlan")==0)
	{
	opUnaireSelec = &ImageInfoCV::FusionPlan;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#merge";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=149&zoom=70,250,100";
	}
if (s.Cmp("Addition")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Add;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#add";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	}
if (s.Cmp("Soustraction")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Sub;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#subtract";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=172&zoom=70,250,100";
	}
if (s.Cmp("Multiplication")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Mul;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#multiply";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=153&zoom=70,250,100";
	}
if (s.Cmp("Division")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Div;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#divide";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=136&zoom=70,250,100";
	}
if (s.Cmp("Convolution")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Convolution;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	}
if (s.Cmp("Dilatation")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Dilatation;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	}
if (s.Cmp("Erosion")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Erosion;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	}
if (s.Cmp("Ouverture")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Ouverture;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=morphologyex#morphologyex";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s.Cmp("Fermeture")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Fermeture;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s.Cmp("ChapeauHaut")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::ChapeauHaut;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s.Cmp("ChapeauBas")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::ChapeauBas;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s.Cmp("GradMorph")==0)
	{
	pOCV.nomOperation=s;
	opBinaireSelec = &ImageInfoCV::GradMorph;
	nomOperation = s;
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s.Cmp("Laplacien")==0)
	{
	pOCV.intParam["ddepth"]=DomaineParametre<int>(50.,0.0,255.0,1.0);
	pOCV.doubleParam["scale"]=DomaineParametre<double>(1,0.0,255.0,1.0);
	pOCV.doubleParam["delta"]=DomaineParametre<double>(0,0.0,255.0,1.0);
	pOCV.intParam["ksize"]=DomaineParametre<int>(3,1,7,2);
	pOCV.intParam["borderType"]=DomaineParametre<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pOCV.lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html#laplacian";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=261&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Laplacien;
	nomOperation = s;
	}
if (s.Cmp("Canny")==0)
	{
	pOCV.doubleParam["threshold1"]=DomaineParametre<double>(50.,0.0,255.0,1.0);
	pOCV.doubleParam["threshold2"]=DomaineParametre<double>(100,0.0,255.0,1.0);
	pOCV.intParam["aperture_size"]=DomaineParametre<int>((int)3,(int)1,(int)255,(int)2);
	pOCV.intParam["kernel_size"]=DomaineParametre<int>(3,1,255,2);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=331&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Canny;
	nomOperation = s;
	}
if (s.Cmp("Contour")==0)
	{
	pOCV.intParam["mode"]=DomaineParametre<int>(cv::RETR_EXTERNAL,cv::RETR_EXTERNAL,cv::RETR_TREE,1);
	pOCV.intParam["method"]=DomaineParametre<int>(cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_TC89_L1 ,1);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#findContour";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=308&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Contour;
	nomOperation = s;
	}
if (s.Cmp("FFT")==0)
	{
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::FFT;
	nomOperation = s;
	}
if (s.Cmp("IFFT")==0)
	{
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	pOCV.refPDF="The OpenCV Reference Manual, Release 2.4.9.0 p325";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::IFFT;
	nomOperation = s;
	}
if (s.Cmp("Seuillage")==0)
	{
	pOCV.doubleParam["thresh"]=DomaineParametre<double>(50.,0.0,255.0,1.0);
	pOCV.doubleParam["maxval"]=DomaineParametre<double>(255.,0.0,255.0,1.0);
	pOCV.intParam["threshold_type"]=DomaineParametre<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#threshold";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=291&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Seuillage;
	nomOperation = s;
	}
if (s.Cmp("AdaptatifSeuillage")==0)
	{
	pOCV.doubleParam["maxValue"]=DomaineParametre<double>(255.,0.0,255.0,1.0);
	pOCV.intParam["thresholdType"]=DomaineParametre<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	pOCV.intParam["adaptiveMethod"]=DomaineParametre<int>(cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_MEAN_C,1);
	pOCV.intParam["blockSize"]=DomaineParametre<int>(21,3,1000,2);
	pOCV.doubleParam["C"]=DomaineParametre<double>(-0,-255,255,1);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#adaptivethreshold";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=280&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::SeuillageAdaptatif;
	nomOperation = s;
	}
if (s.Cmp("LissageMedian")==0)
	{
	pOCV.intParam["ksize"]=DomaineParametre<int>(3,1,255,2);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=medianblur#medianblur";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageMedian;
	nomOperation = s;
	}
if (s.Cmp("LissageMoyenneur")==0)
	{
	pOCV.sizeParam["ksize"]=DomaineParametre<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	pOCV.pointParam["anchor"]=DomaineParametre<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	pOCV.intParam["borderType"]=DomaineParametre<int>(cv::BORDER_DEFAULT,0,255,1);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=blur#blur";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageMoyenne;
	nomOperation = s;
	}
if (s.Cmp("LissageGaussien")==0)
	{
	pOCV.sizeParam["ksize"]=DomaineParametre<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	pOCV.doubleParam["sigmaX"]=DomaineParametre<double>(0.1,0,255.0,0.1);
	pOCV.doubleParam["sigmaY"]=DomaineParametre<double>(0.1,0,255.0,0.1);
	pOCV.intParam["borderType"]=DomaineParametre<int>(cv::BORDER_DEFAULT,0,255,1);
	pOCV.lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=gaussianblur#gaussianblur";
	pOCV.refPDF="http://docs.opencv.org/opencv2refman.pdf#page=263&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageGaussien;
	nomOperation = s;
	}
if (s.Cmp("ComposanteConnexe")==0)
	{
	pOCV.intParam["connectivity"]=DomaineParametre<int>(4,4,8,4);
	pOCV.intParam["ltype"]=DomaineParametre<int>(CV_32S,CV_32S,CV_32S,0);
	pOCV.lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#connectedcomponents";
	pOCV.refPDF="http://docs.opencv.org/opencv3refman.pdf#page=263&zoom=70,250,100";
	pOCV.nomOperation=s;
	opUnaireSelec = &ImageInfoCV::ComposanteConnexe;
	nomOperation = s;
	}


}


ImageInfoCV	**wxOsgApp::ExecuterOperation(Parametre *pOCVNouveau)
{
ImageInfoCV	*im=NULL;
ImageInfoCV	**imTab=NULL;
if (pOCVNouveau==NULL)
	pOCV.nbImageRes=1;
else
	pOCVNouveau->nbImageRes=1;
if (opBinaireSelec)
	{
	
	try
		{
		if (op2==NULL)
			im =((*op1).*opBinaireSelec)(op1,NULL,NULL);
		else
			im =((*op1).*opBinaireSelec)(op1,op2,NULL);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox("An error occured in binary operator :"+s);
		}
	DefPointeurSouris(0,0);

	if (im==NULL)
		{
		opBinaireSelec=NULL;
		op1=NULL;
		op2=NULL;
		return imTab;
		}
	else
		{
		imTab = new ImageInfoCV*[1];
		imTab[0]=im;
		}
	}
if (opUnaireSelec)
	{
	try
		{

		if (pOCVNouveau==NULL)
			im =((*op1).*opUnaireSelec)(op1,pOCV);
		else
			im =((*op1).*opUnaireSelec)(op1,*pOCVNouveau);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox("An error occured in unary operator :"+s);
		}
	DefPointeurSouris(0,0);

	if (im==NULL)
		{
		opUnaireSelec=NULL;
		op1=NULL;
		return imTab;
		}
	else
		{
		imTab = new ImageInfoCV*[1];
		imTab[0]=im;
		}
	}

if (opSurjecUnaire)
	{
	try
		{

		if (pOCVNouveau==NULL)
			imTab =((*op1).*opSurjecUnaire)(op1,pOCV);
		else
			imTab =((*op1).*opSurjecUnaire)(op1,*pOCVNouveau);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox("An error occured in surjection operator :"+s);
		}
	DefPointeurSouris(0,0);

	if (imTab==NULL)
		{
		opUnaireSelec=NULL;
		op1=NULL;
		return imTab;
		}
	}

return imTab; // Le pointeur imTab n'est pas libéré
}


void wxOsgApp::CreerFenetreOperation()
{
if ((opBinaireSelec==NULL && opUnaireSelec==NULL && opSurjecMultiple==NULL) || op1==NULL)
	return;

ImageInfoCV	**im=ExecuterOperation();
if (im==NULL)
	{
	wxLogWarning("Resultat de l'opération vide");
	wxMessageBox("Resultat de l'opération vide");
	return;
	}
for (int nbres=0;nbres<pOCV.nbImageRes;nbres++)
	{
	FenetrePrincipale *f = new FenetrePrincipale(NULL, nomOperation,
		wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
	FenetreZoom *fenZoom = new FenetreZoom(f);
	f->DefOSGApp(this);
	wxString s;
	int ind=-1;
	if (nomOperation.Cmp("Convolution")==0)
		{
		ind = im[nbres]->IndOpConvolution();
		pOCV.doubleParam["IndOpConvolution"].valeur=ind;
		}
	if (nomOperation.Cmp("Erosion")==0)
		{
		ind = im[nbres]->IndOpMorphologie();
		pOCV.doubleParam["IndOpErosion"].valeur=ind;
		}
	if (nomOperation.Cmp("Dilatation")==0)
		{
		ind = im[nbres]->IndOpMorphologie();
		pOCV.doubleParam["IndOpDilatation"].valeur=ind;
		}
	if (opUnaireSelec)
		f->DefHistorique(indOp1Fenetre,-1,nomOperation,&pOCV);
	if (opBinaireSelec)
		f->DefHistorique(indOp1Fenetre,indOp2Fenetre,nomOperation,NULL);
	if (ind!=-1)
		s.Printf("%d : %s(operator %d) of image %d ",nbFenetre,nomOperation,ind,indOp1Fenetre );
	else
		s.Printf("%d : %s of image %d ",nbFenetre,nomOperation,indOp1Fenetre );
	f->SetLabel(s);


	f->AssosierImage(im[nbres]);
	f->Bind(wxEVT_LEAVE_WINDOW, &FenetrePrincipale::SourisQuitterFen, f);

	ImageStatistiques *imgStatIm = new ImageStatistiques(NULL, _("Image Statistic"),
		wxPoint(530,0), wxSize(430,570),
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
	listeFenetre[nbFenetre]=new EnvFenetre(f,fenZoom,imgStatIm);
	indFenetre=nbFenetre;
	f->DefId(nbFenetre);
	imgStatIm->SetLabel("Stat : "+s);
	imgStatIm->DefFenMere(f);
	imgStatIm->OuvertureOngletStatus();
	imgStatIm->DefOSGApp(this);
	f->DefZoom(fenZoom);
	f->DefImgStat(imgStatIm);
	if (dllplplot->IsLoaded() && dllSVGplplotdrv->IsLoaded())
		{
		imgStatIm->OuvertureOngletHistogramme();
		imgStatIm->OuvertureOngletCoupe();
		imgStatIm->OuvertureOngletDistribRadiale();
		imgStatIm->OuvertureOngletDistribAngulaire();
		imgStatIm->OuvertureOngletFocus();
		}
	imgStatIm->OuvertureOngletCouleur();
	imgStatIm->OuvertureOngletPalette();
	imgStatIm->OuvertureOngletRegion();
	imgStatIm->ListerRegion();
	imgStatIm->OuvertureOngletCurseur();
	f->DefHistorique();
	f->Show(true);

	f->NouvelleImage();
	f->MAJNouvelleImage();

	f->RecupDerniereConfig();
	nbFenetre++;
	f->InitIHM();
	}
opBinaireSelec=NULL;
opUnaireSelec=NULL;
opSurjecUnaire=NULL;
op1=NULL;
op2=NULL;
indOp1Fenetre=-1;
indOp2Fenetre=-1;
pOCV.doubleParam.clear();
pOCV.intParam.clear();
}


void wxOsgApp::Erosion(wxCommandEvent &w)
{
}



// `Main program' equivalent, creating windows and returning main app frame
bool wxOsgApp::OnInit()
{
	osg::Image *im=osgDB::readImageFile("F:\\Lib\\OpenSceneGraph-Data-3.0.0\\Images\\reflect.rgb");

	quitter=false;
	serveur = NULL;
    configApp=new wxFileConfig("wxOpenCV","LB","wxOpenCV.ini",wxEmptyString);
	LectureFichierConfig();
#ifndef __MULTILANGUE__
	langue=(wxLanguage)configApp->Read("/langue",langue);
//gestion du langage
    if ( langue == wxLANGUAGE_UNKNOWN )
    {
        int lng = wxGetSingleChoiceIndex
                  (
                    _("Please choose language:"),
                    _("Language"),
                    WXSIZEOF(langNames),
                    langNames
                  );
        langue = lng == -1 ? wxLANGUAGE_DEFAULT : langIds[lng];
    }

    // don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
    // false just because it failed to load wxstd catalog
    if ( !locale.Init(langue, wxLOCALE_DONT_LOAD_DEFAULT) )
    {
        wxLogWarning(_("This language is not supported by the system."));

        // continue nevertheless
    }

    // normally this wouldn't be necessary as the catalog files would be found
    // in the default locations, but when the program is not installed the
    // catalogs are in the build directory where we wouldn't find them by
    // default
    wxLocale::AddCatalogLookupPathPrefix("../lang");

    // Initialize the catalogs we'll be using
    const wxLanguageInfo* pInfo = wxLocale::GetLanguageInfo(langue);
    if (!locale.AddCatalog("main"))
    {
        wxLogError(_("Couldn't find/load the 'main' catalog for locale '%s'."),
                   pInfo ? pInfo->GetLocaleName() : _("unknown"));
    }
    if (!locale.AddCatalog("wxIUA"))
    {
        wxLogError(_("Couldn't find/load the 'main' catalog for locale '%s'."),
                   pInfo ? pInfo->GetLocaleName() : _("unknown"));
    }
    // Now try to add wxstd.mo so that loading "NOTEXIST.ING" file will produce
    // a localized error message:
    locale.AddCatalog("wxstd");
        // NOTE: it's not an error if we couldn't find it!

    // this catalog is installed in standard location on Linux systems and
    // shows that you may make use of the standard message catalogs as well
    //
    // if it's not installed on your system, it is just silently ignored
#ifdef __LINUX__
    {
        wxLogNull noLog;
        locale.AddCatalog("fileutils");
    }
#endif
#endif
configApp->Write("/langue",(long)langue);
configApp->Flush();


wxInitAllImageHandlers();

wxString	cheminPlplot(CHEMIN_PLPLOT_WX);
wxString	cheminPlplotDrv(CHEMIN_PLPLOT_DRV);
wxString	plPlotLibWX(cheminPlplot+"/plplotwxwidgetsd");
wxString	svgPlPlotDrv(cheminPlplotDrv+"/svg");
wxString	wxPlPlotDrv(cheminPlplotDrv+"/wxwidgets");


#ifndef ___AUI__




dllplplot=new wxDynamicLibrary(plPlotLibWX);
dllWXplplotdrv=new wxDynamicLibrary(wxPlPlotDrv);
dllSVGplplotdrv=new wxDynamicLibrary(svgPlPlotDrv);
InterfaceAvance *frame = new InterfaceAvance(NULL,
                                 wxID_ANY,
                                 "wxOpenCV",
                                 wxDefaultPosition,
                                 wxSize(800, 600));

wxBitmap bitmap;
bool  m_isPda = (wxSystemSettings::GetScreenType() <= wxSYS_SCREEN_PDA);
if (m_isPda)
    bitmap = wxBitmap(mobile_xpm);

bool ok = m_isPda
        ? bitmap.IsOk()
        : bitmap.LoadFile(wxT("splash.png"), wxBITMAP_TYPE_PNG);

if (ok)
{
    new wxSplashScreen(bitmap,
        wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
        6000, frame, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSIMPLE_BORDER|wxSTAY_ON_TOP);
}

#if !defined(__WXGTK20__)
    // we don't need it at least on wxGTK with GTK+ 2.12.9
    wxYield();
#endif



frame->DefOSGApp(this);
   frame->Show();
#else
dllplplot=NULL;
dllWXplplotdrv=NULL;
dllSVGplplotdrv=NULL;

class FenetreTest : public wxFrame
{
public :
wxOsgApp *osgApp;
FenetreTest(wxFrame *frame, const wxString& title, const wxPoint& pos, 
    const wxSize& size, long style = wxDEFAULT_FRAME_STYLE): wxFrame(frame, wxID_ANY, title , pos, size, style)
{
    EVT_CLOSE(InterfaceAvance::OnClose)
Bind(wxEVT_CLOSE_WINDOW, &FenetreTest::OnClose,this);

};
void FenetreTest::OnClose(wxCloseEvent& event)
{
wxFrame::OnCloseWindow(event);
((wxOsgApp*)osgApp)->Quitter(NULL);
}


};


FenetreTest *frame=new FenetreTest(NULL,"TEST",wxDefaultPosition,wxSize(800, 600));
    frame->Show();
	frame->osgApp = this;
#endif

camAndor=0;
CameraOpenCV *cam=new CameraOpenCV();
if (cam->TestDriver())
	{
	camOpenCV=1;
	wxLogWarning(_("OpenCV compatible camera detected (%dX%d : %d)"),cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
	}
else
	{
	camOpenCV=0;
	wxLogWarning(_("OpenCV compatible camera not detected"));
	}

delete cam;

if (!dllplplot || !dllplplot->IsLoaded())
	wxLogWarning(_("unable to load plplot"));
if (!dllplplot || !dllWXplplotdrv->IsLoaded())
	wxLogWarning(_("unable to load wxwidgets"));
if (!dllplplot || !dllSVGplplotdrv->IsLoaded())
	wxLogWarning(_("unable to load svg"));

nbFenetre=0;
indFenetre=-1;
modeSouris = SOURIS_STD;
indPointeurSouris=0;
op1=NULL;
op2=NULL;
indOp1Fenetre=-1;
indOp2Fenetre=-1;
opBinaireSelec=NULL;
opUnaireSelec=NULL;
opSurjecUnaire=NULL;
ctrlCamera=NULL;
utilisateurAbsent=0;
#ifndef ___AUI__
if (serveur)
	{
	serveur = new ServeurScilab;
	wxString nomServeur=_T("4242");
	if (serveur->Create(nomServeur))
		{
		wxLogWarning("Server %s started", nomServeur);
	#if wxUSE_DDE_FOR_IPC
		wxLogWarning("Server uses DDE");
	#else // !wxUSE_DDE_FOR_IPC
		wxLogWarning("Server uses TCP");
	#endif // wxUSE_DDE_FOR_IPC/!wxUSE_DDE_FOR_IPC
		}
	else
		{
		wxLogWarning(_("Server %s failed to start"), nomServeur);
		delete serveur;
		serveur = NULL;
		}
	serveur->DefFentrePrincipale(this);
	}
#else
serveur =NULL;
#endif
return true;
}

void wxOsgApp::AnnuleOp()
{opBinaireSelec=NULL;opUnaireSelec=NULL;op1=NULL;op2=NULL;};


void wxOsgApp::OuvertureOutils()
{
if (ctrlCamera==NULL)
	{
	ctrlCamera = new ControleCamera(NULL, _("Camera Control"),
		wxPoint(600,570), wxSize(360,250),
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN	);


	ctrlCamera->DefOSGApp(this);
	}
wxString userName=wxGetUserName();
/*
if (!outils)
	{
	outils = new OutilsImage (NULL, wxT("Image Tools"),
		wxPoint(0,570), wxSize(600,250),
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
	outils->DefOSGApp(this);
	outils->OuvertureOngletCouleur();
	//if (userName.Cmp(_T("laurent"))==0)
		{
		outils->OuvertureOngletTransparence();
		outils->OuvertureOngletQuadrique();
		outils->OuvertureOngletQuadriqueMulti();
		}
	outils->OuvertureOngletFond();
	outils->OuvertureOngletMoyenne();
	}*/
if (ctrlCamera)
	ctrlCamera->Show(false);
//outils->Show(true);
}


void wxOsgApp::TracerZoom(wxPoint p)
{
	{
	if (!listeFenetre[indFenetre]->fPrin->ModeRectangle())
		listeFenetre[indFenetre]->fZoom->TracerZoom(p);
	else
		{
		wxRect r(*listeFenetre[indFenetre]->fPrin->RectangleSelec());
		listeFenetre[indFenetre]->fZoom->TracerZoom(r);
		}
	}
}


void wxOsgApp::RetirerListe(FenetrePrincipale *f)
{
delete listeFenetre[f->IdFenetre()]->fZoom;
delete listeFenetre[f->IdFenetre()]->fStat;
indFenetre=f->IdFenetre();
listeFenetre.erase(f->IdFenetre());
int i=0;
indFenetre=-1;
while (i<nbFenetre)
	{
	if (listeFenetre.find(i)!=listeFenetre.end() && listeFenetre.find(i)->first!=NULL)
		 {
		 indFenetre=i;
		 break;
		 }
	i++;
	}

}

void wxOsgApp::IdFenetreActive(int x)
{
if (listeFenetre.find(x)!=listeFenetre.end())
	{

	if (listeFenetre[x]->fPrin != NULL)
		indFenetre=x;
	}
}

void FenetrePrincipale::OnActivate(wxActivateEvent &w)
{
if (!w.GetActive())
	return;
if (osgApp)
	{
	osgApp->IdFenetreActive(idFenetre);
	if (cam && cam->IsRunning() && osgApp->CtrlCamera())
		{
		wxString s= "Control :" +GetTitle();
		osgApp->CtrlCamera()->SetTitle(s);
		osgApp->CtrlCamera()->DefCamera(cam);
		osgApp->CtrlCamera()->Show(true);
		osgApp->CtrlCamera()->DefParent(this);
		}
	}
}


void FenetrePrincipale::DefCurseur(int r,int xc,int yc)
{
   static char down_bits[128];

    static char down_mask[128] = { 240, 1, 0, 0, 240, 1,
        0, 0, 240, 1, 0, 0, 240, 1, 0, 0, 240, 1, 0, 0, 240, 1,
        0, 0, 240, 1, 0, 0, 240, 1, 0, 0, 255, 31, 0, 0, 255,
        31, 0, 0, 254, 15, 0, 0, 252, 7, 0, 0, 248, 3, 0, 0,
        240, 1, 0, 0, 224, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0 };
	for (int i=0;i<128;i++)
		{
		down_bits[i]=0;
		down_mask[i]=0;
		}

	for (int i=0;i<32;i++)
		for (int j=0;j<32;j++)
			if (i-1==j || i+1==j || 30-i==j || 32-i==j)
				{
				int d=(j-xc)*(j-xc)+(i-yc)*(i-yc);
				if (d >=r*r && d<= r*r+64)
					{

					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					down_bits[j*4+octet]=down_bits[j*4+octet]|val;
					//down_mask[j*4+octet]=down_mask[j*4+octet]|val;
					}
				else
					{

					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					down_mask[j*4+octet]=down_mask[j*4+octet]|val;
					}
				}
			else if (i-2==j || i+2==j || 29-i==j || 33-i==j)
				{
				int d=(j-xc)*(j-xc)+(i-yc)*(i-yc);
				if (d >r*r && d<= r*r+64)
					{

					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					val = ~val;
					down_bits[j*4+octet]=down_bits[j*4+octet]&val;
					//down_mask[j*4+octet]=down_mask[j*4+octet]|val;
					}
				else
					{


					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					down_mask[j*4+octet]=down_mask[j*4+octet]|val;
					}
				}
			else
				{
				int d=(j-xc)*(j-xc)+(i-yc)*(i-yc);
				if (d>=r*r && d<=r*r+4)
					{

					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					down_bits[j*4+octet]=down_bits[j*4+octet]&val;
					}
				else
					{
					int octet= i/8;
					int bit= (i-octet*8);
					int val=1;
					val = val << bit;
					down_mask[j*4+octet]=down_mask[j*4+octet]|val;
					}
				}

    #ifdef __WXMSW__
        wxBitmap down_bitmap(down_bits, 128, 128);
        wxBitmap down_mask_bitmap(down_mask, 128, 128);

        down_bitmap.SetMask(new wxMask(down_mask_bitmap));
        wxImage down_image = down_bitmap.ConvertToImage();
        down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, xc);
        down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, yc);
        wxCursor down_cursor = wxCursor(down_image);
    #elif defined(__WXGTK__) or defined(__WXMOTIF__)
        wxCursor down_cursor = wxCursor(down_bits, 32, 32, 6, 14,
                                        down_mask, wxWHITE, wxBLACK);
    #endif
    SetCursor( wxCursor(wxCURSOR_CROSS));
}

void FenetrePrincipale::RecupDerniereConfig()
{
return;
wxString	repUtilAppli(wxGetUserHome(wxGetUserName()));
repUtilAppli=repUtilAppli+_T("\\TraiMisc");

wxString	nomConfig("\\DernierConfig.txt");
nomConfig=repUtilAppli+nomConfig;
ifstream	gg((const char*)nomConfig.c_str());
if (!gg.is_open())
	return;
string	sd;
string	sh;

gg>>sd>>sh;
wxLogVerbose("last config %s %s",sd,sh);

float	tpsExposition,x,y;
float gainEMCCD;
int	modeGainEMCCD;
gg>>tpsExposition>>modeGainEMCCD>>gainEMCCD;
wxLogVerbose(_T("exposure Time %6.3f Mode Gain %d EMCCDGain %6.3f "),tpsExposition,modeGainEMCCD,gainEMCCD);
if (osgApp->CtrlCamera())
	{
	osgApp->CtrlCamera()->DefTempsExposition(tpsExposition);
	osgApp->CtrlCamera()->DefModeGainEMCCD(modeGainEMCCD);
	osgApp->CtrlCamera()->DefGainEMCCD(gainEMCCD);
	}
/*
gg>>modeMoyenne;
modeMoyenne=modeMoyenne-48;
*/
gg>>x>>y;
//wxLogVerbose(_T("Mean mode %d b0=%f b1=%f"),int(modeMoyenne),x,y);
float dMin=3000;
int pos=0;
/*
for (int i=0;i<NBFILTRE;i++)
	{
	float d=fabs(x-bbButter[i]);
	if (d<dMin)
		{
		dMin=d;
		pos=i;
		}
	}
*/
//if (modeMoyenne)
//	indFiltreMoyenne=pos;
gg>>correctionBiais;
correctionBiais=correctionBiais-48;
gg>>sd;
if (correctionBiais)
	{
	OuvrirBiais((char*)sd.c_str());
	wxLogVerbose(_T("Bias Image %s"),sd);
	}
else
	wxLogVerbose(_T("No Bias Image "));
nomImageBiais=sd;
gg>>correctionTache;
correctionTache=correctionTache-48;
gg>>sd;
if (correctionTache)
	{
	OuvrirTache((char*)sd.c_str());
	fondDejaDefini=1;
	wxLogVerbose(_T("background Image %s"),sd);
	}
else
	wxLogVerbose(_T("No background Image"));
nomImageTache=sd;

gg>>correctionFonction;
correctionFonction=correctionFonction-48;
gg>>sd;
nomImageQuadrique=sd;
}

void FenetrePrincipale::InitIHM()
{
//osgApp->Outils()->OuvertureOngletCouleur(modeMoyenne,indFiltreMoyenne);
//osgApp->ImgStat()->MAJInfo(9,nomImageBiais);
//osgApp->ImgStat()->MAJInfo(11,nomImageTache);

}

string FenetrePrincipale::CommentaireConfig()
{
wxDateTime dd;
dd.SetToCurrent();
wxString	sd=dd.FormatISODate();
wxString	sh=dd.FormatISOTime();

ostrstream wgg;


wgg<<sd<<"\t"<<sh<<"\t";
if (osgApp->CtrlCamera())
	wgg<<osgApp->CtrlCamera()->TempsExposition()<<"\t"<<osgApp->CtrlCamera()->ModeGainEMCCD()<<"\t"<<osgApp->CtrlCamera()->GainEMCCD()<<"\t";
else
	wgg<<1.0<<"\t"<<1.0<<"\t"<<1.0<<"\t";
/*
if (modeMoyenne)
	wgg<<"1\t"<<bbButter[indFiltreMoyenne]<<"\t"<<aaButter[indFiltreMoyenne]<<"\t";
else
	wgg<<"0\t-1\t-1\t";
*/
if (correctionBiais)
	wgg<<"1\t"<<nomImageBiais<<"\t";
else
	wgg<<"0\tNULL\t";
if (correctionTache)
	wgg<<"1\t"<<nomImageTache<<"\t";
else
	wgg<<"0\tNULL\t";
if (correctionFonction)
	wgg<<"1\t"<<nomImageQuadrique<<"\t";
else
	wgg<<"0\tNULL";
wgg<<"\n";
string commentaire(wgg.str(),wgg.pcount());
return commentaire;
}


void FenetrePrincipale::SauveDerniereConfig()
{
wxString	repUtilAppli(wxGetUserHome(wxGetUserName()));
repUtilAppli=repUtilAppli+_T("\\TraiMisc");
if (!::wxDirExists(repUtilAppli))
	::wxMkdir(repUtilAppli);

wxString	nomConfig(_T("\\DernierConfig.txt"));
nomConfig=repUtilAppli+nomConfig;

ofstream	gg((const char *)nomConfig.c_str());
if (!gg.is_open())
	return;
string commentaire(CommentaireConfig());
gg<<commentaire.c_str();
imAcq->DefDescription((char*)commentaire.c_str());
}

ControleCamera		*FenetrePrincipale::CtrlCamera(void)
{
return osgApp->CtrlCamera();
}

void FenetrePrincipale::BasculeZoom()
{
	if (zoomActif)
		zoomActif=false;
	else
		zoomActif=true;
	fenZoom->Show(zoomActif);
	}

void FenetrePrincipale::BasculeStat()
{
	if (statActif)
		statActif=false;
	else
		{
		statActif=true;
		imgStatIm->Refresh(true);
		}

	imgStatIm->Show(statActif);
	}


char FenetrePrincipale::UtilisateurAbsent()
{
if (osgApp)
	return osgApp->UtilisateurAbsent();
return 0;
}

/* My frame constructor */
FenetrePrincipale::FenetrePrincipale(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title , pos, size, style/*|wxVSCROLL |wxHSCROLL*/)
{
origineImage.indOp1=-1;
origineImage.indOp2=-1;
origineImage.op1=NULL;
origineImage.op2=NULL;
tpsPreEvt=-1;


for (int i=0;i<10;i++)
	planActif[i]=true;
osgApp=NULL;
zoomActif=false;
statActif=false;
fenetreSauvee =0;
fenAlgo=NULL;
idFenetre=-1;

    m_dragMode = TEST_DRAG_NONE;
    m_draggedShape = (DragShape*) NULL;
//    m_dragImage = (wxDragImage*) NULL;
    m_currentlyHighlighted = (DragShape*) NULL;
	m_dragImage=NULL;



detectionUtilisateur =  new wxTimer(this,2);
repertoireDuDocument=".";
nomDuDocument="";
modeImage = 0;
modeFiltre =0;
fondDejaDefini=0;
modeTransparence=0;
cTransparence[0]=100;
cTransparence[1]=0;
cTransparence[2]=0;
cTransparence[3]=0;
modeCamera=false;
imageTraitee=true;
interdireAffichage=false;
typeAcqImage =0;
indFiltreMoyenne=0;
pCouleur=NULL;
pAleatoire=NULL;
pJet=NULL;
pRainbow=NULL;
pPerso=NULL;
pPersoInv=NULL;
horlogeSeq=NULL;
imAcq = NULL;
poly=NULL;
dateSeq=NULL;
correctionFonction=0;
correctionTache=0;
correctionBiais=0;
nbImageTache=0;
((wxFrame*)this)->SetBackgroundColour(*wxBLACK);
// Association barre des menus avec la trame

SetIcon(wxIcon(_T("seec64.bmp"),wxBITMAP_TYPE_ICO ));
DefCurseur(5);

cam=NULL;

tabRGB=NULL;
tabRGBTransparence=NULL;
imAcq=NULL;
imAffichee=NULL;
imAcq = NULL;
indPalette=0;
nbImageBiais=0;
nbImageTache=0;
nbImageFonction=0;
dImageBiais=0;
dImageTache=0;

seuilNivBas=NULL;
coeffCanal=NULL;
alphad=1;
alpham=1;
seuilModuleHaut = 7.5*7.5;
seuilModuleBas = 40;
seuilSurface = 1000;
diffHauteur= 40;
nbMarcheFit=0;

SetClientSize(size.x, size.y);
feuille=NULL;
CreateStatusBar(2);
wxStatusBar *statbarOld = GetStatusBar();
statbarOld->Hide();
barreEtat = new BarreInfo(this);
if (barreEtat)
	SetStatusBar(barreEtat);
wxSize tailleUtile;
GetClientSize(&tailleUtile.x, &tailleUtile.y);
GetStatusBar()->Show();
PositionStatusBar();
detectionUtilisateur->Start(TPSMISEENVEILLECOURBE,true);
}

void FenetrePrincipale::InitImageFenetre()
{
cam =NULL;
tabRGB=NULL;
imAcq=NULL;
imAffichee=NULL;

imAcq = new ImageInfoCV(1002,1004,CV_16UC1);
seuilNivBas=new double[imAcq->channels()];
coeffCanal=new double[imAcq->channels()];
for (int i=0;i<imAcq->channels();i++)
	{
	seuilNivBas[i]=0;
	coeffCanal[i]=1;
	}

//imAcq2 = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imAcqBrutFil = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imAcqBrutFilMax = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imAcqBrut1 = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imAcqBrut2 = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imTache = new ImageInfo(REEL_IMAGE,T_FLOAT_IMAGE,1002,1004);
//imRefTache = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);
//imQuadrique = new ImageInfo(REEL_IMAGE,T_FLOAT_IMAGE,1002,1004);
//nivBiais = new ImageInfo(ENTIER_IMAGE,T_SHORT_IMAGE,1002,1004);

//imAcq->DefTypeModeleFctSEEC(1);

//imAcqBrut1->DefTypeModeleFctSEEC(1);
//imAcqBrut2->DefTypeModeleFctSEEC(1);
/*
int nbPixels=imTache->LitNbLigne()*imTache->LitNbColonne();
float			*data=(float*)imTache->LitPlan(0);
unsigned short	*dataRef=(unsigned short*)imRefTache->LitPlan(0);
for (int i=0;i<nbPixels;i++,data++,dataRef++)
	{
	*data=1;
	*dataRef=0;
	}
data=(float*)imQuadrique->LitPlan(0);
for (int i=0;i<nbPixels;i++,data++)
	*data=1;
unsigned short	*ds=(unsigned short*)nivBiais->LitPlan(0);
for (int i=0;i<nbPixels;i++,ds++)
	*ds=10000;
*/
}

/**
Ouvrir un flux video
*/

void FenetrePrincipale::OuvrirVideo(int type)
{
cam = new  CameraOpenCV();
if (!cam->Connectee())
	return;
barreEtat->ActiveVideo();

fenetreSauvee=1;
//ImageInfoCV *imtmp=new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
wxString userName=wxGetUserName();

if (type==8)
	{
	imAcq = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
	cam->DefTypeAcq(CV_8UC3);
	}
if (type==32)
	{
	imAcq = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),CV_32FC3);
	cam->DefTypeAcq(CV_32FC3);
	}
feuille = new ZoneImage(this,wxSize(imAcq->cols/2, imAcq->rows/2));
feuille->DefFenetrePrincipale(this);
//feuille->SetClientSize(tailleUtile);
feuille->Show();



if (imAffichee)
	{
	tabRGB=NULL;
	delete imAffichee;
	}
imAffichee=NULL;

wxLogVerbose("Video opened");
seuilNivBas=new double[imAcq->channels()];
coeffCanal=new double[imAcq->channels()];
for (int i=0;i<imAcq->channels();i++)
	{
	seuilNivBas[i]=0;
	coeffCanal[i]=256;
	}
wxSize	tailleZoneImage;
tailleZoneImage=GetClientSize();

}


/**
Ouvrir l'image
*/
void FenetrePrincipale::OnOuvrir(wxString & s)
{
fenetreSauvee=1;
if (ModeCamera())
	{
	wxMessageBox(_T("Grabbing images stopped!"));
	wxLogWarning(_T("Grabbing images stopped!"));
	ModeCamera();
	}
wxFileName p(s);

wxCharBuffer ww=s.mb_str ();
char *nomFichier=ww.data() ;
if (s.Find("ymlgz")>=0)
	{
	try 
		{
		imAcq =new ImageInfoCV();
		cv::FileStorage fs(nomFichier, cv::FileStorage::READ);
		fs["Image"]>>*((cv::Mat*)imAcq);
		fs.release();
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox(_("An error occured while saving file :")+s);
		}
	}
else
	{
	imAcq=new ImageInfoCV(nomFichier);
/*
imAcq2 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imAcqBrutFil = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imAcqBrutFilMax = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imAcqBrut1 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imAcqBrut2 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imTache = new ImageInfoCV(imtmp->rows,imtmp->cols,CV_32FC(imtmp->channels()));
imRefTache = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
imQuadrique = new ImageInfoCV(imtmp->rows,imtmp->cols,CV_32FC(imtmp->channels()));
nivBiais = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
*/
	}
wxLogVerbose(_T("Image %s loaded"),nomFichier);
//	int nbLigne=imtmp->rows,nbColonne=imtmp->cols,nbPlan=imtmp->channels();
seuilNivBas=new double[imAcq->channels()];
coeffCanal=new double[imAcq->channels()];
for (int i=0;i<imAcq->channels();i++)
	{
	int z=imAcq->depth();
	if (z==2)
		imAcq->flags=(imAcq->flags&0xFFFFFFF8)|CV_16SC1;
	if(imAcq->depth()==CV_8S || imAcq->depth()==CV_8U)
		{
		seuilNivBas[i]=0;
		coeffCanal[i]=256;
		}
	else
		{
		seuilNivBas[i]=0;
		coeffCanal[i]=1;
		}
	}
feuille = new ZoneImage(this,wxSize(imAcq->cols/2, imAcq->rows/2));
feuille->DefFenetrePrincipale(this);
feuille->DefOSGApp(osgApp);
feuille->Show();
if (imAffichee)
	{
	tabRGB=NULL;
	delete imAffichee;
	}
imAffichee=NULL;
if (imAcq->type()==CV_16UC1 && imAcq->MaxIm()[0]<32767.)
	imAcq->flags=(imAcq->flags&0xFFFFFFF0)|CV_16SC1;
wxSize	tailleZoneImage;
tailleZoneImage=GetClientSize();

}

/**
Associer une image à une fenêtre
*/
void FenetrePrincipale::AssosierImage(ImageInfoCV *imCalc)
{
fenetreSauvee=0;
wxString userName=wxGetUserName();
if (imAcq==NULL)
	{
//	ImageInfoCV *imtmp=imCalc;
	imAcq = imCalc;
	/*
	imAcq2 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imAcqBrutFil = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imAcqBrutFilMax = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imAcqBrut1 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imAcqBrut2 = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imTache = new ImageInfoCV(imtmp->rows,imtmp->cols,CV_32FC(imtmp->channels()));
	imRefTache = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	imQuadrique = new ImageInfoCV(imtmp->rows,imtmp->cols,CV_32FC(imtmp->channels()));
	nivBiais = new ImageInfoCV(imtmp->rows,imtmp->cols,imtmp->type());
	*/
	if (imAffichee)
		{
		if (tabRGB)
			delete tabRGB;
		tabRGB=NULL;
		delete imAffichee;
		}
	imAffichee=NULL;

	seuilNivBas=new double[imAcq->channels()];
	coeffCanal=new double[imAcq->channels()];
	for (int i=0;i<imAcq->channels();i++)
		{
		int z=imAcq->depth();
		if (z==2)
			imAcq->flags=(imAcq->flags&0xFFFFFFF8)|CV_16SC1;
		if(imAcq->depth()==CV_8S || imAcq->depth()==CV_8U)
			{
			seuilNivBas[i]=0;
			coeffCanal[i]=256;
			}
		else
			{
			seuilNivBas[i]=0;
			coeffCanal[i]=1;
			}
		}

#ifdef __IMTMP__
	int nbLigne=imtmp->rows,nbColonne=imtmp->cols,nbPlan=imtmp->channels();
int x=imtmp->type();
int t[]={CV_8UC1,CV_8UC2,CV_8UC3,CV_16UC1,CV_16UC2,CV_16UC3,CV_16SC1,CV_16SC2,CV_16SC3,CV_32FC1,CV_32FC2,CV_32FC3,CV_32FC4,CV_64FC1,CV_64FC2,CV_64FC3};
	switch(imtmp->type())
	{
	case CV_8UC1:
		seuilNivBas[0]=0;
		coeffCanal[0]=1;
		for (int i=0;i<nbLigne;i++)
			for (int j=0;j<nbColonne;j++)
				{
				imAcq->at<unsigned char>(i,j) = imtmp->at<unsigned char>(i,j);
				}
		break;
	case CV_8UC3:
		for (int i=0;i<imAcq->channels();i++)
			{
			seuilNivBas[i]=0;
			coeffCanal[i]=1;
			}
		for (int k=0;k<nbPlan;k++)
			for (int i=0;i<nbLigne;i++)
				for (int j=0;j<nbColonne;j++)
					{
					imAcq->at<cv::Vec3b>(i,j) = imtmp->at<cv::Vec3b>(i,j);
					}
		break;
	case CV_16UC1:
		seuilNivBas[0]=0;
		coeffCanal[0]=0.25;
		for (int i=0;i<nbLigne;i++)
			for (int j=0;j<nbColonne;j++)
				{
				imAcq->at<unsigned short>(i,j) = imtmp->at<unsigned short>(i,j);
				}
		break;
	case CV_32FC1:
		for (int i=0;i<nbLigne;i++)
			for (int j=0;j<nbColonne;j++)
				{
				imAcq->at<float>(i,j) = imtmp->at<float>(i,j);
				}
		seuilNivBas[0]=0;
		coeffCanal[0]=1;
		break;
	case CV_32FC2:
		for (int i=0;i<nbLigne;i++)
			for (int j=0;j<nbColonne;j++)
				{
				imAcq->at<cv::Vec2f>(i,j) = imtmp->at<cv::Vec2f>(i,j);
				}
		seuilNivBas[0]=0;
		coeffCanal[0]=1;
		seuilNivBas[1]=0;
		coeffCanal[1]=1;
		break;
	case CV_32FC3:
		for (int i=0;i<nbLigne;i++)
			for (int j=0;j<nbColonne;j++)
				{
				imAcq->at<cv::Vec3f>(i,j) = imtmp->at<cv::Vec3f>(i,j);
				}
		for (int i=0;i<imAcq->channels();i++)
			{
			seuilNivBas[i]=0;
			coeffCanal[i]=1;
			}
		break;
		}
#endif
	}
else
	{
	delete imAcq;
	imAcq = imCalc;
	}
if (!feuille)
	{
	feuille = new ZoneImage(this,wxSize(imAcq->cols/2,imAcq->rows/2));
	feuille->DefFenetrePrincipale(this);
	feuille->DefOSGApp(osgApp);
	}
delete(feuille->BitmapAffichee());
feuille->BitmapAffichee(NULL);
feuille->Update();
}


/**
Ouvrir le fichier d'horodatage
*/
void FenetrePrincipale::OnOuvrirHorodatage(wxCommandEvent& event)
{
wxFileDialog ouverture(this, _("Open Sequence"), _("Seq"), _("Seq"), _T("*.csv"));
if (ouverture.ShowModal()!=wxID_OK)
	return;
string s(ouverture.GetFilename().char_str());
string s2(ouverture.GetDirectory().char_str());
s = "\\"+s;
s=s2+s;
if (dateSeq)
	delete dateSeq;
dateSeq=  new HorodatageSequence((char*)s.c_str());
if (dateSeq->Erreur())
	{
	wxString w;
	w.Printf(_T("Inconsistancy at line :%d"),dateSeq->Erreur());
	wxMessageBox(w,_T("File Format Error "));
	delete dateSeq;
	dateSeq=NULL;
	}
else if (::wxFileExists(dateSeq->NomFichierRapport()))
	{
	wxString w;
	w.Printf(_("File %s already exist. This timestamp file seems already used."),dateSeq->NomFichierRapport());
	wxMessageBox(w,_("Warning "));
	}

}


/**
Enregister l'image
*/
void FenetrePrincipale::Enregistrer(wxCommandEvent& evt)
{
if (imAcq==NULL)
	{
	wxMessageBox(_("Error Saving File"), _("Document is empty"), wxOK );
	return;
	}
if (nomDuDocument.Length()<=0 || repertoireDuDocument.Length()<=0)
	{
	EnregistrerSous(evt);
	return;
	}

wxString s=repertoireDuDocument+_T("\\")+nomDuDocument;
if (::wxFileExists(s))
	if (wxMessageBox(_("File already exist. Overwrite it?"), _T("Message"), wxYES_NO)==wxNO)
		return;
EnregistrerImage();
}

/**
Enregister l'image
*/
void FenetrePrincipale::EnregistrerSous(wxCommandEvent& evt)
{
if (imAcq==NULL)
	return;
wxString typePossible("PNG files (*.png)|*.png|JPG files (*.jpg)|*.jpg|TIFF files (*.tif)|*.tif");
typePossible += "|XML files (*.yml)|*.yml";
wxFileDialog ouverture(this, _("Save As"), wxEmptyString, wxEmptyString, typePossible,wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
if (ouverture.ShowModal()==wxID_CANCEL)
	return;
nomDuDocument = ouverture.GetFilename();
repertoireDuDocument = ouverture.GetDirectory();
wxString s=repertoireDuDocument+_T("\\")+nomDuDocument;
if (::wxFileExists(s))
	if (wxMessageBox("File already exist. Overwrite it?", "Warning", wxYES_NO==wxNO))
		return;
EnregistrerImage();
}

void FenetrePrincipale::ModeCamera(wxCommandEvent& event)
{
modeCamera= !modeCamera;
if (modeCamera )
	{
	if (cam->IsPaused())
		cam->Resume();
	else
		{
		if (cam==NULL)
			cam =new CameraOpenCV();
		cam->image=imAffichee;
		((CameraOpenCV*)cam)->imAcq=imAcq;
		cam->parent=(void*)this;
		cam->Run();
		}
	}
else
	DoPauseThread();
if (barreEtat)
	barreEtat->MAJButton();

};

void FenetrePrincipale::MAJZoom(wxCommandEvent& event)
{
if(event.GetId()>=ZOOM8SUR1)
	fenZoom->Show(true);
tpsPreEvt=-1;
feuille->FacteurZoom(event.GetId()-ZOOM1SUR1);
//AdjustHV(wxHORIZONTAL);
//AdjustHV(wxVERTICAL);
int nume;
int deno;
CalculZoom(nume,deno);
feuille->SetVirtualSize( imAcq->cols*nume/deno, imAcq->rows*nume/deno );

wxClientDC dc(feuille);
DrawWindow(dc);
}

void FenetrePrincipale::DebutAcquisition(wxCommandEvent& event)
{
while (!dateSeq)
	OnOuvrirHorodatage(event);
if (!horlogeSeq)
	horlogeSeq =  new wxTimer(this,1);
if (horlogeSeq->IsRunning())
	return;
dateSeq->RAZ();
int	d=dateSeq->TpsProchaineImage()*1000;
if (d==0)
	d=1;
if (dateSeq->CommentaireFixe())
	osgApp->ImgStat()->MAJInfo(5,(char*)dateSeq->CommentaireFixe());
else
	osgApp->ImgStat()->MAJInfo(5," ");

if (dateSeq->CommentaireVar())
	osgApp->ImgStat()->MAJInfo(7,dateSeq->CommentaireVar());
else
	osgApp->ImgStat()->MAJInfo(7," ");
if (d>0)
	horlogeSeq->Start(d,true);
}

void FenetrePrincipale::FinAcquisition(wxCommandEvent& event)
{
horlogeSeq->Stop();
dateSeq->RAZ();
}

/**
 Menu quitter
*/
void FenetrePrincipale::Quitter(wxCommandEvent& event)
{

//exit(0);
Close(true);
}

FenetrePrincipale::~FenetrePrincipale()
{
}



void FenetrePrincipale::OnClose(wxCloseEvent& event)
{
if (horlogeSeq && horlogeSeq->IsRunning())
	{
	wxMessageBox( _T("You must stop grabbing sequence before!"), _T("Quit Application") ,wxOK);
	event.Veto();
	return;
	}
/*if (ModeCamera())
	{
	wxMessageBox(_T("You must stop grabbing image before!"), _T("Quit Application") , wxOK);
	event.Veto();
	return;

	}*/
if (!osgApp->Quitter())
	{
	wxMessageDialog w(this, _T("Close window") , _T("Quit"), wxYES_NO|wxCENTRE|wxSTAY_ON_TOP);
	if (w.ShowModal()==wxID_YES)
		{
		if (fenetreSauvee==0)
			{
			wxMessageDialog w(this, _T("Do you want to save it?") , _T("Quit"), wxYES_NO|wxCENTRE|wxSTAY_ON_TOP);
			if (w.ShowModal()==wxID_YES)
				{
				wxCommandEvent evt;
				Enregistrer(evt);
				}
			}
		else
			w.Close();
		}
	else
		{
		event.Veto();
		return;
		}
	}
if (cam!=NULL )
	{
	if (osgApp->CtrlCamera()  && osgApp->CtrlCamera()->Camera()==cam)
		{
		osgApp->CtrlCamera()->DefCamera(NULL);
		osgApp->CtrlCamera()->SetTitle(_("Undefined"));
		}
	if (cam->IsPaused())
		{
		cam->parent=NULL;
		cam->Resume();	
		}
}

if (!osgApp->Quitter())
	osgApp->RetirerListe(this);
wxThread::ExitCode rc;
if (cam)
	OnCloseThread(event);

detectionUtilisateur->Stop();delete detectionUtilisateur;

delete []seuilNivBas;
delete []coeffCanal;
delete imAcq;


delete imAffichee;
delete []pLineaire;
delete []pAleatoire;
delete []pJet;
delete []pRainbow;
delete []pPerso;
delete []pPersoInv;
// est déjà fait par delete imAffichee delete tabRGB;
delete []tabRGBTransparence;

wxFrame::OnCloseWindow(event);
}

void FenetrePrincipale::EnregistrerImage()
{
string	nomFichier;

wxString s=repertoireDuDocument+_T("\\")+nomDuDocument;
wxFileName p(s);

nomFichier = s;

vector<int> compression_params;

if (p.GetExt().Cmp("yml")==0)
	{
	nomFichier = nomFichier +"gz";
	cv::FileStorage fs(nomFichier, cv::FileStorage::WRITE);
	fs<<"Image"<<*((cv::Mat*)imAcq);
	fs.release();
	}
else
	{
	if (p.GetExt().Cmp("png")==0)
		{
		compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		}
	if (p.GetExt().Cmp("jpg")==0)
		{
		compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
		compression_params.push_back(95);
		}
	try
		{
		cv::imwrite(nomFichier,*imAcq,compression_params);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox(_("An error occured while saving file :")+s);
		}
	}
}


void FenetrePrincipale::OnIdle(wxIdleEvent &event)
{
if (ModeCamera())
	imageTraitee=true;
//event.RequestMore();
return;
}


void FenetrePrincipale::OnMenuContext(wxContextMenuEvent& event)
{
    wxPoint point = event.GetPosition();
    // If from keyboard
    if (point.x == -1 && point.y == -1) {
        wxSize size = GetSize();
        point.x = size.x / 2;
        point.y = size.y / 2;
    } else {
        point = ScreenToClient(point);
    }
    ShowContextMenu(point);
}

void FenetrePrincipale::ShowContextMenu(const wxPoint& pos)
{
wxMenu menu;

if (osgApp->ModeSouris()==SOURIS_STD)
	{
	menu.Append(Menu_Help_About, _T("&About"));
	menu.Append(Menu_Popup_Palette, _T("&Palette"), CreateMenuPalette(NULL));
	menu.Append(Menu_Popup_Zoom, _T("&Zoom"), CreateMenuZoom(NULL));
	menu.AppendCheckItem(Menu_Rectangle, _T("Stat Rectangle"));
	menu.AppendCheckItem(Menu_Coupe, _T("Section"));
	menu.AppendCheckItem(Menu_FilMax, _T("Filtrage Max"));
	if (osgApp->Fenetre(origineImage.indOp1))
		menu.AppendCheckItem(Menu_ParAlg, _T("Parametrage Algo."));
	menu.AppendSeparator();
	menu.Append(ENREGISTRER_FICHIER, _T("Save"));
	menu.Append(ENREGISTRERSOUS_FICHIER, _T("Save As"));
	menu.Append(CREER_RAPPORT, _T("Create a report"));
	menu.Append(QUITTER_, _T("Close"));
	if (feuille->ModeRectangle())
		menu.Check(Menu_Rectangle, true);
	if (feuille->ModeCoupe())
		menu.Check(Menu_Coupe, true);
	if (modeFiltre)
		menu.Check(Menu_FilMax, true);
	}
else
	{
	menu.AppendCheckItem(MENU_OP1, "Image as Op1");
	menu.AppendCheckItem(MENU_OP2, "Image as Op2");
	menu.AppendSeparator();
	menu.AppendCheckItem(MENU_EXEC_OP, "Execute "+*osgApp->NomOperation());
	menu.Append(RESET_OP,  "Operation canceled");
	if (osgApp->Op1()==imAcq)
		menu.Check(MENU_OP1, true);
	if (osgApp->Op2()==imAcq)
		menu.Check(MENU_OP2, true);

	}


PopupMenu(&menu, pos.x, pos.y);

    // test for destroying items in popup menus
#if 0 // doesn't work in wxGTK!
    menu.Destroy(Menu_Popup_Submenu);

    PopupMenu( &menu, event.GetX(), event.GetY() );
#endif // 0
}

void FenetrePrincipale::PrepOperation(wxCommandEvent& event)
{
switch(event.GetId()){
case MENU_OP1 :
	if (osgApp->Op1()==imAcq)
		osgApp->DefOperande1(NULL);
	else
		{
		osgApp->DefOperande1(imAcq,idFenetre);
		}
	break;
case MENU_OP2 :
	if (osgApp->Op2()==imAcq)
		osgApp->DefOperande2(NULL);
	else
		osgApp->DefOperande2(imAcq,idFenetre);
	break;
case MENU_EXEC_OP:
	osgApp->CreerFenetreOperation();

	break;
case RESET_OP :
	wxMessageDialog w(this, "Do you want to cancel operation?" , _T("Cancel"), wxYES_NO|wxCENTRE|wxSTAY_ON_TOP);
	if (w.ShowModal()==wxID_YES)
		{
		osgApp->AnnuleOp();
		osgApp->DefPointeurSouris(0,0);

		}
	}
}

void FenetrePrincipale::DefHistorique(int ind1,int ind2 ,wxString nomF,Parametre *pOCV) /*< Associer origine à une image */
{
if (ind1!=-1)
	{
	origineImage.indOp1=ind1;
	origineImage.indOp2=ind2;
	origineImage.op1 = osgApp->Fenetre(ind1)->ImAcq();
	if (ind2>=0)
		origineImage.op2 = osgApp->Fenetre(ind2)->ImAcq();
	origineImage.nomOperation=nomF;
	if (pOCV)
		origineImage.pOCV=*pOCV;
	}

if (idFenetre>=0)
	{
	wxString param;
	wxString chemin;
	wxString chaine;
	chemin.Printf("/operateur/%d/",idFenetre);
	osgApp->SauverFichierConfig(chemin,"op",origineImage.nomOperation);
	osgApp->SauverFichierConfig(chemin,"op1",(long)origineImage.indOp1);
	osgApp->SauverFichierConfig(chemin,"op2",(long)origineImage.indOp2);
	std::map<std::string,DomaineParametre<int> >::iterator iti;
	int nb=0;
	for (iti=origineImage.pOCV.intParam.begin();iti!=origineImage.pOCV.intParam.end();iti++)
		{
		chemin.Printf("/operateur/%d/paramEntier/%d",idFenetre,nb);
		osgApp->SauverFichierConfig(chemin,"nom",iti->first);
		osgApp->SauverFichierConfig(chemin,"valeur",(long)iti->second.valeur);
		osgApp->SauverFichierConfig(chemin,"minVal",(long)iti->second.mini);
		osgApp->SauverFichierConfig(chemin,"maxVal",(long)iti->second.maxi);
		osgApp->SauverFichierConfig(chemin,"pasVal",(long)iti->second.pas);
		nb++;
		}
	nb=0;
	std::map<std::string,DomaineParametre<double> >::iterator itd;
	for (itd=origineImage.pOCV.doubleParam.begin();itd!=origineImage.pOCV.doubleParam.end();itd++)
		{
		chemin.Printf("/operateur/%d/paramDouble/%d",idFenetre,nb);
		osgApp->SauverFichierConfig(chemin,"nom",itd->first);
		osgApp->SauverFichierConfig(chemin,"valeur",itd->second.valeur);
		osgApp->SauverFichierConfig(chemin,"minVal",(long)itd->second.mini);
		osgApp->SauverFichierConfig(chemin,"maxVal",(long)itd->second.maxi);
		osgApp->SauverFichierConfig(chemin,"pasVal",(long)itd->second.pas);
		nb++;
		}
	nb=0;
	std::map<std::string,DomaineParametre<cv::Size> >::iterator its;
	for (its=origineImage.pOCV.sizeParam.begin();its!=origineImage.pOCV.sizeParam.end();its++)
		{
		chemin.Printf("/operateur/%d/paramSize/%d",idFenetre,nb);
		osgApp->SauverFichierConfig(chemin,"nom",its->first);
		osgApp->SauverFichierConfig(chemin,"largeur",(long)its->second.valeur.width);
		osgApp->SauverFichierConfig(chemin,"hauteur",(long)its->second.valeur.height);
		nb++;
		}
	}

}

wxMenu *FenetrePrincipale::CreateMenuPalette(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(NOIRETBLANC_, _T("&Gris\tCtrl-F1"));
    menu->Append(NOIRETBLANC_+1, _T("&Arcenciel\tCtrl-F2"));
    menu->Append(NOIRETBLANC_+2, _T("&Arcenciel 8bits\t"));
    menu->Append(NOIRETBLANC_+3, _T("&Gris 8 bits\tCtrl-F2"));
    menu->Append(NOIRETBLANC_+4, _T("&Arcenciel circulaire 8 bits\t"));
    menu->Append(NOIRETBLANC_+5, _T("&gris circulaire 8 bis\t"));
    menu->Append(NOIRETBLANC_+6, _T("&Jet\t"));
    menu->Append(NOIRETBLANC_+7, _T("&Rainbow\t"));
    menu->Append(NOIRETBLANC_+8, _T("&Owner\t"));
    menu->Append(NOIRETBLANC_+9, _T("&Aleatoire\t"));

    return menu;
}

wxMenu *FenetrePrincipale::CreateMenuZoom(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(ZOOM1SUR2, _T("&Zoom 1/2\tCtrl-F1"));
    menu->Append(ZOOM1SUR1, _T("&Zoom x1\tCtrl-F2"));
    menu->Append(ZOOM2SUR1, _T("&Zoom x2\tCtrl-F2"));
    menu->Append(ZOOM4SUR1, _T("&Zoom x4\tCtrl-F2"));
    menu->Append(ZOOM8SUR1, _T("&Zoom x8\tCtrl-F2"));

    return menu;
}

/**
Ouvrir l'image calibre et la ranger dans la classe NanoRaptor
*/
void FenetrePrincipale::SelectPalette(wxCommandEvent& event)
{
wxColour	*p=NULL;
indPalette=event.GetId()-NOIRETBLANC_;
switch(indPalette){
case 6:
	pCouleur=pJet;
	break;
case 7:
	pCouleur=pRainbow;
	break;
case 8:
	pCouleur=pPerso;
	break;
case 9:
	pCouleur=pAleatoire;
	break;
case 0:
	pCouleur=pLineaire;
	break;
case 2:
//	pCouleur=palArcEnCiel;
	break;
	}

MAJNouvelleImage();
osgApp->ImgStat()->DrawPaletteActuelle();
osgApp->ImgStat()->DrawPalette();
return;
}
/*
void FenetrePrincipale::OnScrollTop( wxScrollWinEvent &w)
{
int p=feuille->GetScrollPos(w.GetOrientation());
feuille->SetScrollPos(w.GetOrientation(),p);
wxClientDC dc(feuille);
DrawWindow(dc);
}

void FenetrePrincipale::OnScrollBottom( wxScrollWinEvent &w)
{
}

void FenetrePrincipale::OnScrollUp( wxScrollWinEvent &w)
{
int p=feuille->GetScrollPos(w.GetOrientation())-1;
feuille->SetScrollPos(w.GetOrientation(),p);
wxClientDC dc(feuille);
DrawWindow(dc);
}

void FenetrePrincipale::OnScrollDown( wxScrollWinEvent &w)
{
int p=feuille->GetScrollPos(w.GetOrientation())+1;
feuille->SetScrollPos(w.GetOrientation(),p);
wxClientDC dc(feuille);
DrawWindow(dc);
}

void FenetrePrincipale::OnScrollPageUp( wxScrollWinEvent &w)
{
int p=feuille->GetScrollPos(w.GetOrientation())-8;
feuille->SetScrollPos(w.GetOrientation(),p);
wxClientDC dc(this);
DrawWindow(dc);
}

void FenetrePrincipale::OnScrollPageDown( wxScrollWinEvent &w)
{
int p=feuille->GetScrollPos(w.GetOrientation())+8;
feuille->SetScrollPos(w.GetOrientation(),p);
wxClientDC dc(this);
DrawWindow(dc);
}
*/

void FenetrePrincipale::OnSize( wxSizeEvent &w)
{
if (!imAcq)
	return;
wxSize	taille=GetClientSize();
float	fZoomNume=pow(2.0,feuille->FacteurZoom());
SetClientSize(taille.x, taille.y);
feuille->SetSize(taille);
feuille->OnSize(w);

//wxClientDC dc(feuille);
//DrawWindow(dc);

}

void FenetrePrincipale::DetectionUtilisateur(wxTimerEvent& event)
{
osgApp->DefUtilisateurAbsent(1);
detectionUtilisateur->Start(TPSMISEENVEILLECOURBE);
}

void FenetrePrincipale::OnTimer(wxTimerEvent& event)
{
if (!dateSeq)
	return;
char *t[1];
char	nomFichier[2048];
t[0]=nomFichier;
strcpy(t[0],dateSeq->NomImage());
wxFileName	nomImage(dateSeq->NomImage());
wxString	nomVolume,nomRep,nomFic,extension;
wxString	repImageBrute,repImageCorrigee,repImageFiltre;


nomImage.SplitPath(dateSeq->NomImage(),&nomVolume,&nomRep,&nomFic,&extension);
int nb=nomFic.length()-1;
while (nb>=0 && nomFic[nb]>=48 && nomFic[nb]<='9')
	nb--;
wxString nomDeLaSerie= nomFic.substr(0,nb+1)+_T("_");
repImageBrute=nomRep+nomImage.GetPathSeparator()+nomDeLaSerie+_T("Brut");
repImageCorrigee=nomRep+nomImage.GetPathSeparator()+nomDeLaSerie+_T("Corrigee");
repImageFiltre=nomRep+nomImage.GetPathSeparator()+nomDeLaSerie+_T("Filtre");
wxString nom;

if (dateSeq->IndiceImageEnCours()==0)
	{
	wxString repImageBiais=nomRep+nomImage.GetPathSeparator()+nomDeLaSerie+_T("BiasBackground");

	if (!::wxDirExists(nomRep))
		::wxMkdir(nomRep);
	if (!::wxDirExists(repImageBiais))
		::wxMkdir(repImageBiais);
	if (correctionBiais || correctionFonction || correctionTache)
		{
		wxString	n1,n2,n3,n4;



		if (correctionBiais)
			{
			nomImage.SplitPath(nomImageBiais,&n1,&n2,&n3,&n4);
			nom=nomVolume+nomImage.GetVolumeSeparator()+repImageBiais+nomImage.GetPathSeparator()+
				n3+_T(".")+extension;
			strcpy(t[0],nom.char_str());
//			nivBiais->DoEnregistrer(t);
			wxLogVerbose(_T("Image saved %s "),t[0]);
			}
		if (correctionFonction)
			{
			nomImage.SplitPath(nomImageQuadrique,&n1,&n2,&n3,&n4);
			nom=nomVolume+nomImage.GetVolumeSeparator()+repImageBiais+nomImage.GetPathSeparator()+
				n3+_T(".")+extension;
			strcpy(t[0],nom.char_str());
			//imQuadrique->DoEnregistrer(t);
			wxLogVerbose(_T("Image saved %s "),t[0]);
			}
		if (correctionTache)
			{
			nomImage.SplitPath(nomImageTache,&n1,&n2,&n3,&n4);
			nom=nomVolume+nomImage.GetVolumeSeparator()+repImageBiais+nomImage.GetPathSeparator()+
				n3+_T(".")+extension;
			strcpy(t[0],nom.char_str());
//			imTache->DoEnregistrer(t);
			wxLogVerbose(_T("Image saved %s "),t[0]);
			}

		}
	if (!::wxDirExists(repImageBrute))
		::wxMkdir(repImageBrute);
	if (!::wxDirExists(repImageCorrigee))
		::wxMkdir(repImageCorrigee);
	if (!::wxDirExists(repImageFiltre))
		::wxMkdir(repImageFiltre);
	}


wxString	s=osgApp->ImgStat()->LireInfo(5)+_T("\n")+osgApp->ImgStat()->LireInfo(7)+_T("\n")+CommentaireConfig();
if (s.length())
	{
	imAcq->DefDescription((char*)s.char_str());
//	imAcq2->DefDescription((char*)s.char_str());
//	imAcqBrut2->DefDescription((char*)s.char_str());
	}
if (cam->ModeMoyenne() || correctionBiais || correctionFonction || correctionTache)
	{
	nom=nomVolume+nomImage.GetVolumeSeparator()+repImageCorrigee+nomImage.GetPathSeparator()+nomFic+_T("cor.")+extension;
	strcpy(t[0],nom.char_str());
	imAcq->DoEnregistrer(t);
			wxLogVerbose(_T("Image saved %s "),t[0]);
	}
if (cam->ModeMoyenne() || correctionBiais || correctionFonction || correctionTache)
	{
	nom=nomVolume+nomImage.GetVolumeSeparator()+repImageFiltre+nomImage.GetPathSeparator()+nomFic+_T("corFilmax.")+extension;
	strcpy(t[0],nom.char_str());
//	imAcq2->DoEnregistrer(t);
			wxLogVerbose(_T("Image saved %s "),t[0]);
	}

osgApp->ImgStat()->MAJInfo(1,nomFichier);
ofstream	gg((dateSeq->NomFichierRapport()).c_str(),ios::app);

wxDateTime dd;
dd.SetToCurrent();
wxString	sd=dd.FormatISODate();
wxString	sh=dd.FormatISOTime();

gg<<sd<<"\t"<<sh<<"\t"<<dateSeq->DateImage()<<"\t"<<dateSeq->NomImage()<<"\t";
if (osgApp->CtrlCamera())
	gg<<osgApp->CtrlCamera()->TempsExposition()<<"\t"<<osgApp->CtrlCamera()->ModeGainEMCCD()<<"\t"<<osgApp->CtrlCamera()->GainEMCCD()<<"\t";
else
	gg<<1<<"\t"<<1<<"\t"<<1<<"\t";
/*if (modeMoyenne)
	gg<<"1\t"<<bbButter[indFiltreMoyenne]<<"\t"<<aaButter[indFiltreMoyenne]<<"\t";
else
	gg<<"0\t-1\t-1\t";
*/
if (correctionBiais)
	gg<<"1\t"<<nomImageBiais<<"\t";
else
	gg<<"0\tNULL\t";
if (correctionTache)
	gg<<"1\t"<<nomImageTache<<"\t";
else
	gg<<"0\tNULL\t";
if (correctionFonction)
	gg<<"1\t"<<nomImageQuadrique<<"\t";
else
	gg<<"0\tNULL\t";
if (dateSeq->CommentaireFixe())
	gg<<dateSeq->CommentaireFixe()<<"\t";
else
	gg<<"NULL\t";
if (dateSeq->CommentaireVar())
	gg<<dateSeq->CommentaireVar()<<"\t";
else
	gg<<"NULL\t";
gg<<"\n";
	{
	nom=nomVolume+nomImage.GetVolumeSeparator()+repImageBrute+nomImage.GetPathSeparator()+nomFic+_T(".")+extension;
	strcpy(t[0],nom.char_str());
//	imAcqBrut2->DoEnregistrer(t);
	wxLogVerbose(_T("Image saved %s "),t[0]);
	}
osgApp->ImgStat()->MAJInfo(3,dateSeq->DateImage());
dateSeq->Avancer();
if (dateSeq->CommentaireFixe())
	osgApp->ImgStat()->MAJInfo(5,dateSeq->CommentaireFixe());
else
	osgApp->ImgStat()->MAJInfo(5," ");
if (dateSeq->CommentaireVar())
	osgApp->ImgStat()->MAJInfo(7,dateSeq->CommentaireVar());
else
	osgApp->ImgStat()->MAJInfo(7," ");
int	d=dateSeq->TpsProchaineImage()*1000;
if (d>0)
	{
	horlogeSeq->Start(d,true);
	}
else
	{
	wxMessageBox(_("End"), _("Timestamp ended!"), wxOK);
	wxLogWarning(_("Timestamp ended!"));
	}
}
/*
void FenetrePrincipale::AdjustHV(int orientation)
{
return;
wxRect	recSrc(0,0,imAcq->cols,imAcq->rows);
long 			maxi,mini=0;
wxRect			rGraphique=feuille->GetClientSize();
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

long val= feuille->GetScrollPos(orientation);

if (orientation==wxHORIZONTAL)
	maxi = -(rGraphique.GetRight())*fZoomDeno/fZoomNume+recSrc.GetRight();
else
	maxi = -(rGraphique.GetBottom())*fZoomDeno/fZoomNume+recSrc.GetBottom();
if (maxi<0)
	maxi=-1;
feuille->SetScrollbar(orientation,val,16, maxi);
} // AdjustHV
*/

void FenetrePrincipale::DefinitionFondMicro()
{
if (nbImageTache==0)
	return;
/*int nbPixels=imTache->rows*imTache->cols;
cv::Scalar	m=mean((cv::InputArray)imTache, cv::noArray() );
*imTache = imTache;

float	*data=(float*)imTache->LitPlan(0);
unsigned short *dataRef=(unsigned short *)imRefTache->LitPlan(0);
for (int i=0;i<nbPixels;i++,data++,dataRef++)
	if (*dataRef>0)
		*data =  m/ *dataRef ;
*/
}

void FenetrePrincipale::DefinitionFondQuadrique(wxCommandEvent& event)
{
}


void FenetrePrincipale::EnregistrerBiais()
{
wxString	rep=wxEmptyString;;
if (!::wxDirExists(_T("\\Images")))
	::wxMkdir(_T("\\Images"));
if (!::wxDirExists(_T("\\Images\\BiasBackground")))
	::wxMkdir(_T("\\Images\\BiasBackground"));
if (::wxDirExists(_T("\\Images")))
	if (::wxDirExists(_T("\\Images\\BiasBackground")))
		rep=_T("\\Images\\BiasBackground\\");
wxDateTime d;
d.SetToCurrent();
wxString	sd=d.FormatISODate();
wxString	sh=d.FormatISOTime();
sh.Replace(_T(":"),_T("_"),true);
nomImageBiais=_T("Biais")+sd+_T("_")+sh+_T(".tif");
nomImageBiais=rep+nomImageBiais;
char	nomFichier[2048];
char *t[1];
t[0]=nomFichier;
strcpy(t[0],nomImageBiais.ToAscii());
//nivBiais->DoEnregistrer(t);
wxString message(_("Bias image saved in "));
message=message+nomImageBiais+_T(". Do you want to save it in another folder?");
int reponse = wxMessageBox(message,_T("Bias Info"),wxYES_NO );
if (reponse==wxYES)
	{
	nomImageBiais=_T("Biais")+sd+_T("_")+sh+_T(".tif");
	wxFileDialog ouverture(this, _("Save As"), rep,nomImageBiais , _("*.tif"),wxFD_SAVE );
	if (ouverture.ShowModal()!=wxID_OK)
		return;
	nomImageBiais = ouverture.GetFilename();
	rep = ouverture.GetDirectory();
	nomImageTache=rep+_("\\")+nomImageBiais;
	if (::wxFileExists(nomImageTache))
		if (!wxMessageBox(_("File already exist"), _("Message"), wxYES_NO))
			return;
	strcpy(t[0],nomImageTache.ToAscii());
//	nivBiais->DoEnregistrer(t);

	}

}

void FenetrePrincipale::EnregistrerTache()
{
wxString	rep=wxEmptyString;
if (!::wxDirExists(_T("\\Images")))
	::wxMkdir(_T("\\Images"));
if (!::wxDirExists(_T("\\Images\\BiasBackground")))
	::wxMkdir(_T("\\Images\\BiasBackground"));
if (::wxDirExists(_T("\\Images")))
	if (::wxDirExists(_T("\\Images\\BiasBackground")))
		rep=_T("\\Images\\BiasBackground\\");
wxDateTime d;
d.SetToCurrent();
wxString	sd=d.FormatISODate();
wxString	sh=d.FormatISOTime();
sh.Replace(_T(":"),_T("_"),true);
nomImageTache=_T("RefTache")+sd+_T("_")+sh+_T(".tif");
nomImageTache=rep+nomImageTache;
char	nomFichier[2048];
char *t[1];
t[0]=nomFichier;
strcpy(t[0],nomImageTache.ToAscii());
//imRefTache->DoEnregistrer(t);
DefinitionFondMicro();
nomImageTache=_T("Tache")+sd+_T("_")+sh+_T(".tif");
nomImageTache=rep+nomImageTache;
t[0]=nomFichier;
strcpy(t[0],nomImageTache.ToAscii());
//imTache->DoEnregistrer(t);
wxString message(_T("Background image saved in "));
message=message+nomImageTache+_T(". Do you want to save it in another folder?");
int reponse = wxMessageBox(message,_T("Background Info"),wxYES_NO );
if (reponse==wxYES)
	{
	nomImageTache=_T("Tache")+sd+_T("_")+sh+_T(".tif");
	wxFileDialog ouverture(this, _T("Save As"), rep,nomImageTache , _T("*.tif"),wxFD_SAVE );
	if (ouverture.ShowModal()!=wxID_OK)
		return;
	nomImageTache = ouverture.GetFilename();
	rep = ouverture.GetDirectory();
	nomImageTache=rep+_T("\\")+nomImageTache;
	if (::wxFileExists(nomImageTache))
		if (!wxMessageBox(_T("File already exist"), _T("Message"), wxYES_NO))
			return;
	strcpy(t[0],nomImageTache.ToAscii());
//	imTache->DoEnregistrer(t);

	}


}

void FenetrePrincipale::OuvrirTache(char	*nomFichier)
{
/*
ImageInfoCV *imtmp=new ImageInfoCV(nomFichier);
if (imtmp->rows!=imAcq->cols || imtmp->cols!=imAcq->cols || imtmp->LitTypePixel()!=REEL_IMAGE)
	{
	wxMessageBox(_T("Image has not grabbed using LUCAS-R"));
	delete imtmp;
	return;
	}
if (fondDejaDefini)
	{
	char test=wxMessageBox(_T(" Do you want to add new background to previous gain?"),_T("Background already defined."),wxYES_NO );
	if (test==wxYES)
		{
		int nbPixels=imTache->rows*imTache->cols;
		float	*data1=(float*)imTache->LitPlan(0);
		float	*data2=(float *)imtmp->LitPlan(0);
		for (int i=0;i<nbPixels;i++,data1++,data2++)
				*data1 =  *data1 * *data2 ;
		EnregistrerTache();
		}
	else
		{
		if (imTache)
			 delete imTache;
		imTache = imtmp;
		}

	}
else
	{
	if (imTache)
		 delete imTache;
	imTache = imtmp;
	}
}


void FenetrePrincipale::OuvrirTache(wxCommandEvent& event)
{
wxFileDialog ouverture(this, _T("Ouvrir "), _T("\\Images\\BiasBackground"), _T(""), _T("*.tif"));
if (ouverture.ShowModal()!=wxID_OK)
	return;
if (ModeCamera())
	{
	wxMessageBox(_T("Grabbing images stopped!"));
	ModeCamera(event);
	}
wxString s(ouverture.GetFilename());
wxString s2(ouverture.GetDirectory());
s = _T("\\")+s;
s=s2+s;
wxCharBuffer ww=s.mb_str ();
char *nomFichier=ww.data() ;

OuvrirTache(nomFichier);
nomImageTache=s;
*/
}

void FenetrePrincipale::OuvrirBiais(char *nomFichier)
{
/*ImageInfoCV *imtmp=new ImageInfoCV(nomFichier);
if (imtmp->rows!=imAcq->rows || imtmp->cols!=imAcq->cols || imtmp->LitTaillePixel()!=imAcq->LitTaillePixel())
	{
	wxMessageBox(_T("Image has not grabbed using LUCAS-R"));
	delete imtmp;
	return;
	}
if (nivBiais)
	delete nivBiais;
nivBiais = imtmp;
*/
}


void FenetrePrincipale::OuvrirBiais(wxCommandEvent& event)
{
wxFileDialog ouverture(this, _T("Ouvrir "), _T("\\Images\\BiasBackground"), wxEmptyString, _T("*.tif"));
if (ouverture.ShowModal()!=wxID_OK)
	return;
if (ModeCamera())
	{
	wxMessageBox(_T("Grabbing images stopped!"));
	ModeCamera(event);
	}
wxString s(ouverture.GetFilename());
wxString s2(ouverture.GetDirectory());
s = _T("\\")+s;
s=s2+s;
wxCharBuffer ww=s.mb_str ();
char *nomFichier=ww.data() ;
OuvrirBiais(nomFichier);
nomImageBiais=s;

}

void FenetrePrincipale::DefImageQuadrique()
{
/*
int nbPixels=imTache->LitNbLigne()*imTache->LitNbColonne();
float *val=(float *)imQuadrique->LitPlan(0);
for (int i=0;i<nbPixels;i++,val++)
	{
	double qxy=imQuadrique->FonctionFond(i,poly[0]);
	if (qxy!=0)
		*val = 1./qxy;
	}
wxDateTime d;
d.SetToCurrent();
wxString	sd=d.FormatISODate();
wxString	sh=d.FormatISOTime();
sh.Replace(_T(":"),_T("_"),true);
nomImageQuadrique=_T("Quadrique")+sd+_T("_")+sh+_T(".tif");

wxString	rep=_T("");;
if (!::wxDirExists(_T("\\Images")))
	::wxMkdir(_T("\\Images"));
if (!::wxDirExists(_T("\\Images\\BiasBackground")))
	::wxMkdir(_T("\\Images\\BiasBackground"));
if (::wxDirExists(_T("\\Images")))
	if (::wxDirExists(_T("\\Images\\BiasBackground")))
		rep=_T("\\Images\\BiasBackground\\");

nomImageQuadrique=rep+nomImageQuadrique;
char	nomFichier[2048];
char *t[1];
t[0]=nomFichier;
strcpy(t[0],nomImageQuadrique.ToAscii());
imQuadrique->DoEnregistrer(t);
*/
}

// On some platforms, notably Mac OS X with Core Graphics, we can't blit from
// a window, so we need to draw the background explicitly.
bool GlisserImage::UpdateBackingFromWindow(wxDC& WXUNUSED(windowDC), wxMemoryDC& destDC, const wxRect& WXUNUSED(sourceRect),
                    const wxRect& destRect) const
{
    destDC.SetClippingRegion(destRect);

    if (m_canvas->ImageAffichee())
		{
		wxBitmap b(*m_canvas->ImageAffichee());
        m_canvas->TileBitmap(destRect, destDC, b);
		}
    m_canvas->DrawShapes(destDC);
    return true;
}



bool FenetrePrincipale::TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap)
{
	wxPoint p(0,50);
	TracerDIB(imAffichee,dc);
	return true;
    int			w = bitmap.GetWidth();
    int			h = bitmap.GetHeight();

    int i, j;
    for (i = rect.x; i < rect.x + rect.width; i += w)
    {
        for (j = rect.y; j < rect.y + rect.height; j+= h)
            dc.DrawBitmap(bitmap, i, j+50);
    }
    return true;
}

void wxOsgApp::OnUseScreen(wxCommandEvent& WXUNUSED(event))
{
    m_useScreen = !m_useScreen;
}


ImageInfoCV *FenetrePrincipale::ImageOp1pre()
{
FenetrePrincipale *f=osgApp->Fenetre(origineImage.indOp1);
if (f)
	return f->ImAcq();
return NULL;

}

ImageInfoCV *FenetrePrincipale::ImageOp2pre()
{
FenetrePrincipale *f=osgApp->Fenetre(origineImage.indOp1);
if (f)
	return f->ImAcq();
return NULL;

}

void wxOsgApp::SauverFichierConfig(wxString s,int idFiltre,int type,int taille)
{

ImageInfoCV xx;

if (s.Cmp("conv")==0)
	{
	wxString chemin;
	chemin.Printf("/convolution/%d/",idFiltre);
	configApp->SetPath(chemin);
	for (int i=0;i<xx.opnn[idFiltre]->rows;i++)
		{
		wxString cheminLigne;
		cheminLigne.Printf("%d",i);
		configApp->SetPath(cheminLigne);
		for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
			{
			wxString cle;
			cle.Printf("%d",j);
			configApp->Write(cle,xx.opnn[idFiltre]->at<float>(i,j));
			}
		configApp->SetPath(chemin);
		}
	}
configApp->SetPath("/");
if (s.Cmp("morph")==0)
	{
	wxString chemin;
	chemin.Printf("/morphologie/%d/",idFiltre);
	configApp->SetPath(chemin);
	wxString cle("morph_taille");
	configApp->Write(cle,taille);
	cle="morph_type";
	configApp->Write(cle,type);
	}
configApp->SetPath("/");
configApp->Flush();
}


void wxOsgApp::LectureFichierConfig()
{
ImageInfoCV xx;
// Lecture des opérateurs de convolution
wxString chemin;
chemin.Printf("/convolution");
configApp->SetPath(chemin);
wxString str;
long dummy;
bool bCont = configApp->GetFirstGroup(str, dummy);
while ( bCont )
	{
	wxString tmp1(str);
	long tmp2=dummy;
	wxString chemin=configApp->GetPath();
	configApp->SetPath(chemin+"/"+str);
	long idFiltre;
	str.ToLong(&idFiltre);
	for (int i=0;idFiltre>=0 && idFiltre<NB_OP_CONVOLUTION && i<xx.opnn[idFiltre]->rows;i++)
		{
		wxString cle;
		cle.Printf("%d",i);
		configApp->SetPath(cle);
		for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
			{
			cle.Printf("%d",j);
			configApp->Read(cle,&xx.opnn[idFiltre]->at<float>(i,j));
			}
		configApp->SetPath(chemin);
		}
	str=tmp1;
	dummy=tmp2;
	bCont = configApp->GetNextGroup(str, dummy);
	}

// Lecture des opérateurs de morphologie
chemin.Printf("/morphologie");
configApp->SetPath(chemin);
bCont = configApp->GetFirstGroup(str, dummy);
while ( bCont )
	{
	wxString tmp1(str);
	long tmp2=dummy;
	wxString chemin=configApp->GetPath();
	configApp->SetPath(chemin+"/"+str);
	long idFiltre;
	str.ToLong(&idFiltre);
	long taille,type;
	if (configApp->Read("morph_taille",&taille) && idFiltre >=0 && idFiltre<NB_OP_MORPHOLOGIE)
		{
 		if (configApp->Read("morph_type",&type))
			{
			if (xx.opMorph[idFiltre]!=NULL)
				{
				delete xx.opMorph[idFiltre];
				xx.opMorph[idFiltre]=NULL;
				}
			cv::Mat element = cv::getStructuringElement( type,cv::Size( 2*taille + 1, 2*taille+1 ),cv::Point( taille, taille ) );

			xx.opMorph[idFiltre] = new ImageInfoCV( 2*taille + 1, 2*taille+1,element.type());
			*((cv::Mat *)xx.opMorph[idFiltre]) =element;
			}
		}
	configApp->SetPath(chemin);
	str=tmp1;
	dummy=tmp2;
	bCont = configApp->GetNextGroup(str, dummy);
	}
// Lecture des opérations prédéfinies
int nbOperation=0;
configApp->SetPath("/operateur");
wxString	cleIndOperateur;
listeOperation.resize(configApp->GetNumberOfGroups());
bCont = configApp->GetFirstGroup(cleIndOperateur, dummy);
bool opValide;
while ( bCont )
	{
	wxString tmp1(cleIndOperateur);
	long tmp2=dummy;
	wxString chemin=configApp->GetPath();
// Lecture d'une opération
	configApp->SetPath(chemin+"/"+cleIndOperateur);
// Cette opération est composée d'un nom et de deux opérateurs
	opValide=true;
	if(!configApp->Read(wxString("op"),&listeOperation[nbOperation].nomOperation))
		{
		opValide=false;
		}
	if (opValide && !configApp->Read("op1",&listeOperation[nbOperation].indOp1))
		opValide=false;
	if (opValide && !configApp->Read("op2",&listeOperation[nbOperation].indOp2))
		opValide=false;
	if (opValide)
		{
		wxString cheminParam(chemin+"/"+cleIndOperateur+"/paramEntier");
		configApp->SetPath(cheminParam);
		bCont = configApp->GetFirstGroup(str, dummy);
		while (bCont && opValide)
			{
			wxString cle(cheminParam+"/"+str+"/nom");
			wxString nom;
			if (opValide && !configApp->Read(cle,&nom))
				opValide=false;
			cle=cheminParam+"/"+str+"/valeur";
			long val;
			if (opValide && !configApp->Read(cle,&val))
				opValide=false;
			long minVal,maxVal,pasVal;
			cle=cheminParam+"/"+str+"/minVal";
			if (opValide && !configApp->Read(cle,&minVal))
				opValide=false;
			cle=cheminParam+"/"+str+"/maxVal";
			if (opValide && !configApp->Read(cle,&maxVal))
				opValide=false;
			cle=cheminParam+"/"+str+"/pasVal";
			if (opValide && !configApp->Read(cle,&pasVal))
				opValide=false;
			if (opValide)
				{
				std::string s(nom.ToAscii());
				listeOperation[nbOperation].pOCV.intParam[s]=DomaineParametre<int>(val,minVal,maxVal,pasVal);
				}
			bCont = configApp->GetNextGroup(str, dummy);
			}
		}
	if (opValide)
		{
		wxString cheminParam(chemin+"/"+cleIndOperateur+"/paramDouble");
		configApp->SetPath(cheminParam);
		bCont = configApp->GetFirstGroup(str, dummy);
		while (bCont && opValide)
			{
			wxString cle(cheminParam+"/"+str+"/nom");
			wxString nom;
			if (opValide && !configApp->Read(cle,&nom))
				opValide=false;
			cle=cheminParam+"/"+str+"/valeur";
			double val;
			if (opValide && !configApp->Read(cle,&val))
				opValide=false;
			double minVal,maxVal,pasVal;
			cle=cheminParam+"/"+str+"/minVal";
			if (opValide && !configApp->Read(cle,&minVal))
				opValide=false;
			cle=cheminParam+"/"+str+"/maxVal";
			if (opValide && !configApp->Read(cle,&maxVal))
				opValide=false;
			cle=cheminParam+"/"+str+"/pasVal";
			if (opValide && !configApp->Read(cle,&pasVal))
				opValide=false;
			if (opValide)
				{
				std::string s(nom.ToAscii());
				listeOperation[nbOperation].pOCV.doubleParam[s]=DomaineParametre<double>(val,minVal,maxVal,pasVal);
				}
			bCont = configApp->GetNextGroup(str, dummy);
			}

		}
			
	if (opValide)
		{
		GenerationGraphDot(&listeOperation[nbOperation]);
		nbOperation++;
		}
	configApp->SetPath(chemin);
	str=tmp1;
	dummy=tmp2;
	bCont = configApp->GetNextGroup(cleIndOperateur, dummy);
	}
}

void wxOsgApp::GenerationGraphDot(Operation *op)
{
}



void wxOsgApp::SauverFichierConfig(wxString chemin,wxString cle, wxString chaine)
{
wxString s(configApp->GetPath());

configApp->SetPath(chemin);
configApp->Write(cle,chaine);
configApp->Flush();
configApp->SetPath(s);
}

void wxOsgApp::SauverFichierConfig(wxString chemin,wxString cle, long chaine)
{
wxString s(configApp->GetPath());

configApp->SetPath(chemin);
configApp->Write(cle,chaine);
configApp->Flush();
configApp->SetPath(s);
}

void wxOsgApp::SauverFichierConfig(wxString chemin,wxString cle, double chaine)
{
wxString s(configApp->GetPath());

configApp->SetPath(chemin);
configApp->Write(cle,chaine);
configApp->Flush();
configApp->SetPath(s);
}


void	wxOsgApp::SauverConfiguration(){listeFenetre[indFenetre]->fPrin->SauveDerniereConfig();};
