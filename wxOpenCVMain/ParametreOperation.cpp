#include "ParametreOperation.h"
#include "ImageInfo.h"
#include <map>
#include <wx/translation.h>


using namespace std;
std::map<string,std::map<string,int> > ParametreOperation::listeParam;


void ParametreOperation::InitParamType()
{

listeParam["distance_type"].insert(std::pair<string,int>("|x1-x2| + |y1-y2|",cv::DIST_L1));
listeParam["distance_type"].insert(std::pair<string,int>(_("euclidean distance").ToStdString(),cv::DIST_L2));
listeParam["distance_type"].insert(std::pair<string,int>("max(|x1-x2|,|y1-y2|)",cv::DIST_C));
listeParam["distance_type"].insert(std::pair<string,int>("L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1))",cv::DIST_L12));
listeParam["distance_type"].insert(std::pair<string,int>("distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998",cv::DIST_FAIR));
listeParam["distance_type"].insert(std::pair<string,int>("distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846",cv::DIST_WELSCH));
listeParam["distance_type"].insert(std::pair<string,int>("distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345",cv::DIST_HUBER));

listeParam["connectivity"].insert(std::pair<string,int>(_("4-connex").ToStdString(),4));
listeParam["connectivity"].insert(std::pair<string,int>(_("8-connex").ToStdString(),8));

listeParam["adaptiveMethod"].insert(std::pair<string,int>(_("threshold  mean of neighbourhood").ToStdString(),cv::ADAPTIVE_THRESH_MEAN_C ));
listeParam["adaptiveMethod"].insert(std::pair<string,int>(_("threshold weighted sum of neighbourhood").ToStdString(),cv::ADAPTIVE_THRESH_GAUSSIAN_C));

listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>thresh maxValue else 0").ToStdString(),cv::THRESH_BINARY));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)<thresh maxValue else 0").ToStdString(),cv::THRESH_BINARY_INV));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>thresh thresh else A(x,y)").ToStdString(),cv::THRESH_TRUNC));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>threshold 0 else A(x,y)").ToStdString(),cv::THRESH_TOZERO_INV));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)<threshold 0 else A(x,y)").ToStdString(),cv::THRESH_TOZERO));

listeParam["borderType"].insert(std::pair<string,int>(_("Replicate border aa|abcde|ee").ToStdString(),cv::BORDER_REPLICATE));
listeParam["borderType"].insert(std::pair<string,int>(_("Reflect border   ba|abcde|ed").ToStdString(),cv::BORDER_REFLECT));
listeParam["borderType"].insert(std::pair<string,int>(_("Reflect border   cb|abcde|dc").ToStdString(),cv::BORDER_REFLECT_101));
listeParam["borderType"].insert(std::pair<string,int>(_("Reflect periodic de|abcde|ab").ToStdString(),cv::BORDER_WRAP));
listeParam["borderType"].insert(std::pair<string,int>(_("Constant border  zz|abcde|zz").ToStdString(),cv::BORDER_CONSTANT));

listeParam["ddepth"].insert(std::pair<string,int>(_("Same as input").ToStdString(),-1));
listeParam["ddepth"].insert(std::pair<string,int>(_("16 signed bit").ToStdString(),CV_16S));
listeParam["ddepth"].insert(std::pair<string,int>(_("32 signed bit").ToStdString(),CV_32S));
listeParam["ddepth"].insert(std::pair<string,int>("float",CV_32F));
listeParam["ddepth"].insert(std::pair<string,int>("double",CV_64F));

listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("BGR to Gray").ToStdString(),cv::COLOR_BGR2GRAY));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("RGB to Gray").ToStdString(),cv::COLOR_RGB2GRAY));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("Gray to RGB").ToStdString(),cv::COLOR_GRAY2BGR));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("BGR to Gray").ToStdString(),cv::COLOR_GRAY2RGB));



}

bool ParametreOperation::InitOperation(string s)
{
if (listeParam.size()==0)
	InitParamType();
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
indEtape=0;
idOperation=-1;
indRes=-1;
refPDF="";
lienHtml="";
indOp1Fenetre=-1;
indOp2Fenetre=-1;
indOp3Fenetre=-1;
opVideo=false;
opErreur=0;
if(	s=="cornerharris")
	{
	nomOperation=s;
	nbImageRes=1;
	nbOperande= 1;
	intParam["blockSize"]=DomaineParametreOp<int>(2,2,9,1);
	intParam["ksize"]=DomaineParametreOp<int>(1,1,7,2);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	doubleParam["k"]=DomaineParametreOp<double>(0.04,0.01,10,0.01);
	}
if(	s=="goodfeaturestotrack")
	{
	nbImageRes=0;
	nomOperation=s;
	nbOperande= 1;
	intParam["maxCorners"]=DomaineParametreOp<int>(500,1,1000,1);
	doubleParam["qualityLevel"]=DomaineParametreOp<double>(0.1,0.01,1,0.01);
	doubleParam["minDistance"]=DomaineParametreOp<double>(10,1,1000,01);
	intParam["blockSize"]=DomaineParametreOp<int>(2,2,9,1);
	intParam["useHarrisDetector"]=DomaineParametreOp<int>(1,0,1,1);
	doubleParam["k"]=DomaineParametreOp<double>(0.04,0.01,1,0.01);
	}
if(	s=="houghcircles")
	{
	nbImageRes=0;
	nomOperation=s;
	nbOperande= 1;
	intParam["method"]=DomaineParametreOp<int>(cv::HOUGH_GRADIENT,cv::HOUGH_GRADIENT,cv::HOUGH_GRADIENT,1);
	doubleParam["dp"]=DomaineParametreOp<double>(1,0.001,20,01);
	doubleParam["minDistance"]=DomaineParametreOp<double>(10,1,1000,01);
	doubleParam["param1"]=DomaineParametreOp<double>(100,3,200,0.1);
	doubleParam["param2"]=DomaineParametreOp<double>(30,3,200,0.1);
	intParam["min_radius"]=DomaineParametreOp<int>(100,0,1000,1);
	intParam["max_radius"]=DomaineParametreOp<int>(130,0,1000,1);
	}
if(	s=="houghlines")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=0;
//	intParam["method"]=DomaineParametreOp<int>(cv::HOUGH_STANDARD,cv::HOUGH_STANDARD,cv::HOUGH_MULTI_SCALE,1);
	doubleParam["rho"]=DomaineParametreOp<double>(1,1,200,01);
	doubleParam["theta"]=DomaineParametreOp<double>(0.05,0,6.28,0.01);
	intParam["threshold"]=DomaineParametreOp<int>(50,3,1000,1);
	doubleParam["srn"]=DomaineParametreOp<double>(0.0,0.00,1,0.1);
	doubleParam["stn"]=DomaineParametreOp<double>(0.0,0,1000,0.1);
	}
if(	s=="houghlinesp")
	{
	nbImageRes=0;
	nomOperation=s;
	nbOperande= 1;
	doubleParam["rho"]=DomaineParametreOp<double>(1,1,200,01);
	doubleParam["theta"]=DomaineParametreOp<double>(0.05,0,6.28,0.01);
	intParam["threshold"]=DomaineParametreOp<int>(50,3,1000,1);
	doubleParam["minLineLength"]=DomaineParametreOp<double>(10,1,1000,0.1);
	doubleParam["maxLineGap"]=DomaineParametreOp<double>(0.0,1,1000,1);
	}
if (s=="watershed")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=1;
	}
if (s=="split")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=1;
	}
if (s=="merge")
	{
	nomOperation = s;
	nbOperande= 3;
	nbImageRes=1;
	}
if (s == "bitwise-and")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-or")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-xor")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-not")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 1;
}
if (s == "add")
{
	nomOperation = s;
	nbOperande = 2;
	intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	nbImageRes = 1;
}
if (s == "AdditionPonderee")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["alpha"]=DomaineParametreOp<double>(1,0.1,10,0.1);
	doubleParam["beta"]=DomaineParametreOp<double>(1,0.1,10,0.1);
	doubleParam["gamma"]=DomaineParametreOp<double>(1,0.1,10,0.1);
	}
if (s=="subtract")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	nbOperande= 2;
	}
if (s=="multiply")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.1,10,0.1);
	nbOperande= 2;
	}
if (s=="divide")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	}
if (s=="filter2d")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["IndOpConvolution"]=DomaineParametreOp<int>(xx.IndOpConvolution(),0,NB_OP_CONVOLUTION,1);
	}
if (s=="dilate")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["IndOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	}
if (s=="erode")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["IndOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	}
if (s=="openning")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	}
if (s=="closing")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	}
if (s=="tophat")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	}
if (s=="blackhat")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	}
if (s=="morph_gradient")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	}
if (s=="scharr_mod")
	{
	nbImageRes=1;
	nomOperation=s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	}
if (s=="scharr_x")
	{
	nbImageRes=1;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
	}
if (s=="scharr_y")
	{
	nbImageRes=1;
	opUnaireSelec = &ImageInfoCV::ScharrY;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
	}
if (s=="laplacian")
	{
	nbImageRes=1;
	intParam["ddepth"]=DomaineParametreOp<int>(50.,0.0,255.0,1.0);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.0,255.0,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,255.0,1.0);
	intParam["ksize"]=DomaineParametreOp<int>(3,1,7,2);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
	}
if (s=="canny")
	{
	nbImageRes=1;
	doubleParam["threshold1"]=DomaineParametreOp<double>(50.,0.0,255.0,1.0);
	doubleParam["threshold2"]=DomaineParametreOp<double>(100,0.0,255.0,1.0);
	intParam["aperture_size"]=DomaineParametreOp<int>((int)3,(int)1,(int)255,(int)2);
	intParam["kernel_size"]=DomaineParametreOp<int>(3,1,255,2);
	nomOperation=s;
	}
if (s=="contour")
	{
	nbImageRes=1;
	intParam["mode"]=DomaineParametreOp<int>(cv::RETR_EXTERNAL,cv::RETR_EXTERNAL,cv::RETR_TREE,1);
	intParam["method"]=DomaineParametreOp<int>(cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_TC89_L1 ,1);
	nomOperation=s;
	}

if (s=="cvtcolor")
	{
	nbImageRes=1;
	intParam["ColorSpaceCode"]=DomaineParametreOp<int>(cv::COLOR_BGR2GRAY,cv::COLOR_BGR2GRAY,cv::COLOR_RGB2GRAY,1);
	nomOperation=s;
	}
if (s=="FFT")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="IFFT")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="threshold")
	{
	nbImageRes=1;
	doubleParam["thresh"]=DomaineParametreOp<double>(50.,0.0,255.0,1.0);
	doubleParam["maxval"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["threshold_type"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	nomOperation=s;
	}
if (s=="adaptivethreshold")
	{
	nbImageRes=1;
	doubleParam["maxValue"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["thresholdType"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	intParam["adaptiveMethod"]=DomaineParametreOp<int>(cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_MEAN_C,1);
	intParam["blockSize"]=DomaineParametreOp<int>(21,3,1000,2);
	doubleParam["C"]=DomaineParametreOp<double>(-0,-255,255,1);
	nomOperation=s;
	}
if (s=="medianblur")
	{
	nbImageRes=1;
	intParam["ksize"]=DomaineParametreOp<int>(3,1,255,2);
	nomOperation=s;
	}
if (s=="blur")
	{
	nbImageRes=1;
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
	}
if (s=="gaussianblur")
	{
	nbImageRes=1;
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	doubleParam["sigmaX"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	doubleParam["sigmaY"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
	}
if (s=="connectedcomponents")
	{
	nbImageRes=1;
	intParam["connectivity"]=DomaineParametreOp<int>(4,4,8,4);
	intParam["ltype"]=DomaineParametreOp<int>(CV_32S,CV_32S,CV_32S,0);
	nomOperation=s;
	}
if (s=="distancetransform")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="medianaxis")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="buildopticalflowpyramid")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="calcopticalflowpyrlk")
	{
	nbImageRes=0;
	nomOperation=s;
	opVideo=true;
	intParam["typeCriteria"]=DomaineParametreOp<int>(cv::TermCriteria::COUNT,cv::TermCriteria::COUNT,cv::TermCriteria::COUNT|cv::TermCriteria::EPS,1);
	intParam["maxCountCriteria"]=DomaineParametreOp<int>(20,1,255,1);
	doubleParam["epsilonCriteria"]=DomaineParametreOp<double>(0.0001,0.0000001,255.0,0.001);
	intParam["maxLevel"]=DomaineParametreOp<int>(3,0,8,1);
	sizeParam["winSize"]=DomaineParametreOp<cv::Size>(cv::Size(21,21),cv::Size(3,3),cv::Size(255,255),cv::Size(2,2));
	intParam["flag"]=DomaineParametreOp<int>(0,cv::OPTFLOW_USE_INITIAL_FLOW,cv::OPTFLOW_LK_GET_MIN_EIGENVALS,1);
	doubleParam["minEigThreshold"]=DomaineParametreOp<double>(0.001,0.0000001,100.0,0.001);

	}
if (s=="calcopticalflowfarneback")
	{
	nbImageRes=0;
	opVideo=true;
	nomOperation=s;
	doubleParam["pyr_scale"]=DomaineParametreOp<double>(0.5,0.1,0.80,0.1);
	intParam["levels"]=DomaineParametreOp<int>(3,1,8,1);
	sizeParam["winSize"]=DomaineParametreOp<cv::Size>(cv::Size(15,15),cv::Size(3,3),cv::Size(255,255),cv::Size(2,2));
	intParam["iterations"]=DomaineParametreOp<int>(3,1,20,1);
	intParam["poly_n"]=DomaineParametreOp<int>(5,1,20,1);
	doubleParam["poly_sigma"]=DomaineParametreOp<double>(1.2,0.0000001,100.0,0.001);
	intParam["flag"]=DomaineParametreOp<int>(0,0,cv::OPTFLOW_LK_GET_MIN_EIGENVALS,4);

	}
if (s=="estimaterigidtransform")
	{
	nbImageRes=1;
	nomOperation=s;
	}
if (s=="updatemotionhistory")
	{
	nbImageRes=1;
	nomOperation=s;
	}

if (nomOperation=="")
	return false;
InitPtrFonction();
return true;
}

ParametreOperation::ParametreOperation(string s)
{
opErreur=0;

InitOperation(s);

}


bool ParametreOperation::InitPtrFonction()
{
opAttribut=false;
wxString s(nomOperation);
if(	s=="cornerharris")
	{
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornerharris";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=327&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::DetectCoinHarris;
	}
if(	s=="goodfeaturestotrack")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=329&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::BonAttributs;
	}
if(	s=="houghcircles")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=330&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::HoughCercle;
	}
if(	s=="houghlines")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=332&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::HoughLigne;
	}
if(	s=="houghlinesp")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=333&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::HoughLigneProba;
	}
if (s=="cvtcolor ")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#cvtcolor";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=283&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::RGB_L;
	}
if (s=="watershed")
	{
	opBinaireSelec = &ImageInfoCV::PartageEaux;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#watershed";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=294&zoom=70,250,100";
	return true;
	}
if (s=="split")
	{
	opSurjecUnaire = &ImageInfoCV::SeparationPlan;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#split";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
	}
if (s=="merge")
	{
	opNaireSelec = &ImageInfoCV::FusionPlan;
	nbOperande= 3;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#merge";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=149&zoom=70,250,100";
	return true;
	}
if (s == "bitwise-and")
{
	opBinaireSelec = &ImageInfoCV::EtLogique;
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-and";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=124&zoom=70,250,100";
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-or")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-or";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=125&zoom=70,250,100";
	opBinaireSelec = &ImageInfoCV::OuLogique;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-xor")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-xor";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=126&zoom=70,250,100";
	opBinaireSelec = &ImageInfoCV::OuExcluLogique;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-not")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-not";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=126&zoom=70,250,100";

	opUnaireSelec = &ImageInfoCV::Negation;
	nbOperande = 1;
	nbImageRes = 1;
}
if (s == "add")
	{
	opBinaireSelec = &ImageInfoCV::Add;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#add";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
	}
if (s=="subtract")
	{
	opBinaireSelec = &ImageInfoCV::Sub;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#subtract";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=172&zoom=70,250,100";
	return true;
	}
if (s=="multiply")
	{
	opBinaireSelec = &ImageInfoCV::Mul;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#multiply";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=153&zoom=70,250,100";
	return true;
	}
if (s=="divide")
	{
	opBinaireSelec = &ImageInfoCV::Div;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#divide";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=136&zoom=70,250,100";
	return true;
	}
if (s=="filter2d")
	{
	opBinaireSelec = &ImageInfoCV::Convolution;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	return true;
	}
if (s=="dilate")
	{
	opBinaireSelec = &ImageInfoCV::Dilatation;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	return true;
	}
if (s=="erode")
	{
	opBinaireSelec = &ImageInfoCV::Erosion;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	return true;
	}
if (s=="openning")
	{
	opBinaireSelec = &ImageInfoCV::Ouverture;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=morphologyex#morphologyex";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="closing")
	{
	opBinaireSelec = &ImageInfoCV::Fermeture;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="tophat")
	{
	opBinaireSelec = &ImageInfoCV::ChapeauHaut;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="blackhat")
	{
	opBinaireSelec = &ImageInfoCV::ChapeauBas;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="morph_gradient")
	{
	opBinaireSelec = &ImageInfoCV::GradMorph;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="scharr_mod")
	{
	opUnaireSelec = &ImageInfoCV::ScharrModule;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="scharr_x")
	{
	opUnaireSelec = &ImageInfoCV::ScharrX;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="scharr_y")
	{
	opUnaireSelec = &ImageInfoCV::ScharrY;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="laplacian")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html#laplacian";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=261&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::Laplacien;
	return true;
	}
if (s=="canny")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=331&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::Canny;
	return true;
	}
if (s=="contour")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#findContour";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=308&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::Contour;
	return true;
	}
if (s=="FFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::FFT;
	return true;
	}
if (s=="IFFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::IFFT;
	return true;
	}
if (s=="threshold")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#threshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=291&zoom=70,250,100";
	nomOperation=s;
	opUnaireSelec = &ImageInfoCV::Seuillage;
	return true;
	}
if (s=="adaptivethreshold")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#adaptivethreshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=280&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::SeuillageAdaptatif;
	return true;
	}
if (s=="medianblur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=medianblur#medianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LissageMedian;
	return true;
	}
if (s=="blur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=blur#blur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LissageMoyenne;
	return true;
	}
if (s=="gaussianblur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=gaussianblur#gaussianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=263&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LissageGaussien;
	return true;
	}
if (s=="connectedcomponents")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#connectedcomponents";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=263&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::ComposanteConnexe;
	return true;
	}
if (s=="distancetransform")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::DistanceDiscrete;
	return true;
	}
if (s=="medianaxis")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	return true;
	}
if (s=="buildopticalflowpyramid")
	{
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#buildopticalflowpyramid";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=366&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	return true;
	}
if (s=="calcopticalflowpyrlk")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowpyrlk";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=365&zoom=70,250,100";
	opBinaireSelec = &ImageInfoCV::FlotOptiqueLucasKanadePyramide;
	opVideo=true;

	return true;
	}
if (s=="calcopticalflowfarneback")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	opBinaireSelec = &ImageInfoCV::FlotOptiqueFarnerback;
	opVideo=true;

	return true;
	}
if (s=="estimaterigidtransform")
	{
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#estimaterigidtransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=369&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	return true;
	}
if (s=="updatemotionhistory")
	{
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#updatemotionhistory";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=370&zoom=70,250,100";
	opUnaireSelec = &ImageInfoCV::LigneMediane;
	return true;
	}

return false;
}


ImageInfoCV	**ParametreOperation::ExecuterOperation()
{
ImageInfoCV	*im=NULL;
ImageInfoCV	**imTab=NULL;
nbImageRes=1;
if (opNaireSelec)
	{
	ImageInfoCV *imOp[3]={op1,op2,op3};
	try
		{
		im =(op1->*opNaireSelec) (3,imOp,this);
		}
	catch(cv::Exception& e)
		{
		}

	if (im==NULL)
		{
/*		opNaireSelec=NULL;
		op1=NULL;
		op2=NULL;
		op3=NULL;*/
		return imTab;
		}
	else
		{
		imTab = new ImageInfoCV*[1];
		imTab[0]=im;
		}
	}

if (opBinaireSelec)
	{
	
	try
		{
		if (op1 && op2==NULL)
			im =(op1->*opBinaireSelec)(op1,NULL,this);
		else if (op1)
			im =(op1->*opBinaireSelec)(op1,op2,this);
        else 
            im=NULL;
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		}

	if (im==NULL)
		{
/*		opBinaireSelec=NULL;
		op1=NULL;
		op2=NULL;*/
		return imTab;
		}
	else
		{
		imTab = new ImageInfoCV*[1];
		imTab[0]=im;
		}
	}
if (opUnaireSelec)
	{
	try
		{

		im =(op1->*opUnaireSelec)(op1,this);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		}

	if (im==NULL)
		{
		return imTab;
		}
	else
		{
		imTab = new ImageInfoCV*[1];
		imTab[0]=im;
		}
	}

if (opSurjecUnaire)
	{
	try
		{

		imTab =(op1->*opSurjecUnaire)(op1,this);
		}
	catch(cv::Exception& e)
		{
		wxString s(e.msg);

		}

	if (imTab==NULL)
		{
		return imTab;
		}
	}

return imTab; // Le pointeur imTab n'est pas libéré
}
