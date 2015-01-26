#ifdef __TOTO__
#include "FenetrePrincipale.h"
#include "outilsImage.h"
#include "imagestat.h"
#include "Tableur.h"
#include <fstream>
#include <wx/busyinfo.h>

using namespace std;


#define SEUILLAGEHYSTERSIS

/*!< 1 Commentaire fonction Image Camera */
void FenetrePrincipale::ImageCamera(wxCommandEvent& event)
{
modeImage = 0;
//((wxOsgApp*)osgApp)->Outils()->MAJValeurPalette();
MAJNouvelleImage();

}



void FenetrePrincipale::DefNbMarcheFit(int x)
{
}
#endif
