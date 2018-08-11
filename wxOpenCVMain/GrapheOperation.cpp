#include "wx/wxprec.h"


#include "GrapheOperation.h"
#include "ParametreOperation.h"
#include "SequenceOperation.h"
#include "FenetreAlgo.h"
#include "GlisserForme.h"
#include "ImageStat.h"

#include <wx/hyperlink.h>
#include <algorithm>
#include <memory>


#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

//#undef wxUSE_LOG

static const int NUM_CHILDREN_PER_LEVEL = 5;
static const int NUM_LEVELS = 2;

// verify that the item is ok and insult the user if it is not
#define CHECK_ITEM( item ) if ( !item.IsOk() ) {                                 \
                             wxMessageBox(wxT("Please select some item first!"), \
                                          wxT("Tree sample error"),              \
                                          wxOK | wxICON_EXCLAMATION,             \
                                          this);                                 \
                             return;                                             \
                           }
#undef wxUSE_MENUS

#define MENU_LINK(name) EVT_MENU(TreeTest_##name, GrapheOperation::On##name)

wxBEGIN_EVENT_TABLE(GrapheOperation, wxFrame)
EVT_IDLE(GrapheOperation::OnIdle)
EVT_SIZE(GrapheOperation::OnSize)
EVT_CLOSE(GrapheOperation::OnClose)
MENU_LINK(Quit)
MENU_LINK(ClearLog)

MENU_LINK(TogButtons)
MENU_LINK(TogTwist)
MENU_LINK(TogLines)
MENU_LINK(TogEdit)
MENU_LINK(TogHideRoot)
MENU_LINK(TogRootLines)
MENU_LINK(TogBorder)
MENU_LINK(TogFullHighlight)
MENU_LINK(SetFgColour)
MENU_LINK(SetBgColour)
MENU_LINK(ResetStyle)

MENU_LINK(Highlight)
MENU_LINK(Dump)
#ifndef NO_MULTIPLE_SELECTION
MENU_LINK(DumpSelected)
MENU_LINK(Select)
MENU_LINK(Unselect)
MENU_LINK(ToggleSel)
MENU_LINK(SelectChildren)
#endif // NO_MULTIPLE_SELECTION
MENU_LINK(Rename)
MENU_LINK(Count)
MENU_LINK(CountRec)
MENU_LINK(Sort)
MENU_LINK(SortRev)
MENU_LINK(SetBold)
MENU_LINK(ClearBold)
MENU_LINK(Delete)
MENU_LINK(DeleteChildren)
MENU_LINK(DeleteAll)
MENU_LINK(Recreate)
MENU_LINK(ToggleImages)
//MENU_LINK(ToggleStates)
MENU_LINK(ToggleBell)
//MENU_LINK(ToggleAlternateImages)
//MENU_LINK(ToggleAlternateStates)
MENU_LINK(ToggleButtons)
MENU_LINK(SetImageSize)
MENU_LINK(CollapseAndReset)
MENU_LINK(EnsureVisible)
MENU_LINK(SetFocus)
MENU_LINK(AddItem)
MENU_LINK(AddManyItems)
MENU_LINK(InsertItem)
MENU_LINK(IncIndent)
MENU_LINK(DecIndent)
MENU_LINK(IncSpacing)
MENU_LINK(DecSpacing)
MENU_LINK(ToggleIcon)
//MENU_LINK(ToggleState)
MENU_LINK(SelectRoot)
MENU_LINK(SetFocusedRoot)
MENU_LINK(ClearFocused)

MENU_LINK(ShowFirstVisible)
#ifdef wxHAS_LAST_VISIBLE
MENU_LINK(ShowLastVisible)
#endif // wxHAS_LAST_VISIBLE
MENU_LINK(ShowNextVisible)
MENU_LINK(ShowPrevVisible)
MENU_LINK(ShowParent)
MENU_LINK(ShowPrevSibling)
MENU_LINK(ShowNextSibling)
MENU_LINK(ScrollTo)
MENU_LINK(SelectLast)
#undef MENU_LINK

wxEND_EVENT_TABLE()





GrapheOperation::GrapheOperation(FenetrePrincipale *frame, wxOpencvApp *osg, const wxString& title, int x, int y, int w, int h)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxPoint(x, y), wxSize(w, h)),
    arbre(NULL)
#if wxUSE_LOG
    , infoTexte(NULL),
    classeur(NULL)
#endif // wxUSE_LOG
{
    fenMere = frame;
    osgApp = osg;
    fenAlgo = NULL;
    // This reduces flicker effects - even better would be to define
    // OnEraseBackground to do nothing. When the tree control's scrollbars are
    // show or hidden, the frame is sent a background erase event.
    SetBackgroundColour(*wxWHITE);

    // Give it an icon
    SetIcon(wxICON(sample));


    m_panel = new wxPanel(this);
    classeur = new wxNotebook(m_panel, wxID_ANY);
    fenAlgo = std::make_shared<FenetreInfoOperation>(this, frame, osg);
    infoTexte = new wxTextCtrl(m_panel, wxID_ANY, wxT(""),
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxSUNKEN_BORDER);

    CreateTreeWithDefStyle();
    CreateStatusBar(2);
}


GrapheOperation::GrapheOperation(std::vector<ParametreOperation> arbreOperation, wxOpencvApp * osg, const wxString & title, int x, int y, int w, int h)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxPoint(x, y), wxSize(w, h)),
    arbre(NULL)
#if wxUSE_LOG
    , infoTexte(NULL),
    classeur(NULL)
#endif // wxUSE_LOG
{
    fenMere = NULL;
    osgApp = osg;
    fenAlgo = NULL;
    SetBackgroundColour(*wxWHITE);

    SetIcon(wxICON(sample));

    listeOp = arbreOperation;
    nomSequence = title;
    m_panel = new wxPanel(this);
    classeur = new wxNotebook(m_panel, wxID_ANY);
    fenAlgo = std::make_shared<FenetreInfoOperation>(this, arbreOperation, osg);
    infoTexte = new wxTextCtrl(m_panel, wxID_ANY, wxT(""),
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxSUNKEN_BORDER);

    CreateTreeWithDefStyle();
    CreateStatusBar(2);
}

GrapheOperation::~GrapheOperation()
{
    if (fenMere)
    {
        fenMere->RAZGrapheOperation();
        fenMere = NULL;
    }
}

void GrapheOperation::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void GrapheOperation::OnClose(wxCloseEvent& event)
{
    if (fenMere)
    {
        fenMere->RAZGrapheOperation();
        fenMere = NULL;
    }
    wxFrame::OnCloseWindow(event);
}

void GrapheOperation::CreateTreeWithDefStyle()
{
    long style = wxTR_DEFAULT_STYLE |
#ifndef NO_VARIABLE_HEIGHT
        wxTR_HAS_VARIABLE_ROW_HEIGHT |
#endif
        wxTR_EDIT_LABELS;
    CreateTree(style | wxSUNKEN_BORDER);
}

void GrapheOperation::CreateTree(long style)
{
    arbre = new ArboCalcul((FenetrePrincipale*)fenMere,(wxOpencvApp*)osgApp,m_panel, TreeTest_Ctrl,
        wxDefaultPosition, wxDefaultSize,
        style);

    seq = std::make_shared<SequenceOperation>(true);
    if (listeOp.size() != 0 )
    {
        seq.get()->DefOSGApp(osgApp);
        for (int i = 0; i < listeOp.size(); i++)
            seq.get()->AjouterOperation(listeOp[i]);
        arbre->DefListeOp(listeOp);
        arbre->DefSequence(seq);
        arbre->DefTitre(GetTitle());
    }
    seq.get()->CreerArbre();
    arbre->DefFenAlgo(fenAlgo);
    arbre->DefTextCtrl(infoTexte);
    arbre->Installation();
    fenAlgo.get()->DefArbre(arbre);
    Resize();
}

void GrapheOperation::TogStyle(int id, long flag)
{
    long style = arbre->GetWindowStyle() ^ flag;

    // most treectrl styles can't be changed on the fly using the native
    // control and the tree must be recreated
#ifndef __WXMSW__
    arbre->SetWindowStyle(style);
#else // MSW
    delete arbre;
    CreateTree(style);
#endif // !MSW/MSW

    GetMenuBar()->Check(id, (style & flag) != 0);
}

void GrapheOperation::OnIdle(wxIdleEvent& event)
{
    if (arbre)
    {
        wxTreeItemId idRoot = arbre->GetRootItem();
        wxString status;
        if (idRoot.IsOk())
        {
            wxTreeItemId idLast = arbre->GetLastTreeITem();
            status = wxString::Format(
                wxT("Root/last item is %svisible/%svisible"),
                arbre->IsVisible(idRoot) ? wxT("") : wxT("not "),
                idLast.IsOk() && arbre->IsVisible(idLast)
                ? wxT("") : wxT("not "));
        }
        else
            status = wxT("No root item");

        SetStatusText(status, 1);
    }
    event.Skip();
}

void GrapheOperation::OnSize(wxSizeEvent& event)
{
    if (arbre && infoTexte)
    {
        Resize();
    }

    event.Skip();
}

void GrapheOperation::Resize()
{
    wxSize size = GetClientSize();
    arbre->SetSize(0, 0, size.x, size.y * 1 / 2);
    if (classeur)
        classeur->SetSize(0, size.y / 2, size.x, size.y / 3);
    infoTexte->SetSize(0, 5 * size.y / 6, size.x, size.y / 6);

}



void GrapheOperation::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
    infoTexte->Clear();
}

void GrapheOperation::OnRename(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    (void)arbre->EditLabel(item);
}

void GrapheOperation::OnCount(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    int i = arbre->GetChildrenCount(item, false);

    infoTexte->AppendText(wxString(wxT("%d children\n"), i));
}

void GrapheOperation::OnCountRec(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    int i = arbre->GetChildrenCount(item);

    infoTexte->AppendText(wxString(wxT("%d children"), i));
}

void GrapheOperation::DoSort(bool reverse)
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->DoSortChildren(item, reverse);
}

void GrapheOperation::OnHighlight(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId id = arbre->GetFocusedItem();

    CHECK_ITEM(id);

    wxRect r;
    if (!arbre->GetBoundingRect(id, r, true /* text, not full row */))
    {
        infoTexte->AppendText(wxString(wxT("Failed to get bounding item rect")));
        return;
    }

    wxClientDC dc(arbre);
    dc.SetBrush(*wxRED);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(r);
    arbre->Update();
}

void GrapheOperation::OnDump(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId root = arbre->GetFocusedItem();

    CHECK_ITEM(root);

    arbre->GetItemsRecursively(root);
}

#ifndef NO_MULTIPLE_SELECTION

void GrapheOperation::OnToggleSel(wxCommandEvent& event)
{
    TogStyle(event.GetId(), wxTR_MULTIPLE);
}

void GrapheOperation::OnDumpSelected(wxCommandEvent& WXUNUSED(event))
{
    wxArrayTreeItemIds array;

    size_t count = arbre->GetSelections(array);
    wxLogMessage(wxT("%u items selected"), unsigned(count));

    for (size_t n = 0; n < count; n++)
    {
        wxLogMessage(wxT("\t%s"), arbre->GetItemText(array.Item(n)).c_str());
    }
}

void GrapheOperation::OnSelect(wxCommandEvent& WXUNUSED(event))
{
    arbre->SelectItem(arbre->GetFocusedItem());
}

void GrapheOperation::OnSelectRoot(wxCommandEvent& WXUNUSED(event))
{
    if (!arbre->HasFlag(wxTR_HIDE_ROOT))
        arbre->SelectItem(arbre->GetRootItem());
}

void GrapheOperation::OnSetFocusedRoot(wxCommandEvent& WXUNUSED(event))
{
    if (!arbre->HasFlag(wxTR_HIDE_ROOT))
        arbre->SetFocusedItem(arbre->GetRootItem());
}

void GrapheOperation::OnClearFocused(wxCommandEvent& WXUNUSED(event))
{
    arbre->ClearFocusedItem();
}

void GrapheOperation::OnUnselect(wxCommandEvent& WXUNUSED(event))
{
    arbre->UnselectAll();
}

void GrapheOperation::OnSelectChildren(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();
    if (!item.IsOk())
        item = arbre->GetRootItem();

    arbre->SelectChildren(item);
}

#endif // NO_MULTIPLE_SELECTION

void GrapheOperation::DoSetBold(bool bold)
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->SetItemBold(item, bold);
}

void GrapheOperation::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->Delete(item);
}

void GrapheOperation::OnDeleteChildren(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->DeleteChildren(item);
}

void GrapheOperation::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
    arbre->DeleteAllItems();
}

void GrapheOperation::OnRecreate(wxCommandEvent& event)
{
    OnDeleteAll(event);
}

void GrapheOperation::OnSetImageSize(wxCommandEvent& WXUNUSED(event))
{
    int size = wxGetNumberFromUser(wxT("Enter the size for the images to use"),
        wxT("Size: "),
        wxT("TreeCtrl sample"),
        arbre->ImageSize());
    if (size == -1)
        return;

    arbre->CreateImageList(size);
//    wxGetApp().SetShowImages(true);
}

void GrapheOperation::OnToggleImages(wxCommandEvent& WXUNUSED(event))
{
/*    if (wxGetApp().ShowImages())
    {
        arbre->CreateImageList(-1);
        wxGetApp().SetShowImages(false);
    }
    else*/
    {
        arbre->CreateImageList(0);
//        wxGetApp().SetShowImages(true);
    }
}

/*void GrapheOperation::OnToggleStates(wxCommandEvent& WXUNUSED(event))
{
    if (wxGetApp().ShowStates())
    {
        arbre->CreateStateImageList(true);
        wxGetApp().SetShowStates(false);
    }
    else
    {
        arbre->CreateStateImageList(false);
//        wxGetApp().SetShowStates(true);
    }
}*/

void GrapheOperation::OnToggleBell(wxCommandEvent& event)
{
    arbre->EnableBellOnNoMatch(event.IsChecked());
}



void GrapheOperation::OnToggleButtons(wxCommandEvent& WXUNUSED(event))
{
#if USE_GENERIC_TREECTRL || !defined(__WXMSW__)
    if (wxGetApp().ShowButtons())
    {
        arbre->CreateButtonsImageList(-1);
        wxGetApp().SetShowButtons(false);
    }
    else
    {
        arbre->CreateButtonsImageList(15);
        wxGetApp().SetShowButtons(true);
    }
#endif
}

void GrapheOperation::OnCollapseAndReset(wxCommandEvent& WXUNUSED(event))
{
    arbre->CollapseAndReset(arbre->GetRootItem());
}

void GrapheOperation::OnEnsureVisible(wxCommandEvent& WXUNUSED(event))
{
    const wxTreeItemId
        idLast = arbre->GetLastTreeITem();
    if (idLast.IsOk())
        arbre->EnsureVisible(idLast);
    else
        wxLogMessage("No last item");
}

void GrapheOperation::OnSetFocus(wxCommandEvent& WXUNUSED(event))
{
    arbre->SetFocus();
}

void GrapheOperation::OnInsertItem(wxCommandEvent& WXUNUSED(event))
{
    int image = /*wxGetApp().ShowImages()*/1 ? ArboCalcul::TreeCtrlIcon_File : -1;
    arbre->InsertItem(arbre->GetRootItem(), image, wxT("2nd item"));
}

void GrapheOperation::OnAddItem(wxCommandEvent& WXUNUSED(event))
{
    static int s_num = 0;

    wxString text;
    text.Printf(wxT("Item #%d"), ++s_num);

    arbre->AppendItem(arbre->GetRootItem(),
        text /*,
             ArboCalcul::TreeCtrlIcon_File */);
}

void GrapheOperation::OnAddManyItems(wxCommandEvent& WXUNUSED(event))
{
    wxWindowUpdateLocker lockUpdates(this);

    const wxTreeItemId root = arbre->GetRootItem();
    for (int n = 0; n < 1000; n++)
    {
        arbre->AppendItem(root, wxString::Format("Item #%03d", n));
    }
}

void GrapheOperation::OnIncIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = arbre->GetIndent();
    if (indent < 100)
        arbre->SetIndent(indent + 5);
}

void GrapheOperation::OnDecIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = arbre->GetIndent();
    if (indent > 10)
        arbre->SetIndent(indent - 5);
}

void GrapheOperation::OnIncSpacing(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = arbre->GetSpacing();
    if (indent < 100)
    {
        arbre->SetSpacing(indent + 5);
        arbre->Refresh();
    }
}

void GrapheOperation::OnDecSpacing(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = arbre->GetSpacing();
    if (indent > 10)
    {
        arbre->SetSpacing(indent - 5);
        arbre->Refresh();
    }
}

void GrapheOperation::OnToggleIcon(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->DoToggleIcon(item);
}

/*void GrapheOperation::OnToggleState(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->DoToggleState(item);
}*/

void GrapheOperation::DoShowFirstOrLast(TreeFunc0_t pfn, const wxString& label)
{
    const wxTreeItemId item = (arbre->*pfn)();

    if (!item.IsOk())
    {
        wxLogMessage("There is no %s item", label);
    }
    else
    {
        wxLogMessage("The %s item is \"%s\"",
            label, arbre->GetItemText(item));
    }
}

void GrapheOperation::DoShowRelativeItem(TreeFunc1_t pfn, const wxString& label)
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    if ((pfn == (TreeFunc1_t)&wxTreeCtrl::GetPrevVisible
        || pfn == (TreeFunc1_t)&wxTreeCtrl::GetNextVisible)
        && !arbre->IsVisible(item))
    {
        wxLogMessage("The selected item must be visible.");
        return;
    }

    wxTreeItemId new_item = (arbre->*pfn)(item);

    if (!new_item.IsOk())
    {
        wxLogMessage("There is no %s item", label);
    }
    else
    {
        wxLogMessage("The %s item is \"%s\"",
            label, arbre->GetItemText(new_item));
    }
}

void GrapheOperation::OnScrollTo(wxCommandEvent& WXUNUSED(event))
{
    // scroll to the last but one top level child
    wxTreeItemId item = arbre->GetPrevSibling(
        arbre->GetLastChild(
            arbre->GetRootItem()));
    CHECK_ITEM(item);

    arbre->ScrollTo(item);
}

void GrapheOperation::OnSelectLast(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetLastTreeITem();

    CHECK_ITEM(item);

    arbre->SelectItem(item);
}

void GrapheOperation::OnSetFgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, arbre->GetForegroundColour());
    if (col.IsOk())
        arbre->SetForegroundColour(col);
}

void GrapheOperation::OnSetBgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, arbre->GetBackgroundColour());
    if (col.IsOk())
        arbre->SetBackgroundColour(col);
}

