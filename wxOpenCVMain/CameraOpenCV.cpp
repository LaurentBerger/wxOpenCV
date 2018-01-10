#include "ImageInfo.h"
#include "CameraOpenCV.h"
#include "EvenementCamera.h"
#include "i3ThermalSystem.hpp"
#include "VideoCourbe.h"
//#include <fstream>
#include <wx/time.h> 

using namespace std;
using namespace cv;

char CameraOpenCV::indIdVideo[NBCAMERA]={0,0,0,0,0,0,0,0,0,0};


//#define _FLUXHTTP__

EvtPointSuivis::EvtPointSuivis(wxEventType commandType , int id):wxCommandEvent(m_sortie,id)
    {
        SetId(id);
        SetEventType(commandType);
        m_Expediteur = 0;
        m_sortie = false;
        m_travailEchoue = false;
		indEvt=0;
    }

int CameraOpenCV::PositionVideo(int pos )
{
    if (parent)
    {
        //wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
	    if (pos>=0 && pos<nbImageVideo)
            captureVideo->set(CAP_PROP_POS_FRAMES,static_cast<double>(pos));
        return static_cast<int>(captureVideo->get(CAP_PROP_POS_FRAMES));
    }
    else
    {
        double x = captureVideo->get(CAP_PROP_FRAME_COUNT);
        if (pos >= 0 && pos<nbImageVideo)
            captureVideo->set(CAP_PROP_POS_FRAMES, static_cast<double>(pos));
        return static_cast<int>(captureVideo->get(CAP_PROP_POS_FRAMES));
    }
};

int CameraOpenCV::PositionDebutVideo()
{
    
    return PositionVideo(0);
};

int CameraOpenCV::PositionFinVideo()
{

    if (parent)
    {
        //wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

        captureVideo->set(CAP_PROP_POS_FRAMES,nbImageVideo-2);
        return static_cast<int>(captureVideo->get(CAP_PROP_POS_FRAMES));
    }
    else
    {

        captureVideo->set(CAP_PROP_POS_FRAMES, nbImageVideo - 2);
        return static_cast<int>(captureVideo->get(CAP_PROP_POS_FRAMES));
    }

};

int CameraOpenCV::NbImageVideo()
{
    if (parent)
    {
        //wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
        double x=captureVideo->get(CAP_PROP_FRAME_COUNT);
        nbImageVideo=x;
        return static_cast<int>(x);
    }

    double x = captureVideo->get(CAP_PROP_FRAME_COUNT);
    nbImageVideo=x;
    return static_cast<int>(x);
};


CameraOpenCV::CameraOpenCV(wxString nomFlux):CameraVirtuelle()
{
//nomFlux="0";
wxString		repTravail=wxGetCwd();
strcpy(nomCamera,"OpenCV");
wxString nomOrdinateur(wxGetHostName());
tailleCapteur[0]=wxSize(160,120);tailleCapteur[1]=wxSize(176,144);tailleCapteur[2]=wxSize(320,176);
tailleCapteur[3]=wxSize(320,240);tailleCapteur[4]=wxSize(352,288);tailleCapteur[5]=wxSize(432,240);
tailleCapteur[6]=wxSize(544,288);tailleCapteur[7]=wxSize(640,360);tailleCapteur[8]=wxSize(640,480);
tailleCapteur[9]=wxSize(752,416);tailleCapteur[10]=wxSize(800,448);tailleCapteur[11]=wxSize(800,600);
tailleCapteur[12]=wxSize(864,480);tailleCapteur[13]=wxSize(960,544);tailleCapteur[14]=wxSize(960,720);
tailleCapteur[15]=wxSize(1024,576);tailleCapteur[16]=wxSize(1184,656);tailleCapteur[17]=wxSize(1280,720);
tailleCapteur[18]=wxSize(1280,960);tailleCapteur[19]=wxSize(-1,-1);tailleCapteur[20]=wxSize(-1,-1);
tailleCapteur[21]=wxSize(-1,-1);
/*for (int i=15;i<NB_TAILLE_VIDEO;i++)
	tailleAutorisee[false];*/
modeAcqContinu=1;
acqArretee=0;
indFiltreMoyenne=1;
tpsInactif =30;
fluxOuvert=false;
chgtTaille=false;
aaButter[0]=-0.9996859;
aaButter[1]=-0.9993719;
aaButter[2]=-0.9968633;
aaButter[3]=-0.9937365;
aaButter[4]=-0.9690674;
aaButter[5]=-0.9390625;
aaButter[6]=-0.7265425;
aaButter[7]=-0.5095254;
aaButter[8]=-0.3249;
aaButter[9]=-0.1584;
aaButter[10]=-0.0;
bbButter[0]=0.0001571;
bbButter[1]=0.0003141;
bbButter[2]=0.0015683;
bbButter[3]=0.0031318;
bbButter[4]=0.0154663;
bbButter[5]=0.0304687;
bbButter[6]=0.1367287;
bbButter[7]=0.2452373;
bbButter[8]=0.3375;
bbButter[9]=0.4208;
bbButter[10]=0.5;

parent=NULL;
captureVideo=NULL;
imAcq=NULL;
modeAcqContinu=1;
acqArretee=0;
long indFlux;

// Appel de VideoCapture pour récupérer la taille de la vidéo

if (nomFlux!=wxEmptyString)
	{
	if (nomFlux.ToLong(&indFlux))
		{
		captureVideo = new cv::VideoCapture(indFlux); 
		indId=indFlux;
		}
	else if (nomFlux.SubString(0,4)=="pipe:")
    {
        captureVideo = new I3ThermalSystem("\\\\.\\pipe\\thermalExpert");
        indId = -1;
        fluxVideo = true;
        nomFluxVideo = nomFlux.c_str();
    }
    else

	{
		
		captureVideo = new cv::VideoCapture(); 
//		captureVideo->set(CAP_PROP_FOURCC,'MJPG');
		captureVideo->open(nomFlux.ToStdString());
        PositionDebutVideo();
        modeAcqContinu=0;
		//captureVideo = new cv::VideoCapture(nomFlux.ToStdString()); 
		indId=-1;
        fluxVideo = true;
        nomFluxVideo= nomFlux.c_str();
	}
    double x = captureVideo->get(CAP_PROP_FOURCC);
    int fourcc = captureVideo->get(CAP_PROP_FOURCC);
    string fourcc_str = format("%c%c%c%c", fourcc & 255, (fourcc >> 8) & 255, (fourcc >> 16) & 255, (fourcc >> 24) & 255);
	}
else
	{
	for (indId=0;indId<NBCAMERA ;indId++)
		if (indIdVideo[indId]==0)
			{
			captureVideo = new cv::VideoCapture(indId+ CAP_DSHOW); 
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
	}

if(captureVideo->isOpened())  // check if we succeeded
	{
	cv::Mat frame;
	if( indId==-1)
		fluxOuvert=true;
/*	for (int i=15;i<NB_TAILLE_VIDEO;i++)
		{
		status=captureVideo->set(cv::CAP_PROP_FRAME_WIDTH,tailleCapteur[i].GetX());
		tailleAutorisee[i]=captureVideo->set(cv::CAP_PROP_FRAME_HEIGHT,tailleCapteur[i].GetY());
		}*/
	(*captureVideo) >> frame; // get a new frame from camera
	nbColonnePhys = frame.cols;
	nbLignePhys = frame.rows;
	if (frame.cols*frame.rows==0)
		indId=NBCAMERA;
	switch(frame.type()){
	case CV_8UC3:

		nbCanauxAD = frame.type();
		break;
	default:
		nbCanauxAD = frame.type();
		}
	}
else
	{
	indId=NBCAMERA;
 	return;
	}
if (indId>=0 && indId<NBCAMERA)
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
double x;
x=captureVideo->get(CAP_PROP_EXPOSURE);
reglageTpsExpo=captureVideo->set(CAP_PROP_EXPOSURE,x);
x=captureVideo->get(CAP_PROP_BRIGHTNESS);
reglageLuminosite=captureVideo->set(CAP_PROP_BRIGHTNESS,x);
x=captureVideo->get(CAP_PROP_GAIN);
reglageGain=captureVideo->set(CAP_PROP_GAIN,x);
x=captureVideo->get(CAP_PROP_SATURATION);
reglageSaturation=captureVideo->set(CAP_PROP_SATURATION,x);
x=captureVideo->get(CAP_PROP_FRAME_WIDTH);
reglageTaille=captureVideo->set(CAP_PROP_FRAME_WIDTH,x);
x=captureVideo->get(CAP_PROP_CONTRAST);
reglageContraste=captureVideo->set(CAP_PROP_CONTRAST,x);
x = captureVideo->get(CAP_PROP_FOURCC);
int fourcc = captureVideo->get(CAP_PROP_FOURCC);
string fourcc_str = format("%c%c%c%c", fourcc & 255, (fourcc >> 8) & 255, (fourcc >> 16) & 255, (fourcc >> 24) & 255);

if(!erreurFct)
	AjouteMsgErreur("SetExposureTime Error\n");
//captureVideo->getMinMax(CV_CAP_PROP_GAIN,&gainMin,&gainMax);
}

CameraOpenCV::~CameraOpenCV(void)
{
if (parent==NULL)
	{
	if (Connectee() && indId>=0 && indId<NBCAMERA)
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
else
	{
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

	if (Connectee()&& indId>=0 && indId<NBCAMERA)
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
}

double CameraOpenCV::Gain(double x)
{
if (!parent || !reglageGain)
	return DBL_MAX;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (x==DBL_MAX)
	return captureVideo->get( CAP_PROP_GAIN);
bool b=captureVideo->set(CAP_PROP_GAIN,x);
if (b)
	return x;
return DBL_MAX;
}

double CameraOpenCV::Contraste(double x)
{
if (!parent || !reglageContraste)
	return DBL_MAX;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (x==DBL_MAX)
	return captureVideo->get( CAP_PROP_CONTRAST);
bool b=captureVideo->set(CAP_PROP_CONTRAST,x);
if (b)
	return x;
return DBL_MAX;
}

double CameraOpenCV::Luminosite(double x)
{
if (!parent || !reglageLuminosite)
	return DBL_MAX;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (x==DBL_MAX)
	return captureVideo->get( CAP_PROP_BRIGHTNESS);
bool b=captureVideo->set(CAP_PROP_BRIGHTNESS,x);
if (b)
	return x;
return DBL_MAX;
}
double CameraOpenCV::Saturation(double x)
{
if (!parent || !reglageSaturation)
	return DBL_MAX;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (x==DBL_MAX)
	return captureVideo->get( CAP_PROP_SATURATION);
bool b=captureVideo->set(CAP_PROP_SATURATION,x);
if (b)
	return x;
return DBL_MAX;
}


bool CameraOpenCV::DefTailleImage(int largeur,int hauteur)
{
if (!parent || !reglageTaille)
	return false;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
bool		status=captureVideo->set(cv::CAP_PROP_FRAME_WIDTH,largeur);
if (status)
	status =captureVideo->set(cv::CAP_PROP_FRAME_HEIGHT,hauteur);
chgtTaille=true;
colonneFin=largeur;
ligneFin=hauteur;
nbColonnePhys=colonneFin;
nbLignePhys=ligneFin;
return status;
}

double CameraOpenCV::TempsExposition(double x)
{
if (!parent || !reglageTpsExpo)
	return DBL_MAX;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
if (x==DBL_MAX)
	return captureVideo->get( CAP_PROP_EXPOSURE);
bool b=captureVideo->set(CAP_PROP_EXPOSURE,x);
if (b)
	return x;
return DBL_MAX;
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


int	CameraOpenCV::Acquisition(void) /*<! Acquisition d'une image */
{
if (typeAcq==CV_32FC3)
	{
	UMat frame;
	UMat frameFlt;
	while (!captureVideo->retrieve(frame));
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
	frame.convertTo(frameFlt,CV_32FC3);
	(*((UMat *)imAcq)) =frameFlt; // get a new frame from camera
	}
else if (typeAcq==CV_8UC3)
	{
	UMat frame;
	while (!captureVideo->retrieve(frame));
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

	(*((UMat *)imAcq)) =frame; // get a new frame from camera
	}

return 1;
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

wxThread::ExitCode CameraOpenCV::Acquisition8BitsRGB()
{
int indEvt=0;
if (!captureVideo)
	captureVideo= new VideoCapture(indId); 

if (captureVideo->isOpened())
	{
	Mat     frame1;
	Mat     frame2;
	Mat     frame;
    UMat    frameBuffer;
	int nbPts[100];
	int nbTour = 0;
	std::vector<cv::Point2f> repereIni, repere;
	while (!captureVideo->retrieve(frameBuffer));
	while (!captureVideo->retrieve(frame2));
	while (!captureVideo->retrieve(frame1));
    initDate = getTickCount()/getTickFrequency();
	static bool opActif=false;
	ImageInfoCV *imIni=new ImageInfoCV(frame.rows,frame.cols,frame.flags);
	ImageInfoCV *imPre=NULL;
	bool	frameDejaCopie=false;
	for(;true;)
	{
//        int x= PositionVideo();
		if (!acqArretee &&  captureVideo->grab()) // get a new frame from camera
		{
			captureVideo->retrieve(frame);
            frameDate = getTickCount()/getTickFrequency();
//            x = PositionVideo();
            if (!modeAcqContinu)
                acqArretee=1;
			if (modeMoyenne)	// Filtrage Butterworth
			{
                if (frame.size() != frame1.size())
                    frame.copyTo(frame1);
                if (frame.size() != frame2.size())
                    frame.copyTo(frame2);
				for (int i = 0; i<frame.rows; i++)
				{
					unsigned char *val = frame.ptr(i);
					unsigned char *valB1 = frame1.ptr(i);
					unsigned char *valB2 = frame2.ptr(i);
					for (int j = 0; j<frame.cols; j++)
						for (int k = 0; k<frame.channels(); k++, valB1++, valB2++, val++)
							*val = bbButter[indFiltreMoyenne] * (*valB1 + *valB2) - aaButter[indFiltreMoyenne] * *val;

				}
				frame1.copyTo(frame2);
				frame.copyTo(frame1);
			}
			
			if (parent)
			{
				if (seqOp.size()!=0)
				{
					bool memoriseImage=false;
					opActif =true;
					if (chgtTaille)
					{
						delete imPre;
						delete imIni;
						imIni=new ImageInfoCV(frame.rows,frame.cols,frame.flags);
					}
					frame.copyTo(*imIni);
					vector< vector<ImageInfoCV*> > im;
					im.resize(seqOp.size());
					if (!parent)
						break;
					int indOp=0;
					{
					    wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->paramCam);

					    if (seqActualisee)
					    {
						    seqActualisee=false;
						    delete imPre;
						    imPre=NULL;
						    std::map<ImageInfoCV*, bool>::iterator it;
						    for (it = effaceImage.begin(); it != effaceImage.end(); it++)
							    if (imIni!=it->first)
								    delete it->first;
						    effaceImage.clear();
						    imgParam.clear();
						    //fond.clear();
					    }
					    for (std::vector <ParametreOperation > ::iterator it=seqOp.begin();it!=seqOp.end();it++)
						    {
						    ParametreOperation pOCV=*it;
						    pOCV.indOpFenetre[0]=-1;
						    if (pOCV.indOpFenetre.size()>=2)
							    pOCV.indOpFenetre[1] = -1;
						    if (pOCV.nbOperande==1) // op1 est initialisé
                                {
                                if (indOp>0) // Si une opération a déjà été effectuée l'image précédente est retenue comme paramètre
							        pOCV.op[0]=im[indOp-1][0];
						        else // sinon image initiale
							        {
                                    pOCV.op[0] = imIni;
                                    effaceImage[imIni]=false;
                                    }
                                }
                            else if (pOCV.nbOperande==2 ) // initialisation op2
                            {
							    if (pOCV.opVideo)
							    {
								    if (indOp>0 ) // Si une opération a déjà été effectuée
									    if (im[indOp-1].size()==0)
                                            pOCV.op[1] = NULL;
									    else
                                            pOCV.op[1] = im[indOp - 1][0];
								    else
                                        pOCV.op[1] = imIni;
								
								    if (imgParam.find(pOCV.nomOperation+"prec")!=imgParam.end())
								    {
                                        pOCV.op[0] = imgParam[pOCV.nomOperation + "prec"];
									    effaceImage[imgParam[pOCV.nomOperation + "prec"]]=false;
								    }
								    else
                                        pOCV.op[0] = pOCV.op[1];
							    }
							    else if (indOp>0)
                                    pOCV.op[0] = im[indOp - 1][0];
							}
							if (imgParam.size() != 0)
								pOCV.imgParam = imgParam;
							if (fond.size() != 0)
								pOCV.ecartFond = fond;
							im[indOp] = pOCV.ExecuterOperation();
							if (pOCV.imgParam.size() != 0)
								{
                                imgParam = pOCV.imgParam;
                                if (imgParam.find(pOCV.nomOperation + "prec")!=imgParam.end())
                                    if (imgParam[pOCV.nomOperation + "prec"]==imIni)
                                        {
                                        imIni = new ImageInfoCV(frame.rows, frame.cols, frame.flags);
                                        effaceImage[imIni]=false;
                                        }
                                }
							if (pOCV.ecartFond.size() != 0)
								fond = pOCV.ecartFond;
						    if (pOCV.opErreur != 0) // Arret de la séquence d'opération retour à acquistion vidéo simple
						    {
							    seqOp.clear();
							    effaceImage[imPre] = true;
							    effaceImage.erase(imIni);
							    std::map<ImageInfoCV*, bool>::iterator it;
							    for (it = effaceImage.begin(); it != effaceImage.end(); it++)
								    delete it->first;
							    effaceImage.clear();
							    imPre = NULL;
							    break;
						    }

						    if (im[indOp].size()!=0) // Si l'opérateur donne un résultat non nul
							    effaceImage[im[indOp][0]]=false;
						    if (pOCV.nbOperande==2 && pOCV.opVideo) // Pour les opérateurs binaires spécifiques à la vidéo
							    memoriseImage=true;
                            indOp++;							
						}
					}
					if (indOp>0 && seqOp.size()>0)
					{
						if (!parent)
							break;
						wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

                        if (im[indOp-1].size() )
						{
							if (chgtTaille)
							{
								((FenetrePrincipale*)parent)->ChgtTailleVideo(0);
								chgtTaille=false;
							}
							imAcq->CloneStat(im[indOp-1][0]);
							imAcq->DeplacerFlotOptique(im[indOp-1][0]);
							im[indOp-1][0]->copyTo(*imAcq);
							frameDejaCopie=true;
						}
                        std::map<ImageInfoCV*,bool>::iterator it;
						vector<ImageInfoCV*> elt;
						for (it = effaceImage.begin(); it != effaceImage.end(); it++)
							if (it->second)
							{
								delete it->first;
								elt.push_back(it->first);
							}
							else
								it->second=true;

						for (int ii=0;ii<elt.size();ii++)
							effaceImage.erase(elt[ii]);
				    }
				}
				{
    			    if (!parent)
					    break;
				    wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

				    if (!frameDejaCopie)
					{
					    if (chgtTaille)
					    {
						    ((FenetrePrincipale*)parent)->ChgtTailleVideo(0);
						    chgtTaille=false;
					    }
					    //frame.copyTo((*((UMat *)imAcq))); // get a new frame from camera
                        Mat x=imAcq->getMat(ACCESS_WRITE);
                        frame.copyTo(x);
					    //swap(frameBuffer, (*((UMat *)imAcq)));
					}
                    frameDejaCopie=false;

				}

			}
            else
                wxMilliSleep(50);
			if (parent)
			{
				//wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
				if (!parent)
					break;
				bool attendre=true;
				int nbBoucle=0;
				while(attendre && nbBoucle<100)
				{
					if (!parent)
						break;
                    {
                        wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					    if (indEvt==0 || ((FenetrePrincipale*)parent)->IndEvtCam()==indEvt-1 /*|| nbBoucle==99*/)
						    {
                            EvtPointSuivis *x = new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
                            x->ptId = repereIni;
                            x->ptApp = repere;
                            x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
                            x->indEvt = indEvt;
                            indEvt++;
                            wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
						    attendre=false;
						    }
					}
					nbBoucle++;
					if (attendre)this->Sleep(1);
				}
                
			}
			else
				break;
			if (TestDestroy())
				break;
		}

	if (TestDestroy())
		break;
	}
	delete captureVideo;
    delete imIni;
    delete imPre;
	}
captureVideo=NULL;

return (wxThread::ExitCode)0;  
}

wxThread::ExitCode CameraOpenCV::Acquisition32BitsFloatRGB()
{
#ifdef _FLUXHTTP__
captureVideo = new cv::VideoCapture("192.168.0.1:8080"); 
#else
captureVideo= new VideoCapture(indId); 
#endif
if (captureVideo->isOpened())
	{
	Mat frameFlt1;
	Mat frameFlt2;
	Mat frame;
	Mat frameFlt;
	std::vector<cv::Point2f> repereIni,repere;
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt2,CV_32FC3);
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt1,CV_32FC3);

	for(;true;)
		{
		if (captureVideo->grab()) // get a new frame from camera
			{
			captureVideo->retrieve(frame);
			if (parent)
				{


				if (!modeMoyenne)	// Pas de filtrage Butterworth
					{
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					frame.copyTo(frameFlt,CV_32FC3);
					(*((UMat *)imAcq)).setTo(frameFlt); // get a new frame from camera
					}
				else
					{
					{
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					frame.copyTo(frameFlt,CV_32FC3);

					(*((UMat *)imAcq)).setTo(frameFlt); // get a new frame from camera
					for (int i=0;i<frame.rows;i++)
						{
						float *val=(float *)frameFlt.ptr(i);
						float *valB1=(float *)frameFlt1.ptr(i);
						float *valB2=(float *)frameFlt2.ptr(i);
						for (int j=0;j<frame.cols;j++)
							for (int k=0;k<frame.channels();k++,valB1++,valB2++,val++)
								*val = bbButter[indFiltreMoyenne]*(*valB1 + *valB2)-aaButter[indFiltreMoyenne]* *val;
						
						}
					}
					frameFlt1.copyTo(frameFlt2);
					frameFlt.copyTo(frameFlt1);
					}

				}
			if (parent)
				{
				EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
				x->ptId=repereIni;
				x->ptApp=repere;
				x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
				wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);

				this->Sleep(1);
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


wxThread::ExitCode CameraOpenCV::Entry()
{
if (captureVideo==NULL)
	{
	captureVideo = new cv::VideoCapture(indId); 
	if(!captureVideo || !captureVideo->isOpened())
		return (wxThread::ExitCode)-1;
	}
if (!fluxOuvert &&(indId<0 || indId>=NBCAMERA))
	return (wxThread::ExitCode)-1; 	
if (typeAcq==CV_8UC3 || fluxOuvert)
	return Acquisition8BitsRGB();	
else if (typeAcq==CV_32FC3)
	return Acquisition32BitsFloatRGB();	
return (wxThread::ExitCode)0;  
}

void FenetrePrincipale::OnThreadUpdateQueue(EvtPointSuivis &w)
{

if (cam && (cam->IsRunning() || cam->IsPaused()))
	{
	long x=wxGetUTCTimeMillis().GetLo();
//	if (x<10)
	{
	wxCriticalSectionLocker enter(travailCam);

	delete feuille->BitmapAffichee();
	feuille->BitmapAffichee(NULL);
    DIB(imAcq);
    if (video.isOpened())
    {
        Mat matIm = imAcq->getMat(cv::ACCESS_READ);
        video << matIm;
    }
    imageTraitee = false;
	feuille->Refresh(false);
	}
	indEvtCam=w.indEvt;
	nbImageCam++;
	if (nbImageCam==25)
		{
		cam->DefTpsInactif((x - tpsPreEvt) / 25);
		nbImageCam=0;
		tpsPreEvt=x;
		
		osgApp->CtrlCamera()->DrawOngletStatus();
		}
    long dt= wxGetUTCTimeMillis().GetLo()-x;
    dt=dt+1;
    }
return;
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
	if (cam->IsRunning())
		{
		if (cam->Delete() != wxTHREAD_NO_ERROR )                
			wxLogError("Can't delete the thread!");        
		}
	cam =NULL;	
	}    
 // exit from the critical section to give the thread            
 // the possibility to enter its destructor            
 // (which is guarded with m_pThreadCS critical section!)    
 }

void FenetrePrincipale::DefSeqCamera(std::vector <ParametreOperation> *s)
{  
if (!cam)
	return;

seqOp= *s;
cam->seqActualisee=true;
wxCriticalSectionLocker enter(paramCam);

cam->seqOp=*s;
}

void FenetrePrincipale::RazSeqOp()
{
seqOp.clear();
if (!cam)
	return;

wxCriticalSectionLocker enter(paramCam);

cam->seqOp.clear();
};



