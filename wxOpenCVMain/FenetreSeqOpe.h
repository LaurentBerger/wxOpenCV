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
std::map<int, std::pair< FenetrePrincipale* ,int> >			idFenetre;	/*<! fenetre crée par l'éxécution de la séquence*/

wxPanel							*panneau;	/*!< Pointeur sur le panneau contenant le classeur */
wxNotebook						*classeur;	/*!< le classeur avec ses différents onglets*/
wxString						*nomEtape;	/*!< Nom des étapes dans la listeBox */
int								nbEtape;	/*!< nombre de chaines maximum */
wxListBox						*choixOp;
wxArrayString					fichierSelectionnes; 

wxSize							tailleMax;	/*!< taille maximale de la fenêtre */
public :
FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size,wxOsgApp *, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
~FenetreSequenceOperation();
        /*!< destructeur de la fenetre parametrage */
void InsererCtrlEtape(ParametreOperation *);
        /*!< Création des Ctrl pour une étape de la séquence*/
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
void  OnOpeSelec(wxCommandEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */
void  SelectionFichier(wxCommandEvent& event);
	/*!< Gestion des boutons pour valeurs avec entier pas autre que 1 */

void OnTextValider(wxCommandEvent &w);
void OnKeyDown(wxKeyEvent &);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */


void DefOSGApp(void *w){osgApp=w;};
void DefFenMere(FenetrePrincipale *f){fenMere =f;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres éléments. */
void Executer(wxCommandEvent& event);/*!<Excute la séquence d'opération après modification des paramètres à partir de l'étape indEtape */
void ExecuterSequence(std::vector <ParametreOperation> *sq);/*!<Excute la séquence d'opération après modification des paramètres à partir de l'étape indEtape */
bool OperandePresent(std::vector <ParametreOperation> *sq);/*!< Verifie que les opérandes 1 et 2 sont disponibles*/

public:
};

#endif
