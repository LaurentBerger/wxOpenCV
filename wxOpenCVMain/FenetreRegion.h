#ifndef _FENETREREGION_H_
#define _FENETREREGION_H_

#include "FenetrePrincipale.h"
#include "wx/wx.h"
#include "Tableur.h"
#include <cmath>


// Define a new frame type: this is going to be our main frame
class FenetreRegion : public wxWindow
{

public:
FenetreRegion( wxFrame * );
~FenetreRegion( ){delete []regionSelect;delete []cleTri;delete []valTri;};
void DefOsgApp(void *w){osgApp=w;};
void DefParent(void *w){fenParent=w;};

void ListerRegion(); 
void DefRegionSelect(int i,int val){regionSelect[i]=val;};;
void LireNiveau(int *classe);

void OnSelectCell(wxGridEvent &w);
void OnRangeSelected(wxGridRangeSelectEvent &w);
void MenuContextuel(wxGridEvent &w);
void ShowContextMenu(const wxPoint& pos);
void OterSelection(wxGridEvent &w);
void FusionNiveau(wxCommandEvent &w);
void RAZNiveau(wxCommandEvent &w);
void DefinirNiveau(wxCommandEvent &w);



char *RegionSelect(){return regionSelect;};
private:
void *osgApp;
void *fenParent;
Tableur *listeRegion;
int		nbRegionMax;
char	*regionSelect;
long		*cleTri;
float	*valTri;
int		nbZoneId;
char	gestionControlDown;
    DECLARE_EVENT_TABLE()

};


#endif
