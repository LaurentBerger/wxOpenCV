#include "ParametreOperation.h"
#include "FenetreOperande.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;

void FenetrePrincipale::FenetreOpe()
{


if (!osgApp->FenetreOperande())
	return;

FenetreOperande *fenOpe=new FenetreOperande(this,origineImage.nomOperation,wxPoint(530,0), wxSize(430,570),this->osgApp);
fenOpe->DefFenMere(this);
fenOpe->DefOSGApp(this->osgApp);
fenOpe->Show(true);
fenOpe->Refresh(true);
osgApp->FenetreOperande(fenOpe);
//fenAlgo->SendSizeEvent   ();
}




#define ID_SAUVER_SEQ_CONFIG 1000
#define ID_NOM_SEQUENCE 1001
#define ID_SAUVER_SEQ_XML 1002

FenetreOperande::FenetreOperande(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOpencvApp *osg, long style)
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
Bind(wxEVT_SPINCTRLDOUBLE, &FenetreOperande::OnSpinReel, this);
Bind(wxEVT_COMMAND_COMBOBOX_SELECTED,&FenetreOperande::ComboBox,this);
SetSizerAndFit( topsizer );
Show(true);
}


void FenetreOperande::ComboBox(wxCommandEvent &w)
{
wxOpencvApp *app=(wxOpencvApp *)osgApp;
if (!osgApp || !fenMere)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel().c_str();
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






FenetreOperande::~FenetreOperande()
{
fenMere->RAZFenAlgo();

}

void FenetreOperande::OnActivate(wxActivateEvent& event)
{
}

void FenetreOperande::OnPaint(wxPaintEvent& event)
{
}

void FenetreOperande::OnSpinEntier(wxSpinEvent &w)
{
wxOpencvApp *app=(wxOpencvApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel().c_str();
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

void FenetreOperande::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOpencvApp *app=(wxOpencvApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

ParametreOperation *pOCV=listeOp[ind].first;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel().c_str();
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
if (pOCV->sizeParam.find(nom.substr(0, nom.length() - 2)) != pOCV->sizeParam.end())
{
	if ((w.GetId()) % 4 == 0)
	{
		if (pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.width == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.width = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.height == ((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom.substr(0, nom.length() - 2)].valeur.height = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}
if (pOCV->pointParam.find(nom.substr(0,nom.length()-2)) != pOCV->pointParam.end())
	{
	if (nom.substr(nom.length()-1, 1)=='x')
	{
		if (pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.x == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.x = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.y == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->pointParam[nom.substr(0, nom.length() - 2)].valeur.y = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}
}

void  FenetreOperande::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreOperande::OnSpinMoins(wxSpinEvent& w)
{
}

void FenetreOperande::OnTextValider(wxCommandEvent &w)
{
}
void FenetreOperande::OnKeyDown(wxKeyEvent &)
{
}

void FenetreOperande::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenAlgo();
wxFrame::OnCloseWindow(event);

}

void FenetreOperande::ExecuterOperation(int indEtape)
{
}
