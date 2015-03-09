#include "imageInfo.h"


std::vector<cv::Moments> *ImageInfoCV::CalcMoment()
{
std::vector<cv::Moments>  *m=new std::vector<cv::Moments>[channels()];
int nbCanaux=channels();

if (depth()==CV_32F ||  depth()==CV_64F)
	throw std::logic_error( "Incoheret type" ); 

if (minIm==NULL)
	ExtremumLoc();
for (int i=0;i<nbCanaux;i++)
	{
	m[i].resize(maxIm[i]+1);
	}
switch(depth()){
case CV_32S :
	for (int i=0;i<rows;i++)		
		{
		long *d=(long*)ptr(i);
		for (int j=0;j<cols;j++)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				if (*d>=0)
					{
					m[indCanal][*d].m00++;
					m[indCanal][*d].m01+=i;
					m[indCanal][*d].m10+=j;
					}

				}
			}
		}
	break;
case CV_16U :
	for (int i=0;i<rows;i++)		
		{
		unsigned short *d=(unsigned short*)ptr(i);
		for (int j=0;j<cols;j++)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				m[indCanal][*d].m00++;
				m[indCanal][*d].m01+=i;
				m[indCanal][*d].m10+=j;
				}
			}
		}
	break;
case CV_16S :
	for (int i=0;i<rows;i++)		
		{
		short *d=(short*)ptr(i);
		for (int j=0;j<cols;j++)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				if (*d>=0)
					{
					m[indCanal][*d].m00++;
					m[indCanal][*d].m01+=i;
					m[indCanal][*d].m10+=j;
					}
				}
			}
		}
	break;
case CV_8U :
	for (int i=0;i<rows;i++)		
		{
		unsigned char *d=data+i*step[0];
		float *g=NULL;
		for (int j=0;j<cols;j++)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				m[indCanal][*d].m00++;
				m[indCanal][*d].m01+=i;
				m[indCanal][*d].m10+=j;
				}
			}
		}
	break;
default :
	throw ;
	}
for (int indCanal=0;indCanal<nbCanaux;indCanal++)
	{
	for (int j=0;j<=maxIm[indCanal];j++)
		if (m[indCanal][j].m00>0)
		{
		m[indCanal][*d].m01 /=m[indCanal][j].m00;
		m[indCanal][*d].m10 /=m[indCanal][j].m00;
		}
	}





for ( nb=0;nb<nbPlan;nb++)
	{
	long			*dl=(long *)LitPlan(nb);
	unsigned short	*dd=(unsigned short *)LitPlan(nb);
	unsigned char	*dc=(unsigned char *)LitPlan(nb);
	for (i=0;i<nbLigne;i++)
		for (j=0;j<nbColonne;j++)
			{
			switch (LitTaillePixel()){
			case T_OCTET_IMAGE :
				k = (unsigned char )dc[i*nbColonne+j];
				break;
			case T_SHORT_IMAGE :
				k = (unsigned short)dd[i*nbColonne+j];
				break;
			case T_LONG_IMAGE :
				k = (long)dl[i*nbColonne+j];
				break;
			default :
				k = 0;
				break;
				}
			if (k<=nbNiv)
				{
				par[k*5+3] += (par[k*5+1] - j) * (par[k*5+1] - j);
				par[k*5+4] += (par[k*5+2] - i) * (par[k*5+2] - i);
				}
			}
	}
for (int indCanal=0;indCanal<nbCanaux;indCanal++)
	{
	for (int j=0;j<=maxIm[indCanal];j++)
		if (m[indCanal][j].m00>0)
		{
		m[indCanal][*d].m01 /=m[indCanal][j].m00;
		m[indCanal][*d].m10 /=m[indCanal][j].m00;
		}
	}

for (i=0;i<=nbNiv;i++)
	if (par[i*5] != 0)
		{
		par[i*5+3] /= par[i*5+0];
		par[i*5+4] /= par[i*5+0];
		}
return par;
return NULL;
}


