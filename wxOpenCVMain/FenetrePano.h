#ifndef __FENETREPANO__
#define __FENETREPANO__

#include "FenetrePrincipale.h"
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/string.h>
#include "wx/notebook.h"



/**  La classe FenetreAlgo permet de changer les param�tres des �tapes d'un algorithme.

*/


class FenetrePano : public  wxFrame
{
private :
FenetrePrincipale				*fenMere;
void							*osgApp;
std::map<wxWindow*,std::pair<wxString,int> >	listeOnglet;/*<! Onglet associ� � chaque op�ration */
std::vector<std::pair<ParametreOperation*,int> >		listeOp;	/*<! Onglet associ� � chaque op�ration */

int								nbParamMax;	/*<! Nombre de parametre maximum de l'ensemble de op�rations */
int								nbEtape;	/*<! Nombre d'�tape n�cessaire pour effectuer l'op�ration */
wxPanel							*panneau;	/*!< Pointeur sur le panneau contenant le classeur */
wxNotebook						*classeur;	/*!< le classeur avec ses diff�rents onglets*/
wxSize							tailleMax;	/*!< taille maximale de la fen�tre */

public :
	FenetrePano(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
        const wxSize& size,wxOsgApp *, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
	~FenetrePano();
        /*!< destructeur de la fenetre parametrage */
wxWindow *CreerOngletEtape(wxNotebook *,int);
        /*!< Cr�ation d'un onglet pour une �tape */
// Gestion des �v�nements
void OnActivate(wxActivateEvent& event);
		/*!< Gestion pour maj palette */
void OnPaint(wxPaintEvent& event);
		/*!< Gestion pour maj palette */
void OnSpinEntier(wxSpinEvent &w);
		/*!< Gestion des boutons pour valeurs enti�res */
void OnSpinReel(wxSpinDoubleEvent &w);
		/*!< Gestion des boutons pour valeurs r�elles */
void  OnSpinMoins(wxSpinEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */
void  OnSpinPlus(wxSpinEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */
void SauverSequence(wxCommandEvent &evt);
	/*!< Sauvegarde des �tapes comme s�quence */
void ComboBox(wxCommandEvent &evt);
	/*!<S�lection d'un parametre � l'aide de sa description */
void OnTextValider(wxCommandEvent &w);
void OnKeyDown(wxKeyEvent &);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */


void DefOSGApp(void *w){osgApp=w;};
void DefFenMere(FenetrePrincipale *f){fenMere =f;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres �l�ments. */
void ExecuterOperation(int indEtape);/*!<Excute l'op�ration apr�s modification des param�tres � partir de l'�tape indEtape */
public:
};

#endif
