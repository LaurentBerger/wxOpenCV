#ifndef __FENETRESEQOPE__
#define __FENETRESEQOPE__

#include "FenetrePrincipale.h"
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/string.h>
#include "wx/notebook.h"



/**  La classe FenetreAlgo permet de changer les paramètres des étapes d'un algorithme.

*/


class FenetreSequenceOperation : public  wxFrame
{
private :
FenetrePrincipale				*fenMere;
void							*osgApp;
std::map<wxWindow*,std::pair<wxString,int> >	listeOnglet;/*<! Onglet associé à chaque opération */
std::vector<std::pair<Operation*,int> >			listeOp;	/*<! Onglet associé à chaque opération */

int								nbParamMax;	/*<! Nombre de parametre maximum de l'ensemble de opérations */
int								nbEtape;	/*<! Nombre d'étape nécessaire pour effectuer l'opération */
wxPanel							*panneau;	/*!< Pointeur sur le panneau contenant le classeur */
wxNotebook						*classeur;	/*!< le classeur avec ses différents onglets*/

public :
FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size,wxOsgApp *, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
~FenetreSequenceOperation();
        /*!< destructeur de la fenetre parametrage */
wxWindow *CreerOngletEtape(wxNotebook *,int);
        /*!< Création d'un onglet pour une étape */
// Gestion des évènements
void OnActivate(wxActivateEvent& event);
		/*!< Gestion pour maj palette */
void OnPaint(wxPaintEvent& event);
		/*!< Gestion pour maj palette */
void OnSpinEntier(wxSpinEvent &w);
		/*!< Gestion des boutons pour valeurs entières */
void OnSpinReel(wxSpinDoubleEvent &w);
		/*!< Gestion des boutons pour valeurs réelles */
void  OnSpinMoins(wxSpinEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */
void  OnSpinPlus(wxSpinEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */

void OnTextValider(wxCommandEvent &w);
void OnKeyDown(wxKeyEvent &);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */


void DefOSGApp(void *w){osgApp=w;};
void DefFenMere(FenetrePrincipale *f){fenMere =f;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres éléments. */
void ExecuterOperation(int indEtape);/*!<Excute l'opération après modification des paramètres à partir de l'étape indEtape */
public:
};

#endif
