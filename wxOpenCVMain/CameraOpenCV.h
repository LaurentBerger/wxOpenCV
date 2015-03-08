#ifndef _CAMERAOPENCV_H_
#define _CAMERAOPENCV_H_

#include "CameraVirtuelle.h"


#define NBCAMERA 10

/*! \class CameraOpenCV
   * \brief La classe permet de gérer un flux vidéo reconnu par OpenCV
   *
   */

class CameraOpenCV : public CameraVirtuelle {
protected :
virtual wxThread::ExitCode Entry();
static char	indIdVideo[NBCAMERA];
int			indId; /*<! Indice du périphérique vidéo;*/
bool		fluxOuvert;/*<!Vrai lorsque la ideo est un flux */
long		expoMin,expoMax;
long		gainMin,gainMax;


public : 
ImageInfoCV			*imAcq;

private :
// PROPRIETES DE LA CAMERA
cv::VideoCapture	*captureVideo;			/*<! Gestionnaire de capture d'OpenCV */
int	indLignePremierpixel;
int	nbLigneInterPiste;
	// Mode Random Track	
	// Mode Image
int	colonneDebut;
int	ligneDebut;
int colonneFin;
int	ligneFin;
/////////////////////////////////////////////////////////

// MODE D'ACQUISITION DU CAPTEUR	
double valGain;				/*<! Valeur de gain le flux */
double	tempsExposition; /*<! Temps d'exposition */
	// Mode Accumulate  p30
int		nbScanParAccumulation;
float	periodeAccumulation;	// en seconde

// MODE DU TRIGGER
int	modeDeclenchement;			// Mode de déclenchement de l'acquisition p45 SDK Andor
/////////////////////////////////////////////////////////

// ADChannel
int	nbCanauxAD;				// p 126 SDK

//////////////////////////////////////////////////////////
public :
CameraOpenCV(wxString s=wxEmptyString);
~CameraOpenCV(void);
cv::VideoCapture *CamVideo(){return captureVideo;};
int	Acquisition(void); /*<! Acquisition d'une image */
virtual bool Connectee(){return (indId>=0 && indId<NBCAMERA)|| fluxOuvert;};
virtual void FermerVideo(){delete captureVideo;captureVideo=NULL;};
virtual void DefCoinGauche(int x);
virtual void DefCoinDroit(int x);
virtual void DefCoinHaut(int x);
virtual void DefCoinBas(int x);
virtual void DefTempsExposition(double);
virtual void DefNombreAccumulation(int);
virtual void DefFrequenceAccumulation(double);
virtual void DefNombreCinetique(int);
virtual void DefFrequenceCinetique(double);
virtual void DefTailleImage(int);
virtual void DefBinning(int);
virtual void DefModeAcquisition(int );
virtual void DefModeDeclenchement(int);
virtual void DefZoneLecture(int );

virtual char ImagePrete();
virtual char Image(unsigned short *data,unsigned long &taille);
virtual char TailleImage(unsigned long &taille);
virtual int NbColonne(){ return colonneFin-colonneDebut+1;};
virtual int NbLigne(){return ligneFin-ligneDebut+1;};
virtual int NbCanaux(){return nbCanauxAD;};

virtual ExitCode Acquisition8BitsRGB();
virtual ExitCode Acquisition32BitsFloatRGB();

char EMGain8Bit();
char EMGain12Bit();
char EMGainLinear12Bit();
char EMGainReal12Bit();
char GetFunctionEMCCDGain();
virtual	char IsEMCCD(){if (TestDriver()) return true;return false;};
virtual void DefEMCCDGain(int);
virtual void DefModeGain(int);
virtual void LireIntervalGain(int *x,int *y);
virtual double TempsExposition (){return tempsExposition;};
virtual float TempsPose(){return 0;};
virtual float GainEMCCD(){return valGain;};
virtual int ModeGainEMCCD(){return 0;};




};


wxDECLARE_EVENT(VAL_EVT_PTS_SUIVIS, EvtPointSuivis);

#endif
