#ifndef _CONTROLECAMERA_H_
#define _CONTROLECAMERA_H_

#include "wx/notebook.h"
#include <wx/spinbutt.h>

#include "CameraVirtuelle.h"
/**  La classe ControleCamera permet le controle des param�tres d'une cam�ra. .

*/

#define NBCURSEUR 12

class ControleCamera : public wxFrame 
{
private :
wxPanel			*panneau;			/*!< Pointeur sur le panneau contenant les diff�rents onglets*/
wxBoxSizer		*m_sizerFrame;		/*!< */
wxNotebook		*listeFenetreOnglet;/*!< Pointeur pour acc�s aux onglets*/

wxWindow		*ongletStatus;		/*!< Pointeur pour acc�s informations sur l'echelle de la s�quence */
wxWindow		*ongletGeometries;	/*!< Pointeur pour r�glages de la geom�trie de l'acquisition */
wxWindow		*ongletTemporels;	/*!< Pointeur pour r�glages des temps d'acquisitions */
wxWindow		*ongletFond;		/*!< Pointeur pour gestion du fond */
wxWindow		*ongletMagneto;		/*!< Pointeur pour gestion du fond */
wxWindow		*ongletQuadrique;		/*!< Pointeur pour gestion de l'estimation d'une quadrique d�finie par zones pour un seul niveau*/
wxWindow		*ongletQuadriqueMulti;	/*!< Pointeur pour  l'estimation d'une quadrique d�finie par zones pour plusieurs niveaux*/
wxWindow		*ongletMoyenne;			/*!< Pointeur pour gestion du moyennage image */
wxWindow		*ongletEMCCD;			/*! DEASCTIVE */
CameraVirtuelle *cam;
void			*parent;
void			*osgApp;
wxSlider		*slEMGain;
wxButton		*autoAdjust;

char			commEvt;			/*!< 1->Le thread communiquera avec main par l'�v�nement 358 sinon activera le flag image pr�te*/
char			imagePrete;

public :
ControleCamera(wxFrame *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
// Gestion des �v�nements
void OnSlider(wxScrollEvent &w);
		/*!< Gestion des glissieres */
void OnSpinUp(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnSpinDown(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnChoice( wxCommandEvent& event );
		/*!< Gestion des menus choix */
void OnCaseCocher( wxCommandEvent& event );
		/*!< Gestion des cases � cocher*/
void OnTextValider(wxCommandEvent &w);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */
void NouvelleImage(wxCommandEvent& );
		/*! Evenement d�clench� lors de l'arriv�e d'une nouvelle image */
void EstimationGain(wxCommandEvent& );
		/*! Evenement d�clench� pour d�buter ou arr�ter l'estimation du gain */
void EstimationFond(wxCommandEvent& event);
	/*! Evenement d�clench� pour d�buter ou arr�ter l'estimation du fond */
void ExpositionAutomatique(wxCommandEvent&);
		/*! D�termination d'un temps de pose maximisant la fonction contraste xxx */
void ModeMoyenne(wxCommandEvent &w);
		/*! Activation du filtre de butterworth */
void Play(wxCommandEvent&);
		/*! Jouer la vid�o */
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
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres �l�ments. */
void DefParent(void *w){parent=w;};
		/*!< Definition du pointeur sur la fen�tre contenant le flux video. */
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
