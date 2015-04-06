///////////////////////////////////////////////////////////////////////////////
// Name:        auidemo.cpp
// Purpose:     wxaui: wx advanced user interface - sample/test program
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2005-10-03
// Copyright:   (C) Copyright 2005, Kirix Corporation, All Rights Reserved.
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////



//xgettext --c++ --from-code=cp1252 -a --no-location -s --no-wrap --escape wxIUA.cpp -j fr/wxIUA.po


// For compilers that support precompilation, includes "wx/wx.h".
#include "FenetrePrincipale.h"
#include "wx/wxprec.h"
#include <wx/translation.h>

#include "InterfaceAvance.h"
#include "courbeplplot.h"



#ifdef __WINDOWS__
    #define USE_XPM_BITMAPS 0
#else
    #define USE_XPM_BITMAPS 1
#endif

enum
{
    // menu items
    ID_ADDITION =1,
    ID_SOUSTRACTION,
    ID_DIVISION,
	ID_MULTIPLICATION,
	ID_OU_LOGIQUE,
	ID_ET_LOGIQUE,
	ID_OUEXCLU_LOGIQUE,
	ID_NEGATION,
	ID_EROSION,
    ID_DILATATION,
    ID_OUVERTURE,
    ID_FERMETURE,
    ID_CHAPHAUTBL,
    ID_CHAPHAUTNO,
    ID_GRADMORPH,
    ID_CONVOLUTION,
    ID_GRADIENT_MOD,
    ID_GRADIENT_X,
    ID_GRADIENT_Y,
	ID_LAPLACIEN,
 	ID_LISMOY,
	ID_LISGAU,
	ID_LISMED,
	ID_THRESHOLD,
	ID_ADATHRESHOLD,
    ID_DISTANCEDISCRETE,
    ID_SQUELETTE,
    ID_VORONOI,
	ID_CONTOUR,
	ID_COMPCONNEXE,
	ID_PARTAGE_EAUX,
    ID_CANNY,
	ID_CORNERHARRIS,
	ID_GOODFEATURE,
	ID_HOUGHCIRCLE,
	ID_HOUGHLINE,
	ID_HOUGHLINEP,
	ID_PYRFLOTOPTIQUE,
	ID_CALCFLOTOPTIQUE,
	ID_CALCFLOTOPTIQUEFARNER,
	ID_ESTIM_TRANS,
	ID_PHASE_CORRELATE,
	ID_MAJ_MVT,




	ID_FFT,
	ID_IFFT,
	ID_FUSIONPLAN,
	ID_SEPARATIONPLAN,
	ID_RGBLUMINANCE,
	ID_FIN_OP, // Fin des opérateurs sur les images
	ID_VIDEO_8_UC3,
	ID_VIDEO_32_FC3,
	ID_VIDEO_FLUX
};



BEGIN_EVENT_TABLE(InterfaceAvance, wxFrame)
    EVT_ERASE_BACKGROUND(InterfaceAvance::OnEraseBackground)
    EVT_SIZE(InterfaceAvance::OnSize)

    EVT_MENU(wxID_EXIT, InterfaceAvance::OnQuit)


    EVT_MENU(wxID_OPEN, InterfaceAvance::Ouvrir)
    EVT_MENU_RANGE(ID_ADDITION,ID_FIN_OP, InterfaceAvance::SelectOperation)

	EVT_MENU(ID_VIDEO_8_UC3, InterfaceAvance::Video)
	EVT_MENU(ID_VIDEO_32_FC3, InterfaceAvance::Video)
	EVT_MENU(ID_VIDEO_FLUX, InterfaceAvance::Video)


    EVT_MENU(InterfaceAvance::ID_CreateTree, InterfaceAvance::OnCreateTree)
    EVT_MENU(InterfaceAvance::ID_RAZIHM, InterfaceAvance::OnRazIHM)
    EVT_MENU(InterfaceAvance::ID_CreateText, InterfaceAvance::OnCreateText)
    EVT_MENU(InterfaceAvance::ID_CreateSizeReport, InterfaceAvance::OnCreateSizeReport)
    EVT_MENU(InterfaceAvance::ID_CreateNotebook, InterfaceAvance::OnCreateNotebook)
    EVT_MENU(ID_Settings, InterfaceAvance::OnSettings)
    EVT_MENU(wxID_EXIT, InterfaceAvance::OnExit)
    EVT_MENU(wxID_ABOUT, InterfaceAvance::OnAbout)
//    EVT_MENU_RANGE(InterfaceAvance::ID_FirstPerspective, InterfaceAvance::ID_FirstPerspective+1000,
//                   InterfaceAvance::OnRestorePerspective)
    EVT_AUI_PANE_CLOSE(InterfaceAvance::OnPaneClose)
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, InterfaceAvance::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, InterfaceAvance::OnNotebookPageClosed)
    EVT_CLOSE(InterfaceAvance::OnClose)
// Gestion de l'interface des filtres
	EVT_LISTBOX(LBnxn, InterfaceAvance::AfficheFiltre)
	EVT_LISTBOX(LBIND_MORPH, InterfaceAvance::CtrlMorpho)
	EVT_LISTBOX(LBTY_MORPH,InterfaceAvance::CtrlMorpho)
	EVT_LISTBOX(LBBord,InterfaceAvance::DefBord)
	EVT_LISTBOX(LBPrecision,InterfaceAvance::DefPrecision)
    EVT_SPINCTRL(ROUETAILLE_MORPH, InterfaceAvance::TailleFiltre)
	EVT_GRID_CELL_CHANGING(InterfaceAvance::NouveauFiltre)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(wxSizeReportCtrl, wxControl)
    EVT_PAINT(wxSizeReportCtrl::OnPaint)
    EVT_SIZE(wxSizeReportCtrl::OnSize)
    EVT_ERASE_BACKGROUND(wxSizeReportCtrl::OnEraseBackground)
END_EVENT_TABLE()

#if USE_XPM_BITMAPS
// Barre outils Fichier et edition
    #include "bitmaps/new.xpm"
    #include "bitmaps/open.xpm"
    #include "bitmaps/save.xpm"
    #include "bitmaps/copy.xpm"
    #include "bitmaps/cut.xpm"
    #include "bitmaps/paste.xpm"  // paste XPM
    #include "bitmaps/print.xpm"
    #include "bitmaps/help.xpm"
    #include "bitmaps/preview.xpm"  // paste XPM
// Barre outils opérateur
    #include "bitmaps/addition.xpm"
    #include "bitmaps/soustraction.xpm"
	#include "bitmaps/multiplication.xpm"
	#include "bitmaps/division.xpm"
	#include "bitmaps/etlogique.xpm"
	#include "bitmaps/oulogique.xpm"
	#include "bitmaps/oueclulogique.xpm"
	#include "bitmaps/negation.xpm"
// Barre outils morpholgie
    #include "bitmaps/erosion.xpm"
    #include "bitmaps/dilatation.xpm"
    #include "bitmaps/ouverture.xpm"
    #include "bitmaps/fermeture.xpm"
    #include "bitmaps/chaphautblanc.xpm"
    #include "bitmaps/chaphautnoir.xpm"
    #include "bitmaps/gradmorph.xpm"
// Barre outils convolution
    #include "bitmaps/convolution.xpm"
    #include "bitmaps/gradient_mod.xpm"
    #include "bitmaps/gradient_x.xpm"
    #include "bitmaps/gradient_y.xpm"
    #include "bitmaps/laplacien.xpm"
    #include "bitmaps/flou.xpm"
    #include "bitmaps/LisMoy.xpm"
    #include "bitmaps/LisMed.xpm"
    #include "bitmaps/LisGau.xpm"
// Barre outils binarization
    #include "bitmaps/seuillage.xpm"
    #include "bitmaps/seuillageada.xpm"
    #include "bitmaps/distancediscrete.xpm"
    #include "bitmaps/squelette.xpm"
    #include "bitmaps/voronoi.xpm"
    #include "bitmaps/canny.xpm"
    #include "bitmaps/contour.xpm"
   #include "bitmaps/statconnexe.xpm"
   #include "bitmaps/cmpconnexe.xpm"
// Barre outils Frequence échelle
   #include "bitmaps/fft.xpm"
   #include "bitmaps/ifft.xpm"
// Barre outils autre
   #include "bitmaps/separationplan.xpm"
   #include "bitmaps/fusionplan.xpm"
   #include "bitmaps/rgbluminance.xpm"
// Barre d'outils flot optique
   #include "bitmaps/pyrflotoptique.xpm"
   #include "bitmaps/calcflotoptique.xpm"
   #include "bitmaps/calcflotoptiquefarner.xpm"
   #include "bitmaps/majmvt.xpm"
#include "bitmaps/estimtrans.xpm"
#include "bitmaps/phasecorrel.xpm"


#endif // USE_XPM_BITMAPS

InterfaceAvance::InterfaceAvance(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style)
{
    ongletConvol =NULL;
	ongletMorphologie=NULL;
	ongletParamImage=NULL;
	tbOperation=NULL;
	tbConv=NULL;
	tbMorph=NULL;
	// tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);

    // set frame icon

    // set up default notebook style
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;

    InstallationBarreMenu();

    CreateStatusBar();
    GetStatusBar()->SetStatusText(_("Ready"));


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(wxSize(400,300));



    // prepare a few custom overflow elements for the toolbars' overflow buttons
	InstallationbarreOutils(0);
	InstallationbarreOutils(1);
	InstallationbarreOutils(2);
	InstallationbarreOutils(3);
	InstallationbarreOutils(4);
	InstallationbarreOutils(5);
	InstallationbarreOutils(6);
	InstallationbarreOutils(7);
	InstallationbarreOutils(8);
	InstallationbarreOutils(9);


    wxWindow* wnd10 = CreateTextCtrl(wxEmptyString);

 	wxLogTextCtrl *logWindow = new wxLogTextCtrl((wxTextCtrl *)wnd10);
	wxLog::SetVerbose(true);
	delete wxLog::SetActiveTarget(logWindow);

   // Give this pane an icon, too, just for testing.
    int iconSize = m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE);

    // Make it even to use 16 pixel icons with default 17 caption height.
    iconSize &= ~1;

    m_mgr.AddPane(wnd10, wxAuiPaneInfo().
                  Name(_("Report")).Caption(_("Log window ")).
                  Bottom().Layer(1).Position(1).
                  Icon(wxArtProvider::GetBitmap(wxART_WARNING,
                                                wxART_OTHER,
                                                wxSize(iconSize, iconSize))));
    // make some default perspectives
	pane = CreerChoixOperateur();
    m_mgr.AddPane(pane, wxAuiPaneInfo().Name(_("Parameters Operators")).
                  CenterPane().PaneBorder(false));

    wxString perspective_all = m_mgr.SavePerspective();

    int i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide();
    m_mgr.GetPane(_("Parameters Operators")).Show();
    m_mgr.GetPane(_("grid_content")).Show();
    m_mgr.GetPane(_("Report")).Show().Bottom().Layer(0).Row(0).Position(0);

    wxString perspective_default = m_mgr.SavePerspective();

    m_perspectives.Add(perspective_default);
    m_perspectives.Add(perspective_all);

    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
}

void InterfaceAvance::OnClose(wxCloseEvent& event)
{
wxFrame::OnCloseWindow(event);
((wxOsgApp*)osgApp)->Quitter(NULL);
}


InterfaceAvance::~InterfaceAvance()
{
    m_mgr.UnInit();
}

wxAuiDockArt* InterfaceAvance::GetDockArt()
{
    return m_mgr.GetArtProvider();
}

void InterfaceAvance::DoUpdate()
{
    m_mgr.Update();
}

void InterfaceAvance::OnEraseBackground(wxEraseEvent& event)
{
    event.Skip();
}

void InterfaceAvance::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

void InterfaceAvance::OnSettings(wxCommandEvent& WXUNUSED(evt))
{
    // show the settings pane, and float it
    wxAuiPaneInfo& floating_pane = m_mgr.GetPane(_("settings")).Float().Show();

    if (floating_pane.floating_pos == wxDefaultPosition)
        floating_pane.FloatingPosition(GetStartPosition());

    m_mgr.Update();
}
void InterfaceAvance::OnPaneClose(wxAuiManagerEvent& evt)
{
 if (evt.pane->name == _("test10"))
    {
        int res = wxMessageBox(_("Are you sure you want to close/hide this pane?"),
                               _("wxAUI"),
                               wxYES_NO,
                               this);
        if (res != wxYES)
            evt.Veto();
    }
}

void InterfaceAvance::OnRestorePerspective(wxCommandEvent& WXUNUSED(evt))
{
    m_mgr.LoadPerspective(m_perspectives.Item(0));
}

void InterfaceAvance::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
	wxWindow *w=ctrl->GetCurrentPage();
	int ind=ctrl->GetSelection();
	int indConvol=ctrl->GetPageIndex(ongletConvol);
	int indMorph=ctrl->GetPageIndex(ongletMorphologie);
	if (ctrl->GetPageIndex(ongletConvol)==ctrl->GetSelection())
        {
		int res = wxMessageBox(_("Are you sure you want to close/hide this notebook page?"),
                       _("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
		else
			ongletConvol=NULL;
		}
	if (ctrl->GetPageIndex(ongletParamImage)==ctrl->GetSelection())
        {
		int res = wxMessageBox(_("Are you sure you want to close/hide page?"),
                       _("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
		else
			ongletParamImage=NULL;
		}
	if (ctrl->GetPageIndex(ongletMorphologie)==ctrl->GetSelection())
        {
		int res = wxMessageBox(_("Are you sure you want to close/hide page?"),
                       _("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
		else
			ongletMorphologie =NULL;
		}
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(wxHtmlWindow)))
    {
        int res = wxMessageBox(_("Are you sure you want to close/hide page?"),
                       _("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
    }
}

void InterfaceAvance::OnNotebookPageClosed(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    // selection should always be a valid index
    wxASSERT_MSG(ctrl->GetSelection()  < (int)ctrl->GetPageCount(),
                  wxString::Format(_("Invalid selection %d, only %d pages left"),
                                   ctrl->GetSelection(),
                                   (int)ctrl->GetPageCount()) );

    evt.Skip();
}


wxPoint InterfaceAvance::GetStartPosition()
{
    static int x = 0;
    x += 20;
    wxPoint pt = ClientToScreen(wxPoint(0,0));
    return wxPoint(pt.x + x, pt.y + x);
}

void InterfaceAvance::OnCreateTree(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
                  Caption(_("Tree Control")).
                  Float().FloatingPosition(GetStartPosition()).
                  FloatingSize(wxSize(150,300)));
    m_mgr.Update();
}

void InterfaceAvance::OnRazIHM(wxCommandEvent& WXUNUSED(event))
{
wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
wxCommandEvent event;
OnRestorePerspective(event);
if (ongletParamImage==NULL)
	{
	ongletParamImage = OngletParamImage(pane);
	pane->AddPage(ongletParamImage, _("Image parameters") , false, page_bmp);
	}
if (ongletConvol==NULL)
	{
	ongletConvol = OngletConvolution(pane);
	pane->AddPage(ongletConvol, _("Convolution parameters") , false, page_bmp);
	}
if (ongletMorphologie==NULL)
	{
	ongletMorphologie = OngletMorphologie(pane);
	pane->AddPage(ongletMorphologie, _("Morphology parameters") , false, page_bmp);
	}

}


void InterfaceAvance::OnCreateNotebook(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreerChoixOperateur(), wxAuiPaneInfo().
                  Caption(_("Notebook")).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.Update();
}

void InterfaceAvance::OnCreateText(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateTextCtrl(), wxAuiPaneInfo().
                  Caption(_("Text Control")).
                  Float().FloatingPosition(GetStartPosition()));
    m_mgr.Update();
}

void InterfaceAvance::OnCreateSizeReport(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateSizeReportCtrl(), wxAuiPaneInfo().
                  Caption(_("Client Size Reporter")).
                  Float().FloatingPosition(GetStartPosition()).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.Update();
}

void InterfaceAvance::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void InterfaceAvance::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_("wxOpenCV \nImage processing using libraries wxWidgets, OpenCv and Plplot\n"), _("About wxOpenCV"), wxOK, this);
}

wxTextCtrl* InterfaceAvance::CreateTextCtrl(const wxString& ctrl_text)
{
    static int n = 0;

    wxString text;
    if ( !ctrl_text.empty() )
        text = ctrl_text;
    else
        text.Printf("", ++n);

    return new wxTextCtrl(this,wxID_ANY, text,
                          wxPoint(0,0), wxSize(150,90),
                          wxNO_BORDER | wxTE_MULTILINE);
}

wxPanel* InterfaceAvance::OngletParamImage(wxWindow* parent)
{
ImageInfoCV xx;

wxPanel* panel = new wxPanel( parent,wxID_ANY ,wxPoint(0,0),wxSize(400,400));
wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));

wxString  bord[]={_("Constant"),_("Symetric/limit"),_("Symetric/last pixel"), _("equal last pixel"),
		_("Periodic boundary")};
wxString  tailleRes[]={_("same"),_("16 bits signed"),_("float"), _("double"),
		_("Periodic boundary")};
int		valeurBord[]={cv::BORDER_CONSTANT,cv::BORDER_REFLECT,cv::BORDER_REFLECT_101,cv::BORDER_REPLICATE,
		cv::BORDER_WRAP};
new wxStaticText( panneauCtrl, -1, _("Border type"),wxPoint(10,20) );
wxListBox *frontiere=new wxListBox( panneauCtrl,LBBord,wxPoint(100,20),wxSize(-1,-1),5,bord);
frontiere->SetSelection(xx.FctImage());
new wxStaticText( panneauCtrl, -1, _("Depth of destination"),wxPoint(10,120) );
wxListBox *precision=new wxListBox( panneauCtrl,LBPrecision,wxPoint(100,120),wxSize(-1,-1),4,tailleRes);
precision->SetSelection(xx.FctImage());





return panel;
}



void InterfaceAvance::AfficheFiltre(wxCommandEvent&event)
{
wxWindowList& w=ongletConvol->GetChildren();
if (w.size()!=2)
	return;
Tableur *g=(Tableur *)w[0];
ImageInfoCV xx;
wxListBox *cb=(wxListBox*)event.GetEventObject();
int idFiltre=cb->GetSelection();
indFiltreAffiche = idFiltre;
xx.IndOpConvolution(indFiltreAffiche);

for (int i=0;i<xx.opnn[idFiltre]->cols;i++)
	{
	g->ShowCol(i);
	g->ShowRow(i);
	}

for (int i=xx.opnn[idFiltre]->cols;i<21;i++)
	{
	g->HideCol(i);
	g->HideRow(i);
	}

	for (int i=0;i<xx.opnn[idFiltre]->rows;i++)
		for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
			g->DefCellule(i,j,xx.opnn[idFiltre]->at<float>(i,j));
if (cb->GetStringSelection().Find(_("user"))!=wxNOT_FOUND)
	g->EnableEditing (true);
else
	g->EnableEditing (false);
g->AutoSize();

}


void InterfaceAvance::NouveauFiltre(wxGridEvent& event)
{
int l=event.GetRow(),c=event.GetCol();
wxListBox *cb=(wxListBox*)wxWindow::FindWindowById(LBnxn,this);

int idFiltre=indFiltreAffiche;
wxWindowList& w=ongletConvol->GetChildren();
if (w.size()!=2)
	return;
Tableur *g=(Tableur *)w[0];
ImageInfoCV xx;
double val;
if (event.GetString().ToDouble(&val))
	{
	xx.opnn[idFiltre]->at<float>(l,c)=val;
	((wxOsgApp *)osgApp)->SauverFichierConfig("conv",idFiltre);

	}
}

wxPanel* InterfaceAvance::OngletConvolution(wxWindow* parent)
{
ImageInfoCV xx;
wxString  mnxn[]={_("Mean"),_("Mean weighted"),_("x Difference"),_("y Difference"),_("Roberts x"),
_("Roberts y"),_("Prewitt x"),_("Prewitt y"),_("Sobel x"),_("Sobel y"),_("Frei Chen x"),
		_("Frei Chen y"),_("Laplacian 4 connex"),_("Laplacian 8 connex"),_("User 1"),_("User 2"),
		_("User 3"),_("User 4"),_("User 5"),_("Mean 5x5"),_("Mean weighted 5x5"),_("User 1 5x5"),_("User 2 5x5"),
		_("User 3 5x5"),_("User 45x5 "),_("Mean 7x7"),_("Mean weighted 7x7"),_("User 1 7x7"),_("User 2 7x7"),
		_("User 3 7x7"),_("User 4 7x7")};
	// add the wxPLplot
wxPanel* panel = new wxPanel( parent,wxID_ANY ,wxPoint(0,0),wxSize(400,400));

Tableur*	excel = new Tableur((wxFrame*)panel,20,20);
excel->SetDefaultCellBackgroundColour (wxColour(64,64,255));
excel->SetLabelBackgroundColour(wxColour(128,128,128));
excel->SetDefaultCellFont  ( wxFont(12,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD ) ) ;
indFiltreAffiche=0;
for (int i=0;i<3;i++)
	for (int j=0;j<3;j++)
		excel->DefCellule(i,j,xx.opnn[0]->at<float>(i,j));
excel->AutoSize();
for (int i=3;i<21;i++)
	{
	excel->HideCol(i);
	excel->HideRow(i);
	}
wxFlexGridSizer *box = new wxFlexGridSizer(2,1,0);
box->AddGrowableCol( 1 );
wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));

	wxFlexGridSizer *flex = new wxFlexGridSizer( 1, 2, 0, 0 );
	flex->AddGrowableCol( 1 );
	wxListBox *choix3x3=new wxListBox( panneauCtrl,LBnxn,wxDefaultPosition,wxSize(100,-1),30,mnxn);
	choix3x3->SetSelection(0);
    flex->Add( new wxStaticText( panneauCtrl, -1, _("Filter name") ), 1, wxALL|wxALIGN_CENTRE, 5 );
	flex->Add( choix3x3, 1, wxALL|wxALIGN_CENTRE, 5 );

   panneauCtrl->SetSizer( flex );



box->Add( panneauCtrl, 1, wxALL | wxEXPAND, 10 );
box->Add( excel, 0, wxALL | wxEXPAND, 10 );
  panel->SetSizer( box );
    box->Fit(panel);
    box->SetSizeHints(panel);
	SetSize( 640, 500 );  // set frame size
  SetSizeHints( 220, 150 );  // set minimum frame size

  wxString m_title=_("Histogram");
excel->Show();

return panel;
}

wxPanel* InterfaceAvance::OngletMorphologie(wxWindow* parent)
{
	ImageInfoCV xx;
	wxString  mnxn[]={_("Rectangle"),_("Cross"),_("Ellipse")};
	wxString  nomOp[]={ _("Operator 1"),_("Operator 2"),_("Operator 3"),_("Operator 4"),_("Operator 5"),
						_("Operator 6"),_("Operator 7"),_("Operator 8"),_("Operator 9"),_("Operator 10")};
	wxPanel* panel = new wxPanel( parent,wxID_ANY ,wxPoint(0,0),wxSize(400,400));
	wxFlexGridSizer *box = new wxFlexGridSizer(2,1,0);
	wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));

	box->AddGrowableCol( 1 );

	wxFlexGridSizer *flex = new wxFlexGridSizer( 2, 3, 0, 0 );
	wxListBox *lbMorph=new wxListBox( panneauCtrl,LBTY_MORPH,wxDefaultPosition,wxSize(100,-1),3,mnxn);
	wxListBox *lbNomMorph=new wxListBox( panneauCtrl,LBIND_MORPH,wxDefaultPosition,wxSize(100,-1),10,nomOp);
	lbMorph->SetSelection(0);
	flex->Add( new wxStaticText( panneauCtrl, -1, _("Pattern morphology") ), 1, wxALL|wxALIGN_CENTRE, 5 );
	flex->Add( lbMorph, 1, wxALL|wxALIGN_CENTRE, 5 );
	flex->Add( lbNomMorph, 2, wxALL|wxALIGN_CENTRE, 5 );

	flex->Add( new wxStaticText( panneauCtrl, -1, _("Operator size") ), 0, wxALL|wxALIGN_CENTRE, 5 );
	flex->Add( new wxSpinCtrl( panneauCtrl, ROUETAILLE_MORPH, "2", wxDefaultPosition, wxSize(100,-1),
			wxSP_ARROW_KEYS, 3, 32, 5 ), 0, wxALL|wxALIGN_CENTRE, 5 );

	panneauCtrl->SetSizer( flex );
	box->Add( panneauCtrl, 1, wxALL | wxEXPAND, 10 );
	box->Add(new wxWindow(panel,ID_OP_MORPH,wxPoint(0,0),wxSize(400,400)), 1, wxALL | wxEXPAND, 10 );
	panel->SetSizer( box );
	box->Fit(panel);
	box->SetSizeHints(panel);
	SetSize( 640, 500 );  // set frame size
	SetSizeHints( 220, 150 );  // set minimum frame size

	wxString m_title=_("Histogram");

	return panel;
}

void InterfaceAvance::DefPrecision(wxCommandEvent&event)
{
wxListBox *cb=(wxListBox*)wxWindow::FindWindowById(LBPrecision,this);
int typeResultat[]={-1,CV_16S,CV_32F,CV_64F};
if (event.GetEventObject()==cb)
	{
	ImageInfoCV xx;
	int ind=event.GetSelection();
	xx.DefTypeResultat(typeResultat[ind]);
	}
}

void InterfaceAvance::DefBord(wxCommandEvent&event)
{
wxListBox *cb=(wxListBox*)wxWindow::FindWindowById(LBBord,this);
int		valeurBord[]={cv::BORDER_CONSTANT,cv::BORDER_REFLECT,cv::BORDER_REFLECT_101,cv::BORDER_REPLICATE,
		cv::BORDER_WRAP};

if (event.GetEventObject()==cb)
	{
	ImageInfoCV xx;
	int ind=event.GetSelection();
	xx.DefFctImage(valeurBord[ind]);
	}
}

void InterfaceAvance::CtrlMorpho(wxCommandEvent&event)
{
wxSpinButton *roue1=(wxSpinButton*)wxWindow::FindWindowById(ROUETAILLE_MORPH,this);
wxListBox *cb2=(wxListBox*)wxWindow::FindWindowById(LBTY_MORPH,this);
wxListBox *cb3=(wxListBox*)wxWindow::FindWindowById(LBIND_MORPH,this);

if (event.GetEventObject()==cb3)
	{
	cb2->SetSelection(-1);
	int ind=event.GetSelection();
	AfficheMorpho(ind);
	}
else
	{
	int type =cb2->GetSelection();
	int ind =cb3->GetSelection();
	int taille=roue1->GetValue();
	ModifOpMorpho(type,taille,ind);
	((wxOsgApp *)osgApp)->SauverFichierConfig("morph",ind,type,taille);
	}
}


void InterfaceAvance::ModifOpMorpho(int type,int taille,int ind)
{
if (ind<0 || ind>9)
	return;
ImageInfoCV x;
if (x.opMorph[ind]!=NULL)
	{
	delete x.opMorph[ind];
	x.opMorph[ind]=NULL;
	}
cv::Mat element = cv::getStructuringElement( type,cv::Size( 2*taille + 1, 2*taille+1 ),cv::Point( taille, taille ) );

x.opMorph[ind] = new ImageInfoCV( 2*taille + 1, 2*taille+1,element.type());
*((cv::Mat *)x.opMorph[ind]) =element;
AfficheMorpho(ind);
}

void InterfaceAvance::TailleFiltre(wxSpinEvent& event)
{
	int taille=event.GetValue();
	wxListBox *cb2=(wxListBox*)wxWindow::FindWindowById(LBTY_MORPH,this);
	wxListBox *cb3=(wxListBox*)wxWindow::FindWindowById(LBIND_MORPH,this);
	int type =cb2->GetSelection();
	int ind =cb3->GetSelection();
	if (type==-1 || taille==-1)
		return;

	ModifOpMorpho(type,taille,ind);


}


void InterfaceAvance::AfficheMorpho(int ind)
{
if (ImageInfoCV::opMorph[ind]==NULL)
	{
	wxWindow *cb=(wxWindow*)wxWindow::FindWindowById(ID_OP_MORPH,this);
	wxClientDC h(cb);
	h.Clear();
	return;
	}
ImageInfoCV::opMorph[ind]->IndOpMorphologie(ind);
wxImage op(ImageInfoCV::opMorph[ind]->rows,ImageInfoCV::opMorph[ind]->cols);
for (int i=0;i<ImageInfoCV::opMorph[ind]->rows;i++)
	for (int j=0;j<ImageInfoCV::opMorph[ind]->cols;j++)
		{
		unsigned char c=ImageInfoCV::opMorph[ind]->at<unsigned char>(i,j);
		op.SetRGB(j,i,255*c,255*c,255*c);
		}
wxWindow *cb=(wxWindow*)wxWindow::FindWindowById(ID_OP_MORPH,this);
wxClientDC h(cb);
h.Clear();
wxImage		imageZoom=op.Scale(4*ImageInfoCV::opMorph[ind]->cols,4*ImageInfoCV::opMorph[ind]->rows);
wxBitmap b(imageZoom);
h.DrawBitmap(b,0,0);
}



wxTreeCtrl* InterfaceAvance::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxNO_BORDER);

    wxImageList* imglist = new wxImageList(16, 16, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot(_("wxAUI Project"), 0);
    wxArrayTreeItemIds items;



    items.Add(tree->AppendItem(root, "Item 1", 0));
    items.Add(tree->AppendItem(root, "Item 2", 0));
    items.Add(tree->AppendItem(root, "Item 3", 0));
    items.Add(tree->AppendItem(root, "Item 4", 0));
    items.Add(tree->AppendItem(root, "Item 5", 0));


    int i, count;
    for (i = 0, count = items.Count(); i < count; ++i)
    {
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, "Subitem 1", 1);
        tree->AppendItem(id, "Subitem 2", 1);
        tree->AppendItem(id, "Subitem 3", 1);
        tree->AppendItem(id, "Subitem 4", 1);
        tree->AppendItem(id, "Subitem 5", 1);
    }


    tree->Expand(root);

    return tree;
}

wxSizeReportCtrl* InterfaceAvance::CreateSizeReportCtrl(int width, int height)
{
    wxSizeReportCtrl* ctrl = new wxSizeReportCtrl(this, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(width, height), &m_mgr);
    return ctrl;
}


wxAuiNotebook* InterfaceAvance::CreerChoixOperateur()
{
// create the notebook off-window to avoid flicker
wxSize client_size = GetClientSize();

wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
                                wxPoint(client_size.x, client_size.y),
                                wxSize(430,200),
                                m_notebook_style);
ctrl->Freeze();

wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
if (ongletConvol==NULL)
	{
	ongletConvol = OngletConvolution(ctrl);
	ctrl->AddPage(ongletConvol, _("Convolution filter"), false, page_bmp);
	}
if (ongletMorphologie==NULL)
	{
	ongletMorphologie = OngletMorphologie(ctrl);
	ctrl->AddPage(ongletMorphologie, _("Morphology filter") , false, page_bmp);
	}
if (ongletParamImage==NULL)
	{
	ongletParamImage = OngletParamImage(ctrl);
	ctrl->AddPage(ongletParamImage, _("Operator parameters") , false, page_bmp);
	}

ctrl->Thaw();
return ctrl;
}


void InterfaceAvance::InstallationBarreMenu()
{
	// create menu
    wxMenuBar* mb = new wxMenuBar;

    wxMenu* file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    file_menu->Append(wxID_OPEN);
    wxMenu* param_menu = new wxMenu;
    param_menu->Append(ID_RAZIHM, _("Reset Windows"));

    mb->Append(file_menu, _("&File"));
    mb->Append(param_menu, _("&parameters"));

    SetMenuBar(mb);
}

void InterfaceAvance::InstallationbarreOutils(int indBarre)
{
    wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
//    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);
	wxAuiToolBar* tb;
	wxBitmap tb_bmp;
   // create some toolbars
	enum
		{
			Tool_addition,Tool_soustraction,Tool_multiplication,Tool_division,Tool_oulogique,Tool_etlogique,Tool_ouexclulogique,Tool_negationlogique,
			Tool_erosion,Tool_dilatation,Tool_ouverture,Tool_fermeture,Tool_gradmorph,Tool_chaphautblanc,Tool_chaphautnoir,
			Tool_gradient_mod,Tool_gradient_x,Tool_gradient_y,Tool_laplacien,
			Tool_canny,Tool_cornerharris,Tool_goodfeature,Tool_houghcircle,Tool_houghline,Tool_houghlinep,
			Tool_fft,Tool_ifft,
			Tool_contour,Tool_seuillage,Tool_seuillageada,Tool_distancediscrete,Tool_squelette,Tool_voronoi,Tool_cmpconnexe,Tool_statconnexe,
			Tool_LisMoy,Tool_LisMed,Tool_LisGau,Tool_flou,Tool_convolution,
			Tool_fusionplan,Tool_separationplan,Tool_rgbluminance,
			Tool_pyrflotoptique, Tool_calcflotoptique, Tool_calcflotoptiquefarner, Tool_estimtransfo, Tool_majmvt, Tool_phasecorrel,
			Tool_Max
		};
	switch(indBarre)
	{
	case 0: // Opération arithmétique
		{
		wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(addition);
			INIT_TOOL_BMP(soustraction);
			INIT_TOOL_BMP(multiplication);
			INIT_TOOL_BMP(division);
			INIT_TOOL_BMP(etlogique);
			INIT_TOOL_BMP(oulogique);
			INIT_TOOL_BMP(ouexclulogique);
			INIT_TOOL_BMP(negationlogique);
			INIT_TOOL_BMP(division);
			INIT_TOOL_BMP(division);
			INIT_TOOL_BMP(division);

        if (toolBarBitmaps[Tool_addition].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                        /*  pour personnaliser wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_ADDITION, "", toolBarBitmaps[Tool_addition], _("Add 2 images"), wxITEM_CHECK);
		tb->AddTool(ID_SOUSTRACTION, "", toolBarBitmaps[Tool_soustraction], _("Difference between 2 images"));
		tb->AddTool(ID_MULTIPLICATION, "", toolBarBitmaps[Tool_multiplication], _("product"));
		tb->AddTool(ID_DIVISION, "", toolBarBitmaps[Tool_division], _("Divide"));
		tb->AddTool(ID_ET_LOGIQUE, "", toolBarBitmaps[Tool_division], _("logical and"));
		tb->AddTool(ID_OU_LOGIQUE, "", toolBarBitmaps[Tool_division], _("logical or"));
		tb->AddTool(ID_OUEXCLU_LOGIQUE, "", toolBarBitmaps[Tool_division], _("exclusive or"));
		tb->AddTool(ID_NEGATION, "", toolBarBitmaps[Tool_division], _("Not"));
		wxColour c(255, 0, 0);;
		tb->SetBackgroundColour(c);
		//tb->SetCustomOverflowItems(prepend_items, append_items);
//		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Arithmetic operation").Caption("Arithmetic operation").
					  ToolbarPane().Top().Row(1));
		}
		break;
	case 1 : // Opérateur morphologique
		{
		wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(erosion);
			INIT_TOOL_BMP(dilatation);
			INIT_TOOL_BMP(ouverture);
			INIT_TOOL_BMP(fermeture);
			INIT_TOOL_BMP(gradmorph);
			INIT_TOOL_BMP(chaphautblanc);
			INIT_TOOL_BMP(chaphautnoir);

        if (toolBarBitmaps[Tool_erosion].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_EROSION, "", toolBarBitmaps[Tool_erosion], _("Erode"));
		tb->AddTool(ID_DILATATION, "", toolBarBitmaps[Tool_dilatation], _("Dilate"));
		tb->AddTool(ID_OUVERTURE, "", toolBarBitmaps[Tool_ouverture], _("Openning"));
		tb->AddTool(ID_FERMETURE,"", toolBarBitmaps[Tool_fermeture], _("Closing"));
		tb->AddTool(ID_CHAPHAUTBL, "", toolBarBitmaps[Tool_chaphautblanc], _("Top hat"));
		tb->AddTool(ID_CHAPHAUTNO, "", toolBarBitmaps[Tool_chaphautnoir], _("Black hat"));
		tb->AddTool(ID_GRADMORPH, "", toolBarBitmaps[Tool_gradmorph], _("Morph. gradient"));
//		tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Morphologic operation").Caption("Morphologic operation").
					  ToolbarPane().Top().Row(3));
		}
		break;
	case 2: // Opérateur Spectrale et d'échelle
		{

			wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(fft);
			INIT_TOOL_BMP(ifft);
//			toolBarBitmaps[Tool_fft]= wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(64,64));

        if (toolBarBitmaps[Tool_fft].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_FFT, _("FFT"), toolBarBitmaps[Tool_fft], _("FFT"));
		tb->AddTool(ID_IFFT, _("inverse FFT"), toolBarBitmaps[Tool_ifft], _("Inverse FFT"));
//		tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Scale/Frequency").Caption("Scale/Frequency").
					  ToolbarPane().Top().Row(5));
		}
		break;
	case 3 :// Opérateur lissage
		{

			wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(gradient_mod);
			INIT_TOOL_BMP(gradient_x);
			INIT_TOOL_BMP(gradient_y);
			INIT_TOOL_BMP(laplacien);
			INIT_TOOL_BMP(LisMoy);
			INIT_TOOL_BMP(LisMed);
			INIT_TOOL_BMP(LisGau);
			INIT_TOOL_BMP(flou);
			INIT_TOOL_BMP(convolution);
//			toolBarBitmaps[Tool_fft]= wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(64,64));

        if (toolBarBitmaps[Tool_LisMoy].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_CONVOLUTION, "", toolBarBitmaps[Tool_convolution], _("Convolution"));
		tb->AddTool(ID_GRADIENT_MOD, "", toolBarBitmaps[Tool_gradient_mod], _("Gradient modulus"));
		tb->AddTool(ID_GRADIENT_X, "", toolBarBitmaps[Tool_gradient_x], _("Gradient X"));
		tb->AddTool(ID_GRADIENT_Y, "", toolBarBitmaps[Tool_gradient_y], _("Gradient Y"));
		tb->AddTool(ID_LAPLACIEN, "", toolBarBitmaps[Tool_laplacien], _("Laplacian"));
		tb->AddTool(ID_LISMOY, "", toolBarBitmaps[Tool_LisMoy], _("Blur (mean)"));
		tb->AddTool(ID_LISMED, "", toolBarBitmaps[Tool_LisMed], _("Blur (median filter)"));
		tb->AddTool(ID_LISGAU, "", toolBarBitmaps[Tool_LisGau], _("Blur (gaussian)"));
//		tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Filter").Caption("Filter").
					  ToolbarPane().Top().Row(2));
		}
		break;
	case 4:// Opérateur Vidéo
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
											 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
		tb->SetToolBitmapSize(wxSize(48,48));
		tb->AddTool(ID_VIDEO_8_UC3, "", wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE),_("Video RGB 24bits"));
		tb->AddTool(ID_VIDEO_32_FC3, "", wxArtProvider::GetBitmap(wxART_HARDDISK),_("Video RGB float"));
		tb->AddTool(ID_VIDEO_FLUX, "", wxArtProvider::GetBitmap(wxART_NORMAL_FILE),_("Video file"));
		tb->AddSeparator();
		tb->Realize();
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name("tbVideo").Caption(_("Video Toolbar")).
					  ToolbarPane().Left().
					  GripperTop());
		break;
	case 5 : // Outils opération, seuillage, contour, canny, composante connexe et stat
		{

			wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(contour);
			INIT_TOOL_BMP(seuillage);
			INIT_TOOL_BMP(seuillageada);
			INIT_TOOL_BMP(distancediscrete);
			INIT_TOOL_BMP(squelette);
			INIT_TOOL_BMP(voronoi);
			INIT_TOOL_BMP(cmpconnexe);
			INIT_TOOL_BMP(statconnexe);
			toolBarBitmaps[Tool_fft]= wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(64,64));

        if (toolBarBitmaps[Tool_contour].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_THRESHOLD, wxEmptyString, toolBarBitmaps[Tool_seuillage], _("Thereshold"));
		tb->AddTool(ID_ADATHRESHOLD, wxEmptyString, toolBarBitmaps[Tool_seuillageada], _("Adaptive Thereshold"));
		tb->AddTool(ID_PARTAGE_EAUX, wxEmptyString, toolBarBitmaps[Tool_statconnexe], _("Watershed"));
		tb->AddTool(ID_COMPCONNEXE, wxEmptyString, toolBarBitmaps[Tool_cmpconnexe], _("Connected component"));
		tb->AddTool(ID_CONTOUR, wxEmptyString, toolBarBitmaps[Tool_contour], _("Contour"));
		tb->AddTool(ID_SQUELETTE, wxEmptyString, toolBarBitmaps[Tool_squelette], _("Skeleton"));
		tb->AddTool(ID_DISTANCEDISCRETE, wxEmptyString, toolBarBitmaps[Tool_distancediscrete], _("Transform distance"));
		tb->AddTool(ID_VORONOI, wxEmptyString, toolBarBitmaps[Tool_voronoi], _("Voronoi"));
		//tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("binarize").Caption("binarize").
					  ToolbarPane().Top().Row(4));
		}
	break;
	case 6 : // Ouverture, fermeture de fichiers
		{
		enum
		{
			Tool_new,
			Tool_open,
			Tool_save,
			Tool_copy,
			Tool_cut,
			Tool_paste,
			Tool_print,
			Tool_help,
			Tool_Max
		};

		wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(new);
			INIT_TOOL_BMP(open);
			INIT_TOOL_BMP(save);
			INIT_TOOL_BMP(copy);
			INIT_TOOL_BMP(cut);
			INIT_TOOL_BMP(paste);
			INIT_TOOL_BMP(print);
			INIT_TOOL_BMP(help);
	   int w = toolBarBitmaps[Tool_new].GetWidth(),
			h = toolBarBitmaps[Tool_new].GetHeight();

		// this call is actually unnecessary as the toolbar will adjust its tools
		// size to fit the biggest icon used anyhow but it doesn't hurt neither
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         wxAUI_TB_OVERFLOW |
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbFichier= tb;

		tb->SetToolBitmapSize(wxSize(w, h));

		tb->AddTool(wxID_OPEN, _("Open"),toolBarBitmaps[Tool_open]);


		tb->AddTool(wxID_SAVE, _("Save"), toolBarBitmaps[Tool_save]);
		tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name(_("tb5")).Caption(_("Big Toolbar")).
					  ToolbarPane().Top());
		}
		break;
	case 7 : // Outils couleur, normalisation
		{

			wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(fusionplan);
			INIT_TOOL_BMP(separationplan);
			INIT_TOOL_BMP(rgbluminance);
			toolBarBitmaps[Tool_fft]= wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(64,64));

        if (toolBarBitmaps[Tool_fusionplan].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_FUSIONPLAN, wxEmptyString, toolBarBitmaps[Tool_fusionplan], _("Merge color plan"));
		tb->AddTool(ID_SEPARATIONPLAN, wxEmptyString, toolBarBitmaps[Tool_separationplan], _("split color plan"));
		tb->AddTool(ID_RGBLUMINANCE, wxEmptyString, toolBarBitmaps[Tool_rgbluminance], _("RGB to gray"));
		//tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Color").Caption("Color").
					  ToolbarPane().Top().Row(6));
		}
	break;
	case 8:
		{
		wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(canny);
			INIT_TOOL_BMP(cornerharris);
			INIT_TOOL_BMP(goodfeature);
			INIT_TOOL_BMP(houghcircle);
			INIT_TOOL_BMP(houghline);
			INIT_TOOL_BMP(houghlinep);

        if (toolBarBitmaps[Tool_canny].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_CANNY, wxEmptyString, toolBarBitmaps[Tool_canny], _("Canny edge detector"));
		tb->AddTool(ID_CORNERHARRIS, wxEmptyString, toolBarBitmaps[Tool_cornerharris], _("Harris edge detector"));
		tb->AddTool(ID_GOODFEATURE, wxEmptyString, toolBarBitmaps[Tool_goodfeature], _("Very good features"));
		tb->AddTool(ID_HOUGHCIRCLE, wxEmptyString, toolBarBitmaps[Tool_houghcircle], _("Hough circle"));
		tb->AddTool(ID_HOUGHLINE, wxEmptyString, toolBarBitmaps[Tool_houghline], _("Hough line"));
		tb->AddTool(ID_HOUGHLINEP, wxEmptyString, toolBarBitmaps[Tool_houghlinep], _("hough line proba."));
		//tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("feature").Caption("Feature").
					  ToolbarPane().Top().Row(4));
		}
		break;
	case 9:
		{
		wxBitmap toolBarBitmaps[Tool_Max];

		#if USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
		#else // !USE_XPM_BITMAPS
			#define INIT_TOOL_BMP(bmp) \
				toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
		#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

			INIT_TOOL_BMP(pyrflotoptique);
			INIT_TOOL_BMP(calcflotoptique);
			INIT_TOOL_BMP(calcflotoptiquefarner);
			INIT_TOOL_BMP(estimtransfo);
			INIT_TOOL_BMP(majmvt);
			INIT_TOOL_BMP(phasecorrel);

        if (toolBarBitmaps[Tool_pyrflotoptique].GetHeight()==0)
            wxMessageBox("Probleme","pb");
		tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxAUI_TB_DEFAULT_STYLE |
                                         /*wxAUI_TB_OVERFLOW |*/
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
		tbOperation= tb;
//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
        tb->SetMargins(1,1);
		tb->AddTool(ID_PYRFLOTOPTIQUE, wxEmptyString, toolBarBitmaps[Tool_pyrflotoptique], _("Build pyramid optical flow"));
		tb->AddTool(ID_CALCFLOTOPTIQUE, wxEmptyString, toolBarBitmaps[Tool_calcflotoptique], _("Calculate optical flow"));
		tb->AddTool(ID_CALCFLOTOPTIQUEFARNER, wxEmptyString, toolBarBitmaps[Tool_calcflotoptiquefarner], _("Calculate optical flow(farnerback)"));
		tb->AddTool(ID_ESTIM_TRANS, wxEmptyString, toolBarBitmaps[Tool_estimtransfo], _("Estimate rigid transform"));
		tb->AddTool(ID_MAJ_MVT, wxEmptyString, toolBarBitmaps[Tool_majmvt], _("Update motion history"));
		tb->AddTool(ID_PHASE_CORRELATE, wxEmptyString, toolBarBitmaps[Tool_majmvt], _("Phase correlation"));
		//tb->SetCustomOverflowItems(prepend_items, append_items);
		tb->Realize();
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Optical Flow").Caption("Optical Flow").
					  ToolbarPane().Top().Row(5));
		}
		break;
	}
	    // add the toolbars to the manager
}


void InterfaceAvance::Ouvrir(wxCommandEvent& event)
{
	((wxOsgApp*)osgApp)->Ouvrir(event);

}

void InterfaceAvance::Video(wxCommandEvent& event)
{
if (event.GetId()==ID_VIDEO_32_FC3)
	((wxOsgApp*)osgApp)->Video(event,32);
if (event.GetId()==ID_VIDEO_8_UC3)
	((wxOsgApp*)osgApp)->Video(event,8);
if (event.GetId()==ID_VIDEO_FLUX)
	((wxOsgApp*)osgApp)->Video(event,0);

}



void InterfaceAvance::OnQuit(wxCommandEvent& WXUNUSED(event))
{
Close(true);
}

void InterfaceAvance::SelectOperation(wxCommandEvent& evt)
{
wxString s;
switch(evt.GetId()){
case ID_RGBLUMINANCE:
    s="cvtcolor";
	break;
case ID_FUSIONPLAN:
    s="merge";
	break;
case ID_SEPARATIONPLAN:
    s="split";
	break;
case ID_ADDITION:
    s="add";
	break;
case ID_SOUSTRACTION:
	s="substract";
	break;
case ID_DIVISION:
	s="divide";
	break;
case ID_MULTIPLICATION:
	s="multiply";
	break;
case ID_CONVOLUTION:
	{
	ImageInfoCV x;

	if (x.IndOpConvolution()<0 || x.opnn[x.IndOpConvolution()]==NULL)
		{
		wxMessageBox(_("You must defined convolution operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="filter2d";
	}
	break;
case ID_EROSION:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="erode";
	}
	break;
case ID_DILATATION:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="dilate";
	}
	break;
case ID_OUVERTURE:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="openning";
	}
	break;
case ID_FERMETURE:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="closing";
	}
	break;
case ID_CHAPHAUTBL:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="tophat";
	}
	break;
case ID_CHAPHAUTNO:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="blackhat";
	}
	break;
case ID_GRADMORPH:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	s="morph_gradient";
	}
	break;
case ID_GRADIENT_X:
	s="scharr_x";
	break;
case ID_GRADIENT_Y:
	s="scharr_y";
	break;
case ID_GRADIENT_MOD:
	s="scharr_mod";
	break;
case ID_LAPLACIEN:
	s="laplacian";
	break;
case ID_CANNY:
	s="canny";
	break;
case ID_THRESHOLD:
	s="threshold";
	break;
case ID_ADATHRESHOLD:
	s="adaptivethreshold";
	break;
case ID_CONTOUR:
	s="contour";
	break;
case ID_PARTAGE_EAUX:
	s="watershed";
	break;
case ID_COMPCONNEXE:
	s="connectedcomponents";
	break;
case ID_FFT:
	s="FFT";
	break;
case ID_IFFT:
	s="IFFT";
	break;
case ID_LISMOY:
	s= "blur";
	break;
case ID_LISGAU:
	s="gaussianblur";

	break;
case ID_LISMED:
	s="medianblur";
	break;
case ID_SQUELETTE:
	s="medianaxis";
	break;
case ID_DISTANCEDISCRETE:
case ID_VORONOI:
	s="distancetransform";
	break;
case ID_CORNERHARRIS:
	s="cornerharris";
	break;
case ID_GOODFEATURE:
	s="goodfeaturestotrack";
	break;
case ID_HOUGHCIRCLE:
	s="houghcircles";
	break;
case ID_HOUGHLINE:
	s="houghlines";
	break;
case ID_HOUGHLINEP:
	s="houghlinesp";
	break;
case ID_PYRFLOTOPTIQUE:
	s="buildopticalflowpyramid";
	break;
case ID_CALCFLOTOPTIQUE:
	s="calcopticalflowpyrlk";
	break;
case ID_CALCFLOTOPTIQUEFARNER:
	s="calcopticalflowfarneback";
	break;
case ID_ESTIM_TRANS:
	s = "estimaterigidtransform";
	break;
case ID_PHASE_CORRELATE:
	s = "phasecorrelate";
	break;
case ID_MAJ_MVT:
	s="updatemotionhistory";
	break;
case 	ID_OU_LOGIQUE:
	s = "bitwise-or";
	break;
case 	ID_ET_LOGIQUE:
	s = "bitwise-and";
	break;
case 	ID_OUEXCLU_LOGIQUE:
	s = "bitwise-xor";
	break;
case 	ID_NEGATION:
	s = "bitwise-not";
	break;

	}
((wxOsgApp*)osgApp)->DefOperateurImage(s);
((wxOsgApp*)osgApp)->DefPointeurSouris(1,1);

}


