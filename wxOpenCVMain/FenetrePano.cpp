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
classeur->InsertPage(2, CreerOngletEtape(classeur, 0), _("Best Match"), true);
classeur->InsertPage(3, CreerOngletEtape(classeur, 0), _("Homography"), true);
classeur->InsertPage(4, CreerOngletEtape(classeur, 0), _("Wrap"), true);
classeur->InsertPage(5, CreerOngletEtape(classeur, 0), _("Correct Expo"), true);
classeur->InsertPage(6, CreerOngletEtape(classeur, 0), _("Pano Compo"), true);
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
            for (int i = 0; i < pano->appariement.size(); i++)
            {

                wxString s;
                int nb = pano->appariement[i].num_inliers;
                s.Printf("Image  %d ", nb);
                wxStaticText *ws = new wxStaticText(page, -1, s, wxPoint(col, ligne));
                 nb = pano->appariement[i].num_inliers;
                s.Printf("%d ",  nb);

                ws = new wxStaticText(page, -1, s, wxPoint(colonne[i], ligne));
                if ((i + 1) % pano->features.size() == 0)
                {
                    ligne += 20;
                    col = 10;
                }
            }
            break;
        case 2:
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
            for (int i = 0; i < pano->appariement.size(); i++)
            {

                wxString s;
                int nb = pano->appariement[i].num_inliers;
                s.Printf("Image %d ", nb);
                wxStaticText *ws = new wxStaticText(page, -1, s, wxPoint(col, ligne));
                nb = pano->appariement[i].num_inliers;
                s.Printf("%d ", nb);

                ws = new wxStaticText(page, -1, s, wxPoint(colonne[i], ligne));
                if ((i + 1) % pano->features.size() == 0)
                {
                    ligne += 20;
                    col = 10;
                }
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
}
void FenetrePano::OnKeyDown(wxKeyEvent &)
{
}

void FenetrePano::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenParam();
wxFrame::OnCloseWindow(event);

}

