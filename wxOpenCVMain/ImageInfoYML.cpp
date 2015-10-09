#include "ImageInfo.h"

using namespace std;
using namespace cv;

static void write(cv::FileStorage& fs, const std::string&, const ImageInfoCV& x)
{
    x.write(fs);
}

static void write(FileStorage& fs, const String& objname, const std::vector<Moments>& m) {
    fs << objname << "[";
    for (size_t i=0; i<m.size(); i++) {
        cv::write(fs, m[i].m00);
        cv::write(fs, m[i].m10);
        cv::write(fs, m[i].m01);
        cv::write(fs, m[i].m20);
        cv::write(fs, m[i].m11);
        cv::write(fs, m[i].m02);
        cv::write(fs, m[i].m30);
        cv::write(fs, m[i].m21);
        cv::write(fs, m[i].m12);
        cv::write(fs, m[i].m03);
    }
    fs << "]";
}
void read(FileStorage& fs, const String& objname, std::vector<Moments>& m) {
    FileNode pnodes = fs[objname];
    for (FileNodeIterator it=pnodes.begin();; ) {
        Moments p;
        *it >>p.m00; it++;
        *it >>p.m10; it++;
        *it >>p.m01; it++;
        *it >>p.m20; it++;
        *it >>p.m11; it++;
        *it >>p.m02; it++;
        *it >>p.m30; it++;
        *it >>p.m21; it++;
        *it >>p.m12; it++;
        *it >>p.m03; it++;
        m.push_back(p);
        if (it==pnodes.end()) break;
    }
}
ostream& operator << (ostream &out, const std::vector<Moments>& m) {
    out << "[" ;
    for (size_t i=0; i<m.size(); i++) {
        const Moments &dm = m[i];
        out << dm.m00 << "," << dm.m10 << "," << dm.m01 << "," << dm.m20 <<",";
        out << dm.m11 <<"," << dm.m02 <<"," <<dm.m30 <<"," <<dm.m21 <<"," ;
        out << dm.m12 <<"," <<dm.m03 <<";"<<endl;
    }
    out << "]" << endl;
    return out;
}


void write(FileStorage& fs, const String& objname, const std::vector<DMatch>& matches) {
    fs << objname << "[";
    for (size_t i=0; i<matches.size(); i++) {
        const DMatch &dm = matches[i];
        cv::write(fs, dm.queryIdx);
        cv::write(fs, dm.trainIdx);
        cv::write(fs, dm.imgIdx);
        cv::write(fs, dm.distance);
    }
    fs << "]";
}
void read(FileStorage& fs, const String& objname, std::vector<DMatch>& matches) {
    FileNode pnodes = fs[objname];
    for (FileNodeIterator it=pnodes.begin();; ) {
        DMatch dm;
        *it >> dm.queryIdx; it++;
        *it >> dm.trainIdx; it++;
        *it >> dm.imgIdx;   it++;
        *it >> dm.distance; it++;
        matches.push_back(dm);
        if (it==pnodes.end()) break;
    }
}
ostream& operator << (ostream &out, const std::vector<DMatch>& matches) {
    out << "[" ;
    for (size_t i=0; i<matches.size(); i++) {
        const DMatch &dm = matches[i];
        out << dm.queryIdx << "," << dm.trainIdx << "," << dm.imgIdx << "," << dm.distance << ";" << endl;
    }
    out << "]" << endl;
    return out;
}



void ImageInfoCV::write(cv::FileStorage& fs) const                        //Write serialization for this class
{
    Mat mThis = getMat(cv::ACCESS_READ);
    fs<<"Image"<<mThis;
    if (statComposante.size()!=0)
        {
        for (int i=0;i<channels();i++)
	        {
	        string s;
	        s="StatComposante"+to_string(i);
	        fs<<s<<statComposante[i];
	        }
        }
	if (centreGComposante.size()!=0)
		{
		for (int i=0;i<channels();i++)
			{
			string s;
			s="CentreGComposante"+to_string(i);
			fs<<s<<centreGComposante[i];
			}
		}
	if (moment.size()!=0)
		{
		for (int i=0;i<channels();i++)
			{
				{
			    string s("moments");
			    s=s+to_string(i);
                const String ss(s);
			    ::write(fs,ss,(const vector<Moments>)moment[i]);
				}    
			}
		}
	if (contours.size()!=0)
		{
		for (int i=0;i<channels()&& i<3;i++)
			{
			int indCtr=0;
			for (int j=0;j<contours[i].size();j++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"Ctr"<<j;
				string s(nombre.str());

				fs<<s<<contours[i][j];
				indCtr++;
				}    
			}

		}
	if (contoursPoly.size()!=0)
		{
		for (int i=0;i<channels()&& i<3;i++)
			{
			int indCtr=0;
			for (int j=0;j<contoursPoly[i].size();j++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"CtrPoly"<<j;
				string s(nombre.str());

				fs<<s<<contoursPoly[i][j];
				indCtr++;
				}    
			}

		}
	if (cercle.size()!=0)
		{
		for (int i=0;i<channels()&& i<3;i++)
			{
			int indCtr=0;
			for (int j=0;j<cercle[i].size();j++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"cercle"<<j;
				string s(nombre.str());

				fs<<s<<cercle[i][j];
				indCtr++;
				}    
			}
		}
	if (ligneP.size()!=0)
		{
		for (int i=0;i<channels()&& i<3;i++)
			{
			int indCtr=0;
			for (int j=0;j<ligneP[i].size();j++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"ligneP"<<j;
				string s(nombre.str());

				fs<<s<<ligneP[i][j];
				indCtr++;
				}    
			}
		}
	if (ligne.size()!=0)
		{
		for (int i=0;i<channels()&& i<3;i++)
			{
			int indCtr=0;
			for (int j=0;j<ligne[i].size();j++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"ligne"<<j;
				string s(nombre.str());

				fs<<s<<ligne[i][j];
				indCtr++;
				}    
			}
		}
#ifdef WIN32
    auto  it = pointCle.begin();
    for (; it != pointCle.end(); it++)
    {
        switch (it->first){
        case IMAGEINFOCV_AGAST_DES:
		    fs<<"AGAST"<<it->second;
            break;
        case IMAGEINFOCV_AKAZE_DES:
		    fs<<"AKAZE"<<it->second;
            break;
        case IMAGEINFOCV_BLOB_DES:
		    fs<<"BLOB"<<it->second;
            break;
        case IMAGEINFOCV_BRISK_DES:
		    fs<<"BRISK"<<it->second;
            break;
        case IMAGEINFOCV_KAZE_DES:
		    fs<<"KAZE"<<it->second;
            break;
        case IMAGEINFOCV_ORB_DES:
		    fs<<"ORB"<<it->second;
            break;
        case IMAGEINFOCV_GFTT_GRAY_DES:
		    fs<<"GFTT0"<<it->second;
            break;
        case IMAGEINFOCV_GFTT_GREEN_DES:
		    fs<<"GFTT1"<<it->second;
            break;
        case IMAGEINFOCV_GFTT_RED_DES:
		    fs<<"GFTT2"<<it->second;
            break;
        }
    }
#endif
}
 
 
 ImageInfoCV::ImageInfoCV(char *nomDuFichier):cv::UMat()
{
eSauver=NULL;
InitImageInfo(NULL);
int nb = strlen(nomDuFichier);
string ext(nomDuFichier+nb-3);
std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
if (ext!="yml")
    cv::imread(nomDuFichier,cv::IMREAD_UNCHANGED).copyTo(*((UMat *)(this)));
else
{
    cv::FileStorage fs(nomDuFichier, cv::FileStorage::READ);
    Mat mThis;

    fs["Image"]>>mThis;
    mThis.copyTo(*this);

    cv::FileNode n=fs["StatComposante0"];
    if (!n.empty())
    {
        statComposante.resize(channels());
        centreGComposante.resize(channels());
        for (int i=0;i<channels();i++)
        {
            cv::FileNode n=fs["StatComposante"+to_string(i)];
            if (!n.empty())
                n >> statComposante[i];
        }
        for (int i=0;i<channels();i++)
        {
            cv::FileNode n=fs["CentreGComposante"+to_string(i)];
            if (!n.empty())
                n >> centreGComposante[i];
        }
    }
    n=fs["moments0"];
    if (!n.empty())
    {
        moment.resize(channels());
        for (int i=0;i<channels();i++)
        {
            const string ss="moments"+to_string(i);
            ::read(fs,ss,moment[i]);
        }
        CalcMoment();
    }    
    n=fs["Cmp0Ctr0"];
    if (!n.empty())
    {
        contours.resize(channels());
        int indCtr=0;
        for (int i=0;i<channels();i++)
        {
            n = fs["Cmp" + to_string(i) + "Ctr" + to_string(indCtr++)];
            
            while (!n.empty())
                {
                std::vector<Point> p;
                n>>p;
                contours[i].push_back(p);
                n = fs["Cmp" + to_string(i) + "Ctr" + to_string(indCtr++)];
                }
        }
    }    
    n=fs["Cmp0CtrPoly0"];
    if (!n.empty())
    {
        contoursPoly.resize(channels());
        int indCtr=0;
        for (int i=0;i<channels();i++)
        {
            n = fs["Cmp" + to_string(i) + "CtrPoly" + to_string(indCtr++)];
            
            while (!n.empty())
                {
                std::vector<Point> p;
                n>>p;
                contoursPoly[i].push_back(p);
                n = fs["Cmp" + to_string(i) + "CtrPoly" + to_string(indCtr++)];
                }
        }
    }    
    n=fs["ORB"];
    vector<KeyPoint> cle;
    if (!n.empty())
    {
        n>>cle;
        pointCle.insert(make_pair(IMAGEINFOCV_ORB_DES,cle));
        cle.clear();
    }    
    n=fs["BRISK"];
    if (!n.empty())
    {
        n>>cle;
        pointCle.insert(make_pair(IMAGEINFOCV_BRISK_DES,cle));
        cle.clear();
    }    
    n=fs["AKAZE"];
    if (!n.empty())
    {
        n>>cle;
        pointCle.insert(make_pair(IMAGEINFOCV_AKAZE_DES,cle));
        cle.clear();
    }    
    n=fs["KAZE"];
    if (!n.empty())
    {
        n>>cle;
        pointCle.insert(make_pair(IMAGEINFOCV_KAZE_DES,cle));
        cle.clear();
    }    
    n=fs["BLOB"];
    if (!n.empty())
    {
        n>>cle;
        pointCle.insert(make_pair(IMAGEINFOCV_BLOB_DES,cle));
        cle.clear();
    }    
    fs.release();
}


}


 
 
 void ImageInfoCV::read(const cv::FileNode& node)                          //Read serialization for this class
    {

    }
