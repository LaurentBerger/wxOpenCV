#ifndef _CAMERAOPENCV_H_
#define _CAMERAOPENCV_H_

#include "CameraVirtuelle.h"


#define NBCAMERA 10
#define NBFILTRE 11 // Coefficent du filtre de butterworth

/*! \class CameraOpenCV
   * \brief La classe permet de g�rer un flux vid�o reconnu par OpenCV
   *
   */

class CameraOpenCV : public CameraVirtuelle {
protected :
virtual wxThread::ExitCode Entry();
static char	indIdVideo[NBCAMERA];
int			indId; // Indice du p�riph�rique vid�o;
long		expoMin,expoMax;
long		gainMin,gainMax;

double aaButter[11];
double bbButter[11];


ImageInfoCV			*nivBiais;			/*!< image du niveau z�ro*/
ImageInfoCV			*imAcqBrutFilMax;	/*!< r�sultat de la moyenne glissante avec filtrage maximum imAcqBrutFil = b(imAcqBrut1+imAcqBrut2)-a imAcqBrutFil */
ImageInfoCV			*imAcqBrutFil;		/*!< r�sultat de la moyenne glissante imAcqBrutFil = b(imAcqBrut1+imAcqBrut2)-a imAcqBrutFil */
ImageInfoCV			*imAcqBrut1;		/*!< Derni�re image acquise � ins�rer dans la moyenne glissante*/
ImageInfoCV			*imAcqBrut2;		/*!< Avant Derni�re image acquise � ins�rer dans la moyenne glissante*/
ImageInfoCV			*imAcq2;			/*!< Derni�re image calcul�e incluant les corrections avec filtre maximum */
ImageInfoCV			*imTache;			/*!< Image du gain � appliquer pour supprimer les t�ches optiques ind�pendantes de l'�chantillon */
ImageInfoCV			*imRefTache;		/*!< Image des t�ches optiques ind�pendantes de l'�chantillon */
ImageInfoCV			*imQuadrique;		/*!< Valeur de correction l'intensit� trouv�es � partir de la quadrique */



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
int	modeDeclenchement;			// Mode de d�clenchement de l'acquisition p45 SDK Andor
/////////////////////////////////////////////////////////

// ADChannel
int	nbCanauxAD;				// p 126 SDK

//////////////////////////////////////////////////////////
public :
CameraOpenCV(void);
~CameraOpenCV(void);
int	Acquisition(void){return 0;};
virtual bool Connectee(){return indId>=0 && indId<NBCAMERA;};
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

#endif
