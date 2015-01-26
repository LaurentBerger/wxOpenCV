#ifndef __TABLEUR__
#define __TABLEUR__



// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// Entete
// ----------------------------------------------------------------------------

// Si précompilation possible
#include "wx/wxprec.h"
#include "wx/image.h"
#include <wx/grid.h>
// Sinon
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/socket.h"
#include "wx/notebook.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

class Tableur;

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

/**  La classe Tableur permet l'affichage 3D d'une surface en utilisant OpenGL.
Cette classe hérite de wxGLCanvas et de osgViewerd'informations dans un tableur
*/


class Tableur : public wxGrid
{
private :
int		nbLigne,nbColonne;
char	*donnees;
void	*fenetreMere;

public :
Tableur(wxFrame *,int ,int);
void	DefFenetreMere(void *w){fenetreMere=w;};
void	DefCellule(int l,int c, double val,char *format=NULL);
void	DefCellule(int l,int c, int val,char *format=NULL);
void	DefCellule(int l,int c, char *);
void	DefCellule(int l,int c, wxString *);
double	LitCellule(int l,int c,char	&status);
void	DefTitreColonne(int c, char *s);
void	DefTitreLigne(int l, char *s);
void	DefTitreColonne(int c, wxString &s);
void	DefTitreLigne(int l, wxString &s);

void	SelectionLigne(int l);
void	CelluleVisible(int l,int c);

void AjouteLigne(int);
void EffaceLigne(int);

void CelluleModifiee(wxGridEvent &w);
void OnSelectCell(wxGridEvent &w);
void OnRangeSelected(wxGridRangeSelectEvent &w);
public:
    DECLARE_EVENT_TABLE()

};






#endif