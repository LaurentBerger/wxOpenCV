#include "ImageInfo.h"

using namespace std;
using namespace cv;

static void write(cv::FileStorage& fs, const std::string&, const ImageInfoCV& x)
{
    x.write(fs);
}


void ImageInfoCV::write(cv::FileStorage& fs) const                        //Write serialization for this class
{
fs<<"Image"<<*((cv::Mat*)this);
if (statComposante)
	{
	for (int i=0;i<channels();i++)
		{
		wxString s;
		s.Printf("StatComposante%d",i);
		wxCharBuffer ww=s.mb_str ();
		char *nomChamp=ww.data() ;
		fs<<nomChamp<<*((cv::Mat*)(statComposante[i]));
		}
	}
	if (centreGComposante)
		{
		for (int i=0;i<channels();i++)
			{
			wxString s;
			s.Printf("CentreGComposante%d",i);
			wxCharBuffer ww=s.mb_str ();
			char *nomChamp=ww.data() ;
			fs<<nomChamp<<*((cv::Mat*)(centreGComposante[i]));
			}
		}
/*	if (moment)
		{
		vector<cv::Moments> *m=moment;
		for (int i=0;i<channels();i++)
			{
			wxString s;
			s.Printf("Moment%d",i);
			wxCharBuffer ww=s.mb_str ();
			char *nomChamp=ww.data() ;
			fs<<nomChamp<<m[i];
			}
		}*/
	if (contours)
		{
		std::vector<std::vector<cv::Point> > *ptCtr=contours;
		for (int i=0;i<channels()&& i<3;i++)
			{
			vector<vector<cv::Point> >::iterator itCtr;
			int indCtr=0;
			for (itCtr=ptCtr[i].begin();itCtr!=ptCtr[i].end();itCtr++)
				{
				stringstream nombre;
				nombre<<"Cmp"<<i<<"Ctr"<<indCtr;
				string s(nombre.str());

				fs<<s<<*itCtr;
				indCtr++;
				}    
			}

		}
}
 
 
 
 void ImageInfoCV::read(const cv::FileNode& node)                          //Read serialization for this class
    {
/*        A = (int)node["A"];
        X = (double)node["X"];
        id = (string)node["id"];*/
    }
