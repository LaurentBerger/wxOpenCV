#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include "ImageInfo.h"
#include "wx/wxprec.h"
#include "wx/splash.h"
#include <wx/display.h>
#include "mobile.xpm"

//#include <vld.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ControleCamera.h"
#include "imagestat.h"
#include "FenetreSeqOpe.h"
//#include "OutilsImage.h"
#include "wxOsgApp.h"
#include "FenetrePrincipale.h"
#include "TableauBord.h"
#include "InterfaceAvance.h"
#include "GlisserForme.h"
#include "CameraAndor.h"
#include "CameraOpenCV.h"
#include "FenetreAlgo.h"

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


#include <thread>        
#include <mutex>          


using namespace std;



// Gestion des constantes pour les langues
// language data
static const wxLanguage langIds[] =
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_FRENCH,
    wxLANGUAGE_ENGLISH
};

// note that it makes no sense to translate these strings, they are
// shown before we set the locale anyhow
const wxString langNames[] =
{
    "System default",
    "French",
    "English",
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

wxDECLARE_EVENT(VAL_EVT_CALCUL_FINI, EvtCalculFini);
wxDEFINE_EVENT(VAL_EVT_CALCUL_FINI, EvtCalculFini);

#define VAL_EVT_CALCUL_FINI(id, func) \
 	wx__DECLARE_EVT1(VAL_EVT_CALCUL_FINI, id, GstEvtCalculFini(func))



EvtCalculFini::EvtCalculFini(wxEventType commandType , int id):wxCommandEvent(m_sortie,id)
    {
        SetId(id);
        SetEventType(commandType);
        m_Expediteur = 0;
        m_sortie = false;
        m_travailEchoue = false;
		indEvt=0;
    }


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
	EVT_MENU(ZOOM1SUR8,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM1SUR4,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM1SUR2,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM1SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM2SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM4SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(ZOOM8SUR1,  FenetrePrincipale::MAJZoom)
	EVT_MENU(DEFINITION_FOND_QUADRIQUE,  FenetrePrincipale::DefinitionFondQuadrique)
	EVT_MENU(Menu_Rectangle,  FenetrePrincipale::MAJRectangle)
	EVT_MENU(Menu_Coupe,  FenetrePrincipale::MAJCoupe)
	EVT_MENU(Menu_FilMax,  FenetrePrincipale::MAJFiltreMax)
    EVT_MENU(Menu_ParAlg, FenetrePrincipale::ParamAlgo)
    EVT_MENU(Menu_ParPano, FenetrePrincipale::ParamPano)
    EVT_MENU(Menu_Contour, FenetrePrincipale::TracerContour)
    EVT_MENU(Menu_Contour_Hull, FenetrePrincipale::TracerEnveloppe)
    EVT_MENU(Menu_Defaut_Hull, FenetrePrincipale::TracerDefautEnveloppe)
    EVT_MENU(Menu_Contour_Poly, FenetrePrincipale::TracerContourPoly)
	EVT_MENU(MENU_LIGNEHOUGH,  FenetrePrincipale::TracerLigneHough)
	EVT_MENU(MENU_LIGNEPROBAHOUGH,  FenetrePrincipale::TracerLigneProbaHough)
	EVT_MENU(MENU_CERCLEHOUGH,  FenetrePrincipale::TracerCercleHough)
	EVT_MENU(MENU_BONCOIN,  FenetrePrincipale::TracerBonCoin)
	EVT_MENU(MENU_FLOTOPTIQUE, FenetrePrincipale::TracerFlotOptique)
	EVT_MENU(MENU_REGIONMVT, FenetrePrincipale::TracerRegionMvt)
	EVT_MENU_RANGE(MENU_POINTORB, MENU_POINTKAZE, FenetrePrincipale::TracerDescripteur)

	EVT_MENU(MENU_OP1,  FenetrePrincipale::PrepOperation)
	EVT_MENU(MENU_OP2,  FenetrePrincipale::PrepOperation)
	EVT_MENU(MENU_OP3,  FenetrePrincipale::PrepOperation)
	EVT_MENU(MENU_EXEC_OP,  FenetrePrincipale::PrepOperation)
	EVT_MENU(RESET_OP,  FenetrePrincipale::PrepOperation)
	EVT_MENU(ENREGISTRER_FICHIER,FenetrePrincipale::Enregistrer)
	EVT_MENU(ENREGISTRERSOUS_FICHIER,FenetrePrincipale::EnregistrerSous)
	EVT_MENU(CREER_RAPPORT,FenetrePrincipale::CreerRapport)
	EVT_MENU(COPIER_SELECT,FenetrePrincipale::CopierSelect)
	EVT_MENU(COLLER,FenetrePrincipale::Coller)
	EVT_MENU(EFFACER_SELECT,FenetrePrincipale::EffacerSelect)
	EVT_MENU(COLLER_IMAGE,FenetrePrincipale::CollerImage)
    EVT_MENU(COLLER_IMAGE_CADRER,FenetrePrincipale::CollerImageCadrer)
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


FenetrePrincipale	*wxOsgApp::Graphique(int id)
{
if (id==-1)
	if (nbFenetre>0 && indFenetre>=0)
		{
        if (listeFenetre[indFenetre])
            return listeFenetre[indFenetre]->fPrin;
        return NULL;
        }
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
if (fSeqOpe)
	((FenetreSequenceOperation *)fSeqOpe)->Close(true);
for (int i = 0; i < listeFenetre.size(); i++)
	delete listeFenetre[i];
delete serveur;
if (FenetrePrincipale::pLineaire)
	{
	delete [](FenetrePrincipale::pLineaire);
	delete [](FenetrePrincipale::pLin256);
	delete [](FenetrePrincipale::pLin256Boucle);
	delete [](FenetrePrincipale::pAleatoire);
	delete [](FenetrePrincipale::pJet);
	delete [](FenetrePrincipale::pRainbow);
	delete [](FenetrePrincipale::pRainbow256);
	delete [](FenetrePrincipale::pRainbow256Boucle);
	delete [](FenetrePrincipale::pPerso);
	delete [](FenetrePrincipale::pPersoInv);
	}
#ifdef _DLL_DETECTION__
delete dllplplot;
delete dllWXplplotdrv;
delete dllSVGplplotdrv;
#endif
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
wxString nomFlux=wxEmptyString;
if (type==0)
	{
	wxString dossier;
	configApp->Read("/dossier",&dossier,wxEmptyString);
	wxFileDialog ouverture(NULL, _("Open video file"), dossier, wxEmptyString, "*.tif;*.jpg;*.bmp;*.png;*.yml;*.gz;*.is2;*.mp4;*.avi");

	if (ouverture.ShowModal()!=wxID_OK)
		return;
#ifdef __WINDOWS__
	wxString nomFlux(ouverture.GetDirectory()+"\\"+ouverture.GetFilename ());
#else
	wxString nomFlux(ouverture.GetDirectory()+"/"+ouverture.GetFilename ());
#endif
	f->OuvrirVideo(type,nomFlux);
	}
else
	{
	wxTextEntryDialog  adr( NULL,_("Device address or Ip adress"),"0");   
	adr.ShowModal();
	nomFlux=adr.GetValue();
	f->OuvrirVideo(type,nomFlux);
	}
if (!f->Cam()->Connectee())
	{
	wxCloseEvent evt;

	f->OnClose(evt);
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
/*	imgStatIm->OuvertureOngletDistribRadiale();
	imgStatIm->OuvertureOngletDistribAngulaire();
	imgStatIm->OuvertureOngletFocus();*/
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
	if (CtrlCamera())
		{
		wxString s= "Control :" +f->GetTitle();
		CtrlCamera()->SetTitle(s);
		CtrlCamera()->DefCamera(f->Cam());
		CtrlCamera()->Show(true);
		CtrlCamera()->DefParent(f);
		CtrlCamera()->OuvertureOngletParametresTemporels();

		}

f->ModeCamera(w);
}




void wxOsgApp::Ouvrir(wxCommandEvent &w)
{
wxString dossier;
configApp->Read("/dossier",&dossier,wxEmptyString);

wxFileDialog ouverture(NULL, "Ouvrir ", dossier, wxEmptyString, "*.tif;*.jpg;*.bmp;*.png;*.yml;*.gz;*.is2;*.csv");
if (ouverture.ShowModal()!=wxID_OK)
	return;
configApp->Write("/dossier",ouverture.GetDirectory());
FenetrePrincipale *f = new FenetrePrincipale(NULL, "wxOpenCV",
    posFenetre, wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
posFenetre += wxPoint(20,20);
posFenetre.x=posFenetre.x%500;
posFenetre.y=posFenetre.y%500;
f->DefOSGApp(this);
#ifdef __WINDOWS__
wxString s(ouverture.GetDirectory()+"\\"+ouverture.GetFilename ());
#else
wxString s(ouverture.GetDirectory()+"/"+ouverture.GetFilename ());
#endif
wxString sd(ouverture.GetDirectory());
f->RepertoireDoc(sd);
sd=ouverture.GetFilename ();
f->NomDoc(sd);
f->OnOuvrir(s);
s.Printf("%d : %s",nbFenetre,s);
f->SetLabel(s);
InitFenAssociee(f);
//nbFenetre++;
f->InitIHM();
}

void wxOsgApp::InitFenAssociee(FenetrePrincipale *f)
{
    wxPoint p=f->GetPosition()+wxPoint(f->GetSize().GetWidth(),0);
ImageStatistiques *imgStatIm = new ImageStatistiques(NULL, "Image Statistic",
	p, wxSize(430,570),
	wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
FenetreZoom *fenZoom = new FenetreZoom(f);
wxString s(f->GetTitle());
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
/*	imgStatIm->OuvertureOngletDistribRadiale();
	imgStatIm->OuvertureOngletDistribAngulaire();
	imgStatIm->OuvertureOngletFocus();*/
	}
imgStatIm->OuvertureOngletCouleur();
imgStatIm->OuvertureOngletPalette();
if (f->ImAcq() && f->ImAcq()->StatComposante())
	{
	imgStatIm->OuvertureOngletRegion();
	imgStatIm->ListerRegion();
	}
imgStatIm->OuvertureOngletCurseur();
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES),_("ORB features"));
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES),_("BRISK features"));
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES),_("BLOB features"));
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES),_("AKAZE features"));
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES),_("KAZE features"));
if (f->ImAcq() && f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES)->size()!=0)
    imgStatIm->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES),_("AGAST features"));
listeFenetre[nbFenetre]=new EnvFenetre(f,fenZoom,imgStatIm);
indFenetre=nbFenetre;
f->DefId(nbFenetre++);
f->Show(true);

f->NouvelleImage();
f->MAJNouvelleImage();
f->RecupDerniereConfig();
wxIcon icon;
wxBitmap b(f->ImageAffichee()->Scale(64,64));
icon.CopyFromBitmap(b);
f->SetIcon(icon);

}



void wxOsgApp::DefOperateurImage(wxString &s)
{
ImageInfoCV xx;
pOCV.doubleParam.clear();
pOCV.intParam.clear();
pOCV.sizeParam.clear();
pOCV.pointParam.clear();
if (!pOCV.InitOperation((string)s))
	wxMessageBox(_("Undefined operation"));

}





void ExecuterOperation(wxOsgApp *app,ParametreOperation *pOCVNouveau)
{
ParametreOperation *pAct;
vector<ImageInfoCV*>	r;

if (pOCVNouveau==NULL)
	pAct=app->Operation();
else
	pAct=pOCVNouveau;
if (pAct->operateur)
	{
	try
		{
        cv::ocl::setUseOpenCL(pAct->intParam["opencl_enable"].valeur);

		r =((*pAct->op[0]).*pAct->operateur)(pAct->op,pAct);

		EvtCalculFini *x= new EvtCalculFini(VAL_EVT_CALCUL_FINI);
		x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
        x->r=r;
		wxQueueEvent(app, x);


		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox("An error occured in surjection operator :"+s);
		}
//	app->DefPointeurSouris(0,0);

	}

return ; // Le pointeur imTab n'est pas libéré
}


vector<ImageInfoCV*> wxOsgApp::ExecuterOperation(ParametreOperation *pOCVNouveau, bool noEvt)
{vector<ImageInfoCV*>	r;
 /*
       thread *thOperation= new thread(::ExecuterOperation,this,pOCVNouveau);

    thOperation->detach();
    	DefPointeurSouris(0,0);


    return r;*/

    ParametreOperation *pAct;

if (pOCVNouveau==NULL)
	pAct=&pOCV;
else
	pAct=pOCVNouveau;
if (pAct->operateur)
	{
	try
		{
        cv::ocl::setUseOpenCL(pAct->intParam["opencl_enable"].valeur);

		r =((*pAct->op[0]).*pAct->operateur)(pAct->op,pAct);

        if (!noEvt)
        {
            EvtCalculFini *x= new EvtCalculFini(VAL_EVT_CALCUL_FINI);
		    x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
            x->r=r;
		    wxQueueEvent(this, x);

        }


		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox("An error occured in surjection operator :"+s);
		}
	DefPointeurSouris(0,0);

	}

return r; // Le pointeur imTab n'est pas libéré
}


void wxOsgApp::CreerFenetreOperation()
{
if (pOCV.operateur==NULL  || pOCV.op.size()==0)
	return;

vector<ImageInfoCV*> r=ExecuterOperation();
return;
}



// `Main program' equivalent, creating windows and returning main app frame
bool wxOsgApp::OnInit()
{
cv::ocl::setUseOpenCL(false);

bool b=false;
//b=wxUnsetEnv("PLPLOT_HOME");
//b=wxUnsetEnv("PLPLOT_LIB");
//b=wxUnsetEnv("PLPLOT_DRV_DIR");
//	osg::Image *im=osgDB::readImageFile("F:\\Lib\\OpenSceneGraph-Data-3.0.0\\Images\\reflect.rgb");
	numOpFaite=0;
	numSeqOpe=0;
	quitter=false;
	serveur = NULL;
	fSeqOpe=NULL;
	fenOpe=NULL;
    ctrlCamera=NULL;
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
    if (!locale.AddCatalog("messages"))
    {
        wxLogError(_("Couldn't find/load the 'main' catalog for locale '%s'."),
                   pInfo ? pInfo->GetLocaleName() : _("unknown"));
    }
 /*   if (!locale.AddCatalog("wxIUA"))
    {
        wxLogError(_("Couldn't find/load the 'main' catalog for locale '%s'."),
                   pInfo ? pInfo->GetLocaleName() : _("unknown"));
    }*/
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




//new wxDynamicLibrary(plPlotLibWX);
//new wxDynamicLibrary(wxPlPlotDrv);
//new wxDynamicLibrary(svgPlPlotDrv);

int nbEcran=wxDisplay::GetCount() ;
wxDisplay ecran(0);

wxRect display;
display = ecran.GetGeometry();
display.SetTopRight(display.GetTopRight()-wxPoint(1000,0));

InterfaceAvance *frame = new InterfaceAvance(NULL,
                                 wxID_ANY,
                                 "wxOpenCV",
                                 display.GetTopRight(),
                                 wxSize(800, 800));

wxBitmap bitmap;
bool  m_isPda = (wxSystemSettings::GetScreenType() <= wxSYS_SCREEN_PDA);
if (m_isPda)
    bitmap = wxBitmap(mobile_xpm);

bool ok = m_isPda
        ? bitmap.IsOk()
        : bitmap.LoadFile("splash.png", wxBITMAP_TYPE_PNG);

if (ok)
{
    new wxSplashScreen(bitmap,
        wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
        1000, frame, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSIMPLE_BORDER|wxSTAY_ON_TOP);
}

#if !defined(__WXGTK20__)
    // we don't need it at least on wxGTK with GTK+ 2.12.9
    wxYield();
#endif


Bind(VAL_EVT_CALCUL_FINI, &wxOsgApp::CalculFini,this,wxID_ANY);

frame->DefOSGApp(this);
frame->SetSize(wxSize(800,800));

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
wxLogWarning(_("OpenCV : Build info %s "), cv::getBuildInformation().c_str());
#ifdef _DLL_DETECTION__
if (!dllplplot || !dllplplot->IsLoaded())
	wxLogWarning(_("unable to load plplot"));
if (!dllplplot || !dllWXplplotdrv->IsLoaded())
	wxLogWarning(_("unable to load wxwidgets"));
if (!dllplplot || !dllSVGplplotdrv->IsLoaded())
	wxLogWarning(_("unable to load svg"));
#endif

nbFenetre=0;
indFenetre=-1;
modeSouris = SOURIS_STD;
indPointeurSouris=0;

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
posFenetre = wxPoint(20,20);

return true;
}


void wxOsgApp::CalculFini(EvtCalculFini &w)
{

vector<ImageInfoCV*> r=w.r;
if (r.size()==NULL)
	{
	wxLogWarning("Resultat de l'opération vide");
	wxMessageBox("Resultat de l'opération vide");
	return;
	}
for (int nbres=0;nbres<pOCV.nbImageRes;nbres++)
	{
	FenetrePrincipale *f = new FenetrePrincipale(NULL, pOCV.nomOperation,
		wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
	FenetreZoom *fenZoom = new FenetreZoom(f);
	f->DefOSGApp(this);
	wxString s;
    int ind = -1;
	if (pOCV.nomOperation=="Convolution")
		{
		ind = r[nbres]->IndOpConvolution();
		}
	if (pOCV.nomOperation=="Erosion")
		{
		ind = r[nbres]->IndOpMorphologie();
		}
	if (pOCV.nomOperation=="Dilatation")
		{
		ind = r[nbres]->IndOpMorphologie();
		}
	int idOperation=-1;
	int numEtape=-1;
    if (pOCV.indOpFenetre[0] != -1)
		{
        FenetrePrincipale *f = Fenetre(pOCV.indOpFenetre[0]);
		
		if (f!=NULL)
			{
			if(f->OrigineImage()!=NULL)
				{
				idOperation=f->OrigineImage()->idOperation;
				if (idOperation>=0)
					numEtape=f->OrigineImage()->indEtape+1;
				}
			}
        if (idOperation == -1 && pOCV.indOpFenetre.size()>=2 &&pOCV.indOpFenetre[1] >= 0)
			{
            f = Fenetre(pOCV.indOpFenetre[1]);
			if(f->OrigineImage()!=NULL)
				{
				idOperation=f->OrigineImage()->idOperation;
				if (idOperation>=0)
					numEtape=f->OrigineImage()->indEtape+1;
				}
			}
		}
	if (numEtape==-1)
		numEtape=0;
	if (idOperation==-1)
		idOperation=numOpFaite++;
	if (pOCV.indOpFenetre.size()>=1)
        f->DefHistorique(pOCV.indOpFenetre[0], -1, -1, idOperation, numEtape, pOCV.nomOperation, &pOCV);
	if (pOCV.indOpFenetre.size()>=2)
        f->DefHistorique(pOCV.indOpFenetre[0], pOCV.indOpFenetre[1], -1, idOperation, numEtape, pOCV.nomOperation, &pOCV);
    if (pOCV.indOpFenetre.size() >= 3)
        f->DefHistorique(pOCV.indOpFenetre[0], pOCV.indOpFenetre[1], pOCV.indOpFenetre[2], idOperation, numEtape, pOCV.nomOperation, &pOCV);
	if (ind!=-1)
        s.Printf("%d : %s(operator %d) of image %d ", nbFenetre, pOCV.nomOperation, ind, pOCV.indOpFenetre[0]);
	else
        s.Printf("%d : %s of image %d ", nbFenetre, pOCV.nomOperation, pOCV.indOpFenetre[0]);
	f->SetLabel(s);


	f->AssosierImage(r[nbres]);
    f->DynamiqueAffichage();
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
#ifdef _DLL_DETECTION__
	if (dllplplot->IsLoaded() && dllSVGplplotdrv->IsLoaded())
#endif
		{
		imgStatIm->OuvertureOngletHistogramme();
		imgStatIm->OuvertureOngletCoupe();
/*		imgStatIm->OuvertureOngletDistribRadiale();
		imgStatIm->OuvertureOngletDistribAngulaire();
		imgStatIm->OuvertureOngletFocus();*/
		}
	imgStatIm->OuvertureOngletCouleur();
	imgStatIm->OuvertureOngletPalette();
	if (f->ImAcq() && f->ImAcq()->StatComposante())
		{
		imgStatIm->OuvertureOngletRegion();
		imgStatIm->ListerRegion();
		}
	imgStatIm->OuvertureOngletCurseur();
	f->DefHistorique();
	f->Show(true);

	f->NouvelleImage();
	f->MAJNouvelleImage();

	f->RecupDerniereConfig();
	nbFenetre++;
	f->InitIHM();
	wxCommandEvent evt;
	f->ParamAlgo(evt);
	}
if (pOCV.nbImageRes==0)
	{
    FenetrePrincipale *f = Graphique(pOCV.indOpFenetre[0]);
	if (f)
		{
        f->DynamiqueAffichage();
        f->NouvelleImage();
        f->MAJNouvelleImage();
        wxCommandEvent evt;
		f->ParamAlgo(evt);
		}
    if (pOCV.nomOperation == "orbfeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES),_("ORB features"));
    if (pOCV.nomOperation == "briskfeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES),_("BRISK features"));
    if (pOCV.nomOperation == "blobfeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES),_("BLOB features"));
    if (pOCV.nomOperation == "akazefeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES),_("AKAZE features"));
    if (pOCV.nomOperation == "kazefeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES),_("KAZE features"));
    if (pOCV.nomOperation == "agastfeatures2d")
        listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletKeyPt(f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES),_("AGAST features"));
    if (f->ImAcq()->PtContours()->size() >= 1)
    {
         for (int i=0;i<f->ImAcq()->channels();i++)
         {
             wxString s;
             s.Format("Contour plane %d",i);
             listeFenetre[pOCV.indOpFenetre[0]]->fStat->OuvertureOngletContour((&(*f->ImAcq()->PtContours())[i]),s);
         }

	}
}
pOCV.op.clear();
pOCV.indOpFenetre.clear();
pOCV.doubleParam.clear();
pOCV.intParam.clear();

    return;
}

void wxOsgApp::AnnuleOp()
{
pOCV.InitOperation("");
};


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
	outils = new OutilsImage (NULL, _("Image Tools"),
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


void wxOsgApp::OnUseScreen(wxCommandEvent& WXUNUSED(event))
{
    m_useScreen = !m_useScreen;
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
        cv::Mat m = xx.opnn[idFiltre]->getMat(cv::ACCESS_READ);
		for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
			{
			wxString cle;
			cle.Printf("%d",j);
			configApp->Write(cle,m.at<float>(i,j));
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
// Lecture des opérateurs de convolution commençant par /convolution
// les opérateurs trouvés sont insérés dans le tableau static opnn 
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
        cv::Mat m = xx.opnn[idFiltre]->getMat(cv::ACCESS_RW);
		for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
			{
			cle.Printf("%d",j);
			configApp->Read(cle,&m.at<float>(i,j));
			}
		configApp->SetPath(chemin);
        m.copyTo(*xx.opnn[idFiltre]);
		}
	str=tmp1;
	dummy=tmp2;
	bCont = configApp->GetNextGroup(str, dummy);
	}

// Lecture des opérateurs de morphologie commençant par /morphologie
// les opérateurs trouvés sont insérés dans le tableau static opMorph 
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
			element.copyTo(*xx.opMorph[idFiltre]);
			}
		}
	configApp->SetPath(chemin);
	str=tmp1;
	dummy=tmp2;
	bCont = configApp->GetNextGroup(str, dummy);
	}
// Lecture des opérations prédéfinies commençant par /opérateur
// Chaque opération est définie par son nom, opérande 1, opérande 2 puis les paramètres 
// entiers, réels de l'opération
int nbOperation=0;
configApp->SetPath("/operateur");
wxString	cleIndOperateur;
int nbOpConfig=10;
listeOperation.resize(nbOpConfig);
bCont = configApp->GetFirstGroup(cleIndOperateur, dummy);
bool opValide;
while ( bCont )
	{
	if (nbOperation==nbOpConfig)
		{
		nbOpConfig+=nbOperation;
		listeOperation.resize(nbOpConfig);
		}
	wxString tmp1(cleIndOperateur);
	long tmpDummy=dummy;
	wxString cheminSeq=configApp->GetPath();
// Lecture d'une séquence d'opération
	configApp->SetPath(cheminSeq+"/"+cleIndOperateur);
// Cette séquence est composée d'opérations
	opValide=true;
// Une opération est composée d'un nom et de deux opérateurs
	if(!configApp->Read("idOperation",&listeOperation[nbOperation].idOperation))
		{
		opValide=false;
		}
	wxString valCleChaine;
	if (configApp->Read("nomSequence",&valCleChaine))
		{
		listeOperation[nbOperation].nomSequence=valCleChaine;
		}
	wxString	cleIndEtape;
	bool bSeqOp;
	bSeqOp = configApp->GetFirstGroup(cleIndEtape, dummy);
	int	nbEtape=0;
	while(bSeqOp)
		{
		if (nbOperation>=nbOpConfig)
			{
			nbOpConfig+=10;
			listeOperation.resize(nbOpConfig);
			}
		wxString tmpCle(cleIndEtape);
		long tmp2=dummy;
		wxString chemin=configApp->GetPath();
	// Lecture d'une séquence d'opération
		configApp->SetPath(chemin+"/"+cleIndEtape);
        int val;
        if (opValide && !configApp->Read("op", &valCleChaine))
            opValide = false;
        else
            listeOperation[nbOperation].nomOperation = valCleChaine;
        if (opValide && !configApp->Read("nbOperande", &val))
            {
			opValide = false;
			listeOperation[nbOperation].nbOperande=0;
			}
        else
            listeOperation[nbOperation].nbOperande = val;
        listeOperation[nbOperation].indOpFenetre.resize(listeOperation[nbOperation].nbOperande);
        listeOperation[nbOperation].op.resize(listeOperation[nbOperation].nbOperande);

        for (int i = 0; i < listeOperation[nbOperation].nbOperande; i++)
        {
            string s("op");
            s += to_string(i);
            if (opValide && !configApp->Read(s, &listeOperation[nbOperation].indOpFenetre[0]))
			    opValide=false;
            listeOperation[nbOperation].op[i] = NULL;
        }
		if (opValide && !configApp->Read("res",&listeOperation[nbOperation].indRes))
			opValide=false;
		if (opValide && !configApp->Read("idOperation",&listeOperation[nbOperation].idOperation))
			opValide=false;
		if (opValide && !configApp->Read("indEtape",&listeOperation[nbOperation].indEtape))
			opValide=false;
		if (opValide)
			{
            listeOperation[nbOperation].op.resize(listeOperation[nbOperation].nbOperande);
            for (int i = 0; i < listeOperation[nbOperation].nbOperande; i++)
            {
                listeOperation[nbOperation].op[i] = NULL;
            }
			if (listeOperation[nbOperation].idOperation>numOpFaite)
				numOpFaite =listeOperation[nbOperation].idOperation;
			wxString cheminParam(chemin+"/"+cleIndEtape+"/paramEntier");
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
					listeOperation[nbOperation].intParam[s]=DomaineParametreOp<int>(val,minVal,maxVal,pasVal);
					}
				bCont = configApp->GetNextGroup(str, dummy);
				}
			}
		if (opValide)
			{
			wxString cheminParam(chemin+"/"+cleIndEtape+"/paramDouble");
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
					listeOperation[nbOperation].doubleParam[s]=DomaineParametreOp<double>(val,minVal,maxVal,pasVal);
					}
				bCont = configApp->GetNextGroup(str, dummy);
				}
			
			}
		if (opValide)
			{
			wxString cheminParam(chemin+"/"+cleIndEtape+"/paramSize");
			configApp->SetPath(cheminParam);
			bCont = configApp->GetFirstGroup(str, dummy);
			while (bCont && opValide)
				{
				wxString cle(cheminParam+"/"+str+"/nom");
				wxString nom;
				if (opValide && !configApp->Read(cle,&nom))
					opValide=false;
				cle=cheminParam+"/"+str+"/largeur";
				int largeur,larMin,larMax,larPas;
				if (opValide && !configApp->Read(cle,&largeur))
					opValide=false;
				int hauteur,hauMin,hauMax,hauPas;
				cle=cheminParam+"/"+str+"/hauteur";
				if (opValide && !configApp->Read(cle,&hauteur))
					opValide=false;
				cle=cheminParam+"/"+str+"/larMin";
				if (opValide && !configApp->Read(cle,&larMin))
					opValide=false;
				cle=cheminParam+"/"+str+"/hauMin";
				if (opValide && !configApp->Read(cle,&hauMin))
					opValide=false;
				cle=cheminParam+"/"+str+"/larMax";
				if (opValide && !configApp->Read(cle,&larMax))
					opValide=false;
				cle=cheminParam+"/"+str+"/hauMax";
				if (opValide && !configApp->Read(cle,&hauMax))
					opValide=false;
				cle=cheminParam+"/"+str+"/larPas";
				if (opValide && !configApp->Read(cle,&larPas))
					opValide=false;
				cle=cheminParam+"/"+str+"/hauPas";
				if (opValide && !configApp->Read(cle,&hauPas))
					opValide=false;
				if (opValide)
					{
					std::string s(nom.ToAscii());
					listeOperation[nbOperation].sizeParam[s]=DomaineParametreOp<cv::Size>(cv::Size(largeur,hauteur),cv::Size(larMin,hauMin),cv::Size(larMax,hauMax),cv::Size(larPas,larPas));
					}
				bCont = configApp->GetNextGroup(str, dummy);
				}
			
			}
		if (opValide)
		{
			wxString cheminParam(chemin + "/" + cleIndEtape + "/paramPoint");
			configApp->SetPath(cheminParam);
			bCont = configApp->GetFirstGroup(str, dummy);
			while (bCont && opValide)
			{
				wxString cle(cheminParam + "/" + str + "/nom");
				wxString nom;
				if (opValide && !configApp->Read(cle, &nom))
					opValide = false;
				cle = cheminParam + "/" + str + "/largeur";
				int largeur, larMin, larMax, larPas;
				if (opValide && !configApp->Read(cle, &largeur))
					opValide = false;
				int hauteur, hauMin, hauMax, hauPas;
				cle = cheminParam + "/" + str + "/hauteur";
				if (opValide && !configApp->Read(cle, &hauteur))
					opValide = false;
				cle = cheminParam + "/" + str + "/larMin";
				if (opValide && !configApp->Read(cle, &larMin))
					opValide = false;
				cle = cheminParam + "/" + str + "/hauMin";
				if (opValide && !configApp->Read(cle, &hauMin))
					opValide = false;
				cle = cheminParam + "/" + str + "/larMax";
				if (opValide && !configApp->Read(cle, &larMax))
					opValide = false;
				cle = cheminParam + "/" + str + "/hauMax";
				if (opValide && !configApp->Read(cle, &hauMax))
					opValide = false;
				cle = cheminParam + "/" + str + "/larPas";
				if (opValide && !configApp->Read(cle, &larPas))
					opValide = false;
				cle = cheminParam + "/" + str + "/hauPas";
				if (opValide && !configApp->Read(cle, &hauPas))
					opValide = false;
				if (opValide)
				{
					std::string s(nom.ToAscii());
					listeOperation[nbOperation].pointParam[s] = DomaineParametreOp<cv::Point>(cv::Point(largeur, hauteur), cv::Point(larMin, hauMin), cv::Point(larMax, hauMax), cv::Point(larPas, larPas));
				}
				bCont = configApp->GetNextGroup(str, dummy);
			}

		}
		if (opValide)
			{
			GenerationGraphDot(&listeOperation[nbOperation]);
			nbOperation++;
			nbEtape++;
			}
		configApp->SetPath(chemin);
		str=tmp1;
		dummy=tmp2;
		bSeqOp = configApp->GetNextGroup(cleIndEtape, dummy);

		}
	numSeqOpe++;
	if (numSeqOpe>=tabOperation.size())
		{
		cout <<"pb";

		}

	tabOperation[listeOperation[nbOperation-1].idOperation].resize(nbEtape);
	for (int i=0;i<nbEtape;i++)
		{
		listeOperation[nbOperation-nbEtape+i].InitPtrFonction();
		tabOperation[listeOperation[nbOperation-1].idOperation][i]=listeOperation[nbOperation-nbEtape+i];
		}
		
	configApp->SetPath(cheminSeq);
	str=tmp1;
	dummy=tmpDummy;
	bCont = configApp->GetNextGroup(cleIndOperateur, dummy);
	}
if (numOpFaite>=0)
	numOpFaite++;
}

void wxOsgApp::GenerationGraphDot(ParametreOperation *op)
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

void wxOsgApp::SauverOperationFichierConfig(ParametreOperation &origineImage)
{
wxString param;
wxString chemin;
wxString chaine;
chemin.Printf("/operateur/%d/",origineImage.idOperation);
SauverFichierConfig(chemin,"idOperation",(long)origineImage.idOperation);
chemin.Printf("/operateur/%d/%d/",origineImage.idOperation,origineImage.indEtape);
SauverFichierConfig(chemin, "op", origineImage.nomOperation);
SauverFichierConfig(chemin, "nbOperande",(long) origineImage.nbOperande);
SauverFichierConfig(chemin,"op",origineImage.nomOperation);
for (int i = 0; i < origineImage.nbOperande; i++)
{
    string s("op");
    s += to_string(i);
    SauverFichierConfig(chemin, s, (long)origineImage.indOpFenetre[i]);
}
SauverFichierConfig(chemin,"res",(long)origineImage.indRes);
SauverFichierConfig(chemin,"indEtape",(long)origineImage.indEtape);
SauverFichierConfig(chemin,"idOperation",(long)origineImage.idOperation);
SauverFichierConfig(chemin,"nomSequence",origineImage.nomSequence);
std::map<std::string,DomaineParametreOp<int> >::iterator iti;
int nb=0;
for (iti=origineImage.intParam.begin();iti!=origineImage.intParam.end();iti++)
	{
	chemin.Printf("/operateur/%d/%d/paramEntier/%d",origineImage.idOperation,origineImage.indEtape,nb);
	SauverFichierConfig(chemin,"nom",iti->first);
	SauverFichierConfig(chemin,"valeur",(long)iti->second.valeur);
	SauverFichierConfig(chemin,"minVal",(long)iti->second.mini);
	SauverFichierConfig(chemin,"maxVal",(long)iti->second.maxi);
	SauverFichierConfig(chemin,"pasVal",(long)iti->second.pas);
	nb++;
	}
nb=0;
std::map<std::string,DomaineParametreOp<double> >::iterator itd;
for (itd=origineImage.doubleParam.begin();itd!=origineImage.doubleParam.end();itd++)
	{
	chemin.Printf("/operateur/%d/%d/paramDouble/%d",origineImage.idOperation,origineImage.indEtape,nb);
	SauverFichierConfig(chemin,"nom",itd->first);
	SauverFichierConfig(chemin,"valeur",itd->second.valeur);
	SauverFichierConfig(chemin,"minVal",itd->second.mini);
	SauverFichierConfig(chemin,"maxVal",itd->second.maxi);
	SauverFichierConfig(chemin,"pasVal",itd->second.pas);
	nb++;
	}
nb=0;
std::map<std::string, DomaineParametreOp<cv::Size> >::iterator its;
for (its = origineImage.sizeParam.begin(); its != origineImage.sizeParam.end(); its++)
{
	chemin.Printf("/operateur/%d/%d/paramSize/%d", origineImage.idOperation, origineImage.indEtape, nb);
	SauverFichierConfig(chemin, "nom", its->first);
	SauverFichierConfig(chemin, "largeur", (long)its->second.valeur.width);
	SauverFichierConfig(chemin, "hauteur", (long)its->second.valeur.height);
	SauverFichierConfig(chemin, "larMin", (long)its->second.mini.width);
	SauverFichierConfig(chemin, "hauMin", (long)its->second.mini.height);
	SauverFichierConfig(chemin, "larMax", (long)its->second.maxi.width);
	SauverFichierConfig(chemin, "hauMax", (long)its->second.maxi.height);
	SauverFichierConfig(chemin, "larPas", (long)its->second.pas.width);
	SauverFichierConfig(chemin, "hauPas", (long)its->second.pas.height);
	nb++;
}
std::map<std::string, DomaineParametreOp<cv::Point> >::iterator itp;
for (itp = origineImage.pointParam.begin(); itp != origineImage.pointParam.end(); itp++)
{
	chemin.Printf("/operateur/%d/%d/paramPoint/%d", origineImage.idOperation, origineImage.indEtape, nb);
	SauverFichierConfig(chemin, "nom", itp->first);
	SauverFichierConfig(chemin, "largeur", (long)itp->second.valeur.x);
	SauverFichierConfig(chemin, "hauteur", (long)itp->second.valeur.y);
	SauverFichierConfig(chemin, "larMin", (long)itp->second.mini.x);
	SauverFichierConfig(chemin, "hauMin", (long)itp->second.mini.y);
	SauverFichierConfig(chemin, "larMax", (long)itp->second.maxi.x);
	SauverFichierConfig(chemin, "hauMax", (long)itp->second.maxi.y);
	SauverFichierConfig(chemin, "larPas", (long)itp->second.pas.x);
	SauverFichierConfig(chemin, "hauPas", (long)itp->second.pas.y);
	nb++;
}
}

void  wxOsgApp::DefOperande1(ImageInfoCV* im,int i)
{
    if (pOCV.indOpFenetre.size()<=0)
    {
       pOCV.op.push_back(im); 
       pOCV.indOpFenetre.push_back(i); 
    }
    else
    {
        pOCV.op[0]=im;
        pOCV.indOpFenetre[0]=i;
    }
}

void  wxOsgApp::DefOperande2(ImageInfoCV* im, int i)
{ 
    if (pOCV.indOpFenetre.size()<=1)
    {
       pOCV.op.push_back(im); 
       pOCV.indOpFenetre.push_back(i); 
    }
    else
    {
        pOCV.op[1]=im;
        pOCV.indOpFenetre[1]=i;
    }
}
void  wxOsgApp::DefOperande3(ImageInfoCV* im, int i)
{ 
    if (pOCV.indOpFenetre.size()<=2)
    {
       pOCV.op.push_back(im); 
       pOCV.indOpFenetre.push_back(i); 
    }
    else
    {
        pOCV.op[2]=im;
        pOCV.indOpFenetre[2]=i;
    }
}
void  wxOsgApp::DefOperandeN(ImageInfoCV* im, int i)
{ 
    pOCV.op.push_back(im); 
    pOCV.indOpFenetre.push_back(i); 
}

void  wxOsgApp::SupOperandeN(ImageInfoCV* im, int i)
{ 
}

ImageInfoCV * wxOsgApp::OpId(int id)
{
if (pOCV.op.size()>id)
    return pOCV.op[id];
return NULL;
};

int wxOsgApp::IndOpId(int id)
{ 
if (pOCV.op.size()>id)
    return pOCV.indOpFenetre[id];
return -1;
};

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


