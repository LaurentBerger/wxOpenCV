#ifndef _IMAGESTAT_H_
#define _IMAGESTAT_H_

#include <wx/notebook.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/string.h>
#include "Tableur.h"
#include "courbeplplot.h"
#include "FenetreRegion.h"



/**  La classe ImageStatitisques permet l'affichage de l'histogramme, des param�tres stat.. .

*/


class ImageStatistiques : public wxFrame 
{
private :
FenetrePrincipale	*fenMere;
Tableur				*curseur;
Tableur				*excelPalette;
wxPanel				*panneau;			/*!< Pointeur sur le panneau contenant les diff�rents onglets*/
wxBoxSizer			*m_sizerFrame;		/*!< */
wxNotebook			*listeFenetreOnglet;/*!< Pointeur pour acc�s aux onglets*/

wxWindow				*ongletStatus;		/*!< Pointeur pour acc�s informations sur l'echelle de la s�quence */
wxWindow			    *ongletCouleur;		/*!< Pointeur pour acc�s informations sur l'echelle de la s�quence */
FenetreFocus			*ongletFocus;		/*!< Pointeur pour acc�s informations sur Focus variance fft */
FenetreHistogramme		*ongletHistogramme;	/*!< Pointeur pour acc�s informations sur hsitogramme image */
FenetreDistribRadiale	*ongletDistribRadiale; /*!< Pointeur pour acc�s informations sur distrib radiale */
FenetreDistribAngulaire	*ongletDistribAngulaire; /*!< Pointeur pour acc�s informations sur distrib radiale */
FenetreCoupe			*ongletCoupe;	/*!< Pointeur pour acc�s informations sur hsitogramme image */
wxWindow				*ongletPalette; /*!< Pointeur pour la fenetre avec la paette */
wxWindow				*ongletCurseur; /*!< tableur des donn�es proche du curseur */
FenetreRegion			*ongletRegionR; /*!< Pour le regroupement des r�gions avant recherche quadrique */
FenetreRegion			*ongletRegionV; /*!< Pour le regroupement des r�gions avant recherche quadrique */
FenetreRegion			*ongletRegionB; /*!< Pour le regroupement des r�gions avant recherche quadrique */
std::map <wxString,FenetreRegion*> ongletKeyPt; /*!< Onglets associ�s aux descripteurs */
std::map <wxString,FenetreRegion*> ongletContour; /*!< Onglets associ�s aux contours */
std::map <wxString,FenetreRegion*> ongletCmpConnexe; /*!< Onglets associ�s aux composantes connexes */
wxSlider				**slNivMin;				/*!< R�glage du niveau bas de la palette par canal*/
wxSlider				**slGain;				/*!< R�glage du niveau haut de la palette par canal*/
wxSpinCtrl				*spAjustAuto;			/*!< Ajustement auto de la palette avec % de satureation */
bool                    jamaisVue;              /*!< La fen�tre n'a jamais �t� montr�e sur l'�cran*/

void  *osgApp;

public :
ImageStatistiques(wxFrame *frame, const wxString& title, const wxPoint& pos, 
        const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
        /*!< Constructeur de la fenetre parametrage */
~ImageStatistiques();
        /*!< destructeur de la fenetre parametrage */
void SauverHistogrammeSVG(wxString &s){ongletHistogramme->SauverSVG(s);};
        /*!< Sauvegarde de l'histogramme au format SVG */
int IdFenetre(){return fenMere->IdFenetre();};
// Gestion des �v�nements
void OnActivate(wxActivateEvent& event);
		/*!< Gestion pour maj palette */
void OnPaint(wxPaintEvent& event);
		/*!< Gestion pour maj palette */
void OnSlider(wxScrollEvent &w);
		/*!< Gestion des glissieres */
void OnSpinUp(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnSpinDown(wxScrollEvent &w);
		/*!< Gestion des boutons spins */
void OnSpinCtrl(wxSpinEvent &w);
		/*!< Gestion des boutons spins */
void OnTextValider(wxCommandEvent &w);
	void OnKeyDown(wxKeyEvent &);

void OnClose(wxCloseEvent& event);
		/*!< Fermeture de la fenetre parametrage */
void OnNotebook(wxNotebookEvent& event) ;
		/*!< Gestion des �v�nements notebook */

void OnSelectCell(wxGridEvent &w);
void OnRangeSelected(wxGridRangeSelectEvent &w);


void DefOSGApp(void *w){osgApp=w;};
void DefFenMere(FenetrePrincipale *f){fenMere =f;};
		/*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres �l�ments. */
void OuvertureOngletKeyPt(std::vector <cv::KeyPoint> *p,wxString titre);
void OuvertureOngletContour(std::vector<std::vector<cv::Point> > *p,wxString titre);
void OuvertureOngletCouleur();
void OuvertureOngletStatus();
void OuvertureOngletHistogramme();
void OuvertureOngletDistribRadiale();
void OuvertureOngletDistribAngulaire();
void OuvertureOngletCoupe();
void OuvertureOngletFocus();
void OuvertureOngletPalette();
void OuvertureOngletRegion(); 
void OuvertureOngletCurseur();
void DrawPaletteActuelle();
void DrawPalette();
void OuvertureOngletCouleur(char modeMoyenne,int x);
// Acc�s aux membres priv�s
wxWindow* OngletStatus(){return ongletStatus;};
wxWindow* OngletFocus(){return ongletFocus;};
wxWindow* OngletCouleur(){return ongletCouleur;};
wxWindow* OngletHistogramme(){return ongletHistogramme;};
Tableur *TableurHistogramme(){return ongletHistogramme->Grille();};

void MAJOnglet(int indOnglet);
		/*!< Mise � jour des onglets lors d'un changement de donn�es */

void MAJValeurPalette();
		/*!< Mise � jour des valeurs de la palette */
void MAJMinMaxHisto();
		/*!< mise � jour min et max */
void MAJStatRec(int ind,float *t);
void AfficheValCurseur(ImageInfoCV *, int ,int );
void RAZStat();
void MAJInfo(int art,char *info);
void MAJInfo(int art,int d);
void MAJInfo(int art,wxString ss);
void ListerRegion(); 
void SelectRegion(int w,int plan); 
void LireNiveau(int *x){}; 
bool JamaisVue(bool x=true){if (!x) jamaisVue=false;return jamaisVue;};

wxString LireInfo(int art);

void MAJCoupe(wxRect &);
void Plot(bool b); /*!< true si mise � jour des courbes n�cessaires*/
cv::Mat	*Histogramme(){return ongletHistogramme->Histogramme();};
public:
    DECLARE_EVENT_TABLE()
};

#endif
