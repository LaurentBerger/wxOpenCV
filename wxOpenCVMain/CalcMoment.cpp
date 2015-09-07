#include "ImageInfo.h"
#include <stdexcept>


std::vector<std::vector<cv::Moments> > *ImageInfoCV::CalcMoment()
{
std::vector<std::vector<cv::Moments> >  m(channels());
huMoment.resize(channels());
int nbCanaux=channels();

if (depth()==CV_32F ||  depth()==CV_64F)
	throw std::logic_error( "Incoherent type" );

if (minIm.size()==0)
	ExtremumLoc();
for (int i=0;i<nbCanaux;i++)
	{
	m[i].resize(maxIm[i]+1);
	}
cv::Mat mThis = getMat(cv::ACCESS_READ);
if (moment.size() == 0)
{
    switch(depth()){
    case CV_32S :
	    for (int i=0;i<rows;i++)
		    {
		    long *d=(long*)mThis.ptr(i);
		    for (int j=0;j<cols;j++)
			    {
			    for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				    {
				    if (*d>=0)
					    {
					    m[indCanal][*d].m00++;
					    m[indCanal][*d].m01+=i;
					    m[indCanal][*d].m02+=i*i;
					    m[indCanal][*d].m03+=i*i*i;
					    m[indCanal][*d].m10+=j;
					    m[indCanal][*d].m20+=j*j;
					    m[indCanal][*d].m30+=j*j*j;
					    m[indCanal][*d].m11+=i*j;
					    }

				    }
			    }
		    }
	    break;
    case CV_16U :
	    for (int i=0;i<rows;i++)
		    {
		    unsigned short *d=(unsigned short*)mThis.ptr(i);
		    for (int j=0;j<cols;j++)
			    {
			    for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				    {
					    m[indCanal][*d].m00++;
					    m[indCanal][*d].m01+=i;
					    m[indCanal][*d].m02+=i*i;
					    m[indCanal][*d].m03+=i*i*i;
					    m[indCanal][*d].m10+=j;
					    m[indCanal][*d].m20+=j*j;
					    m[indCanal][*d].m30+=j*j*j;
					    m[indCanal][*d].m11+=i*j;
				    }
			    }
		    }
	    break;
    case CV_16S :
	    for (int i=0;i<rows;i++)
		    {
		    short *d=(short*)mThis.ptr(i);
		    for (int j=0;j<cols;j++)
			    {
			    for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				    {
				    if (*d>=0)
					    {
					    m[indCanal][*d].m00++;
					    m[indCanal][*d].m01+=i;
					    m[indCanal][*d].m02+=i*i;
					    m[indCanal][*d].m03+=i*i*i;
					    m[indCanal][*d].m10+=j;
					    m[indCanal][*d].m20+=j*j;
					    m[indCanal][*d].m30+=j*j*j;
					    m[indCanal][*d].m11+=i*j;
					    }
				    }
			    }
		    }
	    break;
    case CV_8U :
	    for (int i=0;i<rows;i++)
		    {
		    unsigned char *d=mThis.ptr(i);
		    float *g=NULL;
		    for (int j=0;j<cols;j++)
			    {
			    for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				    {
					    m[indCanal][*d].m00++;
					    m[indCanal][*d].m01+=i;
					    m[indCanal][*d].m02+=i*i;
					    m[indCanal][*d].m03+=i*i*i;
					    m[indCanal][*d].m10+=j;
					    m[indCanal][*d].m20+=j*j;
					    m[indCanal][*d].m30+=j*j*j;
					    m[indCanal][*d].m11+=i*j;
				    }
			    }
		    }
	    break;
    default :
	    throw ;
	    }
}
else
    m=moment;
for (int indCanal = 0; indCanal<nbCanaux; indCanal++)
{
    huMoment[indCanal].resize(maxIm[indCanal]+1);
    for (int k=0;k<=maxIm[indCanal];k++)
		if (m[indCanal][k].m00>0)
			{
			double ybarre=m[indCanal][k].m01/m[indCanal][k].m00,xbarre=m[indCanal][k].m10/m[indCanal][k].m00;
			m[indCanal][k].mu11=m[indCanal][k].m11-ybarre*m[indCanal][k].m10;
			m[indCanal][k].mu02=m[indCanal][k].m02-ybarre*m[indCanal][k].m01;
			m[indCanal][k].mu20=m[indCanal][k].m20-xbarre*m[indCanal][k].m10;
			m[indCanal][k].mu03=m[indCanal][k].m03-3*ybarre*m[indCanal][k].m02+2*ybarre*ybarre*m[indCanal][k].m01;
			m[indCanal][k].mu30=m[indCanal][k].m30-3*xbarre*m[indCanal][k].m20+2*xbarre*xbarre*m[indCanal][k].m10;
			m[indCanal][k].nu02=m[indCanal][k].mu02/pow(m[indCanal][k].m00,2);
			m[indCanal][k].nu20=m[indCanal][k].mu20/pow(m[indCanal][k].m00,2);
			m[indCanal][k].nu03=m[indCanal][k].mu03/pow(m[indCanal][k].m00,2.5);
			m[indCanal][k].nu11=m[indCanal][k].mu11/pow(m[indCanal][k].m00,2);
			m[indCanal][k].nu12=m[indCanal][k].mu12/pow(m[indCanal][k].m00,2.5);
			m[indCanal][k].nu21=m[indCanal][k].mu21/pow(m[indCanal][k].m00,2.5);
			m[indCanal][k].nu30=m[indCanal][k].mu30/pow(m[indCanal][k].m00,2.5);
			huMoment[indCanal][k].resize(7);
			huMoment[indCanal][k][0]=m[indCanal][k].nu20+m[indCanal][k].nu02;
			huMoment[indCanal][k][1]=pow(m[indCanal][k].nu20-m[indCanal][k].nu02,2.0)+4*m[indCanal][k].nu11;
			huMoment[indCanal][k][2]=pow(m[indCanal][k].nu30-3*m[indCanal][k].nu12,2.0)+pow(3*m[indCanal][k].nu21-m[indCanal][k].nu03,2.0);
			huMoment[indCanal][k][3]=pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)+pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0);
			huMoment[indCanal][k][4]=	(m[indCanal][k].nu30-3*m[indCanal][k].nu12)*(m[indCanal][k].nu30+m[indCanal][k].nu12)*
									(pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)-3*pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0))+
									(3*m[indCanal][k].nu21-m[indCanal][k].nu03)*(m[indCanal][k].nu21+m[indCanal][k].nu03)*
									(3*pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)-pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0));
			huMoment[indCanal][k][5]=	(m[indCanal][k].nu20-m[indCanal][k].nu02)*
									(pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)-pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0))+
									4*m[indCanal][k].nu11*(m[indCanal][k].nu30+m[indCanal][k].nu12)*(m[indCanal][k].nu21+m[indCanal][k].nu03);
			huMoment[indCanal][k][6]=	(3*m[indCanal][k].nu21-m[indCanal][k].nu03)*(m[indCanal][k].nu30+m[indCanal][k].nu12)*
									(pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)-3*pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0))-
									(m[indCanal][k].nu30-3*m[indCanal][k].nu12)*(m[indCanal][k].nu21+m[indCanal][k].nu03)*
									(3*pow(m[indCanal][k].nu30+m[indCanal][k].nu12,2.0)-pow(m[indCanal][k].nu21+m[indCanal][k].nu03,2.0));

			}
}
moment=m;
return &moment;
}


