#include "ImageInfo.h"
#include <vector>
#include "opencv2/optflow.hpp"


/**
 * @function IndOpConvolution
 * @brief Choix de l'opérateur de convolution par son indice dans la liste des opérateurs
 */
int		ImageInfoCV::IndOpConvolution(int i)  
{
if (i>=0)
	indOpConvolution=i;
return indOpConvolution;
}


/**
 * @function IndOpMorphologie
 * @brief Choix de l'opérateur de morphologie par son indice dans la liste des opérateurs
 */
int		ImageInfoCV::IndOpMorphologie(int i)
{
if (i>=0)
	indOpMorphologie=i;
return indOpMorphologie;
}

/**
 * @function Add
 * @brief Addition de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Add(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	if (im1->depth()!=im2->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
	cv::add( *im1, *im2, *im,cv::noArray(),pOCV->intParam["ddepth"].valeur );
	}
else
	cv::add( *im1, *im2, *im,cv::noArray(),typeResultat );
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Add
 * @brief Addition de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::AddPonderee(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;


if (im1->depth()!=im2->depth() && pOCV)
	pOCV->intParam["ddepth"].valeur=CV_32F;
if (pOCV)
	cv::addWeighted( *im1,pOCV->intParam["alpha"].valeur, *im2,pOCV->intParam["beta"].valeur,pOCV->intParam["gamma"].valeur, *im,pOCV->intParam["ddepth"].valeur );
else
	cv::addWeighted( *im1,1, *im2,1,0, *im,-1);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Add
 * @brief Différence de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Sub(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	if (im1->depth()!=im2->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
	cv::subtract( *im1, *im2, *im,cv::noArray(),pOCV->intParam["ddepth"].valeur );
	}
else
	cv::subtract( *im1, *im2, *im,cv::noArray(),typeResultat );
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Prod
 * @brief Produit de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Mul(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	if (im1->depth()!=im2->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
	cv::multiply( *im1, *im2, *im,pOCV->doubleParam["scale"].valeur, pOCV->intParam["ddepth"].valeur);
	}
else
	cv::multiply( *im1, *im2, *im,1, typeResultat);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Prod
 * @brief Division de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Div(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	if (im1->depth()!=im2->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
	cv::divide( *im1, *im2, *im,pOCV->doubleParam["scale"].valeur, pOCV->intParam["ddepth"].valeur);
	}
else
	cv::divide( *im1, *im2, *im,1, typeResultat);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::EtLogique(ImageInfoCV	*im1, ImageInfoCV	*im2, ParametreOperation *pOCV )
{
	ImageInfoCV	*im = new ImageInfoCV;

	cv::bitwise_and(*im1, *im2, *im, cv::noArray());
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::OuLogique(ImageInfoCV	*im1, ImageInfoCV	*im2, ParametreOperation *pOCV )
{
	ImageInfoCV	*im = new ImageInfoCV;

	cv::bitwise_or(*im1, *im2, *im, cv::noArray());
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::OuExcluLogique(ImageInfoCV	*im1, ImageInfoCV	*im2, ParametreOperation *pOCV)
{
	ImageInfoCV	*im = new ImageInfoCV;

	cv::bitwise_xor(*im1, *im2, *im, cv::noArray());
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}


/**
* @function Negation
* @brief Negation logique d'une image image
*/
std::vector<ImageInfoCV *>ImageInfoCV::Negation(ImageInfoCV	*im1,  ParametreOperation *pOCV)
{
	ImageInfoCV	*im = new ImageInfoCV;

	cv::bitwise_not(*im1, *im, cv::noArray());
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}



/**
 * @function Erosion
 * @brief Erosion d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Erosion(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::erode( *im1, *im, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
else
	cv::erode( *im1, *im, *im2 );
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Dilatation(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::dilate( *im1, *im, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
else
	cv::dilate( *im1, *im, *im2 );
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::Ouverture(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;


if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im2);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}
 
std::vector<ImageInfoCV *>ImageInfoCV::Fermeture(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im2);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ChapeauHaut(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im2);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ChapeauBas(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im2);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::GradMorph(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im2);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



/**
 * @function Convolution
 * @brief Convolution d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Convolution(ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::filter2D( *im1, *im,typeResultat, *im1->opnn[pOCV->intParam["IndOpConvolution"].valeur] );
else
	cv::filter2D( *im1, *im,typeResultat, *im2);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Laplacien(ImageInfoCV	*im1,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Laplacian( *im1, *im, typeResultat,pOCV->intParam["ksize"].valeur,
	pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function ScharrModule
 * @brief Module du gradient Scharr d'une image im1 
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrModule(ImageInfoCV	*im1,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
ImageInfoCV	imx ;
ImageInfoCV	imy;
ImageInfoCV	imAbsx;
ImageInfoCV	imAbsy;

cv::Scharr( *im1, imx, pOCV->intParam["ddepth"].valeur,1,0,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );
cv::Scharr( *im1, imy, pOCV->intParam["ddepth"].valeur,0,1,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );
abs( imx);
abs( imy);
addWeighted( imx, 0.5, imy, 0.5, 0, *im );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function ScharrX
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrX(ImageInfoCV	*im1,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *im1, *im, pOCV->intParam["ddepth"].valeur,1,0,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


/**
 * @function ScharrY
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrY(ImageInfoCV	*im1,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *im1, *im, pOCV->intParam["ddepth"].valeur,0,1,
	pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



/**
 * @function Canny
 * @brief filtrage de canny d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Canny(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (imSrc->channels()==1)
	{
	cv::Canny(	*imSrc, *im, pOCV->doubleParam["threshold1"].valeur,
				pOCV->doubleParam["threshold2"].valeur,pOCV->intParam["aperture_size"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::Canny( planCouleur[i], d[i], pOCV->doubleParam["threshold1"].valeur,
					pOCV->doubleParam["threshold2"].valeur,pOCV->intParam["aperture_size"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Seuillage
 * @brief Seuillage d'une image imSrc 
 */
std::vector<ImageInfoCV *>ImageInfoCV::SeuillageAdaptatif(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->channels()==1)
	{
	cv::adaptiveThreshold( *imSrc, *im, pOCV->doubleParam["maxValue"].valeur,pOCV->intParam["adaptiveMethod"].valeur,
		pOCV->intParam["thresholdType"].valeur,pOCV->intParam["blockSize"].valeur,pOCV->doubleParam["C"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
	cv::adaptiveThreshold( planCouleur[i], d[i], pOCV->doubleParam["maxValue"].valeur,pOCV->intParam["adaptiveMethod"].valeur,
		pOCV->intParam["thresholdType"].valeur,pOCV->intParam["blockSize"].valeur,pOCV->doubleParam["C"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::Contour(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV *im1=new ImageInfoCV;
*(cv::Mat*)im1= imSrc->clone();
std::vector<std::vector<cv::Point> > f;
if (imSrc->channels()==1)
	{
	cv::findContours( *im1,  f,pOCV->intParam["mode"].valeur,pOCV->intParam["method"].valeur);	
	}
 else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(),*im1);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im1);
return r;
}

void ImageInfoCV::Threshold( cv::InputArray _src, cv::OutputArray _dst, double thresh, double maxval, int type )
{
}

std::vector<ImageInfoCV *>ImageInfoCV::Seuillage(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->depth()==CV_16S)
	{
	pOCV->doubleParam["thresh"].maxi=32767; 
	}
if (imSrc->channels()==1)
	{
	
	threshold( *imSrc, *im, pOCV->doubleParam["thresh"].valeur,pOCV->doubleParam["maxval"].valeur,
		pOCV->intParam["threshold_type"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::threshold(	planCouleur[i], d[i], pOCV->doubleParam["thresh"].valeur,
						pOCV->doubleParam["maxval"].valeur,pOCV->intParam["threshold_type"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;

}
std::vector<ImageInfoCV *>ImageInfoCV::LissageMoyenne(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::blur( *imSrc, *im, pOCV->sizeParam["ksize"].valeur,pOCV->pointParam["anchor"].valeur,pOCV->intParam["borderType"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::LissageGaussien(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::GaussianBlur( *imSrc, *im, pOCV->sizeParam["ksize"].valeur,
	pOCV->doubleParam["sigmaX"].valeur,pOCV->doubleParam["sigmaY"].valeur,
	pOCV->intParam["borderType"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::LissageMedian(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::medianBlur( *imSrc, *im, pOCV->intParam["ksize"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::FFT(ImageInfoCV	*im1,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( im1->rows );
int n = cv::getOptimalDFTSize( im1->cols ); // on the border add zero values

if (im1->channels()==1)
	{
	if (im1->type()!=CV_32FC1 && im->type()!=CV_64FC1)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		im1->convertTo(*imSrc, CV_32F);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,*im,cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::dft(*imSrc,*im,cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::dft(*im1,*im,cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<Mat> planCouleurSrc;
	Mat *planCouleurDst=new Mat[im1->channels()];
	Mat *d=new Mat[im1->channels()];
	cv::split( *im1, planCouleurSrc );
	for (int i=0;i<im1->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32F);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - im1->rows, 0, n - im1->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::dft(planCouleurDst[i],d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	int nb=im->channels();
	delete []d;
	delete []planCouleurDst;

	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::IFFT(ImageInfoCV	*im1,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( im1->rows );
int n = cv::getOptimalDFTSize( im1->cols ); // on the border add zero values

if (im1->channels()==2)
	{
	if (im1->type()!=CV_32FC2 && im->type()!=CV_64FC2)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		im1->convertTo(*imSrc, CV_32FC2);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::idft(*imSrc,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::idft(*im1,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<Mat> planCouleurSrc;
	Mat *planCouleurDst=new Mat[im1->channels()];
	Mat *d=new Mat[im1->channels()];
	cv::split( *im1, planCouleurSrc );
	for (int i=0;i<im1->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32FC2);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - im1->rows, 0, n - im1->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::idft(planCouleurDst[i],d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	int nb=im->channels();
	delete []d;
	delete []planCouleurDst;

	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



std::vector<ImageInfoCV*> ImageInfoCV::FusionPlan(int nbPlan,ImageInfoCV **ve,ParametreOperation *paramOCV)
{
std::vector<ImageInfoCV	*> r;
ImageInfoCV	*result = new ImageInfoCV;
std::vector<cv::Mat> listePlan;
if (nbPlan>=1)
	listePlan.push_back(*(ve[0]));
else 
	return r;
if (nbPlan>=2)
        listePlan.push_back(*(ve[1]));
if (nbPlan>=3)
        listePlan.push_back(*(ve[2]));
cv::merge(listePlan,*result);

r.push_back(result);
return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::RGB_L(ImageInfoCV *im1,ParametreOperation *paramOCV)
{
ImageInfoCV	*result=new ImageInfoCV;
 
cv::cvtColor( *im1, *result,paramOCV->intParam["ColorSpaceCode"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(result);
return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::SeparationPlan(ImageInfoCV *im1,ParametreOperation *paramOCV)
{
ImageInfoCV	**result=new ImageInfoCV*[im1->channels()];
std::vector<Mat> *planCouleurSrc=new std::vector<Mat>;
std::vector<ImageInfoCV	*> r;
 
cv::split( *im1, *planCouleurSrc );
for (int i=0;i<im1->channels();i++)
	{
	result[i]=new ImageInfoCV;
	cv::merge((const cv::Mat *)&(*planCouleurSrc)[i], 1, *result[i]);
	r.push_back(result[i]);
	}
paramOCV->nbImageRes=im1->channels();

return r;

}


std::vector<ImageInfoCV *> ImageInfoCV::ComposanteConnexe(ImageInfoCV *im1,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (im1->channels()==1)
	{
	if (im->statComposante==NULL)
		{
		im->statComposante = new cv::Mat*[im1->channels()]; 
		im->centreGComposante = new cv::Mat*[im1->channels()]; 
		im->contours = new std::vector<std::vector<cv::Point> >[im1->channels()]; 
		im->arbreContour = new std::vector<cv::Vec4i> [im1->channels()]; 

		for (int i=0;i<im1->channels();i++)
			{
			im->statComposante[i] = new cv::Mat; 
			im->centreGComposante[i] = new cv::Mat; 
			}
		}

	connectedComponentsWithStats(*im1, *im,*(im->statComposante[0]),*(im->centreGComposante[0]), paramOCV->intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, *(im->contours),*im->arbreContour, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	im->CalcMoment();

	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[im1->channels()];
	if (im->statComposante==NULL)
		{
		im->statComposante = new cv::Mat*[im1->channels()]; 
		im->centreGComposante = new cv::Mat*[im1->channels()]; 
		im->contours = new std::vector<std::vector<cv::Point> >[im1->channels()]; 
		im->arbreContour = new std::vector<cv::Vec4i> [im1->channels()]; 
		for (int i=0;i<im1->channels();i++)
			{
			im->statComposante[i] = new cv::Mat; 
			im->centreGComposante[i] = new cv::Mat; 
			}
		}
		
	cv::split( *im1, planCouleur );
	for (int i=0;i<im1->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],*(im->statComposante[i]),*(im->centreGComposante[i]), paramOCV->intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	im->CalcMoment();

	std::vector<cv::Moments> *mmt =new std::vector<cv::Moments> [im1->channels()];
	for (int i=0;i<im1->channels();i++)
		{
		mmt[i].resize(im->contours[i].size()); 
		for (int j=0;j<im->contours[i].size();j++)
			{
			mmt[i][j] = cv::moments( im->contours[i][j], false );
			}
		}
	delete []mmt;
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::PartageEaux (ImageInfoCV	*im1,ImageInfoCV	*im2,ParametreOperation *pOCV)

{
std::vector<ImageInfoCV	*> r;
ImageInfoCV	*im =new ImageInfoCV;
if (!im2)
	return r;
im2->convertTo(*im, CV_32S);
watershed(*im1, *im);

r.push_back(im);
return r;
}

std::vector<ImageInfoCV		*> ImageInfoCV::DistanceDiscrete (ImageInfoCV *im1,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (im1->channels()==1)
	{
	cv::distanceTransform(*im1, *im, cv::DIST_L1, 3);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[im1->channels()];
		
	cv::split( *im1, planCouleur );
	for (int i=0;i<im1->channels();i++)
		{
		cv::distanceTransform(planCouleur[i], d[i], cv::DIST_L1, 3);
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV		*> ImageInfoCV::Voronoi (ImageInfoCV *imSrc,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->channels()==1)
	{

	connectedComponentsWithStats(*imSrc, *im,*(im->statComposante[0]),*(im->centreGComposante[0]), paramOCV->intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, *(im->contours),*im->arbreContour, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
		
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],*(im->statComposante[i]),*(im->centreGComposante[i]), paramOCV->intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::HoughLigne(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (imSrc!=this)
	return r;
if (ligne==NULL)
	ligne = new std::vector<cv::Vec2f>[imSrc->channels()];
if (imSrc->channels()==1)
	{
	cv::HoughLines(*imSrc,ligne[0],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["srn"].valeur,pOCV->doubleParam["stn"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::HoughLines( planCouleur[i],ligne[i],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["srn"].valeur,pOCV->doubleParam["stn"].valeur);

		}
	delete []d;
	}
AjoutOpAttribut(pOCV);
r.push_back(this);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::HoughCercle(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (imSrc!=this)
	return r;
if (cercle==NULL)
	cercle = new std::vector<cv::Vec3f>[imSrc->channels()];
if (imSrc->channels()==1)
	{
/*   HoughCircles(*imSrc, cercle[0], cv::HOUGH_GRADIENT, 1, 10,
                 100, 30, 1, 30);
*/
	cv::HoughCircles(*imSrc,cercle[0],pOCV->intParam["method"].valeur,pOCV->doubleParam["dp"].valeur,
		pOCV->doubleParam["minDistance"].valeur,pOCV->doubleParam["param1"].valeur,pOCV->doubleParam["param2"].valeur,
		pOCV->intParam["min_radius"].valeur,pOCV->intParam["max_radius"].valeur);

	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::HoughCircles(d[i],cercle[i],pOCV->doubleParam["method"].valeur,pOCV->doubleParam["dp"].valeur,
			pOCV->intParam["minDistance"].valeur,pOCV->doubleParam["param1"].valeur,pOCV->doubleParam["param2"].valeur,
			pOCV->doubleParam["min_radius"].valeur,pOCV->doubleParam["max_radius"].valeur);
		}
	delete []d;
	}
AjoutOpAttribut(pOCV);
r.push_back(this);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::HoughLigneProba(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (imSrc != this)
	return r;
if (ligneP==NULL)
	ligneP = new std::vector<cv::Vec4i>[imSrc->channels()];
if (imSrc->channels()==1)
	{
	cv::HoughLinesP(*imSrc,ligneP[0],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["minLineLength"].valeur,pOCV->doubleParam["maxLineGap"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::HoughLinesP( planCouleur[i],ligneP[i],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["minLineLength"].valeur,pOCV->doubleParam["maxLineGap"].valeur);

		}
	delete []d;
	}
AjoutOpAttribut(pOCV);

r.push_back(this);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::BonAttributs(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;


if (imSrc!=this)
	return r;
if (boncoin==NULL)
	boncoin = new std::vector<cv::Point2f>[imSrc->channels()];
if (imSrc->channels()==1)
	{
	cv::goodFeaturesToTrack(*imSrc,boncoin[0],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
		pOCV->doubleParam["minDistance"].valeur,cv::noArray(),pOCV->intParam["blockSize"].valeur,
		pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::goodFeaturesToTrack(planCouleur[i],boncoin[i],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
			pOCV->doubleParam["minDistance"].valeur,Mat(),pOCV->intParam["blockSize"].valeur,
			pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);

		}
	}
AjoutOpAttribut(pOCV);
r.push_back(this);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::DetectCoinHarris(ImageInfoCV	*imSrc,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (imSrc->channels()==1)
	{
	cv::cornerHarris(*imSrc, *im, pOCV->intParam["blockSize"].valeur,pOCV-> intParam["ksize"].valeur,pOCV->doubleParam["k"].valeur, 
	pOCV->intParam["borderType"].valeur);

	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::cornerHarris(planCouleur[i], d[i], pOCV->intParam["blockSize"].valeur,pOCV-> intParam["ksize"].valeur,pOCV->doubleParam["k"].valeur, 
		pOCV->intParam["borderType"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueLucasKanadePyramide(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV)
{

std::vector<ImageInfoCV	*> r;

if (imPrec==NULL || imSuiv==NULL)
	return r;
if (channels()!=imSuiv->channels())
	{
	pOCV->opErreur=3;
	pOCV->msgErreur="depth images are different";
	return r;
	}
if (boncoin==NULL )
	{
	pOCV->opErreur=1;
	pOCV->msgErreur="No feature to track";
	return r;
	}
if (imSuiv->BonCoin(true)==NULL)
	{
	pOCV->opErreur=2;
	pOCV->msgErreur="Unitialized op2 BonCoin";
	return r;
	}
if (imSuiv->CoinRef(true)==NULL)
	{
	pOCV->opErreur=3;
	pOCV->msgErreur="Unitialized op2 CoinRef";
	return r;
	}
	
cv::TermCriteria critere(pOCV->intParam["typeCriteria"].valeur,pOCV->intParam["maxCountCriteria"].valeur,pOCV->doubleParam["epsilonCriteria"].valeur);

std::vector<uchar> status;
std::vector<float> err;
for (int i=0;i<imPrec->channels();i++)
	{
	calcOpticalFlowPyrLK(*imPrec,*imSuiv,imPrec->BonCoin()[i],imSuiv->BonCoin()[i],status,err,pOCV->sizeParam["winSize"].valeur,
		pOCV->intParam["maxLevel"].valeur,critere,pOCV->intParam["flag"].valeur,pOCV->doubleParam["minEigThreshold"].valeur);
	int k,l;
	imSuiv->CoinRef()[i].resize(imSuiv->BonCoin()[i].size());
	for (k=l=0;k<imSuiv->BonCoin()[i].size();k++)
		{
		if (status[k] || imSuiv==imPrec)
			{
			imSuiv->CoinRef()[i][l]=boncoin[i][k];
			imSuiv->BonCoin()[i][l++]=imSuiv->BonCoin()[i][k];
			}
		}
	imSuiv->BonCoin()[i].resize(l);
	for (k=l=0;k<imSuiv->BonCoin()[i].size();k++)
		{
		if (!status[k])
			{
			imSuiv->CoinRef()[i][l++]=boncoin[i][k];
			}
		}
	}
//imSuiv->CloneStat(this);
AjoutOpAttribut(pOCV);
pOCV->imgParam[pOCV->nomOperation + "prec"] = imSuiv;

r.push_back(imSuiv);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueFarnerback(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (imPrec==NULL )
	{
	r.push_back(imSuiv);
	return r;
	}
if (imPrec->channels()!=imSuiv->channels())
	return r;
if (imSuiv->FlotOptique(true)==NULL)
	return r;
if (imPrec->channels()==1)
	{
	calcOpticalFlowFarneback(*imPrec, *imSuiv, imSuiv->FlotOptique()[0], pOCV->doubleParam["pyr_scale"].valeur, pOCV->intParam["levels"].valeur, pOCV->sizeParam["winSize"].valeur.width, 
		pOCV->intParam["iterations"].valeur, pOCV->doubleParam["poly_sigma"].valeur, pOCV->intParam["flag"].valeur, pOCV->intParam["flag"].valeur);
	}
else
	{
	std::vector<Mat> d1,d2;
	cv::split( *imPrec, d1 );
	cv::split( *imSuiv, d2 );
	for (int i=0;i<imPrec->channels();i++)
		{
		calcOpticalFlowFarneback(d1[i], d2[i], imSuiv->FlotOptique()[i], pOCV->doubleParam["pyr_scale"].valeur, pOCV->intParam["levels"].valeur, pOCV->sizeParam["winSize"].valeur.width,
			pOCV->intParam["iterations"].valeur, pOCV->doubleParam["poly_sigma"].valeur, pOCV->intParam["flag"].valeur, pOCV->intParam["flag"].valeur);
		}
	}
AjoutOpAttribut(pOCV);


r.push_back(imSuiv);
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::PhaseCorrelate(ImageInfoCV	*imPrec, ImageInfoCV	*imSuiv, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (imPrec == NULL)
	{
	r.push_back(imSuiv);
	return r;
	}
if (imPrec->channels() != imSuiv->channels())
	return r;
if (!imSuiv->Ponderation())
	{
	imSuiv->Ponderation(true);
	}
Mat im1,im2;
imPrec->convertTo(im1, CV_64F);
imSuiv->convertTo(im2, CV_64F);
pOCV->doubleParam["response"]=0;
cv::Point2d shift = cv::phaseCorrelate(im1, im2, *imSuiv->Ponderation());
double rayon = std::sqrt(shift.x*shift.x + shift.y*shift.y);
pOCV->doubleParam["dx"].valeur=shift.x;
pOCV->doubleParam["dx"].mini=shift.x;
pOCV->doubleParam["dx"].maxi=shift.x;
pOCV->doubleParam["dx"].res=false;
pOCV->doubleParam["dy"].valeur=shift.y;
pOCV->doubleParam["dy"].mini=shift.y;
pOCV->doubleParam["dy"].maxi=shift.y;
pOCV->doubleParam["dy"].res=false;
AjoutOpAttribut(pOCV);
r.push_back(imSuiv); 
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::MAJHistoriqueMvt(ImageInfoCV	*imPrec, ImageInfoCV	*imSuiv, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (imPrec == NULL)
	{
	if (pOCV->imgParam.find(pOCV->nomOperation + "prec") == pOCV->imgParam.end())
		
		{
		throw std::string("First image is NULL in MAJHistoriqueMvt");
		return r;
		}
	imPrec = pOCV->imgParam[pOCV->nomOperation + "prec"];
	}
if (imSuiv == NULL)
	{

	throw std::string("Second image is NULL in MAJHistoriqueMvt");
	return r;
	}
if (imPrec->channels() != imSuiv->channels() || imPrec->size() != imSuiv->size())
{
	throw std::string("Channel and size must be equal");
	return r;
}
if (imPrec->channels() != 1)
{
	throw std::string("Number of channel must be 1");
	return r;
}
ImageInfoCV *mhi=NULL;
if (pOCV->imgParam.find(pOCV->nomOperation + "mhi") == pOCV->imgParam.end())
	{
	mhi=new ImageInfoCV(imPrec->rows,imPrec->cols,CV_32FC1);
	pOCV->imgParam[pOCV->nomOperation + "mhi"] = mhi;
	}
else
	mhi = pOCV->imgParam[pOCV->nomOperation + "mhi"];
Mat *silh1=mhi->Silh(true);
absdiff(*imSuiv, *imPrec, *silh1); 
threshold(*silh1, *silh1, pOCV->doubleParam["thresh"].valeur, pOCV->doubleParam["maxval"].valeur, pOCV->intParam["threshold_type"].valeur);
pOCV->doubleParam["timestamp"].valeur = (double)clock() / CLOCKS_PER_SEC;
cv::motempl::updateMotionHistory(*silh1, *mhi, pOCV->doubleParam["timestamp"].valeur, pOCV->doubleParam["duration"].valeur); // update MHI
pOCV->imgParam[pOCV->nomOperation+"prec"] = imSuiv;
mhi->ParamOCVUpdateMotionHistory(pOCV);

r.push_back(mhi);
return r;

}





std::vector<ImageInfoCV	*>ImageInfoCV::CalcOrientationMvt(ImageInfoCV	*mhi, ParametreOperation *pOCV)
{
if (mhi->depth()!=CV_32FC1)
	throw std::string("CalcOrientationMvt :image must be single channel floating-point");
if (masque)
	delete masque;
if (orient)
	delete orient;
masque = new Mat();
orient = new Mat();	// calculate motion gradient orientation and valid orientation mask
cv::motempl::calcMotionGradient(*mhi, *masque, *orient, pOCV->doubleParam["delta1"].valeur, pOCV->doubleParam["delta2"].valeur, pOCV->intParam["aperture_size"].valeur);
mhi->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(this);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::SegmenteMvt(ImageInfoCV	*mhi, ParametreOperation *pOCV)
{
if (mhi->depth() != CV_32FC1)
	throw std::string("CalcOrientationMvt :image must be single channel floating-point");
if (!masque)
	throw std::string("SegmenteMvt : masque is NULL");
if (!orient) 
	throw std::string("SegmenteMvt : orient is NULL");
regionsMvt.clear();
if (segmvt==NULL)
	segmvt  = new cv::Mat();
cv::motempl::segmentMotion(*mhi, *segmvt, regionsMvt, pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur, pOCV->doubleParam["segThresh"].valeur);
if (pOCV->intParam["calcGlobalOrientation"].valeur==1)
	{
	Mat mask;
	mhi->convertTo(mask, CV_8U, 255. / pOCVUpdateMotionHistory->doubleParam["duration"].valeur, (pOCVUpdateMotionHistory->doubleParam["duration"].valeur - pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur)*255. / pOCVUpdateMotionHistory->doubleParam["duration"].valeur);
	angle.resize(regionsMvt.size());
	for (int i = 0; i < (int)regionsMvt.size(); i++) 
		{
		Mat silh_roi = (*silh)(regionsMvt[i]);
		Mat mhi_roi = (*mhi)(regionsMvt[i]);
		Mat orient_roi = (*orient)(regionsMvt[i]);
		Mat mask_roi = mask(regionsMvt[i]);
		angle[i] = cv::motempl::calcGlobalOrientation(orient_roi, mask_roi, mhi_roi, pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur, pOCVUpdateMotionHistory->doubleParam["duration"].valeur);

		}
	}
mhi->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(mhi);
return r;
}


std::vector<ImageInfoCV		*>ImageInfoCV::TransAffine(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = new ImageInfoCV();
Mat			inter;
cv::Point2f srcTri[3];
cv::Point2f dstTri[3];

Mat rotation(2, 3, CV_32FC1);
Mat affinite(2, 3, CV_32FC1);

srcTri[0] = pOCV->pointParam["src1"].valeur;
srcTri[1] = pOCV->pointParam["src2"].valeur;
srcTri[2] = pOCV->pointParam["src3"].valeur;

dstTri[0] = pOCV->pointParam["dst1"].valeur;
dstTri[1] = pOCV->pointParam["dst2"].valeur;
dstTri[2] = pOCV->pointParam["dst3"].valeur;

affinite = getAffineTransform(srcTri, dstTri);
cv::warpAffine(*imSrc, inter, affinite, pOCV->sizeParam["dsize"].valeur, pOCV->intParam["flags"].valeur, pOCV->intParam["borderMode"].valeur, pOCV->doubleParam["borderValue"].valeur);

/** Rotating the image after Warp */

/// Compute a rotation matrix with respect to the center of the image

/// Get the rotation matrix with the specifications above
rotation = getRotationMatrix2D(pOCV->pointParam["centre"].valeur, pOCV->doubleParam["angle"].valeur, pOCV->doubleParam["scale"].valeur);
warpAffine(inter, *imDst, rotation, pOCV->sizeParam["dsize"].valeur, pOCV->intParam["flags"].valeur, pOCV->intParam["borderMode"].valeur, pOCV->doubleParam["borderValue"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(imDst);
return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::TransPerspective(ImageInfoCV	*mhi, ParametreOperation *pOCV)
{
if (mhi->depth() != CV_32FC1)
	throw std::string("CalcOrientationMvt :image must be single channel floating-point");
if (masque)
	delete masque;
if (orient)
	delete orient;
masque = new Mat();
orient = new Mat();	// calculate motion gradient orientation and valid orientation mask
cv::motempl::calcMotionGradient(*mhi, *masque, *orient, pOCV->doubleParam["delta1"].valeur, pOCV->doubleParam["delta2"].valeur, pOCV->intParam["aperture_size"].valeur);
mhi->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(this);
return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::Dimension(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::resize(*imSrc, *imDst, pOCV->sizeParam["dsize"].valeur, pOCV->doubleParam["fx"].valeur, pOCV->doubleParam["fy"].valeur, pOCV->intParam["interpolationFlags"].valeur);

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::CorrigeAberation(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::Mat matriceCamera(3,3,CV_64FC1);
	std::vector<double> coeffDistor;

	matriceCamera=0;
	matriceCamera.at<double>(0,0)=pOCV->doubleParam["fx"].valeur;
	matriceCamera.at<double>(1, 1) = pOCV->doubleParam["fy"].valeur;
	matriceCamera.at<double>(0, 2) = pOCV->doubleParam["cx"].valeur;
	matriceCamera.at<double>(1, 2) = pOCV->doubleParam["cy"].valeur;
	matriceCamera.at<double>(2, 2) = 1;
	coeffDistor.push_back(pOCV->doubleParam["k1"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["k2"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["p1"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["p2"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["k3"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["k4"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["k5"].valeur);
	coeffDistor.push_back(pOCV->doubleParam["k6"].valeur);

	cv::undistort(*imSrc, *imDst, matriceCamera, coeffDistor, cv::noArray());

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::LogPolar(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::logPolar(*imSrc, *imDst, pOCV->pointParam["center"].valeur, pOCV->doubleParam["M"].valeur ,pOCV->intParam["interpolationFlags"].valeur);

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::Fond_MOG(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = NULL;

if (pOCV->imgParam.find(pOCV->nomOperation + "fgmask") == pOCV->imgParam.end())
	{
	imDst = new ImageInfoCV();
	pOCV->imgParam[pOCV->nomOperation + "fgmask"] = imDst;
	}
else
	imDst = pOCV->imgParam[pOCV->nomOperation + "fgmask"];
if (pOCV->ecartFond.size() == 0)
	{
	cv::Ptr<cv::BackgroundSubtractor> b;
	b = cv::bgsegm::createBackgroundSubtractorMOG(); 
	pOCV->ecartFond["MOG"]=b;
	}
if (pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->getHistory()!=pOCV->intParam["history"].valeur)
	pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->setHistory(pOCV->intParam["history"].valeur);
if (pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->getBackgroundRatio()!=pOCV->doubleParam["BackgroundRatio"].valeur)
	pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->setBackgroundRatio(pOCV->doubleParam["BackgroundRatio"].valeur);
if (pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->getNMixtures()!=pOCV->intParam["mixtures"].valeur)
	pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->setNMixtures(pOCV->intParam["mixtures"].valeur);
if (pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->getNoiseSigma()!=pOCV->doubleParam["NoiseSigma"].valeur)
	pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->setNoiseSigma(pOCV->doubleParam["NoiseSigma"].valeur);
pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->apply(*imSrc, *imDst, pOCV->doubleParam["learningRate"].valeur);
std::vector<ImageInfoCV	*> r;
switch (pOCV->intParam["ResultImage"].valeur){
case 0:
	r.push_back(imDst);
	break;
case 1:
	{
	ImageInfoCV *arrierePlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "arrierePlan") == pOCV->imgParam.end())
		{
		arrierePlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "arrierePlan"] = arrierePlan;
		}
	else
		arrierePlan = pOCV->imgParam[pOCV->nomOperation + "arrierePlan"];
	pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->getBackgroundImage(*arrierePlan);
	r.push_back(arrierePlan);
	}
	break;
case 2:
	{
	ImageInfoCV *premierPlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "premierPlan") == pOCV->imgParam.end())
		{
		premierPlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "premierPlan"] = premierPlan;
		}
	else
		premierPlan = pOCV->imgParam[pOCV->nomOperation + "premierPlan"];
	imSrc->copyTo(*premierPlan,*imDst);
	r.push_back(premierPlan);
	}
	break;
	}
	
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_MOG2(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = NULL;

if (pOCV->imgParam.find(pOCV->nomOperation + "fgmask") == pOCV->imgParam.end())
	{
	imDst = new ImageInfoCV();
	pOCV->imgParam[pOCV->nomOperation + "fgmask"] = imDst;
	}
else
	imDst = pOCV->imgParam[pOCV->nomOperation + "fgmask"];
if (pOCV->ecartFond.size() == 0)
	{
	cv::Ptr<cv::BackgroundSubtractor> b;
	b = cv::createBackgroundSubtractorMOG2(); 
	pOCV->ecartFond["MOG2"]=b;
	}

if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getHistory() != pOCV->intParam["History"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setHistory(pOCV->intParam["History"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getNMixtures()!=pOCV->intParam["mixtures"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setNMixtures(pOCV->intParam["mixtures"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getBackgroundRatio()!=pOCV->doubleParam["BackgroundRatio"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setBackgroundRatio(pOCV->doubleParam["BackgroundRatio"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getVarThreshold()!=pOCV->doubleParam["VarThreshold"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarThreshold(pOCV->doubleParam["VarThreshold"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getVarThresholdGen()!=pOCV->doubleParam["VarThresholdGen"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarThresholdGen(pOCV->doubleParam["VarThresholdGen"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getVarInit()!=pOCV->doubleParam["VarInit"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarInit(pOCV->doubleParam["VarInit"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getVarMin()!=pOCV->doubleParam["VarMin"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarMin(pOCV->doubleParam["VarMin"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getVarMax()!=pOCV->doubleParam["VarMax"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarMax(pOCV->doubleParam["VarMax"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getComplexityReductionThreshold()!=pOCV->doubleParam["ComplexityReductionThreshold"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setComplexityReductionThreshold(pOCV->doubleParam["ComplexityReductionThreshold"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getDetectShadows()!=pOCV->intParam["DetectShadows"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setDetectShadows(pOCV->intParam["DetectShadows"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getShadowValue()!=pOCV->intParam["ShadowValue"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setShadowValue(pOCV->intParam["ShadowValue"].valeur);
if (pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getShadowThreshold()!=pOCV->doubleParam["ShadowThreshold"].valeur)
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->setVarThreshold(pOCV->doubleParam["ShadowThreshold"].valeur);
pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->apply(*imSrc, *imDst, pOCV->doubleParam["learningRate"].valeur);
std::vector<ImageInfoCV	*> r;
switch (pOCV->intParam["ResultImage"].valeur){
case 0:
	r.push_back(imDst);
	break;
case 1:
	{
	ImageInfoCV *arrierePlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "arrierePlan") == pOCV->imgParam.end())
		{
		arrierePlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "arrierePlan"] = arrierePlan;
		}
	else
		arrierePlan = pOCV->imgParam[pOCV->nomOperation + "arrierePlan"];
	pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->getBackgroundImage(*arrierePlan);
	r.push_back(arrierePlan);
	}
	break;
case 2:
	{
	ImageInfoCV *premierPlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "premierPlan") == pOCV->imgParam.end())
		{
		premierPlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "premierPlan"] = premierPlan;
		}
	else
		{
		premierPlan = pOCV->imgParam[pOCV->nomOperation + "premierPlan"];
		premierPlan->setTo(cv::Scalar(0,0,0));
		}
	imSrc->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_KNN(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = NULL;

if (pOCV->imgParam.find(pOCV->nomOperation + "fgmask") == pOCV->imgParam.end())
	{
	imDst = new ImageInfoCV();
	pOCV->imgParam[pOCV->nomOperation + "fgmask"] = imDst;
	}
else
	imDst = pOCV->imgParam[pOCV->nomOperation + "fgmask"];
if (pOCV->ecartFond.size() == 0)
	{
	cv::Ptr<cv::BackgroundSubtractor> b;
	b = cv::createBackgroundSubtractorKNN();
	pOCV->ecartFond["KNN"] = b;
	}

if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getHistory() != pOCV->intParam["History"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setHistory(pOCV->intParam["History"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getNSamples() != pOCV->intParam["Nsamples"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setNSamples(pOCV->intParam["Nsamples"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getkNNSamples() != pOCV->intParam["KNNSamples"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setkNNSamples(pOCV->intParam["KNNSamples"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getDist2Threshold() != pOCV->doubleParam["Dist2Threshold"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setDist2Threshold(pOCV->doubleParam["Dist2Threshold"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getDetectShadows() != pOCV->intParam["DetectShadows"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setDetectShadows(pOCV->intParam["DetectShadows"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getShadowValue() != pOCV->intParam["ShadowValue"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setShadowValue(pOCV->intParam["ShadowValue"].valeur);
if (pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getShadowThreshold() != pOCV->doubleParam["ShadowThreshold"].valeur)
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->setShadowThreshold(pOCV->doubleParam["ShadowThreshold"].valeur);
pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->apply(*imSrc, *imDst, pOCV->doubleParam["learningRate"].valeur);
std::vector<ImageInfoCV	*> r;
switch (pOCV->intParam["ResultImage"].valeur){
case 0:
	r.push_back(imDst);
	break;
case 1:
	{
	ImageInfoCV *arrierePlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "arrierePlan") == pOCV->imgParam.end())
		{
		arrierePlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "arrierePlan"] = arrierePlan;
		}
	else
		arrierePlan = pOCV->imgParam[pOCV->nomOperation + "arrierePlan"];
	pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->getBackgroundImage(*arrierePlan);
	r.push_back(arrierePlan);
	}
break;
case 2:
	{
	ImageInfoCV *premierPlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "premierPlan") == pOCV->imgParam.end())
		{
		premierPlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "premierPlan"] = premierPlan;
		}
	else
		{
		premierPlan = pOCV->imgParam[pOCV->nomOperation + "premierPlan"];
		premierPlan->setTo(cv::Scalar(0, 0, 0));
		}
	imSrc->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_GMG(ImageInfoCV	*imSrc, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = NULL;

if (pOCV->imgParam.find(pOCV->nomOperation + "fgmask") == pOCV->imgParam.end())
	{
	imDst = new ImageInfoCV();
	pOCV->imgParam[pOCV->nomOperation + "fgmask"] = imDst;
	}
else
	imDst = pOCV->imgParam[pOCV->nomOperation + "fgmask"];
if (pOCV->ecartFond.size() == 0)
	{
	cv::Ptr<cv::BackgroundSubtractor> b;
	b = cv::bgsegm::createBackgroundSubtractorGMG(20,0.7);
	pOCV->ecartFond["GMG"] = b;
	}

if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getMaxFeatures() != pOCV->intParam["Maxfeatures"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setMaxFeatures(pOCV->intParam["Maxfeatures"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getDefaultLearningRate() != pOCV->doubleParam["learningRate"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setDefaultLearningRate(pOCV->doubleParam["learningRate"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getNumFrames() != pOCV->intParam["NumFrames"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setNumFrames(pOCV->intParam["NumFrames"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getQuantizationLevels() != pOCV->intParam["QuantizationLevels"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setQuantizationLevels(pOCV->intParam["QuantizationLevels"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getBackgroundPrior() != pOCV->doubleParam["BackgroundPrior"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setBackgroundPrior(pOCV->doubleParam["BackgroundPrior"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getSmoothingRadius() != pOCV->intParam["SmoothingRadius"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setSmoothingRadius(pOCV->intParam["SmoothingRadius"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getDecisionThreshold() != pOCV->doubleParam["DecisionThreshold"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setDecisionThreshold(pOCV->doubleParam["DecisionThreshold"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getUpdateBackgroundModel() != pOCV->intParam["updateBackgroundModel"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setUpdateBackgroundModel(pOCV->intParam["updateBackgroundModel"].valeur);
/*
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getMinVal() != pOCV->doubleParam["MinVal"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setMinVal(pOCV->doubleParam["MinVal"].valeur);
if (pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getMaxVal() != pOCV->doubleParam["MaxVal"].valeur)
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->setMaxVal(pOCV->doubleParam["MaxVal"].valeur);
*/
pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->apply(*imSrc, *imDst);
std::vector<ImageInfoCV	*> r;
switch (pOCV->intParam["ResultImage"].valeur){
case 0:
	r.push_back(imDst);
	break;
case 1:
	{
	ImageInfoCV *arrierePlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "arrierePlan") == pOCV->imgParam.end())
		{
		arrierePlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "arrierePlan"] = arrierePlan;
		}
	else
		arrierePlan = pOCV->imgParam[pOCV->nomOperation + "arrierePlan"];
	pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->getBackgroundImage(*arrierePlan);
	r.push_back(arrierePlan);
	}
break;
case 2:
	{
	ImageInfoCV *premierPlan = NULL;
	if (pOCV->imgParam.find(pOCV->nomOperation + "premierPlan") == pOCV->imgParam.end())
		{
		premierPlan = new ImageInfoCV();
		pOCV->imgParam[pOCV->nomOperation + "premierPlan"] = premierPlan;
		}
	else
		{
		premierPlan = pOCV->imgParam[pOCV->nomOperation + "premierPlan"];
		premierPlan->setTo(cv::Scalar(0, 0, 0));
		}
	imSrc->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


