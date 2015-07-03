#include "ParametreOperation.h"
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




#define ID_SAUVER_SEQ_CONFIG 1000
#define ID_NOM_SEQUENCE 1001
#define ID_SAUVER_SEQ_XML 1002

FenetreAlgo::FenetreAlgo(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOsgApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX  | wxCAPTION )
{

wxSize s(size);
tailleMax=wxSize(0,0);
wxPoint p(0,100);

int hMax=0,lMax=0;
ParametreOperation *pOCV=frame->ParamOCV();
fenMere= frame;
osgApp=osg;

classeur = new wxNotebook(this, wxID_ANY);
FenetrePrincipale *f=fenMere;
nbEtape=0;
nbParamMax=0;
while(f && f->OrigineImage()->indOpFenetre.size()>0)
	{
    int id = f->OrigineImage()->indOpFenetre[0];
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
map<string,ParametreOperation>::iterator it;

for (it = fenMere->ImAcq()->ListeOpAttribut()->begin(); it != fenMere->ImAcq()->ListeOpAttribut()->end();it++)
	nbEtape++;
nbParamMax = 2 * (nbParamMax + 2);
f=fenMere;
int nb=nbEtape-1;
listeOp.resize(nbEtape);
for (it = fenMere->ImAcq()->ListeOpAttribut()->begin(); it != fenMere->ImAcq()->ListeOpAttribut()->end(); it++)
		{
		listeOp[nb]=std::pair< ParametreOperation*,int>(&it->second,fenMere->IdFenetre());
		wxWindow *w=CreerOngletEtape(classeur,nb);
		listeOnglet[w] = std::pair<wxString, int>(it->second.nomOperation, nb);
		wxString nom(_("Step"));
		nom.Printf("%s %d : %s", nom, it->second.indEtape, it->second.nomOperation);
		classeur->InsertPage(0,w,nom,nbEtape==1);
		nb--;
		}
while (f && f->OrigineImage()->indOpFenetre.size() > 0)
	{
	if (f->OrigineImage()->indOpFenetre.size() > 0)
		{
		listeOp[nb]=std::pair< ParametreOperation*,int>(f->OrigineImage(),f->IdFenetre()) ;
		wxWindow *w=CreerOngletEtape(classeur,nb);
		listeOnglet[w]=std::pair<wxString,int>(f->OrigineImage()->nomOperation,nb);
		wxString nom(_("Step"));
		nom.Printf("%s %d : %s",nom,nb,f->OrigineImage()->nomOperation);
		classeur->InsertPage(0,w,nom,nbEtape==1);
        int id = f->OrigineImage()->indOpFenetre[0];
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
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreAlgo::SauverSequence, this, ID_SAUVER_SEQ_CONFIG);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreAlgo::SauverSequence, this, ID_SAUVER_SEQ_XML);
Bind(wxEVT_SPINCTRLDOUBLE, &FenetreAlgo::OnSpinReel, this);
Bind(wxEVT_COMMAND_COMBOBOX_SELECTED,&FenetreAlgo::ComboBox,this);
Bind(wxEVT_TEXT_ENTER, &FenetreAlgo::OnTextValider, this);
SetSizerAndFit( topsizer );
Show(true);
}


FenetreAlgo::FenetreAlgo(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
         const wxSize& size, ParametreOperation &pOCV, long style)
         : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCAPTION)
{

    wxSize s(size);
    tailleMax = wxSize(0, 0);
    wxPoint p(0, 100);

    int hMax = 0, lMax = 0;
    fenMere = frame;

    classeur = new wxNotebook(this, wxID_ANY);
    FenetrePrincipale *f = fenMere;
    nbEtape = 1;
    nbParamMax = pOCV.intParam.size() + pOCV.doubleParam.size() + pOCV.pointParam.size() + pOCV.sizeParam.size();
    nbParamMax = 2 * (nbParamMax + 2);
    f = fenMere;
    listeOp.push_back(make_pair(&pOCV,-1));
    wxWindow *w = CreerOngletEtape(classeur, 0);
    classeur->InsertPage(0, w, pOCV.nomOperation, nbEtape == 1);
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(classeur, 1, wxGROW | wxEXPAND, 10);
    wxBoxSizer *partieBasse = new wxBoxSizer(wxHORIZONTAL);
    panneau = new wxPanel(this, wxID_ANY);
    wxColour fond(*wxLIGHT_GREY);
    fond.Set(fond.Red(), 255, fond.Blue());
    panneau->SetBackgroundColour(fond);
    topsizer->Add(panneau, 1, wxGROW | wxEXPAND, 10);

    panneau->SetSizer(partieBasse);
    Bind(wxEVT_SPINCTRLDOUBLE, &FenetreAlgo::OnSpinReel, this);
    Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &FenetreAlgo::ComboBox, this);
    SetSizerAndFit(topsizer);
    Show(true);
}



void FenetreAlgo::ComboBox(wxCommandEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp )
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
if (fenMere)
	ExecuterOperation(ind);

}




void FenetreAlgo::SauverSequence(wxCommandEvent &evt)
{
if (osgApp == NULL || fenMere == NULL)
	return;
FenetrePrincipale *f=fenMere;
int nb=nbEtape-1;
std::map <int,std::vector <ParametreOperation > >  *t=((wxOsgApp *)osgApp)->TabSeqOperation();
(*t)[((wxOsgApp *)osgApp)->NumSeqOpe()].resize(nbEtape);
wxTextCtrl *w = (wxTextCtrl*)panneau->FindWindowById(ID_NOM_SEQUENCE, panneau);

map<string, ParametreOperation>::iterator it;
{
cv::FileStorage fsx;
wxString nomFic(w->GetValue());
nomFic.Replace(" ","_");
cv::FileStorage fsy;
if (evt.GetId() == ID_SAUVER_SEQ_XML)
    {
    fsx.open((string)nomFic+".xml", cv::FileStorage::WRITE);
    fsy.open((string)nomFic+".yml", cv::FileStorage::WRITE);
    }
for (it = fenMere->ImAcq()->ListeOpAttribut()->begin(); it != fenMere->ImAcq()->ListeOpAttribut()->end(); it++)
	{
	listeOp[nb].first->idOperation=((wxOsgApp *)osgApp)->NumSeqOpe();
	if (listeOp[nb].first->indEtape==-1)
		listeOp[nb].first->indEtape=nb;
	listeOp[nb].first->nomSequence=w->GetValue();
	ParametreOperation p;
	p=*(listeOp[nb].first);
    if (evt.GetId() == ID_SAUVER_SEQ_CONFIG)
	    ((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
    string nomEtape("Operation");
    nomEtape+=to_string(nbEtape);
    if (fsx.isOpened())
        p.write(fsx);
    if (fsy.isOpened())
        p.write(fsy);
    (*t)[listeOp[nb].first->idOperation][nb] = p;

	nb--;
	}

while(f && f->OrigineImage()->indOpFenetre.size()>0)
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
        if (evt.GetId() == ID_SAUVER_SEQ_CONFIG)
            ((wxOsgApp *)osgApp)->SauverOperationFichierConfig(p);
        if (fsx.isOpened())
            p.write(fsx);
        if (fsy.isOpened())
            p.write(fsy);
        int id = f->OrigineImage()->indOpFenetre[0];
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

}



wxWindow *FenetreAlgo::CreerOngletEtape(wxNotebook *classeur,int indOp)
{
// nbParamMax  nombre d'article maximum par onglet
wxWindow *page = new wxWindow(classeur,-1);
ParametreOperation *pOCV=listeOp[indOp].first;
int nbParam=1;
int ligne=50;
int indOriCtrl=1+indOp*nbParamMax;// Dépend de l'indice de l'opérateur pour éviter le recouvrement des onglets 
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
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first+" x",p, s);
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	wxSpinCtrlDouble *spw=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
    spw->SetDigits(5);
	if(its->second.res)
		spw->Disable();
	spw->SetRange(its->second.mini.width, its->second.maxi.width);
	spw->SetIncrement((double)its->second.pas.width); 
	p += wxPoint(s.GetX(),0);
	nbParam++;
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first+" y",p, s);
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sph->SetRange(0,256); 
	wxSpinCtrlDouble *sph=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
    sph->SetDigits(5);
	if (its->second.res)
		sph->Disable();
	sph->SetRange(its->second.mini.height, its->second.maxi.height);
	sph->SetIncrement((double)its->second.pas.height); 
		
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
		if (iti->second.res)
			choixOp->Disable();

		}
	else
		{
		wxSpinCtrlDouble *sp=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
		sp->SetRange(iti->second.mini,iti->second.maxi); 
		sp->SetIncrement(iti->second.pas); 
		if (iti->second.res)
			sp->Disable();
	}
	nbParam++;
	ligne+=20;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
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
int indCouleur=0;
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
	spw->SetRange(itp->second.mini.x, itp->second.maxi.y);
	spw->SetIncrement((double)itp->second.pas.x);
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
    if (fenMere && itp->second.mouseScan)
    {
        spinSouris.push_back(make_pair(spw,sph));
        paramSouris.push_back(&itp->second);
        ongletSouris.push_back(page);
        fenMere->Feuille()->AjouteForme(wxPoint(itp->second.valeur.x,itp->second.valeur.y),indCouleur++,0,this,spinSouris.size()-1,itp->first);
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


FenetreAlgo::~FenetreAlgo()
{
if (fenMere)
{
    fenMere->Feuille()->ClearShapes();
    fenMere->RAZFenAlgo();
}

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
double x = w.GetValue();
if (pOCV->doubleParam.find(nom) != pOCV->doubleParam.end())
	{
	if (pOCV->doubleParam[nom].valeur==x)
		return;
	pOCV->doubleParam[nom].valeur=x;
	}
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	if (pOCV->intParam[nom].valeur==(int)x)
		return;
	pOCV->intParam[nom].valeur=x;
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
if (pOCV->pointParam.find(nom.substr(0,nom.length()-2)) != pOCV->pointParam.end())
	{
	if (nom.substr(nom.length()-1, 1)=='x')
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
}
if (fenMere)
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

void FenetreAlgo::ExecuterOperation(int indOperation)
{
if (!osgApp)
	return;
wxOsgApp	*app=(wxOsgApp *)osgApp;
ImageInfoCV **im=NULL;
int indEtape = listeOp[indOperation].first->indEtape;
for (int i=indEtape;i<nbEtape;i++)
	{
	int indFen1=app->RechercheFenetre(listeOp[i].first->op[0]);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
	}
vector<ImageInfoCV*>r;
for (int ii=indEtape;ii<nbEtape;ii++)
	{
    ParametreOperation *pOCV=NULL;
    int i = 0;
    while(i<listeOp.size() && listeOp[i].first->indEtape!=ii)
        i++;
    pOCV = listeOp[i].first;
    wxString  nomOperation(listeOp[i].first->nomOperation);
	app->DefOperateurImage(nomOperation);
	int indFen1=app->RechercheFenetre(listeOp[i].first->op[0]);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
		app->DefOperande1(listeOp[i].first->op[0],indFen1);
	//else
//		app->DefOperande1(im[0]);
	int indFen2=-1;
    if (listeOp[i].first->op.size()>1)
        indFen2=app->RechercheFenetre(listeOp[i].first->op[1]);
	if(indFen2>=0)
        app->DefOperande2(listeOp[i].first->op[1],indFen2);
    else
        app->DefOperande2(NULL,indFen2);
	r=app->ExecuterOperation(pOCV);
    listeOp[i].first->indEtape = ii;
	if (r.size()!=0)
		{
		FenetrePrincipale *f;

		f =app->Fenetre(listeOp[i].second);
		
		if (f->ImAcq()!=r[0])
			f->AssosierImage(r[0]);
		if (ii<nbEtape-1)
			{
            int i = 0;
            while (i<listeOp.size() && listeOp[i].first->indEtape != ii+1)
                i++;
            listeOp[i ].first->op[0] = r[0];
			}
        f->DynamiqueAffichage();

		f->NouvelleImage();
		f->MAJNouvelleImage();
		if (f->ImgStat())
        {
            f->ImgStat()->Plot(true);
            f->ImgStat()->MAJOnglet(-1);
        }
		f->DefHistorique();
		}
	}

}


void FenetreAlgo::PositionSouris(int ind, wxPoint p)
{
    if (ind<0 || ind>=spinSouris.size())
        return;

    spinSouris[ind].first->SetValue(p.x);
    spinSouris[ind].first->Refresh();
    spinSouris[ind].second->SetValue(p.y);
    spinSouris[ind].second->Refresh();
    paramSouris[ind]->valeur.x=p.x;
    paramSouris[ind]->valeur.x=p.y;
    ExecuterOperation(listeOnglet[ongletSouris[ind]].second);

}




