#include "FenetreAlgo.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;

void FenetrePrincipale::ParamAlgo(wxCommandEvent& event)
{
if (!fenAlgo==NULL)
	return;
fenAlgo=new FenetreAlgo(this,origineImage.nomOperation,wxPoint(530,0), wxSize(430,570),this->osgApp);
fenAlgo->DefFenMere(this);
fenAlgo->DefOSGApp(this->osgApp);
fenAlgo->Show(true);
fenAlgo->Refresh(true);
//fenAlgo->SendSizeEvent   ();
}




#define ID_SAUVER_SEQ 1000
#define ID_NOM_SEQUENCE 1001
 
FenetreAlgo::FenetreAlgo(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
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
FenetrePrincipale *f=fenMere;
nbEtape=0;
nbParamMax=0;
while(f && f->OrigineImage()->indOp1Fenetre>=0)
	{
	int id=f->OrigineImage()->indOp1Fenetre;
	if (id>=0)
		{
		int nbParam=f->OrigineImage()->intParam.size();
		nbParam+=f->OrigineImage()->doubleParam.size();
		nbParam+=2*f->OrigineImage()->pointParam.size();
		nbParam+=2*f->OrigineImage()->sizeParam.size();
		if (nbParamMax<nbParam)
			nbParamMax=nbParam;
		nbEtape++;
		f=((wxOsgApp *)osgApp)->Fenetre(id);
		}
	else 
		f=NULL;
	}
if (fenMere->ImAcq()->HoughLigne())
	nbEtape++;
if (fenMere->ImAcq()->HoughLigneProba())
	nbEtape++;
if (fenMere->ImAcq()->HoughCercle())
	nbEtape++;
if (fenMere->ImAcq()->BonCoin())
	nbEtape++;
if (fenMere->ImAcq()->ParamOCVLucasKanade())
	nbEtape++;
if (fenMere->ImAcq()->ParamOCVGunnarFarneback())
	nbEtape++;
if (fenMere->ImAcq()->ParamOCVPhaseCorrelate())
	nbEtape++;
nbParamMax = 2 * (nbParamMax + 2);
f=fenMere;
int nb=nbEtape-1;
listeOp.resize(nbEtape);
if (fenMere->ImAcq()->HoughLigne())
	{
	listeOp[nb]=std::pair< ParametreOperation*,int>(fenMere->ImAcq()->ParamOCVHoughLigne(),fenMere->IdFenetre());
	wxWindow *w=CreerOngletEtape(classeur,nb);
	listeOnglet[w]=std::pair<wxString,int>(fenMere->ImAcq()->ParamOCVHoughLigne()->nomOperation,nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s",nom,nb,fenMere->ImAcq()->ParamOCVHoughLigne()->nomOperation);
	classeur->InsertPage(0,w,nom,nbEtape==1);
	nb--;
	}
if (fenMere->ImAcq()->HoughLigneProba())
	{
	listeOp[nb]=std::pair< ParametreOperation*,int>(fenMere->ImAcq()->ParamOCVHoughLigneProba(),fenMere->IdFenetre());
	wxWindow *w=CreerOngletEtape(classeur,nb);
	listeOnglet[w]=std::pair<wxString,int>(fenMere->ImAcq()->ParamOCVHoughLigneProba()->nomOperation,nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s",nom,nb,fenMere->ImAcq()->ParamOCVHoughLigneProba()->nomOperation);
	classeur->InsertPage(0,w,nom,nbEtape==1);
	nb--;
	}
if (fenMere->ImAcq()->HoughCercle())
	{
	listeOp[nb]=std::pair< ParametreOperation*,int>(fenMere->ImAcq()->ParamOCVHoughCercle(),fenMere->IdFenetre());
	wxWindow *w=CreerOngletEtape(classeur,nb);
	listeOnglet[w]=std::pair<wxString,int>(fenMere->ImAcq()->ParamOCVHoughCercle()->nomOperation,nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s",nom,nb,fenMere->ImAcq()->ParamOCVHoughCercle()->nomOperation);
	classeur->InsertPage(0,w,nom,nbEtape==1);
	nb--;
	}
if (fenMere->ImAcq()->BonCoin() && fenMere->ImAcq()->ParamOCVBonCoin())
	{
	listeOp[nb]=std::pair< ParametreOperation*,int>(fenMere->ImAcq()->ParamOCVBonCoin(),fenMere->IdFenetre());
	wxWindow *w=CreerOngletEtape(classeur,nb);
	listeOnglet[w]=std::pair<wxString,int>(fenMere->ImAcq()->ParamOCVBonCoin()->nomOperation,nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s",nom,nb,fenMere->ImAcq()->ParamOCVBonCoin()->nomOperation);
	classeur->InsertPage(0,w,nom,nbEtape==1);
	nb--;
	}
if (fenMere->ImAcq()->ParamOCVLucasKanade())
	{
	listeOp[nb]=std::pair< ParametreOperation*,int>(fenMere->ImAcq()->ParamOCVLucasKanade(),fenMere->IdFenetre());
	wxWindow *w=CreerOngletEtape(classeur,nb);
	listeOnglet[w]=std::pair<wxString,int>(fenMere->ImAcq()->ParamOCVLucasKanade()->nomOperation,nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s",nom,nb,fenMere->ImAcq()->ParamOCVLucasKanade()->nomOperation);
	classeur->InsertPage(0,w,nom,nbEtape==1);
	nb--;
	}
if (fenMere->ImAcq()->ParamOCVGunnarFarneback())
{
	listeOp[nb] = std::pair< ParametreOperation*, int>(fenMere->ImAcq()->ParamOCVGunnarFarneback(), fenMere->IdFenetre());
	wxWindow *w = CreerOngletEtape(classeur, nb);
	listeOnglet[w] = std::pair<wxString, int>(fenMere->ImAcq()->ParamOCVGunnarFarneback()->nomOperation, nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s", nom, nb, fenMere->ImAcq()->ParamOCVGunnarFarneback()->nomOperation);
	classeur->InsertPage(0, w, nom, nbEtape == 1);
	nb--;
}
if (fenMere->ImAcq()->ParamOCVPhaseCorrelate())
{
	listeOp[nb] = std::pair< ParametreOperation*, int>(fenMere->ImAcq()->ParamOCVPhaseCorrelate(), fenMere->IdFenetre());
	wxWindow *w = CreerOngletEtape(classeur, nb);
	listeOnglet[w] = std::pair<wxString, int>(fenMere->ImAcq()->ParamOCVPhaseCorrelate()->nomOperation, nb);
	wxString nom(_("Step"));
	nom.Printf("%s %d : %s", nom, nb, fenMere->ImAcq()->ParamOCVPhaseCorrelate()->nomOperation);
	classeur->InsertPage(0, w, nom, nbEtape == 1);
	nb--;
}
while (f && f->OrigineImage()->indOp1Fenetre >= 0)
	{
	if (f->OrigineImage())
		{
		listeOp[nb]=std::pair< ParametreOperation*,int>(f->OrigineImage(),f->IdFenetre()) ;
		wxWindow *w=CreerOngletEtape(classeur,nb);
		listeOnglet[w]=std::pair<wxString,int>(f->OrigineImage()->nomOperation,nb);
		wxString nom(_("Step"));
		nom.Printf("%s %d : %s",nom,nb,f->OrigineImage()->nomOperation);
		classeur->InsertPage(0,w,nom,nbEtape==1);
		int id=f->OrigineImage()->indOp1Fenetre;
		if (id>=0)
			f=((wxOsgApp *)osgApp)->Fenetre(id);
		else 
			f=NULL;
		nb--;
		}
	else 
		f=NULL;
	}
wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
topsizer->Add( classeur, 1, wxGROW|wxEXPAND,10  );
wxBoxSizer *partieBasse=new wxBoxSizer(wxHORIZONTAL);
panneau=new wxPanel(this, wxID_ANY);
wxColour fond(*wxLIGHT_GREY);
fond.Set(fond.Red(),255,fond.Blue());
panneau->SetBackgroundColour(fond);
topsizer->Add( panneau, 1, wxGROW|wxEXPAND,10  );

wxButton *button = new wxButton( panneau, ID_SAUVER_SEQ, _("Save all step as Macro")  );
wxStaticText *st = new wxStaticText( panneau, -1, _("         Name ")  );
wxString nomMacro;
nomMacro.Printf("Macro %d",osg->NumSeqOpe());
wxTextCtrl *caseNomMacro = new wxTextCtrl( panneau, ID_NOM_SEQUENCE, nomMacro);
partieBasse->Add(button,0, wxALIGN_CENTER_VERTICAL|wxALL);
partieBasse->Add(st,0,wxALIGN_CENTER_VERTICAL| wxALL);
partieBasse->Add(caseNomMacro,0, wxALIGN_CENTER_VERTICAL|wxALL);
panneau->SetSizer(partieBasse);
Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FenetreAlgo::SauverSequence,this,ID_SAUVER_SEQ);
Bind(wxEVT_SPINCTRLDOUBLE,&FenetreAlgo::OnSpinReel,this);
Bind(wxEVT_COMMAND_COMBOBOX_SELECTED,&FenetreAlgo::ComboBox,this);
SetSizerAndFit( topsizer );
Show(true);
}


void FenetreAlgo::ComboBox(wxCommandEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp || !fenMere)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->doubleParam.find(nom)!=pOCV->doubleParam.end())
	{
	if (pOCV->doubleParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	pOCV->doubleParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{

	if (ParametreOperation::listeParam.find(nom)!=ParametreOperation::listeParam.end())
		{
		int nb=	((wxComboBox*)(w.GetEventObject()))->GetCurrentSelection ();
		int i=0;
		std::map <string,int  >::iterator iter = pOCV->listeParam[nom].begin(); 
		for (; iter !=pOCV->listeParam[nom].end() && i!=nb; ++iter,++i);
		if (i==nb)
			pOCV->intParam[nom].valeur=iter->second;
		}
	else 
		{
		if (pOCV->intParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->intParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId())%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.width=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.height=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
ExecuterOperation(ind);

}




void FenetreAlgo::SauverSequence(wxCommandEvent &evt)
{
if (osgApp==NULL || fenMere==NULL)
	return;
FenetrePrincipale *f=fenMere;
int nb=nbEtape-1;
std::map <int,std::vector <ParametreOperation > >  *t=((wxOsgApp *)osgApp)->TabSeqOperation();
(*t)[((wxOsgApp *)osgApp)->NumSeqOpe()].resize(nbEtape);

if (fenMere->ImAcq()->HoughLigne())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;

	nb--;
	}
if (fenMere->ImAcq()->HoughLigneProba())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;
	nb--;
	}
if (fenMere->ImAcq()->HoughCercle())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;
	nb--;
	}
if (fenMere->ImAcq()->BonCoin())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;
	nb--;
	}
if (fenMere->ImAcq()->ParamOCVLucasKanade())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;
	nb--;
	}
if (fenMere->ImAcq()->ParamOCVGunnarFarneback())
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
	((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
	(*t)[listeOp[nb].first->idOperation][nb]=p;
	nb--;
	}


while(f && f->OrigineImage()->indOp1Fenetre>=0)
	{
	if (f->OrigineImage())
		{
		listeOp[nb]=std::pair< ParametreOperation*,int>(f->OrigineImage(),f->IdFenetre()) ;
		listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
		if (listeOp[nb].first->indEtape==-1)
			listeOp[nb].first->indEtape=nb;
		wxTextCtrl *w=(wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE,panneau);
		listeOp[nb].first->nomSequence=w->GetValue();
		ParametreOperation p;
		p=*(listeOp[nb].first);
		((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
		int id=f->OrigineImage()->indOp1Fenetre;
		if (id>=0)
			f=((wxOsgApp *)osgApp)->Fenetre(id);
		else 
			f=NULL;
		(*t)[listeOp[nb].first->idOperation][nb]=p;
		nb--;
		}
	else 
		f=NULL;
	}

}



wxWindow *FenetreAlgo::CreerOngletEtape(wxNotebook *classeur,int indOp)
{
wxWindow *page = new wxWindow(classeur,-1);
ParametreOperation *pOCV=listeOp[indOp].first;
int nbParam=1;
int ligne=50;
int indOriCtrl=1+indOp*nbParamMax;
new wxHyperlinkCtrl (page,indOriCtrl," OpenCV Documentation",pOCV->lienHtml,wxPoint(10,10),wxSize(400,20));
new wxHyperlinkCtrl(page,indOriCtrl+1,"PDF Documentation",pOCV->refPDF,wxPoint(10,30),wxSize(400,20));
if (tailleMax.x<410)
	tailleMax.x= 410;
if (tailleMax.y<ligne)
	tailleMax.y= ligne;
std::map<std::string,DomaineParametreOp<cv::Size> >::iterator its;
for (its=pOCV->sizeParam.begin();its!=pOCV->sizeParam.end();its++)
	{
	wxString nombre;
	nombre.Printf("%d",its->second.valeur.width);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first+" X",p, s);
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	wxSpinCtrlDouble *spw=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	spw->SetRange(0,256); 
	spw->SetIncrement((double)its->second.pas.width); 
	spw->SetRange(0,256); 
	if(!its->second.res)
		spw->Disable();
	p += wxPoint(s.GetX(),0);
	nbParam++;
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first+" y",p, s);
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sph->SetRange(0,256); 
	wxSpinCtrlDouble *sph=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	sph->SetRange(0, 256);
	sph->SetIncrement((double)its->second.pas.height); 
	if (!its->second.res)
		sph->Disable();
		
	nbParam++;
	ligne+=20;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	}
std::map<std::string,DomaineParametreOp<int> >::iterator iti;
for (iti=pOCV->intParam.begin();iti!=pOCV->intParam.end();iti++)
	{
	wxString nombre;
	nombre.Printf("%d",iti->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,iti->first,p, s);
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *sp=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sp->SetRange(iti->second.mini,iti->second.maxi); 
	if (ParametreOperation::listeParam.find(iti->first)!=ParametreOperation::listeParam.end())
		{
		int nbChaine=pOCV->listeParam[iti->first].size();
		wxString *choix=new wxString[nbChaine],choixDefaut;
		int i=0;
		for (std::map <string,int  >::iterator iter = pOCV->listeParam[iti->first].begin() ; iter !=pOCV->listeParam[iti->first].end(); ++iter,++i)
			{
			choix[i]=iter->first;
			if (iter->second==iti->second.valeur)
				choixDefaut=iter->first;
			}
		wxComboBox *choixOp=new wxComboBox(page,indOriCtrl+2*nbParam+1,choixDefaut,p,wxSize(250,-1),nbChaine,choix);
		if (!iti->second.res)
			choixOp->Disable();

		}
	else
		{
		wxSpinCtrlDouble *sp=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
		sp->SetRange(iti->second.mini,iti->second.maxi); 
		sp->SetIncrement(iti->second.pas); 
		if (!iti->second.res)
			sp->Disable();
	}
	nbParam++;
	ligne+=20;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	}
std::map<std::string,DomaineParametreOp<double> >::iterator itd;
for (itd=pOCV->doubleParam.begin();itd!=pOCV->doubleParam.end();itd++)
	{
	wxString nombre;
	nombre.Printf("%f",itd->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,itd->first,p, s);
	p += wxPoint(s.GetX(),0);
	wxSpinCtrlDouble *sp=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	sp->SetRange(itd->second.mini, itd->second.maxi);
	sp->SetIncrement(itd->second.pas); 
	if (!itd->second.res)
		sp->Disable();
	nbParam++;
	ligne+=20;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	}
//page->SetClientSize(tailleMax+wxSize(20,40));
return page;
}


FenetreAlgo::~FenetreAlgo()
{
fenMere->RAZFenAlgo();

}

void FenetreAlgo::OnActivate(wxActivateEvent& event)
{
}

void FenetreAlgo::OnPaint(wxPaintEvent& event)
{
}

void FenetreAlgo::OnSpinEntier(wxSpinEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	int ds=pOCV->intParam[nom].valeur-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
	if (ds==0)
		return;
	int valeur=pOCV->intParam[nom].maxi+1;
	if(ds<0 )
		valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
	if(ds>0 )
		valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
	if (	valeur >=pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi && 
		(valeur%pOCV->intParam[nom].pas)==(pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
		{
		pOCV->intParam[nom].valeur=valeur;
		((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
			ExecuterOperation(ind);
		}
	else
		wxMessageBox("Selected value is out of range or forbidden");
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId()-1)%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.width-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.width+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
		if (	valeur >=pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width && 
			(valeur%pOCV->sizeParam[nom].pas.width)==(pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
			{
			pOCV->sizeParam[nom].valeur.width=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
			ExecuterOperation(ind);
			}
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.height-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.height+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
		if (	valeur >=pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height && 
			(valeur%pOCV->sizeParam[nom].pas.height)==(pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
			{
			pOCV->sizeParam[nom].valeur.height=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
			ExecuterOperation(ind);

			}
		}
	}

}

void FenetreAlgo::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->doubleParam.find(nom)!=pOCV->doubleParam.end())
	{
	if (pOCV->doubleParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	pOCV->doubleParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	if (pOCV->intParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	pOCV->intParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId())%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.width=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.height=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
ExecuterOperation(ind);
}

void  FenetreAlgo::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreAlgo::OnSpinMoins(wxSpinEvent& w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	int ds=pOCV->intParam[nom].valeur-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
	if (ds==0)
		return;
	int valeur=pOCV->intParam[nom].maxi+1;
	if(ds<0 )
		valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
	if(ds>0 )
		valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
	if (	valeur >=pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi && 
		(valeur%pOCV->intParam[nom].pas)==(pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
		{
		pOCV->intParam[nom].valeur=valeur;
		((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
			ExecuterOperation(ind);
		}
	else
		wxMessageBox("Selected value is out of range or forbidden");
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId()-1)%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.width-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.width+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
		if (	valeur >=pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width && 
			(valeur%pOCV->sizeParam[nom].pas.width)==(pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
			{
			pOCV->sizeParam[nom].valeur.width=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
			ExecuterOperation(ind);
			}
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.height-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.height+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
		if (	valeur >=pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height && 
			(valeur%pOCV->sizeParam[nom].pas.height)==(pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
			{
			pOCV->sizeParam[nom].valeur.height=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
			ExecuterOperation(ind);

			}
		}
	}
}

void FenetreAlgo::OnTextValider(wxCommandEvent &w)
{
}
void FenetreAlgo::OnKeyDown(wxKeyEvent &)
{
}

void FenetreAlgo::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenAlgo();
wxFrame::OnCloseWindow(event);

}

void FenetreAlgo::ExecuterOperation(int indEtape)
{
if (!osgApp)
	return;
wxOsgApp	*app=(wxOsgApp *)osgApp;
ImageInfoCV **im=NULL;
for (int i=indEtape;i<nbEtape;i++)
	{
	int indFen1=app->RechercheFenetre(listeOp[i].first->op1);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
	}
for (int i=indEtape;i<nbEtape;i++)
	{
	ParametreOperation *pOCV=listeOp[i].first;
	wxString  nomOperation(listeOp[i].first->nomOperation);
	app->DefOperateurImage(nomOperation);
	int indFen1=app->RechercheFenetre(listeOp[i].first->op1);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
//	if (im==NULL)
		app->DefOperande1(listeOp[i].first->op1,indFen1);
	//else
//		app->DefOperande1(im[0]);
	int indFen2=app->RechercheFenetre(listeOp[i].first->op2);
	app->DefOperande2(listeOp[i].first->op2,indFen2);
	im=app->ExecuterOperation(pOCV);
	if (im!=NULL)
		{
		FenetrePrincipale *f;

		f =app->Fenetre(listeOp[i].second);
		
		if (f->ImAcq()!=im[0])
			f->AssosierImage(im[0]);
		if (i<nbEtape-1)
			{
			listeOp[i+1].first->op1 =im[0];
			}

		f->NouvelleImage();
		f->MAJNouvelleImage();
		if (f->ImgStat())
			f->ImgStat()->Plot(true);
		f->DefHistorique();
		}
	}

}
