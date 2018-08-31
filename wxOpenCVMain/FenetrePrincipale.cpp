#include "FenetrePrincipale.h"
#include "ControleCamera.h"
#include "imagestat.h"
#include "FenetreSeqOpe.h"
#include "CameraOpenCV.h"
#include "wxOpencvApp.h"
#include "FenetreAlgo.h"
#include "GrapheOperation.h"
#include <wx/display.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <strstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include "wxOpenCVConfig.h"
#include "VideoCourbe.h"

using namespace std;


/* My frame constructor */
FenetrePrincipale::FenetrePrincipale(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title , pos, size, style/*|wxVSCROLL |wxHSCROLL*/)
{
tpsPreEvt=-1;
nbImageCam=0;
FenetrePrincipale::nbObjetFenetrePrincipale++;
imGain=NULL;
correctionGain=false;
tracerContour=false;
tracerEnveloppe=false;
tracerDefautEnveloppe=false;
tracerLigneHough=false;
tracerLigneProbaHough=false;
tracerCercleHough=false;
tracerBonCoin=false;
tracerORBPoint = false;
tracerSURFPoint = false;
tracerSIFTPoint = false;
tracerBRISKPoint = false;
tracerBLOBPoint = false;
tracerKAZEPoint = false;
tracerAKAZEPoint = false;
tracerKAZEPoint = false;
tracerAGASTPoint = false;
tracerFREAKPoint = false;
tracerMSERPoint = false;
tracerDNNCaffe = false;
tracerDNNYolo = false;
tracerDNNTensor = false;
tracerContourPoly=false;
imgStatIm = NULL;
indEvtCam=0;
for (int i=0;i<10;i++)
	planActif[i]=true;
osgApp=NULL;
zoomActif=false;
statActif=false;
fenetreSauvee =0;
RAZGrapheOperation();
fenPano = NULL;
courbeVideo=NULL;
idFenetre=-1;




//detectionUtilisateur =  new wxTimer(this,2);
detectionUtilisateur=NULL;
repertoireDuDocument=".";
nomDuDocument=wxEmptyString;
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
indSeqTabApp=-1;
pCouleur=NULL;
horlogeSeq=NULL;
imAcq = NULL;
poly=NULL;
dateSeq=NULL;
correctionFonction=0;
correctionTache=0;
correctionBiais=0;
nbImageTache=0;
SetBackgroundColour(*wxWHITE);
// Association barre des menus avec la trame

SetIcon(wxIcon("wxocv.bmp",wxBITMAP_TYPE_ICO ));
DefCurseur(5);

cam=NULL;

imAcq=NULL;
imAffichee=NULL;
imAcq = NULL;
indPalette=0;
nbImageBiais=0;
nbImageTache=0;
nbImageFonction=0;
dImageBiais=0;
dImageTache=0;

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
if (detectionUtilisateur)
	detectionUtilisateur->Start(TPSMISEENVEILLECOURBE,true);
}



ImageInfoCV *FenetrePrincipale::ImageOp1pre()
{
FenetrePrincipale *f = osgApp->Fenetre(origineImage.indOpFenetre[0]);
if (f)
	return f->ImAcq();
return NULL;

}

ImageInfoCV *FenetrePrincipale::ImageOp2pre()
{
FenetrePrincipale *f = osgApp->Fenetre(origineImage.indOpFenetre[0]);
if (f)
	return f->ImAcq();
return NULL;

}


int FenetrePrincipale::IdFenetreOp1pre()
{
if (origineImage.op.size()>=1)
    return origineImage.indOpFenetre[0];
return -1;
};


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



void FenetrePrincipale::DefCurseur(int r,int xc,int yc)
{
   static char down_bits[128];

    static char down_mask[128] = { char(240), char(1), char(0), char(0), char(240), char(1),
        char(0), char(0), char(240), char( 1), char(0), char(0), char(240), char(1), char(0), char(0),
        char(240), char( 1), char( 0), char( 0), char( 240), char(1),
        char(0), char(0), char(240), char(1), char(0),
        char(0), char(240), char(1), char(0), char(0), char(255), char(31), char(0), char(0), char(255),
        char(31), char(0), char(0), char(254), char(15), char(0), char(0), char(252), char(7),
        char( 0), char(0), char(248), char(3), char(0), char(0),
        char(240), char(1), char(0), char(0), char(224), 0, 0, 0, char(64), 0, 0, 0, 0, 0, 0, 0, char(0),
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, char(0),
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, char(0),
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, char(0),
        0, 0, 0, 0, char(0) };
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
    wxIcon icon;
    wxBitmap b(ImageAffichee()->Scale(64, 64));
    icon.CopyFromBitmap(b);
    SetIcon(icon);

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
	tpsPreEvt=-1;
    if (imgStatIm->JamaisVue())
    {
        wxPoint p=GetPosition()+wxPoint(GetSize().GetWidth(),0);
        imgStatIm->SetPosition(p);

    }
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

void FenetrePrincipale::InitImageFenetre()
{
cam =NULL;
imAcq=NULL;
imAffichee=NULL;
imGain=NULL;

imAcq = new ImageInfoCV(1002,1004,CV_16UC1);
for (int i=0;i<imAcq->channels();i++)
	{
	seuilNivBas.push_back(0);
	coeffCanal.push_back(1.);
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

void FenetrePrincipale::OuvrirVideo(int type,wxString nomFlux)
{
if (type==0)
	{
	cam = new  CameraOpenCV(nomFlux);

	imAcq = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
	cam->DefTypeAcq(CV_8UC3);
	}
else
	cam = new  CameraOpenCV(nomFlux);
if (!cam->Connectee())
	{
	wxMessageBox(_("Error : stream cannot be opened"));
	return;
	}
courbeVideo = new FenetreCourbe(this);

barreEtat->ActiveVideo();
correctionGain=false;
fenetreSauvee=1;
//ImageInfoCV *imtmp=new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
wxString userName=wxGetUserName();

imGain = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),CV_32FC3);
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




wxLogVerbose("Video opened");
for (int i=0;i<imAcq->channels();i++)
	{
	seuilNivBas.push_back(0);
	coeffCanal.push_back(1.);
	}
wxSize	tailleZoneImage;
tailleZoneImage=GetClientSize();

}

/**
Ouvrir un flux video
*/

void FenetrePrincipale::ChgtTailleVideo(int type)
{
if (!cam)
	return;
if (type==0)
	{
	if (imAcq)
		delete imAcq;

	imAcq = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
	((CameraOpenCV*)cam)->imAcq=imAcq;

	}
barreEtat->ActiveVideo();
correctionGain=false;
fenetreSauvee=1;
//ImageInfoCV *imtmp=new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
wxString userName=wxGetUserName();
delete imGain;
imGain = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),CV_32FC3);
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

if (imAffichee)
	{
	delete imAffichee;
	}

imAffichee=NULL;

wxLogVerbose("Size Video changed");

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
//wxImage im(s);
if (s.Find("yml")>=0)
	{
	try
		{
		imAcq =new ImageInfoCV(nomFichier);
/*        imAcq->read()
		cv::FileStorage fs(nomFichier, cv::FileStorage::READ);
		fs["Image"]>>*((cv::UMat*)imAcq);
		fs.release();*/
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		wxMessageBox(_("An error occured while saving file :")+s);
		}
	}
else if (s.Find(".is2")>=0 ||s.Find(".IS2")>=0)
	{
/*	imAcq =new ImageInfoCV(480,640,CV_8UC3);
	ifstream fs;
	fs.open(nomFichier,ios::binary);
	unsigned short *tmp=new unsigned short[640*480*2];
	fs.seekg(169642);
	fs.read((char*)tmp,640*480*2);
	fs.close();
	for (int i=0;i<480;i++)
		for (int j=0;j<640;j++)
			{
			char r,g,b;
			r=(tmp[i*640+j]&0xF800)>>8;
			g=(tmp[i*640+j]&0x07D0)>>3;
			b=(tmp[i*640+j]&0x001F)<<3;
			cv::Vec3b v(b,g,r);
			imAcq->at< cv::Vec3b >(i,j)=v;
			}
	*/
	imAcq =new ImageInfoCV(240,320,CV_16UC1);
	ifstream fs;
	float t[10];
	fs.open(nomFichier,ios::binary);
	unsigned short *tmp=new unsigned short[320*240*2];
	fs.seekg(5880);
	fs.read((char*)t,10*4);
	fs.seekg(5882);
	fs.read((char*)t,10*4);
	fs.seekg(15986);
	fs.read((char*)tmp,320*240*2);

	float *t1=new float[4000];
	float *t2=new float[4000];
	int *i1=new int[4000];
	int *i2=new int[4000];
	fs.seekg(0);
	fs.read((char*)t1,4000*4);
	fs.seekg(0);
	fs.read((char*)i1,4000*4);
	fs.seekg(2);
	fs.read((char*)t2,4000*4);
	fs.seekg(2);
	fs.read((char*)i2,4000*4);

	fs.close();
	ofstream fsr;
	fsr.open("float.txt");

	for (int i=0;i<4000;i++)
		fsr<<t1[i]<<"\t"<<t2[i]<<"\t"<<i1[i]<<"\t"<<i2[i]<<"\n";
    cv::Mat m(240,320,CV_16S);
    for (int i=0;i<240;i++)
		for (int j=0;j<320;j++)
			{
			m.at< unsigned short >(i,j)=tmp[i*320+j];
			}
     m.copyTo(*imAcq);

	}
else if (s.Find(".16b")>=0 ||s.Find(".16B")>=0)
	{
/*	imAcq =new ImageInfoCV(480,640,CV_8UC3);
	ifstream fs;
	fs.open(nomFichier,ios::binary);
	unsigned short *tmp=new unsigned short[640*480*2];
	fs.seekg(169642);
	fs.read((char*)tmp,640*480*2);
	fs.close();
	for (int i=0;i<480;i++)
		for (int j=0;j<640;j++)
			{
			char r,g,b;
			r=(tmp[i*640+j]&0xF800)>>8;
			g=(tmp[i*640+j]&0x07D0)>>3;
			b=(tmp[i*640+j]&0x001F)<<3;
			cv::Vec3b v(b,g,r);
			imAcq->at< cv::Vec3b >(i,j)=v;
			}
	*/
	imAcq =new ImageInfoCV(256,256,CV_16SC1);
	ifstream fs;
	float t[10];
	fs.open(nomFichier,ios::binary);
	short *tmp=new short[256*256];
	fs.seekg(256);
	fs.read((char*)tmp,256*256*2);

	fs.close();

    cv::Mat m(256,256,CV_16S);
	for (int i=0;i<256;i++)
		for (int j=0;j<256;j++)
			{
			m.at< short >(i,j)=tmp[i*256+j];
			}
     m.copyTo(*imAcq);

	delete tmp;

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
for (int i=0;i<imAcq->channels();i++)
	{
	int z=imAcq->depth();
	if (z==2)
		imAcq->flags=(imAcq->flags&0xFFFFFFF8)|CV_16SC1;
	seuilNivBas.push_back(0);
	coeffCanal.push_back(1);
	}
feuille = new ZoneImage(this,wxSize(imAcq->cols/2, imAcq->rows/2));
feuille->DefFenetrePrincipale(this);
feuille->DefOSGApp(osgApp);
feuille->Show();
if (imAffichee)
	{
	delete imAffichee;
	}
imAffichee=NULL;
if (imAcq->type()==CV_16UC1 && (*(imAcq->MaxIm()))[0]<32767.)
	imAcq->flags=(imAcq->flags&0xFFFFFFF0)|CV_16SC1;
int		fZoomNume,fZoomDeno;
int nbEcran=wxDisplay::GetCount() ;
wxDisplay ecran(0);
wxRect display;
display = ecran.GetGeometry();
int n=1;
wxRect r;
do {
    n--;
    feuille->FacteurZoom(n);
    CalculZoom(fZoomNume,fZoomDeno);
    r =wxRect(wxSize((imAcq->cols*fZoomNume)/fZoomDeno,(imAcq->rows*fZoomNume)/fZoomDeno)+wxSize(5,5));
    }
while (!display.Contains(r) &&n>-3);
feuille->FacteurZoom(n);
CalculZoom(fZoomNume,fZoomDeno);
wxSize sa(wxSize((imAcq->cols*fZoomNume)/fZoomDeno,(imAcq->rows*fZoomNume)/fZoomDeno)+wxSize(5,5));
SetClientSize(sa);
feuille->SetVirtualSize(wxSize((imAcq->cols*fZoomNume)/fZoomDeno, (imAcq->rows*fZoomNume)/fZoomDeno));


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
		delete imAffichee;
		}
	imAffichee=NULL;

	for (int i=0;i<imAcq->channels();i++)
		{
		int z=imAcq->depth();
		if (z==2)
			imAcq->flags=(imAcq->flags&0xFFFFFFF8)|CV_16SC1;
		seuilNivBas.push_back(0);
		coeffCanal.push_back(1.);
		}

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
    int		fZoomNume,fZoomDeno;
    int nbEcran=wxDisplay::GetCount() ;
    wxDisplay ecran(0);
    wxRect display;
    display = ecran.GetGeometry();
    int n=1;
    wxRect r;
    do {
        n--;
        feuille->FacteurZoom(n);
        CalculZoom(fZoomNume,fZoomDeno);
        r =wxRect(wxSize((imAcq->cols*fZoomNume)/fZoomDeno,(imAcq->rows*fZoomNume)/fZoomDeno)+wxSize(5,5));
        }
    while (!display.Contains(r) &&n>-3);
    feuille->FacteurZoom(n);
    CalculZoom(fZoomNume,fZoomDeno);
    wxSize sa(wxSize((imAcq->cols*fZoomNume)/fZoomDeno,(imAcq->rows*fZoomNume)/fZoomDeno)+wxSize(5,5));
    SetClientSize(sa);
    feuille->SetVirtualSize(wxSize((imAcq->cols*fZoomNume)/fZoomDeno, (imAcq->rows*fZoomNume)/fZoomDeno));
	}




delete(feuille->BitmapAffichee());
feuille->BitmapAffichee(NULL);
feuille->Update();
if (imgStatIm && ImAcq() && ImAcq()->StatComposante())
	imgStatIm->ListerRegion();
}

void FenetrePrincipale::DynamiqueAffichage(char type)
{
    imAcq->ExtremumLoc();
    for (int i = 0; i < imAcq->channels() && i < 3; i++)
    {
		seuilNivBas[i]=(*(imAcq->MinIm()))[i];
        if ((*(imAcq->MaxIm()))[i]-(*(imAcq->MinIm()))[i]!=0)
		    coeffCanal[i]=256/((*(imAcq->MaxIm()))[i]-(*(imAcq->MinIm()))[i]);
        else
            coeffCanal[i]=0;

    }


}


/**
Ouvrir le fichier d'horodatage
*/
void FenetrePrincipale::OnOuvrirHorodatage(wxCommandEvent& event)
{
wxFileDialog ouverture(this, _("Open Timestamp"), _("Seq"), _("Seq"), _T("*.csv"));
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
	if (wxMessageBox("File already exist. Overwrite it?", "Warning", wxYES_NO))
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
//		((CameraOpenCV*)cam)->FermerVideo();
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

feuille->Refresh();
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
    if (fenOperation)
        fenOperation->DefFenMere(NULL);
    if (courbeVideo)
        courbeVideo->DefFenMere(NULL);
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
if (osgApp && !osgApp->Quitter())
	{
    bool b = false;
    if (osgApp->ConfirmFermeturefenetre())
    {
        wxMessageDialog w(this, _T("Close window"), _T("Quit"), wxYES_NO | wxCENTRE | wxSTAY_ON_TOP);
        b = w.ShowModal() == wxID_YES;
        w.Close();
		if (b && fenetreSauvee==0)
			{
			wxMessageDialog w(this, _T("Do you want to save it?") , _T("Quit"), wxYES_NO|wxCENTRE|wxSTAY_ON_TOP);
			if (w.ShowModal()==wxID_YES)
				{
				wxCommandEvent evt;
				Enregistrer(evt);
				}
			}
	    else
		    {
		    event.Veto();
		    return;
		    }
    }
	}
if (cam!=NULL )
	{
	if (osgApp && osgApp->CtrlCamera()  && osgApp->CtrlCamera()->Camera()==cam)
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
if (fenOperation)
{
    fenOperation->DefFenMere(NULL);
    fenOperation->OnClose(event);
}
if (courbeVideo)
    courbeVideo->DefFenMere(NULL);
if (osgApp && !osgApp->Quitter())
	osgApp->RetirerListe(this);
if (cam)
	OnCloseThread(event);
if (detectionUtilisateur)
	detectionUtilisateur->Stop();
delete detectionUtilisateur;

delete imAcq;
delete imGain;


delete imAffichee;
// est déjà fait par delete imAffichee delete tabRGB;

imAcq=NULL;
imGain=NULL;
imAffichee=NULL;
nbObjetFenetrePrincipale--;
wxFrame::OnCloseWindow(event);
}

void FenetrePrincipale::EnregistrerImage()
{
string	nomFichier;

wxString s=repertoireDuDocument+_T("\\")+nomDuDocument;
wxFileName p(s);

nomFichier = s.c_str();

vector<int> compression_params;

if (p.GetExt().Cmp("yml")==0)
	{
	cv::FileStorage fs(nomFichier, cv::FileStorage::WRITE);
	imAcq->write(fs);
/*
	fs<<"Image"<<*((cv::UMat*)imAcq);
	if (imAcq->StatComposante())
	{
	for (int i=0;i<imAcq->channels();i++)
		{
		wxString s;
		s.Printf("StatComposante%d",i);
		wxCharBuffer ww=s.mb_str ();
		char *nomChamp=ww.data() ;
		fs<<nomChamp<<*((cv::UMat*)(imAcq->StatComposante()[i]));
		}
	}
	if (imAcq->CentreGComposante())
		{
		for (int i=0;i<imAcq->channels();i++)
			{
			wxString s;
			s.Printf("CentreGComposante%d",i);
			wxCharBuffer ww=s.mb_str ();
			char *nomChamp=ww.data() ;
			fs<<nomChamp<<*((cv::UMat*)(imAcq->CentreGComposante()[i]));
			}
		}
	if (imAcq->MomentComposante())
		{
		vector<cv::Moments> *m=imAcq->MomentComposante();
		for (int i=0;i<imAcq->channels();i++)
			{
			wxString s;
			s.Printf("Moment%d",i);
			wxCharBuffer ww=s.mb_str ();
			char *nomChamp=ww.data() ;
			fs<<nomChamp<<m[i];
			}
		}
	if (imAcq->PtContours())
		{
		std::vector<std::vector<cv::Point> > *ptCtr=imAcq->PtContours();
		for (int i=0;i<imAcq->channels()&& i<3;i++)
			{
			vector<vector<cv::Point> >::iterator itCtr;
			int indCtr=0;
			for (itCtr=ptCtr[i].begin();itCtr!=ptCtr[i].end();itCtr++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"Ctr"<<indCtr;
				string s(nombre.str());

				fs<<s<<*itCtr;
				indCtr++;
				}
			}

		}*/
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

return;

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
case MENU_OP3 :
	if (osgApp->Op3()==imAcq)
		osgApp->DefOperande3(NULL);
	else
		osgApp->DefOperande3(imAcq,idFenetre);
	break;
case MENU_EXEC_OP:
	{
    if (Cam() && Cam()->IsRunning())
	{
        int indEtape=max(origineImage.indEtape,imAcq->EtapeOp());
	    osgApp->DefEtapeOperation(indEtape+1);
        std::map <int,std::vector <ParametreOperation > >  *p=osgApp->TabSeqOperation();
        if (indSeqTabApp==-1)
        {
            indSeqTabApp=p->size();
            p->insert(make_pair(indSeqTabApp,seqOp));
        }
        seqOp.push_back(*osgApp->Operation());
        (*p)[indSeqTabApp].push_back(*osgApp->Operation());
	    wxCriticalSectionLocker enter(paramCam);
	    DefSeqCamera(&seqOp);
        osgApp->RAZOp();
        osgApp->DefPointeurSouris(0,0);
	}
    else
    {
        int indEtape=max(origineImage.indEtape,imAcq->EtapeOp());
	    osgApp->DefEtapeOperation(indEtape+1);
	    osgApp->CreerFenetreOperation();
    }
	}
	break;
case RESET_OP :
    {
	    wxMessageDialog w(this, "Do you want to cancel operation?" , _T("Cancel"), wxYES_NO|wxCENTRE|wxSTAY_ON_TOP);
	    if (w.ShowModal()==wxID_YES)
		    {
		    osgApp->AnnuleOp();
		    osgApp->DefPointeurSouris(0,0);

		    }

    }
    break;
case MENU_SET_NEW_OP:
    {
        ArboCalcul *arbre = (ArboCalcul*)osgApp->ArboCalcul();
        if (!arbre)
            break;
        wxTreeItemId id = osgApp->NoeudCalcul();
        arbre->ModifNoeud(this, id);
        osgApp->DefPointeurSouris(0, 0);

    }
    break;
	}
}

void FenetrePrincipale::DefHistorique(int ind1,int ind2,int ind3,int idOpe,int numE ,wxString nomF,ParametreOperation *pOCV) /*< Associer origine à une image */
{
if (ind1!=-1)
	{
	if (pOCV)
		origineImage=*pOCV;
    origineImage.indOpFenetre.push_back(ind1);
    origineImage.indOpFenetre.push_back(ind2);
    origineImage.indOpFenetre.push_back(ind3);
	origineImage.indRes=idFenetre;
	origineImage.idOperation=idOpe;
	origineImage.indEtape=numE;
	origineImage.op[0] = osgApp->Fenetre(ind1)->ImAcq();
	if (ind2>=0)
		origineImage.op[1] = osgApp->Fenetre(ind2)->ImAcq();
	if (ind3>=0)
		origineImage.op[2] = osgApp->Fenetre(ind3)->ImAcq();
	origineImage.nomOperation=nomF.c_str();
	}
}

wxMenu *FenetrePrincipale::CreateMenuPalette(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(NOIRETBLANC_, _("&Linear\tCtrl-F1"));
    menu->Append(NOIRETBLANC_ + 1, _("&autumn\t"));
    menu->Append(NOIRETBLANC_ + 2, _("&bone\t"));
    menu->Append(NOIRETBLANC_ + 3, _("&jet\t"));
    menu->Append(NOIRETBLANC_ + 4, _("&winter\t"));
    menu->Append(NOIRETBLANC_ + 5, _("&rainbow\t"));
    menu->Append(NOIRETBLANC_ + 6, _("&ocean\t"));
    menu->Append(NOIRETBLANC_ + 7, _("&summer\t"));
    menu->Append(NOIRETBLANC_ + 8, _("&spring\t"));
    menu->Append(NOIRETBLANC_ + 9, _("&cool\t"));
    menu->Append(NOIRETBLANC_ + 10, _("&hsv\t"));
    menu->Append(NOIRETBLANC_ + 11, _("&pink\t"));
    menu->Append(NOIRETBLANC_ + 12, _("&hot\t"));
    menu->Append(NOIRETBLANC_ + 13, _("&parula\t"));
    menu->Append(NOIRETBLANC_ + 14, _("&randomize\t"));

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
    wxColour	*p = NULL;
indPalette=event.GetId()-NOIRETBLANC_;
pCouleur = paletteDispo[indPalette];

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
    if (!feuille)
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
	wxFileDialog ouverture(this, _("Save As"), rep,nomImageBiais , "*.tif",wxFD_SAVE );
	if (ouverture.ShowModal()!=wxID_OK)
		return;
	nomImageBiais = ouverture.GetFilename();
	rep = ouverture.GetDirectory();
	nomImageTache=rep+"\\"+nomImageBiais;
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
if (!::wxDirExists("\\Images"))
	::wxMkdir("\\Images");
if (!::wxDirExists("\\Images\\BiasBackground"))
	::wxMkdir("\\Images\\BiasBackground");
if (::wxDirExists("\\Images"))
	if (::wxDirExists("\\Images\\BiasBackground"))
		rep="\\Images\\BiasBackground\\";
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
wxString message(_("Background image saved in "));
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


void FenetrePrincipale::AjoutPointCourbeVideo(int c,double x, double y)
{
    if (!courbeVideo || cam==NULL)
        return;
    courbeVideo->Ajoute(c,x,y);
}


void FenetrePrincipale::CopierSelect(wxCommandEvent& evt)
{
    cv::Mat masque = cv::Mat::zeros(imAcq->size(),CV_8UC1);
    bool empty = true;
    for (int i = 0; i < NB_MAX_RECTANGLE;i++)
        if (feuille->rectDsMasque[i] )
        {
            if (!feuille->rectSelect[i].IsEmpty())
                empty = false;
            masque(cv::Rect(feuille->rectSelect[i].x,feuille->rectSelect[i].y,feuille->rectSelect[i].GetWidth(),feuille->rectSelect[i].GetHeight()))=1;
        }
    if (!empty)
        (*imAcq).copyTo(imClipBoard,masque);
    else
        wxMessageBox("Source is empty. Select a mask !");
}

void FenetrePrincipale::CollerImage(wxCommandEvent& evt)
{
if (imClipBoard.empty())
	{
	wxMessageBox("Clipboard is empty", "Clipboard is empty", wxOK );
	return;
	}
		FenetrePrincipale *f = new FenetrePrincipale(NULL, "wxOpenCV",
		wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
		wxString s;
		s.Printf("%d : %s( %d) of image %d ",osgApp->NbFenetre(),_("Copy of "),IdFenetre());
		f->SetTitle(s);
		f->DefOSGApp(osgApp);
        ImageInfoCV	*im = new ImageInfoCV;
        imClipBoard.copyTo(*im);
		f->AssosierImage(im);
		osgApp->InitFenAssociee(f);
		f->InitIHM();
}

void FenetrePrincipale::CollerImageCadrer(wxCommandEvent& evt)
{
    if (imClipBoard.empty())
	    {
	    wxMessageBox("Clipboard is empty", "Clipboard is empty", wxOK );
	    return;
	    }
    cv::Mat masque = cv::Mat::zeros(imAcq->size(),CV_8UC1);
    for (int i = 0; i < NB_MAX_RECTANGLE;i++)
        if (feuille->rectDsMasque[i] )
        {
            masque(cv::Rect(feuille->rectSelect[i].x,feuille->rectSelect[i].y,feuille->rectSelect[i].GetWidth(),feuille->rectSelect[i].GetHeight()))=1;
        }
    (*imAcq).copyTo(imClipBoard,masque);
    int xMin=imAcq->cols,yMin=imAcq->rows;
    int xMax=0,yMax=0;
    for (int i = 0; i < NB_MAX_RECTANGLE;i++)
        if (feuille->rectDsMasque[i] )
        {
            if (xMin>feuille->rectSelect[i].GetTopLeft().x)
                xMin = feuille->rectSelect[i].GetTopLeft().x;
            if (yMin>feuille->rectSelect[i].GetTopLeft().y)
                yMin = feuille->rectSelect[i].GetTopLeft().y;
            if (xMax<feuille->rectSelect[i].GetRightBottom().x)
                xMax = feuille->rectSelect[i].GetRightBottom().x;
            if (yMax<feuille->rectSelect[i].GetLeftBottom().y)
                yMax = feuille->rectSelect[i].GetLeftBottom().y;
        }
    imClipBoard(cv::Rect(xMin,yMin,xMax-xMin+1,yMax-yMin+1)).copyTo(imClipBoard);

	FenetrePrincipale *f = new FenetrePrincipale(NULL, "wxOpenCV",
	wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
	wxString s;
	s.Printf("%d : %s( %d) of image %d ",osgApp->NbFenetre(),_("Copy of "),IdFenetre());
	f->SetTitle(s);
	f->DefOSGApp(osgApp);
    ImageInfoCV	*im = new ImageInfoCV;
    imClipBoard.copyTo(*im);
	f->AssosierImage(im);
	osgApp->InitFenAssociee(f);
	f->InitIHM();
}

void FenetrePrincipale::Coller(wxCommandEvent& evt)
{
if (imAcq==NULL)
	{
	wxMessageBox(_T("Error CreerRapport"), _T("Document is empty"), wxOK );
	return;
	}
}

void FenetrePrincipale::EffacerSelect(wxCommandEvent& evt)
{
if (imAcq==NULL)
	{
	wxMessageBox(_T("Error CreerRapport"), _T("Document is empty"), wxOK );
	return;
	}
}
