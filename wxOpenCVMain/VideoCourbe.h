#ifndef __VIDEOCOURBE__
#define __VIDEOCOURBE__

#include "FenetrePrincipale.h"
#include "wxPLplotwindow.h"
#include "Tableur.h"
#include <vector>
using namespace std;

//template< class WXWINDOW >



class FenetreCourbe : public wxPLplotwindow<wxFrame>
{

public:
    FenetreCourbe(  wxWindow* parent, wxWindowID id=-1, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                int pl_style = wxPLPLOT_NONE );
    ~FenetreCourbe();

    void OnClose(wxCloseEvent& event);
    void OnChar( wxKeyEvent& event );  
    void DefOSGApp(void *w){osgApp=w;};
    void DefFenMere(FenetrePrincipale *f){fenMere =f;};
    void Ajoute(int nb,double xi,double yi){if (nb==-1) {Plot();return;}x[nb].push_back(xi);y[nb].push_back(yi);};
    void AnnulCourbe(){x.clear();y.clear();};

  void Plot(  )
{
    if (x.size()==0)
        return;
    if (x[0].size()==0)
        return;
    wxPLplotstream* pls = GetStream();

    const size_t  np = 500;
    PLFLT         xmin, xmax;
    PLFLT         ymin = 1e30, ymax = 1e-30;

    int nb=0;
    int deb=0,fin=x[nb].size()-1;
    if (x[nb].size()>100)
        deb = x[nb].size()-100;
    xmin = x[nb][deb];

    xmax = x[nb][x[nb].size()-1];
    ymin = y[nb][0];
    ymax = y[nb][0];
    for ( nb = 0; nb<x.size()  && nb<6; nb++)
    {
        if (x[nb].size()>0)
        {
            for ( size_t i = deb; i < x[nb].size(); i++ )
            {
                ymin = MIN( ymin, y[nb][i] );
                ymax = MAX( ymax, y[nb][i] );
            }

        }
    }
    pls->adv( 0 );
    pls->col0( 1 );
    pls->env( xmin, xmax, ymin, ymax, 0, 0 );
    pls->col0( 2 );
    pls->lab( "x", "y", "sin(x)/x" );

    for (int nb = 0; nb<x.size()  && nb<6; nb++)
    {

            pls->col0( 3+nb );
            pls->width( 2 );
            pls->line( min(int(x[nb].size()),100), x[nb].data()+deb, y[nb].data()+deb );
    }


    RenewPlot();
}

private:
    wxFrame	*frame;
    FenetrePrincipale				*fenMere;
    void							*osgApp;
    vector<vector<PLFLT> >         x, y;
};

#endif
