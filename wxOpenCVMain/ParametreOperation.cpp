#include "ParametreOperation.h"
#include "ImageInfo.h"
#include <map>
#ifndef __SCILABOPENCV__
#include <wx/translation.h>
#endif


using namespace std;

map<string,map<string,int> > ParametreOperation::listeParam;
map<std::string, ParametreOperation> ImageInfoCV::listeOperation;

#ifndef __WIN32__ // en rélaité C++11
std::string to_string(double x)
{
std::ostringstream convert;   // stream used for the conversion

convert << x;      // insert the textual representation of 'Number' in the characters in the stream

return convert.str(); // set 'Result' to the contents of the stream
}

#endif


void ParametreOperation::write(cv::FileStorage& fs) const {
    string s("Operation");
    s+= to_string(indEtape);
    fs<<s<<"{:";
    fs<< "op"<< this->nomOperation;
    fs << "nbOperande" << nbOperande;
    for (int i = 0; i < nbOperande; i++)
    {
        string s("op");
        s += to_string(i);
        fs << s << indOpFenetre[i];
    }
    fs << "res" << indRes;
    fs << "indEtape" << indEtape;
    fs << "idOperation" << idOperation;
    fs << "nomSequence" << nomSequence;
    int nb = 0;
    fs << "intParam" << "[";
#ifdef __WIN32__ // en realité C++11
    for (auto iti = intParam.begin(); iti != intParam.end(); iti++)
        {
        fs << "{:" << "nom"<<iti->first;
        fs << "valeur" << iti->second.valeur;
        fs << "minVal" << iti->second.mini;
        fs << "maxVal" << iti->second.maxi;
        fs << "pasVal" << iti->second.pas<<"}";
        }
    fs << "]" << "doubleParam" << "[";
    nb = 0;
    map<string, DomaineParametreOp<double> >::iterator itd;
    for (auto iti = doubleParam.begin(); iti != doubleParam.end(); iti++)
        {
        fs << "{:" << "nom" << iti->first;
        fs << "valeur" << iti->second.valeur;
        fs << "minVal" << iti->second.mini;
        fs << "maxVal" << iti->second.maxi;
        fs << "pasVal" << iti->second.pas << "}";
        nb++;
        }
    fs << "]" << "sizeParam" << "[";
    nb = 0;
    for (auto itp = sizeParam.begin(); itp != sizeParam.end(); itp++)
        {
        fs << "{:" << "nom" << itp->first;
        fs << "largeur" << itp->second.valeur.width;
        fs << "hauteur" << itp->second.valeur.height;
        fs << "larMin" << itp->second.mini.width;
        fs << "hauMin" << itp->second.mini.height;
        fs << "larMax" << itp->second.maxi.width;
        fs << "hauMax" << itp->second.maxi.height;
        fs << "larPas" << itp->second.pas.width;
        fs << "hauPas" << itp->second.pas.height << "}";;
        nb++;
        }
    std::map<std::string, DomaineParametreOp<cv::Point> >::iterator itp;
    fs << "]" << "pointParam" << "[";
    for (auto itp = pointParam.begin(); itp != pointParam.end(); itp++)
        {
        fs << "{:" << "nom" << itp->first;
        fs << "largeur" << itp->second.valeur.x;
        fs << "hauteur" << itp->second.valeur.y;
        fs << "larMin" << itp->second.mini.x;
        fs << "hauMin" << itp->second.mini.y;
        fs << "larMax" << itp->second.maxi.x;
        fs << "hauMax" << itp->second.maxi.y;
        fs << "larPas" << itp->second.pas.x;
        fs << "hauPas" << itp->second.pas.y << "}";;
        }
#else
{
std::map<std::string,DomaineParametreOp<int> >::const_iterator iti;
    for ( iti = intParam.begin(); iti != intParam.end(); iti++)
        {
        fs << "{:" << "nom"<<iti->first;
        fs << "valeur" << iti->second.valeur;
        fs << "minVal" << iti->second.mini;
        fs << "maxVal" << iti->second.maxi;
        fs << "pasVal" << iti->second.pas<<"}";
        }
 }
    fs << "]" << "doubleParam" << "[";
    nb = 0;
 {
 std::map<std::string, DomaineParametreOp<double> >::const_iterator iti;
    for (iti = doubleParam.begin(); iti != doubleParam.end(); iti++)
        {
        fs << "{:" << "nom" << iti->first;
        fs << "valeur" << iti->second.valeur;
        fs << "minVal" << iti->second.mini;
        fs << "maxVal" << iti->second.maxi;
        fs << "pasVal" << iti->second.pas << "}";
        nb++;
        }
 }
    fs << "]" << "sizeParam" << "[";
    nb = 0;
    for (map<string, DomaineParametreOp<cv::Size> >::const_iterator itp = sizeParam.begin(); itp != sizeParam.end(); itp++)
        {
        fs << "{:" << "nom" << itp->first;
        fs << "largeur" << itp->second.valeur.width;
        fs << "hauteur" << itp->second.valeur.height;
        fs << "larMin" << itp->second.mini.width;
        fs << "hauMin" << itp->second.mini.height;
        fs << "larMax" << itp->second.maxi.width;
        fs << "hauMax" << itp->second.maxi.height;
        fs << "larPas" << itp->second.pas.width;
        fs << "hauPas" << itp->second.pas.height << "}";;
        nb++;
        }
    fs << "]" << "pointParam" << "[";
    for (map<string, DomaineParametreOp<cv::Point> >::const_iterator itp = pointParam.begin(); itp != pointParam.end(); itp++)
        {
        fs << "{:" << "nom" << itp->first;
        fs << "largeur" << itp->second.valeur.x;
        fs << "hauteur" << itp->second.valeur.y;
        fs << "larMin" << itp->second.mini.x;
        fs << "hauMin" << itp->second.mini.y;
        fs << "larMax" << itp->second.maxi.x;
        fs << "hauMax" << itp->second.maxi.y;
        fs << "larPas" << itp->second.pas.x;
        fs << "hauPas" << itp->second.pas.y << "}";;
        }

#endif
    fs << "]" ;
    fs << "}";

}

void ParametreOperation::read(const cv::FileNode& node)                          //Read serialization for this class
{
    int nbEtape=0;
    string s;
    if (!node.empty())
    {
        cv::FileNode op = node;
        nomOperation=(string)op["op"];
        nbOperande= op["nbOperande"];
        indOpFenetre.resize(nbOperande);
        this->op.resize(nbOperande);
        for (int i = 0; i < nbOperande; i++)
        {
            string s("op");
            s += to_string(i);
            indOpFenetre[i] = op[s];
            this->op[i] = NULL;
        }
        indRes = (int)op["res"];
        indEtape = (int)op["indEtape"];
        idOperation = (int)op["idOperation"];
        nomSequence = (string)op["nomSequence"];
        cv::FileNode opl = op["intParam"];
        cv::FileNodeIterator it = opl.begin(), it_end = opl.end();
        for (; it != it_end; ++it)
            {
            intParam[(string)(*it)["nom"]] = DomaineParametreOp<int>((*it)["valeur"], (*it)["minVal"], (*it)["maxVal"], (*it)["pasVal"]);;
            }
        opl = op["doubleParam"];
        it = opl.begin(), it_end = opl.end();
        for (; it != it_end; ++it)
            {
            doubleParam[(string)(*it)["nom"]] = DomaineParametreOp<double>((double)(*it)["valeur"], (double)(*it)["minVal"], (double)(*it)["maxVal"], (double)(*it)["pasVal"]);
            }
        opl = op["sizeParam"];
        it = opl.begin(), it_end = opl.end();
        for (; it != it_end; ++it)
            {
            int w11 = (*it)["largeur"], w12 = (*it)["hauteur"];
            int w21 = (*it)["larMin"], w22 = (*it)["hauMin"];
            int w31 = (*it)["larMax"], w32 = (*it)["hauMax"];
            int w41 = (*it)["larPas"], w42 = (*it)["hauPas"];
            sizeParam[(string)(*it)["nom"]] = DomaineParametreOp<cv::Size>(cv::Size(w11, w12), cv::Size(w21, w22), cv::Size(w31, w32), cv::Size(w41, w42));
            }
        opl = op["pointParam"];
        it = opl.begin(), it_end = opl.end();
        for (; it != it_end; ++it)
            {
            int w11 = (*it)["largeur"], w12 = (*it)["hauteur"];
            int w21 = (*it)["larMin"], w22 = (*it)["hauMin"];
            int w31 = (*it)["larMax"], w32 = (*it)["hauMax"];
            int w41 = (*it)["larPas"], w42 = (*it)["hauPas"];
            pointParam[(string)(*it)["nom"]] = DomaineParametreOp<cv::Point>(cv::Point(w11, w12), cv::Point(w21, w22), cv::Point(w31, w32), cv::Point(w41, w42));
            }
        }
}



void ParametreOperation::InitParamType()
{
#ifndef __SCILABOPENCV__
listeParam["distance_type"].insert(std::pair<string,int>("|x1-x2| + |y1-y2|",cv::DIST_L1));
listeParam["distance_type"].insert(std::pair<string,int>(_("euclidean distance").ToStdString(),cv::DIST_L2));
listeParam["distance_type"].insert(std::pair<string,int>("max(|x1-x2|,|y1-y2|)",cv::DIST_C));
listeParam["distance_type"].insert(std::pair<string,int>("L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1))",cv::DIST_L12));
listeParam["distance_type"].insert(std::pair<string,int>("distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998",cv::DIST_FAIR));
listeParam["distance_type"].insert(std::pair<string,int>("distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846",cv::DIST_WELSCH));
listeParam["distance_type"].insert(std::pair<string,int>("distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345",cv::DIST_HUBER));

listeParam["normType"].insert(std::pair<string,int>(_("L1 Distance").ToStdString(),cv::NORM_L1));
listeParam["normType"].insert(std::pair<string,int>(_("L2 Distance").ToStdString(),cv::NORM_L2));
listeParam["normType"].insert(std::pair<string,int>(_("Hamming distance").ToStdString(),cv::NORM_HAMMING));
listeParam["normType"].insert(std::pair<string,int>(_("Hamming 2 distance").ToStdString(),cv::NORM_HAMMING2));
listeParam["normType"].insert(std::pair<string,int>(_("auto").ToStdString(),-1));



listeParam["connectivity"].insert(std::pair<string,int>(_("4-connex").ToStdString(),4));
listeParam["connectivity"].insert(std::pair<string,int>(_("8-connex").ToStdString(),8));

listeParam["adaptiveMethod"].insert(std::pair<string,int>(_("threshold  mean of neighbourhood").ToStdString(),cv::ADAPTIVE_THRESH_MEAN_C ));
listeParam["adaptiveMethod"].insert(std::pair<string,int>(_("threshold weighted sum of neighbourhood").ToStdString(),cv::ADAPTIVE_THRESH_GAUSSIAN_C));

listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>thresh maxValue else 0").ToStdString(),cv::THRESH_BINARY));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)<thresh maxValue else 0").ToStdString(),cv::THRESH_BINARY_INV));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>thresh thresh else A(x,y)").ToStdString(),cv::THRESH_TRUNC));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)>threshold 0 else A(x,y)").ToStdString(),cv::THRESH_TOZERO_INV));
listeParam["threshold_type"].insert(std::pair<string,int>(_("A(x,y)<threshold 0 else A(x,y)").ToStdString(),cv::THRESH_TOZERO));

listeParam["OTSU"].insert(std::pair<string,int>(_("manual threshold").ToStdString(),0));
listeParam["OTSU"].insert(std::pair<string,int>(_("Automatic threshold").ToStdString(),1));

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

listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("add alpha channel to RGB ").ToStdString(), cv::COLOR_BGR2BGRA));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("remove alpha channel ").ToStdString(), cv::COLOR_BGRA2BGR));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("convert between RGB and BGR color spaces (with or without alpha channel) ").ToStdString(), cv::COLOR_BGR2RGBA));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("BGR to Gray").ToStdString(),cv::COLOR_BGR2GRAY));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("RGB to Gray").ToStdString(),cv::COLOR_RGB2GRAY));
listeParam["ColorSpaceCode"].insert(std::pair<string,int>(_("Gray to RGB").ToStdString(),cv::COLOR_GRAY2BGR));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Convert BGR to YCrCb").ToStdString(), cv::COLOR_BGR2YCrCb));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Convert RGB  to YCrCb").ToStdString(), cv::COLOR_RGB2YCrCb));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Convert YCrCb  to BGR").ToStdString(), cv::COLOR_YCrCb2BGR));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Convert YCrCb  to RGB").ToStdString(), cv::COLOR_YCrCb2RGB));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("BGR to HSV").ToStdString(), cv::COLOR_BGR2HSV));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("RGB to HSV").ToStdString(), cv::COLOR_RGB2HSV));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("BGR to Yuv").ToStdString(), cv::COLOR_BGR2YUV));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("RGB to Yuv").ToStdString(), cv::COLOR_RGB2YUV));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv to RGB").ToStdString(), cv::COLOR_YUV2BGR));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv to BGR").ToStdString(), cv::COLOR_YUV2RGB));

listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv(422) to RGB").ToStdString(), cv::COLOR_YUV2RGB_UYVY));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv(422) to BGR").ToStdString(), cv::COLOR_YUV2BGR_UYVY));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv to RGB (NV12)").ToStdString(), cv::COLOR_YUV2RGB_NV12));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Yuv to BGR (NV12)").ToStdString(), cv::COLOR_YUV2BGR_NV12));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("BGR to Lab").ToStdString(), cv::COLOR_BGR2Lab));
listeParam["ColorSpaceCode"].insert(std::pair<string, int>(_("Lab to BGR").ToStdString(), cv::COLOR_Lab2BGR));




listeParam["InterpolationFlags"].insert(std::pair<string, int>(_("nearest neighbor interpolation").ToStdString(), cv::INTER_NEAREST));
listeParam["InterpolationFlags"].insert(std::pair<string, int>(_("bilinear interpolation").ToStdString(), cv::INTER_LINEAR));
listeParam["InterpolationFlags"].insert(std::pair<string, int>(_("bicubic interpolation").ToStdString(), cv::INTER_CUBIC));
listeParam["InterpolationFlags"].insert(std::pair<string, int>(_("resampling using pixel area relation").ToStdString(), cv::INTER_AREA));
listeParam["InterpolationFlags"].insert(std::pair<string, int>(_("Lanczos interpolation over 8x8 neighborhood").ToStdString(), CV_INTER_LANCZOS4));

listeParam["ResultImage"].insert(std::pair<string, int>(_("Mask").ToStdString(), 0));
listeParam["ResultImage"].insert(std::pair<string, int>(_("Background").ToStdString(), 1));
listeParam["ResultImage"].insert(std::pair<string, int>(_("Foreground").ToStdString(), 2));

listeParam["diffusivity"].insert(std::pair<string, int>(_("DIFF_PM_G1").ToStdString(), 0));
listeParam["diffusivity"].insert(std::pair<string, int>(_("DIFF_PM_G2").ToStdString(), 1));
listeParam["diffusivity"].insert(std::pair<string, int>(_("DIFF_WEICKERT").ToStdString(), 2));
listeParam["diffusivity"].insert(std::pair<string, int>(_("DIFF_CHARBONNIER").ToStdString(), 2));

listeParam["ba_cost_func"].insert(std::pair<string, int>(_("reproj").ToStdString(), 0));
listeParam["ba_cost_func"].insert(std::pair<string, int>(_("ray").ToStdString(), 1));

listeParam["Stitch_descriptor"].insert(std::pair<string, int>(_("orb").ToStdString(), 0));
listeParam["Stitch_descriptor"].insert(std::pair<string, int>(_("surf").ToStdString(), 1));

listeParam["warp_type"].insert(std::pair<string, int>(_("plane").ToStdString(), 0));
listeParam["warp_type"].insert(std::pair<string, int>(_("cylindrical").ToStdString(), 1));
listeParam["warp_type"].insert(std::pair<string, int>(_("spherical").ToStdString(), 2));
listeParam["warp_type"].insert(std::pair<string, int>(_("fisheye").ToStdString(), 3));
listeParam["warp_type"].insert(std::pair<string, int>(_("stereographic").ToStdString(), 4));
listeParam["warp_type"].insert(std::pair<string, int>(_("compressedPlaneA2B1").ToStdString(), 5));
listeParam["warp_type"].insert(std::pair<string, int>(_("compressedPlaneA1.5B1").ToStdString(), 6));
listeParam["warp_type"].insert(std::pair<string, int>(_("compressedPlanePortraitA2B1").ToStdString(), 7));
listeParam["warp_type"].insert(std::pair<string, int>(_("compressedPlanePortraitA1.5B1").ToStdString(), 8));
listeParam["warp_type"].insert(std::pair<string, int>(_("paniniA2B1").ToStdString(), 9));
listeParam["warp_type"].insert(std::pair<string, int>(_("paniniA1.5B1").ToStdString(), 10));
listeParam["warp_type"].insert(std::pair<string, int>(_("paniniPortraitA2B1").ToStdString(), 11));
listeParam["warp_type"].insert(std::pair<string, int>(_("paniniPortraitA1.5B1").ToStdString(), 12));
listeParam["warp_type"].insert(std::pair<string, int>(_("mercator").ToStdString(), 13));
listeParam["warp_type"].insert(std::pair<string, int>(_("transverseMercator").ToStdString(), 14));

listeParam["seam_find_type"].insert(std::pair<string, int>(_("no").ToStdString(), 0));
listeParam["seam_find_type"].insert(std::pair<string, int>(_("voronoi").ToStdString(), 1));
listeParam["seam_find_type"].insert(std::pair<string, int>(_("gc_color").ToStdString(), 2));
listeParam["seam_find_type"].insert(std::pair<string, int>(_("gc_colorgrad").ToStdString(), 3));
listeParam["seam_find_type"].insert(std::pair<string, int>(_("dp_color").ToStdString(), 4));
listeParam["seam_find_type"].insert(std::pair<string, int>(_("dp_colorgrad").ToStdString(), 5));

listeParam["blend_type"].insert(std::pair<string, int>(_("NO").ToStdString(), cv::detail::Blender::NO));
listeParam["blend_type"].insert(std::pair<string, int>(_("FEATHER").ToStdString(), cv::detail::Blender::FEATHER));
listeParam["blend_type"].insert(std::pair<string, int>(_("MULTI_BAND").ToStdString(), cv::detail::Blender::MULTI_BAND));

listeParam["expos_comp_type"].insert(std::pair<string, int>(_("NO").ToStdString(), cv::detail::ExposureCompensator::NO));
listeParam["expos_comp_type"].insert(std::pair<string, int>(_("GAIN").ToStdString(), cv::detail::ExposureCompensator::GAIN));
listeParam["expos_comp_type"].insert(std::pair<string, int>(_("GAIN_BLOCKS").ToStdString(), cv::detail::ExposureCompensator::GAIN_BLOCKS));
 
listeParam["matcher"].insert(std::pair<string, int>(_("Brute force matcher").ToStdString(), 0));
listeParam["matcher"].insert(std::pair<string, int>(_("FlannBasedMatcher").ToStdString(), 1));



listeParam["wave_correct"].insert(std::pair<string, int>(_("WAVE_CORRECT_HORIZ").ToStdString(), cv::detail::WAVE_CORRECT_HORIZ));
listeParam["wave_correct"].insert(std::pair<string, int>(_("WAVE_CORRECT_VERT").ToStdString(), cv::detail::WAVE_CORRECT_VERT));

listeParam["image_mask"].insert(std::pair<string, int>(_("enable mask").ToStdString(), 1));
listeParam["image_mask"].insert(std::pair<string, int>(_("disable mask").ToStdString(),0));

listeParam["matrix_type"].insert(std::pair<string, int>(_("unsigned char").ToStdString(), CV_8U));
listeParam["matrix_type"].insert(std::pair<string, int>(_("char").ToStdString(), CV_8S));
listeParam["matrix_type"].insert(std::pair<string, int>(_("unsigned short").ToStdString(), CV_16U));
listeParam["matrix_type"].insert(std::pair<string, int>(_("short").ToStdString(), CV_16S));
listeParam["matrix_type"].insert(std::pair<string, int>(_("int").ToStdString(), CV_32S));
listeParam["matrix_type"].insert(std::pair<string, int>(_("float").ToStdString(), CV_32F));
listeParam["matrix_type"].insert(std::pair<string, int>(_("double").ToStdString(), CV_64F));

listeParam["maskSize"].insert(std::pair<string, int>(_("DIST_MASK_PRECISE").ToStdString(), 0));
listeParam["maskSize"].insert(std::pair<string, int>(_("DIST_MASK_3").ToStdString(), 3));
listeParam["maskSize"].insert(std::pair<string, int>(_("DIST_MASK_5").ToStdString(), 5));

listeParam["opencl_enable"].insert(std::pair<string, int>(_("false").ToStdString(), 0));
listeParam["opencl_enable"].insert(std::pair<string, int>(_("true").ToStdString(), 1));

listeParam["method"].insert(std::pair<string, int>(_("regular method using all the points").ToStdString(), 0));
listeParam["method"].insert(std::pair<string, int>(_("RANSAC-based robust method").ToStdString(), CV_RANSAC));
listeParam["method"].insert(std::pair<string, int>(_("Least-Median robust method").ToStdString(), CV_LMEDS));
listeParam["method"].insert(std::pair<string, int>(_("PROSAC-based robust method").ToStdString(), cv::RHO));
#endif
}

bool ParametreOperation::InitOperation(string s)
{
    bool initEnCours = false;
if (listeParam.size()==0)
	InitParamType();
ImageInfoCV xx;
operateur=NULL;
nomOperation="";
nomSequence="";
nbImageRes=0;
nbOperande=0;
indEtape=-1;
idOperation=-1;
indRes=-1;
refPDF="";
lienHtml="";
opVideo=false;
opErreur=0;
if (xx.listeOperation.size() != 0 && xx.listeOperation.find(s) != xx.listeOperation.end())
{
    *this = xx.listeOperation[s];
	InitPtrFonction();
    return true;
}
//intParam["Save result"] = DomaineParametreOp<int>(0, 0, 0, 1);
//intParam["Send packet"] = DomaineParametreOp<int>(0, 0, 0, 1);
intParam["opencl_enable"]=  DomaineParametreOp<int>(1, 0, 1, 1);
if (s == "fond_gaussianmixture")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	intParam["history"] = DomaineParametreOp<int>(200, 2, 10000, 1);
	intParam["mixtures"] = DomaineParametreOp<int>(5, 1, 255, 1);
	doubleParam["BackgroundRatio"] = DomaineParametreOp<double>(0.7, 0.01, 100, 0.01);
	doubleParam["NoiseSigma"] = DomaineParametreOp<double>(30 * .5, 0, 100, 0.01);
	doubleParam["learningRate"] = DomaineParametreOp<double>(0, 0, 100, 0.01);
	intParam["ResultImage"] = DomaineParametreOp<int>(0, 0, 2, 1);
    xx.listeOperation.insert(make_pair(s, *this));
	}

if (s == "fond_gaussianmixture2")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	intParam["History"] = DomaineParametreOp<int>(500, 2, 10000, 1);
	intParam["mixtures"] = DomaineParametreOp<int>(5, 1, 255, 1);
	doubleParam["VarThreshold"] = DomaineParametreOp<double>(16, 0.01, 100, 0.01);
	doubleParam["BackgroundRatio"] = DomaineParametreOp<double>(0.9, 0.01, 100, 0.01);
	doubleParam["VarThresholdGen"] = DomaineParametreOp<double>(9, 0, 100, 1);
	doubleParam["VarInit"] = DomaineParametreOp<double>(15, 0, 100, 1);
	doubleParam["VarMax"] = DomaineParametreOp<double>(5*15, 0, 100, 1);
	doubleParam["VarMin"] = DomaineParametreOp<double>(4, 0, 100, 1);
	doubleParam["ComplexityReductionThreshold"] = DomaineParametreOp<double>(0.05, 0, 100, 0.01);
	doubleParam["ShadowThreshold"] = DomaineParametreOp<double>(0.5, 0, 1, .1);
	intParam["DetectShadows"] = DomaineParametreOp<int>(0, 0, 1, 1);
	intParam["ShadowValue"] = DomaineParametreOp<int>(127, 0, 255, 1);
	doubleParam["learningRate"] = DomaineParametreOp<double>(-1, -1, 100, 0.01);
	intParam["ResultImage"] = DomaineParametreOp<int>(0, 0, 2, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "fond_knn")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	intParam["History"] = DomaineParametreOp<int>(500, 1, 10000, 1);
	intParam["Nsamples"] = DomaineParametreOp<int>(7, 1, 1000, 1);
	intParam["KNNSamples"] = DomaineParametreOp<int>(7, 1, 1000, 1);
	intParam["ShadowValue"] = DomaineParametreOp<int>(127, 1, 255, 1);
	doubleParam["Dist2Threshold"] = DomaineParametreOp<double>(400, 0, 10000, 1);
	doubleParam["ShadowThreshold"] = DomaineParametreOp<double>(0.5, 0, 100, 0.01);
	intParam["DetectShadows"] = DomaineParametreOp<int>(0, 0, 1, 1);
	doubleParam["learningRate"] = DomaineParametreOp<double>(0, 0, 100, 0.01);
	intParam["ResultImage"] = DomaineParametreOp<int>(0, 0, 2, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "fond_gmg")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	intParam["updateBackgroundModel"] = DomaineParametreOp<int>(1, 0, 1, 1);
	intParam["NumFrames"] = DomaineParametreOp<int>(20, 2, 10000, 1);
	intParam["Maxfeatures"] = DomaineParametreOp<int>(64, 2, 10000, 1);
	intParam["QuantizationLevels"] = DomaineParametreOp<int>(16, 1, 255, 1);
	intParam["SmoothingRadius"] = DomaineParametreOp<int>(7, 1, 255, 1);
	doubleParam["BackgroundPrior"] = DomaineParametreOp<double>(0.8, 0.01, 100, 0.01);
	doubleParam["DecisionThreshold"] = DomaineParametreOp<double>(0.7, 0, 100, 0.01);
	doubleParam["MinVal"] = DomaineParametreOp<double>(0, 0, 100, 0.01);
	doubleParam["MaxVal"] = DomaineParametreOp<double>(0, 0, 100, 0.01);
	doubleParam["learningRate"] = DomaineParametreOp<double>(0.025, -1, 100, 0.01);
	intParam["ResultImage"] = DomaineParametreOp<int>(0, 0, 2, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}


if (s == "clahe")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	doubleParam["clipLimit"] = DomaineParametreOp<double>(0, 0, 10000, 1);
	doubleParam["tilesGridSize"] = DomaineParametreOp<double>(0, 0, 10000, 1);
    intParam["ColorSpaceCode"] = DomaineParametreOp<int>(cv::COLOR_BGR2Lab, cv::COLOR_BGR2GRAY, cv::COLOR_RGB2GRAY, 1);

    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "logPolar")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	pointParam["center"] = DomaineParametreOp<cv::Point>(cv::Point(0, 0), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	doubleParam["M"] = DomaineParametreOp<double>(1, 0, 10000, 1);
	intParam["InterpolationFlags"] = DomaineParametreOp<int>(cv::INTER_NEAREST, cv::INTER_NEAREST, CV_INTER_LANCZOS4, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "linearPolar")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	pointParam["center"] = DomaineParametreOp<cv::Point>(cv::Point(0, 0), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	doubleParam["M"] = DomaineParametreOp<double>(1, 0, 10000, 1);
	intParam["InterpolationFlags"] = DomaineParametreOp<int>(cv::INTER_NEAREST, cv::INTER_NEAREST, CV_INTER_LANCZOS4, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "undistort")
	{
	doubleParam["fx"] = DomaineParametreOp<double>(1000, 00, 10000, 1);
	doubleParam["fy"] = DomaineParametreOp<double>(1000, 00, 10000, 1);
	doubleParam["cx"] = DomaineParametreOp<double>(1, -1000, 1000, 1);
	doubleParam["cy"] = DomaineParametreOp<double>(1, -1000, 1000, 1);
	doubleParam["k1"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["k2"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["p1"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["p2"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["k3"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["k4"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["k5"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	doubleParam["k6"] = DomaineParametreOp<double>(0, -1800, 1800, 1);
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	operateur = &ImageInfoCV::CorrigeAberation;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "wrapAffine") // inclus la différence de deux images successives
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	pointParam["src1"] = DomaineParametreOp<cv::Point>(cv::Point(10, 10), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["src2"] = DomaineParametreOp<cv::Point>(cv::Point(100, 10), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["src3"] = DomaineParametreOp<cv::Point>(cv::Point(10, 100), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["dst1"] = DomaineParametreOp<cv::Point>(cv::Point(10,10), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["dst2"] = DomaineParametreOp<cv::Point>(cv::Point(100, 10), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["dst3"] = DomaineParametreOp<cv::Point>(cv::Point(10, 100), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	pointParam["centre"] = DomaineParametreOp<cv::Point>(cv::Point(50, 50), cv::Point(-1000, -1000), cv::Point(1000, 1000), cv::Point(1, 1),true);
	doubleParam["angle"] = DomaineParametreOp<double>(0, -180, 180, 1);
	doubleParam["scale"] = DomaineParametreOp<double>(1, 0.0000, 180, 0.1);
	sizeParam["dsize"] = DomaineParametreOp<cv::Size>(cv::Size(1000, 1000), cv::Size(1, 1), cv::Size(10000, 10000), cv::Size(1, 1));
	intParam["InterpolationFlags"] = DomaineParametreOp<int>(CV_INTER_LINEAR, CV_INTER_LINEAR, CV_INTER_LANCZOS4, 1);
	intParam["borderMode"] = DomaineParametreOp<int>(IPL_BORDER_CONSTANT, IPL_BORDER_CONSTANT, IPL_BORDER_WRAP, 1);
	doubleParam["borderValue"] = DomaineParametreOp<double>(0, -1000, 1000, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "warpperspective") // inclus la différence de deux images successives
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	sizeParam["dsize"] = DomaineParametreOp<cv::Size>(cv::Size(1000, 1000), cv::Size(1, 1), cv::Size(10000, 10000), cv::Size(1, 1));
	intParam["InterpolationFlags"] = DomaineParametreOp<int>(CV_INTER_LINEAR, CV_INTER_LINEAR, CV_INTER_LANCZOS4, 1);
	intParam["borderMode"] = DomaineParametreOp<int>(IPL_BORDER_CONSTANT, IPL_BORDER_CONSTANT, IPL_BORDER_WRAP, 1);
	doubleParam["borderValue"] = DomaineParametreOp<double>(0, -1000, 1000, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
 if (s == "recons")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
    intParam["nbPts"] = DomaineParametreOp<int>(5, 2, 10000, 1);;
    intParam["tailleOperateur"] = DomaineParametreOp<int>(10, 2, 10000, 1);;
	xx.listeOperation.insert(make_pair(s, *this));
	}
if (s == "resize") // inclus la différence de deux images successives
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	doubleParam["fx"] = DomaineParametreOp<double>(0, 0, 10000, 1);
	doubleParam["fy"] = DomaineParametreOp<double>(0, 0.0000, 10000,1);
	sizeParam["dsize"] = DomaineParametreOp<cv::Size>(cv::Size(1000, 1000), cv::Size(1, 1), cv::Size(10000, 10000), cv::Size(1, 1));
	intParam["InterpolationFlags"] = DomaineParametreOp<int>(CV_INTER_LINEAR, CV_INTER_NN, CV_INTER_LANCZOS4, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "updatemotionhistory") // inclus la différence de deux images successives
{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo=true;
	doubleParam["timestamp"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	doubleParam["duration"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	doubleParam["thresh"] = DomaineParametreOp<double>(50., 0.0, 255.0, 1.0);
	doubleParam["maxval"] = DomaineParametreOp<double>(255., 0.0, 255.0, 1.0);
	intParam["threshold_type"] = DomaineParametreOp<int>(cv::THRESH_BINARY, cv::THRESH_BINARY, cv::THRESH_TOZERO_INV, 1);

    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "calcmotiongradient") // inclus la différence de deux images successives
{
	nomOperation = s;
	opAttribut=true;
	nbImageRes = 1;
	nbOperande = 1;
	doubleParam["delta1"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	doubleParam["delta2"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	intParam["aperture_size"] = DomaineParametreOp<int>((int)3, (int)3, (int)7, (int)2);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "segmentmotion") // inclus la différence de deux images successives
	{
	opAttribut = true;
	nomOperation = s;
	nbImageRes = 2;
	nbOperande = 1;
	doubleParam["timestamp"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	doubleParam["segthresh"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
	intParam["calcGlobalOrientation"] = DomaineParametreOp<int>(0, 0, 1, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "cornerharris")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	intParam["blockSize"] = DomaineParametreOp<int>(2, 2, 9, 1);
	intParam["ksize"] = DomaineParametreOp<int>(1, 1, 7, 2);
	intParam["borderType"] = DomaineParametreOp<int>(cv::BORDER_CONSTANT, cv::BORDER_CONSTANT, cv::BORDER_WRAP, 1);
	doubleParam["k"] = DomaineParametreOp<double>(0.04, 0.01, 10, 0.01);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "matchdescriptormatcher")
	{
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["normType"] = DomaineParametreOp<int>(-1, 0, 1, 1);

    intParam["Matcher"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["crossCheck"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["keepBest"] = DomaineParametreOp<int>(-1, -1, 10000, 1);
//    intParam["Matcher"] = DomaineParametreOp<int>(0, 0, 1, 1);
    opAttribut = true;
	opVideo = true;
	nomOperation = s;
	nbImageRes = 0;
	nbOperande = 2;
	lienHtml = "http://docs.opencv.org/modules/features2d/doc/feature_detection_and_description.html#match";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=436&zoom=70,250,100";
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "findhomography")
	{
    intParam["method"] = DomaineParametreOp<int>(0, 0, 3, 1);
    doubleParam["ransacReprojThreshold"] = DomaineParametreOp<double>(3, 0.1, 10, .1);

    intParam["maxIters"] = DomaineParametreOp<int>(2000, 1, 20000, 100);
    doubleParam["confidence"] = DomaineParametreOp<double>(0.995, 0, 1, 0.01);
//    intParam["Matcher"] = DomaineParametreOp<int>(0, 0, 1, 1);
    opAttribut = true;
	opVideo = true;
	nomOperation = s;
	nbImageRes = 0;
	nbOperande = 2;
	lienHtml = "http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga4abc2ece9fab9398f2e560d53c8c9780";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=436&zoom=70,250,100";
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "mserfeatures2d")
    {
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    intParam["delta"] = DomaineParametreOp<int>(5, 1, 255, 1);
    intParam["minArea"] = DomaineParametreOp<int>(60, 1, 1000000, 1);
    intParam["maxArea"] = DomaineParametreOp<int>(14400, 1, 1000000, 1);
    doubleParam["maxVariation"] = DomaineParametreOp<double>(8, 1, 128, 1);
    doubleParam["minDiversity"] = DomaineParametreOp<double>(0.2, 0, 200, .1);
    intParam["maxEvolution"] = DomaineParametreOp<int>(200, 1, 10000, 1);;
    doubleParam["areaThreshold"] = DomaineParametreOp<double>(1.01, 0, 100, 0.01);
    doubleParam["minMargin"] = DomaineParametreOp<double>(0.003, 0, 100, .1);
    intParam["pass2Only"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["edgeBlurSize"] = DomaineParametreOp<int>(5, 1, 13, 2);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "blobfeatures2d")
    {
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    doubleParam["thresholdStep"] = DomaineParametreOp<double>(10, 1, 255, 1);
    doubleParam["minThreshold"] = DomaineParametreOp<double>(50, 1, 255, 1);;
    doubleParam["maxThreshold"] = DomaineParametreOp<double>(220, 0, 255, 1);
    intParam["minRepeatability"] = DomaineParametreOp<int>(2, 1, 10000, 1);
    doubleParam["minDistBetweenBlobs"] = DomaineParametreOp<double>(10, 1, 10000, 1);
    intParam["filterByColor"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["blobColor"] = DomaineParametreOp<int>(0, 0, 255, 1);
    intParam["filterByArea"] = DomaineParametreOp<int>(0, 0, 1, 1);
    doubleParam["minArea"] = DomaineParametreOp<double>(25, 0, 1000000, 1);
    doubleParam["maxArea"] = DomaineParametreOp<double>(5000,1,1000000,1);
    intParam["filterByCircularity"] = DomaineParametreOp<int>(0, 0, 1, 1);
    doubleParam["minCircularity"] = DomaineParametreOp<double>(0.8,0,10000,0.1);
    doubleParam["maxCircularity"] = DomaineParametreOp<double>(10000, 0, 10000, 0.1);
    intParam["filterByInertia"] = DomaineParametreOp<int>(0, 0, 1, 1);
    doubleParam["minInertiaRatio"] = DomaineParametreOp<double>(0.1,0,10000,0.1);
    doubleParam["maxInertiaRatio"] = DomaineParametreOp<double>(10000, 0, 10000, 0.1);
    intParam["filterByConvexity"] = DomaineParametreOp<int>(0, 0, 1, 1);
    doubleParam["minConvexity"] = DomaineParametreOp<double>(0.95, 0, 10000, 0.1);
    doubleParam["maxConvexity"] = DomaineParametreOp<double>(10000,0,10000,0.1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "orbfeatures2d")
    {
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["EdgeThreshold"] = DomaineParametreOp<int>(31, 1, 255, 1);
    doubleParam["FastThreshold"] = DomaineParametreOp<double>(20, 1, 100, 1);;
    intParam["FirstLevel"] = DomaineParametreOp<int>(0, 0, 128, 1);
    intParam["MaxFeatures"] = DomaineParametreOp<int>(500, 1, 10000, 1);
    intParam["NLevels"] = DomaineParametreOp<int>(8, 1, 128, 1);
    intParam["PatchSize"] = DomaineParametreOp<int>(31, 1, 200, 2);
    doubleParam["ScaleFactor"] = DomaineParametreOp<double>(1.2, 1, 10, 0.1);
    intParam["ScoreType"] = DomaineParametreOp<int>(cv::ORB::HARRIS_SCORE, cv::ORB::HARRIS_SCORE, cv::ORB::FAST_SCORE, 1);
    intParam["WTA_K"] = DomaineParametreOp<int>(2, 2, 4, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "akazefeatures2d")
    {
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["DescriptorChannels"] = DomaineParametreOp<int>(3, 1, 255, 1);
    intParam["DescriptorSize"] = DomaineParametreOp<int>(0, 0, 100, 1);;
    intParam["DescriptorType"] = DomaineParametreOp<int>(cv::AKAZE::DESCRIPTOR_MLDB, cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT, cv::AKAZE::DESCRIPTOR_MLDB, 1);
    intParam["Diffusivity"] = DomaineParametreOp<int>(cv::KAZE::DIFF_PM_G2, cv::KAZE::DIFF_PM_G1, cv::KAZE::DIFF_CHARBONNIER, 1);
    intParam["NOctaveLayers"] = DomaineParametreOp<int>(4, 1, 128, 1);
    intParam["NOctaves"] = DomaineParametreOp<int>(4, 1, 200, 2);
    doubleParam["Threshold"] = DomaineParametreOp<double>(0.001, 0.0001, 10, 0.001);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "kazefeatures2d")
    {
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["extended"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["upright "] = DomaineParametreOp<int>(0, 0, 1, 1);;
    intParam["Diffusivity"] = DomaineParametreOp<int>(cv::KAZE::DIFF_PM_G2, cv::KAZE::DIFF_PM_G1, cv::KAZE::DIFF_CHARBONNIER, 1);
    intParam["NOctaveLayers"] = DomaineParametreOp<int>(4, 1, 128, 1);
    intParam["NOctaves"] = DomaineParametreOp<int>(4, 1, 200, 2);
    doubleParam["Threshold"] = DomaineParametreOp<double>(0.001, 0.0001, 10, 0.001);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "briskfeatures2d")
    {
    nomOperation = s;
    nbImageRes = 0;
    nbOperande = 1;
    opAttribut = true;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["thresh"] = DomaineParametreOp<int>(30, 1, 255, 1);
    intParam["octaves"] = DomaineParametreOp<int>(3, 1, 255, 1);
    doubleParam["patternScale"] = DomaineParametreOp<double>(1.0, 0.1, 100, 0.1);;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "goodfeaturestotrack")
	{
	nbImageRes=0;
	nomOperation=s;
	nbOperande= 1;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["maxCorners"] = DomaineParametreOp<int>(500, 1, 1000, 1);
	doubleParam["qualityLevel"]=DomaineParametreOp<double>(0.1,0.01,1,0.01);
	doubleParam["minDistance"]=DomaineParametreOp<double>(10,1,1000,01);
	intParam["blockSize"]=DomaineParametreOp<int>(2,2,9,1);
	intParam["useHarrisDetector"]=DomaineParametreOp<int>(1,0,1,1);
	doubleParam["k"]=DomaineParametreOp<double>(0.04,0.01,1,0.01);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "houghcircles")
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
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "houghlines")
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
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "houghlinesp")
	{
	nbImageRes=0;
	nomOperation=s;
	nbOperande= 1;
	doubleParam["rho"]=DomaineParametreOp<double>(1,1,200,01);
	doubleParam["theta"]=DomaineParametreOp<double>(0.05,0,6.28,0.01);
	intParam["threshold"]=DomaineParametreOp<int>(50,3,1000,1);
	doubleParam["minLineLength"]=DomaineParametreOp<double>(10,1,1000,0.1);
	doubleParam["maxLineGap"]=DomaineParametreOp<double>(0.0,1,1000,1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "watershed")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=1;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "split")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=1;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "merge")
	{
	nomOperation = s;
	nbOperande= 3;
	nbImageRes=1;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "bitwise-and")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "bitwise-or")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "bitwise-xor")
{
	nomOperation = s;
	nbOperande = 2;
	nbImageRes = 1;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "bitwise-not")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 1;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "convertto")
	{
    intParam["matrix_type"] = DomaineParametreOp<int>(CV_32F, -1, CV_32F, 1);
	doubleParam["alpha"]=DomaineParametreOp<double>(1,-1000,1000,0.1);
	doubleParam["beta"]=DomaineParametreOp<double>(0,-10000,10000,1);
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 1;
    xx.listeOperation.insert(make_pair(s, *this));
	}

if (s == "add")
{
	nomOperation = s;
	nbOperande = 2;
	intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    nbImageRes = 1;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "addweighted")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	doubleParam["alpha"]=DomaineParametreOp<double>(1,0,10,0.1);
	doubleParam["beta"]=DomaineParametreOp<double>(1,0,10,0.1);
	doubleParam["gamma"]=DomaineParametreOp<double>(1,-65536,65536,1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "subtract")
	{
	nbImageRes=1;
	nomOperation=s;
    intParam["image_mask"] = DomaineParametreOp<int>(0, 0, 1, 1);
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	nbOperande= 2;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "multiply")
	{
	nbImageRes=1;
	nomOperation=s;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.1,10,0.1);
	nbOperande= 2;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "divide")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "filter2d")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["indOpConvolution"]=DomaineParametreOp<int>(xx.IndOpConvolution(),0,NB_OP_CONVOLUTION,1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "dilate")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "erode")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "openning")
	{
	nbImageRes=1;
	nomOperation=s;
	nbOperande= 2;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "closing")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "tophat")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "blackhat")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "morph_gradient")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	intParam["indOpMorphologie"]=DomaineParametreOp<int>(xx.IndOpMorphologie(),0,NB_OP_MORPHOLOGIE,1);
	intParam["nbIter"]=DomaineParametreOp<int>(1,1,10,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "deriche_mod")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	doubleParam["alphaDerive"]=DomaineParametreOp<double>(0.75,0.01,1000,0.01);
	doubleParam["alphaMoyenne"]=DomaineParametreOp<double>(0.25,0.0,1000,0.01);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "deriche_x")
	{
	nbImageRes=1;
    nbOperande = 1;
	doubleParam["alphaDerive"]=DomaineParametreOp<double>(0.75,0.01,10,0.1);
	doubleParam["alphaMoyenne"]=DomaineParametreOp<double>(0.25,0.0,1000,0.1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "deriche_y")
	{
	nbImageRes=1;
    nbOperande = 1;
	doubleParam["alphaDerive"]=DomaineParametreOp<double>(0.75,0.01,10,0.1);
	doubleParam["alphaMoyenne"]=DomaineParametreOp<double>(0.25,0.0,1000,0.1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "scharr_mod")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
	intParam["ddepth"]=DomaineParametreOp<int>(-1,-1,CV_32F,1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "scharr_x")
	{
	nbImageRes=1;
    nbOperande = 1;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "scharr_y")
	{
	nbImageRes=1;
    nbOperande = 1;
    intParam["ddepth"] = DomaineParametreOp<int>(-1, -1, CV_32F, 1);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.01,10,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,1000,1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "laplacian")
	{
	nbImageRes=1;
    nbOperande = 1;
    intParam["ddepth"] = DomaineParametreOp<int>(50., 0.0, 255.0, 1.0);
	doubleParam["scale"]=DomaineParametreOp<double>(1,0.0,255.0,0.1);
	doubleParam["delta"]=DomaineParametreOp<double>(0,0.0,255.0,1.0);
	intParam["ksize"]=DomaineParametreOp<int>(3,1,7,2);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "canny")
	{
	nbImageRes=1;
    nbOperande = 1;
    intParam["Otsu threshold"]=DomaineParametreOp<int>(0,0,1 ,1);
    doubleParam["threshold1"] = DomaineParametreOp<double>(50., 0.0, 255.0, 1.0);
	doubleParam["threshold2"]=DomaineParametreOp<double>(100,0.0,255.0,1.0);
	intParam["aperture_size"]=DomaineParametreOp<int>((int)3,(int)1,(int)255,(int)2);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "contour")
	{
	nbImageRes=0;
    nbOperande = 1;
    intParam["mode"] = DomaineParametreOp<int>(cv::RETR_EXTERNAL, cv::RETR_EXTERNAL, cv::RETR_TREE, 1);
	intParam["method"]=DomaineParametreOp<int>(cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_TC89_L1 ,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "convexhull")
	{
	nbImageRes=0;
    nbOperande = 1;
    intParam["clockwise"] = DomaineParametreOp<int>(cv::RETR_EXTERNAL, cv::RETR_EXTERNAL, cv::RETR_TREE, 1);
	intParam["returnpoints"]=DomaineParametreOp<int>(cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_TC89_L1 ,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "convexitydefects")
	{
	nbImageRes=0;
    nbOperande = 1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "approxpolydp")
	{
	nbImageRes=0;
    nbOperande = 1;
    doubleParam["epsilon"] = DomaineParametreOp<double>(0, 0, 1000, 1);
	intParam["closed"]=DomaineParametreOp<int>(0,0,1 ,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}

if (s == "cvtcolor")
	{
	nbImageRes=1;
    nbOperande = 1;
    intParam["ColorSpaceCode"] = DomaineParametreOp<int>(cv::COLOR_BGR2GRAY, cv::COLOR_BGR2GRAY, cv::COLOR_RGB2GRAY, 1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "FFT")
	{
    nbOperande = 1;
    nbImageRes = 1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "IFFT")
	{
    nbOperande = 1;
    nbImageRes = 1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "threshold")
	{
    nbOperande = 1;
    nbImageRes = 1;
	doubleParam["thresh"]=DomaineParametreOp<double>(50.,0.0,255.0,1.0);
	doubleParam["maxval"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["threshold_type"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
    intParam["OTSU"] = DomaineParametreOp<int>(0, 0, 1, 1);

	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "adaptivethreshold")
	{
    nbOperande = 1;
    nbImageRes = 1;
	doubleParam["maxValue"]=DomaineParametreOp<double>(255.,0.0,255.0,1.0);
	intParam["thresholdType"]=DomaineParametreOp<int>(cv::THRESH_BINARY,cv::THRESH_BINARY,cv::THRESH_TOZERO_INV,1);
	intParam["adaptiveMethod"]=DomaineParametreOp<int>(cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::ADAPTIVE_THRESH_MEAN_C,1);
	intParam["blockSize"]=DomaineParametreOp<int>(21,3,1000,2);
	doubleParam["C"]=DomaineParametreOp<double>(-0,-255,255,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "medianblur")
	{
    nbOperande = 1;
    nbImageRes = 1;
	intParam["ksize"]=DomaineParametreOp<int>(3,1,255,2);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "blur")
	{
    nbOperande = 1;
    nbImageRes = 1;
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	pointParam["anchor"]=DomaineParametreOp<cv::Point>(cv::Point(-1,-1),cv::Point(0,0),cv::Point(255,255),cv::Point(1,1));
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "gaussianblur")
	{
    nbOperande = 1;
    nbImageRes = 1;
	sizeParam["ksize"]=DomaineParametreOp<cv::Size>(cv::Size(3,3),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
	doubleParam["sigmaX"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	doubleParam["sigmaY"]=DomaineParametreOp<double>(0.1,0,255.0,0.1);
	intParam["borderType"]=DomaineParametreOp<int>(cv::BORDER_CONSTANT,cv::BORDER_CONSTANT,cv::BORDER_WRAP,1);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "connectedcomponents")
	{
    nbOperande = 1;
    nbImageRes = 1;
	intParam["connectivity"]=DomaineParametreOp<int>(4,4,8,4);
	intParam["ltype"]=DomaineParametreOp<int>(CV_32S,CV_32S,CV_32S,0);
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "distancetransform")
	{
    nbOperande = 1;
    nbImageRes = 1;
	nomOperation=s;
	intParam["distance_type"]=DomaineParametreOp<int>(cv::DIST_L1,cv::DIST_L1,cv::DIST_HUBER,1);
    intParam["maskSize"]=DomaineParametreOp<int>(cv::DIST_MASK_PRECISE ,3,11,2);
    intParam["labelType"]=DomaineParametreOp<int>(cv::DIST_LABEL_CCOMP,cv::DIST_LABEL_CCOMP,cv::DIST_LABEL_PIXEL ,1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "medianaxis")
	{
	nbImageRes=1;
    nbOperande = 1;
    nomOperation = s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "buildopticalflowpyramid")
	{
    nbOperande = 1;
    nbImageRes = 1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "calcopticalflowpyrlk")
{
    nbOperande = 2;
    nbImageRes = 0;
	nomOperation = s;
	opVideo = true;
	intParam["typeCriteria"] = DomaineParametreOp<int>(cv::TermCriteria::COUNT, cv::TermCriteria::COUNT, cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 1);
	intParam["maxCountCriteria"] = DomaineParametreOp<int>(20, 1, 255, 1);
	doubleParam["epsilonCriteria"] = DomaineParametreOp<double>(0.0001, 0.0000001, 255.0, 0.001);
	intParam["maxLevel"] = DomaineParametreOp<int>(3, 0, 8, 1);
	sizeParam["winSize"] = DomaineParametreOp<cv::Size>(cv::Size(21, 21), cv::Size(3, 3), cv::Size(255, 255), cv::Size(2, 2));
	intParam["flag"] = DomaineParametreOp<int>(0, cv::OPTFLOW_USE_INITIAL_FLOW, cv::OPTFLOW_LK_GET_MIN_EIGENVALS+cv::OPTFLOW_USE_INITIAL_FLOW, 4);
	doubleParam["minEigThreshold"] = DomaineParametreOp<double>(0.001, 0.0000001, 100.0, 0.001);
    xx.listeOperation.insert(make_pair(s, *this));

}
if (s == "phasecorrelate")
{
    nbOperande = 2;
    nbImageRes = 0;
	nomOperation = s;
	opVideo = true;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "calcopticalflowfarneback")
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
    nbOperande = 2;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "estimaterigidtransform")
	{
    nbOperande = 1;
    nbImageRes = 1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "updatemotionhistory")
	{
	nbImageRes=1;
	nomOperation=s;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "detailfeaturesfinder")
	{
	nomOperation=s;
	nbOperande= 15;
	nbImageRes=1;
	intParam["Stitch_descriptor"] = DomaineParametreOp<int>(1, 0, 1, 1);
    doubleParam["surf_hess_thresh"]=DomaineParametreOp<double>(300,1,1000,1);
	intParam["surf_num_octaves"]=DomaineParametreOp<int>(3,1,8,1);
	intParam["surf_num_layers"]=DomaineParametreOp<int>(4,1,8,1);
	intParam["surf_num_octaves_descr"]=DomaineParametreOp<int>(3,1,8,1);
	intParam["surf_num_layers_descr"]=DomaineParametreOp<int>(4,1,8,1);
    doubleParam["orb_scaleFactor"]=DomaineParametreOp<double>(1.3,1,1000,1);
	intParam["orb_nfeatures"]=DomaineParametreOp<int>(1500,1,500000,1);
	intParam["orb_nlevels"]=DomaineParametreOp<int>(5,1,8,10);
    sizeParam["orb_grid_size"]=DomaineParametreOp<cv::Size>(cv::Size(3,1),cv::Size(1,1),cv::Size(255,255),cv::Size(2,2));
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "detailmatchesinfo")
	{
    intParam["try_use_gpu"]=DomaineParametreOp<int>(0,0,1,1);
    doubleParam["match_conf"]=DomaineParametreOp<double>(0.3,0.01,1000,0.1);
    intParam["num_matches_thresh1"] = DomaineParametreOp<int>(6,1,1000,1);
    intParam["num_matches_thresh2"] = DomaineParametreOp<int>(6,1,1000,1);
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=0;
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "leavebiggestcomponent")
	{
	nomOperation=s;
	nbOperande= 1;
	nbImageRes=0;
    doubleParam["conf_thresh"]=DomaineParametreOp<double>(0.6,0.,1000,0.1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "homographybasedestimator")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 0;
	intParam["is_focals_estimated"] = DomaineParametreOp<int>(0, 0, 1, 1);
	intParam["do_wave_correct"] = DomaineParametreOp<int>(0, 0, 1, 1);
	intParam["ba_refine_mask_0"] = DomaineParametreOp<int>(1, 0, 1, 1);
	intParam["ba_refine_mask_2"] = DomaineParametreOp<int>(1, 0, 1, 1);
	intParam["ba_refine_mask_3"] = DomaineParametreOp<int>(1, 0, 1, 1);
	intParam["ba_refine_mask_4"] = DomaineParametreOp<int>(1, 0, 1, 1);
	doubleParam["conf_thresh"] = DomaineParametreOp<double>(0.6, 0., 1000, 0.1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "wraperwrap")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 0;
	intParam["warp_type"] = DomaineParametreOp<int>(0, 0, 1, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "correctionexpo")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 0;
	intParam["expos_comp_type"] = DomaineParametreOp<int>(cv::detail::ExposureCompensator::GAIN_BLOCKS, cv::detail::ExposureCompensator::NO, cv::detail::ExposureCompensator::GAIN_BLOCKS, 1);
	intParam["seam_find_type"] = DomaineParametreOp<int>(2, 0, 5, 1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (s == "panocomposition")
{
	nomOperation = s;
	nbOperande = 1;
	nbImageRes = 0;
	intParam["blend_type"] = DomaineParametreOp<int>(cv::detail::Blender::MULTI_BAND, cv::detail::Blender::NO, cv::detail::Blender::MULTI_BAND, 1);
	doubleParam["blend_strength"] = DomaineParametreOp<double>(5, 0., 1000, 0.1);
    xx.listeOperation.insert(make_pair(s, *this));
}
if (nomOperation == "")
	return false;
InitPtrFonction();
    xx.listeOperation[s].InitPtrFonction();
return true;
}

ParametreOperation::ParametreOperation(string s)
{
opErreur=0;
op.resize(3);
op[0] = NULL;
op[1] = NULL;
op[2] = NULL;
indOpFenetre.resize(3);
indOpFenetre[0] = -1;
indOpFenetre[1] = -1;
indOpFenetre[2] = -1;

InitOperation(s);

}


bool ParametreOperation::InitPtrFonction()
{
opAttribut=false;
string s(nomOperation);

if (s == "fond_gaussianmixture2")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#backgroundsubtractormog2";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=375&zoom=70,250,100";
	operateur = &ImageInfoCV::Fond_MOG2;
	}
if (s == "fond_gaussianmixture")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#backgroundsubtractormog";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=373&zoom=70,250,100";
	operateur = &ImageInfoCV::Fond_MOG;
	}
if (s == "fond_knn")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#backgroundsubtractorknn";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=373&zoom=70,250,100";
	operateur = &ImageInfoCV::Fond_KNN;
	}

if (s == "fond_gmg")
	{
	nomOperation = s;
	nbImageRes = 1;
	nbOperande = 1;
	opVideo = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#backgroundsubtractorgmg";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=373&zoom=70,250,100";
	operateur = &ImageInfoCV::Fond_GMG;
	}

if (s == "updatemotionhistory") // inclus la différence de deux images successives
	{
	nomOperation = s;
	nbOperande = 1;
	opVideo = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#updatemotionhistory";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=370&zoom=70,250,100";
	operateur = &ImageInfoCV::MAJHistoriqueMvt;
	}
if (s == "calcmotiongradient") // inclus la différence de deux images successives
	{
	nomOperation = s;
	nbImageRes = 0;
	opVideo = true;
	opAttribut = true;
	nbOperande = 1;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#calcmotiongradient";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=371&zoom=70,250,100";
	operateur = &ImageInfoCV::CalcOrientationMvt;
	}
if (s == "segmentmotion") // inclus la différence de deux images successives
{
	nomOperation = s;
	nbImageRes = 0;
	opAttribut = true;
	opVideo = true;
	nbOperande = 1;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#segmentmotion";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=371&zoom=70,250,100";
	operateur = &ImageInfoCV::SegmenteMvt;

}
if (s == "cornerharris")
	{
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornerharris";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=327&zoom=70,250,100";
	operateur = &ImageInfoCV::DetectCoinHarris;
	}

if (s == "mserfeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/trunk/d3/d28/classcv_1_1MSER.html";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=422&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectMser;
    }
if (s == "blobfeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/trunk/d0/d7a/classcv_1_1SimpleBlobDetector.html";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=422&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectBlob;
    }
if (s == "orbfeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/modules/features2d/doc/feature_detection_and_description.html#orb";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=422&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectOrb;
    }
if (s == "akazefeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/3.0-beta/modules/features2d/doc/feature_detection_and_description.html#akaze";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=4418&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectAkaze;
    }
if (s == "kazefeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/3.0-beta/modules/features2d/doc/feature_detection_and_description.html#kaze";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=4418&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectKaze;
    }
if (s == "briskfeatures2d")
    {
    opAttribut = true;
    nomOperation = s;
    nbOperande = 1;
    lienHtml = "http://docs.opencv.org/trunk/d8/d30/classcv_1_1AKAZE.html";
    refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=422&zoom=70,250,100";
    operateur = &ImageInfoCV::DetectBrisk;
    }
 if (s == "findhomography")
	{
    opAttribut = true;
	opVideo = true;
	nomOperation = s;
	nbImageRes = 0;
	nbOperande = 2;
	lienHtml = "http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga4abc2ece9fab9398f2e560d53c8c9780";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=436&zoom=70,250,100";
	operateur = &ImageInfoCV::FindHomography;
    }
if (s == "matchdescriptormatcher")
	{
	opAttribut = true;
	opVideo = true;
	nomOperation = s;
	nbOperande = 2;
	lienHtml = "http://docs.opencv.org/modules/features2d/doc/feature_detection_and_description.html#match";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=436&zoom=70,250,100";
	operateur = &ImageInfoCV::AppariePoint;
	}
if (s == "goodfeaturestotrack")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=329&zoom=70,250,100";
	operateur = &ImageInfoCV::BonAttributs;
	}
if(	s=="houghcircles")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=330&zoom=70,250,100";
	operateur = &ImageInfoCV::HoughCercle;
	}
if(	s=="houghlines")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=332&zoom=70,250,100";
	operateur = &ImageInfoCV::HoughLigne;
	}
if(	s=="houghlinesp")
	{
	opAttribut=true;
	nomOperation=s;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=333&zoom=70,250,100";
	operateur = &ImageInfoCV::HoughLigneProba;
	}
if (s=="cvtcolor")
	{
	lienHtml="http://docs.opencv.org/master/d7/d1b/group__imgproc__misc.html#ga397ae87e1288a81d2363b61574eb8cab";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=283&zoom=70,250,100";
    nbOperande = 1;
    nomOperation = s;
	operateur = &ImageInfoCV::RGB_L;
	}
if (s=="clahe")
	{
	lienHtml="http://docs.opencv.org/master/d6/db6/classcv_1_1CLAHE.html";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=283&zoom=70,250,100";
    nbOperande = 1;
    nomOperation = s;
	operateur = &ImageInfoCV::Clahe;
	}
if (s=="watershed")
	{
	operateur = &ImageInfoCV::PartageEaux;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#watershed";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=294&zoom=70,250,100";
	return true;
	}
if (s=="split")
	{
	operateur = &ImageInfoCV::SeparationPlan;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#split";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
	}
if (s=="merge")
	{
	operateur = &ImageInfoCV::FusionPlan;
	nbOperande= 3;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#merge";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=149&zoom=70,250,100";
	return true;
	}
if (s == "bitwise-and")
{
	operateur = &ImageInfoCV::EtLogique;
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-and";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=124&zoom=70,250,100";
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-or")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-or";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=125&zoom=70,250,100";
	operateur = &ImageInfoCV::OuLogique;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-xor")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-xor";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=126&zoom=70,250,100";
	operateur = &ImageInfoCV::OuExcluLogique;
	nbOperande = 2;
	nbImageRes = 1;
}
if (s == "bitwise-not")
{
	lienHtml = "http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#bitwise-not";
	refPDF = "http://docs.opencv.org/opencv2refman.pdf#page=126&zoom=70,250,100";

	operateur = &ImageInfoCV::Negation;
	nbOperande = 1;
	nbImageRes = 1;
}
if (s == "convertto")
	{
	operateur = &ImageInfoCV::ConvertType;
	nbOperande= 1;
	lienHtml="http://docs.opencv.org/modules/core/doc/basic_structures.html#mat-convertto";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
	}
if (s == "add")
	{
	operateur = &ImageInfoCV::Add;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#add";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
	}
if (s == "addweighted")
	{
	operateur = &ImageInfoCV::AddPonderee;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#addweighted";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=121&zoom=70,250,100";
	return true;
}
if (s=="subtract")
	{
	operateur = &ImageInfoCV::Sub;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#subtract";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=172&zoom=70,250,100";
	return true;
	}
if (s=="multiply")
	{
	operateur = &ImageInfoCV::Mul;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#multiply";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=153&zoom=70,250,100";
	return true;
	}
if (s=="divide")
	{
	operateur = &ImageInfoCV::Div;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#divide";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=136&zoom=70,250,100";
	return true;
	}
if (s=="filter2d")
	{
	operateur = &ImageInfoCV::Convolution;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	return true;
	}
if (s=="dilate")
	{
	operateur = &ImageInfoCV::Dilatation;
	nbOperande= 2;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	return true;
	}
if (s=="erode")
	{
	operateur = &ImageInfoCV::Erosion;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=255&zoom=70,250,100";
	return true;
	}
if (s=="openning")
	{
	operateur = &ImageInfoCV::Ouverture;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=morphologyex#morphologyex";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="closing")
	{
	operateur = &ImageInfoCV::Fermeture;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="tophat")
	{
	operateur = &ImageInfoCV::ChapeauHaut;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="blackhat")
	{
	operateur = &ImageInfoCV::ChapeauBas;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="morph_gradient")
	{
	operateur = &ImageInfoCV::GradMorph;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=260&zoom=70,250,100";
	return true;
	}
if (s=="scharr_mod")
	{
	operateur = &ImageInfoCV::ScharrModule;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="scharr_x")
	{
	operateur = &ImageInfoCV::ScharrX;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="scharr_y")
	{
	operateur = &ImageInfoCV::ScharrY;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="deriche_mod")
	{
	operateur = &ImageInfoCV::ModuleGradientDeriche;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="deriche_x")
	{
	operateur = &ImageInfoCV::GradientDericheX;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="deriche_y")
	{
	operateur = &ImageInfoCV::GradientDericheY;
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	return true;
	}
if (s=="laplacian")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html#laplacian";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=261&zoom=70,250,100";
	operateur = &ImageInfoCV::Laplacien;
	return true;
	}
if (s=="canny")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=331&zoom=70,250,100";
	operateur = &ImageInfoCV::Canny;
	return true;
	}
if (s=="contour")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#findContour";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=308&zoom=70,250,100";
	operateur = &ImageInfoCV::Contour;
	return true;
	}
if (s == "convexhull")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#ga014b28e56cb8854c0de4a211cb2be656";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=316&zoom=70,250,100";
	operateur = &ImageInfoCV::ConvexHull;
	return true;
}
if (s == "convexitydefects")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#gada4437098113fd8683c932e0567f47ba";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=317&zoom=70,250,100";
	operateur = &ImageInfoCV::ConvexityDefects;
	return true;
}
if (s == "approxpolydp")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#ga0012a5fdaea70b8a9970165d98722b4c";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=313&zoom=70,250,100";
	operateur = &ImageInfoCV::ApproxPolyDP;
	return true;
}

if (s=="FFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	operateur = &ImageInfoCV::FFT;
	return true;
	}
if (s=="IFFT")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=133&zoom=70,250,100";
	operateur = &ImageInfoCV::IFFT;
	return true;
	}
if (s=="threshold")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#threshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=291&zoom=70,250,100";
	nomOperation=s;
	operateur = &ImageInfoCV::Seuillage;
	return true;
	}
if (s=="adaptivethreshold")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=threshold#adaptivethreshold";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=280&zoom=70,250,100";
	operateur = &ImageInfoCV::SeuillageAdaptatif;
	return true;
	}
if (s=="medianblur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=medianblur#medianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=266&zoom=70,250,100";
	operateur = &ImageInfoCV::LissageMedian;
	return true;
	}
if (s=="blur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=blur#blur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=254&zoom=70,250,100";
	operateur = &ImageInfoCV::LissageMoyenne;
	return true;
	}
if (s=="gaussianblur")
	{
	lienHtml="http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=gaussianblur#gaussianblur";
	refPDF="http://docs.opencv.org/opencv2refman.pdf#page=263&zoom=70,250,100";
	operateur = &ImageInfoCV::LissageGaussien;
	return true;
	}
if (s=="connectedcomponents")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#connectedcomponents";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=263&zoom=70,250,100";
	operateur = &ImageInfoCV::ComposanteConnexe;
	return true;
	}
if (s=="distancetransform")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	operateur = &ImageInfoCV::DistanceDiscrete;
	return true;
	}
if (s=="medianaxis")
	{
	lienHtml="http://docs.opencv.org/trunk/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=287&zoom=70,250,100";
	operateur = &ImageInfoCV::LigneMediane;
	return true;
	}
if (s == "wrapAffine")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpaffine";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=277&zoom=70,250,100";
	operateur = &ImageInfoCV::TransAffine;
	return true;
	}
if (s == "warpperspective")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpaffine";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=277&zoom=70,250,100";
	operateur = &ImageInfoCV::TransPerspective;
	return true;
	}
if (s == "resize")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=277&zoom=70,250,100";
	operateur = &ImageInfoCV::Dimension;
	return true;
	}
if (s == "recons")
	{
	lienHtml = "";
	refPDF = "";
	operateur = &ImageInfoCV::Recons2d;
	return true;
	}
if (s == "logPolar")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#logpolar";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=277&zoom=70,250,100";
	operateur = &ImageInfoCV::LogPolar;
	return true;
	}
if (s == "linearPolar")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#linearpolar";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=277&zoom=70,250,100";
	operateur = &ImageInfoCV::LinearPolar;
	return true;
	}
if (s == "undistort")
	{
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#undistort";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=278&zoom=70,250,100";
	operateur = &ImageInfoCV::CorrigeAberation;
	return true;
	}
if (s == "buildopticalflowpyramid")
	{
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#buildopticalflowpyramid";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=366&zoom=70,250,100";
	//operateur = &ImageInfoCV::LigneMediane;
	return true;
	}
if (s == "calcopticalflowpyrlk")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#calcopticalflowpyrlk";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=365&zoom=70,250,100";
	operateur = &ImageInfoCV::FlotOptiqueLucasKanadePyramide;
	opVideo = true;

	return true;
}
if (s == "phasecorrelate")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#phasecorrelate";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=365&zoom=70,250,100";
	operateur = &ImageInfoCV::PhaseCorrelate;
	opVideo = true;

	return true;
}

if (s=="calcopticalflowfarneback")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	operateur = &ImageInfoCV::FlotOptiqueFarnerback;
	opVideo=true;

	return true;
	}
if (s=="detailfeaturesfinder")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/3.0-beta/modules/stitching/doc/matching.html#detail-featuresfinder";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	operateur = &ImageInfoCV::DetailFeaturesFinder;

	return true;
	}
if (s=="detailmatchesinfo")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/3.0-beta/modules/stitching/doc/matching.html#detail-featuresmatcher-match";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	operateur = &ImageInfoCV::DetailMatchesInfo;

	return true;
	}
if (s=="leavebiggestcomponent")
	{
	opAttribut=true;
	lienHtml="http://docs.opencv.org/3.0-beta/modules/stitching/doc/matching.html#detail-bestof2nearestmatcher";
	refPDF="http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	operateur = &ImageInfoCV::LeaveBiggestComponent;

	return true;
	}
if (s == "homographybasedestimator")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=367&zoom=70,250,100";
	operateur = &ImageInfoCV::HomographyBasedEstimator;

	return true;
}
if (s == "wraperwrap")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=660&zoom=70,250,100";
	operateur = &ImageInfoCV::WraperWrap;

	return true;
}
if (s == "correctionexpo")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=660&zoom=70,250,100";
	operateur = &ImageInfoCV::CorrectionExpo;

	return true;
}
if (s == "panocomposition")
{
	opAttribut = true;
	lienHtml = "http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html?highlight=buildoptical#calcopticalflowfarneback";
	refPDF = "http://docs.opencv.org/opencv3refman.pdf#page=660&zoom=70,250,100";
	operateur = &ImageInfoCV::PanoComposition;

	return true;
}

return false;
}


std::vector<ImageInfoCV*> ParametreOperation::ExecuterOperation()
{
std::vector <ImageInfoCV*> r;
nbImageRes=1;
if (operateur)
	{
	try
		{
        // 
        cv::ocl::setUseOpenCL(intParam["opencl_enable"].valeur);

		r =(op[0]->*operateur) (op,this);

		}
	catch(cv::Exception& e)
		{
		string s(e.msg);
		}

	}


return r; 
}
