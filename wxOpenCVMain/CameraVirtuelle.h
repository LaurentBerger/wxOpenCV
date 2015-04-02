#ifndef _CAMERAVIRTUELLE_H_
#define _CAMERAVIRTUELLE_H_

#include <string>
#include <vector>
#include <limits.h>
#include "opencv2/opencv.hpp"
#include <wx/wx.h>
#include "ParametreOperation.h"

#define NB_COEFF_FILTRE 11
#define NB_TAILLE_VIDEO 20
/*! \class EvtPointSuivis
   * \brief la classe d�finit un les caract�ristiques d'un �v�nement
   *
   *  La classe EvtPointSuivis d�finit un nouveau type d'�v�nement utilis� pour informer la fen�tre qu'une 
   * nouvelle image issue d'un flux vid�o est disponible. L'�v�nement est envoy� � la fen�tre g�rant l'affichage du flux vid�o.
   */
class EvtPointSuivis : public wxCommandEvent
{
public:
    EvtPointSuivis(wxEventType commandType, int id = 0);
    EvtPointSuivis(const EvtPointSuivis &evt):wxCommandEvent(evt)
	{
		m_Expediteur = evt.m_Expediteur;
		m_sortie = evt.m_sortie;
		m_travailEchoue = evt.m_travailEchoue;
	}

    virtual wxEvent* Clone() const
    {
        return new EvtPointSuivis(*this);
    }

	std::vector<cv::Point2f>	ptId;	/*!<  Points identifi�s dans l'image a v�rifier */ 
	std::vector<cv::Point2f>	ptApp;	/*!<  Points identifi�s dans l'image a v�rifier */
	void*					m_Expediteur;
    bool					m_sortie;
    bool					m_travailEchoue;
	int						indEvt;		/*!< Nombre d'�v�nement envoy� */
};

typedef void (wxEvtHandler::*EvtPointSuivisFonction)(EvtPointSuivis&);
#define GstEvtPointSuivis(func) wxEVENT_HANDLER_CAST(EvtPointSuivisFonction, func)   

/*! \class CameraVirtuelle
   * \brief classe permettant de capturer un flux vid�o
   *
   *  La classe CameraVirtuelle permet de capturer un flux vid�o reconnu par OpenCV ou par une cam�ra Andor(non maintenu). Un thread est cr�e envoyant un �venement 
   * � la fen�tre affichant l'image pour indiquer qu'une nouvelle image est disponible.
   */

class CameraVirtuelle : public wxThread{
public:
std::vector <ParametreOperation> seqOp;
bool		seqActualisee;/*<!Vrai lorsque les param�tres de la s�quence sont modifi�s*/

protected :
virtual wxThread::ExitCode Entry(){return 0;};

/*!< S�quence d'op�ration � appliquer sur une image recopier � partir de la fenetre m�re */

char			testDriver; /*!< test de la pr�sence du driver*/

unsigned int	erreurFct; /*!< num�ro d'erreur */
unsigned int	drapeauErreur; /*!< activation d'erreur*/
std::string		chaineErreur; /*!< Message d'erreur*/

char			nomCamera[256];/*!<  IDENTIFICATEUR CAMERA*/
int				typeAcq;		/*!< Type de l'acquisition CV_UC3 CV_32_FC3 */
int				nbColonnePhys; /*!< NOMBRE DE colonne PHYSIQUE DU DETECTEUR*/

int				nbLignePhys;/*!<NOMBRE DE ligne PHYSIQUE DU DETECTEUR*/

double		aaButter[NB_COEFF_FILTRE],bbButter[NB_COEFF_FILTRE];
int			indFiltreMoyenne;	/*!< Indice des coefficients du filtre Butterworth*/ 
bool		modeMoyenne;		/*!< Moyenne temporelle active lors de l'acquisition */
long		tpsInactif;
wxSize		tailleCapteur[NB_TAILLE_VIDEO];
bool		tailleAutorisee[NB_TAILLE_VIDEO];	
bool		reglageTaille;
bool		reglageGain;
bool		reglageContraste;
bool		reglageLuminosite;
bool		reglageSaturation;
bool		reglageTpsExpo;
	

public :
void				*parent;		/*!< Pointeur sur la fen�tre principale*/
wxBitmap			m_bitmap;		/*!< bitmap inutilis�*/
wxImage				*image;			/*!< Image contenant la vid�o*/



public :
CameraVirtuelle(void): wxThread(wxTHREAD_DETACHED){testDriver=0;drapeauErreur=0;reglageTaille=false;reglageTpsExpo=false;
reglageGain=false;reglageContraste=false;reglageLuminosite=false;reglageSaturation=false;modeMoyenne=false;};

void AjouteMsgErreur(char *msg){chaineErreur+=msg;drapeauErreur+=1;};
void AjouteMsgErreur(char *msg,unsigned int x){chaineErreur+=msg;drapeauErreur+=1;};

virtual wxSize* LitTailleCapteur(){return tailleCapteur;};
virtual bool*	LitTailleAutorisee(){return tailleAutorisee;};
virtual bool Connectee(){return 0;};
virtual	char TestDriver(void){return testDriver;};
virtual int Acquisition(void){return 0;};
virtual char* NomCamera(){return nomCamera;};
virtual int NbColonnePhys(){ return nbColonnePhys;};
virtual int NbLignePhys(){return nbLignePhys;};
virtual int NbColonne(){ return 0;};
virtual int NbLigne(){return 0;};
virtual int NbCanaux(){return 0;};
virtual void DefTypeAcq(int x){typeAcq=x;};
virtual long DefTpsInactif( long  x=-1){if (x!=-1) tpsInactif=x;return tpsInactif;};
virtual void DefCoinGauche(int){return;};
virtual void DefCoinDroit(int){return;};
virtual void DefCoinHaut(int){return;};
virtual void DefCoinBas(int){return;};
virtual void DefEMCCDGain(int){return;};
virtual void DefModeGain(int){return;};
virtual void LireIntervalGain(int *x,int *y){*x=0;*y=0;};
virtual void DefTempsExposition(double){return;};
virtual void DefNombreAccumulation(int){return;};
virtual void DefFrequenceAccumulation(double){return;};
virtual void DefNombreCinetique(int){return;};
virtual void DefFrequenceCinetique(double){return;};
virtual void DefTailleImage(int){return;};
virtual void DefBinning(int){return;};
virtual void DefModeAcquisition(int ){return;};
virtual void DefModeDeclenchement(int){return;};
virtual void DefZoneLecture(int ){return;};
virtual float GainEMCCD(){return 0;};
virtual int ModeGainEMCCD(){return -1;};
virtual char ImagePrete(){return 0;};
virtual char Image(unsigned short *data,unsigned long &taille){taille=0;return 1;};
virtual char TailleImage(unsigned long &taille){taille=0;return 1;};;
virtual	char IsEMCCD(){return false;};
virtual	char Erreur(){return drapeauErreur;};

virtual void ActiveModeMoyenne(){modeMoyenne=true;};
virtual void DesActiveModeMoyenne(){modeMoyenne=false;};
virtual bool ModeMoyenne(){return modeMoyenne;};
virtual int IndFiltreMoyenne(){return indFiltreMoyenne;};
virtual void DefIndFiltreMoyenne(int x){indFiltreMoyenne=x;};

bool ReglageTaille(){return reglageTaille;};
bool ReglageGain(){return reglageGain;};
bool ReglageContraste(){return reglageContraste;};
bool ReglageLuminosite(){return reglageLuminosite;};
bool ReglageSaturation(){return reglageSaturation;};
bool ReglageTpsExpo(){return reglageTpsExpo;};
virtual double TempsExposition(double =DBL_MAX){return DBL_MAX;} ;
virtual double Gain(double =DBL_MAX){return DBL_MAX;};
virtual double Contraste(double =DBL_MAX){return DBL_MAX;};
virtual double Luminosite(double =DBL_MAX){return DBL_MAX;};
virtual double Saturation(double =DBL_MAX){return DBL_MAX;};
virtual bool DefTailleImage(int largeur,int hauteur){return false;};

};

#endif