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


void ArboCalcul::OnMenuSelect(wxCommandEvent& evt)
{
    wxTreeItemId w = GetSelection();
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
        menu.Append(TreeTest_Highlight, wxT("&Select item") + GetItemText(id));
        if (!item->Operation() && GetChildrenCount(id) == 0)
        {
            menu.AppendSeparator();
            menu.Append(TreeTest_Select_operande, wxT("&New operande"));
        }

        PopupMenu(&menu, pt);
    }
    // wxUSE_MENUS
}


void ArboCalcul::SauverSequence(wxTreeItemId &idParent)
{
    if (osgApp == NULL)
        return;
    wxString nomFic("Nom A Voir");
    nomFic.Replace(" ", "_");
    wxTreeItemId t = GetRootItem();
    if (t == idParent)
    {
        nbEtape = 0;
        ArboCalculParam p;
        p.fs.open((std::string)nomFic.c_str() + ".xml", cv::FileStorage::WRITE);
        ExplorerArbre(t,p,&ArboCalcul::SauverNoeud);
        p.fs.release();
        nbEtape = 0;
        p.fs.open((std::string)nomFic.c_str() + ".yml", cv::FileStorage::WRITE);;
        ExplorerArbre(t, p, &ArboCalcul::SauverNoeud);
        p.fs.release();
    }

}

void ArboCalcul::ExplorerArbre(wxTreeItemId &id, ArboCalculParam &p, void (ArboCalcul::*fonctionNoeud)(wxTreeItemId &, ArboCalculParam &p, bool))
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
                ExplorerArbre(tfch, p, fonctionNoeud);
                (this->*fonctionNoeud)(id, p,false);
            }
            else
            {
                wxTreeItemId tfch = tf;
                ExplorerArbre(tfch, p, fonctionNoeud);
                (this->*fonctionNoeud)(id, p,false);
            }
            tf = GetNextChild(id, cookie);
        }
        (this->*fonctionNoeud)(id, p,true);
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
void ArboCalcul::SauverNoeud(wxTreeItemId &id, ArboCalculParam & p, bool quitterBranche)
{
    if (!quitterBranche)
        return;
    InfoNoeud *item = (InfoNoeud *)GetItemData(id);
    if (item && item->Operation() && p.fs.isOpened())
    {
        item->Operation()->indEtape = nbEtape;
        item->Operation()->write(p.fs);
        nbEtape++;
    }

}

void ArboCalcul::ExecuterNoeud(wxTreeItemId &id, ArboCalculParam & p, bool quitterBranche)
{
    if (!quitterBranche)
        return;
    InfoNoeud *data = (InfoNoeud *)GetItemData(id);

    if (data->Operation())
    {
        ParametreOperation *pOCV = data->Operation();
        if (((wxOpencvApp*)osgApp)->VerifImagesExiste(pOCV) )
        {
            FenetrePrincipale *f=((wxOpencvApp *)osgApp)->Graphique(pOCV->indRes);
            if (!f)
            {
                r = pOCV->ExecuterOperation();
                if (pOCV->opErreur)
                {
                    pOCV->opErreur = 0;
                    return;
                }
                if (r.size() != 0)
                {
                    item = (InfoNoeud *)arbre->GetItemData(t);
                    FenetrePrincipale *f = item->Fenetre();
                    if (!f)
                    {
                        f = ((wxOpencvApp*)osgApp)->CreerFenetre(r, 0);
                        if (!f)
                            return;
                        ArboCalculParam p;
                        p.fen = f;
                        p.indFen = item->IndiceFenetre();
                        arbre->ExplorerArbre(arbre->GetRootItem(), p, &ArboCalcul::ReplacerIdParFenetre);
                        item->DefFenetre(f);
                    }
                    if (!f)
                        return;
                    std::vector<std::pair<ParametreOperation*, int>> pl = FindOperande(f->ImAcq());
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
                }

            }
        }
    }

}

void ArboCalcul::ReplacerIdParFenetre(wxTreeItemId &id, ArboCalculParam & p, bool quitterBranche)
{
    InfoNoeud *item = (InfoNoeud *)GetItemData(id);
    if (item)
    {
        wxTreeItemId idParent;
        idParent = item->getParent();
        if (item->Fenetre() || item->IndiceFenetre()>=0)
        {
            if (item->IndiceFenetre() == p.indFen)
            {
                int ind = -1;
                if (p.fen)
                    ind = ((wxOpencvApp *)osgApp)->RechercheFenetre(((FenetrePrincipale*)p.fen)->ImAcq());
                if (ind >= 0)
                {
                    item->DefFenetre(p.fen);
                    item->DefIndFenetre(ind);
                    item->DefTitle(p.fen->GetTitle());
                }
                wxString n(p.fen->GetTitle());
                wxIconBundle iconBundle = p.fen->GetIcons();
                wxIcon icon = iconBundle.GetIcon(wxSize(32, 32), wxIconBundle::FALLBACK_NEAREST_LARGER);

                listeImage.get()->Add(icon);
                listeImage.get()->Add(icon);
                SetItemText(id, p.fen->GetTitle());
                SetItemImage(id, listeImage.get()->GetImageCount() - 2);
             }
        }
        else if (item->Operation())
        {
            ParametreOperation *pOCV = item->Operation();
            for (int i = 0; i < pOCV->nbOperande; i++)
            {
                if (pOCV->indOpFenetre[i] == p.indFen)
                {
                    pOCV->indOpFenetre[i] = ((FenetrePrincipale*)p.fen)->IdFenetre();
                    if (i < pOCV->op.size())
                        pOCV->op[i] = ((FenetrePrincipale*)p.fen)->ImAcq();
                    else
                        pOCV->op.push_back(((FenetrePrincipale*)p.fen)->ImAcq());
                }
            }
        }
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

