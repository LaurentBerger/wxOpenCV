#ifndef __INTERFACEAVANCE__
#define __INTERFACEAVANCE__


#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

#include "wx/aui/aui.h"

#include <vector>
// -- application --



class wxSizeReportCtrl;

// -- frame --

class Commande {
public :
int			idEvt;
wxString	nomFichierIcone;
wxString	chaineAide;
wxString	chaineOperation;
int			numBarreoutils;
wxBitmap	bitmap;

Commande(void){};
Commande(int a,const char *b[], wxString c, wxString d, int e) ;
};

class InterfaceAvance : public wxFrame
{
    enum
    {
        ID_CreateTree = wxID_HIGHEST+1,
		ID_RAZIHM,
		ID_ParamMorph,
		ID_ParamCondLimit,
        ID_CreateGrid,
        ID_CreateText,
        ID_CreateHTML,
        ID_CreateNotebook,
        ID_CreateSizeReport,
        ID_GridContent,
        ID_TextContent,
        ID_TreeContent,
        ID_HTMLContent,
        ID_NotebookContent,
        ID_SizeReportContent,
 //       ID_CreatePerspective,
 //       ID_CopyPerspectiveCode,
        ID_Settings,

        ID_SampleItem

 //       ID_FirstPerspective = ID_CreatePerspective+1000
    };

public:
    InterfaceAvance(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~InterfaceAvance();

    wxAuiDockArt* GetDockArt();
    void DoUpdate();
	void DefOSGApp(void *w){osgApp=w;};
	void OnClose(wxCloseEvent& event);
	void InitCommande();
	/*!
	*  \brief InitCommand
	*  Fonction initialisant les commandes avec une commande = icone, num�ro barre d'outils,identificateur d'op�ration,chaine d'aide
	*/
private:
    wxTextCtrl* CreateTextCtrl(const wxString& text = wxEmptyString);
    wxPanel* OngletConvolution(wxWindow* parent = NULL);
    wxPanel* OngletMorphologie(wxWindow* parent = NULL);
    wxTreeCtrl* CreateTreeCtrl();
    wxSizeReportCtrl* CreateSizeReportCtrl(int width = 80, int height = 80);
    wxPoint GetStartPosition();
    wxAuiNotebook* CreerChoixOperateur();
   /*!
     *  \brief Cr�ation des onglets pour le choix des op�rateurs convolutions,
	 *  morphologie et type du r�sultat
     */

	void	*osgApp;			// Application principale
	std::map<int,Commande>	bouton;


private:

    void OnEraseBackground(wxEraseEvent& evt);
    void OnSize(wxSizeEvent& evt);

    void OnCreateTree(wxCommandEvent& evt);
    void OnRazIHM(wxCommandEvent& evt);
    void OnCreateNotebook(wxCommandEvent& evt);
    void OnCreateText(wxCommandEvent& evt);
    void OnCreateSizeReport(wxCommandEvent& evt);
    void OnRestorePerspective(wxCommandEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnTabAlignment(wxCommandEvent &evt);


    void OnPaneClose(wxAuiManagerEvent& evt);
	void InstallationBarreMenu();
	void InstallationbarreOutils(int);

    void OnQuit(wxCommandEvent& event);
// Fonctions li�es aux boutons
	void Ouvrir(wxCommandEvent& event);
    /*!
     *  \brief gestionnaire d'�v�nement du bouton ouvrir
     *
     *  Ouverture d'un document

     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void Video(wxCommandEvent& event);
    /*!
     *  \brief gestionnaire d'�v�nement du bouton video
     *
     *  Ouverture d'une  fen�tre pour l'acquistion video

     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void SelectOperation(wxCommandEvent& event);
    /*!
     *  \brief gestionnaire d'�v�nement des boutons op�rations
     *
     *  Apr�s un clic dans le bouton �rosion, le curseur devient une main et une fen�tre image peut �tre s�lectionn�e.
	 *  Les images sont additionn�es.

     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */

	wxPanel* OngletParamImage(wxWindow* parent = NULL);
    /*!
     *  \brief gestionnaire d'�v�nement pour la liste des param�tres utilis�s dans les calculs
     *
     *  La fen�tre permet de modifier les conditions aux limites (borderInterpolate p247), 
	 *  le type du r�sultat (depth dans les op�rations p256) et l'ancre du noyau (p256) et 
	 *	la valeur du d�calage (delta p256)


     *  \param parent : fen�tre m�re
     */
// Fonctions li�es aux param�trages des filtres
	void DefPrecision(wxCommandEvent &event);
    /*!
     *  \brief gestionnaire d'�v�nement pour la liste des pr�cisions 
     *
	 *  D�finition de la pr�cision du r�sultat
     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void DefBord(wxCommandEvent &event);
    /*!
     *  \brief gestionnaire d'�v�nement pour d�finr les conditions aux limites
     *
	 *  D�finition du mode d'extrapolation des pixels hors image
     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void AfficheFiltre(wxCommandEvent &event);
    /*!
     *  \brief gestionnaire d'�v�nement pour la liste des op�rateurs de convolution
     *
     *  La liste permet l'indice de l'op�rateur. L'op�rateur est affich�e sous forme num�rique dans un tableur.


     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void CtrlMorpho(wxCommandEvent &event);
    /*!
     *  \brief gestionnaire d'�v�nement pour la liste des op�rateurs de morphologie et la forme de cet op�ratuer
     *
     *  Les deux listes permettent de g�rer la forme de l'op�rateur de morphologie et sa taille. L'indice de l'op�rateur. L'op�rateur pr�c�dent de la classe 
     *  ImageInfoCV est d�sallou� et un nouveau est cr�e avec les caract�ristiques d�finies par les cont�les
	 *
     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */
	void AfficheMorpho(int ind);
    /*!
     *  \brief Affichage de l'op�rateur de morphologie
     *
     *  L'op�rateur de morphologie ImageinfoCV::opMorph[ind] est affich� avec un zoom 4 dans l'onglet ongletMorphologie.

	 *  \param ind	Indice de l'op�rateur initialis� dans le tableau ImageinfoCV::opMorph
     */

	void ModifOpMorpho(int type,int taille,int ind);
    /*!
     *  \brief Modifiction d'un op�rateur de morphologie
     *
     *  Le contr�le permet de g�rer la taille de l'op�rateur de morphologie. L'op�rateur pr�c�dent de la classe 
     *  ImageInfoCV est d�sallou� et un nouveau est cr�e avec les caract�ristiques d�finies par les cont�les

     *  \param type : type de l'op�rateur (cv::MORPH_RECT,cv::MORPH_CROSS,cv::MORPH_ELLIPS)
	 *  \param taille l'op�rateur aura une dimmansion de 2*taille+1
	 *  \param ind	Indice de l'op�rateur initialis� dans le tableau ImageinfoCV::opMorph
     */
	void TailleFiltre(wxSpinEvent& event);
   /*!
     *  \brief S�lection d'un op�rateur de morphologie
     *
     *  Le contr�le permet de s�lectionner l'un des op�rateurs de morphologie
	 *
     *  \param event : �v�nement envoy� par le gestionnaire d'�v�nement de WX
     */

	void NouveauFiltre(wxGridEvent& event);
private:

    wxAuiManager m_mgr;
    wxArrayString m_perspectives;
    wxMenu* m_perspectives_menu;
	wxAuiNotebook *pane;
	wxPanel *ongletConvol;
	wxPanel *ongletMorphologie;
	wxPanel *ongletParamImage;
	wxWindow *infoWindow;
    long m_notebook_style;
    long m_notebook_theme;
	wxAuiToolBar *tbFichier; /*< Barre d'outils pour les fichiers*/
	wxAuiToolBar *tbOperation; /*< Barre d'outils pour les op�rations �l�mentaires */
	wxAuiToolBar *tbConv; /*< Barre d'outils pour la convolution */
	wxAuiToolBar *tbMorph; /*< Barre d'outils pour la morphologie */

	int	indFiltreAffiche;
    DECLARE_EVENT_TABLE()
};


// -- wxSizeReportCtrl --
// (a utility control that always reports it's client size)

class wxSizeReportCtrl : public wxControl
{
public:

    wxSizeReportCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     wxAuiManager* mgr = NULL)
                     : wxControl(parent, id, pos, size, wxNO_BORDER)
    {
        m_mgr = mgr;
    }

private:

    void OnPaint(wxPaintEvent& WXUNUSED(evt))
    {
        wxPaintDC dc(this);
        wxSize size = GetClientSize();
        wxString s;
        int h, w, height;

        s.Printf(_("Size: %d x %d"), size.x, size.y);

        dc.SetFont(*wxNORMAL_FONT);
        dc.GetTextExtent(s, &w, &height);
        height += 3;
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawRectangle(0, 0, size.x, size.y);
        dc.SetPen(*wxLIGHT_GREY_PEN);
        dc.DrawLine(0, 0, size.x, size.y);
        dc.DrawLine(0, size.y, size.x, 0);
        dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2));

        if (m_mgr)
        {
            wxAuiPaneInfo pi = m_mgr->GetPane(this);

            s.Printf(_("Layer: %d"), pi.dock_layer);
            dc.GetTextExtent(s, &w, &h);
            dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*1));

            s.Printf(_("Dock: %d Row: %d"), pi.dock_direction, pi.dock_row);
            dc.GetTextExtent(s, &w, &h);
            dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*2));

            s.Printf(_("Position: %d"), pi.dock_pos);
            dc.GetTextExtent(s, &w, &h);
            dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*3));

            s.Printf(_("Proportion: %d"), pi.dock_proportion);
            dc.GetTextExtent(s, &w, &h);
            dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*4));
        }
    }

    void OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
    {
        // intentionally empty
    }

    void OnSize(wxSizeEvent& WXUNUSED(evt))
    {
        Refresh();
    }
private:

    wxAuiManager* m_mgr;

    DECLARE_EVENT_TABLE()
};



#endif
