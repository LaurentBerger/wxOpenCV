#include "FenetreSeqOpe.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;

#define IDSAUVER 500
#define IND_OPE 100 
#define LISTE_OP_SEQ 101
#define IND_HYPER 10 // hyperlien
#define IND_STATIC 200 // champ static
#define IND_SPIN 300 // bouton spin
#define IND_SELECMULTIPLE IND_HYPER-1 // Selection de fichier
#define IND_FENETREUNIQUE IND_HYPER-2 // Selection de fichier

FenetreSequenceOperation::FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOsgApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
ImageInfoCV xx;
fenMere=frame;
tailleMax=wxSize(0,0);
panneau = new wxPanel( this,wxID_ANY ,wxPoint(0,0),wxSize(400,400));
//wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));
this->osgApp =osg;
std::map <int,std::vector <ParametreOperation > >  *t=osg->TabSeqOperation();

new wxStaticText( panneau, -1, _("Sequence"),wxPoint(10,20),wxSize(60,20) );
new wxButton( panneau,wxID_OK,_("Execute all"),wxPoint(150,20),wxSize(70,20));
new wxCheckBox( panneau,IND_FENETREUNIQUE,_("No new window"),wxPoint(150,40),wxSize(120,20));
new wxButton( panneau,IND_SELECMULTIPLE,_("Select file to process"),wxPoint(230,20),wxSize(150,20));
wxSpinCtrl *spw=new wxSpinCtrl(panneau,IND_OPE,_("Sequence"),wxPoint(80,20),wxSize(60,20));
spw->SetRange(0,t->size()-1);
spw->SetValue(0);
nbEtape=100;
int n=(*t).begin()->first;
if (nbEtape<(*t)[n].size())
	nbEtape=(*t)[n].size();
nomEtape=new wxString[nbEtape];
int i=0;

for (std::vector <ParametreOperation >::iterator it = (*t)[n].begin() ; it != (*t)[n].end(); ++it,++i)
    nomEtape[i]=(*it).nomOperation;
choixOp=new wxListBox( panneau,LISTE_OP_SEQ,wxPoint(80,60),wxSize(150,-1),(*t)[n].size(),nomEtape);
choixOp->SetSelection(0);
InsererCtrlEtape(&((*t)[n][0]));
Bind(wxEVT_SPINCTRL, &FenetreSequenceOperation::OnSpinEntier,this);
Bind(wxEVT_SPINCTRLDOUBLE, &FenetreSequenceOperation::OnSpinReel,this);
Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FenetreSequenceOperation::OnOpeSelec,this);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreSequenceOperation::Executer,this,wxID_OK);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreSequenceOperation::SelectionFichier,this,IND_SELECMULTIPLE);
}

void FenetreSequenceOperation::InsererCtrlEtape(ParametreOperation *op)
{
ParametreOperation *pOCV=op;
int ligne=150;
int	indStatic=IND_STATIC;
int indSpin=IND_SPIN;
int indHyper=IND_HYPER;

wxStaticText		*wst;
wxHyperlinkCtrl		*whc;
wxSpinCtrlDouble	*wsd;

if ((whc=(wxHyperlinkCtrl*)wxWindow::FindWindowById(indHyper,panneau))!=NULL)
	whc->SetURL(pOCV->lienHtml);
else
	new wxHyperlinkCtrl (panneau,indHyper," OpenCV Documentation",pOCV->lienHtml,wxPoint(10,ligne-30),wxSize(150,20));
indHyper++;
if ((whc=(wxHyperlinkCtrl*)wxWindow::FindWindowById(indHyper,panneau))!=NULL)
	whc->SetURL(pOCV->lienHtml);
else
	new wxHyperlinkCtrl(panneau,indHyper,"PDF Documentation",pOCV->refPDF,wxPoint(160,ligne-30),wxSize(200,20));
indHyper++;
std::map<std::string,DomaineParametreOp<cv::Size> >::iterator its;
if (tailleMax.x<410)
	tailleMax.x= 410;
if (tailleMax.y<ligne)
	tailleMax.y= ligne;
for (its=pOCV->sizeParam.begin();its!=pOCV->sizeParam.end();its++)
	{
	wxString nombre;
	nombre.Printf("%d",its->second.valeur.width);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(its->first + " x");
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau, indStatic, its->first + " x", p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);
	wsd->SetIncrement((double)its->second.pas.width); 
	wsd->SetRange(its->second.mini.width, its->second.maxi.width);
	wsd->SetValue(its->second.valeur.width);
	wsd->Show(true);
	indSpin++;
	p += wxPoint(s.GetX(),0);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(its->first + " y");
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau, indStatic, its->first + " y", p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);

	wsd->SetName(its->first);
	wsd->SetRange(its->second.mini.height, its->second.maxi.height);
	wsd->SetIncrement((double)its->second.pas.height); 
	wsd->SetValue(its->second.valeur.height);
	wsd->Show(true);
	indSpin++;	
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	ligne+=20;
	}
std::map<std::string,DomaineParametreOp<int> >::iterator iti;
for (iti=pOCV->intParam.begin();iti!=pOCV->intParam.end();iti++)
	{
	wxString nombre;
	nombre.Printf("%d",iti->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(iti->first);
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau,indStatic,iti->first,p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);
	wsd->SetRange(iti->second.mini,iti->second.maxi); 
	wsd->SetName(iti->first);
	wsd->SetValue(iti->second.valeur);
	wsd->SetIncrement(iti->second.pas); 
	wsd->Show(true);
	indSpin++;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	ligne+=20;
	}
std::map<std::string,DomaineParametreOp<double> >::iterator itd;
for (itd=pOCV->doubleParam.begin();itd!=pOCV->doubleParam.end();itd++)
	{
	wxString nombre;
	nombre.Printf("%f",itd->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(itd->first);
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau,indStatic,itd->first,p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd=new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);
	wsd->SetRange(itd->second.mini,itd->second.maxi); 
	wsd->SetName(itd->first);
	wsd->SetIncrement(itd->second.pas); 
	wsd->SetValue(itd->second.valeur);
	wsd->Show(true);
	indSpin++;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	ligne+=20;
	}
std::map<std::string, DomaineParametreOp<cv::Point> >::iterator itp;
for (itp = pOCV->pointParam.begin(); itp != pOCV->pointParam.end(); itp++)
{
	wxString nombre;
	nombre.Printf("%d", itp->second.valeur.x);
	wxPoint p(10, ligne);
	wxSize	s(100, 20);
	if ((wst = (wxStaticText*)panneau->FindWindowById(indStatic, panneau)) != NULL)
	{
		wst->SetLabel(itp->first + " x");
		wst->Move(p);
	}
	else
		wst = new wxStaticText(panneau, indStatic, itp->first + " x", p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(), 0);
	//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	if ((wsd = (wxSpinCtrlDouble*)panneau->FindWindowById(indSpin, panneau)) == NULL)
		wsd = new wxSpinCtrlDouble(panneau, indSpin, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
	else
		wsd->Move(p);
	wsd->SetRange(0, 256);
	wsd->SetIncrement((double)itp->second.pas.x);
	wsd->SetRange(itp->second.mini.y, itp->second.maxi.y);
	wsd->SetValue(itp->second.valeur.x);
	wsd->Show(true);
	indSpin++;
	p += wxPoint(s.GetX(), 0);
	if ((wst = (wxStaticText*)panneau->FindWindowById(indStatic, panneau)) != NULL)
	{
		wst->SetLabel(itp->first + " y");
		wst->Move(p);
	}
	else
		wst = new wxStaticText(panneau, indStatic, itp->first + " y", p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(), 0);
	nombre.Printf("%d", itp->second.valeur.y);
	//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	//	sph->SetRange(0,256); 
	if ((wsd = (wxSpinCtrlDouble*)panneau->FindWindowById(indSpin, panneau)) == NULL)
		wsd = new wxSpinCtrlDouble(panneau, indSpin, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
	else
		wsd->Move(p);

	wsd->SetName(itp->first);
	wsd->SetRange(itp->second.mini.y, itp->second.maxi.y);
	wsd->SetIncrement((double)itp->second.pas.y);
	wsd->SetValue(itp->second.valeur.y);
	wsd->Show(true);
	indSpin++;
	if (tailleMax.x<p.x + s.x)
		tailleMax.x = p.x + s.x;
	if (tailleMax.y<p.y + s.y)
		tailleMax.y = p.y + s.y;
	ligne += 20;
}
int i = indStatic;
while (panneau->FindWindowById(i,panneau)&& i<IND_SPIN)
	{
	panneau->FindWindowById(i,panneau)->Show(false);
	i++;
	}
i=indSpin;
while (panneau->FindWindowById(i,panneau))
	{
	panneau->FindWindowById(i,panneau)->Show(false);
	i++;
	}
SetClientSize(tailleMax+wxSize(10,10));
panneau->Update();
}


FenetreSequenceOperation::~FenetreSequenceOperation()
{
delete []nomEtape;
nomEtape=NULL;	
if (osgApp)
	((wxOsgApp*)osgApp)->FenetreSeqOpe(NULL);

}

void FenetreSequenceOperation::OnActivate(wxActivateEvent& event)
{
}

void FenetreSequenceOperation::OnPaint(wxPaintEvent& event)
{
}

void FenetreSequenceOperation::OnSpinEntier(wxSpinEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
int opSelec=w.GetValue();
std::map <int,std::vector <ParametreOperation > >  *t=((wxOsgApp*)osgApp)->TabSeqOperation();
std::map <int,std::vector <ParametreOperation > >::iterator it=(*t).begin();
for (int i=0;i<opSelec&& it!=(*t).end();i++,it++);
choixOp->Clear();
if (nbEtape<it->second.size())
	{
	nbEtape=(*t)[opSelec].size();
	delete nomEtape;
	nomEtape=new wxString[nbEtape];
	}
int i=0;
for (std::vector <ParametreOperation >::iterator it2 = it->second.begin() ; it2 != it->second.end(); ++it2,++i)
    nomEtape[i]=(*it2).nomOperation;
choixOp->Insert((*it).second.size(),nomEtape,0);
choixOp->SetSelection(0);
InsererCtrlEtape(&(it->second)[0]);

}

void FenetreSequenceOperation::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);
if (ws==NULL)
	return;
string nom;
int opSelec=choixOp->GetSelection();


std::map <int,std::vector <ParametreOperation > >  *t=app->TabSeqOperation();
std::map <int,std::vector <ParametreOperation > >::iterator it=(*t).begin();
for (int i=0;i<ws->GetValue();i++,it++);

nom=((wxWindow*)w.GetEventObject())->GetName();
ParametreOperation p=it->second[opSelec];
wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 100, this);
if (!st)
	throw("wxStaticText undefined");
nom = st->GetLabel();
if (it->second[opSelec].doubleParam.find(nom) != it->second[opSelec].doubleParam.end())
{
	if (it->second[opSelec].doubleParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	it->second[opSelec].doubleParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
}
if (it->second[opSelec].intParam.find(nom) != it->second[opSelec].intParam.end())
{
	if (it->second[opSelec].intParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	it->second[opSelec].intParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
}
if (it->second[opSelec].sizeParam.find(nom.substr(0, nom.length() - 2)) != it->second[opSelec].sizeParam.end())
{
	if ((w.GetId()) % 4 == 0)
	{
		if (it->second[opSelec].sizeParam[nom.substr(0, nom.length() - 2)].valeur.width == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		it->second[opSelec].sizeParam[nom.substr(0, nom.length() - 2)].valeur.width = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (it->second[opSelec].sizeParam[nom.substr(0, nom.length() - 2)].valeur.height == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		it->second[opSelec].sizeParam[nom.substr(0, nom.length() - 2)].valeur.height = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}
if (it->second[opSelec].pointParam.find(nom.substr(0, nom.length() - 2)) != it->second[opSelec].pointParam.end())
{
	if (nom.substr(nom.length() - 1, 1) == 'x')
	{
		if (it->second[opSelec].pointParam[nom.substr(0, nom.length() - 2)].valeur.x == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		it->second[opSelec].pointParam[nom.substr(0, nom.length() - 2)].valeur.x = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (it->second[opSelec].pointParam[nom.substr(0, nom.length() - 2)].valeur.y == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		it->second[opSelec].pointParam[nom.substr(0, nom.length() - 2)].valeur.y = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}





if (OperandePresent(&(it->second)))
	ExecuterSequence(&(it->second));
}


void FenetreSequenceOperation::OnOpeSelec(wxCommandEvent& event)
{
int opSelec;
wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);
if (ws==NULL)
	return;
if (!osgApp)
	return;
wxOsgApp *app=(wxOsgApp *)osgApp;
std::map <int,std::vector <ParametreOperation > >  *t=app->TabSeqOperation();
opSelec=ws->GetValue();
std::map <int,std::vector <ParametreOperation > >::iterator it=(*t).begin();
for (int i=0;i<opSelec&& it!=(*t).end();i++,it++);
int indOpe=event.GetInt();
if (indOpe<0 || indOpe>=it->second.size())
	return;
InsererCtrlEtape(&(it->second[indOpe]));
}

void FenetreSequenceOperation::SelectionFichier(wxCommandEvent& event)
{
if (!osgApp)
	return;
wxOsgApp *app=(wxOsgApp *)osgApp;

wxFileDialog ouverture(NULL, "Ouvrir ", wxEmptyString, wxEmptyString, "*.tif;*.jpg;*.bmp;*.png;*.yml;*.gz",wxFD_FILE_MUST_EXIST |wxFD_MULTIPLE );
if (ouverture.ShowModal()!=wxID_OK)
	return;
ouverture.GetPaths( fichierSelectionnes);
std::map <int,std::vector <ParametreOperation > >  *t=app->TabSeqOperation();
int opSelec;
wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);
if (ws==NULL)
	return;
opSelec=ws->GetValue();
std::map <int,std::vector <ParametreOperation > >::iterator itRef=(*t).begin();
for (int i=0;i<opSelec && itRef!=(*t).end();i++,itRef++);

if (itRef!=(*t).end())
	for (int i=0;i<fichierSelectionnes.size();i++)
		{
		vector<ImageInfoCV*> r;
		std::map <int,std::vector <ParametreOperation > >::iterator it=itRef;
		wxCharBuffer ww=fichierSelectionnes[i].mb_str ();
		char *nomFichier=ww.data() ;
		ImageInfoCV *imIni= new ImageInfoCV(nomFichier);
		ImageInfoCV **im=NULL;
		ImageInfoCV *imTmp=NULL;
		for (std::vector <ParametreOperation > ::iterator it=itRef->second.begin();it!=itRef->second.end();it++)
			{
			ParametreOperation pOCV=*it;
			if (im)
				{
				pOCV.op1=im[0];
				if (imTmp)
					delete imTmp;
				imTmp=im[0];
				}
			else
				pOCV.op1=imIni;
			pOCV.indOp1Fenetre=-1;

			r=pOCV.ExecuterOperation();
			}
		delete imIni;
		delete imTmp;
		wxString nom(nomFichier);
		nom=itRef->second.begin()->nomSequence+nom+".yml";
		ww=nom.mb_str ();
		nomFichier=ww.data() ;
		cv::FileStorage fs(nomFichier, cv::FileStorage::WRITE);
		fs<<"Image"<<*((cv::Mat*)r[0]);
		fs.release();

		delete im[0];
		}


}



void  FenetreSequenceOperation::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreSequenceOperation::OnSpinMoins(wxSpinEvent& w)
{
}

void FenetreSequenceOperation::OnTextValider(wxCommandEvent &w)
{
}
void FenetreSequenceOperation::OnKeyDown(wxKeyEvent &)
{
}

void FenetreSequenceOperation::OnClose(wxCloseEvent& event)
{
delete []nomEtape;
nomEtape=NULL;
((wxOsgApp *)osgApp)->FenetreSeqOpe((void*)0xFFFFFFFF);
wxFrame::OnCloseWindow(event);

}

void FenetreSequenceOperation::Executer(wxCommandEvent& event)
{
int opSelec;
wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);
if (ws==NULL)
	return;
if (!osgApp)
	return;
wxOsgApp *app=(wxOsgApp *)osgApp;
std::map <int,std::vector <ParametreOperation > >  *t=app->TabSeqOperation();
opSelec=ws->GetValue();
std::map <int,std::vector <ParametreOperation > >::iterator it=(*t).begin();
for (int i=0;i<opSelec&& it!=(*t).end();i++,it++);
ExecuterSequence(&(it->second));
}

void FenetreSequenceOperation::ExecuterSequence(std::vector <ParametreOperation> *sq)
{
if (!osgApp)
	return;
OperandePresent(sq);
wxOsgApp	*app=(wxOsgApp *)osgApp;
ImageInfoCV **im=NULL;
ImageInfoCV *imTmp=NULL;
int i=0;
if (fenMere->Cam() && fenMere->Cam()->IsRunning())
	{
	wxCriticalSectionLocker enter(((FenetrePrincipale*)fenMere)->paramCam);

	fenMere->DefSeqCamera(sq);
	return;
	}	

vector<ImageInfoCV*> r;
for (std::vector <ParametreOperation > ::iterator it=sq->begin();it!=sq->end();it++)
	{
	ParametreOperation pOCV=*it;
	wxString  nomOperation(it->nomOperation);
	app->DefOperateurImage(nomOperation);
	int indFen1=fenMere->IdFenetre();
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
	if (r.size()==0)
		{
		pOCV.op1=fenMere->ImAcq();
		pOCV.indOp1Fenetre=indFen1;
		}
	else
		{
		if (imTmp)
			delete imTmp;
		imTmp=r[0];
		pOCV.op1=r[0];
		pOCV.indOp1Fenetre=-1;
		}

	int indFen2=it->indOp2Fenetre;
	if (indFen2>=0)
		{
		
		pOCV.op2=app->Graphique(indFen2)->ImAcq();
		}
//	if (pOCV.intParam.find(
	r=app->ExecuterOperation(&pOCV);
	}
if (r.size()!=0)
	{
	wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);
	wxCheckBox *wb=(wxCheckBox *)wxWindow::FindWindowById(IND_HYPER-2,panneau);
	int opSelec;
	if (ws)
		opSelec=ws->GetValue();
	else
		opSelec=9999;
	FenetrePrincipale *f;
	if (wb->GetValue() && idFenetre.find(opSelec)!=idFenetre.end() && app->Graphique(idFenetre[opSelec].second) )
		{
		f=idFenetre[opSelec].first;
		f->AssosierImage(r[0]);

		}
	else
		{
		f = new FenetrePrincipale(NULL, "wxOpenCV",
		wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
		wxString s;
		s.Printf("%d : %s( %d) of image %d ",app->NbFenetre(),_("Sequence"),opSelec,fenMere->IdFenetre());	
		f->SetTitle(s);
		f->DefOSGApp(app);
		idFenetre[opSelec] = make_pair (f, f->IdFenetre());
		f->AssosierImage(r[0]);
		app->InitFenAssociee(f);
		f->InitIHM();
		}


	f->NouvelleImage();
	f->MAJNouvelleImage();
	if (f->ImgStat())
		f->ImgStat()->Plot(true);
	f->DefHistorique();
	}
}



bool FenetreSequenceOperation::OperandePresent(std::vector <ParametreOperation> *sq)
{
if (!osgApp)
	return false;
wxOsgApp	*app=(wxOsgApp *)osgApp;
ImageInfoCV **im=NULL;
ImageInfoCV *imTmp=NULL;
int i=0;
bool videoActive=false;
if (fenMere->Cam() && fenMere->Cam()->IsRunning())
	videoActive=true;
for (std::vector <ParametreOperation > ::iterator it=sq->begin();it!=sq->end();it++)
	{
	ParametreOperation pOCV=*it;
	wxString  nomOperation(it->nomOperation);
	int indFen1=fenMere->IdFenetre();
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return false;
		}
	if (!fenMere->ImAcq())
		{
		wxMessageBox(_("Empty image?"),_("Problem"), wxOK );
		return false;
		}
	long indFen2=it->indOp2Fenetre;
	bool annuler=false;
	while (it->opBinaireSelec && !app->Graphique(indFen2) && !videoActive)
		{
			wxTextEntryDialog  adr( NULL,_("Empty image. Give window id of image"),"0");   
			if (adr.ShowModal())
				adr.GetValue().ToCLong(&indFen2);
			else
				annuler=true;
			it->indOp2Fenetre = indFen2 ;
		}
	if (annuler)
		return false;
	if (it->opBinaireSelec && !videoActive && app->Fenetre(indFen2))
		it->op2=app->Graphique(indFen2)->ImAcq();
	else
		it->op2 =NULL;
	}
return true;
}
