#include "ImageInfo.h"

ImageInfoCV 	**OperateurConv(long nbop,long nbL,long nbC);
ImageInfoCV 	**OperateurMorph();

ImageInfoCV **ImageInfoCV::op33=OperateurConv(19,3,3);
ImageInfoCV **ImageInfoCV::op55=OperateurConv(6,5,5);
ImageInfoCV **ImageInfoCV::op77=OperateurConv(6,7,7);
ImageInfoCV **ImageInfoCV::opnn=OperateurConv(-1,-1,-1);
ImageInfoCV **ImageInfoCV::opMorph=OperateurMorph();


void ImageInfoCV::InitOp()
{
if (ImageInfoCV::indOpConvolution!=-1)
	return;
return;
indOpConvolution=0;
indOpMorphologie=0;
op33=OperateurConv(19,3,3);
op55=OperateurConv(6,5,5);	
op77=OperateurConv(6,7,7);
opnn=OperateurConv(-1,-1,-1);
opMorph=OperateurMorph();
}

void ImageInfoCV::EffaceOp()
{
for (int i=0;i<19;i++)
	delete op33[i];
delete []op33;
for (int i=0;i<6;i++)
	delete op55[i];
delete[]op55;
for (int i=0;i<6;i++)
	delete op77[i];
delete[]op77;
for (int i=0;i<19+6+6;i++)
	delete opnn[i];
delete[]opnn;
for (int i=0;i<NB_OP_MORPHOLOGIE;i++)
		delete opMorph[i];
delete[]opMorph;
}

ImageInfoCV **OperateurMorph()
{
ImageInfoCV 	**r = new ImageInfoCV*[NB_OP_MORPHOLOGIE];
	for (int i=0;i<NB_OP_MORPHOLOGIE;i++)
		r[i] = NULL;
return r;
}


ImageInfoCV **OperateurConv(long nbOp,long nbL,long nbC)
{
ImageInfoCV 	**r;
long		i;
float		s=0;
int			nbLop=nbL,nbCop=nbC;
int			indOp=0;
std::vector<cv::Mat> rTmp(NB_OP_CONVOLUTION);


if (nbOp!=-1)
	{
	r = new ImageInfoCV*[nbOp];
    for (i = 0; i < nbOp; i++)
    {
		r[i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
        rTmp[i] = cv::Mat::zeros(nbLop,nbCop,CV_32F);
    }
	}
else
	{
	r = new ImageInfoCV*[NB_OP_CONVOLUTION];
	}
	
if(nbL==3 || nbOp==-1)
	{
	if (nbOp==-1)
	{
		nbLop=3;
		nbCop=3;
        for (indOp = 0; indOp < 19; indOp++)
        {
			r[indOp] = new ImageInfoCV(nbLop,nbCop,CV_32F);
            rTmp[indOp] = cv::Mat::zeros(nbLop,nbCop,CV_32F);
        }
    }
    for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[0].at<float>(i,j)  =(float) 1./9;
// Moyenne pondérée
	rTmp[1].at<float>(0,0)=(float)7./78;
	rTmp[1].at<float>(0,1)=(float)10./78;
	rTmp[1].at<float>(0,2)=(float)7./78;
	rTmp[1].at<float>(1,0)=(float)10./78;
	rTmp[1].at<float>(1,1)=(float)10./78;
	rTmp[1].at<float>(1,2)=(float)10./78;
	rTmp[1].at<float>(2,0)=(float)7./78;
	rTmp[1].at<float>(2,1)=(float)10./78;
	rTmp[1].at<float>(2,2)=(float)7./78;
// Différence   x
	rTmp[2].at<float>(0,0)=0;
	rTmp[2].at<float>(0,1)=0;
	rTmp[2].at<float>(0,2)=0;
	rTmp[2].at<float>(1,0)=(float)-1./2;
	rTmp[2].at<float>(1,1)=0;
	rTmp[2].at<float>(1,2)=(float)1./2;
	rTmp[2].at<float>(2,0)=0;
	rTmp[2].at<float>(2,1)=0;
	rTmp[2].at<float>(2,2)=0;
// Différence   y
	rTmp[3].at<float>(0,0)=0;
	rTmp[3].at<float>(0,1)=(float)-1./2;
	rTmp[3].at<float>(0,2)=0;
	rTmp[3].at<float>(1,0)=0;
	rTmp[3].at<float>(1,1)=0;
	rTmp[3].at<float>(1,2)=0;
	rTmp[3].at<float>(2,0)=0;
	rTmp[3].at<float>(2,1)=(float)1./2;
	rTmp[3].at<float>(2,2)=0;
// Roberts x
	rTmp[4].at<float>(0,0)=1;
	rTmp[4].at<float>(0,1)=0;
	rTmp[4].at<float>(0,2)=0;
	rTmp[4].at<float>(1,0)=0;
	rTmp[4].at<float>(1,1)=-1;
	rTmp[4].at<float>(1,2)=0;
	rTmp[4].at<float>(2,0)=0;
	rTmp[4].at<float>(2,1)=0;
	rTmp[4].at<float>(2,2)=0;
// Roberts y
	rTmp[5].at<float>(0,0)=0;
	rTmp[5].at<float>(0,1)=-1;
	rTmp[5].at<float>(0,2)=0;
	rTmp[5].at<float>(1,0)=1;
	rTmp[5].at<float>(1,1)=0;
	rTmp[5].at<float>(1,2)=0;
	rTmp[5].at<float>(2,0)=0;
	rTmp[5].at<float>(2,1)=0;
	rTmp[5].at<float>(2,2)=0;
// Prewitt x
	rTmp[6].at<float>(0,0)=(float)-1./6;
	rTmp[6].at<float>(0,1)=0;
	rTmp[6].at<float>(0,2)=(float)1./6;
	rTmp[6].at<float>(1,0)=(float)-1./6;
	rTmp[6].at<float>(1,1)=0;
	rTmp[6].at<float>(1,2)=(float)1./6;
	rTmp[6].at<float>(2,0)=(float)-1./6;
	rTmp[6].at<float>(2,1)=0;
	rTmp[6].at<float>(2,2)=(float)1./6;
// Prewitt y
	rTmp[7].at<float>(0,0)=(float)-1./6;
	rTmp[7].at<float>(0,1)=(float)-1./6;
	rTmp[7].at<float>(0,2)=(float)-1./6;
	rTmp[7].at<float>(1,0)=0;
	rTmp[7].at<float>(1,1)=0;
	rTmp[7].at<float>(1,2)=0;
	rTmp[7].at<float>(2,0)=(float)1./6;
	rTmp[7].at<float>(2,1)=(float)1./6;
	rTmp[7].at<float>(2,2)=(float)1./6;
// Sobel x
	rTmp[8].at<float>(0,0)=(float)1./8;
	rTmp[8].at<float>(0,1)=0;
	rTmp[8].at<float>(0,2)=(float)-1./8;
	rTmp[8].at<float>(1,0)=(float)1./4;
	rTmp[8].at<float>(1,1)=0;
	rTmp[8].at<float>(1,2)=(float)-1./4;
	rTmp[8].at<float>(2,0)=(float)1./8;
	rTmp[8].at<float>(2,1)=0;
	rTmp[8].at<float>(2,2)=(float)-1./8;
// Sobel y
	rTmp[9].at<float>(0,0)=(float)-1./8;
	rTmp[9].at<float>(0,1)=(float)-1./4;
	rTmp[9].at<float>(0,2)=(float)-1./8;
	rTmp[9].at<float>(1,0)=0;
	rTmp[9].at<float>(1,1)=0;
	rTmp[9].at<float>(1,2)=0;
	rTmp[9].at<float>(2,0)=(float)1./8;
	rTmp[9].at<float>(2,1)=(float)1./4;
	rTmp[9].at<float>(2,2)=(float)1./8;
// Frei-Chen x
	rTmp[10].at<float>(0,0)=(float)10/(20+10*sqrt(2.));
	rTmp[10].at<float>(0,1)=0;
	rTmp[10].at<float>(0,2)=(float)-10/(20+10*sqrt(2.));
	rTmp[10].at<float>(1,0)=(float)14/(20+10*sqrt(2.));
	rTmp[10].at<float>(1,1)=0;
	rTmp[10].at<float>(1,2)=(float)-14/(20+10*sqrt(2.));
	rTmp[10].at<float>(2,0)=(float)10/(20+10*sqrt(2.));
	rTmp[10].at<float>(2,1)=0;
	rTmp[10].at<float>(2,2)=(float)-10/(20+10*sqrt(2.));
// Frei-Chen y
	rTmp[11].at<float>(0,0)=(float)-10/(20+10*sqrt(2.));
	rTmp[11].at<float>(0,1)=(float)-14/(20+10*sqrt(2.));
	rTmp[11].at<float>(0,2)=(float)-10/(20+10*sqrt(2.));
	rTmp[11].at<float>(1,0)=0;
	rTmp[11].at<float>(1,1)=0;
	rTmp[11].at<float>(1,2)=0;
	rTmp[11].at<float>(2,0)=(float)10/(20+10*sqrt(2.));
	rTmp[11].at<float>(2,1)=(float)14/(20+10*sqrt(2.));
	rTmp[11].at<float>(2,2)=(float)10/(20+10*sqrt(2.));
// Laplacien 4
	rTmp[12].at<float>(0,0)=0;
	rTmp[12].at<float>(0,1)=-1;
	rTmp[12].at<float>(0,2)=0;
	rTmp[12].at<float>(1,0)=-1;
	rTmp[12].at<float>(1,1)=4;
	rTmp[12].at<float>(1,2)=-1;
	rTmp[12].at<float>(2,0)=0;
	rTmp[12].at<float>(2,1)=-1;
	rTmp[12].at<float>(2,2)=0;
	rTmp[13].at<float>(0,0)=(float)-1./8;
	rTmp[13].at<float>(0,1)=(float)-1./8;
	rTmp[13].at<float>(0,2)=(float)-1./8;
	rTmp[13].at<float>(1,0)=(float)-1./8;
	rTmp[13].at<float>(1,1)=1;
	rTmp[13].at<float>(1,2)=(float)-1./8;
	rTmp[13].at<float>(2,0)=(float)-1./8;
	rTmp[13].at<float>(2,1)=(float)-1./8;
	rTmp[13].at<float>(2,2)=(float)-1./8;
	for (int k=0;k<19;k++)
		{
        rTmp[k].copyTo(*(r[k]));
		}
	}
if(nbL==5 || nbOp==-1)
	{
	if (nbOp==-1)
	{
		nbLop=5;
		nbCop=5;
		for (i=0;i<6;i++)
        {
            r[indOp+i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
            rTmp[indOp+i] = cv::Mat::zeros(nbLop,nbCop,CV_32F);
		}
    }
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp].at<float>(i,j)  =(float) 1./(nbL*nbC);
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) 1./(1+abs(i-2)+abs(j-2));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp+1].at<float>(i,j)  =1./(1+abs(i-2)+abs(j-2))/s;
	
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) exp(-((i-2)*(i-2)+(j-2)*(j-2)));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp+2].at<float>(i,j)  =(float) exp(-((i-2)*(i-2)+(j-2)*(j-2)))/s;
	for (int jj=0;jj<6;jj++)
		{
        rTmp[indOp + jj].copyTo(*(r[indOp + jj]));
		}
	indOp+=6;
	}
if ( nbL==7 || nbOp==-1)
{
	if (nbOp==-1)
	{
		nbLop=7;
		nbCop=7;
		nbOp=6;
		for (i=0;i<nbOp;i++)
        {
			r[indOp+i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
            rTmp[indOp+i] = cv::Mat::zeros(nbLop,nbCop,CV_32F);
		}
    }
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp].at<float>(i,j)  =(float) 1./(nbL*nbC);
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) 1./(1+abs(i-3)+abs(j-3));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp+1].at<float>(i,j)  =(float) 1./(1+abs(i-3)+abs(j-3))/s;
	
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) exp(-((i-3)*(i-3)+(j-3)*(j-3)));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			rTmp[indOp+2].at<float>(i,j)  =(float) exp(-((i-3)*(i-3)+(j-3)*(j-3)))/s;
	for (int jj=3;jj<nbOp;jj++)
		{
		rTmp[indOp+jj].copyTo(*(r[indOp + jj]));
		}
	}
    return r;
}
