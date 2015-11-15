#ifndef _CONTROLECAMERA_H_
#define _CONTROLECAMERA_H_

#include "wx/notebook.h"
#include <wx/spinbutt.h>

#include "CameraVirtuelle.h"
/**  La classe ControleCamera permet le controle des paramètres d'une caméra. .

*/

#define NBCURSEUR 12

class ControleCamera : public wxFrame 
{
private :
wxPanel			*panneau;			/*!< Pointeur sur le panneau contenant les différents onglets*/
wxBoxSizer		*m_sizerFrame;		/*!< */
wxNotebook		*listeFenetreOnglet;/*!< Pointeur pour accès aux onglets*/

wxWindow		*ongletStatus;		/*!< Pointeur pour accès informations sur l'echelle de la séquence */
wxWindow		*ongletGeometries;	/*!< Pointeur pour réglages de la geométrie de l'acquisition */
wxWindow		*ongletTemporels;	/*!< Pointeur pour réglages des temps d'acquisitions */
wxWindow		*ongletFond;		/*!< Pointeur pour gestion du fond */
wxWindow		*ongletMagneto;		/*!< Pointeur pour gestion du fond */
wxWindow		*ongletQuadrique;		/*!< Pointeur pour gestion de l'estimation d'une quadrique définie par zones pour un seul niveau*/
wxWindow		*ongletQuadriqueMulti;	/*!< Pointeur pour  l'estimation d'une quadrique définie par zones pour plusieurs niveaux*/
wxWindow		*ongletMoyenne;			/*!< Pointeur pour gestion du moyennage image */
wxWindow		*ongletEMCCD;			/*! DEASCTIVE */
CameraVirtuelle *cam;
void			*parent;
void			*osgApp;
wxSlider		*slEMGain;
wxButton		*autoAdjust;

char			commEvt;			/*!< 1->Le thread communiquera avec main par l'événement 358 sinon activera le flag image prête*/
char			imagePrete;

public :
ControleCamera(wxFrame *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
// Gestion des évènements
void OnSlider(wxScrollEvent &w);
		/*!< Gestion des glissieres */
void OnSpinUp(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnSpinDown(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnChoice( wxCommandEvent& event );
		/*!< Gestion des menus choix */
void OnCaseCocher( wxCommandEvent& event );
		/*!< Gestion des cases à cocher*/
void OnTextValider(wxCommandEvent &w);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */
void NouvelleImage(wxCommandEvent& );
		/*! Evenement déclenché lors de l'arrivée d'une nouvelle image */
void EstimationGain(wxCommandEvent& );
		/*! Evenement déclenché pour débuter ou arrêter l'estimation du gain */
void EstimationFond(wxCommandEvent& event);
	/*! Evenement déclenché pour débuter ou arrêter l'estimation du fond */
void ExpositionAutomatique(wxCommandEvent&);
		/*! Détermination d'un temps de pose maximisant la fonction contraste xxx */
void ModeMoyenne(wxCommandEvent &w);
		/*! Activation du filtre de butterworth */
void Play(wxCommandEvent&);
		/*! Jouer la vidéo */
void Record(wxCommandEvent&);
		/*! Enregistrer */
void Pause(wxCommandEvent&);
		/*! pause */
void DebutAcquisition(void);
		/*!< Debut de l'acquisition video */
void FinAcquisition(void);

float TempsExposition(){return cam->TempsExposition();};
float GainEMCCD(){return cam->GainEMCCD();};	
int	 ModeGainEMCCD(){return cam->ModeGainEMCCD();};
void DefTempsExposition(float x);
void DefGainEMCCD(float x);	
void DefModeGainEMCCD(int x);	
		
void DefOSGApp(void *w){osgApp=w;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres éléments. */
void DefParent(void *w){parent=w;};
		/*!< Definition du pointeur sur la fenêtre contenant le flux video. */
void OuvertureOngletStatus();
void OuvertureOngletFond();
void OuvertureOngletParametresTemporels();
void OuvertureOngletParametresGeometries();
void OuvertureOngletMagneto();
void OuvertureOngletMoyenne();
void OuvertureOngletEMCCD();

void	DrawOngletStatus();

CameraVirtuelle	*Camera(){return cam;};
void	DefCamera(CameraVirtuelle *c);
void	DefImagePrete(char	x){imagePrete=1;};
char	CommunicationEvenement(){return commEvt;};
char	ImagePrete(void);
char	Image();
int		NbColonne(){ return cam->NbColonne();};
int		NbLigne(){return cam->NbLigne();};
void	PThread(void *);

public:
    DECLARE_EVENT_TABLE()
};

#endif
