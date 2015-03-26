/*#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>*/
#ifndef __IMAGEINFO__
#define __IMAGEINFO__

#include <ctime>
#include <map>
#include <string>

#include "opencv2/opencv.hpp" 
#include "ImageConstante.h"
#include "ParametreOperation.h"
typedef short CodeErreur;
#define ERREUR OuiERREUR

#define NB_OP_MORPHOLOGIE 10
#define NB_OP_CONVOLUTION 40





class ImageInfoCV : public cv::Mat {
// Type pour les images openCV en binaire type()
// xxxyyy 3 bits de poids faibles pour taille et signe :
// yyy
// 000 non sign� 8 bits, 011 8 bits sign�, 010 non sign� 16 bits, 011 16 bits sign�
// 101 32 bits , 110 64 bits
// xxx bit de poids fort pour le nombre de canaux
// 000 1 canal, 001 2 canaux, 010 3 canaux 
public :
static ImageInfoCV **op33;
static ImageInfoCV **op55;
static ImageInfoCV **op77;
static ImageInfoCV **opnn;
static ImageInfoCV **opMorph;

protected :
// 

static char *tpFct[9];



void	*eSauver;
private	:
#ifdef __INDOTEXTE__
char	*nomImage;			//< Nom de l'image sur le disque
char	*natureImage;		// M�thode de saisie de l'image
char	dateCreation[30];
char	*createur;			// utilisateur
double	resolX;				// Pixel par cm
double	resolY;				// Pixel par cm 
double	resolZ;
char	*uniteZ;
char	*typeMateriel;		// Nom de la chaine d'acquisition
char	*marqueMateriel;	// marque de la chaine d'acquisition
char	*copyright;
char	*nomProjet;			// Nom du projet associ�e � l'image
char	*nomPgm;			// Nom et version du pgm 
char	*ordinateur;
char	*description;
char	*champUtil[20];
							//membres utilis�s en fonction du contexte 
long	nbChampCopolymere;
char	**copolymere;

long	nbChampGonfle;
char	**gonfle;

long	nbChampComposite;
char	**composite;

long	nbChampMicroscope;
char	**microscope;

long	nbChampCamera;
char	**camera;

long	nbChampScanner;
char	**scanner;

long	nbChampSimulation;
char	**simulation;
#endif

char converCplxEnt;
static int fctImage;
static int typeSeuillage;
static int tailleOperateur;
static int centrageResultat;
static int typeOndelette;
static int tailleOndelette;
static int nbIterOperateur;
static int indOpConvolution; /*< Indice de l'op�rateur de convolution s�lectionn� */
static int indOpMorphologie; /*< Indice de l'op�rateur de morphologie s�lectionn� */
static int typeResultat;	 /*< Type du r�sultat -1, ou constante OpenCV  p256 reference manual */
/***********************************************
********** INFO IMAGE **************************
************************************************/
double *minIm;			/*< Minimimum pour chaque plan de l'image */
double *maxIm;			/*< Maximimum pour chaque plan de l'image */
cv::Point	*locMin;	/*< Position du miminmu pour chaque plan */
cv::Point	*locMax;	/*< Position du miminmu pour chaque plan */
cv::Mat		**statComposante; /*< Statistique des composantes de chaque plan http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html */
cv::Mat		**centreGComposante; /*< Centre de gravite http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html */
std::vector<std::vector<cv::Point> > *contours; /*< Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<cv::Vec4i> *arbreContour; /*< Arborescence des Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<cv::Moments> *moment;	/*<!http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/moments/moments.html*/
std::vector<double> *huMoment;		/*<!http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=moments#humoments*/
std::vector<cv::Vec3f> *cercle;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles*/
std::vector<cv::Vec4i> *ligneP;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp */
std::vector<cv::Vec2f> *ligne;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines */
std::vector<cv::Point2f> *boncoin;	/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack */
std::vector<cv::Point2f> *coinRef;	/*<! Les pixels de r�f�rences de l'image pour calcul du flot optique */
cv::Mat	*flotOptique;				/*<! Flot optique associ� � l'image calcul� par calcopticalFlowFarnerBack*/

ParametreOperation *pOCVHoughLigne;
ParametreOperation *pOCVHoughLigneProba;
ParametreOperation *pOCVHoughCercle;
ParametreOperation *pOCVBonCoin;
ParametreOperation *pOCVLucasKanade;

public : 
//	********* Constructeurs et destructeur
ImageInfoCV(long nbL,long nbC,int type);
explicit ImageInfoCV(void);
ImageInfoCV(void *);
ImageInfoCV(char *);
~ImageInfoCV(void);
void DoEnregistrer(char **nomFic,long =0,void * = NULL);
void SauverProprieteProjet(char *);
void DefProprieteImage(char *);
// Lecture des images au format YML
void write(cv::FileStorage& fs) const  ;                      //Write serialization for this class
void read(const cv::FileNode& node);                          //Read serialization for this class


private :
void InitImageInfo(void *eTiff=NULL);
void InitOp();
char **DefChampMultiple(char *,long &);
void DefChampMultiple(long ,char *c,char ***,long &);
char *ConversionChampUtilChampTiff(char **,long,long &);

//	********* D�finition des membres priv�s
public :
void EffaceOp();
void	DefFctImage(int i){fctImage=i;};
void	DefFctImage(char *);
void	DefTypeSeuillage(char*);
void	DefTypeResultat(int i){typeResultat=i;};
void	DefTailleOperateur(char*);
void	DefCentrageResultat(char*);
void	DefTypeOndelette(char *);
void	DefTailleOndelette(long);
void	DefNbIterOperateur(long);
void	DefConverCplxEnt(char c);
int		IndOpConvolution(int i=-1);  /*< Indice de l'op�rateur de convolution s�lectionn� */
int		IndOpMorphologie(int i=-1); /*< Indice de l'op�rateur de morphologie s�lectionn� */
int		FctImage(){return fctImage;};
int		TypeResultat(){return typeResultat;};


void 	DefNomImage(char *);
void 	DefNatureImage(char *);
void 	DefDateCreation(char *);
void 	DefCreateur(char *);
void	DefResolX(double);
void	DefResolY(double);
void	DefResolZ(double z);
void	DefUniteZ(char *);
void	DefChampUtil(char *,long i,long nbOctets=-1);
void	DefChampCopolymere(char *);
void	DefChampCopolymere(long ,char *);
void	DefChampGonfle(char *);
void	DefChampGonfle(long ,char *);
void	DefChampComposite(char *);
void	DefChampComposite(long ,char *);
void	DefChampMicroscope(char *);
void	DefChampMicroscope(long ,char *);
void	DefChampScanner(char *);
void	DefChampScanner(long ,char *);
void	DefChampCamera(char *);
void	DefChampCamera(long ,char *);
void	DefChampSimulation(char *);
void	DefChampSimulation(long ,char *);
void	DefDescription(char *);
void	DefMarqueMateriel(char *);
void	DefTypeMateriel(char *);
void	DefCopyright(char *);
void	DefOrdinateur(char *);
void 	DefNomPgm(char *);
void 	DefNomProjet(char *);

int	LitNbLigne(){return rows;};
int	LitNbColonne(){return cols;};
int	LitNbPlan(){return channels();};

char* 	LitNomImage(void);
char* 	LitNatureImage(void);
char*  	LitDateCreation(void);
char* 	LitCreateur(void);
double	LitResolX(void);
double	LitResolY(void);
double	LitResolZ(void);
char*	LitUniteZ(void);
char*	LitChampUtil(long);
#ifdef __INFOTEXTE__
long	LitNbChampCopolymere(void){return nbChampCopolymere;};
long	LitNbChampComposite(void){return nbChampComposite;};
long	LitNbChampGonfle(void){return nbChampGonfle;};
long	LitNbChampMicroscope(void){return nbChampMicroscope;};
long	LitNbChampScanner(void){return nbChampScanner;};
long	LitNbChampCamera(void){return nbChampCamera;};
long	LitNbChampSimulation(void){return nbChampSimulation;};
#endif
char*	LitChampCopolymere(long );
char*	LitChampComposite(long );
char*	LitChampGonfle(long );
char*	LitChampMicroscope(long );
char*	LitChampScanner(long );
char*	LitChampCamera(long );
char*	LitChampSimulation(long );
char*	LitDescription(void);
char*	LitMarqueMateriel(void);
char*	LitTypeMateriel(void);
char*	LitCopyright(void);
char*	LitOrdinateur(void);
char*	LitNomPgm(void);
char*	LitNomProjet(void);
char*	InfoImage(void);

//Utilitaire pour info sur l'image
char *InfoPixel(long x,long y,long =-1,long =-1); 

// Surcharge des op�rateurs
ImageInfoCV 	*operator* (ImageInfoCV	&z);		// Convolution		
ImageInfoCV 	*operator& (ImageInfoCV	&z);		// Erosion
ImageInfoCV 	*operator| (ImageInfoCV	&z);		// Dilatation
ImageInfoCV 	*operator+ (ImageInfoCV	&z);		// Addition
ImageInfoCV 	*operator+ (double	z);		
ImageInfoCV 	*operator- (ImageInfoCV	&z);		 // Soustraction
ImageInfoCV 	*operator- (double	z);		
ImageInfoCV 	*operator^ (ImageInfoCV	&z);		 // Produit
ImageInfoCV 	*operator^ (double	z);		
ImageInfoCV 	*operator/ (ImageInfoCV	&z);		 // Division
ImageInfoCV 	*operator/ (double	z);		
ImageInfoCV 	&operator= (ImageInfoCV	&z);
ImageInfoCV	*Variance (long);

// Fonctions membres equivalentes aux op�rateurs utilisant OPENCV
ImageInfoCV 	*Add(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*AddPonderee(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Sub(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Mul(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Div(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Erosion(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Dilatation(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Ouverture(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Fermeture(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*ChapeauHaut(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*ChapeauBas(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*GradMorph(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Convolution(ImageInfoCV	*,ImageInfoCV	* = NULL,ParametreOperation *pOCV=NULL);
ImageInfoCV		*PartageEaux (ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*FlotOptiqueLucasKanadePyramide(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*FlotOptiqueFarnerback(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV=NULL);
ImageInfoCV 	*Laplacien(ImageInfoCV	*,ParametreOperation *pOCV);
ImageInfoCV 	*ScharrX(ImageInfoCV	*im1,ParametreOperation *pOCV);
ImageInfoCV 	*ScharrY(ImageInfoCV	*im1,ParametreOperation *pOCV);
ImageInfoCV 	*ScharrModule(ImageInfoCV	*im1,ParametreOperation *pOCV);
ImageInfoCV 	*Contour(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*Seuillage(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*SeuillageAdaptatif(ImageInfoCV	*imSrc,ParametreOperation *pOCV);
ImageInfoCV 	*LissageMedian(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*LissageMoyenne(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*LissageGaussien(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*LissageBilateral(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*FFT(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*IFFT(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV		*FusionPlan(int nbPlan,ImageInfoCV	**,ParametreOperation *paramOCV);
ImageInfoCV		*ComposanteConnexe (ImageInfoCV *result,ParametreOperation *paramOCV);
ImageInfoCV		**SeparationPlan(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV		*RGB_L(ImageInfoCV *im1,ParametreOperation *paramOCV);
ImageInfoCV		*DistanceDiscrete (ImageInfoCV *result,ParametreOperation *paramOCV);
ImageInfoCV		*Voronoi (ImageInfoCV *result,ParametreOperation *paramOCV);
ImageInfoCV		*LigneMediane(ImageInfoCV *result,ParametreOperation *paramOCV);
ImageInfoCV 	*Canny(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*HoughLigne(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*HoughCercle(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*HoughLigneProba(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*BonAttributs(ImageInfoCV	*,ParametreOperation *paramOCV);
ImageInfoCV 	*DetectCoinHarris(ImageInfoCV	*,ParametreOperation *paramOCV);


void ExtremumLoc(ImageInfoCV *mask=NULL );

// Transform�e de fourier
ImageInfoCV	*Fft (char dimension=0 );
ImageInfoCV   *IFft (char dimension=0);

// Corr�lation et intercorr�lation
ImageInfoCV 	*Correlation (ImageInfoCV	&z);

// Deconvolution it�rative et de wiener
ImageInfoCV 	*Deconvolution (ImageInfoCV	&z);
ImageInfoCV 	*Wiener(ImageInfoCV	&z,double	varianceBruit=0,long larBruit=-1,double seuilFiltre=5);

// Densit� spectrale de puissance
ImageInfoCV 	*DensiteSpectralePuissance (void);
ImageInfoCV 	*DensiteSpectralePuissanceAR (void);

// Transform�e en ondelette
ImageInfoCV 	*DOndelette(void);
ImageInfoCV 	*IOndelette(void);

// Op�rateurs unaires
ImageInfoCV	*Sqr (void);
ImageInfoCV	*Abs (void);

// Seuilage et composantes connexes 
long  SeuilImage(float *seuil,int ligDeb=0, int colDeb=0, int hauteur=-1, int largeur=-1);
void  SelectionMaxLocaux(ImageInfoCV &);
ImageInfoCV *Binarisation(float *seuilBin,ImageInfoCV* =NULL);
ImageInfoCV *NonMaximumSuppression(ImageInfoCV* ,ImageInfoCV*,float);
ImageInfoCV *SeuilHysteresis(float seuil1,float seuil2,ImageInfoCV* =NULL);
std::vector<cv::Moments> *CalcMoment();


// Op�rateurs Voronoi
ImageInfoCV 	*Voronoi (ImageInfoCV	&z);

// Squelette
ImageInfoCV	*DistObjetFond();
bool MaxLocal(int c,int i,int j);
bool TraiterMediane(int c,int &r,int &s,int dr,int ds,int &q,ImageInfoCV &im);
void SuiviChemin(int c,int i,int j,ImageInfoCV &im);
bool MaxLocal(int i,int j);
bool TraiterMediane(int &r,int &s,int dr,int ds,int &q,ImageInfoCV &im);
void SuiviChemin(int i,int j,ImageInfoCV &im);

// Reconstruction d'image
ImageInfoCV *Recons2d(long nbPts,long tailleOperateur=0);
ImageInfoCV *ZoomImage(long =64,long =64);

// Histogramme
ImageInfoCV *EgalisationHistogramme (void);

// Ajout de bruit � une image
ImageInfoCV *BruitImpulsionnel(double proba=0.25,long iMin=0,long iMax=255);
ImageInfoCV *BruitGaussienMultiplicatif(double variance=3.0);
ImageInfoCV *BruitLaplacien(double v=3.0);

// Op�ration g�om�trique
ImageInfoCV *TransformationGeometrique(double **t, char =0);

// Travaux sp�cifiques � la couleur
void DefDataRGBVersPlan(unsigned char *d);	//Conversion de plan RGB vers Plan R plan G plan B

// Filtrage de deriche et shen
ImageInfoCV *GradientDericheX(double alphaDerive=0.75,double alphaMoyenne=0.25);
ImageInfoCV *GradientDericheY(double alphaDerive=0.75,double alphaMoyenne=0.25);
// Filtrage image bruit�e Paillou
ImageInfoCV *PaillouX(double w=50,double a=51);
ImageInfoCV *PaillouY(double w=50,double a=51);
ImageInfoCV *ModuleGradientPaillou(double w=50,double a=51,ImageInfoCV * = NULL);

// Module du gradient
ImageInfoCV *ModuleGradient(ImageInfoCV &,ImageInfoCV &);
ImageInfoCV *ModuleGradientDeriche(double alphaDerive=0.75,double alphaMoyenne=0.25,ImageInfoCV * = NULL,char quadrant=0);

// Statistique nanoraptor
ImageInfoCV* ConversionCouleurEnEntier(float );

// Contour
ImageInfoCV *SuiviLigneGradient(ImageInfoCV &,ImageInfoCV &,ImageInfoCV * = NULL);


// Sp�cifique MPI
virtual void DiffusionMPI(void);

// ********* Acc�s aux membres priv�s et 
//	Lecture des membres priv�s
double *MinIm(){if (!minIm) ExtremumLoc(); return minIm;};		/*< Minimum de l'image pour chaque canal */
double *MaxIm(){if (!maxIm) ExtremumLoc();return maxIm;};		/*< Maximum de l'image pour chaque canal */
char    *LitFctImage(void);
char    *LitTypeSeuillage(void);
char    *LitTailleOperateur(void);
char    *LitCentrageResultat(void);
char	LitValCentrageResultat(void);
char    *LitTypeOndelette(void);
long	LitTailleOndelette(void);
long  	LitNbIterOperateur(void);
char  	LitConverCplxEnt(void);

cv::Mat	**StatComposante(){return statComposante;};
cv::Mat	**CentreGComposante(){return centreGComposante;};
std::vector<cv::Moments> *MomentComposante(){return moment;};	 
std::vector<std::vector<cv::Point> > *PtContours(){return contours;};
std::vector<cv::Vec4i> *ArboContour(){return arbreContour;}; /*< Arborescence des Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<double> *HuMoment(){return huMoment;};
std::vector<cv::Vec3f> *HoughCercle(){return cercle;};
std::vector<cv::Vec4i> *HoughLigneProba(){return ligneP;};
std::vector<cv::Vec2f> *HoughLigne(){return ligne;};
std::vector<cv::Point2f> *BonCoin(){return boncoin;};
std::vector<cv::Point2f> *CoinRef(bool init=false){if (init && coinRef==NULL) coinRef = new std::vector<cv::Point2f>[channels()];return coinRef;};
ParametreOperation *ParamOCVHoughLigne(ParametreOperation *p=NULL);
ParametreOperation *ParamOCVHoughLigneProba(ParametreOperation *p=NULL);
ParametreOperation *ParamOCVHoughCercle(ParametreOperation *p=NULL);
ParametreOperation *ParamOCVBonCoin(ParametreOperation *p=NULL);
ParametreOperation *ParamOCVLucasKanade(ParametreOperation *p=NULL);
int EtapeOp();  /*<! retourne l'indice de l'�tape de l'op�rateur le plus grand */
void CloneStat(ImageInfoCV *im);

// MODIFICATION d'une fonction OPENCV
void Threshold( cv::InputArray _src, cv::OutputArray _dst, double thresh, double maxval, int type ); 

};




#endif //__IMAGEINFO__
