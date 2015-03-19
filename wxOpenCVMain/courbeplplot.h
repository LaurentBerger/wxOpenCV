#ifndef _COURBEPLPLOT_H_
#define _COURBEPLPLOT_H_

#include "wx/wx.h"
#include "wxPLplotwindow.h"
#include "FenetrePrincipale.h"
#include "Tableur.h"
#include <cmath>
#include "ImageInfo.h"




class FenetreCourbe : public wxPLplotwindow<wxFrame>
{
public:
  FenetreCourbe( wxFrame* frame, wxWindow* parent, wxWindowID id=-1, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                int pl_style = wxPLPLOT_NONE );

  void OnChar( wxKeyEvent& event );  
private:
  wxFrame	*mframe;
};

#define NB_MAX_CANAUX 5
// Define a new frame type: this is going to be our main frame
class FenetreHistogramme : public wxWindow
{
Tableur *excel;
wxPanel* panel;
wxSizer *box;
// L'image a au maximum 5 canaux */
PLFLT	*x[NB_MAX_CANAUX],*y[NB_MAX_CANAUX],*yFiltre[NB_MAX_CANAUX];  /*!< courbe des histogrammes pour les trois plans */
int		nbGraines[5]; /*!< Nombre de graines pour l'histogramme des canaux */
float	minHisto[NB_MAX_CANAUX],maxHisto[NB_MAX_CANAUX]; /*!< Minimum et maximum de l'histogramme */
cv::Mat	histoImage[NB_MAX_CANAUX];

void *osgApp;
void *fenMere;
public:
FenetreHistogramme( wxFrame * );
~FenetreHistogramme(); 

void DefFenetrePrincipale(void *f){fenMere=f;};
void MajHistogramme(); /*!<Calcul de l'histogramme de l'image en tenant compte de la sélection */
void Plot(char =0);
void SauverSVG(wxString &nomFichier){courbe->SavePlot(wxString("svg"),nomFichier);};

void DefOsgApp(void *w){osgApp=w;};
cv::Mat*	Histogramme(int i=0){return &(histoImage[i]);};
Tableur *Grille(){return excel;};
/*!<Accès à la grille associée à l'histogramme */
// Evènement
void OnKeyDown(wxKeyEvent &event);
void OnClose(wxCloseEvent& event); 
void NouvelHistogramme(wxGridEvent& event); /*!< Saisie d'une nouvelle dans le tableur (nombre de grianes) */

private:
  void OnQuit( wxCommandEvent& event );
  void OnAbout( wxCommandEvent& event );
  void OnBackgroundColor( wxCommandEvent& event );

private:
//  FenetreCourbe* plotwindow;
  wxPLplotwindow<wxPanel> *courbe;
  bool bgcolor;
  int m_backend;

  DECLARE_EVENT_TABLE()
};

// Define a new frame type: this is going to be our main frame
class FenetreCoupe : public wxWindow
{
wxRect	rCoupe;
Tableur *excel;
wxSizer *box;
wxPanel* panel;
PLFLT *x[NB_MAX_CANAUX],*y[NB_MAX_CANAUX],*yFiltre[NB_MAX_CANAUX];  /*!< courbe des coupes pour les trois plans */
void *osgApp;
void *fenMere;
public:
 FenetreCoupe( wxFrame * );
~FenetreCoupe(); 
void Plot(bool );
void DefRect(wxRect &r){rCoupe=r;};
void DefFenetrePrincipale(void *f){fenMere=f;};
void DefOsgApp(void *w){osgApp=w;};
void	DefCellule(int l,int c, double val,char *format=NULL);
void CoordonneeGraphique();
void OnKeyDown(wxKeyEvent &event);

private:
  void OnQuit( wxCommandEvent& event );
  void OnAbout( wxCommandEvent& event );
  void OnBackgroundColor( wxCommandEvent& event );
private:
  wxPLplotwindow<wxPanel> *courbe;
  bool bgcolor;
  int m_backend;

  DECLARE_EVENT_TABLE()
};

// Define a new frame type: this is going to be our main frame
class FenetreDistribRadiale : public wxWindow
{
wxRect	rCoupe;
Tableur *excel;
void *fenMere;
void *osgApp;
double **dr;
int		nbPlan;
int		nbVal;
PLFLT *x;
PLFLT *y;

public:
  FenetreDistribRadiale( wxFrame * );
~FenetreDistribRadiale(); 
  void Plot(bool );
  void DefRect(wxRect &r){rCoupe=r;};
void DefFenetrePrincipale(void *f){fenMere=f;};
void DefOsgApp(void *w){osgApp=w;};
void	DefCellule(int l,int c, double val,char *format=NULL);

private:
  void OnQuit( wxCommandEvent& event );
  void OnAbout( wxCommandEvent& event );
  void OnBackgroundColor( wxCommandEvent& event );

private:
  wxPLplotwindow<wxPanel> *courbe;
  bool bgcolor;
  int m_backend;

  DECLARE_EVENT_TABLE()
};

// Define a new frame type: this is going to be our main frame
class FenetreDistribAngulaire : public wxWindow
{
wxRect	rCoupe;
Tableur *excel;
void *osgApp;
void *fenMere;
double **dr;
PLFLT *x;
PLFLT *y;
public:
  FenetreDistribAngulaire( wxFrame * );
~FenetreDistribAngulaire(void);
  void Plot(bool );
  void DefRect(wxRect &r){rCoupe=r;};
void DefFenetrePrincipale(void *f){fenMere=f;};
void DefOsgApp(void *w){osgApp=w;};
void	DefCellule(int l,int c, double val,char *format=NULL);

private:
  void OnQuit( wxCommandEvent& event );
  void OnAbout( wxCommandEvent& event );
  void OnBackgroundColor( wxCommandEvent& event );

private:
  wxPLplotwindow<wxPanel> *courbe;
  bool bgcolor;
  int m_backend;

  DECLARE_EVENT_TABLE()
};


#define NBELT_FILE 200
// Define a new frame type: this is going to be our main frame
class FenetreFocus : public wxWindow
{
float	histoStat[NB_MAX_RECTANGLE][NBELT_FILE];
int		posFile[NB_MAX_RECTANGLE];
Tableur *excel;
void	*osgApp;
void *fenMere;
char	razHisto;
public:
  FenetreFocus( wxFrame * );
  void Plot(int);
void	DefOsgApp(void *w){osgApp=w;};
void DefFenetrePrincipale(void *f){fenMere=f;};
void	DefCellule(int l,int c, double val,char *format=NULL);
void	RAZHistoStat(){razHisto=1;};
private:
void	OnQuit( wxCommandEvent& event );
void	OnAbout( wxCommandEvent& event );
void	OnBackgroundColor( wxCommandEvent& event );

private:
  wxPLplotwindow<wxPanel> *courbe;
  bool bgcolor;
  int m_backend;

  DECLARE_EVENT_TABLE()
};

#endif
