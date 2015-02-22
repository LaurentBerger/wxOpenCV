#ifndef __FENETRESEQOPE__
#define __FENETRESEQOPE__

#include "FenetrePrincipale.h"
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/string.h>
#include "wx/notebook.h"



/**  La classe FenetreAlgo permet de changer les param�tres des �tapes d'un algorithme.

*/


class FenetreSequenceOperation : public  wxFrame
{
private :
FenetrePrincipale				*fenMere;
void							*osgApp;
std::map<wxWindow*,std::pair<wxString,int> >	listeOnglet;/*<! Onglet associ� � chaque op�ration */
std::vector<std::pair<Operation*,int> >			listeOp;	/*<! Onglet associ� � chaque op�ration */

wxPanel							*panneau;	/*!< Pointeur sur le panneau contenant le classeur */
wxNotebook						*classeur;	/*!< le classeur avec ses diff�rents onglets*/
wxString						*nomEtape;	/*!< Nom des �tapes dans la listeBox */
int								nbEtape;	/*!< nombre de chaines maximum */
wxListBox						*choixOp;

wxSize							tailleMax;	/*!< taille maximale de la fen�tre */
public :
FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size,wxOsgApp *, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
~FenetreSequenceOperation();
        /*!< destructeur de la fenetre parametrage */
void InsererCtrlEtape(Operation *);
        /*!< Cr�ation des Ctrl pour une �tape de la s�quence*/
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
void  OnOpeSelec(wxCommandEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */

void OnTextValider(wxCommandEvent &w);
void OnKeyDown(wxKeyEvent &);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */


void DefOSGApp(void *w){osgApp=w;};
void DefFenMere(FenetrePrincipale *f){fenMere =f;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres �l�ments. */
void Executer(wxCommandEvent& event);/*!<Excute la s�quence d'op�ration apr�s modification des param�tres � partir de l'�tape indEtape */
void ExecuterSequence(std::vector <Operation> *sq);/*!<Excute la s�quence d'op�ration apr�s modification des param�tres � partir de l'�tape indEtape */
public:
};

#endif
