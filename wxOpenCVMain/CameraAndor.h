#ifndef _CAMERAANDOR_H_
#define _CAMERAANDOR_H_
#include "CameraVirtuelle.h"
#ifdef __LIBANDOR__
#include "D:\lib\Andor\ATMCD32D.H"        		// Andor functions


class CameraAndor : public CameraVirtuelle {
private :
// PROPRIETES DE LA CAMERA
AndorCapabilities caps;			// AndorCapabilities structure p88
// MODE DE LECTURE DU CAPTEUR
int	modeLecture;				// Mode de lecture du capteur p22-27 SDK Andor
	// Mode Single track (pour spectroscopie)
int	pisteCentrale;				// piste ensemble de lignes
int	nbLignePiste;				// nombre de ligne composant une piste
	// Mode multi-track (pour spectroscopie)
int	nbPiste;
int	indLignePremierpixel;
int	nbLigneInterPiste;
	// Mode Random Track	
int	*positionPiste;		//ligne début et fin de chaque piste. Le nombre de pistes est défini par nbPiste
	// Mode Image
int	nbColonneParPixel;
int	nbLigneParPixel;
int	colonneDebut;
int	ligneDebut;
int colonneFin;
int	ligneFin;
/////////////////////////////////////////////////////////

// MODE D'ACQUISITION DU CAPTEUR	
int	modeAcquisition;			// Mode d'acquisition du capteur p28-43 SDK Andor
int emCCDGainMode;				// mode utilisé pour le gain de l'EMCCD
	// Mode Single Scan p28-29
float	tempsExposition;
	// Mode Accumulate  p30
int		nbScanParAccumulation;
float	periodeAccumulation;	// en seconde
	// Mode Kinetic 31-32
int	nbAccumulationparCinetique;
float periodeCinetique;			// en seconde
	// Mode Run Till Abort 33-35
	// Mode Fast Kinetic
	// mode Frame transfer
int	frameTransferMode;
/////////////////////////////////////////////////////////

// MODE DU TRIGGER
int	modeDeclenchement;			// Mode de déclenchement de l'acquisition p45 SDK Andor
/////////////////////////////////////////////////////////

// ADChannel
int	nbCannauxAD;				// p 126 SDK

// SHIFT SPEED
int	nbVitesseDecalageVerticale;	// Nombre de Vitesses de décalage verticale du capteur p51 SDK Andor
float	*vitesseDecalageVerticale;
int	nbVitesseDecalageHorizontale;	// Nombre de  vitesses de décalage verticlae du capteur p51 SDK Andor
float **vitesseDecalageHorizontale;	// Dimension donnée par ADNumber  et nbVitesseDecalageHorizontale
/////////////////////////////////////////////////////////

// SHUTTER CONTROL
int	controleObturateur;			// Controle de l'obturateur	p52 SDK Andor
/////////////////////////////////////////////////////////

// TEMERATURE CONTROL
int controleTemperature;		// Controle de la température de refroidissement du CCD
int	temperature;				// Temperature en °C
/////////////////////////////////////////////////////////
public :
CameraAndor(void);
~CameraAndor(void);
int	Acquisition(void){return 0;};
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

char EMGain8Bit();
char EMGain12Bit();
char EMGainLinear12Bit();
char EMGainReal12Bit();
char GetFunctionEMCCDGain();
virtual	char IsEMCCD(){if (TestDriver()) return true;return false;};
virtual void DefEMCCDGain(int);
virtual void DefModeGain(int);
virtual void LireIntervalGain(int *x,int *y);
virtual float TempsExposition (){return tempsExposition;};
virtual float TempsPose(){return 0;};
virtual float GainEMCCD(){int gain;GetEMCCDGain(&gain);return gain;};
virtual int ModeGainEMCCD(){return emCCDGainMode;};




};
#else
typedef int AndorCapabilities ;

class CameraAndor : public CameraVirtuelle {
private :
// PROPRIETES DE LA CAMERA
AndorCapabilities caps;			// AndorCapabilities structure p88
// MODE DE LECTURE DU CAPTEUR
int	modeLecture;				// Mode de lecture du capteur p22-27 SDK Andor
	// Mode Single track (pour spectroscopie)
int	pisteCentrale;				// piste ensemble de lignes
int	nbLignePiste;				// nombre de ligne composant une piste
	// Mode multi-track (pour spectroscopie)
int	nbPiste;
int	indLignePremierpixel;
int	nbLigneInterPiste;
	// Mode Random Track	
int	*positionPiste;		//ligne début et fin de chaque piste. Le nombre de pistes est défini par nbPiste
	// Mode Image
int	nbColonneParPixel;
int	nbLigneParPixel;
int	colonneDebut;
int	ligneDebut;
int colonneFin;
int	ligneFin;
/////////////////////////////////////////////////////////

// MODE D'ACQUISITION DU CAPTEUR	
int	modeAcquisition;			// Mode d'acquisition du capteur p28-43 SDK Andor
int emCCDGainMode;				// mode utilisé pour le gain de l'EMCCD
	// Mode Single Scan p28-29
float	tempsExposition;
	// Mode Accumulate  p30
int		nbScanParAccumulation;
float	periodeAccumulation;	// en seconde
	// Mode Kinetic 31-32
int	nbAccumulationparCinetique;
float periodeCinetique;			// en seconde
	// Mode Run Till Abort 33-35
	// Mode Fast Kinetic
	// mode Frame transfer
int	frameTransferMode;
/////////////////////////////////////////////////////////

// MODE DU TRIGGER
int	modeDeclenchement;			// Mode de déclenchement de l'acquisition p45 SDK Andor
/////////////////////////////////////////////////////////

// ADChannel
int	nbCannauxAD;				// p 126 SDK

// SHIFT SPEED
int	nbVitesseDecalageVerticale;	// Nombre de Vitesses de décalage verticale du capteur p51 SDK Andor
float	*vitesseDecalageVerticale;
int	nbVitesseDecalageHorizontale;	// Nombre de  vitesses de décalage verticlae du capteur p51 SDK Andor
float **vitesseDecalageHorizontale;	// Dimension donnée par ADNumber  et nbVitesseDecalageHorizontale
/////////////////////////////////////////////////////////

// SHUTTER CONTROL
int	controleObturateur;			// Controle de l'obturateur	p52 SDK Andor
/////////////////////////////////////////////////////////

// TEMERATURE CONTROL
int controleTemperature;		// Controle de la température de refroidissement du CCD
int	temperature;				// Temperature en °C
/////////////////////////////////////////////////////////
public :
CameraAndor(void){};
~CameraAndor(void);
int	Acquisition(void){return 0;};
virtual void DefCoinGauche(int x){};
virtual void DefCoinDroit(int x){};
virtual void DefCoinHaut(int x){};
virtual void DefCoinBas(int x){};
virtual void DefTempsExposition(double){};
virtual void DefNombreAccumulation(int){};
virtual void DefFrequenceAccumulation(double){};
virtual void DefNombreCinetique(int){};
virtual void DefFrequenceCinetique(double){};
virtual void DefTailleImage(int){};
virtual void DefBinning(int){};
virtual void DefModeAcquisition(int ){};
virtual void DefModeDeclenchement(int){};
virtual void DefZoneLecture(int ){};

virtual char ImagePrete(){return 0;};
virtual char Image(unsigned short *data,unsigned long &taille){return 0;};
virtual char TailleImage(unsigned long &taille){return 0;};
virtual int NbColonne(){ return colonneFin-colonneDebut+1;};
virtual int NbLigne(){return ligneFin-ligneDebut+1;};

char EMGain8Bit(){return 0;};
char EMGain12Bit(){return 0;};
char EMGainLinear12Bit(){return 0;};
char EMGainReal12Bit(){return 0;};
char GetFunctionEMCCDGain(){return 0;};
virtual	char IsEMCCD(){return false;};
virtual void DefEMCCDGain(int);
virtual void DefModeGain(int);
virtual void LireIntervalGain(int *x,int *y);
virtual float TempsExposition (){return tempsExposition;};
virtual float TempsPose(){return 0;};
virtual float GainEMCCD(){return 0;};
virtual int ModeGainEMCCD(){return emCCDGainMode;};




};

#endif



#endif
