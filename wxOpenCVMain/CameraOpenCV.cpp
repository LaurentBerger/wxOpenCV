#include "ImageInfo.h"
#include "CameraOpenCV.h"
#include "EvenementCamera.h"
//#include <fstream>
#include <wx/time.h> 

using namespace std;
using namespace cv;

char CameraOpenCV::indIdVideo[NBCAMERA]={0,0,0,0,0,0,0,0,0,0};

wxDECLARE_EVENT(VAL_EVT_PTS_SUIVIS, EvtPointSuivis);

//#define _FLUXHTTP__

EvtPointSuivis::EvtPointSuivis(wxEventType commandType , int id):wxCommandEvent(m_sortie,id)
    {
        SetId(id);
        SetEventType(commandType);
        m_Expediteur = 0;
        m_sortie = false;
        m_travailEchoue = false;
    }



CameraOpenCV::CameraOpenCV(void)
{
wxString		repTravail=wxGetCwd();
strcpy(nomCamera,"OpenCV");
wxString nomOrdinateur(wxGetHostName());
//double FenetrePrincipale::aaButter[NBFILTRE]={-0.9996859,-0.9993719,-0.9968633,-0.9937365,-0.9690674,-0.9390625,-0.7265425,-0.5095254,-0.3249,-0.1584,-0.0,};
//double FenetrePrincipale::bbButter[NBFILTRE]={0.0001571,0.0003141,0.0015683,0.0031318,0.0154663,0.0304687,0.1367287,0.2452373,0.3375,0.4208,0.5,};

parent=NULL;
captureVideo=NULL;
imAcq=NULL;
imAcq2 = NULL;
imAcqBrutFil = NULL;
imAcqBrutFilMax = NULL;
imAcqBrut1 = NULL;
imAcqBrut2 = NULL;
imTache = NULL;
imRefTache = NULL;
imQuadrique = NULL;
nivBiais = NULL;

/*
imAcq2 = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imAcqBrutFil = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imAcqBrutFilMax = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imAcqBrut1 = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imAcqBrut2 = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imTache = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imRefTache = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
imQuadrique = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());
nivBiais = new ImageInfoCV(cam->NbLigne(),cam->NbColonne(),cam->NbCanaux());

delete imAcq2;
delete imAcqBrutFil;
delete imAcqBrutFilMax;
delete imAcqBrut1;
delete imAcqBrut2;
delete imTache;
delete imRefTache;
delete imQuadrique;
delete nivBiais;


*/

// Appel de VideoCapture pour récupérer la taille de la vidéo
#ifdef _FLUXHTTP__
captureVideo = new cv::VideoCapture("192.168.0.1:8080"); 
indId=0;
#else
for (indId=0;indId<NBCAMERA ;indId++)
	if (indIdVideo[indId]==0)
		{
		captureVideo = new cv::VideoCapture(indId); 
		if(captureVideo && captureVideo->isOpened())
			break;
		if(captureVideo)
			{
			delete captureVideo;
			captureVideo=NULL;
			}
		}
if (indId==NBCAMERA)
	return;
#endif
if(captureVideo->isOpened())  // check if we succeeded
	{
	cv::Mat frame;
	(*captureVideo) >> frame; // get a new frame from camera
	nbColonnePhys = frame.cols;
	nbLignePhys = frame.rows;
	switch(frame.type()){
	case CV_8UC3:

		nbCanauxAD = frame.type();
		break;
	default:
		nbCanauxAD = frame.type();
		}
	}
else
 	return;
indIdVideo[indId]=1;

chaineErreur="";
char	tmp[1024];
strcpy(tmp,repTravail.mb_str( wxConvUTF8));
testDriver=1;
if (!captureVideo)
	{
	AjouteMsgErreur("Initialization errors:\n");
    AjouteMsgErreur("Initialize Error\n");
    }
//PrepareAcquisition();
indLignePremierpixel=1;
nbLigneInterPiste=1;
colonneDebut=1;
ligneDebut=1;
colonneFin=nbColonnePhys;
ligneFin=nbLignePhys;
//tempsExposition = captureVideo->getMinMax(CV_CAP_PROP_EXPOSURE,&expoMin,&expoMax);
nbScanParAccumulation=1;
periodeAccumulation=1;	// en seconde
modeDeclenchement=0;			// Mode de déclenchement de l'acquisition p45 SDK Andor

erreurFct=captureVideo->set(CAP_PROP_EXPOSURE,tempsExposition);
if(!erreurFct)
	AjouteMsgErreur("SetExposureTime Error\n");
//captureVideo->getMinMax(CV_CAP_PROP_GAIN,&gainMin,&gainMax);
}

CameraOpenCV::~CameraOpenCV(void)
{
if (parent==NULL)
	{
	if (Connectee())
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
else
	{
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

	if (Connectee())
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
}

void CameraOpenCV::DefTempsExposition(double x)
{
if (!parent)
	return;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
captureVideo->set(CAP_PROP_EXPOSURE,x);
}

void CameraOpenCV::DefModeGain(int i)
{
}

void CameraOpenCV::DefEMCCDGain(int v)
{
}

void CameraOpenCV::LireIntervalGain(int *x,int *y)
{
}

void CameraOpenCV::DefNombreAccumulation(int x)
{
}

void CameraOpenCV::DefFrequenceAccumulation(double x)
{
}

void CameraOpenCV::DefNombreCinetique(int x)
{
}

void CameraOpenCV::DefFrequenceCinetique(double x)
{
}

void CameraOpenCV::DefTailleImage(int article)
{
}

void CameraOpenCV::DefBinning(int article)
{
}

void CameraOpenCV::DefModeAcquisition(int article)
{
}
void CameraOpenCV::DefModeDeclenchement(int article)
{
chaineErreur="";
}
void CameraOpenCV::DefZoneLecture(int article)
{
chaineErreur="";
}

char CameraOpenCV::ImagePrete()
{
return 0;
}

char CameraOpenCV::Image(unsigned short *data,unsigned long &taille)
{

chaineErreur="";
if (TestDriver()==0)
	{
/*
	ifstream g("imagepardefaut.txt");
	char	nomFichier[1024];
	static ImageInfoCV *imTest= NULL;
	if (!imTest)
		if (g.is_open())
			{
			g.getline(nomFichier,1024);
			imTest = new ImageInfoCV(nomFichier);
			}
		else
			imTest = new ImageInfoCV("C:\\Images\\seec images\\20091102_Treated\\EPC_P89_START_j7h0m29_END_j7h16m17_T45\\EgPC_P89_Day9A45_09.tif");
	unsigned short *dataTest=(unsigned short *)imTest->data;
	for (int i=0;i<(ligneFin-ligneDebut+1);i++)
		for (int j=0;j<colonneFin-colonneDebut+1;j++,data++,dataTest++)
			{
			*data =*dataTest;
			}
	return 1;
*/
	}
taille=0;return 1;
}


char CameraOpenCV::TailleImage(unsigned long &taille)
{
return 1;
};

void CameraOpenCV::DefCoinGauche(int x)
{
return;
}

void CameraOpenCV::DefCoinDroit(int x)
{
return;
}

void CameraOpenCV::DefCoinHaut(int x)
{
return;
}

void CameraOpenCV::DefCoinBas(int x)
{
return;
}

char CameraOpenCV::EMGain8Bit()
{
	return 0;
}

char CameraOpenCV::EMGain12Bit()
{
	return 0;
}

char CameraOpenCV::EMGainLinear12Bit()
{
	return 0;
}

char CameraOpenCV::EMGainReal12Bit()
{
	return 0;
}

	
char CameraOpenCV::GetFunctionEMCCDGain()
{
	return 0;
}

wxThread::ExitCode CameraOpenCV::Entry()
{
if (indId<0 || indId>=NBCAMERA)
	return (wxThread::ExitCode)-1; 		
#ifdef _FLUXHTTP__
captureVideo = new cv::VideoCapture("192.168.0.1:8080"); 
#else
captureVideo= new VideoCapture(indId); 
#endif
if (captureVideo->isOpened())
	{
	Mat edges; 
	Mat frame;
	std::vector<cv::Point2f> repereIni,repere;
	std::vector<unsigned char> statusPtsSuivis;
	std::vector<float> errPtsSuivis;
	(*captureVideo) >> frame;
    TermCriteria termcrit(3, 20, 0.03);
    Size subPixWinSize(10,10), winSize(31,31);

	Mat imGris,imGrisIni;
	int	nbMax=1000;
	double	qlevel=0.01;
	double	minDistance =10;
	int tailleFen=10;
	this->Sleep(100);
//	repere.resize(nbMax);
	for (int i=0;i<1;i++)
			(*captureVideo) >> frame;
	cvtColor(frame, imGrisIni, COLOR_BGR2GRAY);
	goodFeaturesToTrack(imGrisIni, repereIni,  nbMax,  qlevel,minDistance,Mat(),3, 0, 0.04);
	try 
		{
		cornerSubPix(imGrisIni, repereIni, subPixWinSize, Size(-1, -1),termcrit);
		}
	catch(exception& e)
		{
		}


	std::vector<cv::Mat>  pyrIni;
	std::vector<cv::Mat>  pyr;
	buildPyramid(imGrisIni,pyrIni,5);

	for(;true;)
		{
		if (captureVideo->retrieve(frame)) // get a new frame from camera
			{
			if (parent)
				{

				wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

				(*((Mat *)imAcq)) =frame; // get a new frame from camera
		/*		cvtColor(frame, imGris, CV_BGR2GRAY);

				calcOpticalFlowPyrLK( imGrisIni, imGris,  repereIni, repere,
					statusPtsSuivis,errPtsSuivis, cvSize( tailleFen, tailleFen ), 5, 
					cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 ), 0 );
		*/

				}
			if (parent)
				{
				EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
				x->ptId=repereIni;
				x->ptApp=repere;
				x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
				wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
//				this->Sleep(20);
				}
			else
				break;
			if (TestDestroy())
				break;
			}
		}
	delete captureVideo;
	}
captureVideo=NULL;

return (wxThread::ExitCode)0;  
}

void FenetrePrincipale::OnThreadUpdateQueue(EvtPointSuivis &w)
{
if (!cam->IsRunning())
	return;
//wxClientDC dc(this);
//DrawWindow (dc);
long x=wxGetUTCTimeMillis().GetLo()-w.GetTimestamp();
if (x<10)
	{
	delete feuille->BitmapAffichee();
	feuille->BitmapAffichee(NULL);
	MAJNouvelleImage();
	}
}

void FenetrePrincipale::DoPauseThread()
{    
if (cam==NULL)
	return;
// anytime we access the m_pThread pointer we must ensure that it won't    
// be modified in the meanwhile; since only a single thread may be    
// inside a given critical section at a given time, the following code    
// is safe:    
wxCriticalSectionLocker enter(travailCam);    
// without a critical section, once reached this point it may happen        
// that the OS scheduler gives control to the MyThread::Entry() function,        
// which in turn may return (because it completes its work) making        
// invalid the m_pThread pointer        
if (cam->Pause() != wxTHREAD_NO_ERROR )            
	wxLogError("Can't pause the thread!");    
}

void FenetrePrincipale::OnCloseThread(wxCloseEvent&)
{    
wxCriticalSectionLocker enter(travailCam);

if (cam)         // does the thread still exist?        
	{            
	cam->parent=NULL;
	wxMessageOutputDebug().Printf("MYFRAME: deleting thread");            
	if (cam->Delete() != wxTHREAD_NO_ERROR )                
		wxLogError("Can't delete the thread!");        
	cam =NULL;	
	}    
 // exit from the critical section to give the thread            
 // the possibility to enter its destructor            
 // (which is guarded with m_pThreadCS critical section!)    
 }

