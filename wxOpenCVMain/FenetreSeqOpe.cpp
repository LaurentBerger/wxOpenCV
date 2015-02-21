#include "FenetreSeqOpe.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;


#define IND_OPE 100 
#define LISTE_OP_SEQ 101
#define IND_HYPER 10
#define IND_STATIC 200
#define IND_SPIN 300

FenetreSequenceOperation::FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOsgApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
ImageInfoCV xx;
panneau = new wxPanel( this,wxID_ANY ,wxPoint(0,0),wxSize(400,400));
//wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));
this->osgApp =osg;
std::map <int,std::vector <Operation > >  *t=osg->TabSeqOperation();

new wxStaticText( panneau, -1, _("Sequence"),wxPoint(10,20) );
wxSpinCtrl *spw=new wxSpinCtrl(panneau,IND_OPE,_("Sequence"),wxPoint(80,20));
spw->SetRange(0,t->size()-1);
spw->SetValue(0);
nbEtape=100;
if (nbEtape<(*t)[0].size())
	nbEtape=(*t)[0].size();
nomEtape=new wxString[nbEtape];
int i=0;
for (std::vector <Operation >::iterator it = (*t)[0].begin() ; it != (*t)[0].end(); ++it,++i)
    nomEtape[i]=(*it).nomOperation;
choixOp=new wxListBox( panneau,LISTE_OP_SEQ,wxPoint(80,50),wxSize(150,-1),(*t)[0].size(),nomEtape);
choixOp->SetSelection(0);
InsererCtrlEtape(&((*t)[0][0]));
Bind(wxEVT_SPINCTRL, &FenetreSequenceOperation::OnSpinEntier,this);
Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FenetreSequenceOperation::OnOpeSelec,this);
}

void FenetreSequenceOperation::InsererCtrlEtape(Operation *op)
{
Parametre *pOCV=&op->pOCV;
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
	new wxHyperlinkCtrl (panneau,indHyper," OpenCV Documentation",pOCV->lienHtml,wxPoint(10,ligne-50),wxSize(400,20));
indHyper++;
if ((whc=(wxHyperlinkCtrl*)wxWindow::FindWindowById(indHyper,panneau))!=NULL)
	whc->SetURL(pOCV->lienHtml);
else
	new wxHyperlinkCtrl(panneau,indHyper,"PDF Documentation",pOCV->refPDF,wxPoint(10,ligne-20),wxSize(400,20));
indHyper++;
std::map<std::string,DomaineParametre<cv::Size> >::iterator its;
for (its=pOCV->sizeParam.begin();its!=pOCV->sizeParam.end();its++)
	{
	wxString nombre;
	nombre.Printf("%d",its->second.valeur.width);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(its->first);
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau,indStatic,its->first,p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	wsd->SetRange(0,256); 
	wsd->SetIncrement((double)its->second.pas.width); 
	wsd->SetValue(its->second.valeur.width);
	wsd->SetRange(0,256); 
	wsd->Show(true);
	indSpin++;
	p += wxPoint(s.GetX(),0);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indStatic,panneau))!=NULL)
		{
		wst->SetLabel(its->first);
		wst->Move(p);
		}
	else
		wst =new wxStaticText(panneau,indStatic,its->first,p, s);
	wst->Show(true);
	indStatic++;
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sph->SetRange(0,256); 
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);

	wsd->SetRange(0,256); 
	wsd->SetIncrement((double)its->second.pas.height); 
	wsd->SetValue(its->second.valeur.height);
	wsd->Show(true);
	indSpin++;	
	ligne+=20;
	}
std::map<std::string,DomaineParametre<int> >::iterator iti;
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
//	wxSpinCtrl *sp=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sp->SetRange(iti->second.mini,iti->second.maxi); 
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	else
		wsd->Move(p);
	wsd->SetRange(iti->second.mini,iti->second.maxi); 
	wsd->SetValue(iti->second.valeur);
	wsd->SetIncrement(iti->second.pas); 
	wsd->Show(true);
	indSpin++;
	ligne+=20;
	}
std::map<std::string,DomaineParametre<double> >::iterator itd;
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
	wsd->SetRange(itd->second.mini,itd->second.maxi); 
	wsd->SetIncrement(itd->second.pas); 
	wsd->SetValue(itd->second.valeur);
	wsd->Show(true);
	indSpin++;
	ligne+=20;
	}
int i=indStatic;
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
SetClientSize(250,ligne+20);
panneau->Update();
}


FenetreSequenceOperation::~FenetreSequenceOperation()
{
//delete nomEtape;
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
std::map <int,std::vector <Operation > >  *t=((wxOsgApp*)osgApp)->TabSeqOperation();
std::map <int,std::vector <Operation > >::iterator it=(*t).begin();
for (int i=0;i<opSelec;i++,it++);
choixOp->Clear();
if (nbEtape<it->second.size())
	{
	nbEtape=(*t)[opSelec].size();
	delete nomEtape;
	nomEtape=new wxString[nbEtape];
	}
int i=0;
for (std::vector <Operation >::iterator it2 = it->second.begin() ; it2 != it->second.end(); ++it2,++i)
    nomEtape[i]=(*it2).nomOperation;
choixOp->Insert((*it).second.size(),nomEtape,0);
choixOp->SetSelection(0);
InsererCtrlEtape(&(it->second)[0]);

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
std::map <int,std::vector <Operation > >  *t=app->TabSeqOperation();
opSelec=ws->GetValue();
std::map <int,std::vector <Operation > >::iterator it=(*t).begin();
for (int i=0;i<opSelec;i++,it++);
int indOpe=event.GetInt();
if (indOpe<0 || indOpe>=it->second.size())
	return;
InsererCtrlEtape(&(it->second[indOpe]));
}


void FenetreSequenceOperation::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

Parametre *pOCV=&listeOp[ind].first->pOCV;
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
	if ((w.GetId()-1)%4==0)
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

void  FenetreSequenceOperation::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreSequenceOperation::OnSpinMoins(wxSpinEvent& w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

Parametre *pOCV=&listeOp[ind].first->pOCV;
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

void FenetreSequenceOperation::OnTextValider(wxCommandEvent &w)
{
}
void FenetreSequenceOperation::OnKeyDown(wxKeyEvent &)
{
}

void FenetreSequenceOperation::OnClose(wxCloseEvent& event)
{
((wxOsgApp *)osgApp)->FenetreSeqOpe((void*)0xFFFFFFFF);
wxFrame::OnCloseWindow(event);

}

void FenetreSequenceOperation::ExecuterOperation(int indEtape)
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
	Parametre *pOCV=&listeOp[i].first->pOCV;
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
