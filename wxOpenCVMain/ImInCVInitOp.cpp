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


if (nbOp!=-1)
	{
	r = new ImageInfoCV*[nbOp];
	for (i=0;i<nbOp;i++)
		r[i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
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
		for (indOp=0;indOp<19;indOp++)
			r[indOp] = new ImageInfoCV(nbLop,nbCop,CV_32F);
		}
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[0]->at<float>(i,j)  =(float) 1./9;
// Moyenne pondérée
	r[1]->at<float>(0,0)=(float)7./78;
	r[1]->at<float>(0,1)=(float)10./78;
	r[1]->at<float>(0,2)=(float)7./78;
	r[1]->at<float>(1,0)=(float)10./78;
	r[1]->at<float>(1,1)=(float)10./78;
	r[1]->at<float>(1,2)=(float)10./78;
	r[1]->at<float>(2,0)=(float)7./78;
	r[1]->at<float>(2,1)=(float)10./78;
	r[1]->at<float>(2,2)=(float)7./78;
// Différence   x
	r[2]->at<float>(0,0)=0;
	r[2]->at<float>(0,1)=0;
	r[2]->at<float>(0,2)=0;
	r[2]->at<float>(1,0)=(float)-1./2;
	r[2]->at<float>(1,1)=0;
	r[2]->at<float>(1,2)=(float)1./2;
	r[2]->at<float>(2,0)=0;
	r[2]->at<float>(2,1)=0;
	r[2]->at<float>(2,2)=0;
// Différence   y
	r[3]->at<float>(0,0)=0;
	r[3]->at<float>(0,1)=(float)-1./2;
	r[3]->at<float>(0,2)=0;
	r[3]->at<float>(1,0)=0;
	r[3]->at<float>(1,1)=0;
	r[3]->at<float>(1,2)=0;
	r[3]->at<float>(2,0)=0;
	r[3]->at<float>(2,1)=(float)1./2;
	r[3]->at<float>(2,2)=0;
// Roberts x
	r[4]->at<float>(0,0)=1;
	r[4]->at<float>(0,1)=0;
	r[4]->at<float>(0,2)=0;
	r[4]->at<float>(1,0)=0;
	r[4]->at<float>(1,1)=-1;
	r[4]->at<float>(1,2)=0;
	r[4]->at<float>(2,0)=0;
	r[4]->at<float>(2,1)=0;
	r[4]->at<float>(2,2)=0;
// Roberts y
	r[5]->at<float>(0,0)=0;
	r[5]->at<float>(0,1)=-1;
	r[5]->at<float>(0,2)=0;
	r[5]->at<float>(1,0)=1;
	r[5]->at<float>(1,1)=0;
	r[5]->at<float>(1,2)=0;
	r[5]->at<float>(2,0)=0;
	r[5]->at<float>(2,1)=0;
	r[5]->at<float>(2,2)=0;
// Prewitt x
	r[6]->at<float>(0,0)=(float)-1./6;
	r[6]->at<float>(0,1)=0;
	r[6]->at<float>(0,2)=(float)1./6;
	r[6]->at<float>(1,0)=(float)-1./6;
	r[6]->at<float>(1,1)=0;
	r[6]->at<float>(1,2)=(float)1./6;
	r[6]->at<float>(2,0)=(float)-1./6;
	r[6]->at<float>(2,1)=0;
	r[6]->at<float>(2,2)=(float)1./6;
// Prewitt y
	r[7]->at<float>(0,0)=(float)-1./6;
	r[7]->at<float>(0,1)=(float)-1./6;
	r[7]->at<float>(0,2)=(float)-1./6;
	r[7]->at<float>(1,0)=0;
	r[7]->at<float>(1,1)=0;
	r[7]->at<float>(1,2)=0;
	r[7]->at<float>(2,0)=(float)1./6;
	r[7]->at<float>(2,1)=(float)1./6;
	r[7]->at<float>(2,2)=(float)1./6;
// Sobel x
	r[8]->at<float>(0,0)=(float)1./8;
	r[8]->at<float>(0,1)=0;
	r[8]->at<float>(0,2)=(float)-1./8;
	r[8]->at<float>(1,0)=(float)1./4;
	r[8]->at<float>(1,1)=0;
	r[8]->at<float>(1,2)=(float)-1./4;
	r[8]->at<float>(2,0)=(float)1./8;
	r[8]->at<float>(2,1)=0;
	r[8]->at<float>(2,2)=(float)-1./8;
// Sobel y
	r[9]->at<float>(0,0)=(float)-1./8;
	r[9]->at<float>(0,1)=(float)-1./4;
	r[9]->at<float>(0,2)=(float)-1./8;
	r[9]->at<float>(1,0)=0;
	r[9]->at<float>(1,1)=0;
	r[9]->at<float>(1,2)=0;
	r[9]->at<float>(2,0)=(float)1./8;
	r[9]->at<float>(2,1)=(float)1./4;
	r[9]->at<float>(2,2)=(float)1./8;
// Frei-Chen x
	r[10]->at<float>(0,0)=(float)10/(20+10*sqrt(2.));
	r[10]->at<float>(0,1)=0;
	r[10]->at<float>(0,2)=(float)-10/(20+10*sqrt(2.));
	r[10]->at<float>(1,0)=(float)14/(20+10*sqrt(2.));
	r[10]->at<float>(1,1)=0;
	r[10]->at<float>(1,2)=(float)-14/(20+10*sqrt(2.));
	r[10]->at<float>(2,0)=(float)10/(20+10*sqrt(2.));
	r[10]->at<float>(2,1)=0;
	r[10]->at<float>(2,2)=(float)-10/(20+10*sqrt(2.));
// Frei-Chen y
	r[11]->at<float>(0,0)=(float)-10/(20+10*sqrt(2.));
	r[11]->at<float>(0,1)=(float)-14/(20+10*sqrt(2.));
	r[11]->at<float>(0,2)=(float)-10/(20+10*sqrt(2.));
	r[11]->at<float>(1,0)=0;
	r[11]->at<float>(1,1)=0;
	r[11]->at<float>(1,2)=0;
	r[11]->at<float>(2,0)=(float)10/(20+10*sqrt(2.));
	r[11]->at<float>(2,1)=(float)14/(20+10*sqrt(2.));
	r[11]->at<float>(2,2)=(float)10/(20+10*sqrt(2.));
// Laplacien 4
	r[12]->at<float>(0,0)=0;
	r[12]->at<float>(0,1)=-1;
	r[12]->at<float>(0,2)=0;
	r[12]->at<float>(1,0)=-1;
	r[12]->at<float>(1,1)=4;
	r[12]->at<float>(1,2)=-1;
	r[12]->at<float>(2,0)=0;
	r[12]->at<float>(2,1)=-1;
	r[12]->at<float>(2,2)=0;
	r[13]->at<float>(0,0)=(float)-1./8;
	r[13]->at<float>(0,1)=(float)-1./8;
	r[13]->at<float>(0,2)=(float)-1./8;
	r[13]->at<float>(1,0)=(float)-1./8;
	r[13]->at<float>(1,1)=1;
	r[13]->at<float>(1,2)=(float)-1./8;
	r[13]->at<float>(2,0)=(float)-1./8;
	r[13]->at<float>(2,1)=(float)-1./8;
	r[13]->at<float>(2,2)=(float)-1./8;
	for (int k=14;k<19;k++)
		{
		for (int i=0;i<nbLop;i++)
			for (int j=0;j<nbCop;j++)
				r[k]->at<float>(i,j) = 0;
		}
	}
if(nbL==5 || nbOp==-1)
	{
	if (nbOp==-1)
		{
		nbLop=5;
		nbCop=5;
		for (i=0;i<6;i++)
			r[indOp+i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
		}
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp]->at<float>(i,j)  =(float) 1./(nbL*nbC);
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) 1./(1+abs(i-2)+abs(j-2));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp+1]->at<float>(i,j)  =1./(1+abs(i-2)+abs(j-2))/s;
	
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) exp(-((i-2)*(i-2)+(j-2)*(j-2)));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp+2]->at<float>(i,j)  =(float) exp(-((i-2)*(i-2)+(j-2)*(j-2)))/s;
	for (int jj=3;jj<6;jj++)
		{
		for (int i=0;i<nbLop;i++)
			for (int j=0;j<nbCop;j++)
				r[indOp+jj]->at<float>(i,j)  =0;
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
			r[indOp+i] = new ImageInfoCV(nbLop,nbCop,CV_32F);
		}
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp]->at<float>(i,j)  =(float) 1./(nbL*nbC);
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) 1./(1+abs(i-3)+abs(j-3));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp+1]->at<float>(i,j)  =(float) 1./(1+abs(i-3)+abs(j-3))/s;
	
	s=0;
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			s +=(float) exp(-((i-3)*(i-3)+(j-3)*(j-3)));
	for (int i=0;i<nbLop;i++)
		for (int j=0;j<nbCop;j++)
			r[indOp+2]->at<float>(i,j)  =(float) exp(-((i-3)*(i-3)+(j-3)*(j-3)))/s;
	for (int jj=3;jj<nbOp;jj++)
		{
		for (int i=0;i<nbLop;i++)
			for (int j=0;j<nbCop;j++)
				r[indOp+jj]->at<float>(i,j)  =0;
		}
	}
return r;
}
