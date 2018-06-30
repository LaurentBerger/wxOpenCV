#include "ImageInfo.h"

using namespace cv;

#ifdef OLDGRADIENT


std::vector<ImageInfoCV	*> ImageInfoCV::GradientDericheXOld(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)//double alphaDerive,double alphaMoyenne)
{
std::vector<ImageInfoCV	*> r;

Mat im1(size(),CV_32FC(channels()));
ImageInfoCV *imRes=new ImageInfoCV(rows,cols,CV_32FC(channels()));
cv::Mat mThis = getMat(cv::ACCESS_RW);
cv::Mat m2 = imRes->getMat(cv::ACCESS_RW);

float				*f1,*f2;

long				i,j,nb;
long				tailleSequence=(rows>cols)?rows:cols;
double				*g1=new double[tailleSequence],*g2=new double[tailleSequence];
double	p=pOCV->doubleParam["alphaDerive"].valeur;
double	k=pow(1-exp(-p),2.0)/(1+2*p*exp(-p)-exp(-2*p));
double	kp=pow(1-exp(-p),2.0)/exp(-p);
double a1=1,a2=0;
double a3=0,a4=0;
double a5=0,a6=kp*exp(-p),a7=-kp*exp(-p),a8=0;
double b1=0,b3=2*exp(-p);
double b2=0,b4=-2*exp(-p);



kp=pow(1-exp(-p),2.0)/exp(-p);
a1=0,a2=kp*exp(-p),a3=-kp*exp(-p),a4=0;
b1=2*exp(-p);
b2=-exp(-2*p);

	
switch(depth()){
case CV_8U :
	{
	unsigned char		*c1;
	for (nb=0;nb<channels();nb++)
		{
		for (j=0;j<cols;j++)
			{
			// Filtre causal vertical
			c1 = (unsigned char*)mThis.ptr(0)+nb;
			f2 = (float*)im1.ptr(0)+nb;
			f2 += j;
			c1+=j;
			i=0;
			g1[i] = (a1 + a2 +b1+b2)* *c1  ;
			g1[i] = (a1 + a2 )* *c1  ;
			i++;
			c1+=cols;
			g1[i] = a1 * *c1 + a2 * c1[-cols] + (b1+b2) * g1[i-1];
			g1[i] = a1 * *c1 + a2 * c1[-cols] + (b1) * g1[i-1];
			i++;
			c1+=cols;
			for (i=2;i<rows;i++,c1+=cols)
				g1[i] = a1 * *c1 + a2 * c1[-cols] +b1*g1[i-1]+
						b2 *g1[i-2];
						
			// Filtre anticausal vertical
			c1 = (unsigned char*)mThis.ptr(0)+nb;
			c1 += (rows-1)*cols+j;
			i = rows-1;
			g2[i] =(a3+a4+b1+b2)* *c1; 
			g2[i] =(a3+a4)* *c1; 
			i--;
			c1-=cols;
			g2[i] = a3* c1[cols] + a4 * c1[cols]+(b1+b2)*g2[i+1];
			g2[i] = a3* c1[cols] + a4 * c1[cols]+(b1)*g2[i+1];
			i--;
			c1-=cols;
			for (i=rows-3;i>=0;i--,c1-=cols)
				g2[i] = a3*c1[cols] +a4* c1[2*cols]+
						b1*g2[i+1]+b2*g2[i+2];
			for (i=0;i<rows;i++,f2+=cols)
				*f2 = (float)(g1[i]+g2[i]);
			}
		}
	}
	break;
case CV_16S :
case CV_16U :
	{
	unsigned short		*c1;
	for (nb=0;nb<channels();nb++)
		{
		for (j=0;j<cols;j++)
			{
			c1 = ((unsigned short*)mThis.ptr(0))+nb;
			f2 = ((float*)im1.ptr(0))+nb;
			f2 += j;
			c1+=j;
			i=0;
			g1[i] = (a1 + a2 +b1+b2)* *c1  ;
			g1[i] = (a1 + a2 )* *c1  ;
			i++;
			c1+=cols;
			g1[i] = a1 * *c1 + a2 * c1[-cols] + (b1+b2) * g1[i-1];
			g1[i] = a1 * *c1 + a2 * c1[-cols] + (b1) * g1[i-1];
			i++;
			c1+=cols;
			for (i=2;i<rows;i++,c1+=cols)
				g1[i] = a1 * *c1 + a2 * c1[-cols] +b1*g1[i-1]+
						b2 *g1[i-2];
						
			// Filtre anticausal vertical
			c1 = ((unsigned short*)mThis.ptr(0))+nb;
			c1 += (rows-1)*cols+j;
			i = rows-1;
			g2[i] =(a3+a4+b1+b2)* *c1; 
			g2[i] =(a3+a4)* *c1; 
			i--;
			c1-=cols;
			g2[i] = (a3+a4)* c1[cols] +(b1+b2)*g2[i+1];
			g2[i] = (a3+a4)* c1[cols] +(b1)*g2[i+1];
			i--;
			c1-=cols;
			for (i=rows-3;i>=0;i--,c1-=cols)
				g2[i] = a3*c1[cols] +a4* c1[2*cols]+
						b1*g2[i+1]+b2*g2[i+2];
			c1 = ((unsigned short*)mThis.ptr(0))+nb+j;
			for (i=0;i<rows;i++,f2+=cols,c1+=cols)
				*f2 = 0**c1+(float)(g1[i]+g2[i]);
			}
		}
	}
	 break;
case CV_32S :
	 break;
case CV_32F :
	 break;
case CV_64F :
	 break;
default :
	return r;
	}
p=pOCV->doubleParam["alphaMoyenne"].valeur;
k=pow(1-exp(-p),2.0)/(1+2*p*exp(-p)-exp(-2*p));
a5=k,a6=k*exp(-p)*(p-1);
a7=k*exp(-p)*(p+1),a8=-k*exp(-2*p);
b3=2*exp(-p);
b4=-exp(-2*p);

for (nb=0;nb<channels();nb++)
	{
	for (i=0;i<rows;i++)
		{
	    f2 = ((float*)m2.ptr(i))+nb;;
		f1 = ((float*)im1.ptr(i))+nb;
		j=0;
		g1[j] = (a5 +a6+b3+b4)* *f1 ;
		g1[j] = (a5 +a6)* *f1 ;
		j++;
		f1++;
		g1[j] = a5 * f1[0]+a6*f1[j-1]+(b3+b4) * g1[j-1];
		g1[j] = a5 * f1[0]+a6*f1[j-1]+(b3) * g1[j-1];
		j++;
		f1++;
		for (j=2;j<cols;j++,f1++)
			g1[j] = a5 * f1[0] + a6 * f1[-1]+b3*g1[j-1]+b4*g1[j-2];
		f1 = ((float*)im1.ptr(0))+nb;;
		f1 += i*cols+cols-1;
		j=cols-1;
		g2[j] = (a7+a8+b3+b4)* *f1;
		g2[j] = (a7+a8)* *f1;
		j--;
		f1--;
		g2[j] = (a7+a8) * f1[1]  +(b3+b4) * g2[j+1];
		g2[j] = (a7+a8) * f1[1]  +(b3) * g2[j+1];
		j--;
		f1--;
		for (j=cols-3;j>=0;j--,f1--)
			g2[j] = a7*f1[1]+a8*f1[2]+b3*g2[j+1]+b4*g2[j+2];
		for (j=0;j<cols;j++,f2++)
			*f2 = (float)(g1[j]+g2[j]);
		}
	}
r.push_back(imRes);
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::GradientDericheYOLD(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
/*
http://www.esiee.fr/~coupriem/Algo/algoima.html
*/
Mat im1(size(),CV_32FC(channels()));
ImageInfoCV *imRes=new ImageInfoCV(rows,cols,CV_32FC(channels()));
cv::Mat mThis = getMat(cv::ACCESS_RW);
cv::Mat im2 = imRes->getMat(cv::ACCESS_RW);
float				*f1,*f2;

long				i,j,nb;
long				tailleSequence=(rows>cols)?rows:cols;
double				*g1=new double[tailleSequence],*g2=new double[tailleSequence];
double	p=pOCV->doubleParam["alphaDerive"].valeur;
double	k=pow(1-exp(-p),2.0)/(1+2*p*exp(-p)-exp(-2*p));
double	kp=pow(1-exp(-p),2.0)/exp(-p);
double a1=1,a2=0;
double a3=0,a4=0;
double a5=0,a6=kp*exp(-p),a7=-kp*exp(-p),a8=0;
double b1=0,b3=2*exp(-p);
double b2=0,b4=-2*exp(-p);



kp=pow(1-exp(-p),2.0)/exp(-p);
a1=0,a2=kp*exp(-p),a3=-kp*exp(-p),a4=0;
b1=2*exp(-p);
b2=-exp(-2*p);

switch(depth()){
case CV_8U :
case CV_8S :
	{
	unsigned char		*c1;
	for (nb=0;nb<channels();nb++)
		{
		for (i=0;i<rows;i++)
			{
		    f1 = (float*)im1.ptr(i)+nb;
			c1 = (unsigned char*)mThis.ptr(i)+(nb);
			j=0;
			g1[j] = (a1 +a2+b1+b2)* *c1 ;
			g1[j] = (a1 +a2)* *c1 ;
			j++;
			c1++;
			g1[j] = a1 * c1[0]+a2*c1[j-1]+(b1+b2) * g1[j-1];
			g1[j] = a1 * c1[0]+a2*c1[j-1]+(b1) * g1[j-1];
			j++;
			c1++;
			for (j=2;j<cols;j++,c1++)
				g1[j] = a1 * c1[0] + a2 * c1[-1]+b1*g1[j-1]+b2*g1[j-2];
			c1 = (unsigned char*)mThis.ptr(0)+(nb);
			c1 += i*cols+cols-1;
			j=cols-1;
			g2[j] = (a3+a4+b1+b2)* *c1;
			g2[j] = (a3+a4)* *c1;
			j--;
			g2[j] = (a3+a4) * c1[1]  +(b1+b2) * g2[j+1];
			g2[j] = (a3+a4) * c1[1]  +(b1) * g2[j+1];
			j--;
			c1--;
			for (j=cols-3;j>=0;j--,c1--)
				g2[j] = a3*c1[1]+a4*c1[2]+b1*g2[j+1]+b2*g2[j+2];
			for (j=0;j<cols;j++,f1++)
				*f1 = (float)(g1[j]+g2[j]);
			}
		}
	}
	break;
case CV_16S :
case CV_16U :
	{
	unsigned short		*c1;
	for (nb=0;nb<channels();nb++)
		{
		f1 = ((float*)im1.ptr(0))+nb;
		for (i=0;i<rows;i++)
			{
			c1 = ((unsigned short*)mThis.ptr(0))+(nb);
			c1 += i*cols;
			j=0;
			g1[j] = (a1 +a2+b1+b2)* *c1 ;
			g1[j] = (a1 +a2)* *c1 ;
			j++;
			c1++;
			g1[j] = a1 * c1[0]+a2*c1[j-1]+(b1+b2) * g1[j-1];
			g1[j] = a1 * c1[0]+a2*c1[j-1]+(b1) * g1[j-1];
			j++;
			c1++;
			for (j=2;j<cols;j++,c1++)
				g1[j] = a1 * c1[0] + a2 * c1[-1]+b1*g1[j-1]+b2*g1[j-2];
			c1 = ((unsigned short*)mThis.ptr(0))+(nb);
			c1 += i*cols+cols-1;
			j=cols-1;
			g2[j] = (a3+a4+b1+b2)* *c1;
			g2[j] = (a3+a4)* *c1;
			j--;
			c1--;
			g2[j] = (a3+a4) * c1[1]  +(b1+b2) * g2[j+1];
			g2[j] = (a3+a4) * c1[1]  +(b1) * g2[j+1];
			j--;
			c1--;
			for (j=cols-3;j>=0;j--,c1--)
				g2[j] = a3*c1[1]+a4*c1[2]+b1*g2[j+1]+b2*g2[j+2];
			for (j=0;j<cols;j++,f1++)
				*f1 = (float)(g1[j]+g2[j]);
			}
		}
	}
	break;
default :
	return r;
	}
p=pOCV->doubleParam["alphaMoyenne"].valeur;
k=pow(1-exp(-p),2.0)/(1+2*p*exp(-p)-exp(-2*p));
a5=k,a6=k*exp(-p)*(p-1);
a7=k*exp(-p)*(p+1),a8=-k*exp(-2*p);
b3=2*exp(-p);
b4=-exp(-2*p);

for (nb=0;nb<channels();nb++)
	{
	for (j=0;j<cols;j++)
		{
		f1 = (float*)im1.ptr(0)+(nb);
		f1+=j;
		i=0;
		g1[i] = (a5 + a6 +b3+b4)* *f1  ;
		g1[i] = (a5 + a6 )* *f1  ;
		i++;
		f1+=cols;
		g1[i] = a5 * *f1 + a6 * f1[-cols] + (b3+b4) * g1[i-1];
		g1[i] = a5 * *f1 + a6 * f1[-cols] + (b3) * g1[i-1];
		i++;
		f1+=cols;
		for (i=2;i<rows;i++,f1+=cols)
			g1[i] = a5 * *f1 + a6 * f1[-cols] +b3*g1[i-1]+
					b4 *g1[i-2];
					
		// Filtre anticausal vertical
		f1 = (float*)im1.ptr(0)+(nb);
		f1 += (rows-1)*cols+j;
		i = rows-1;
		g2[i] =(a7+a8+b3+b4)* *f1; 
		g2[i] =(a7+a8)* *f1; 
		i--;
		f1-=cols;
		g2[i] = (a7+a8)* f1[cols] +(b3+b4)*g2[i+1];
		g2[i] = (a7+a8)* f1[cols] +(b3)*g2[i+1];
		i--;
		f1-=cols;
		for (i=rows-3;i>=0;i--,f1-=cols)
			g2[i] = a7*f1[cols] +a8* f1[2*cols]+
					b3*g2[i+1]+b4*g2[i+2];
		for (i=0;i<rows;i++,f2+=cols)
        {
            f2 = ((float*)im2.ptr(i))+(j*channels());
			*f2 = (float)(g1[i]+g2[i]);
        }
		}
	}
r.push_back(imRes);
return r;
}
#endif
