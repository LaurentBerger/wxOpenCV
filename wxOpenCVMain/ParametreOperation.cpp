#include "ParametreOperation.h"
#include "ImageInfo.h"
#include <map>
#include <wx/translation.h>

std::map<wxString,std::map<wxString,int> > ParametreOperation::listeParam;


void ParametreOperation::InitParamType()
{

listeParam["connectivity"].insert(std::pair<wxString,int>(_("4-connex"),4));
listeParam["connectivity"].insert(std::pair<wxString,int>(_("8-connex"),8));

listeParam["adaptiveMethod"].insert(std::pair<wxString,int>(_("threshold  mean of neighbourhood"),cv::ADAPTIVE_THRESH_MEAN_C ));
listeParam["adaptiveMethod"].insert(std::pair<wxString,int>(_("threshold weighted sum of neighbourhood"),cv::ADAPTIVE_THRESH_GAUSSIAN_C));

listeParam["threshold_type"].insert(std::pair<wxString,int>(_("A(x,y)>thresh maxValue else 0"),cv::THRESH_BINARY));
listeParam["threshold_type"].insert(std::pair<wxString,int>(_("A(x,y)<thresh maxValue else 0"),cv::THRESH_BINARY_INV));
listeParam["threshold_type"].insert(std::pair<wxString,int>(_("A(x,y)>thresh thresh else A(x,y)"),cv::THRESH_TRUNC));
listeParam["threshold_type"].insert(std::pair<wxString,int>(_("A(x,y)>threshold 0 else A(x,y)"),cv::THRESH_TOZERO_INV));
listeParam["threshold_type"].insert(std::pair<wxString,int>(_("A(x,y)<threshold 0 else A(x,y)"),cv::THRESH_TOZERO));

listeParam["borderType"].insert(std::pair<wxString,int>(_("Replicate border aa|abcde|ee"),cv::BORDER_REPLICATE));
listeParam["borderType"].insert(std::pair<wxString,int>(_("Reflect border   ba|abcde|ed"),cv::BORDER_REFLECT));
listeParam["borderType"].insert(std::pair<wxString,int>(_("Reflect border   cb|abcde|dc"),cv::BORDER_REFLECT_101));
listeParam["borderType"].insert(std::pair<wxString,int>(_("Reflect periodic de|abcde|ab"),cv::BORDER_WRAP));
listeParam["borderType"].insert(std::pair<wxString,int>(_("Constant border  zz|abcde|zz"),cv::BORDER_CONSTANT));

listeParam["ddepth"].insert(std::pair<wxString,int>(_("Same as input"),-1));
listeParam["ddepth"].insert(std::pair<wxString,int>(_("16 signed bit"),CV_16S));
listeParam["ddepth"].insert(std::pair<wxString,int>(_("32 signed bit"),CV_32S));
listeParam["ddepth"].insert(std::pair<wxString,int>("float",CV_32F));
listeParam["ddepth"].insert(std::pair<wxString,int>("double",CV_64F));

listeParam["distance_type"].insert(std::pair<wxString,int>("|x1-x2| + |y1-y2|",cv::DIST_L1));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("euclidean distance"),cv::DIST_L2));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("max(|x1-x2|,|y1-y2|)"),cv::DIST_C));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1))"),cv::DIST_L12));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998"),cv::DIST_FAIR));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846"),cv::DIST_WELSCH));
listeParam["distance_type"].insert(std::pair<wxString,int>(_("distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345"),cv::DIST_HUBER));


}

bool ParametreOperation::InitOperation(string s)
{
ImageInfoCV xx;
op1=NULL;
op2=NULL;
op3=NULL;
opSurjecMultiple=NULL;
opNaireSelec=NULL;
opBinaireSelec=NULL;
opUnaireSelec=NULL;
opSurjecUnaire=NULL;
nomOperation="";
nomSequence="";
nbImageRes=0;
nbOperande=0;
refPDF="";
lienHtml="";
indOp1Fenetre=-1;
indOp2Fenetre=-1;
indOp3Fenetre=-1;

if (s=="PartageEaux")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::PartageEaux;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#watershed";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=294&zoom=70,250,100";
	}
if (s=="SeparationPlan")
	{
	nomOperation=s;
	opSurjecUnaire = &ImageInfoCV::SeparationPlan;
	nbOperande= 1;
	nomOperation = s;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#split";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	}
if (s=="FusionPlan")
	{
	opNaireSelec = &ImageInfoCV::FusionPlan;
	nomOperation = s;
	nbOperande= 3;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#merge";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=149&zoom=70,250,100";
	}
if (s=="Addition")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Add;
	nomOperation = s;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#add";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	}
if (s=="Soustraction")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Sub;
	nomOperation = s;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#subtract";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=172&zoom=70,250,100";
	}
if (s=="Multiplication")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Mul;
	nomOperation = s;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#multiply";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=153&zoom=70,250,100";
	}
if (s=="Division")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Div;
	nomOperation = s;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#divide";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=136&zoom=70,250,100";
	}
if (s=="Convolution")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Convolution;
	nomOperation = s;
	nbOperande= 2;
	intParam["IndOpConvolution"]=DomaineParametreOp<int>(xx.IndOpConvolution(),0,NB_OP_CONVOLUTION,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	}
if (s=="Dilatation")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Dilatation;
	nomOperation = s;
	nbOperande= 2;
	intParam["IndOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	}
if (s=="Erosion")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Erosion;
	nbOperande= 2;
	nomOperation = s;
	intParam["IndOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	}
if (s=="Ouverture")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Ouverture;
	nomOperation = s;
	nbOperande= 2;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=morphologyex#morphologyex";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s=="Fermeture")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::Fermeture;
	nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s=="ChapeauHaut")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::ChapeauHaut;
	nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s=="ChapeauBas")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::ChapeauBas;
	nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s=="GradMorph")
	{
	nomOperation=s;
	opBinaireSelec = &ImageInfoCV::GradMorph;
	nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	}
if (s=="Scharr_mod")
	{
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::ScharrModule;
	nomOperation = s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	}
if (s=="Scharr_x")
	{
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::ScharrX;
	nomOperation = s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	}
if (s=="Scharr_y")
	{
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::ScharrY;
	nomOperation = s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	}
if (s=="Laplacien")
	{
	intParam["ddepth"]=DomaineParametreOp<int>(50.,0.0,255.0,1.0);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.0,255.0,1.0);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,255.0,1.0);
	intParam["ksize"]=DomaineParametreOp<int>(3,1,7,2);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html#laplacian";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=261&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Laplacien;
	nomOperation = s;
	}
if (s=="Canny")
	{
	doubleParam["threshold1"]=DomaineParametreOp<double>(50.,0.0,255.0,1.0);
	doubleParam["threshold2"]=DomaineParametreOp<double>(100,0.0,255.0,1.0);
	intParam["aperture_size"]=DomaineParametreOp<int>((int)3,(int)1,(int)255,(int)2);
	intParam["kernel_size"]=DomaineParametreOp<int>(3,1,255,2);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=331&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Canny;
	nomOperation = s;
	}
if (s=="Contour")
	{
	intParam["mode"]=DomaineParametreOp<int>(cv::RETR_EXTERNAL,cv::RETR_EXTERNAL,cv::RETR_TREE,1);
	intParam["method"]=DomaineParametreOp<int>(cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_TC89_L1 ,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#findContour";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=308&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Contour;
	nomOperation = s;
	}
if (s=="FFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::FFT;
	nomOperation = s;
	}
if (s=="IFFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="The OpenCV Reference Manual, Release 2.4.9.0 p325";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::IFFT;
	nomOperation = s;
	}
if (s=="Seuillage")
	{
	doubleParam["thresh"]=DomaineParametreOp<double>(50.,0.0,255.0,1.0);
	doubleParam["maxval"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["threshold_type"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#threshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=291&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Seuillage;
	nomOperation = s;
	}
if (s=="AdaptatifSeuillage")
	{
	doubleParam["maxValue"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["thresholdType"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	intParam["adaptiveMethod"]=DomaineParametreOp<int>(cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_MEAN_C,1);
	intParam["blockSize"]=DomaineParametreOp<int>(21,3,1000,2);
	doubleParam["C"]=DomaineParametreOp<double>(-0,-255,255,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#adaptivethreshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=280&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::SeuillageAdaptatif;
	nomOperation = s;
	}
if (s=="LissageMedian")
	{
	intParam["ksize"]=DomaineParametreOp<int>(3,1,255,2);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=medianblur#medianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageMedian;
	nomOperation = s;
	}
if (s=="LissageMoyenneur")
	{
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=blur#blur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageMoyenne;
	nomOperation = s;
	}
if (s=="LissageGaussien")
	{
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	doubleParam["sigmaX"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	doubleParam["sigmaY"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=gaussianblur#gaussianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=263&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LissageGaussien;
	nomOperation = s;
	}
if (s=="ComposanteConnexe")
	{
	intParam["connectivity"]=DomaineParametreOp<int>(4,4,8,4);
	intParam["ltype"]=DomaineParametreOp<int>(CV_32S,CV_32S,CV_32S,0);
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#connectedcomponents";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=263&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::ComposanteConnexe;
	nomOperation = s;
	}
if (s=="DistanceDiscrete")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::DistanceDiscrete;
	nomOperation = s;
	}
if (s=="LigneMediane")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	nomOperation = s;
	}
if (nomOperation=="")
	return false;
return true;
}

ParametreOperation::ParametreOperation(string s)
{
InitOperation(s);

}


bool ParametreOperation::InitPtrFonction()
{
wxString s(nomOperation);
if (s=="PartageEaux")
	{
	opBinaireSelec = &ImageInfoCV::PartageEaux;
	return true;
	}
if (s=="SeparationPlan")
	{
	opSurjecUnaire = &ImageInfoCV::SeparationPlan;
	return true;
	}
if (s=="FusionPlan")
	{
	opNaireSelec = &ImageInfoCV::FusionPlan;
	return true;
	}
if (s=="Addition")
	{
	opBinaireSelec = &ImageInfoCV::Add;
	return true;
	}
if (s=="Soustraction")
	{
	opBinaireSelec = &ImageInfoCV::Sub;
	return true;
	}
if (s=="Multiplication")
	{
	opBinaireSelec = &ImageInfoCV::Mul;
	return true;
	}
if (s=="Division")
	{
	opBinaireSelec = &ImageInfoCV::Div;
	return true;
	}
if (s=="Convolution")
	{
	opBinaireSelec = &ImageInfoCV::Convolution;
	return true;
	}
if (s=="Dilatation")
	{
	opBinaireSelec = &ImageInfoCV::Dilatation;
	return true;
	}
if (s=="Erosion")
	{
	opBinaireSelec = &ImageInfoCV::Erosion;
	return true;
	}
if (s=="Ouverture")
	{
	opBinaireSelec = &ImageInfoCV::Ouverture;
	return true;
	}
if (s=="Fermeture")
	{
	opBinaireSelec = &ImageInfoCV::Fermeture;
	return true;
	}
if (s=="ChapeauHaut")
	{
	opBinaireSelec = &ImageInfoCV::ChapeauHaut;
	return true;
	}
if (s=="ChapeauBas")
	{
	opBinaireSelec = &ImageInfoCV::ChapeauBas;
	return true;
	}
if (s=="GradMorph")
	{
	opBinaireSelec = &ImageInfoCV::GradMorph;
	return true;
	}
if (s=="Scharr_mod")
	{
	opUnaireSelec = &ImageInfoCV::ScharrModule;
	return true;
	}
if (s=="Scharr_x")
	{
	opUnaireSelec = &ImageInfoCV::ScharrX;
	return true;
	}
if (s=="Scharr_y")
	{
	opUnaireSelec = &ImageInfoCV::ScharrY;
	return true;
	}
if (s=="Laplacien")
	{
	opUnaireSelec = &ImageInfoCV::Laplacien;
	return true;
	}
if (s=="Canny")
	{
	opUnaireSelec = &ImageInfoCV::Canny;
	return true;
	}
if (s=="Contour")
	{
	opUnaireSelec = &ImageInfoCV::Contour;
	return true;
	}
if (s=="FFT")
	{
	opUnaireSelec = &ImageInfoCV::FFT;
	return true;
	}
if (s=="IFFT")
	{
	opUnaireSelec = &ImageInfoCV::IFFT;
	return true;
	}
if (s=="Seuillage")
	{
	opUnaireSelec = &ImageInfoCV::Seuillage;
	return true;
	}
if (s=="AdaptatifSeuillage")
	{
	opUnaireSelec = &ImageInfoCV::SeuillageAdaptatif;
	return true;
	}
if (s=="LissageMedian")
	{
	opUnaireSelec = &ImageInfoCV::LissageMedian;
	return true;
	}
if (s=="LissageMoyenneur")
	{
	opUnaireSelec = &ImageInfoCV::LissageMoyenne;
	return true;
	}
if (s=="LissageGaussien")
	{
	opUnaireSelec = &ImageInfoCV::LissageGaussien;
	return true;
	}
if (s=="ComposanteConnexe")
	{
	opUnaireSelec = &ImageInfoCV::ComposanteConnexe;
	return true;
	}
if (s=="DistanceDiscrete")
	{
	opUnaireSelec = &ImageInfoCV::DistanceDiscrete;
	return true;
	}
if (s=="LigneMediane")
	{
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	return true;
	}
return false;
}