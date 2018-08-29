#include "wx/wxprec.h"


#include "GrapheOperation.h"
#include "FenetreAlgo.h"
#include "GlisserForme.h"
#include "ImageStat.h"

#include <wx/hyperlink.h>
#include <algorithm>

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



void FenetreInfoOperation::AjouterEtape(int nb, ParametreOperation *pOCV, int idFenetre, wxTreeItemId &n)
{
    if (nb < 0)
        return;
    noeuds.insert(std::pair<ParametreOperation *, wxTreeItemId &>(pOCV, n));
    listeOp.push_back(std::pair< ParametreOperation*, int>(pOCV, idFenetre));
    wxWindow *w = CreerOngletEtape(classeur, nb);
    if (!w)
        return;
    listeOnglet[w] = std::pair<wxString, int>(pOCV->nomOperation, nb);
    wxString nom(_("Step"));
    nom.Printf("%s %d : %s", nom, nb, pOCV->nomOperation);
    classeur->InsertPage(0, w, nom, nb == 1);

}

wxWindow *FenetreInfoOperation::CreerOngletEtape(wxNotebook *classeur, int indOp)
{
    // nbParamMax  nombre d'article maximum par onglet
    wxWindow *page = new wxWindow(classeur, -1);
    if (indOp >= listeOp.size())
        return NULL;
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
    classeur->SetSelection(classeur->GetPageCount() - 1 - ind);
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
        ExecuterOperation(ind);

}

std::vector<std::pair<ParametreOperation*, int>> FenetreInfoOperation::FindOperande(ImageInfoCV *x)
{
    std::map<ParametreOperation *, wxTreeItemId  >::iterator w = noeuds.begin();
    std::vector<std::pair<ParametreOperation*, int>> p;
    for (auto n : noeuds)
    {
        for (int i = 0; i<n.first->nbOperande && i<n.first->op.size(); i++)
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

    std::map<ParametreOperation *, wxTreeItemId  >::iterator w = noeuds.find(pOCV);
    std::vector<ImageInfoCV*>r;
    if (w == noeuds.end())
        return;
    wxTreeItemId noeud ;
    noeud = arbre->GetRootItem();
    ArboCalculParam p;
    arbre->ExplorerArbre(noeud,p,&ArboCalcul::ExecuterNoeud);
    noeud = w->second;



    while (noeud.IsOk() && noeud != arbre->GetRootItem())
    {
        if (pOCV != NULL)
        {

            wxTreeItemId t = noeud;
            InfoNoeud *item = (InfoNoeud *)arbre->GetItemData(t);
            t = item->getParent();
            item = (InfoNoeud *)arbre->GetItemData(t);
            if (item)
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
                    FenetrePrincipale *f = item->Fenetre();
                    if (!f)
                    {
                        f = ((wxOpencvApp*)osgApp)->CreerFenetre(r,0);
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
