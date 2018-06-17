void GrapheOperation::GrapheOperation()
{


    if (!osgApp->GrapheOperation())
        return;

    GrapheOperation *fenOpe = new GrapheOperation(this, origineImage.nomOperation, wxPoint(530, 0), wxSize(430, 570), this->osgApp);
    fenOpe->DefFenMere(this);
    fenOpe->DefOSGApp(this->osgApp);
    fenOpe->Show(true);
    fenOpe->Refresh(true);
    osgApp->FenetreOperande(fenOpe);

    //fenAlgo->SendSizeEvent   ();
}
git add 



#define ID_SAUVER_SEQ_CONFIG 1000
#define ID_NOM_SEQUENCE 1001
#define ID_SAUVER_SEQ_XML 1002

GrapheOperation::GrapheOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, wxOsgApp *osg, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCAPTION)
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
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(classeur, 1, wxGROW | wxEXPAND, 10);
    wxBoxSizer *partieBasse = new wxBoxSizer(wxHORIZONTAL);
    panneau = new wxPanel(this, wxID_ANY);
    wxColour fond(*wxLIGHT_GREY);
    fond.Set(fond.Red(), 255, fond.Blue());
    panneau->SetBackgroundColour(fond);
    topsizer->Add(panneau, 1, wxGROW | wxEXPAND, 10);

    wxButton *button1 = new wxButton(panneau, ID_SAUVER_SEQ_CONFIG, _("Save all step as Macro in cfg "));
    wxButton *button2 = new wxButton(panneau, ID_SAUVER_SEQ_XML, _("Save all step as Macro in xml"));
    wxStaticText *st = new wxStaticText(panneau, -1, _("         Name "));
    wxString nomMacro;
    nomMacro.Printf("Macro %d", osg->NumSeqOpe());
    wxTextCtrl *caseNomMacro = new wxTextCtrl(panneau, ID_NOM_SEQUENCE, nomMacro);
    partieBasse->Add(button1, 0, wxALIGN_CENTER_VERTICAL | wxALL);
    partieBasse->Add(button2, 0, wxALIGN_CENTER_VERTICAL | wxALL);
    partieBasse->Add(st, 0, wxALIGN_CENTER_VERTICAL | wxALL);
    partieBasse->Add(caseNomMacro, 0, wxALIGN_CENTER_VERTICAL | wxALL);
    panneau->SetSizer(partieBasse);
    Bind(wxEVT_SPINCTRLDOUBLE, &FenetreOperande::OnSpinReel, this);
    Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &FenetreOperande::ComboBox, this);
    SetSizerAndFit(topsizer);
    Show(true);
}
