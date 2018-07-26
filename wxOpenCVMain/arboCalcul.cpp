#include "wx/wxprec.h"


#include "GrapheOperation.h"
#include "FenetreAlgo.h"
#include "GlisserForme.h"
#include "ImageStat.h"

#include <wx/hyperlink.h>
#include <algorithm>



#if USE_GENERIC_TREECTRL
wxBEGIN_EVENT_TABLE(ArboCalcul, wxGenericTreeCtrl)
#else
wxBEGIN_EVENT_TABLE(ArboCalcul, wxTreeCtrl)
#endif
#if 0       // there are so many of those that logging them causes flicker
EVT_TREE_GET_INFO(TreeTest_Ctrl, ArboCalcul::OnGetInfo)
#endif
//EVT_TREE_SET_INFO(TreeTest_Ctrl, ArboCalcul::OnSetInfo)
//EVT_TREE_ITEM_EXPANDED(TreeTest_Ctrl, ArboCalcul::OnItemExpanded)
//EVT_TREE_ITEM_EXPANDING(TreeTest_Ctrl, ArboCalcul::OnItemExpanding)
//EVT_TREE_ITEM_COLLAPSED(TreeTest_Ctrl, ArboCalcul::OnItemCollapsed)
///EVT_TREE_ITEM_COLLAPSING(TreeTest_Ctrl, ArboCalcul::OnItemCollapsing)

///EVT_TREE_SEL_CHANGED(TreeTest_Ctrl, ArboCalcul::OnSelChanged)
//EVT_TREE_SEL_CHANGING(TreeTest_Ctrl, ArboCalcul::OnSelChanging)
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





// ArboCalcul implementation :
#if USE_GENERIC_TREECTRL
wxIMPLEMENT_DYNAMIC_CLASS(ArboCalcul, wxGenericTreeCtrl);
#else
wxIMPLEMENT_DYNAMIC_CLASS(ArboCalcul, wxTreeCtrl);
#endif

ArboCalcul::ArboCalcul(FenetrePrincipale *frame, wxOpencvApp *osg, wxWindow *parent, const wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style)
    : wxTreeCtrl(parent, id, pos, size, style),
    info(NULL)
{
    m_reverseSort = false;

    CreateImageList();
    CreateStateImageList();
    fenMere = frame;
    osgApp = osg;

    nbEtape = 0;
    nbParamMax = 0;
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ArboCalcul::OnMenuSelect, this, -1);

}
int ArboCalcul::FindIdOperation(int id)
{
	for (int i = 0; i < listeOp.size(); i++)
	{
		if (listeOp[i].idOperation == id)
			return i;
	}
	return -1;

}

int ArboCalcul::FindMaxEtapeOperation()
{
	int idMax = -1,iMax=-1;
	for (int i = 0; i < listeOp.size(); i++)
	{
		if (listeOp[i].indEtape > idMax)
		{
			idMax = listeOp[i].indEtape;
			iMax = i;
		}
	}
	return iMax;

}

int ArboCalcul::FindIdResOperation(int id)
{
	for (int i = 0; i < listeOp.size(); i++)
	{
		if (listeOp[i].indRes == id)
			return i;
	}
	return -1;

}


void ArboCalcul::Installation()
{
    wxString n;
    FenetrePrincipale *f = fenMere;
    listeImage = std::make_shared<wxImageList>(16,16);
    if (f)
    {
        listeImage.get()->Add(f->GetIcon());
        listeImage.get()->Add(f->GetIcon());
    }
    else
    {
        listeImage.get()->Add(wxArtProvider::GetIcon(wxART_WARNING));
        listeImage.get()->Add(wxArtProvider::GetIcon(wxART_WARNING));

    }
    SetImageList(listeImage.get());
    wxTreeItemId rootId;
    if (f)
    {
        rootId = AddRoot(f->GetTitle(), 0, 1, new InfoNoeud(f->GetTitle(), f, NULL));
        PileCalcul(rootId, f);
    }
    else
    { 
		int id=FindMaxEtapeOperation();
        if (id >= 0)
        {
            rootId = AddRoot(std::to_string(listeOp[id].indRes), 0, 1, new InfoNoeud(wxString(std::to_string(id)), wxString(std::to_string(id)),id, NULL));
            PileCalcul(rootId, &listeOp[id]);
        }

    }
    nbParamMax = 30;

}

void ArboCalcul::Printf(InfoNoeud *v)
{
    if (!info)
        return;
    if (v->Fenetre())
    {
        FenetrePrincipale *f = v->Fenetre();
        ImageInfoCV *im = f->ImAcq();
        if (!im)
            return;
        info->Clear();
        info->AppendText(wxString(im->InfoImage()));
    }
    else if (v->Operation())
    {

    }


};


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

    {
        wxIcon icons[2];
        icons[0] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_LIST);
        icons[1] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_LIST);

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

    if (f )
    {
        wxString n(f->GetTitle());
        wxIconBundle iconBundle = f->GetIcons();
        wxIcon icon=iconBundle.GetIcon(wxSize(32, 32), wxIconBundle::FALLBACK_NEAREST_LARGER);

        listeImage.get()->Add(icon);
        listeImage.get()->Add(icon);
        wxTreeItemId id;
        if (GetRootItem() == idParent)
            id = idParent;
        else
            id = AppendItem(idParent, n, listeImage.get()->GetImageCount()-2, listeImage.get()->GetImageCount() - 1, new InfoNoeud(n, f, idParent));
        std::map<std::string, ParametreOperation>::iterator it;
        for (it = f->ImAcq()->ListeOpAttribut()->begin(); it != f->ImAcq()->ListeOpAttribut()->end(); it++)
        {
            wxString n(it->second.nomOperation);
            wxTreeItemId idOp = AppendItem(id, n, -1, -1, new InfoNoeud(n, &it->second,nbEtape, id));
            fenAlgo.get()->AjouterEtape(nbEtape, &it->second, f->IdFenetre(),id);
            nbEtape++;
        }
        if (!f->OrigineImage()->nomOperation.empty())
            PileCalcul(id, f->OrigineImage());

    }
}

void ArboCalcul::PileCalcul(const wxTreeItemId& idParent, int indRes)
{

    if (indRes>=0)
    {
        wxString n(std::to_string(indRes));

        listeImage.get()->Add(wxArtProvider::GetIcon(wxART_WARNING));
        listeImage.get()->Add(wxArtProvider::GetIcon(wxART_WARNING));
        wxTreeItemId id;
        if (GetRootItem() == idParent)
            id = idParent;
        else
            id = AppendItem(idParent, n, listeImage.get()->GetImageCount() - 2, listeImage.get()->GetImageCount() - 1, new InfoNoeud(n, wxString(std::to_string(indRes)),indRes, idParent));
        int idOp = FindIdResOperation(indRes);
        if (idOp>=0)
            PileCalcul(id, &listeOp[idOp]);

    }

}


void ArboCalcul::PileCalcul(const wxTreeItemId& idParent, ParametreOperation *pOCV)
{

    if (pOCV != NULL)
    {
        wxString n(pOCV->nomOperation);
        wxBitmap b = ((wxOpencvApp*)osgApp)->BitmapOperateur(wxString(pOCV->nomOperation));
        wxImage image;
        wxBitmap newBitmap; ;
        wxIcon icon;
        if (b.IsOk())
        {
            image = b.ConvertToImage();
            image.Rescale(32, 32);
            newBitmap = wxBitmap(image);
            icon.CopyFromBitmap(newBitmap);
        }
        else
        {
            icon = wxArtProvider::GetIcon(wxART_WARNING);
        }

        listeImage.get()->Add(icon);
        listeImage.get()->Add(icon);
        wxTreeItemId id = AppendItem(idParent, n, listeImage.get()->GetImageCount() - 2, listeImage.get()->GetImageCount() - 1, new InfoNoeud(n, pOCV, nbEtape, idParent));
        fenAlgo.get()->AjouterEtape(nbEtape, pOCV, -1, id);
        nbEtape++;
        for (int i = 0; i<pOCV->nbOperande; i++)
        {
            int idF = pOCV->indOpFenetre[i];
            FenetrePrincipale *fId = ((wxOpencvApp *)osgApp)->Fenetre(idF);
            if (fId)
                PileCalcul(id, fId);
            else
            {
                PileCalcul(id, idF);

            }

        }

    }
}

bool ArboCalcul::ModifNoeud(FenetrePrincipale *f, wxTreeItemId w)
{
    wxString n(f->GetTitle());
    wxIconBundle iconBundle = f->GetIcons();
    wxIcon icon = iconBundle.GetIcon(wxSize(32, 32), wxIconBundle::FALLBACK_NEAREST_LARGER);

    listeImage.get()->Add(icon);
    listeImage.get()->Add(icon);
    InfoNoeud *item = (InfoNoeud *)GetItemData(w);
    wxTreeItemId idParent;
    if (item != NULL)
    {
        idParent = item->getParent();
    }
    else
        return false;
    InfoNoeud *itempocv = (InfoNoeud *)GetItemData(idParent);
    if (item->Fenetre())
    {
        if (itempocv->Operation())
        {
            ParametreOperation *pocv = itempocv->Operation();
            ImageInfoCV *imAcq = item->Fenetre()->ImAcq();
            for (int i = 0; i < pocv->nbOperande; i++)
            {
                if (pocv->op[i] == imAcq)
                {
                    pocv->op[i] = f->ImAcq();
                    pocv->indOpFenetre[i] = ((wxOpencvApp*)osgApp)->RechercheFenetre(f->ImAcq());
                    break;
                }
            }
        }
        else
            return false;
        SetItemText(w, n);
        SetItemData(w, new InfoNoeud(n, f, idParent));
        SetItemImage(w, listeImage.get()->GetImageCount() - 2);
    //    id = AppendItem(idParent, n, listeImage.get()->GetImageCount() - 2, listeImage.get()->GetImageCount() - 1, new InfoNoeud(n, f, idParent));
        if (item != NULL)
        {
            delete item;

        }
        if (itempocv->Operation())
            fenAlgo.get()->ExecuterOperation(itempocv->IndiceOnglet());
    }
    else
    {
        ArboCalculParam p;
        p.indFen = item->IndiceFenetre();
        p.fen = f;
        wxTreeItemId root = GetRootItem();
        ExplorerArbre(root, p, &ArboCalcul::ReplacerIdParFenetre);
    }
    Collapse(idParent);
    Expand(idParent);
    return true;
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

//TREE_EVENT_HANDLER(OnGetInfo)
//TREE_EVENT_HANDLER(OnSetInfo)
//TREE_EVENT_HANDLER(OnItemExpanded)
//TREE_EVENT_HANDLER(OnItemExpanding)
//TREE_EVENT_HANDLER(OnItemCollapsed)
//TREE_EVENT_HANDLER(OnSelChanged)
//TREE_EVENT_HANDLER(OnSelChanging)

#undef TREE_EVENT_HANDLER




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
    InfoNoeud *item = (InfoNoeud *)GetItemData(itemId);

    if (item != NULL)
    {
        item->ShowInfo(this);
        if (item->Operation())
            fenAlgo.get()->ActiverOnglet(item->IndiceOnglet());
    }

}

void ArboCalcul::OnItemStateClick(wxTreeEvent& event)
{
    // toggle item state
    wxTreeItemId itemId = event.GetItem();
//    DoToggleState(itemId);

    wxLogMessage(wxT("Item \"%s\" state changed to %d"),
        GetItemText(itemId), GetItemState(itemId));
}

void ArboCalcul::OnMenuSelect(wxCommandEvent& evt)
{
    wxTreeItemId w=GetSelection();
    InfoNoeud *item = (InfoNoeud *)GetItemData(w);

    switch (evt.GetId()) {
    case TreeTest_Save:
        SauverSequence(w);
        break;
    case TreeTest_About:
        Printf(item);
        break;
    case TreeTest_Highlight:
        if (item->Fenetre())
        {
            item->Fenetre()->Iconize(false);
            item->Fenetre()->SetFocus();
            item->Fenetre()->Raise();
            item->Fenetre()->Show(true);
        }
        else if (item->Operation())
            {
            }
        else
            wxLogMessage(wxT("Unknown"));
        break;
    case TreeTest_Select_operande:
        ((wxOpencvApp*)osgApp)->DefPointeurSouris(3, 1);
        ((wxOpencvApp*)osgApp)->DefNoeudCalcul(this, GetSelection());
        break;
    }

    {

    }
}
void ArboCalcul::OnItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET(itemId.IsOk(), "should have a valid item");

    InfoNoeud *item = (InfoNoeud *)GetItemData(itemId);
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


    event.Skip();
}

void ArboCalcul::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
    wxString title;
    if (!id.IsOk())
        return;
    idEvt = id;
    InfoNoeud *item = (InfoNoeud *)GetItemData(id);

    if (item != NULL)
    {
        wxMenu menu(title);
        if (item->getParent() == NULL)
        {
            menu.Append(TreeTest_Save, wxT("&Save"));
            menu.AppendSeparator();

        }
        if (item->Operation())
            title << wxT("Operation ") << GetItemText(id);
        else
        {
            title << wxT("Image ") << GetItemText(id);

        }
        menu.Append(TreeTest_About, wxT("&About"));
        menu.AppendSeparator();
        menu.Append(TreeTest_Highlight, wxT("&Select item")+GetItemText(id));
        if (!item->Operation() && GetChildrenCount(id)==0)
        {
            menu.AppendSeparator();
            menu.Append(TreeTest_Select_operande, wxT("&New operande"));
        }

        PopupMenu(&menu, pt);
    }
// wxUSE_MENUS
}

void ArboCalcul::OnItemRClick(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET(itemId.IsOk(), "should have a valid item");

    InfoNoeud *item = (InfoNoeud *)GetItemData(itemId);

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
        InfoNoeud *item = (InfoNoeud *)GetItemData(id);
        if (item)
        {
            wxLogMessage(wxT("Item '%s' under mouse"), item->Operation()->nomOperation);
        }
    }

    event.Skip();
}

void ArboCalcul::SauverSequence(wxTreeItemId &idParent)
{
    if (osgApp == NULL )
        return;
    wxString nomFic("Nom A Voir");
    nomFic.Replace(" ", "_");
    cv::FileStorage fsy;
    fsy.open((std::string)nomFic.c_str() + ".yml", cv::FileStorage::WRITE);
    wxTreeItemId t = GetRootItem();
    if (t == idParent)
    {
        nbEtape = 0;
        ArboCalculParam p;
        p.fs.open((std::string)nomFic.c_str() + ".xml", cv::FileStorage::WRITE);
        SauverNoeud(t, p);
        nbEtape = 0;
        p.fs.open((std::string)nomFic.c_str() + ".yml", cv::FileStorage::WRITE);;
        SauverNoeud(t, p);
    }

}

void ArboCalcul::ExplorerArbre(wxTreeItemId &id, ArboCalculParam &p, void (ArboCalcul::*fonctionNoeud)(wxTreeItemId &, ArboCalculParam &p) )
{
    if (osgApp == NULL)
        return;
    if (id.IsOk())
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId tf = GetFirstChild(id, cookie);
        while (tf.IsOk())
        {
            InfoNoeud *item = (InfoNoeud *)GetItemData(tf);
            if (item->Operation())
            {
                wxTreeItemId tfch = tf;
                ExplorerArbre(tfch, p,fonctionNoeud);
            }
            else
            {
                wxTreeItemId tfch = tf;
                ExplorerArbre(tfch, p, fonctionNoeud);
            }
            tf = GetNextChild(id, cookie);
        }
        (this->*fonctionNoeud)(id, p);
/*        InfoNoeud *item = (InfoNoeud *)GetItemData(id);
        if (item->Operation())
        {
            item->Operation()->indEtape = nbEtape;
            item->Operation()->write(fs);
            nbEtape++;
        }*/
    }
    return;
}
void ArboCalcul::SauverNoeud(wxTreeItemId &id, ArboCalculParam & p)
{
    InfoNoeud *item = (InfoNoeud *)GetItemData(id);
    if (item && item->Operation() && p.fs.isOpened())
    {
        item->Operation()->indEtape = nbEtape;
        item->Operation()->write(p.fs);
        nbEtape++;
    }

}

void ArboCalcul::ReplacerIdParFenetre(wxTreeItemId &id, ArboCalculParam & p)
{
    InfoNoeud *item = (InfoNoeud *)GetItemData(id);
    if (item )
    {
        if (item->IndiceFenetre() == p.indFen)
        {
            item->DefFenetre(p.fen);
            wxString n(p.fen->GetTitle());
            wxIconBundle iconBundle = p.fen->GetIcons();
            wxIcon icon = iconBundle.GetIcon(wxSize(32, 32), wxIconBundle::FALLBACK_NEAREST_LARGER);

            listeImage.get()->Add(icon);
            listeImage.get()->Add(icon);
            wxTreeItemId idParent;
            if (item != NULL)
            {
                idParent = item->getParent();
            }
            SetItemText(id, p.fen->GetTitle());
            SetItemData(id, new InfoNoeud(p.fen->GetTitle(), p.fen, idParent));
            SetItemImage(id, listeImage.get()->GetImageCount() - 2);
            //    id = AppendItem(idParent, n, listeImage.get()->GetImageCount() - 2, listeImage.get()->GetImageCount() - 1, new InfoNoeud(n, f, idParent));
            if (item != NULL)
            {
                delete item;

            }

        }
    }

}

/*void ArboCalcul::SauverNoeud(wxTreeItemId &id, cv::FileStorage &fs)
{
    if (osgApp == NULL )
        return;
    if (id.IsOk())
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId tf = GetFirstChild(id, cookie);
        while (tf.IsOk())
        {
            InfoNoeud *item = (InfoNoeud *)GetItemData(tf);
            if (item->Operation())
            {
                wxTreeItemId tfch = tf;
                SauverNoeud(tfch, fs);
            }
            else
            {
                wxTreeItemId tfch = tf;
                SauverNoeud(tfch, fs);
            }
            tf = GetNextChild(id, cookie);
        }
        InfoNoeud *item = (InfoNoeud *)GetItemData(id);
        if (item->Operation())
        {
            item->Operation()->indEtape = nbEtape;
            item->Operation()->write(fs);
            nbEtape++;
        }
    }
    return;
}*/



static inline const wxChar *Bool2String(bool b)
{
    return b ? wxT("") : wxT("not ");
}

void InfoNoeud::ShowInfo(wxTreeCtrl *tree)
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

FenetreInfoOperation::FenetreInfoOperation(GrapheOperation *t, FenetrePrincipale *frame, wxOpencvApp *osg)
{
    if (frame == NULL)
        return;
    int hMax = 0, lMax = 0;
    fenMere = frame;
    osgApp = osg;
    wFen = t->Panel();
    classeur = t->Classeur();
    arbre = NULL;
    FenetrePrincipale *f = fenMere;
    nbEtape = 0;
    nbParamMax = 30;
    t->Bind(wxEVT_SPINCTRLDOUBLE, &FenetreInfoOperation::OnSpinReel, this);
    t->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &FenetreInfoOperation::ComboBox, this);
    t->Bind(wxEVT_TEXT_ENTER, &FenetreInfoOperation::OnTextValider, this);
    //    SetSizerAndFit(topsizer);
    t->Show(true);
    listeOp.clear();
    return;
}

FenetreInfoOperation::FenetreInfoOperation(GrapheOperation *t, std::vector<ParametreOperation> arbreOperation, wxOpencvApp *osg)
{
    int hMax = 0, lMax = 0;
    fenMere = NULL;
    osgApp = osg;
    wFen = t->Panel();
    classeur = t->Classeur();
    arbre = NULL;
    nbEtape = 0;
    nbParamMax = 30;
    t->Bind(wxEVT_SPINCTRLDOUBLE, &FenetreInfoOperation::OnSpinReel, this);
    t->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &FenetreInfoOperation::ComboBox, this);
    t->Bind(wxEVT_TEXT_ENTER, &FenetreInfoOperation::OnTextValider, this);
    //    SetSizerAndFit(topsizer);
    t->Show(true);
    listeOp.clear();
}



void FenetreInfoOperation::AjouterEtape(int nb, ParametreOperation *pOCV, int idFenetre,wxTreeItemId &n)
{
    noeuds.insert(std::pair<ParametreOperation *,wxTreeItemId &>(pOCV, n));
    listeOp.push_back(std::pair< ParametreOperation*, int>(pOCV, idFenetre));
    wxWindow *w = CreerOngletEtape(classeur, nb);
    listeOnglet[w] = std::pair<wxString, int>(pOCV->nomOperation, nb);
    wxString nom(_("Step"));
    nom.Printf("%s %d : %s", nom, nb, pOCV->nomOperation);
    classeur->InsertPage(0, w, nom, nb == 1);

}

wxWindow *FenetreInfoOperation::CreerOngletEtape(wxNotebook *classeur, int indOp)
{
    // nbParamMax  nombre d'article maximum par onglet
    wxWindow *page = new wxWindow(classeur, -1);
    ParametreOperation *pOCV = listeOp[indOp].first;
    int nbParam = 1;
    int ligne = 50;
    int indOriCtrl = 1 + indOp * nbParamMax;// Dépend de l'indice de l'opérateur pour éviter le recouvrement des onglets 
    new wxHyperlinkCtrl(page, indOriCtrl, " OpenCV Documentation", pOCV->lienHtml, wxPoint(10, 10), wxSize(400, 20));
    new wxHyperlinkCtrl(page, indOriCtrl + 1, "PDF Documentation", pOCV->refPDF, wxPoint(10, 30), wxSize(400, 20));
    if (tailleMax.x<410)
        tailleMax.x = 410;
    if (tailleMax.y<ligne)
        tailleMax.y = ligne;
    std::map<std::string, DomaineParametreOp<cv::Size> >::iterator its;
    for (its = pOCV->sizeParam.begin(); its != pOCV->sizeParam.end(); its++)
    {
        wxString nombre;
        nombre.Printf("%d", its->second.valeur.width);
        wxPoint p(10, ligne);
        wxSize	s(100, 20);
        new wxStaticText(page, indOriCtrl + 2 * nbParam, its->first + " x", p, s);
        p += wxPoint(s.GetX(), 0);
        //	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
        wxSpinCtrlDouble *spw = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        spw->SetDigits(5);
        if (its->second.res)
            spw->Disable();
        spw->SetRange(its->second.mini.width, its->second.maxi.width);
        spw->SetIncrement((double)its->second.pas.width);
        spw->SetValue(its->second.valeur.width);
        p += wxPoint(s.GetX(), 0);
        nbParam++;
        new wxStaticText(page, indOriCtrl + 2 * nbParam, its->first + " y", p, s);
        p += wxPoint(s.GetX(), 0);
        nombre.Printf("%d", its->second.valeur.height);
        //	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
        //	sph->SetRange(0,256); 
        wxSpinCtrlDouble *sph = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        sph->SetDigits(5);
        if (its->second.res)
            sph->Disable();
        sph->SetRange(its->second.mini.height, its->second.maxi.height);
        sph->SetIncrement((double)its->second.pas.height);
        sph->SetValue(its->second.valeur.height);

        nbParam++;
        ligne += 20;
        if (tailleMax.x<p.x + s.x)
            tailleMax.x = p.x + s.x;
        if (tailleMax.y<p.y + s.y)
            tailleMax.y = p.y + s.y;
    }
    std::map<std::string, DomaineParametreOp<int> >::iterator iti;
    for (iti = pOCV->intParam.begin(); iti != pOCV->intParam.end(); iti++)
    {
        wxString nombre;
        nombre.Printf("%d", iti->second.valeur);
        wxPoint p(10, ligne);
        wxSize	s(100, 20);
        new wxStaticText(page, indOriCtrl + 2 * nbParam, iti->first, p, s);
        p += wxPoint(s.GetX(), 0);
        //	wxSpinCtrl *sp=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
        //	sp->SetRange(iti->second.mini,iti->second.maxi); 
        if (ParametreOperation::listeParam.find(iti->first) != ParametreOperation::listeParam.end())
        {
            int nbChaine = pOCV->listeParam[iti->first].size();
            wxString *choix = new wxString[nbChaine], choixDefaut;
            int i = 0;
            for (std::map <std::string, int >::iterator iter = pOCV->listeParam[iti->first].begin(); iter != pOCV->listeParam[iti->first].end(); ++iter, ++i)
            {
                choix[i] = iter->first;
                if (iter->second == iti->second.valeur)
                    choixDefaut = iter->first;
            }
            wxComboBox *choixOp = new wxComboBox(page, indOriCtrl + 2 * nbParam + 1, choixDefaut, p, wxSize(250, -1), nbChaine, choix);
            if (iti->second.res)
                choixOp->Disable();

        }
        else
        {
            wxSpinCtrlDouble *sp = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
            sp->SetRange(iti->second.mini, iti->second.maxi);
            sp->SetIncrement(iti->second.pas);
            if (iti->second.res)
                sp->Disable();
        }
        nbParam++;
        ligne += 20;
        if (tailleMax.x<p.x + s.x)
            tailleMax.x = p.x + s.x;
        if (tailleMax.y<p.y + s.y)
            tailleMax.y = p.y + s.y;
    }
    std::map<std::string, DomaineParametreOp<double> >::iterator itd;
    for (itd = pOCV->doubleParam.begin(); itd != pOCV->doubleParam.end(); itd++)
    {
        wxString nombre;
        nombre.Printf("%f", itd->second.valeur);
        wxPoint p(10, ligne);
        wxSize	s(100, 20);
        new wxStaticText(page, indOriCtrl + 2 * nbParam, itd->first, p, s);
        p += wxPoint(s.GetX(), 0);
        wxSpinCtrlDouble *sp = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        sp->SetDigits(5);
        if (itd->second.res)
            sp->Disable();
        sp->SetRange(itd->second.mini, itd->second.maxi);
        sp->SetIncrement(itd->second.pas);
        nbParam++;
        ligne += 20;
        if (tailleMax.x<p.x + s.x)
            tailleMax.x = p.x + s.x;
        if (tailleMax.y<p.y + s.y)
            tailleMax.y = p.y + s.y;
    }
    std::map<std::string, DomaineParametreOp<cv::Point> >::iterator itp;
    int indCouleur = 0;
    for (itp = pOCV->pointParam.begin(); itp != pOCV->pointParam.end(); itp++)
    {

        wxString nombre;
        nombre.Printf("%d", itp->second.valeur.x);
        wxPoint p(10, ligne);
        wxSize	s(100, 20);
        new wxStaticText(page, indOriCtrl + 2 * nbParam, itp->first + " x", p, s);
        p += wxPoint(s.GetX(), 0);
        //	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
        wxSpinCtrlDouble *spw = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        spw->SetDigits(5);
        if (itp->second.res)
            spw->Disable();
        spw->SetRange(itp->second.mini.x, itp->second.maxi.x);
        spw->SetIncrement((double)itp->second.pas.x);
        spw->SetValue(itp->second.valeur.x);
        p += wxPoint(s.GetX(), 0);
        nbParam++;
        new wxStaticText(page, indOriCtrl + 2 * nbParam, itp->first + " y", p, s);
        p += wxPoint(s.GetX(), 0);
        nombre.Printf("%d", itp->second.valeur.y);
        //	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
        //	sph->SetRange(0,256); 
        wxSpinCtrlDouble *sph = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        if (itp->second.res)
            sph->Disable();
        sph->SetRange(itp->second.mini.y, itp->second.maxi.y);
        sph->SetIncrement((double)itp->second.pas.y);
        sph->SetValue(itp->second.valeur.y);
        if (fenMere && itp->second.mouseScan)
        {
            spinSouris.push_back(std::make_pair(spw, sph));
            paramSouris.push_back(&itp->second);
            ongletSouris.push_back(page);
            DragShape *f = fenMere->Feuille()->AjouteForme(wxPoint(itp->second.valeur.x, itp->second.valeur.y), indCouleur++, 0, this, spinSouris.size() - 1, itp->first);
            formeSouris[spw] = f;
            formeSouris[sph] = f;
        }

        nbParam++;
        ligne += 20;
        if (tailleMax.x<p.x + s.x)
            tailleMax.x = p.x + s.x;
        if (tailleMax.y<p.y + s.y)
            tailleMax.y = p.y + s.y;
    }
    //page->SetClientSize(tailleMax+wxSize(20,40));
    return page;
}



void FenetreInfoOperation::ActiverOnglet(int ind)
{
    classeur->SetSelection(classeur->GetPageCount()-1-ind);
}

void FenetreInfoOperation::OnActivate(wxActivateEvent& event)
{
}

void FenetreInfoOperation::OnPaint(wxPaintEvent& event)
{
}

void FenetreInfoOperation::OnSpinEntier(wxSpinEvent &w)
{
    wxOpencvApp *app = (wxOpencvApp *)osgApp;
    if (!osgApp)
        return;
    std::string nom;
    int ind = listeOnglet[classeur->GetCurrentPage()].second;

    ParametreOperation *pOCV = listeOp[ind].first;
    wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 1, wFen);
    nom = st->GetLabel().c_str();
    if (pOCV->intParam.find(nom) != pOCV->intParam.end())
    {
        int ds = pOCV->intParam[nom].valeur - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
        if (ds == 0)
            return;
        int valeur = pOCV->intParam[nom].maxi + 1;
        if (ds<0)
            valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
        if (ds>0)
            valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
        if (valeur >= pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi &&
            (valeur%pOCV->intParam[nom].pas) == (pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
        {
            pOCV->intParam[nom].valeur = valeur;
            ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
            ExecuterOperation(ind);
        }
        else
            wxMessageBox("Selected value is out of range or forbidden");
    }
    if (pOCV->sizeParam.find(nom) != pOCV->sizeParam.end())
    {
        if ((w.GetId() - 1) % 4 == 0)
        {
            if (pOCV->sizeParam[nom].valeur.width == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
                return;
            int ds = pOCV->sizeParam[nom].valeur.width - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
            if (ds == 0)
                return;
            int valeur = pOCV->sizeParam[nom].maxi.width + 1;
            if (ds<0)
                valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
            if (ds>0)
                valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
            if (valeur >= pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width &&
                (valeur%pOCV->sizeParam[nom].pas.width) == (pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
            {
                pOCV->sizeParam[nom].valeur.width = valeur;
                ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
                ExecuterOperation(ind);
            }
        }
        else
        {
            if (pOCV->sizeParam[nom].valeur.height == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
                return;
            int ds = pOCV->sizeParam[nom].valeur.height - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
            if (ds == 0)
                return;
            int valeur = pOCV->sizeParam[nom].maxi.height + 1;
            if (ds<0)
                valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
            if (ds>0)
                valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
            if (valeur >= pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height &&
                (valeur%pOCV->sizeParam[nom].pas.height) == (pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
            {
                pOCV->sizeParam[nom].valeur.height = valeur;
                ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
                ExecuterOperation(ind);

            }
        }
    }

}

void FenetreInfoOperation::OnSpinReel(wxSpinDoubleEvent &w)
{
    wxOpencvApp *app = (wxOpencvApp *)osgApp;
    if (!osgApp)
        return;
    std::string nom;
    int ind = listeOnglet[classeur->GetCurrentPage()].second;

    ParametreOperation *pOCV = listeOp[ind].first;
    wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 1, wFen);
    nom = st->GetLabel().c_str();
    double x = w.GetValue();
    if (pOCV->doubleParam.find(nom) != pOCV->doubleParam.end())
    {
        if (pOCV->doubleParam[nom].valeur == x)
            return;
        pOCV->doubleParam[nom].valeur = x;
    }
    if (pOCV->intParam.find(nom) != pOCV->intParam.end())
    {
        if (pOCV->intParam[nom].valeur == (int)x)
            return;
        pOCV->intParam[nom].valeur = x;
    }
    if (pOCV->sizeParam.find(nom.substr(0, nom.length() - 2)) != pOCV->sizeParam.end())
    {
        if ((w.GetId()) % 4 == 0)
        {
            if (pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.width == (int)x)
                return;
            pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.width = (int)x;
        }
        else
        {
            if (pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.height == (int)x)
                return;
            pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.height = (int)x;
        }
    }
    if (pOCV->pointParam.find(nom.substr(0, nom.length() - 2)) != pOCV->pointParam.end())
    {
        if (nom.substr(nom.length() - 1, 1) == 'x')
        {
            if (pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.x == (int)x)
                return;
            pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.x = (int)x;
        }
        else
        {
            if (pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.y == (int)x)
                return;
            pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.y = (int)x;
        }
        if (formeSouris.find((wxSpinCtrlDouble*)w.GetEventObject()) != formeSouris.end())
        {
            wxPoint p(pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.x, pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.y);
            formeSouris[(wxSpinCtrlDouble*)w.GetEventObject()]->SetPosition(p, false);
            fenMere->Feuille()->Update();
        }
    }
    //if (fenMere)
        ExecuterOperation(ind);
}

void  FenetreInfoOperation::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreInfoOperation::OnSpinMoins(wxSpinEvent& w)
{
    wxOpencvApp *app = (wxOpencvApp *)osgApp;
    if (!osgApp)
        return;
    std::string nom;
    int ind = listeOnglet[classeur->GetCurrentPage()].second;

    ParametreOperation *pOCV = listeOp[ind].first;
    wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 1, wFen);
    nom = st->GetLabel().c_str();
    if (pOCV->intParam.find(nom) != pOCV->intParam.end())
    {
        int ds = pOCV->intParam[nom].valeur - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
        if (ds == 0)
            return;
        int valeur = pOCV->intParam[nom].maxi + 1;
        if (ds<0)
            valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
        if (ds>0)
            valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
        if (valeur >= pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi &&
            (valeur%pOCV->intParam[nom].pas) == (pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
        {
            pOCV->intParam[nom].valeur = valeur;
            ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
            ExecuterOperation(ind);
        }
        else
            wxMessageBox("Selected value is out of range or forbidden");
    }
    if (pOCV->sizeParam.find(nom) != pOCV->sizeParam.end())
    {
        if ((w.GetId() - 1) % 4 == 0)
        {
            if (pOCV->sizeParam[nom].valeur.width == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
                return;
            int ds = pOCV->sizeParam[nom].valeur.width - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
            if (ds == 0)
                return;
            int valeur = pOCV->sizeParam[nom].maxi.width + 1;
            if (ds<0)
                valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
            if (ds>0)
                valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
            if (valeur >= pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width &&
                (valeur%pOCV->sizeParam[nom].pas.width) == (pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
            {
                pOCV->sizeParam[nom].valeur.width = valeur;
                ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
                ExecuterOperation(ind);
            }
        }
        else
        {
            if (pOCV->sizeParam[nom].valeur.height == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
                return;
            int ds = pOCV->sizeParam[nom].valeur.height - ((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
            if (ds == 0)
                return;
            int valeur = pOCV->sizeParam[nom].maxi.height + 1;
            if (ds<0)
                valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
            if (ds>0)
                valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
            if (valeur >= pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height &&
                (valeur%pOCV->sizeParam[nom].pas.height) == (pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
            {
                pOCV->sizeParam[nom].valeur.height = valeur;
                ((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
                ExecuterOperation(ind);

            }
        }
    }
}

void FenetreInfoOperation::OnTextValider(wxCommandEvent &w)
{
}
void FenetreInfoOperation::OnKeyDown(wxKeyEvent &)
{
}

void FenetreInfoOperation::OnClose(wxCloseEvent& event)
{
    fenMere->RAZGrapheOperation();

}


void FenetreInfoOperation::ComboBox(wxCommandEvent &w)
{
	wxOpencvApp *app = (wxOpencvApp *)osgApp;
	if (!osgApp)
		return;
	std::string nom;
	int ind = listeOnglet[classeur->GetCurrentPage()].second;

	ParametreOperation *pOCV = listeOp[ind].first;
	wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 1, wFen);
	nom = st->GetLabel().c_str();
	if (pOCV->doubleParam.find(nom) != pOCV->doubleParam.end())
	{
		if (pOCV->doubleParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->doubleParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	if (pOCV->intParam.find(nom) != pOCV->intParam.end())
	{

		if (ParametreOperation::listeParam.find(nom) != ParametreOperation::listeParam.end())
		{
			int nb = ((wxComboBox*)(w.GetEventObject()))->GetCurrentSelection();
			int i = 0;
			std::map <std::string, int  >::iterator iter = pOCV->listeParam[nom].begin();
			for (; iter != pOCV->listeParam[nom].end() && i != nb; ++iter, ++i);
			if (i == nb)
				pOCV->intParam[nom].valeur = iter->second;
		}
		else
		{
			if (pOCV->intParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
				return;
			pOCV->intParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
	if (pOCV->sizeParam.find(nom) != pOCV->sizeParam.end())
	{
		if ((w.GetId()) % 4 == 0)
		{
			if (pOCV->sizeParam[nom].valeur.width == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
				return;
			pOCV->sizeParam[nom].valeur.width = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
		else
		{
			if (pOCV->sizeParam[nom].valeur.height == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
				return;
			pOCV->sizeParam[nom].valeur.height = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
	if (fenMere)
		ExecuterOperation(ind);

}

std::vector<std::pair<ParametreOperation*, int>> FenetreInfoOperation::FindOperande(ImageInfoCV *x)
{
    std::map<ParametreOperation *, wxTreeItemId  >::iterator w = noeuds.begin();
    std::vector<std::pair<ParametreOperation*,int>> p;
    for (auto n : noeuds)
    {
        for (int i = 0;i<n.first->nbOperande && i<n.first->op.size();i++)
            if (n.first->op[i] == x)
            {
                p.push_back(std::make_pair(n.first, i));
            }
    }
    return p;
}

void FenetreInfoOperation::ExecuterOperation(int indOperation)
{
    if (!osgApp)
        return;
    wxOpencvApp	*app = (wxOpencvApp *)osgApp;
    ImageInfoCV **im = NULL;
    ParametreOperation *pOCV = listeOp[indOperation].first;

    std::map<ParametreOperation *, wxTreeItemId  >::iterator w = noeuds.find( pOCV);
    std::vector<ImageInfoCV*>r;
    if (w == noeuds.end())
        return;
    wxTreeItemId noeud=w->second;
    while (noeud != arbre->GetRootItem()) 
    {
        if (pOCV != NULL)
        {
            if (app->VerifImagesExiste(pOCV))
            {

                wxTreeItemId t = noeud;
                InfoNoeud *item = (InfoNoeud *)arbre->GetItemData(t);
                t = item->getParent();
                item = (InfoNoeud *)arbre->GetItemData(t);
                if (item )
                {
                    app->DefOperateurImage(wxString(pOCV->nomOperation));
                    for (int i = 0; i < pOCV->nbOperande && i<pOCV->op.size(); i++)
                    {
                        int indFen = app->RechercheFenetre(pOCV->op[i]);
                        if (indFen < 0)
                        {
                            wxMessageBox(_("Previous image is closed?"), _("Problem"), wxOK);
                            return;
                        }
                        else
                           app->DefOperandeN(pOCV->op[i], indFen);
                    }
                    r = pOCV->ExecuterOperation();
                    if (pOCV->opErreur)
                    {
                        wxMessageBox(pOCV->msgErreur, _("Exception"), wxICON_ERROR);
                        pOCV->opErreur = 0;
                        return;
                    }
                    if (r.size() != 0)
                    {
                        item = (InfoNoeud *)arbre->GetItemData(t);
                        FenetrePrincipale *f=item->Fenetre();
                        std::vector<std::pair<ParametreOperation*, int>> pl=FindOperande(f->ImAcq());
                        for (auto p : pl)
                        {
                            p.first->op[p.second] = r[0];
                        }
                        if (f && f->ImAcq() != r[0])
                            f->AssosierImage(r[0]);
                        f->DynamiqueAffichage();

                        f->NouvelleImage();
                        f->MAJNouvelleImage();
                        if (f->ImgStat())
                        {
                            f->ImgStat()->Plot(true);
                            f->ImgStat()->MAJOnglet(-1);
                        }
                        f->DefHistorique();
                        noeud = item->getParent();
                        if (noeud && noeud != arbre->GetRootItem())
                        {
                            InfoNoeud *item = (InfoNoeud *)arbre->GetItemData(noeud);
                            pOCV = item->Operation();
                        }
                        else
                        {
                            pOCV = NULL;
                            noeud = arbre->GetRootItem();
                        }
                    }
                }
                else
                {
                    pOCV = NULL;
                    noeud = arbre->GetRootItem();
                }
            }
            else
            {
                pOCV = NULL;
                noeud = arbre->GetRootItem();
            }

        }

    } 

}

