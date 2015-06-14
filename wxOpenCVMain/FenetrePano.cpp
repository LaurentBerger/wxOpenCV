#include "ParametreOperation.h"
#include "FenetrePano.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;

void FenetrePrincipale::ParamPano(wxCommandEvent& event)
{

    if (imAcq->ParamPano() == NULL)
        return;
    if (!fenPano==NULL)
        return;
    fenPano = new FenetrePano(this, origineImage.nomOperation, wxPoint(530, 0), wxSize(430, 570), this->osgApp);
    fenPano->DefFenMere(this);
    fenPano->DefOSGApp(this->osgApp);
    fenPano->Show(true);
    fenPano->Refresh(true);
//fenAlgo->SendSizeEvent   ();
}




#define ID_SAUVER_SEQ_CONFIG 1000
#define ID_NOM_SEQUENCE 1001
#define ID_SAUVER_SEQ_XML 1002

FenetrePano::FenetrePano(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOsgApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX  | wxCAPTION )
{

/*
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

    wxNotebook *notebook = new wxNotebook( this, wxID_ANY );
    topsizer->Add( notebook, 1, wxGROW );

    wxButton *button = new wxButton( this, wxID_OK, wxT("OK") );
    topsizer->Add( button, 0, wxALIGN_RIGHT | wxALL, 10 );

    // First page: one big text ctrl
    wxTextCtrl *multi = new wxTextCtrl( notebook, wxID_ANY, wxT("TextCtrl."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    notebook->AddPage( multi, wxT("Page One") );

    // Second page: a text ctrl and a button
    wxPanel *panel = new wxPanel( notebook, wxID_ANY );
    notebook->AddPage( panel, wxT("Page Two") );


    // Tell dialog to use sizer
    SetSizerAndFit( topsizer );
*/
wxSize s(size);
tailleMax=wxSize(0,0);
wxPoint p(0,100);
//panneau=new wxPanel(this, wxID_ANY,  p, s, wxTAB_TRAVERSAL  | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE   );

int hMax=0,lMax=0;
ParametreOperation *pOCV=frame->ParamOCV();
fenMere= frame;
osgApp=osg;

classeur = new wxNotebook(this, wxID_ANY);
classeur->InsertPage(0, CreerOngletEtape(classeur, 0), _("Features"), true);
classeur->InsertPage(1, CreerOngletEtape(classeur, 1), _("Match"), true);
classeur->InsertPage(2, CreerOngletEtape(classeur, 2), _("Best Match"), true);
classeur->InsertPage(3, CreerOngletEtape(classeur, 3), _("Homography"), true);
wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
topsizer->Add( classeur, 1, wxGROW|wxEXPAND,10  );
wxBoxSizer *partieBasse=new wxBoxSizer(wxHORIZONTAL);
panneau=new wxPanel(this, wxID_ANY);
wxColour fond(*wxLIGHT_GREY);
fond.Set(fond.Red(),255,fond.Blue());
panneau->SetBackgroundColour(fond);
topsizer->Add( panneau, 1, wxGROW|wxEXPAND,10  );

wxButton *button1 = new wxButton(panneau, ID_SAUVER_SEQ_CONFIG, _("Save all step as Macro in cfg "));
wxButton *button2 = new wxButton(panneau, ID_SAUVER_SEQ_XML, _("Save all step as Macro in xml"));
wxStaticText *st = new wxStaticText(panneau, -1, _("         Name "));
wxString nomMacro;
nomMacro.Printf("Macro %d",osg->NumSeqOpe());
wxTextCtrl *caseNomMacro = new wxTextCtrl( panneau, ID_NOM_SEQUENCE, nomMacro);
partieBasse->Add(button1, 0, wxALIGN_CENTER_VERTICAL | wxALL);
partieBasse->Add(button2, 0, wxALIGN_CENTER_VERTICAL | wxALL);
partieBasse->Add(st, 0, wxALIGN_CENTER_VERTICAL | wxALL);
partieBasse->Add(caseNomMacro,0, wxALIGN_CENTER_VERTICAL|wxALL);
panneau->SetSizer(partieBasse);
SetSizerAndFit( topsizer );
Bind(wxEVT_SPINCTRL, &FenetrePano::OnSpinEntier, this);
Bind(wxEVT_TEXT_ENTER, &FenetrePano::OnTextValider, this);
Show(true);
}

void FenetrePano::ComboBox(wxCommandEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp || !fenMere)
	return;

}



wxWindow *FenetrePano::CreerOngletEtape(wxNotebook *classeur, int indOp)
{
    if (fenMere == NULL || fenMere->ImAcq() == NULL || fenMere->ImAcq()->ParamPano() == NULL)
        return NULL;
    Panoramique *pano = fenMere->ImAcq()->ParamPano();
    int ligne = 10;
    int col = 10;
    vector<int >colonne;
    wxWindow *page = new wxWindow(classeur,-1);
    wxSpinCtrl *sb = NULL;
    wxString s;
    switch(indOp)
    {
        case 0:
            for (int i = 0; i < pano->features.size(); i++)
            {
                wxString s;
                int nb = pano->features[i].keypoints.size();
                s.Printf("Image %d : %d features", pano->indOpFenetre[i],nb );

                wxStaticText *ws = new wxStaticText(page, -1, s, wxPoint(10, ligne));
                ligne += 20;
            }
            break;
        case 1:
            colonne.resize(pano->features.size());
            col = 100;
            for (int i = 0; i < pano->features.size(); i++)
            {
                colonne[i] = col;
                wxString s;
                s.Printf("Image %d ", i);
                wxStaticText *ws = new wxStaticText(page, -1, s, wxPoint(colonne[i], ligne));
                col += ws->GetSize().x + 10;
            }
            ligne += 20;
            for (int i = 0; i < pano->appariement.size(); i++)
            {

                wxString s;
                wxStaticText *ws;
                if (i%pano->op.size() == 0)
                {
                    s.Printf("Image %d ", int(i/pano->op.size()));
                    ws = new wxStaticText(page, -1, s, wxPoint(10, ligne));
                }
                int nb = pano->appariement[i].num_inliers;
                s.Printf("%d ",  nb);

                ws = new wxStaticText(page, -1, s, wxPoint(colonne[i%pano->features.size()], ligne));
                if ((i + 1) % pano->features.size() == 0)
                {
                    ligne += 20;
                    col = 10;
                }
            }
            break;
        case 2:
            for (int i = 0; i < pano->bijection.size(); i++)
            {

                wxString s;
                s.Printf("Image %d is in panorama", i, pano->indOpFenetre[i]);
                wxStaticText *ws = new wxStaticText(page, -1, s, wxPoint(col, ligne));
                ligne += 20;
                col = 10;
            }
            break;
        case 3:
            if (pano->cameras.size() == 0)
                return page;

            new wxStaticText(page, -1, _("Camera "), wxPoint(col, ligne));
            sb = new wxSpinCtrl(page, 1000, _("Camera "), wxPoint(100, ligne), wxSize(50, 50));
            sb->SetRange(0, pano->cameras.size()-1);
            ligne = 60;
            new wxStaticText(page, -1, _("focal "), wxPoint(10, ligne),wxSize(100,20));
            new wxStaticText(page, -1, _("ppx "), wxPoint(10, ligne + 20), wxSize(100, 20));
            new wxStaticText(page, -1, _("ppy "), wxPoint(10, ligne + 40), wxSize(100, 20));
            new wxStaticText(page, -1, _("aspect "), wxPoint(10, ligne + 60), wxSize(100, 20));
            s.Printf("%lf", pano->cameras[0].focal);
            new wxTextCtrl(page, 1100, s, wxPoint(110, ligne), wxSize(60, 20), wxTE_PROCESS_ENTER);
            s.Printf("%lf", pano->cameras[0].ppx);
            new wxTextCtrl(page, 1101, s, wxPoint(110, ligne + 20), wxSize(60, 20), wxTE_PROCESS_ENTER);
            s.Printf("%lf", pano->cameras[0].ppy);
            new wxTextCtrl(page, 1102, s, wxPoint(110, ligne + 40), wxSize(60, 20), wxTE_PROCESS_ENTER);
            s.Printf("%lf", pano->cameras[0].aspect);
            new wxTextCtrl(page, 1103, s, wxPoint(110, ligne + 60), wxSize(60, 20), wxTE_PROCESS_ENTER);
            for (int i = 0; i < 3; i++)
            {
                int nbc = pano->cameras[0].R.channels();
                int ty = pano->cameras[0].R.type();
                int de = pano->cameras[0].R.depth();
                col = 200;
                for (int j = 0; j < 3; j++)
                {

                    wxString s;
                    s.Printf("%lf",  pano->cameras[0].R.at<float>(j,i));
                    wxTextCtrl *ws = new wxTextCtrl(page, 2000 + i * 3 + j, s, wxPoint(col, ligne), wxSize(60, 20), wxTE_PROCESS_ENTER);
                    col += 70;
                }
                ligne += 20;
            }
            ligne = 60;
            for (int i = 0; i < 3; i++)
            {
                col = 400;
                wxString s;
                if (pano->cameras[0].t.depth() == CV_32F)
                    s.Printf("%f", pano->cameras[0].t.at<float>(i, 0));
                else
                    s.Printf("%lf", pano->cameras[0].t.at<double>(i, 0));
                wxTextCtrl *ws = new wxTextCtrl(page, 2100 + i, s, wxPoint(col, ligne), wxSize(60, 20), wxTE_PROCESS_ENTER);
                ligne += 20;
            }
            break;

        default:
            break;
    }
    if (ligne > tailleMax.y)
        tailleMax.y = ligne;
    if (col > tailleMax.x)
        tailleMax.y = col;
    return page;
}


FenetrePano::~FenetrePano()
{
    fenMere->RAZFenParam();

}

void FenetrePano::OnActivate(wxActivateEvent& event)
{
    if (!event.GetActive())
        return;
    int i = classeur->GetSelection();
}

void FenetrePano::OnPaint(wxPaintEvent& event)
{
}

void FenetrePano::OnSpinEntier(wxSpinEvent &w)
{
    if (fenMere == NULL || fenMere->ImAcq() == NULL || fenMere->ImAcq()->ParamPano() == NULL)
        return ;
    Panoramique *pano = fenMere->ImAcq()->ParamPano();
    if (w.GetId() == 1000) // Bouton indice de caméra
    {
        wxTextCtrl *ws;
        wxString s;
        ws=(wxTextCtrl*)wxWindow::FindWindowById(1100, this); 
        s.Printf("%lf", pano->cameras[w.GetValue()].focal);
        ws->SetValue(s);
        ws = (wxTextCtrl*)wxWindow::FindWindowById(1101, this);
        s.Printf("%lf", pano->cameras[w.GetValue()].ppx);
        ws->SetValue(s);
        ws = (wxTextCtrl*)wxWindow::FindWindowById(1102, this);
        s.Printf("%lf", pano->cameras[w.GetValue()].ppy);
        ws->SetValue(s);
        ws = (wxTextCtrl*)wxWindow::FindWindowById(1103, this);
        s.Printf("%lf", pano->cameras[w.GetValue()].aspect);
        ws->SetValue(s);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                wxString s;
                s.Printf("%f", pano->cameras[w.GetValue()].R.at<float>(j, i));
                ws = (wxTextCtrl*)wxWindow::FindWindowById( 2000 + i * 3 + j,this);
                ws->SetValue(s);
                
            }
        }
        for (int i = 0; i < 3; i++)
        {
            wxString s;
            if (pano->cameras[w.GetValue()].t.depth() == CV_32F)
                s.Printf("%f", pano->cameras[w.GetValue()].t.at<float>(i, 0));
            else
                s.Printf("%lf", pano->cameras[w.GetValue()].t.at<double>(i, 0));
            ws = (wxTextCtrl*)wxWindow::FindWindowById(2100 + i, this);
            ws->SetValue(s);
        }

    }
}

void FenetrePano::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
}

void  FenetrePano::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetrePano::OnSpinMoins(wxSpinEvent& w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
}

void FenetrePano::OnTextValider(wxCommandEvent &w)
{
    if (fenMere == NULL || fenMere->ImAcq() == NULL || fenMere->ImAcq()->ParamPano() == NULL)
        return;
    Panoramique *pano = fenMere->ImAcq()->ParamPano();
    wxSpinCtrl* ws = (wxSpinCtrl*)wxWindow::FindWindowById(1000, this);
    if (ws == NULL)
        return;
    int i = ws->GetValue();
    wxString s;
    double val;
    s = w.GetString();
    s.ToDouble(&val);
    switch (w.GetId())
    {
    case 1100:
        pano->cameras[i].focal = val;
        break;
    case 1101:
        pano->cameras[i].ppx = val;
        break;
    case 1102:
        pano->cameras[i].ppy = val;
        break;
    case 1103:
        pano->cameras[i].aspect = val;
        break;
    case 2000:
        pano->cameras[i].R.at<float>(0, 0) = (float)val;
        break;
    case 2001:
        pano->cameras[i].R.at<float>(0, 1) = (float)val;
        break;
    case 2002:
        pano->cameras[i].R.at<float>(0, 2) = (float)val;
        break;
    case 2003:
        pano->cameras[i].R.at<float>(1, 0) = (float)val;
        break;
    case 2004:
        pano->cameras[i].R.at<float>(1,1) = (float)val;
        break;
    case 2005:
        pano->cameras[i].R.at<float>(1,2) = (float)val;
        break;
    case 2006:
        pano->cameras[i].R.at<float>(2,0) = (float)val;
        break;
    case 2007:
        pano->cameras[i].R.at<float>(2,1) = (float)val;
        break;
    case 2008:
        pano->cameras[i].R.at<float>(2, 2) = (float)val;
        break;
    case 2100:
        if (pano->cameras[i].t.depth()==CV_32F)
            pano->cameras[i].t.at<float>(0, 0) = (float)val;
        else
            pano->cameras[i].t.at<double>(0, 0) = (float)val;
        break;
    case 2101:
        if (pano->cameras[i].t.depth() == CV_32F)
            pano->cameras[i].t.at<float>(1, 0) = (float)val;
        else
            pano->cameras[i].t.at<double>(1, 0) = (float)val;
        break;
    case 2102:
        if (pano->cameras[i].t.depth() == CV_32F)
            pano->cameras[i].t.at<float>(2, 0) = (float)val;
        else
            pano->cameras[i].t.at<double>(2, 0) = (float)val;
        break;
    }
}
void FenetrePano::OnKeyDown(wxKeyEvent &)
{
}

void FenetrePano::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenParam();
wxFrame::OnCloseWindow(event);

}

