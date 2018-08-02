#ifndef __GRAPHEOPERATION__
#define __GRAPHEOPERATION__
#include <memory>
#include "FenetrePrincipale.h"
#include "GrapheOperation.h"
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/string.h>
#include "wx/notebook.h"
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/log.h"
#endif

#include "wx/colordlg.h"
#include "wx/numdlg.h"

#include "wx/artprov.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/renderer.h"
#include "wx/wupdlock.h"

#define USE_GENERIC_TREECTRL 0

#if USE_GENERIC_TREECTRL
#include "wx/generic/treectlg.h"
#ifndef wxTreeCtrl
#define wxTreeCtrl wxGenericTreeCtrl
#define sm_classwxTreeCtrl sm_classwxGenericTreeCtrl
#endif
#endif

class GrapheOperation;
class ArboCalcul;

class InfoNoeud : public wxTreeItemData
{
    enum
    {
        NON_INIT=0,OPERATION_VALIDE = 1, OPERATION_SEQUENCE_FICHIER, FENETRE_RESULTAT, FENETRE_DONNEE,
        INDICE_FENETRE_VALIDE, INDICE_FENETRE_LOGIQUE
    };
private:
    wxString m_desc;
    FenetrePrincipale *fen;     // non nul si noeud = fenêtre
    ParametreOperation *pOCV;   // non nul si noeud = opération
    const wxTreeItemId res;     // Noeud de niveau supérieur = résultat NULL si racine
    int indOnglet;
    int indFen;
    int typeNoeud;  
public:
    InfoNoeud(const wxString& desc, FenetrePrincipale *f,const wxTreeItemId pUp) : res(pUp),m_desc(desc), fen(f), indFen(-1),pOCV(NULL),indOnglet(-1) { }
	InfoNoeud(const wxString& desc, ParametreOperation *p, int n, const wxTreeItemId pUp) : res(pUp), m_desc(desc),indFen(-1), fen(NULL), pOCV(p), indOnglet(n) { }
	InfoNoeud(const wxString& desc, wxString &s, int n, const wxTreeItemId pUp) : res(pUp), m_desc(desc), fen(NULL), indFen(n), pOCV(NULL), indOnglet(n) { }

    void ShowInfo(wxTreeCtrl *tree);
    wxString const& GetDesc() const { return m_desc; }
    ParametreOperation *Operation() { return pOCV; };
    int IndiceOnglet() { return indOnglet; };
    int IndiceFenetre() { return indFen; };
    FenetrePrincipale *Fenetre() { return fen; };
    void DefFenetre(FenetrePrincipale *fz) { fen = fz; };
    void DefIndFenetre(int ind) { indFen = ind; };
    void DefTitle(wxString s) { m_desc = s; };
    wxTreeItemId getParent() { return res; };
};

class FenetreInfoOperation
{
private:
    FenetrePrincipale * fenMere;
    void							*osgApp;
    std::map<wxWindow*, std::pair<wxString, int> >	listeOnglet;/*<! Onglet associé à chaque opération */
    std::vector<std::pair<ParametreOperation*, int> >		listeOp;	/*<! Onglet associé à chaque opération */
    wxPanel *wFen;
    int								nbParamMax;	/*<! Nombre de parametre maximum de l'ensemble de opérations */
    int								nbEtape;	/*<! Nombre d'étape nécessaire pour effectuer l'opération */
    wxNotebook						*classeur;	/*!< le classeur avec ses différents onglets*/
    wxSize							tailleMax;	/*!< taille maximale de la fenêtre */
    std::vector<std::pair<wxSpinCtrlDouble*, wxSpinCtrlDouble*> >		spinSouris;	/*<! Spin modifiable par la souris */
    std::vector<DomaineParametreOp<cv::Point>*> 		                paramSouris;	/*<! Spin modifiable par la souris */
    std::vector<wxWindow *> 		                                    ongletSouris;	/*<! Spin modifiable par la souris */
    std::map<wxSpinCtrlDouble*, DragShape *> 		                    formeSouris;	/*<! Spin modifiable par la souris et forme*/
    std::map<ParametreOperation * ,wxTreeItemId  > noeuds;
    ArboCalcul *arbre;

public:
    /*!< Constructeur de la fenetre parametrage */
    FenetreInfoOperation(GrapheOperation *t, FenetrePrincipale *frame, wxOpencvApp *osg);
    FenetreInfoOperation(GrapheOperation * t, std::vector<ParametreOperation> arbreOperation, wxOpencvApp * osg);
    ~FenetreInfoOperation() {};
    /*!< destructeur de la fenetre parametrage */
    wxWindow *CreerOngletEtape(wxNotebook *, int);
    /*!< Création d'un onglet pour une étape */
    void MAJOngletEtape(int indOp);
    /*!< Mise à jour de l'onglet après l'opération */
    void ActiverOnglet(int);
    /*!< Mise à jour de l'onglet après l'opération */
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
    void SauverSequence(wxCommandEvent &evt);
    /*!< Sauvegarde des étapes comme séquence */
    void ComboBox(wxCommandEvent &evt);
    std::vector<std::pair<ParametreOperation*,int>> FindOperande(ImageInfoCV * x);
    /*!<Sélection d'un parametre à l'aide de sa description */
    void OnTextValider(wxCommandEvent &w);
    void OnKeyDown(wxKeyEvent &);

    void OnClose(wxCloseEvent& event);
    /*!< Fermeture de la fenetre parametrage */
    void  PositionSouris(int, wxPoint);
    /*!<Spin modifé à partir de la souris */


    void DefOSGApp(void *w) { osgApp = w; };
    void DefArbre(ArboCalcul *w) { arbre = w; };
    void DefFenMere(FenetrePrincipale *f) { fenMere = f; };
    /*!< Definition du pointeur sur l'application. Permet le dialogue avec les autres éléments. */
    void ExecuterOperation(int indEtape);/*!<Excute l'opération après modification des paramètres à partir de l'étape indEtape */
    int NbParamSouris() { return spinSouris.size(); }
    void AjouterEtape(int nb, ParametreOperation *pOCV, int idFenetre,wxTreeItemId &n);
    wxNotebook *Classeur() { return classeur; };

};

struct ArboCalculParam {
    cv::FileStorage fs;
    FenetrePrincipale *fen = NULL;
    int indFen = -1;
};

class ArboCalcul : public wxTreeCtrl
{
//    typedef void (ArboCalcul::*FonctionNoeud)(wxTreeItemid &,ArboCalculParam &p);
private:
    FenetrePrincipale       *fenMere;
    void                    *osgApp;
    int                     m_imageSize;               // current size of images
    bool                    m_reverseSort;             // flag for OnCompareItems
    wxTreeItemId            m_draggedItem;             // item being dragged right now
    wxTreeItemId            idEvt;             // item being dragged right now
    std::shared_ptr<wxImageList>             listeImage;
    std::shared_ptr<FenetreInfoOperation> fenAlgo;
    int             nbEtape;
    int             nbParamMax;
    wxTextCtrl      *info;
    std::vector<ParametreOperation> listeOp;
    wxString nomArbre;
    bool listeOpPrete;
public:
    enum
    {
        TreeCtrlIcon_File,
        TreeCtrlIcon_FileSelected,
        TreeCtrlIcon_Folder,
        TreeCtrlIcon_FolderSelected,
        TreeCtrlIcon_FolderOpened
    };

    ArboCalcul() { info = NULL; }
    ArboCalcul(FenetrePrincipale *frame, wxOpencvApp *osg, wxWindow *parent, const wxWindowID id,
        const wxPoint& pos, const wxSize& size,
        long style);
	int FindIdOperation(int id);
    int FindMaxEtapeOperation();
//	int FindMaxIdOperation();
	int FindIdResOperation(int id);
    void DefTextCtrl(wxTextCtrl *t) { info = t; };
    void Printf(InfoNoeud *) ;
    void Installation();
    virtual ~ArboCalcul() {};

    void OnContextMenu(wxContextMenuEvent& event);
    void OnItemMenu(wxTreeEvent& event);
    void OnMenuSelect(wxCommandEvent& event);
//    void OnGetInfo(wxTreeEvent& event);
//    void OnSetInfo(wxTreeEvent& event);
//    void OnItemExpanded(wxTreeEvent& event);
//    void OnItemExpanding(wxTreeEvent& event);
//    void OnItemCollapsed(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);
//    void OnSelChanged(wxTreeEvent& event);
//    void OnSelChanging(wxTreeEvent& event);
    void OnItemActivated(wxTreeEvent& event);
    void OnItemStateClick(wxTreeEvent& event);
    void OnItemRClick(wxTreeEvent& event);

    void OnRMouseDown(wxMouseEvent& event);
    void OnRMouseUp(wxMouseEvent& event);
    void OnRMouseDClick(wxMouseEvent& event);

    void SauverSequence(wxTreeItemId &idParent);

    void SauverNoeud(wxTreeItemId & id, ArboCalculParam & p,bool quitterBranche);

    void ReplacerIdParFenetre(wxTreeItemId & id, ArboCalculParam & p, bool quitterBranche);

    void ExplorerArbre(wxTreeItemId & id, ArboCalculParam & p, void (ArboCalcul::*FonctionNoeud)(wxTreeItemId &, ArboCalculParam &,bool));


//    void SauverNoeud(wxTreeItemId & idParent, ArboCalulParam &p);

    wxTreeItemId GetLastTreeITem() const;
    void GetItemsRecursively(const wxTreeItemId& idParent,
        wxTreeItemIdValue cookie = 0);

    void CreateImageList(int size = 16);
    void CreateButtonsImageList(int size = 11);
    void CreateStateImageList(bool del = false);

    void DefFenAlgo(std::shared_ptr<FenetreInfoOperation> f) { fenAlgo = f; };
    void DefListeOp(std::vector<ParametreOperation > f) { listeOp = f; };
    void DefTitre(wxString s) { nomArbre = s; };
    void DoSortChildren(const wxTreeItemId& item, bool reverse = false)
    {
        m_reverseSort = reverse; wxTreeCtrl::SortChildren(item);
    }

    void DoToggleIcon(const wxTreeItemId& item);

    void ShowMenu(wxTreeItemId id, const wxPoint& pt);

    int ImageSize(void) const { return m_imageSize; }

    void ResetBrokenStateImages()
    {
        const size_t count = GetStateImageList()->GetImageCount();
        int state = count > 0 ? count - 1 : wxTREE_ITEMSTATE_NONE;
        DoResetBrokenStateImages(GetRootItem(), 0, state);
    }
    bool ModifNoeud(FenetrePrincipale * f, wxTreeItemId w);

protected:
    virtual int OnCompareItems(const wxTreeItemId& i1, const wxTreeItemId& i2) wxOVERRIDE;

    // is this the test item which we use in several event handlers?
    bool IsTestItem(const wxTreeItemId& item)
    {
        // the test item is the first child folder
        return GetItemParent(item) == GetRootItem() && !GetPrevSibling(item);
    }

private:
    // Find the very last item in the tree.
    void PileCalcul(const wxTreeItemId& idParent, FenetrePrincipale *f);
    void PileCalcul(const wxTreeItemId & idParent, int indRes);
    void PileCalcul(const wxTreeItemId & idParent, ParametreOperation * pOCV);
    void DoResetBrokenStateImages(const wxTreeItemId& idParent,
        wxTreeItemIdValue cookie, int state);

    void LogEvent(const wxChar *name, const wxTreeEvent& event);


    // NB: due to an ugly wxMSW hack you _must_ use wxDECLARE_DYNAMIC_CLASS();
    //     if you want your overloaded OnCompareItems() to be called.
    //     OTOH, if you don't want it you may omit the next line - this will
    //     make default (alphabetical) sorting much faster under wxMSW.
    wxDECLARE_DYNAMIC_CLASS(MyTreeCtrl);
    wxDECLARE_EVENT_TABLE();
};

// Define a new frame type
class GrapheOperation : public wxFrame
{
private:
    FenetrePrincipale * fenMere;
    void                *osgApp;
    wxPanel *m_panel;
    ArboCalcul *arbre;
    wxNotebook *classeur;
    std::shared_ptr<FenetreInfoOperation> fenAlgo;
    wxTextCtrl *infoTexte;
    wxString nomSequence; 
    std::vector<ParametreOperation> listeOp;
public:
    // ctor and dtor
    GrapheOperation(FenetrePrincipale *frame, wxOpencvApp *osg, const wxString& title, int x, int y, int w, int h);
    GrapheOperation(std::vector<ParametreOperation> arbreOperation, wxOpencvApp *osg, const wxString& title, int x, int y, int w, int h);
    virtual ~GrapheOperation();
    void DefFenMere(FenetrePrincipale *f) { fenMere = f; };

    // menu callbacks
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent & event);
    void OnClearLog(wxCommandEvent& event);

    void OnTogButtons(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_HAS_BUTTONS);
    }
    void OnTogTwist(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_TWIST_BUTTONS);
    }
    void OnTogLines(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_NO_LINES);
    }
    void OnTogEdit(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_EDIT_LABELS);
    }
    void OnTogHideRoot(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_HIDE_ROOT);
    }
    void OnTogRootLines(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_LINES_AT_ROOT);
    }
    void OnTogBorder(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_ROW_LINES);
    }
    void OnTogFullHighlight(wxCommandEvent& event)
    {
        TogStyle(event.GetId(), wxTR_FULL_ROW_HIGHLIGHT);
    }

    void OnResetStyle(wxCommandEvent& WXUNUSED(event))
    {
        CreateTreeWithDefStyle();
    }

    void OnSetFgColour(wxCommandEvent& event);
    void OnSetBgColour(wxCommandEvent& event);

    void OnHighlight(wxCommandEvent& event);
    void OnDump(wxCommandEvent& event);
#ifndef NO_MULTIPLE_SELECTION
    void OnDumpSelected(wxCommandEvent& event);
    void OnSelect(wxCommandEvent& event);
    void OnUnselect(wxCommandEvent& event);
    void OnToggleSel(wxCommandEvent& event);
    void OnSelectChildren(wxCommandEvent& event);
#endif // NO_MULTIPLE_SELECTION
    void OnSelectRoot(wxCommandEvent& event);
    void OnSetFocusedRoot(wxCommandEvent& event);
    void OnClearFocused(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnDeleteChildren(wxCommandEvent& event);
    void OnDeleteAll(wxCommandEvent& event);

    void OnRecreate(wxCommandEvent& event);
    void OnToggleButtons(wxCommandEvent& event);
    void OnToggleImages(wxCommandEvent& event);
//    void OnToggleStates(wxCommandEvent& event);
    void OnToggleBell(wxCommandEvent& event);
    void OnSetImageSize(wxCommandEvent& event);
    void OnCollapseAndReset(wxCommandEvent& event);

    void OnSetBold(wxCommandEvent& WXUNUSED(event)) { DoSetBold(true); }
    void OnClearBold(wxCommandEvent& WXUNUSED(event)) { DoSetBold(false); }

    void OnEnsureVisible(wxCommandEvent& event);
    void OnSetFocus(wxCommandEvent& event);

    void OnCount(wxCommandEvent& event);
    void OnCountRec(wxCommandEvent& event);

    void OnRename(wxCommandEvent& event);
    void OnSort(wxCommandEvent& WXUNUSED(event)) { DoSort(); }
    void OnSortRev(wxCommandEvent& WXUNUSED(event)) { DoSort(true); }

    void OnAddItem(wxCommandEvent& event);
    void OnAddManyItems(wxCommandEvent& event);
    void OnInsertItem(wxCommandEvent& event);

    void OnIncIndent(wxCommandEvent& event);
    void OnDecIndent(wxCommandEvent& event);

    void OnIncSpacing(wxCommandEvent& event);
    void OnDecSpacing(wxCommandEvent& event);

    void OnToggleIcon(wxCommandEvent& event);
//    void OnToggleState(wxCommandEvent& event);

    void OnShowFirstVisible(wxCommandEvent& WXUNUSED(event))
    {
        DoShowFirstOrLast(&wxTreeCtrl::GetFirstVisibleItem, "first visible");
    }
#ifdef wxHAS_LAST_VISIBLE // we don't have it currently but may add later
    void OnShowLastVisible(wxCommandEvent& WXUNUSED(event))
    {
        DoShowFirstOrLast(&wxTreeCtrl::GetLastVisibleItem, "last visible");
    }
#endif // wxHAS_LAST_VISIBLE

    void OnShowNextVisible(wxCommandEvent& WXUNUSED(event))
    {
        DoShowRelativeItem(&wxTreeCtrl::GetNextVisible, "next visible");
    }
    void OnShowPrevVisible(wxCommandEvent& WXUNUSED(event))
    {
        DoShowRelativeItem(&wxTreeCtrl::GetPrevVisible, "previous visible");
    }

    void OnShowParent(wxCommandEvent& WXUNUSED(event))
    {
        DoShowRelativeItem(&wxTreeCtrl::GetItemParent, "parent");
    }
    void OnShowPrevSibling(wxCommandEvent& WXUNUSED(event))
    {
        DoShowRelativeItem(&wxTreeCtrl::GetPrevSibling, "previous sibling");
    }
    void OnShowNextSibling(wxCommandEvent& WXUNUSED(event))
    {
        DoShowRelativeItem(&wxTreeCtrl::GetNextSibling, "next sibling");
    }

    void OnScrollTo(wxCommandEvent& event);
    void OnSelectLast(wxCommandEvent& event);

    void OnIdle(wxIdleEvent& event);
    void OnSize(wxSizeEvent& event);
    wxPanel *Panel() { return m_panel; };
    wxNotebook *Classeur() { return classeur; };
    wxString NomSequence() { return nomSequence; };
    std::vector<ParametreOperation> SequenceOperation() { return listeOp; };
private:
    void TogStyle(int id, long flag);

    void DoSort(bool reverse = false);

    void Resize();

    void CreateTreeWithDefStyle();
    void CreateTree(long style);

    // common parts of OnShowFirst/LastVisible() and OnShowNext/PrevVisible()
    typedef wxTreeItemId(wxTreeCtrl::*TreeFunc0_t)() const;
    void DoShowFirstOrLast(TreeFunc0_t pfn, const wxString& label);

    typedef wxTreeItemId(wxTreeCtrl::*TreeFunc1_t)(const wxTreeItemId&) const;
    void DoShowRelativeItem(TreeFunc1_t pfn, const wxString& label);



    void DoSetBold(bool bold = true);

    wxDECLARE_EVENT_TABLE();
};

// menu and control ids
enum
{
    TreeTest_Quit = wxID_EXIT,
    TreeTest_Save = wxID_SAVE,
    TreeTest_About = wxID_ABOUT,
    TreeTest_ClearLog = wxID_CLEAR,
    TreeTest_TogButtons = wxID_HIGHEST,
    TreeTest_Select_operande ,
    TreeTest_TogTwist,
    TreeTest_TogLines,
    TreeTest_TogEdit,
    TreeTest_TogHideRoot,
    TreeTest_TogRootLines,
    TreeTest_TogBorder,
    TreeTest_TogFullHighlight,
    TreeTest_SetFgColour,
    TreeTest_SetBgColour,
    TreeTest_ResetStyle,
    TreeTest_Highlight,
    TreeTest_Dump,
    TreeTest_DumpSelected,
    TreeTest_Count,
    TreeTest_CountRec,
    TreeTest_Sort,
    TreeTest_SortRev,
    TreeTest_SetBold,
    TreeTest_ClearBold,
    TreeTest_Rename,
    TreeTest_Delete,
    TreeTest_DeleteChildren,
    TreeTest_DeleteAll,
    TreeTest_Recreate,
    TreeTest_ToggleImages,
    TreeTest_ToggleStates,
    TreeTest_ToggleBell,
    TreeTest_ToggleAlternateImages,
    TreeTest_ToggleAlternateStates,
    TreeTest_ToggleButtons,
    TreeTest_SetImageSize,
    TreeTest_ToggleSel,
    TreeTest_CollapseAndReset,
    TreeTest_EnsureVisible,
    TreeTest_SetFocus,
    TreeTest_AddItem,
    TreeTest_AddManyItems,
    TreeTest_InsertItem,
    TreeTest_IncIndent,
    TreeTest_DecIndent,
    TreeTest_IncSpacing,
    TreeTest_DecSpacing,
    TreeTest_ToggleIcon,
    TreeTest_ToggleState,
    TreeTest_Select,
    TreeTest_Unselect,
    TreeTest_SelectRoot,
    TreeTest_ClearFocused,
    TreeTest_SetFocusedRoot,
    TreeTest_SelectChildren,
    TreeTest_ShowFirstVisible,
    TreeTest_ShowLastVisible,
    TreeTest_ShowNextVisible,
    TreeTest_ShowPrevVisible,
    TreeTest_ShowParent,
    TreeTest_ShowPrevSibling,
    TreeTest_ShowNextSibling,
    TreeTest_ScrollTo,
    TreeTest_SelectLast,
    TreeTest_Ctrl = 1000
};
#endif

