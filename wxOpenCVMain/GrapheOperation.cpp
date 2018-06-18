#include "wx/wxprec.h"


#include "GrapheOperation.h"

#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\icon1.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\icon2.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\icon3.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\icon4.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\icon5.xpm"

#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\state1.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\state2.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\state3.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\state4.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\state5.xpm"

#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\unchecked.xpm"
#include "G:\Lib\wxWidgets-3.1.0\samples\treectrl\checked.xpm"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

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
MENU_LINK(ToggleStates)
MENU_LINK(ToggleBell)
MENU_LINK(ToggleAlternateImages)
MENU_LINK(ToggleAlternateStates)
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
MENU_LINK(ToggleState)
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

#if USE_GENERIC_TREECTRL
wxBEGIN_EVENT_TABLE(ArboCalcul, wxGenericTreeCtrl)
#else
wxBEGIN_EVENT_TABLE(ArboCalcul, wxTreeCtrl)
#endif
EVT_TREE_BEGIN_DRAG(TreeTest_Ctrl, ArboCalcul::OnBeginDrag)
EVT_TREE_BEGIN_RDRAG(TreeTest_Ctrl, ArboCalcul::OnBeginRDrag)
EVT_TREE_END_DRAG(TreeTest_Ctrl, ArboCalcul::OnEndDrag)
EVT_TREE_BEGIN_LABEL_EDIT(TreeTest_Ctrl, ArboCalcul::OnBeginLabelEdit)
EVT_TREE_END_LABEL_EDIT(TreeTest_Ctrl, ArboCalcul::OnEndLabelEdit)
EVT_TREE_DELETE_ITEM(TreeTest_Ctrl, ArboCalcul::OnDeleteItem)
#if 0       // there are so many of those that logging them causes flicker
EVT_TREE_GET_INFO(TreeTest_Ctrl, ArboCalcul::OnGetInfo)
#endif
EVT_TREE_SET_INFO(TreeTest_Ctrl, ArboCalcul::OnSetInfo)
EVT_TREE_ITEM_EXPANDED(TreeTest_Ctrl, ArboCalcul::OnItemExpanded)
EVT_TREE_ITEM_EXPANDING(TreeTest_Ctrl, ArboCalcul::OnItemExpanding)
EVT_TREE_ITEM_COLLAPSED(TreeTest_Ctrl, ArboCalcul::OnItemCollapsed)
EVT_TREE_ITEM_COLLAPSING(TreeTest_Ctrl, ArboCalcul::OnItemCollapsing)

EVT_TREE_SEL_CHANGED(TreeTest_Ctrl, ArboCalcul::OnSelChanged)
EVT_TREE_SEL_CHANGING(TreeTest_Ctrl, ArboCalcul::OnSelChanging)
EVT_TREE_KEY_DOWN(TreeTest_Ctrl, ArboCalcul::OnTreeKeyDown)
EVT_TREE_ITEM_ACTIVATED(TreeTest_Ctrl, ArboCalcul::OnItemActivated)
EVT_TREE_STATE_IMAGE_CLICK(TreeTest_Ctrl, ArboCalcul::OnItemStateClick)

// so many different ways to handle right mouse button clicks...
EVT_CONTEXT_MENU(ArboCalcul::OnContextMenu)
// EVT_TREE_ITEM_MENU is the preferred event for creating context menus
// on a tree control, because it includes the point of the click or item,
// meaning that no additional placement calculations are required.
EVT_TREE_ITEM_MENU(TreeTest_Ctrl, ArboCalcul::OnItemMenu)
EVT_TREE_ITEM_RIGHT_CLICK(TreeTest_Ctrl, ArboCalcul::OnItemRClick)

EVT_RIGHT_DOWN(ArboCalcul::OnRMouseDown)
EVT_RIGHT_UP(ArboCalcul::OnRMouseUp)
EVT_RIGHT_DCLICK(ArboCalcul::OnRMouseDClick)
wxEND_EVENT_TABLE()




GrapheOperation::GrapheOperation(FenetrePrincipale *frame, wxOpencvApp *osg, const wxString& title, int x, int y, int w, int h)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxPoint(x, y), wxSize(w, h)),
    arbre(NULL)
#if wxUSE_LOG
    , m_textCtrl(NULL)
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

#if wxUSE_LOG
    // create the controls
    m_textCtrl = new wxTextCtrl(m_panel, wxID_ANY, wxT(""),
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxSUNKEN_BORDER);

    // set our text control as the log target
    wxLogTextCtrl *logWindow = new wxLogTextCtrl(m_textCtrl);
    delete wxLog::SetActiveTarget(logWindow);
#endif // wxUSE_LOG

    CreateTreeWithDefStyle();
    CreateStatusBar(2);
}

GrapheOperation::~GrapheOperation()
{
#if wxUSE_LOG
    delete wxLog::SetActiveTarget(NULL);
#endif // wxUSE_LOG
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
    if (arbre
#if wxUSE_LOG
        && m_textCtrl
#endif // wxUSE_LOG
        )
    {
        Resize();
    }

    event.Skip();
}

void GrapheOperation::Resize()
{
    wxSize size = GetClientSize();
    arbre->SetSize(0, 0, size.x, size.y* 1 / 3);
    m_textCtrl->SetSize(0, 1 * size.y / 3, size.x, size.y / 3);
    if (fenAlgo)
        fenAlgo->SetSize(0, 2 * size.y / 3, size.x, size.y / 3);

}

void GrapheOperation::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void GrapheOperation::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
    m_textCtrl->Clear();
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

    wxLogMessage(wxT("%d children"), i);
}

void GrapheOperation::OnCountRec(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    int i = arbre->GetChildrenCount(item);

    wxLogMessage(wxT("%d children"), i);
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
        wxLogMessage(wxT("Failed to get bounding item rect"));
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

void GrapheOperation::OnToggleStates(wxCommandEvent& WXUNUSED(event))
{
/*    if (wxGetApp().ShowStates())
    {
        arbre->CreateStateImageList(true);
        wxGetApp().SetShowStates(false);
    }
    else*/
    {
        arbre->CreateStateImageList(false);
//        wxGetApp().SetShowStates(true);
    }
}

void GrapheOperation::OnToggleBell(wxCommandEvent& event)
{
    arbre->EnableBellOnNoMatch(event.IsChecked());
}

void GrapheOperation::OnToggleAlternateImages(wxCommandEvent& WXUNUSED(event))
{
    bool alternateImages = arbre->AlternateImages();

    arbre->SetAlternateImages(!alternateImages);
    arbre->CreateImageList(0);
}

void GrapheOperation::OnToggleAlternateStates(wxCommandEvent& WXUNUSED(event))
{
    bool alternateStates = arbre->AlternateStates();

    arbre->SetAlternateStates(!alternateStates);
    arbre->CreateStateImageList();

    // normal states < alternate states
    // so we must reset broken states
    if (alternateStates)
        arbre->ResetBrokenStateImages();
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

void GrapheOperation::OnToggleState(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = arbre->GetFocusedItem();

    CHECK_ITEM(item);

    arbre->DoToggleState(item);
}

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

// ArboCalcul implementation
#if USE_GENERIC_TREECTRL
wxIMPLEMENT_DYNAMIC_CLASS(ArboCalcul, wxGenericTreeCtrl);
#else
wxIMPLEMENT_DYNAMIC_CLASS(ArboCalcul, wxTreeCtrl);
#endif

ArboCalcul::ArboCalcul(FenetrePrincipale *frame, wxOpencvApp *osg,wxWindow *parent, const wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style)
    : wxTreeCtrl(parent, id, pos, size, style),
    m_alternateImages(false),
    m_alternateStates(false)
{
    m_reverseSort = false;

    CreateImageList();
    CreateStateImageList();
    fenMere = frame;
    osgApp = osg;

    FenetrePrincipale *f = fenMere;
    
    wxString n;
    f->GetTitle();
    wxTreeItemId rootId = AddRoot(f->GetTitle(),-1, -1,new MyTreeItemData(f->GetTitle()));
    PileCalcul(rootId, f);
}

void ArboCalcul::CreateImageList(int size)
{
    if (size == -1)
    {
        SetImageList(NULL);
        return;
    }
    if (size == 0)
        size = m_imageSize;
    else
        m_imageSize = size;

    // Make an image list containing small icons
    wxImageList *images = new wxImageList(size, size, true);

    // should correspond to TreeCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons[5];

    if (m_alternateImages)
    {
        icons[TreeCtrlIcon_File] = wxIcon(icon1_xpm);
        icons[TreeCtrlIcon_FileSelected] = wxIcon(icon2_xpm);
        icons[TreeCtrlIcon_Folder] = wxIcon(icon3_xpm);
        icons[TreeCtrlIcon_FolderSelected] = wxIcon(icon4_xpm);
        icons[TreeCtrlIcon_FolderOpened] = wxIcon(icon5_xpm);
    }
    else
    {
        wxSize iconSize(size, size);

        icons[TreeCtrlIcon_File] =
            icons[TreeCtrlIcon_FileSelected] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_LIST, iconSize);
        icons[TreeCtrlIcon_Folder] =
            icons[TreeCtrlIcon_FolderSelected] =
            icons[TreeCtrlIcon_FolderOpened] = wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, iconSize);
    }

    for (size_t i = 0; i < WXSIZEOF(icons); i++)
    {
        int sizeOrig = icons[0].GetWidth();
        if (size == sizeOrig)
        {
            images->Add(icons[i]);
        }
        else
        {
            images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
        }
    }

    AssignImageList(images);
}

void ArboCalcul::CreateStateImageList(bool del)
{
    if (del)
    {
        SetStateImageList(NULL);
        return;
    }

    wxImageList *states;
    wxBusyCursor wait;

    if (m_alternateStates)
    {
        wxIcon icons[5];
        icons[0] = wxIcon(state1_xpm);  // yellow
        icons[1] = wxIcon(state2_xpm);  // green
        icons[2] = wxIcon(state3_xpm);  // red
        icons[3] = wxIcon(state4_xpm);  // blue
        icons[4] = wxIcon(state5_xpm);  // black

        int width = icons[0].GetWidth(),
            height = icons[0].GetHeight();

        // Make an state image list containing small icons
        states = new wxImageList(width, height, true);

        for (size_t i = 0; i < WXSIZEOF(icons); i++)
            states->Add(icons[i]);
    }
    else
    {
        wxIcon icons[2];
        icons[0] = wxIcon(unchecked_xpm);
        icons[1] = wxIcon(checked_xpm);

        int width = icons[0].GetWidth(),
            height = icons[0].GetHeight();

        // Make an state image list containing small icons
        states = new wxImageList(width, height, true);

        for (size_t i = 0; i < WXSIZEOF(icons); i++)
            states->Add(icons[i]);
    }

    AssignStateImageList(states);
}

void ArboCalcul::CreateButtonsImageList(int WXUNUSED(size))
{
}

int ArboCalcul::OnCompareItems(const wxTreeItemId& item1,
    const wxTreeItemId& item2)
{
    if (m_reverseSort)
    {
        // just exchange 1st and 2nd items
        return wxTreeCtrl::OnCompareItems(item2, item1);
    }
    else
    {
        return wxTreeCtrl::OnCompareItems(item1, item2);
    }
}

void ArboCalcul::PileCalcul(const wxTreeItemId& idParent, FenetrePrincipale *f)
{

    if (f && f->OrigineImage()->indOpFenetre.size() != 0)
    {
        for (int i=0;i<f->OrigineImage()->nbOperande;i++)
        {
            int id = f->OrigineImage()->indOpFenetre[i];
            FenetrePrincipale *fId = ((wxOpencvApp *)osgApp)->Fenetre(id);
            wxTreeItemId noeud = AppendItem(idParent, fId->GetTitle(),-1,-1, new MyTreeItemData(fId->GetTitle() +"data item"));

            if (fId)
                PileCalcul(noeud, fId);
        }
        wxString n(f->OrigineImage()->nomOperation);
        wxTreeItemId id = AppendItem(idParent, n, -1,-1, new MyTreeItemData(n));


    }
}




wxTreeItemId ArboCalcul::GetLastTreeITem() const
{
    wxTreeItemId item = GetRootItem();
    for (;; )
    {
        wxTreeItemId itemChild = GetLastChild(item);
        if (!itemChild.IsOk())
            break;

        item = itemChild;
    }

    return item;
}

void ArboCalcul::GetItemsRecursively(const wxTreeItemId& idParent,
    wxTreeItemIdValue cookie)
{
    wxTreeItemId id;

    if (!cookie)
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if (!id.IsOk())
        return;

    wxString text = GetItemText(id);
    wxLogMessage(text);

    if (ItemHasChildren(id))
        GetItemsRecursively(id);

    GetItemsRecursively(idParent, cookie);
}

void ArboCalcul::DoToggleIcon(const wxTreeItemId& item)
{
    int image = GetItemImage(item) == TreeCtrlIcon_Folder
        ? TreeCtrlIcon_File
        : TreeCtrlIcon_Folder;
    SetItemImage(item, image, wxTreeItemIcon_Normal);

    image = GetItemImage(item, wxTreeItemIcon_Selected) == TreeCtrlIcon_FolderSelected
        ? TreeCtrlIcon_FileSelected
        : TreeCtrlIcon_FolderSelected;
    SetItemImage(item, image, wxTreeItemIcon_Selected);
}

void ArboCalcul::DoToggleState(const wxTreeItemId& item)
{
    if (m_alternateStates)
    {
        // sets random state unlike current
        int state = GetItemState(item);
        int nState;

        srand(time(NULL));
        do {
            nState = rand() % GetStateImageList()->GetImageCount();
        } while (nState == state);

        SetItemState(item, nState);
    }
    else
    {
        // we have only 2 checkbox states, so next state will be reversed
        SetItemState(item, wxTREE_ITEMSTATE_NEXT);
    }
}

void ArboCalcul::DoResetBrokenStateImages(const wxTreeItemId& idParent,
    wxTreeItemIdValue cookie, int state)
{
    wxTreeItemId id;

    if (!cookie)
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if (!id.IsOk())
        return;

    int curState = GetItemState(id);
    if (curState != wxTREE_ITEMSTATE_NONE && curState > state)
        SetItemState(id, state);

    if (ItemHasChildren(id))
        DoResetBrokenStateImages(id, 0, state);

    DoResetBrokenStateImages(idParent, cookie, state);
}

void ArboCalcul::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxString text;
    if (item.IsOk())
        text << wxT('"') << GetItemText(item).c_str() << wxT('"');
    else
        text = wxT("invalid item");
    wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

// avoid repetition
#define TREE_EVENT_HANDLER(name)                                 \
void ArboCalcul::name(wxTreeEvent& event)                        \
{                                                                \
    LogEvent(wxT(#name), event);                                  \
    event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnBeginRDrag)
TREE_EVENT_HANDLER(OnDeleteItem)
TREE_EVENT_HANDLER(OnGetInfo)
TREE_EVENT_HANDLER(OnSetInfo)
TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)
TREE_EVENT_HANDLER(OnSelChanged)
TREE_EVENT_HANDLER(OnSelChanging)

#undef TREE_EVENT_HANDLER

void LogKeyEvent(const wxChar *name, const wxKeyEvent& event)
{
    wxString key;
    long keycode = event.GetKeyCode();
    {
        switch (keycode)
        {
        case WXK_BACK: key = wxT("BACK"); break;
        case WXK_TAB: key = wxT("TAB"); break;
        case WXK_RETURN: key = wxT("RETURN"); break;
        case WXK_ESCAPE: key = wxT("ESCAPE"); break;
        case WXK_SPACE: key = wxT("SPACE"); break;
        case WXK_DELETE: key = wxT("DELETE"); break;
        case WXK_START: key = wxT("START"); break;
        case WXK_LBUTTON: key = wxT("LBUTTON"); break;
        case WXK_RBUTTON: key = wxT("RBUTTON"); break;
        case WXK_CANCEL: key = wxT("CANCEL"); break;
        case WXK_MBUTTON: key = wxT("MBUTTON"); break;
        case WXK_CLEAR: key = wxT("CLEAR"); break;
        case WXK_SHIFT: key = wxT("SHIFT"); break;
        case WXK_ALT: key = wxT("ALT"); break;
        case WXK_CONTROL: key = wxT("CONTROL"); break;
        case WXK_MENU: key = wxT("MENU"); break;
        case WXK_PAUSE: key = wxT("PAUSE"); break;
        case WXK_CAPITAL: key = wxT("CAPITAL"); break;
        case WXK_END: key = wxT("END"); break;
        case WXK_HOME: key = wxT("HOME"); break;
        case WXK_LEFT: key = wxT("LEFT"); break;
        case WXK_UP: key = wxT("UP"); break;
        case WXK_RIGHT: key = wxT("RIGHT"); break;
        case WXK_DOWN: key = wxT("DOWN"); break;
        case WXK_SELECT: key = wxT("SELECT"); break;
        case WXK_PRINT: key = wxT("PRINT"); break;
        case WXK_EXECUTE: key = wxT("EXECUTE"); break;
        case WXK_SNAPSHOT: key = wxT("SNAPSHOT"); break;
        case WXK_INSERT: key = wxT("INSERT"); break;
        case WXK_HELP: key = wxT("HELP"); break;
        case WXK_NUMPAD0: key = wxT("NUMPAD0"); break;
        case WXK_NUMPAD1: key = wxT("NUMPAD1"); break;
        case WXK_NUMPAD2: key = wxT("NUMPAD2"); break;
        case WXK_NUMPAD3: key = wxT("NUMPAD3"); break;
        case WXK_NUMPAD4: key = wxT("NUMPAD4"); break;
        case WXK_NUMPAD5: key = wxT("NUMPAD5"); break;
        case WXK_NUMPAD6: key = wxT("NUMPAD6"); break;
        case WXK_NUMPAD7: key = wxT("NUMPAD7"); break;
        case WXK_NUMPAD8: key = wxT("NUMPAD8"); break;
        case WXK_NUMPAD9: key = wxT("NUMPAD9"); break;
        case WXK_MULTIPLY: key = wxT("MULTIPLY"); break;
        case WXK_ADD: key = wxT("ADD"); break;
        case WXK_SEPARATOR: key = wxT("SEPARATOR"); break;
        case WXK_SUBTRACT: key = wxT("SUBTRACT"); break;
        case WXK_DECIMAL: key = wxT("DECIMAL"); break;
        case WXK_DIVIDE: key = wxT("DIVIDE"); break;
        case WXK_F1: key = wxT("F1"); break;
        case WXK_F2: key = wxT("F2"); break;
        case WXK_F3: key = wxT("F3"); break;
        case WXK_F4: key = wxT("F4"); break;
        case WXK_F5: key = wxT("F5"); break;
        case WXK_F6: key = wxT("F6"); break;
        case WXK_F7: key = wxT("F7"); break;
        case WXK_F8: key = wxT("F8"); break;
        case WXK_F9: key = wxT("F9"); break;
        case WXK_F10: key = wxT("F10"); break;
        case WXK_F11: key = wxT("F11"); break;
        case WXK_F12: key = wxT("F12"); break;
        case WXK_F13: key = wxT("F13"); break;
        case WXK_F14: key = wxT("F14"); break;
        case WXK_F15: key = wxT("F15"); break;
        case WXK_F16: key = wxT("F16"); break;
        case WXK_F17: key = wxT("F17"); break;
        case WXK_F18: key = wxT("F18"); break;
        case WXK_F19: key = wxT("F19"); break;
        case WXK_F20: key = wxT("F20"); break;
        case WXK_F21: key = wxT("F21"); break;
        case WXK_F22: key = wxT("F22"); break;
        case WXK_F23: key = wxT("F23"); break;
        case WXK_F24: key = wxT("F24"); break;
        case WXK_NUMLOCK: key = wxT("NUMLOCK"); break;
        case WXK_SCROLL: key = wxT("SCROLL"); break;
        case WXK_PAGEUP: key = wxT("PAGEUP"); break;
        case WXK_PAGEDOWN: key = wxT("PAGEDOWN"); break;
        case WXK_NUMPAD_SPACE: key = wxT("NUMPAD_SPACE"); break;
        case WXK_NUMPAD_TAB: key = wxT("NUMPAD_TAB"); break;
        case WXK_NUMPAD_ENTER: key = wxT("NUMPAD_ENTER"); break;
        case WXK_NUMPAD_F1: key = wxT("NUMPAD_F1"); break;
        case WXK_NUMPAD_F2: key = wxT("NUMPAD_F2"); break;
        case WXK_NUMPAD_F3: key = wxT("NUMPAD_F3"); break;
        case WXK_NUMPAD_F4: key = wxT("NUMPAD_F4"); break;
        case WXK_NUMPAD_HOME: key = wxT("NUMPAD_HOME"); break;
        case WXK_NUMPAD_LEFT: key = wxT("NUMPAD_LEFT"); break;
        case WXK_NUMPAD_UP: key = wxT("NUMPAD_UP"); break;
        case WXK_NUMPAD_RIGHT: key = wxT("NUMPAD_RIGHT"); break;
        case WXK_NUMPAD_DOWN: key = wxT("NUMPAD_DOWN"); break;
        case WXK_NUMPAD_PAGEUP: key = wxT("NUMPAD_PAGEUP"); break;
        case WXK_NUMPAD_PAGEDOWN: key = wxT("NUMPAD_PAGEDOWN"); break;
        case WXK_NUMPAD_END: key = wxT("NUMPAD_END"); break;
        case WXK_NUMPAD_BEGIN: key = wxT("NUMPAD_BEGIN"); break;
        case WXK_NUMPAD_INSERT: key = wxT("NUMPAD_INSERT"); break;
        case WXK_NUMPAD_DELETE: key = wxT("NUMPAD_DELETE"); break;
        case WXK_NUMPAD_EQUAL: key = wxT("NUMPAD_EQUAL"); break;
        case WXK_NUMPAD_MULTIPLY: key = wxT("NUMPAD_MULTIPLY"); break;
        case WXK_NUMPAD_ADD: key = wxT("NUMPAD_ADD"); break;
        case WXK_NUMPAD_SEPARATOR: key = wxT("NUMPAD_SEPARATOR"); break;
        case WXK_NUMPAD_SUBTRACT: key = wxT("NUMPAD_SUBTRACT"); break;
        case WXK_NUMPAD_DECIMAL: key = wxT("NUMPAD_DECIMAL"); break;
        case WXK_BROWSER_BACK: key = wxT("BROWSER_BACK"); break;
        case WXK_BROWSER_FORWARD: key = wxT("BROWSER_FORWARD"); break;
        case WXK_BROWSER_REFRESH: key = wxT("BROWSER_REFRESH"); break;
        case WXK_BROWSER_STOP: key = wxT("BROWSER_STOP"); break;
        case WXK_BROWSER_SEARCH: key = wxT("BROWSER_SEARCH"); break;
        case WXK_BROWSER_FAVORITES: key = wxT("BROWSER_FAVORITES"); break;
        case WXK_BROWSER_HOME: key = wxT("BROWSER_HOME"); break;
        case WXK_VOLUME_MUTE: key = wxT("VOLUME_MUTE"); break;
        case WXK_VOLUME_DOWN: key = wxT("VOLUME_DOWN"); break;
        case WXK_VOLUME_UP: key = wxT("VOLUME_UP"); break;
        case WXK_MEDIA_NEXT_TRACK: key = wxT("MEDIA_NEXT_TRACK"); break;
        case WXK_MEDIA_PREV_TRACK: key = wxT("MEDIA_PREV_TRACK"); break;
        case WXK_MEDIA_STOP: key = wxT("MEDIA_STOP"); break;
        case WXK_MEDIA_PLAY_PAUSE: key = wxT("MEDIA_PLAY_PAUSE"); break;
        case WXK_LAUNCH_MAIL: key = wxT("LAUNCH_MAIL"); break;
        case WXK_LAUNCH_APP1: key = wxT("LAUNCH_APP1"); break;
        case WXK_LAUNCH_APP2: key = wxT("LAUNCH_APP2"); break;

        default:
        {
            if (keycode < 128 && wxIsprint((int)keycode))
                key.Printf(wxT("'%c'"), (char)keycode);
            else if (keycode > 0 && keycode < 27)
                key.Printf(_("Ctrl-%c"), wxT('A') + keycode - 1);
            else
                key.Printf(wxT("unknown (%ld)"), keycode);
        }
        }
    }

    wxLogMessage(wxT("%s event: %s (flags = %c%c%c%c)"),
        name,
        key.c_str(),
        event.ControlDown() ? wxT('C') : wxT('-'),
        event.AltDown() ? wxT('A') : wxT('-'),
        event.ShiftDown() ? wxT('S') : wxT('-'),
        event.MetaDown() ? wxT('M') : wxT('-'));
}

void ArboCalcul::OnTreeKeyDown(wxTreeEvent& event)
{
    LogKeyEvent(wxT("Tree key down "), event.GetKeyEvent());

    event.Skip();
}

void ArboCalcul::OnBeginDrag(wxTreeEvent& event)
{
    // need to explicitly allow drag
    if (event.GetItem() != GetRootItem())
    {
        m_draggedItem = event.GetItem();

        wxPoint clientpt = event.GetPoint();
        wxPoint screenpt = ClientToScreen(clientpt);

        wxLogMessage(wxT("OnBeginDrag: started dragging %s at screen coords (%i,%i)"),
            GetItemText(m_draggedItem).c_str(),
            screenpt.x, screenpt.y);

        event.Allow();
    }
    else
    {
        wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
    }
}

void ArboCalcul::OnEndDrag(wxTreeEvent& event)
{
    wxTreeItemId itemSrc = m_draggedItem,
        itemDst = event.GetItem();
    m_draggedItem = (wxTreeItemId)0l;

    // where to copy the item?
    if (itemDst.IsOk() && !ItemHasChildren(itemDst))
    {
        // copy to the parent then
        itemDst = GetItemParent(itemDst);
    }

    if (!itemDst.IsOk())
    {
        wxLogMessage(wxT("OnEndDrag: can't drop here."));

        return;
    }

    wxString text = GetItemText(itemSrc);
    wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
        text.c_str(), GetItemText(itemDst).c_str());

    // just do append here - we could also insert it just before/after the item
    // on which it was dropped, but this requires slightly more work... we also
    // completely ignore the client data and icon of the old item but could
    // copy them as well.
    //
    // Finally, we only copy one item here but we might copy the entire tree if
    // we were dragging a folder.
    int image = /*wxGetApp().ShowImages()*/1 ? TreeCtrlIcon_File : -1;
    wxTreeItemId id = AppendItem(itemDst, text, image);

 //   if (wxGetApp().ShowStates())
        SetItemState(id, GetItemState(itemSrc));
}

void ArboCalcul::OnBeginLabelEdit(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnBeginLabelEdit"));

    // for testing, prevent this item's label editing
    wxTreeItemId itemId = event.GetItem();
    if (IsTestItem(itemId))
    {
        wxMessageBox(wxT("You can't edit this item."));

        event.Veto();
    }
    else if (itemId == GetRootItem())
    {
        // test that it is possible to change the text of the item being edited
        SetItemText(itemId, wxT("Editing root item"));
    }
}

void ArboCalcul::OnEndLabelEdit(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnEndLabelEdit"));

    // don't allow anything except letters in the labels
    if (!event.GetLabel().IsWord())
    {
        wxMessageBox(wxT("The new label should be a single word."));

        event.Veto();
    }
}

void ArboCalcul::OnItemCollapsing(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnItemCollapsing"));

    // for testing, prevent the user from collapsing the first child folder
    wxTreeItemId itemId = event.GetItem();
}

void ArboCalcul::OnItemActivated(wxTreeEvent& event)
{
    // show some info about this item
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);

    if (item != NULL)
    {
        item->ShowInfo(this);
    }

    wxLogMessage(wxT("OnItemActivated"));
}

void ArboCalcul::OnItemStateClick(wxTreeEvent& event)
{
    // toggle item state
    wxTreeItemId itemId = event.GetItem();
    DoToggleState(itemId);

    wxLogMessage(wxT("Item \"%s\" state changed to %d"),
        GetItemText(itemId), GetItemState(itemId));
}

void ArboCalcul::OnItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET(itemId.IsOk(), "should have a valid item");

    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);
    wxPoint clientpt = event.GetPoint();
    wxPoint screenpt = ClientToScreen(clientpt);

    wxLogMessage(wxT("OnItemMenu for item \"%s\" at screen coords (%i, %i)"),
        item ? item->GetDesc() : wxString(wxS("unknown")), screenpt.x, screenpt.y);

    ShowMenu(itemId, clientpt);
    event.Skip();
}

void ArboCalcul::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint pt = event.GetPosition();

    wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

    event.Skip();
}

void ArboCalcul::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
    wxString title;
    if (id.IsOk())
    {
        title << wxT("Menu for ") << GetItemText(id);
    }
    else
    {
        title = wxT("Menu for no particular item");
    }


    wxMenu menu(title);
    menu.Append(TreeTest_About, wxT("&About"));
    menu.AppendSeparator();
    menu.Append(TreeTest_Highlight, wxT("&Highlight item"));
    menu.Append(TreeTest_Dump, wxT("&Dump"));

    PopupMenu(&menu, pt);
// wxUSE_MENUS
}

void ArboCalcul::OnItemRClick(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET(itemId.IsOk(), "should have a valid item");

    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);

    wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc() : wxString(wxS("unknown")));

    event.Skip();
}

void ArboCalcul::OnRMouseDown(wxMouseEvent& event)
{
    wxLogMessage(wxT("Right mouse button down"));

    event.Skip();
}

void ArboCalcul::OnRMouseUp(wxMouseEvent& event)
{
    wxLogMessage(wxT("Right mouse button up"));

    event.Skip();
}

void ArboCalcul::OnRMouseDClick(wxMouseEvent& event)
{
    wxTreeItemId id = HitTest(event.GetPosition());
    if (!id)
    {
        wxLogMessage(wxT("No item under mouse"));
    }
    else
    {
        MyTreeItemData *item = (MyTreeItemData *)GetItemData(id);
        if (item)
        {
            wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
        }
    }

    event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
    return b ? wxT("") : wxT("not ");
}

void MyTreeItemData::ShowInfo(wxTreeCtrl *tree)
{
    wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
        wxT("%u children (%u immediately under this item)."),
        m_desc.c_str(),
        Bool2String(tree->IsSelected(GetId())),
        Bool2String(tree->IsExpanded(GetId())),
        Bool2String(tree->IsBold(GetId())),
        unsigned(tree->GetChildrenCount(GetId())),
        unsigned(tree->GetChildrenCount(GetId(), false)));
}
