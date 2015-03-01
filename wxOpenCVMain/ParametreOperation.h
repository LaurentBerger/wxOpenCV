#ifndef __PARAMETREOPERATION__
#define __PARAMETREOPERATION__
#include <wx/string.h>
#include <map>
#include <string>
#include "opencv2/opencv.hpp" 

class ImageInfoCV;

using namespace std;


/*! \class DomaineParametre
   * \brief la classe�DomaineParametre d�finit la valeur, le domaine de d�finition, le pas de la valeur
   * utilis� pour un param�tre de fonctions. Le pas est utilis� pour modifier l'op�ration dans une fen�tre de dialogue.
   */

template <class TypeValeur>
class DomaineParametreOp {
public :
TypeValeur valeur;		/*< Valeur actuelle du param�tre*/
TypeValeur mini,maxi;	/*< Valeur extremum du param�tre */
TypeValeur pas;		/*< Pas entre deux valeurs */
DomaineParametreOp(TypeValeur a,TypeValeur b,TypeValeur c,TypeValeur d):valeur(a),mini(b),maxi(c),pas(d){};
DomaineParametreOp(TypeValeur a):valeur(a),mini(a),maxi(a),pas(a){};
DomaineParametreOp():valeur(TypeValeur()),mini(TypeValeur()),maxi(TypeValeur()),pas(TypeValeur()){};
//DomaineParametre():valeur(cv:Point(0,0)),mini(cv:Point(0,0)),maxi(cv:Point(0,0)),pas(cv:Point(0,0)){};
};

/*! \class Parametre
   * \brief la classe�Parametre d�finit les noms et domaine des param�tres n�cessaires pour effectuer une op�ration.
   * les param�tres peuvent �tre du type int, double size ou point (type OpenCV). Un lien sur la doc de la fonction 
   * est associ� aux param�tres
   * nbImageRes d�signe le nombre d'image r�sultat.  
   */

class ParametreOperation {
void InitParamType();


public :
wxString	nomSequence;
wxString	nomOperation;
std::map<std::string,DomaineParametreOp<double> > doubleParam;
std::map<std::string,DomaineParametreOp<int> > intParam;
std::map<std::string,DomaineParametreOp<cv::Size> > sizeParam;
std::map<std::string,DomaineParametreOp<cv::Point> > pointParam;
std::string lienHtml;
std::string refPDF;
int nbImageRes;	
int nbOperande;
int idOperation;
int indEtape;
int indRes;

ImageInfoCV *op1;	/*!< Op�rande 1 pour l'op�ration demand�e */
ImageInfoCV *op2;	/*!< Op�rande 2 pour l'op�ration demand�e */
ImageInfoCV *op3;	/*!< Op�rande 3 pour l'op�ration demand�e uniquement fusion plan*/
//ParametreOperation pOCV;	/*!< parametre de l'op�rateur Unaire */
int	indOp1Fenetre;	/*!< Indice de la fen�tre contenant l'image de op�rande 1 */
int indOp2Fenetre;  /*!< Indice de la fen�tre contenant l'image de op�rande 2 */
int indOp3Fenetre;  /*!< Indice de la fen�tre contenant l'image de op�rande 2 */


// surjection nombre d'images, les images dans un tableau et les param�tres
ImageInfoCV**  (ImageInfoCV::*opSurjecMultiple)(int,ImageInfoCV **,ParametreOperation *) ;
// Op�ration programm�e ternaire : Fusion plan
ImageInfoCV*  (ImageInfoCV::*opNaireSelec)(int ,ImageInfoCV **,ParametreOperation *) ;
// Op�ration programm�e binaire
ImageInfoCV*  (ImageInfoCV::*opBinaireSelec)(ImageInfoCV *,ImageInfoCV *,ParametreOperation *) ;
// Op�ration programm�e unaire avec parametre
ImageInfoCV*  (ImageInfoCV::*opUnaireSelec)(ImageInfoCV *,ParametreOperation *) ;
// Surjection (plusieurs r�sultats   avec parametre
ImageInfoCV**  (ImageInfoCV::*opSurjecUnaire)(ImageInfoCV *,ParametreOperation *) ;

static std::map<wxString,std::map<wxString,int> > listeParam;


ParametreOperation(string s);	
ParametreOperation(){InitOperation("");};	
bool InitOperation(string s);
bool InitPtrFonction();
};

/*
string nomOperation[]={
	"PartageEaux","SeparationPlan","FusionPlan","Addition","Soustraction",
	"Multiplication","Division","Convolution","Dilatation","Erosion","Ouverture",
	"Fermeture","ChapeauHaut","ChapeauBas","GradMorph","Scharr_mod","Scharr_x",
	"Scharr_y","Laplacien","Canny","Contour","FFT","IFFT","Seuillage",
	"AdaptatifSeuillage","LissageMedian","LissageMoyenneur","LissageGaussien",
	"ComposanteConnexe","DistanceDiscrete","LigneMediane"};						
*/
#endif
