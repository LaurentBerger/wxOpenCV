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
#include "FenetreAlgo.h"


#ifdef __WINDOWS__
    #define USE_XPM_BITMAPS 1
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
    ID_ADDITION_PONDEREE,
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
    ID_GRADIENT_DERICHE_MOD,
    ID_GRADIENT_DERICHE_X,
    ID_GRADIENT_DERICHE_Y,
    ID_GRADIENT_PAILLOU_MOD,
    ID_GRADIENT_PAILLOU_X,
    ID_GRADIENT_PAILLOU_Y,
	ID_LAPLACIEN,
 	ID_LISMOY,
	ID_LISGAU,
    ID_INPAINT,
	ID_LISMED,
	ID_THRESHOLD,
	ID_ADATHRESHOLD,
    ID_CLAHE,
    ID_DISTANCEDISCRETE,
    ID_SQUELETTE,
    ID_VORONOI,
	ID_CONTOUR,
	ID_COMPCONNEXE,
    ID_CONVEX_HULL, ///
    ID_CONVEX_DEFAUT,////
    ID_APPROX_POLY,///
    ID_APPROX_CHAIN,///

    ID_PARTAGE_EAUX,
    ID_GRABCUT,
    ID_CALCBACKPROJECT,
    ID_KMEANS,

    ID_SUPERPIXELLSC,
    ID_SUPERPIXELSEEDS,
    ID_SUPERPIXELSLIC,

    ID_CANNY,
	ID_CORNERHARRIS,
	ID_GOODFEATURE,
	ID_HOUGHCIRCLE,
	ID_HOUGHLINE,
	ID_HOUGHLINEP,
	ID_PYRFLOTOPTIQUE,
	ID_CALCFLOTOPTIQUE,
	ID_CALCFLOTOPTIQUEFARNER,
	ID_PHASE_CORRELATE,
	ID_MAJ_MVT,
	ID_GRAD_MVT,
	ID_SEG_MVT,




	ID_FFT,
	ID_IFFT,
	ID_FUSIONPLAN,
	ID_SEPARATIONPLAN,
	ID_RGBLUMINANCE,
    ID_CONVERTTYPE,
	ID_ROTATION_AFFINE,
	ID_NOUVELLE_DIMENSION,
	ID_WHITTAKER,
	ID_LOGPOLAR,
	ID_LINEARPOLAR,
	ID_UNDISTORT,

	ID_FOND_MOG,
	ID_FOND_MOG2,
	ID_FOND_KNN,
	ID_FOND_MGM,
	ID_DES_ORB,
	ID_DES_BRISK,
    ID_DES_AKAZE,
    ID_DES_KAZE,
    ID_DES_AGAST,
    ID_DES_MSER,
    ID_DES_BLOB,
    ID_DES_SIFT,
    ID_DES_SURF,
	ID_DES_MATCH,
	ID_HOMOGRAPHY,
    ID_WARPPERPESCTIVE,

    ID_DES_PANOFEATURE,
    ID_DES_PANORESIZE,
    ID_DES_PANOMATCH,
    ID_DES_PANOBESTMATCH,
    ID_DES_PANOHOMOGRAPHY,
	ID_PANO_WRAPPER,
	ID_PANO_EXPO,
	ID_PANO_COMPO,




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
#include "FichierIcon.h"

#endif // USE_XPM_BITMAPS


Commande::Commande(int a,const char *b[], wxString c, wxString d, int e) :idEvt(a), chaineAide(c), chaineOperation(d), numBarreoutils(e)
{
#if USE_XPM_BITMAPS
#define INIT_TOOL_BMP(bmp) \
	toolBarBitmaps[Tool_##bmp] = wxBitmap(b)
#else // !USE_XPM_BITMAPS
#define INIT_TOOL_BMP(bmp) \
	toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS
//wxMessageBox(b,"",wxOK,NULL);
	bitmap = wxBitmap(b);
};



void InterfaceAvance::InitCommande()
{
#define CONSTRUCTEUR_CMD(var,ID,var_icon,aide,fct,num) var[ID]=Commande(ID,var_icon, aide, fct,num)
bouton[ID_ADDITION] = Commande(ID_ADDITION, addition_xpm, _("Add 2 images"), "add", 0);
    CONSTRUCTEUR_CMD(bouton, ID_ADDITION,addition_xpm, _("Add 2 images"), "add", 0);
    CONSTRUCTEUR_CMD(bouton, ID_SOUSTRACTION, soustraction_xpm, _("Difference between 2 images"), "subtract", 0);
    CONSTRUCTEUR_CMD(bouton, ID_MULTIPLICATION, multiplication_xpm,_("product"), "multiply", 0);
    CONSTRUCTEUR_CMD(bouton, ID_ADDITION_PONDEREE, additionponderee_xpm,_("Add weighted"), "addweighted", 0);
    CONSTRUCTEUR_CMD(bouton, ID_DIVISION, division_xpm,_("Divide"), "divide", 0);
    CONSTRUCTEUR_CMD(bouton, ID_ET_LOGIQUE, etlogique_xpm,_("logical and"), "bitwise-and", 0);
    CONSTRUCTEUR_CMD(bouton, ID_OU_LOGIQUE, oulogique_xpm,_("logical or"), "bitwise-or", 0);
    CONSTRUCTEUR_CMD(bouton, ID_OUEXCLU_LOGIQUE, division_xpm, _("exclusive or"), "bitwise-xor", 0);
    CONSTRUCTEUR_CMD(bouton, ID_NEGATION, nonlogique_xpm, _("Not"), "bitwise-not", 0);

    CONSTRUCTEUR_CMD(bouton, ID_EROSION, erosion_xpm, _("Erode"), "erode", 1);
    CONSTRUCTEUR_CMD(bouton, ID_DILATATION, dilatation_xpm, _("Dilate"), "dilate", 1);
    CONSTRUCTEUR_CMD(bouton, ID_OUVERTURE, ouverture_xpm, _("Openning"), "openning", 1);
    CONSTRUCTEUR_CMD(bouton, ID_FERMETURE, fermeture_xpm, _("closing"), "closing", 1);
    CONSTRUCTEUR_CMD(bouton, ID_CHAPHAUTBL, chaphautblanc_xpm, _("Top hat"), "tophat", 1);
    CONSTRUCTEUR_CMD(bouton, ID_CHAPHAUTNO, chaphautnoir_xpm,_("Black hat"), "blackhat", 1);
    CONSTRUCTEUR_CMD(bouton, ID_GRADMORPH, gradmorph_xpm,_("Morph. gradient"), "morph_gradient", 1);

    CONSTRUCTEUR_CMD(bouton, ID_FFT, fft_xpm, _("FFT"), "FFT", 2);
    CONSTRUCTEUR_CMD(bouton, ID_IFFT, ifft_xpm, _("inverse FFT"), "IFFT", 2);

    CONSTRUCTEUR_CMD(bouton, ID_CONVOLUTION, convolution_xpm, _("Convolution"), "filter2d", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_MOD, gradient_mod_xpm,_("Gradient modulus"), "scharr_mod", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_X, gradient_x_xpm, _("Gradient X"), "scharr_x", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_Y, gradient_y_xpm, _("Gradient Y"), "scharr_y", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_PAILLOU_MOD, gradient_modDeriche_xpm,_("Gradient modulus(PAILLOU)"), "paillou_mod", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_PAILLOU_X, gradient_xDeriche_xpm, _("Gradient X(PAILLOU)"), "paillou_x", 3);
    CONSTRUCTEUR_CMD(bouton, ID_GRADIENT_PAILLOU_Y, gradient_yDeriche_xpm, _("Gradient Y(PAILLOU)"), "paillou_y", 3);
    CONSTRUCTEUR_CMD(bouton, ID_LAPLACIEN, laplacien_xpm, _("Laplacian"), "laplacian", 3);
    CONSTRUCTEUR_CMD(bouton, ID_LISMOY, LisMoy_xpm, _("Blur (mean)"), "blur", 3);
    CONSTRUCTEUR_CMD(bouton, ID_LISMED, LisMed_xpm, _("Blur (median filter)"), "medianblur", 3);
    CONSTRUCTEUR_CMD(bouton, ID_LISGAU, LisGau_xpm,_("Blur (gaussian)"), "gaussianblur", 3);
    CONSTRUCTEUR_CMD(bouton, ID_INPAINT, inpaint_xpm,_("Inpaint"), "inpaint", 3);

    CONSTRUCTEUR_CMD(bouton, ID_VIDEO_8_UC3, camera_xpm, _("Video RGB 24bits"), " ", 4);
    CONSTRUCTEUR_CMD(bouton, ID_VIDEO_32_FC3, camerafloat_xpm, _("Video RGB float"), " ", 4);
    CONSTRUCTEUR_CMD(bouton, ID_VIDEO_FLUX, camera_xpm, _("Video file"), " ", 4);

    CONSTRUCTEUR_CMD(bouton, ID_THRESHOLD, seuillage_xpm, _("Thereshold"), "threshold", 5);
    CONSTRUCTEUR_CMD(bouton, ID_ADATHRESHOLD, seuillageada_xpm, _("Adaptive Thereshold"), "adaptivethreshold", 5);
    CONSTRUCTEUR_CMD(bouton, ID_CLAHE, clahe_xpm, _("Clahe"), "clahe", 5);

    CONSTRUCTEUR_CMD(bouton, ID_PARTAGE_EAUX, watershed_xpm, _("Watershed"), "watershed", 5);
    CONSTRUCTEUR_CMD(bouton, ID_GRABCUT, grabcut_xpm, _("Grabcut"), "grabcut", 5);
    CONSTRUCTEUR_CMD(bouton, ID_CALCBACKPROJECT, backproject_xpm, _("Calcbackproject"), "calcbackproject", 5);
    CONSTRUCTEUR_CMD(bouton, ID_KMEANS, kmeans_xpm, _("K-Means"), "kmean", 5);
    CONSTRUCTEUR_CMD(bouton, ID_SUPERPIXELLSC, superpixelslc_xpm, _("SuperpixelLSC"), "SuperpixelLSC", 5);
    CONSTRUCTEUR_CMD(bouton, ID_SUPERPIXELSEEDS, superpixelseeds_xpm, _("SuperpixelSEEDS"), "SuperpixelSEEDS", 5);
    CONSTRUCTEUR_CMD(bouton, ID_SUPERPIXELSLIC, superpixelslic_xpm, _("SuperpixelSLIC"), "SuperpixelSLIC", 5);



    CONSTRUCTEUR_CMD(bouton, ID_COMPCONNEXE, cmpconnexe_xpm,_("Connected component"), "connectedcomponents", 5);
    CONSTRUCTEUR_CMD(bouton, ID_CONTOUR, contour_xpm,_("Contour"), "contour", 5);
    CONSTRUCTEUR_CMD(bouton, ID_SQUELETTE, squelette_xpm,_("Skeleton"), "medianaxis", 5);
    CONSTRUCTEUR_CMD(bouton, ID_DISTANCEDISCRETE, distancediscrete_xpm, _("Transform distance"), "distancetransform", 5);
    CONSTRUCTEUR_CMD(bouton, ID_VORONOI, voronoi_xpm,_("Voronoi"), " ", 5);
    CONSTRUCTEUR_CMD(bouton, ID_CONVEX_HULL, convexhull_xpm,_("Finds the convex hull of a point set."), "convexhull", 5);
    CONSTRUCTEUR_CMD(bouton, ID_CONVEX_DEFAUT, convexdefaut_xpm,_("Finds the convexity defects of a contour."), "convexitydefects", 5);
    CONSTRUCTEUR_CMD(bouton, ID_APPROX_POLY, approxpoly_xpm,_("Approximates a polygonal curve(s)"), "approxpolydp", 5);
    CONSTRUCTEUR_CMD(bouton, ID_APPROX_CHAIN, approxchain_xpm,_("Approximates Freeman chain(s) with a polygonal curve"), "Approxchains", 5);

    CONSTRUCTEUR_CMD(bouton, wxID_OPEN, open_xpm,_("Open"), " ", 6);
    CONSTRUCTEUR_CMD(bouton, wxID_SAVE, save_xpm,_("Save"), " ", 6);
    /**/
 //   CONSTRUCTEUR_CMD(bouton, ID_FUSIONPLAN, fusionplan_xpm, _("Merge color plan"), "merge", 7);
    CONSTRUCTEUR_CMD(bouton, ID_FUSIONPLAN, fusionplan_xpm, _("Merge color plan"), "merge", 7);
    CONSTRUCTEUR_CMD(bouton, ID_SEPARATIONPLAN, separationplan_xpm, _("split color plan"), "split", 7);
    CONSTRUCTEUR_CMD(bouton, ID_RGBLUMINANCE, rgbluminance_xpm,_("RGB to gray"), "cvtcolor", 7);
    CONSTRUCTEUR_CMD(bouton, ID_CONVERTTYPE, converttype_xpm,_("Convert type"), "convertto", 7);
	CONSTRUCTEUR_CMD(bouton, ID_ROTATION_AFFINE,wrapaffine_xpm, _("Transform affine-rotation"), "wrapAffine", 7);
	CONSTRUCTEUR_CMD(bouton, ID_NOUVELLE_DIMENSION, resize_xpm,_("Resize"), "resize", 7);
	CONSTRUCTEUR_CMD(bouton, ID_WHITTAKER, recons_xpm,_("Zoom"), "recons", 7);
    
	CONSTRUCTEUR_CMD(bouton, ID_LOGPOLAR, logpolar_xpm,_("Log polar"), "logPolar", 7);
	CONSTRUCTEUR_CMD(bouton, ID_LINEARPOLAR, linearpolar_xpm,_("Linear polar"), "linearPolar", 7);
	CONSTRUCTEUR_CMD(bouton, ID_UNDISTORT, undistort_xpm,_("Compensate for lens distortion"), "undistort", 7);


    CONSTRUCTEUR_CMD(bouton, ID_CANNY, canny_xpm,_("Canny edge detector"), "canny", 8);
    CONSTRUCTEUR_CMD(bouton, ID_CORNERHARRIS, cornerharris_xpm, _("Harris edge detector"), "cornerharris", 8);
    CONSTRUCTEUR_CMD(bouton, ID_GOODFEATURE, goodfeature_xpm,_("Very good features"), "goodfeaturestotrack", 8);
    CONSTRUCTEUR_CMD(bouton, ID_HOUGHCIRCLE, houghcircle_xpm,_("Hough circle"), "houghcircles", 8);
    CONSTRUCTEUR_CMD(bouton, ID_HOUGHLINE, houghline_xpm,_("Hough line"), "houghlines", 8);
    CONSTRUCTEUR_CMD(bouton, ID_HOUGHLINEP, houghlinep_xpm,_("hough line proba."), "houghlinesp", 8);


    CONSTRUCTEUR_CMD(bouton, ID_PYRFLOTOPTIQUE, pyrflotoptique_xpm, _("Build pyramid optical flow"), "buildopticalflowpyramid", 9);
    CONSTRUCTEUR_CMD(bouton, ID_CALCFLOTOPTIQUE, calcflotoptique_xpm ,_("Calculate optical flow (LucasKanade)"), "calcopticalflowpyrlk", 9);
    CONSTRUCTEUR_CMD(bouton, ID_CALCFLOTOPTIQUEFARNER, calcflotoptiquefarner_xpm,_("Calculate optical flow(farnerback)"), "calcopticalflowfarneback", 9);
    CONSTRUCTEUR_CMD(bouton, ID_MAJ_MVT, majmvt_xpm,_("Update motion history"), "updatemotionhistory", 9);
    CONSTRUCTEUR_CMD(bouton, ID_GRAD_MVT, motiongradient_xpm,_("motion Gradient"), "calcmotiongradient", 9);
    CONSTRUCTEUR_CMD(bouton, ID_SEG_MVT, segmentationmotion_xpm ,_("Independant motions"), "segmentmotion", 9);
    CONSTRUCTEUR_CMD(bouton, ID_PHASE_CORRELATE, phasecorrelate_xpm, _("Phase correlation"), "phasecorrelate", 9);

    CONSTRUCTEUR_CMD(bouton, ID_FOND_MOG, MOG_xpm,_("Gaussian mixture"), "fond_gaussianmixture", 10);
    CONSTRUCTEUR_CMD(bouton, ID_FOND_MOG2, MOG2_xpm, _("Gaussian mixture 2"), "fond_gaussianmixture2", 10);
    CONSTRUCTEUR_CMD(bouton, ID_FOND_KNN, KNN_xpm,_("knn method"), "fond_knn", 10);
    CONSTRUCTEUR_CMD(bouton, ID_FOND_MGM, GMG_xpm,_("GMG method"), "fond_gmg", 10);

    CONSTRUCTEUR_CMD(bouton, ID_DES_ORB, ORB_xpm,_("ORB keypoint"), "orbfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_BRISK, BRISK_xpm, _("BRISK keypoint"), "briskfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_AKAZE, AKAZE_xpm,_("Akaze keypoint"), "akazefeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_KAZE, KAZE_xpm,_("kaze keypoint"), "kazefeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_SIFT, Sift_xpm,_("Sift keypoint"), "siftfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_SURF, Surf_xpm,_("Surf keypoint"), "surffeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_AGAST, Agast_xpm,_("Agatse keypoint"), "agastfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_BLOB, blob_xpm,_("BLOB keypoint"), "blobfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_MSER, mser_xpm,_("MSER region"), "mserfeatures2d", 11);
    CONSTRUCTEUR_CMD(bouton, ID_DES_MATCH, Match_xpm, _("Match"), "matchdescriptormatcher", 11);
    CONSTRUCTEUR_CMD(bouton, ID_HOMOGRAPHY, homography_xpm,_("Homography"), "findhomography", 11); 
    CONSTRUCTEUR_CMD(bouton, ID_WARPPERPESCTIVE, warpperspective_xpm,_("warpPerspective"), "warpperspective", 11); 
    

    CONSTRUCTEUR_CMD(bouton, ID_DES_PANOFEATURE, panofeature_xpm,_("Stitching feature"), "detailfeaturesfinder", 12);
    CONSTRUCTEUR_CMD(bouton, ID_DES_PANOMATCH, panomatch_xpm,_("Stitching feature"), "detailmatchesinfo", 12);
    CONSTRUCTEUR_CMD(bouton, ID_DES_PANOBESTMATCH, panobestmatch_xpm,_("Stitching feature"), "leavebiggestcomponent", 12);
	CONSTRUCTEUR_CMD(bouton, ID_DES_PANOHOMOGRAPHY, panohomography_xpm, _("Stitching feature"), "homographybasedestimator", 12);
	CONSTRUCTEUR_CMD(bouton, ID_PANO_WRAPPER, panowraper_xpm, _("Wraper Wrap"), "wraperwrap", 12);
	CONSTRUCTEUR_CMD(bouton, ID_PANO_EXPO, panocorrectexpo_xpm, _("Exposure compensator"), "correctionexpo", 12);
	CONSTRUCTEUR_CMD(bouton, ID_PANO_COMPO, panocomposition_xpm, _("Wrapper"), "panocomposition", 12);

}

void InterfaceAvance::OnMyButtonRightDown(wxMouseEvent& event)
{
    std::string s;
    ImageInfoCV xx;

    s = bouton[event.GetId()].chaineOperation.c_str();
    ParametreOperation p(s);
    p.InitPtrFonction();
    FenetreAlgo *f = new FenetreAlgo(NULL, _("Operation"), wxDefaultPosition, wxDefaultSize, xx.listeOperation[s], wxDEFAULT_FRAME_STYLE);
    f->Show(true);
}


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
	InitCommande();
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
	InstallationbarreOutils(10);
	InstallationbarreOutils(11);
	InstallationbarreOutils(12);


    wxWindow* wnd10 = CreateTextCtrl(wxEmptyString);

 	wxLogTextCtrl *logWindow = new wxLogTextCtrl((wxTextCtrl *)wnd10);
	wxLog::SetVerbose(true);
	delete wxLog::SetActiveTarget(logWindow);
 /*   (wxTextCtrl *)wnd10->Disable();
     wnd10->SetForegroundColour(*wxBLACK);
     wnd10->SetBackgroundColour(*wxWHITE);*/
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
cv::Mat m = xx.opnn[idFiltre]->getMat(cv::ACCESS_READ);
for (int i=0;i<xx.opnn[idFiltre]->rows;i++)
	for (int j=0;j<xx.opnn[idFiltre]->cols;j++)
		g->DefCellule(i,j,m.at<float>(i,j));
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
    cv::Mat m = xx.opnn[idFiltre]->getMat(cv::ACCESS_RW);
	m.at<float>(l,c)=val;
    m.copyTo(*xx.opnn[idFiltre]);
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
cv::Mat m = xx.opnn[0]->getMat(cv::ACCESS_READ);
for (int i=0;i<3;i++)
	for (int j=0;j<3;j++)
		excel->DefCellule(i,j,m.at<float>(i,j));
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

	flex->Add( new wxStaticText( panneauCtrl, -1, _("Operator half-size") ), 0, wxALL|wxALIGN_CENTRE, 5 );
	flex->Add( new wxSpinCtrl( panneauCtrl, ROUETAILLE_MORPH, "1", wxDefaultPosition, wxSize(100,-1),
			wxSP_ARROW_KEYS, 1, 32, 5 ), 0, wxALL|wxALIGN_CENTRE, 5 );

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
element.copyTo(*x.opMorph[ind]);
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
cv::Mat m = ImageInfoCV::opMorph[ind]->getMat(cv::ACCESS_READ);
for (int i=0;i<ImageInfoCV::opMorph[ind]->rows;i++)
	for (int j=0;j<ImageInfoCV::opMorph[ind]->cols;j++)
		{
		unsigned char c=m.at<unsigned char>(i,j);
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
			Tool_bidon,Tool_addition,Tool_soustraction,Tool_multiplication,Tool_division,Tool_oulogique,Tool_etlogique,Tool_ouexclulogique,Tool_negationlogique,
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

	tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxAUI_TB_DEFAULT_STYLE |
		/*  pour personnaliser wxAUI_TB_OVERFLOW |*/
		wxAUI_TB_TEXT |
		wxAUI_TB_HORZ_TEXT);
	//			 wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
	tb->SetMargins(1, 1);
	std::map<int,Commande>::iterator it;

	for (it = bouton.begin(); it != bouton.end(); it++)
        if (it->second.numBarreoutils == indBarre)
        {
 	        tb->AddTool(it->second.idEvt, "", it->second.bitmap, it->second.chaineAide);

        }
        tb->Bind(wxEVT_AUITOOLBAR_RIGHT_CLICK, wxMouseEventHandler(InterfaceAvance::OnMyButtonRightDown),this);
	tb->Realize();

    switch (indBarre)
	{
	case 0: // Opération arithmétique
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Arithmetic operation").Caption("Arithmetic operation").
					  ToolbarPane().Top().Row(1));
		break;
	case 1 : // Opérateur morphologique
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Morphologic operation").Caption("Morphologic operation").
					  ToolbarPane().Top().Row(3));
		break;
	case 2: // Opérateur Spectrale et d'échelle
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Scale/Frequency").Caption("Scale/Frequency").
					  ToolbarPane().Top().Row(5));
		break;
	case 3 :// Opérateur lissage
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Filter").Caption("Filter").
					  ToolbarPane().Top().Row(2));
		break;
	case 4:// Opérateur Vidéo
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name("tbVideo").Caption(_("Video Toolbar")).
					  ToolbarPane().Left().
					  GripperTop());
		break;
	case 5 : // Outils opération, seuillage, contour, canny, composante connexe et stat
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("binarize").Caption("binarize").
					  ToolbarPane().Top().Row(4));
		break;
	case 6 : // Ouverture, fermeture de fichiers
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name(_("tb5")).Caption(_("Big Toolbar")).
					  ToolbarPane().Top());
		break;
	case 7 : // Outils couleur, normalisation

		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("Color").Caption("Color").
					  ToolbarPane().Top().Row(7));
		break;
	case 8:
		m_mgr.AddPane(tb,  wxAuiPaneInfo().
					  Name("feature").Caption("Feature").
					  ToolbarPane().Top().Row(4));
		break;
	case 9:
		m_mgr.AddPane(tb, wxAuiPaneInfo().
			Name("Optical Flow").Caption("Optical Flow").
			ToolbarPane().Top().Row(5));
		break;
	case 10:
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name("Background Subtractor").Caption("Background Subtractor").
					  ToolbarPane().Top().Row(5));
		break;
	case 11:
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name("Feature 2D").Caption("Feature 2D").
					  ToolbarPane().Top().Row(6));
		break;
	case 12:
		m_mgr.AddPane(tb, wxAuiPaneInfo().
					  Name("Stitching").Caption("Stitching").
					  ToolbarPane().Top().Row(8));
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
case ID_CONVOLUTION:
	{
	ImageInfoCV x;

	if (x.IndOpConvolution()<0 || x.opnn[x.IndOpConvolution()]==NULL)
		{
		wxMessageBox(_("You must defined convolution operator first!"),_("Error"), wxOK,this);
		return;
		}
	}
	break;
case ID_EROSION:
case ID_DILATATION:
case ID_OUVERTURE:
case ID_FERMETURE:
case ID_CHAPHAUTBL:
case ID_CHAPHAUTNO:
case ID_GRADMORPH:
	{
	ImageInfoCV x;

	if (x.IndOpMorphologie()<0 || x.opMorph[x.IndOpMorphologie()]==NULL)
		{
		wxMessageBox(_("You must defined morphological operator first!"),_("Error"), wxOK,this);
		return;
		}
	}
	break;
	}
((wxOsgApp*)osgApp)->DefOperateurImage(bouton[evt.GetId()].chaineOperation);
((wxOsgApp*)osgApp)->DefPointeurSouris(1,1);

}


