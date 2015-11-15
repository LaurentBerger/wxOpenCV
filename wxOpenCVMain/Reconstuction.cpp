#include "ImageInfo.h"

using namespace cv;

std::vector<ImageInfoCV	*> ImageInfoCV::SincXY(std::vector< ImageInfoCV *> op, ParametreOperation *pOCV)//(long x,long y,long nbPts)
{
double x = pOCV->doubleParam["x"].valeur;
double y = pOCV->doubleParam["y"].valeur;
int nbPts = pOCV->intParam["nbPts"].valeur;
cv::Mat mThis = getMat(cv::ACCESS_RW);

int	i,j; 
double	lx=x/double(nbPts+1),ly=y/double(nbPts+1),pi=acos(-1.);
double	w,s=0;

if (type()==CV_32F)	
	{
	double	h;
	for (i=0,s=0;i<rows;i++)
    {
        double	*dDst = (double *)mThis.ptr(i);
		for (j=0;j<cols;j++,dDst++)
			*dDst = 0;
    }
	for (i=0,s=0;i<rows;i++)
    {
        double	*dDst = (double *)mThis.ptr(i);
		for (j=0;j<cols;j++,dDst++)
			{
			h =0.54+0.46*cos(2*pi*(i-(rows-1)/2.)/rows);
			w = pi*(ly-(i-(rows-1)/2.)); 
			if (w!=0)
				*dDst = float(sin(w)/w)*h;
			else
				*dDst = float(1)*h;
			h =0.54+0.46*cos(2*pi*(j-(cols-1)/2.)/cols);
			w = pi*(lx-(j-(cols-1)/2.));
			if (w!=0)
				*dDst = *dDst * float(sin(w)/w)*h;
			s += *dDst;
			}
    }
    }
if (type()==CV_64F)	
	{
	double	h;
	for (i=0,s=0;i<rows;i++)
    {
        double	*dDst = (double *)mThis.ptr(i);
		for (j=0;j<cols;j++,dDst++)
			*dDst = 0;
    }
	for (i=0,s=0;i<rows;i++)
    {
        double	*dDst = (double *)mThis.ptr(i);
		for (j=0;j<cols;j++,dDst++)
			{
			if (i!=(rows-1)/2)
                h=sin(i-(rows-1)/2)/(i-(rows-1)/2);
            else
                h=1;
			if (j!=(cols-1)/2)
                h=h*sin(j-(cols-1)/2)/(j-(cols-1)/2);
            else
                h=1*h;
			w = pi*(ly-(i-(rows-1)/2.)); 
			if (w!=0)
				*dDst = double(sin(w)/w)*h;
			else
				*dDst = double(1)*h;
			h =0.54+0.46*cos(2*pi*(j-(cols-1)/2.)/cols);
			w = pi*(lx-(j-(cols-1)/2));
			if (w!=0)
				*dDst = *dDst * double(sin(w)/w);
			s += *dDst;
			}
    }
    }
mThis /=  s;
std::vector<ImageInfoCV	*>  r;
r.push_back(this);
return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::Recons2d(std::vector< ImageInfoCV *> op, ParametreOperation *pOCV)//(long nbPts,long tailleOperateur)
{
cv::Mat mThis = getMat(cv::ACCESS_RW);
    int nbPts = pOCV->intParam["nbPts"].valeur;
    int tailleOperateur = pOCV->intParam["tailleOperateur"].valeur;
ImageInfoCV 	*imRecons=new ImageInfoCV((1+nbPts)* op[0]->rows,(1+nbPts)* op[0]->cols,CV_8UC(op[0]->channels()));
ImageInfoCV     filtre(2*tailleOperateur+1,2*tailleOperateur+1,CV_64FC1);
unsigned char	*dSrc;
unsigned char	*dDst;
long	i,j,k,l,m,n;
long	nbTour=0;
ParametreOperation pSinc;

std::vector<Mat> plan;
std::vector<Mat> planRecons;
cv::Mat mRecons = imRecons->getMat(cv::ACCESS_RW);

split(mThis,plan);
split(mRecons,planRecons);

for (long nb=0;nb<plan.size();nb++)
	{
	for (i=0;i<nbPts+1;i++)
		for (j=0;j<nbPts+1;j++)
		{
            pSinc.doubleParam["x"].valeur=i-(nbPts+1)/2.;
            pSinc.doubleParam["y"].valeur=j-(nbPts+1)/2.;
            pSinc.intParam["nbPts"].valeur=nbPts+1;
            std::vector<ImageInfoCV*> pp;
            pp.push_back(&filtre);
            filtre.SincXY(pp,&pSinc);
            cv::Mat mFiltre = filtre.getMat(cv::ACCESS_RW);

			dSrc = (unsigned char	*)plan[nb].ptr(0);
 			for (l=0;l<rows;l++,dDst += (nbPts)*mRecons.cols)
 			{
       		    dDst = (unsigned char	*)planRecons[nb].ptr(j+l*(nbPts+1)) +i;

				for (k=0;k<cols;k++,dDst += nbPts+1,dSrc++)
 				{
 					double	s=0;
 					long	t=tailleOperateur;
                    double	*fil=(double *)mFiltre.ptr(0);
 					for (m=-t;m<=t;m++)
                    {

 						for (n=-t;n<=t;n++,fil++)
                            if (l+m>=0 && l+m<rows && k+n>=0 && k+n<cols)
                                s += plan[nb].at<uchar>(l+m,k+n) * *fil;
                    }
					if (s<0)
 						*dDst = 0;
 					else if (s<255)
 						*dDst = (unsigned char)s;
 					else
 						*dDst = 255;
 				}
			}
        }
	}
cv::merge(planRecons,mRecons);

std::vector<ImageInfoCV	*>  r;
r.push_back(imRecons);
return r;
}

