#include "ImageInfo.h"
#include <vector>
#include "opencv2/optflow.hpp"
#include "Panoramique.h"

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
std::vector<ImageInfoCV *>ImageInfoCV::Add(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
    if (op[0]->depth() != op[1]->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
    if (pOCV->intParam["image_mask"].valeur == 1)
        cv::add(*op[0], *op[1], *im,*op[0]->MasqueOperateur(), pOCV->intParam["ddepth"].valeur);
    else
        cv::add(*op[0], *op[1], *im, cv::noArray(), pOCV->intParam["ddepth"].valeur);
}
else
    if (pOCV->intParam["image_mask"].valeur == 1)
        cv::add(*op[0], *op[1], *im, *op[0]->MasqueOperateur(), typeResultat);
    else
        cv::add(*op[0], *op[1], *im, cv::noArray(), typeResultat);
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ConvertType(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()==0)
    return r;
ImageInfoCV	*im = new ImageInfoCV;


    op[0]->convertTo( *im, pOCV->intParam["matrix_type"].valeur, pOCV->doubleParam["alpha"].valeur,pOCV->doubleParam["beta"].valeur);
r.push_back(im);
return r;
}


/**
 * @function AddPonderee
 * @brief Addition pondérée de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::AddPonderee(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;


if (op[0]->depth() != op[1]->depth())
    pOCV->intParam["ddepth"].valeur = CV_32F;
if (pOCV)
    cv::addWeighted(*op[0], pOCV->doubleParam["alpha"].valeur, *op[1], pOCV->doubleParam["beta"].valeur, pOCV->doubleParam["gamma"].valeur, *im, pOCV->intParam["ddepth"].valeur);
else
	cv::addWeighted( *op[0],1, *op[1],1,0, *im,-1);
r.push_back(im);
return r;
}

/**
 * @function Add
 * @brief Différence de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Sub(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV)
	{
	if (op[0]->depth()!=op[1]->depth())
		pOCV->intParam["ddepth"].valeur=CV_32F;
    if (pOCV->intParam["image_mask"].valeur == 1)
        cv::subtract(*op[0], *op[1], *im, *op[0]->MasqueOperateur(), pOCV->intParam["ddepth"].valeur);
    else
        cv::subtract(*op[0], *op[1], *im, UMat(), pOCV->intParam["ddepth"].valeur);
}
else
    if (pOCV->intParam["image_mask"].valeur == 1)
        cv::subtract(*op[0], *op[1], *im, *op[0]->MasqueOperateur(), pOCV->intParam["ddepth"].valeur);
    else
        cv::subtract(*op[0], *op[1], *im, UMat(), pOCV->intParam["ddepth"].valeur);
r.push_back(im);
return r;
}

/**
 * @function Prod
 * @brief Produit de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Mul(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV)
	{
    if (op[0]->depth() != op[1]->depth())
        pOCV->intParam["ddepth"].valeur = CV_32F;
    cv::multiply(*op[0], *op[1], *im, pOCV->doubleParam["scale"].valeur, pOCV->intParam["ddepth"].valeur);
	}
else
    cv::multiply(*op[0], *op[1], *im, 1, typeResultat);
r.push_back(im);
return r;
}

/**
 * @function Prod
 * @brief Division de deux images
 */
std::vector<ImageInfoCV *>ImageInfoCV::Div(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV)
	{
    if (op[0]->depth() != op[1]->depth())
        pOCV->intParam["ddepth"].valeur = CV_32F;
    cv::divide(*op[0], *op[1], *im, pOCV->doubleParam["scale"].valeur, pOCV->intParam["ddepth"].valeur);
	}
else
    cv::divide(*op[0], *op[1], *im, 1, typeResultat);
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::EtLogique(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV )
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV->intParam["image_mask"].valeur == 1)
    cv::bitwise_and(*op[0], *op[1], *im, *op[0]->MasqueOperateur());
else
    cv::bitwise_and(*op[0], *op[1], *im, UMat());
r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::OuLogique(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV )
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV->intParam["image_mask"].valeur == 1)
    cv::bitwise_or(*op[0], *op[1], *im, *op[0]->MasqueOperateur());
else
    cv::bitwise_or(*op[0], *op[1], *im, UMat());
r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::OuExcluLogique(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op.size()<2)
    return r;
ImageInfoCV	*im = new ImageInfoCV;

if (pOCV->intParam["image_mask"].valeur == 1)
    cv::bitwise_xor(*op[0], *op[1], *im, *op[0]->MasqueOperateur());
else
    cv::bitwise_xor(*op[0], *op[1], *im, UMat());
r.push_back(im);
	return r;
}


/**
* @function Negation
* @brief Negation logique d'une image image
*/
std::vector<ImageInfoCV *>ImageInfoCV::Negation(std::vector< ImageInfoCV*> op,  ParametreOperation *pOCV)
{
	ImageInfoCV	*im = new ImageInfoCV;

    if (pOCV->intParam["image_mask"].valeur == 1)
        cv::bitwise_not(*op[0], *im, *op[0]->MasqueOperateur());
    else
        cv::bitwise_not(*op[0], *im, UMat());
    std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}



/**
 * @function Erosion
 * @brief Erosion d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Erosion(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
    if (op.size() == 1|| op[1]==NULL)
        cv::erode(*op[0], *im, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue);
    else
        cv::erode(*op[0], *im, *op[1],ancrage,nbIter,borderType,borderValue);
    
}
else
    if (op.size()==1|| op[1]==NULL)
        cv::erode(*op[0], *im, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur]);
    else
        cv::erode(*op[0], *im, *op[1]);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Dilatation(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
    if (op.size() == 1|| op[1]==NULL)
        cv::dilate(*op[0], *im, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue);
    else
        cv::dilate(*op[0], *im, *op[1],ancrage,nbIter,borderType,borderValue);
    
}
else

    if (op.size() == 1|| op[1]==NULL)
        cv::dilate(*op[0], *im, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur]);
    else
        cv::dilate(*op[0], *im, *op[1]);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::Ouverture(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_OPEN, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_OPEN, *op[1],ancrage,nbIter,borderType,borderValue);
	}
else
    if (op.size() == 1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_OPEN, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_OPEN, *op[1]);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}
 
std::vector<ImageInfoCV *>ImageInfoCV::Fermeture(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
	if (op.size()==1 || op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_CLOSE, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_CLOSE, *op[1],ancrage,nbIter,borderType,borderValue);
	}
else
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_CLOSE, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_CLOSE, *op[1]);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ChapeauHaut(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_TOPHAT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_TOPHAT, *op[1],ancrage,nbIter,borderType,borderValue);
	}
else
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_TOPHAT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_TOPHAT, *op[1]);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ChapeauBas(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_BLACKHAT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_BLACKHAT, *op[1],ancrage,nbIter,borderType,borderValue);
	}
else
	if (op.size()==1|| op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_BLACKHAT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_BLACKHAT, *op[1]);
	std::vector<ImageInfoCV	*> r;
	r.push_back(im);
	return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::GradMorph(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
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
	if (op.size()==1 || op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_GRADIENT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_GRADIENT, *op[1],ancrage,nbIter,borderType,borderValue);
	}
else
	if (op.size()==1 || op[1]==NULL)
		cv::morphologyEx( *op[0], *im,cv::MORPH_GRADIENT, *op[0]->opMorph[pOCV->intParam["indOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *op[0], *im,cv::MORPH_GRADIENT, *op[1]);
std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



/**
 * @function Convolution
 * @brief Convolution d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Convolution(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (op.size()==1 || op[1]==NULL)
	cv::filter2D( *op[0], *im,typeResultat, *op[0]->opnn[pOCV->intParam["IndOpConvolution"].valeur] );
else
	cv::filter2D( *op[0], *im,typeResultat, *op[1]);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Laplacien(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Laplacian( *op[0], *im, typeResultat,pOCV->intParam["ksize"].valeur,
	pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


/**
 * @function ModuleGradientDeriche
 * @brief Module du gradient deriche
 */
std::vector<ImageInfoCV *>ImageInfoCV::ModuleGradientDeriche(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
UMat	imx ;
UMat	imy;
UMat	imAbsx;
UMat	imAbsy;

    std::vector<ImageInfoCV	*> r;
std::vector<ImageInfoCV *> r1=GradientDericheX( op, pOCV);
std::vector<ImageInfoCV *> r2=GradientDericheY( op, pOCV);
if (r1.size() == 1 && r2.size()==1)
{
    absdiff((UMat)(*r1[0]), cv::Scalar::all(0), imAbsx);
    absdiff((UMat)(*r2[0]), cv::Scalar::all(0), imAbsy);
    addWeighted(imAbsx, 0.5, imAbsy, 0.5, 0, *im);
    r.push_back(im);
}
return r;
}

/**
 * @function ScharrModule
 * @brief Module du gradient Scharr d'une image im1 
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrModule(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
UMat	imx ;
UMat	imy;
UMat	imAbsx;
UMat	imAbsy;

cv::Scharr( *op[0], imx, pOCV->intParam["ddepth"].valeur,1,0,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );
cv::Scharr( *op[0], imy, pOCV->intParam["ddepth"].valeur,0,1,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );
absdiff(imx, cv::Scalar::all(0), imAbsx);
absdiff(imy, cv::Scalar::all(0), imAbsy);
addWeighted(imAbsx, 0.5, imAbsy, 0.5, 0, *im);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function ScharrX
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrX(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *op[0], *im, pOCV->intParam["ddepth"].valeur,1,0,pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


/**
 * @function ScharrY
 * @brief Dilatation d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::ScharrY(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *op[0], *im, pOCV->intParam["ddepth"].valeur,0,1,
	pOCV->doubleParam["scale"].valeur,pOCV->doubleParam["delta"].valeur,pOCV->intParam["borderType"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Inpaint
 * @brief inpaint d'une image im1 
 */
std::vector<ImageInfoCV *>ImageInfoCV::Inpaint(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::inpaint( *op[0],masqueMat, *im, pOCV->doubleParam["radius"].valeur,pOCV->intParam["Inpaint method"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::Clahe(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = new ImageInfoCV();

cv::Ptr<cv::CLAHE> c = cv::createCLAHE();
c->setClipLimit(pOCV->doubleParam["clipLimit"].valeur);
c->setClipLimit(pOCV->doubleParam["tilesGridSize"].valeur);
if (op[0]->channels()==1)
{
    c->apply(*op[0],*imDst);
}
else
{
    cv::Mat dst;
    cv::cvtColor(*op[0],dst,pOCV->intParam["ColorSpaceCode"].valeur);
    std::vector<cv::Mat> x;
    cv::split(dst,x);
    c->apply(x[0],x[0]);
    cv::merge(x,dst);
    cv::cvtColor(dst,*imDst,cv::COLOR_Lab2BGR);
}




op[0]->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(imDst);
return r;
}



/**
 * @function Canny
 * @brief filtrage de canny d'une image im1 par l'opérateur im2 où l'opérateur par défaut
 */
std::vector<ImageInfoCV *>ImageInfoCV::Canny(std::vector<ImageInfoCV *> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
double otsu=-1;

if (op[0]->channels()==1)
	{
    if (pOCV->intParam["Otsu threshold"].valeur==1)
    {
        otsu = threshold(*op[0], *im, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        pOCV->doubleParam["threshold1"].valeur = otsu;
        pOCV->doubleParam["threshold1"].valeur = otsu/2;
    }
	cv::Canny(	*op[0], *im, pOCV->doubleParam["threshold1"].valeur,
				pOCV->doubleParam["threshold2"].valeur,pOCV->intParam["aperture_size"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
        if (pOCV->intParam["Otsu threshold"].valeur==1)
        {
            otsu = threshold(planCouleur[i], d[i], 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            pOCV->doubleParam["threshold1"].valeur = otsu;
            pOCV->doubleParam["threshold1"].valeur = otsu/2;
        }
		cv::Canny( planCouleur[i], d[i], pOCV->doubleParam["threshold1"].valeur,
					pOCV->doubleParam["threshold2"].valeur,pOCV->intParam["aperture_size"].valeur);
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

/**
 * @function Seuillage
 * @brief Seuillage d'une image imSrc 
 */
std::vector<ImageInfoCV *>ImageInfoCV::SeuillageAdaptatif(std::vector<ImageInfoCV	*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (op[0]->channels()==1)
	{
	cv::adaptiveThreshold( *op[0], *im, pOCV->doubleParam["maxValue"].valeur,pOCV->intParam["adaptiveMethod"].valeur,
		pOCV->intParam["thresholdType"].valeur,pOCV->intParam["blockSize"].valeur,pOCV->doubleParam["C"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
    	cv::adaptiveThreshold( planCouleur[i], d[i], pOCV->doubleParam["maxValue"].valeur,pOCV->intParam["adaptiveMethod"].valeur,
		pOCV->intParam["thresholdType"].valeur,pOCV->intParam["blockSize"].valeur,pOCV->doubleParam["C"].valeur);
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::Contour(std::vector<ImageInfoCV	*>op,ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    if (op[0]!=this)
    {
        return r;

    }
    contours.resize(op[0]->channels());

    UMat im=op[0]->clone();
    std::vector<std::vector<cv::Point> > f;
    if (op[0]->channels()==1)
	    {
	    cv::findContours( im,  contours[0],pOCV->intParam["mode"].valeur,pOCV->intParam["method"].valeur);	
	    }
     else
	    {
	    std::vector<UMat> planCouleur;
	    cv::split( im, planCouleur );
        contours.resize(op[0]->channels());
	    for (int i=0;i<op[0]->channels();i++)
		    {
    	    cv::findContours( planCouleur[i],  contours[i],pOCV->intParam["mode"].valeur,pOCV->intParam["method"].valeur);	
		    }
	    }

    AjoutOpAttribut(pOCV);
    r.push_back(this);
    return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::ConvexHull(std::vector< ImageInfoCV *> op, ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    ImageInfoCV *im1=new ImageInfoCV;
    if (op[0]!=this)
    {
        return r;

    }


    contoursHull.resize(channels());
    for (int i=0;i<channels();i++)
	    {
        contoursHull[i].resize(contours[i].size());
        for (int j = 0; j<contours[i].size();j++)
            convexHull(contours[i][j],contoursHull[i][j]);
	    }

    AjoutOpAttribut(pOCV);
    r.push_back(this);
    return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::ConvexityDefects(std::vector< ImageInfoCV *> op, ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    ImageInfoCV *im1=new ImageInfoCV;
    if (op[0]!=this || contours.size()==0 || contoursHull.size()==0 || contours.size()!=contoursHull.size() )
    {
        return r;

    }


    defautConvexite.resize(channels());
    for (int i=0;i<channels();i++)
	{
        defautConvexite[i].resize(contours[i].size());
        for (int j = 0; j<contours[i].size();j++)
        {
            defautConvexite[i][j].clear();
            //if (contoursHull[i][j].size()!=0)
            try
            {
                convexityDefects(contours[i][j],contoursHull[i][j],defautConvexite[i][j]);
            }
            catch(cv::Exception& e)
            {
                defautConvexite[i][j].clear();
            }
		    {
		    }
            
	    }
    }

    AjoutOpAttribut(pOCV);
    r.push_back(this);
    return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::ApproxPolyDP(std::vector< ImageInfoCV *> op, ParametreOperation *pOCV)
{
    if (contours.size()==0)
	    {

	    throw std::string("You must used connected components first");
        std::vector<ImageInfoCV	*> r;
	    return r;
	    }
    ImageInfoCV *im1=new ImageInfoCV;
    if (op[0]->channels()==1)
	    {
		contoursPoly.resize(op[0]->channels()); 
        contoursPoly[0].resize(contours[0].size());
        for( size_t k = 0; k < contoursPoly[0].size(); k++ )
            approxPolyDP(contours[0][k], contoursPoly[0][k], pOCV->doubleParam["epsilon"].valeur, pOCV->intParam["closed"].valeur);
	    }
     else
	    {
		contoursPoly.resize(op[0]->channels()); 
        for (int i = 0; i < op[0]->channels(); i++)
        {
            contoursPoly[i].resize(contours[i].size());
            for( size_t k = 0; k < contoursPoly[i].size(); k++ )
                approxPolyDP(contours[i][k], contoursPoly[i][k], pOCV->doubleParam["epsilon"].valeur, pOCV->intParam["closed"].valeur);
        }
	    }

    AjoutOpAttribut(pOCV);
    std::vector<ImageInfoCV	*> r;
    r.push_back(this);
    return r;
}




void ImageInfoCV::Threshold( cv::InputArray _src, cv::OutputArray _dst, double thresh, double maxval, int type )
{
}

std::vector<ImageInfoCV *>ImageInfoCV::Seuillage(std::vector<ImageInfoCV	*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int otsu=0;
if (pOCV->intParam["OTSU"].valeur==1)
    otsu=cv::THRESH_OTSU;
if (op[0]->depth()==CV_16S)
	{
	pOCV->doubleParam["thresh"].maxi=32767; 
	}
if (op[0]->channels()==1)
	{
	
	threshold( *op[0], *im, pOCV->doubleParam["thresh"].valeur,pOCV->doubleParam["maxval"].valeur,
		pOCV->intParam["threshold_type"].valeur|otsu);
	}
else
	{
	std::vector<UMat> planCouleur;
	std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		cv::threshold(	planCouleur[i], d[i], pOCV->doubleParam["thresh"].valeur,
						pOCV->doubleParam["maxval"].valeur,pOCV->intParam["threshold_type"].valeur|otsu);
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;

}
std::vector<ImageInfoCV *>ImageInfoCV::LissageMoyenne(std::vector<ImageInfoCV	*>op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::blur( *op[0], *im, pOCV->sizeParam["ksize"].valeur,pOCV->pointParam["anchor"].valeur,pOCV->intParam["borderType"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::LissageGaussien(std::vector<ImageInfoCV	*>op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::GaussianBlur( *op[0], *im, pOCV->sizeParam["ksize"].valeur,
	pOCV->doubleParam["sigmaX"].valeur,pOCV->doubleParam["sigmaY"].valeur,
	pOCV->intParam["borderType"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::LissageMedian(std::vector<ImageInfoCV	*>op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::medianBlur( *op[0], *im, pOCV->intParam["ksize"].valeur);

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::FFT(std::vector< ImageInfoCV*> op,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( op[0]->rows );
int n = cv::getOptimalDFTSize( op[0]->cols ); // on the border add zero values

if (op[0]->channels()==1)
	{
	if (op[0]->type()!=CV_32FC1 && im->type()!=CV_64FC1)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		op[0]->convertTo(*imSrc, CV_32F);
		if (op[0]->rows!=m ||op[0]->cols!=n)
			{
			UMat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,*im,cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::dft(*imSrc,*im,cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::dft(*op[0],*im,cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<UMat> planCouleurSrc;
	UMat *planCouleurDst=new UMat[op[0]->channels()];
	std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleurSrc );
	for (int i=0;i<op[0]->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32F);
		if (op[0]->rows!=m ||op[0]->cols!=n)
			{
			UMat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - op[0]->rows, 0, n - op[0]->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::dft(planCouleurDst[i],d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge(d, *im);
	int nb=im->channels();
	delete []planCouleurDst;

	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::IFFT(std::vector< ImageInfoCV*> op,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( op[0]->rows );
int n = cv::getOptimalDFTSize( op[0]->cols ); // on the border add zero values

if (op[0]->channels()==2)
	{
	if (op[0]->type()!=CV_32FC2 && im->type()!=CV_64FC2)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		op[0]->convertTo(*imSrc, CV_32FC2);
		if (op[0]->rows!=m ||op[0]->cols!=n)
			{
			UMat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::idft(*imSrc,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::idft(*op[0],*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<UMat> planCouleurSrc;
	UMat *planCouleurDst=new UMat[op[0]->channels()];
	std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleurSrc );
	for (int i=0;i<op[0]->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32FC2);
		if (op[0]->rows!=m ||op[0]->cols!=n)
			{
			UMat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - op[0]->rows, 0, n - op[0]->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::idft(planCouleurDst[i],d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge(d, *im);
	int nb=im->channels();
	delete []planCouleurDst;

	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



std::vector<ImageInfoCV*> ImageInfoCV::FusionPlan(std::vector<ImageInfoCV	*>op,ParametreOperation *paramOCV)
{
std::vector<ImageInfoCV	*> r;
ImageInfoCV	*result = new ImageInfoCV;
std::vector<cv::UMat> listePlan;
if (op.size()>=1)
	listePlan.push_back(*(op[0]));
else 
	return r;
if (op.size()>=2)
        listePlan.push_back(*(op[1]));
if (op.size()>=3)
        listePlan.push_back(*(op[2]));
cv::merge(listePlan,*result);

r.push_back(result);
return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::RGB_L(std::vector<ImageInfoCV	*>op,ParametreOperation *paramOCV)
{
ImageInfoCV	*result=new ImageInfoCV;
 
cv::cvtColor( *op[0], *result,paramOCV->intParam["ColorSpaceCode"].valeur );

std::vector<ImageInfoCV	*> r;
r.push_back(result);
return r;
}

std::vector<ImageInfoCV *> ImageInfoCV::SeparationPlan(std::vector<ImageInfoCV	*>op,ParametreOperation *paramOCV)
{
ImageInfoCV	**result=new ImageInfoCV*[op[0]->channels()];
std::vector<UMat> planCouleurSrc(op[0]->channels());
std::vector<ImageInfoCV	*> r;
 
cv::split( *op[0], planCouleurSrc );
for (int i=0;i<op[0]->channels();i++)
	{
	result[i]=new ImageInfoCV;
    planCouleurSrc[i].copyTo(*result[i]);
//	cv::merge(planCouleurSrc[i], *result[i]);
	r.push_back(result[i]);
	}
paramOCV->nbImageRes=op[0]->channels();

return r;

}


std::vector<ImageInfoCV *> ImageInfoCV::ComposanteConnexe(std::vector<ImageInfoCV	*>op,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (im->statComposante.size()==0)
	{
	im->statComposante.resize(op[0]->channels()); 
	im->centreGComposante.resize(op[0]->channels()); 
	im->contours.resize(op[0]->channels()); 
	im->arbreContour.resize(op[0]->channels()); 

	}
if (op[0]->channels()==1)
	{

	connectedComponentsWithStats(*op[0], *im,im->statComposante[0],im->centreGComposante[0], paramOCV->intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, im->contours[0],im->arbreContour[0], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	im->CalcMoment();

	}
else
	{
	std::vector<UMat> planCouleur;
    std::vector<UMat> d(op[0]->channels());
	//UMat *d=new UMat[op[0]->channels()];
		
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],im->statComposante[i],im->centreGComposante[i], paramOCV->intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge(d, *im);
	im->CalcMoment();

	std::vector<cv::Moments> *mmt =new std::vector<cv::Moments> [op[0]->channels()];
	for (int i=0;i<op[0]->channels();i++)
		{
		mmt[i].resize(im->contours[i].size()); 
		for (int j=0;j<im->contours[i].size();j++)
			{
			mmt[i][j] = cv::moments( im->contours[i][j], false );
			}
		}
	delete []mmt;
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::PartageEaux (std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)

{
std::vector<ImageInfoCV	*> r;
ImageInfoCV	*im =new ImageInfoCV;
if (op.size()!=2)
	return r;
op[1]->convertTo(*op[0], CV_32S);
watershed(*op[0], *im);

r.push_back(im);
return r;
}

std::vector<ImageInfoCV		*> ImageInfoCV::DistanceDiscrete (std::vector<ImageInfoCV *>op,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (op[0]->channels()==1)
	{
	cv::distanceTransform(*op[0], *im, paramOCV->intParam["distance_type"].valeur, paramOCV->intParam["maskSize"].valeur,paramOCV->intParam["labelType"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
    std::vector<UMat> d(op[0]->channels());
		
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		cv::distanceTransform(planCouleur[i], d[i], paramOCV->intParam["distance_type"].valeur, paramOCV->intParam["maskSize"].valeur,paramOCV->intParam["labelType"].valeur);
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV		*> ImageInfoCV::Voronoi (std::vector<ImageInfoCV *> op,ParametreOperation *paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (op[0]->channels()==1)
	{

	connectedComponentsWithStats(*op[0], *im,im->statComposante[0],im->centreGComposante[0], paramOCV->intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, im->contours[0],im->arbreContour[0], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	}
else
	{
	std::vector<UMat> planCouleur;
    std::vector<UMat> d(op[0]->channels());
		
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],im->statComposante[i],im->centreGComposante[i], paramOCV->intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}


std::vector<ImageInfoCV *>ImageInfoCV::HoughLigne(std::vector<ImageInfoCV *> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (op[0]!=this)
	return r;
if (ligne.size()==0)
	ligne.resize(op[0]->channels());
if (op[0]->channels()==1)
	{
	cv::HoughLines(*op[0],ligne[0],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["srn"].valeur,pOCV->doubleParam["stn"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	UMat *d=new UMat[op[0]->channels()];
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
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


std::vector<ImageInfoCV *>ImageInfoCV::HoughCercle(std::vector<ImageInfoCV *> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (op[0]!=this)
	return r;
if (cercle.size()==0)
	cercle.resize(op[0]->channels());
if (op[0]->channels()==1)
	{
/*   HoughCircles(*imSrc, cercle[0], cv::HOUGH_GRADIENT, 1, 10,
                 100, 30, 1, 30);
*/
	cv::HoughCircles(*op[0],cercle[0],pOCV->intParam["method"].valeur,pOCV->doubleParam["dp"].valeur,
		pOCV->doubleParam["minDistance"].valeur,pOCV->doubleParam["param1"].valeur,pOCV->doubleParam["param2"].valeur,
		pOCV->intParam["min_radius"].valeur,pOCV->intParam["max_radius"].valeur);

	}
else
	{
	std::vector<UMat> planCouleur;
	UMat *d=new UMat[op[0]->channels()];
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
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

std::vector<ImageInfoCV *>ImageInfoCV::HoughLigneProba(std::vector<ImageInfoCV *> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (op[0] != this)
	return r;
if (ligneP.size()==0)
	ligneP.resize(op[0]->channels());
if (op[0]->channels()==1)
	{
	cv::HoughLinesP(*op[0],ligneP[0],pOCV->doubleParam["rho"].valeur,pOCV->doubleParam["theta"].valeur,
		pOCV->intParam["threshold"].valeur,pOCV->doubleParam["minLineLength"].valeur,pOCV->doubleParam["maxLineGap"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	UMat *d=new UMat[op[0]->channels()];
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
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

std::vector<ImageInfoCV *>ImageInfoCV::BonAttributs(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (pOCV->imgParam.find("calcopticalflowpyrlkprec")!=pOCV->imgParam.end() )
{
    AjoutOpAttribut(pOCV);
    r.push_back(this);
    return r;
}

if (op[0]!=this)
	return r;
if (boncoin.size()==0)
	boncoin.resize(op[0]->channels());
if (op[0]->channels()==1)
	{
	cv::goodFeaturesToTrack(*op[0],boncoin[0],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
		pOCV->doubleParam["minDistance"].valeur,cv::noArray(),pOCV->intParam["blockSize"].valeur,
		pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		cv::goodFeaturesToTrack(planCouleur[i],boncoin[i],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
			pOCV->doubleParam["minDistance"].valeur,UMat(),pOCV->intParam["blockSize"].valeur,
			pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);

		}
	}
AjoutOpAttribut(pOCV);
r.push_back(this);
return r;

/*{

if (op[0]!=this)
	return r;
if (boncoin.size()==0)
	boncoin.resize(op[0]->channels());
if (op[0]->channels()==1)
	{
	cv::goodFeaturesToTrack(*op[0],boncoin[0],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
		pOCV->doubleParam["minDistance"].valeur,cv::noArray(),pOCV->intParam["blockSize"].valeur,
		pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);
	}
else
	{
	std::vector<UMat> planCouleur;
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		cv::goodFeaturesToTrack(planCouleur[i],boncoin[i],pOCV->intParam["maxCorners"].valeur,pOCV->doubleParam["qualityLevel"].valeur,
			pOCV->doubleParam["minDistance"].valeur,UMat(),pOCV->intParam["blockSize"].valeur,
			pOCV->intParam["useHarrisDetector"].valeur,pOCV->doubleParam["k"].valeur);

		}
	}
AjoutOpAttribut(pOCV);
r.push_back(this);
return r;
}*/
}

std::vector<ImageInfoCV *>ImageInfoCV::DetectCoinHarris(std::vector< ImageInfoCV*> op,ParametreOperation *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (op[0]->channels()==1)
	{
	cv::cornerHarris(*op[0], *im, pOCV->intParam["blockSize"].valeur,pOCV-> intParam["ksize"].valeur,pOCV->doubleParam["k"].valeur, 
	pOCV->intParam["borderType"].valeur);

	}
else
	{
	std::vector<UMat> planCouleur;
    std::vector<UMat> d(op[0]->channels());
	cv::split( *op[0], planCouleur );
	for (int i=0;i<op[0]->channels();i++)
		{
		cv::cornerHarris(planCouleur[i], d[i], pOCV->intParam["blockSize"].valeur,pOCV-> intParam["ksize"].valeur,pOCV->doubleParam["k"].valeur, 
		pOCV->intParam["borderType"].valeur);
		}
	cv::merge(d, *im);
	}

std::vector<ImageInfoCV	*> r;
r.push_back(im);
return r;
}



std::vector<ImageInfoCV	*> ImageInfoCV::DetectOrb(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] != this)
	return r;


if (pOCV->detecteur.size() == 0 || pOCV->detecteur.find("ORB") == pOCV->detecteur.end())
	{
	cv::Ptr<cv::Feature2D> b;
	b = cv::ORB::create();
	pOCV->detecteur["ORB"] = b;
	}
if (pointCle.find(IMAGEINFOCV_ORB_DES)==pointCle.end())
{
    std::vector<cv::KeyPoint> x;
    pointCle.insert(std::make_pair(IMAGEINFOCV_ORB_DES,x));
}
else
    pointCle[IMAGEINFOCV_ORB_DES].clear();
if (descripteur.find(IMAGEINFOCV_ORB_DES)==descripteur.end())
    descripteur.insert(std::make_pair(IMAGEINFOCV_ORB_DES,cv::Mat()));
else
    descripteur[IMAGEINFOCV_ORB_DES] =  cv::Mat();

if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getEdgeThreshold() != pOCV->intParam["EdgeThreshold"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setEdgeThreshold(pOCV->intParam["EdgeThreshold"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getFastThreshold() != pOCV->doubleParam["FastThreshold"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setFastThreshold(pOCV->doubleParam["FastThreshold"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getFirstLevel() != pOCV->intParam["FirstLevel"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setFirstLevel(pOCV->intParam["FirstLevel"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getMaxFeatures() != pOCV->intParam["MaxFeatures"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setMaxFeatures(pOCV->intParam["MaxFeatures"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getNLevels() != pOCV->intParam["NLevels"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setNLevels(pOCV->intParam["NLevels"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getPatchSize() != pOCV->intParam["PatchSize"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setPatchSize(pOCV->intParam["PatchSize"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getScaleFactor() != pOCV->doubleParam["ScaleFactor"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setScaleFactor(pOCV->doubleParam["ScaleFactor"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getScoreType() != pOCV->intParam["ScoreType"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setScoreType(pOCV->intParam["ScoreType"].valeur);
if (pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->getWTA_K() != pOCV->intParam["WTA_K"].valeur)
	pOCV->detecteur["ORB"].dynamicCast<cv::ORB>()->setWTA_K(pOCV->intParam["WTA_K"].valeur);

if (pOCV->intParam["image_mask"].valeur == 1)
    pOCV->detecteur["ORB"]->detectAndCompute(*op[0], *op[0]->MasqueOperateur(), *(op[0]->PointCle(IMAGEINFOCV_ORB_DES)), *(op[0]->Descripteur(IMAGEINFOCV_ORB_DES)));
else
    pOCV->detecteur["ORB"]->detectAndCompute(*op[0], UMat(), *(op[0]->PointCle(IMAGEINFOCV_ORB_DES)), *(op[0]->Descripteur(IMAGEINFOCV_ORB_DES)));

if (matches.find(IMAGEINFOCV_ORB_DES)!=matches.end())
    matches[IMAGEINFOCV_ORB_DES].clear();
    else
    {
        std::vector<cv::DMatch>  x;
        matches.insert(std::make_pair(IMAGEINFOCV_ORB_DES,x));
    }

AjoutOpAttribut(pOCV);

r.push_back(this);
return r;
}

std::vector<ImageInfoCV	*>ImageInfoCV::DetectMser(std::vector<ImageInfoCV	*>op , ParametreOperation *pOCV)
    {
    std::vector<ImageInfoCV	*> r;
    if (op[0] != this)
        return r;
    if (op[0] != this)
        return r;


        {
        cv::Ptr<cv::Feature2D> b;
        b = cv::MSER::create(pOCV->intParam["delta"].valeur, pOCV->intParam["minArea"].valeur, pOCV->intParam["maxArea"].valeur,
                             pOCV->doubleParam["maxVariation"].valeur, pOCV->doubleParam["minDiversity"].valeur, pOCV->intParam["maxEvolution"].valeur,
                             pOCV->doubleParam["areaThreshold"].valeur, pOCV->doubleParam["minMargin"].valeur, pOCV->intParam["edgeBlurSize"].valeur);
        pOCV->detecteur["MSER"] = b;
        }

    pOCV->detecteur["MSER"].dynamicCast<cv::MSER>()->setPass2Only(pOCV->intParam["pass2Only"].valeur);
    kMser.clear();
    if (pOCV->detecteur["MSER"].dynamicCast<cv::MSER>())
    {
        std::vector<cv::Rect>  zone;
        pOCV->detecteur["MSER"].dynamicCast<cv::MSER>()->detectRegions(*op[0], kMser, zone);
    }

    AjoutOpAttribut(pOCV);

    r.push_back(this);
    return r;
    }

std::vector<ImageInfoCV	*>ImageInfoCV::DetectBlob(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
    {
    std::vector<ImageInfoCV	*> r;
    if (op[0] != this)
        return r;


        {
        cv::SimpleBlobDetector::Params pDefaultBLOB;
        // This is default parameters for SimpleBlobDetector
        pDefaultBLOB.thresholdStep = pOCV->doubleParam["thresholdStep"].valeur;
        pDefaultBLOB.minThreshold = pOCV->doubleParam["minThreshold"].valeur;
        pDefaultBLOB.maxThreshold = pOCV->doubleParam["maxThreshold"].valeur;
        pDefaultBLOB.minRepeatability = pOCV->intParam["minRepeatability"].valeur;
        pDefaultBLOB.minDistBetweenBlobs = pOCV->doubleParam["minDistBetweenBlobs"].valeur;
        pDefaultBLOB.filterByColor = pOCV->intParam["filterByColor"].valeur;
        pDefaultBLOB.blobColor = pOCV->intParam["blobColor"].valeur;
        pDefaultBLOB.filterByArea = pOCV->intParam["filterByArea"].valeur;
        pDefaultBLOB.minArea = pOCV->doubleParam["minArea"].valeur;
        pDefaultBLOB.maxArea = pOCV->doubleParam["maxArea"].valeur;
        pDefaultBLOB.filterByCircularity = pOCV->intParam["filterByCircularity"].valeur;
        pDefaultBLOB.minCircularity = pOCV->doubleParam["minCircularity"].valeur;
        pDefaultBLOB.maxCircularity = pOCV->doubleParam["maxCircularity"].valeur;
        pDefaultBLOB.filterByInertia = pOCV->intParam["filterByInertia"].valeur;
        pDefaultBLOB.minInertiaRatio = pOCV->doubleParam["minInertiaRatio"].valeur;
        pDefaultBLOB.maxInertiaRatio = pOCV->doubleParam["maxInertiaRatio"].valeur;
        pDefaultBLOB.filterByConvexity = pOCV->intParam["filterByConvexity"].valeur;
        pDefaultBLOB.minConvexity = pOCV->doubleParam["minConvexity"].valeur;
        pDefaultBLOB.maxConvexity = pOCV->doubleParam["maxConvexity"].valeur;
        cv::Ptr<cv::Feature2D> b;
        b = cv::SimpleBlobDetector::create(pDefaultBLOB);
        pOCV->detecteur["BLOB"] = b;
        std::vector<cv::KeyPoint> x;
        if (pointCle.find(IMAGEINFOCV_BLOB_DES)==pointCle.end())
            pointCle.insert(std::make_pair(IMAGEINFOCV_BLOB_DES,x));
        else
            pointCle[IMAGEINFOCV_BLOB_DES].clear();
        }

    if (pOCV->intParam["image_mask"].valeur == 1)
        pOCV->detecteur["BLOB"]->detect(*op[0], pointCle[IMAGEINFOCV_BLOB_DES], *op[0]->MasqueOperateur());
    else
        pOCV->detecteur["BLOB"]->detect(*op[0], pointCle[IMAGEINFOCV_BLOB_DES], UMat());


    AjoutOpAttribut(pOCV);

    r.push_back(this);
    return r;
    }

std::vector<ImageInfoCV	*>ImageInfoCV::DetectBrisk(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
    {
    std::vector<ImageInfoCV	*> r;
    if (op[0] != this)
        return r;


    if (pOCV->detecteur.size() == 0 || pOCV->detecteur.find("BRISK") == pOCV->detecteur.end())
        {
        cv::Ptr<cv::Feature2D> b;
        b = cv::BRISK::create(pOCV->intParam["thresh"].valeur, pOCV->intParam["octaves"].valeur, pOCV->doubleParam["patternScale"].valeur);
        pOCV->detecteur["BRISK"] = b;
        std::vector<cv::KeyPoint> x;
        if (pointCle.find(IMAGEINFOCV_BRISK_DES)==pointCle.end())
            pointCle.insert(std::make_pair(IMAGEINFOCV_BRISK_DES,x));
        else
            pointCle[IMAGEINFOCV_BRISK_DES].clear();
        if (descripteur.find(IMAGEINFOCV_BRISK_DES)==descripteur.end())
            descripteur.insert(std::make_pair(IMAGEINFOCV_BRISK_DES,cv::Mat()));
        else
            descripteur[IMAGEINFOCV_BRISK_DES] =  cv::Mat();
       }


    if (pOCV->intParam["image_mask"].valeur == 1)
        pOCV->detecteur["BRISK"]->detectAndCompute(*op[0], *op[0]->MasqueOperateur(), *(op[0]->PointCle(IMAGEINFOCV_BRISK_DES)), *(op[0]->Descripteur(IMAGEINFOCV_BRISK_DES)));
    else
        pOCV->detecteur["BRISK"]->detectAndCompute(*op[0], UMat(), *(op[0]->PointCle(IMAGEINFOCV_BRISK_DES)), *(op[0]->Descripteur(IMAGEINFOCV_BRISK_DES)));

    if (matches.find(IMAGEINFOCV_BRISK_DES)!=matches.end())
        matches[IMAGEINFOCV_BRISK_DES].clear();
    else
    {
        std::vector<cv::DMatch>  x;
        matches.insert(std::make_pair(IMAGEINFOCV_BRISK_DES,x));
    }

    AjoutOpAttribut(pOCV);

    r.push_back(this);
    return r;
    }
std::vector<ImageInfoCV	*>ImageInfoCV::DetectAgast(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] != this)
    return r;


if (pOCV->detecteur.size() == 0 || pOCV->detecteur.find("AKAZE") == pOCV->detecteur.end())
    {
    cv::Ptr<cv::Feature2D> b;
 //   b = cv::AGAST();create();
    pOCV->detecteur["AGAST"] = b;
    std::vector<cv::KeyPoint> x;
    if (pointCle.find(IMAGEINFOCV_AGAST_DES)==pointCle.end())
        pointCle.insert(std::make_pair(IMAGEINFOCV_AGAST_DES,x));
    else
        pointCle[IMAGEINFOCV_AGAST_DES].clear();
    if (descripteur.find(IMAGEINFOCV_AGAST_DES)==descripteur.end())
        descripteur.insert(std::make_pair(IMAGEINFOCV_AGAST_DES,cv::Mat()));
    else
        descripteur[IMAGEINFOCV_AGAST_DES] =  cv::Mat();
    }


    if (matches.find(IMAGEINFOCV_AGAST_DES)!=matches.end())
        matches[IMAGEINFOCV_AGAST_DES].clear();
    else
    {
        std::vector<cv::DMatch>  x;
        matches.insert(std::make_pair(IMAGEINFOCV_AGAST_DES,x));
    }

AjoutOpAttribut(pOCV);

r.push_back(this);
return r;
}



std::vector<ImageInfoCV	*>ImageInfoCV::DetectKaze(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] != this)
    return r;


if (pOCV->detecteur.size() == 0 || pOCV->detecteur.find("KAZE") == pOCV->detecteur.end())
    {
    cv::Ptr<cv::Feature2D> b;
    b = cv::KAZE::create();
    pOCV->detecteur["KAZE"] = b;
        std::vector<cv::KeyPoint> x;
        if (pointCle.find(IMAGEINFOCV_KAZE_DES)==pointCle.end())
            pointCle.insert(std::make_pair(IMAGEINFOCV_KAZE_DES,x));
        else
            pointCle[IMAGEINFOCV_KAZE_DES].clear();
    if (descripteur.find(IMAGEINFOCV_KAZE_DES)==descripteur.end())
        descripteur.insert(std::make_pair(IMAGEINFOCV_KAZE_DES,cv::Mat()));
    else
        descripteur[IMAGEINFOCV_KAZE_DES] =  cv::Mat();
    }

if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getExtended() != pOCV->intParam["extended"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setExtended(pOCV->intParam["DescriptorChannels"].valeur);
if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getUpright() != pOCV->doubleParam["upright"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setUpright(pOCV->doubleParam["upright"].valeur);
if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getDiffusivity() != pOCV->intParam["Diffusivity"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setDiffusivity(pOCV->intParam["Diffusivity"].valeur);
if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getNOctaveLayers() != pOCV->intParam["NOctaveLayers"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setNOctaveLayers(pOCV->intParam["NOctaveLayers"].valeur);
if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getNOctaves() != pOCV->intParam["NOctaves"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setNOctaves(pOCV->intParam["NOctaves"].valeur);
if (pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->getThreshold() != pOCV->doubleParam["Threshold"].valeur)
    pOCV->detecteur["KAZE"].dynamicCast<cv::KAZE>()->setThreshold(pOCV->doubleParam["Threshold"].valeur);
if (pOCV->intParam["image_mask"].valeur == 1)
    pOCV->detecteur["KAZE"]->detectAndCompute(*op[0], *op[0]->MasqueOperateur(), *(op[0]->PointCle(IMAGEINFOCV_KAZE_DES)), *(op[0]->Descripteur(IMAGEINFOCV_KAZE_DES)));
else
    pOCV->detecteur["KAZE"]->detectAndCompute(*op[0], UMat(), *(op[0]->PointCle(IMAGEINFOCV_KAZE_DES)), *(op[0]->Descripteur(IMAGEINFOCV_KAZE_DES)));

    if (matches.find(IMAGEINFOCV_KAZE_DES)!=matches.end())
        matches[IMAGEINFOCV_KAZE_DES].clear();
    else
    {
        std::vector<cv::DMatch>  x;
        matches.insert(std::make_pair(IMAGEINFOCV_KAZE_DES,x));
    }

AjoutOpAttribut(pOCV);

r.push_back(this);
return r;
}


std::vector<ImageInfoCV	*>ImageInfoCV::DetectAkaze(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] != this)
    return r;


if (pOCV->detecteur.size() == 0 || pOCV->detecteur.find("AKAZE") == pOCV->detecteur.end())
    {
    cv::Ptr<cv::Feature2D> b;
    b = cv::AKAZE::create();
    pOCV->detecteur["AKAZE"] = b;
    std::vector<cv::KeyPoint> x;
    if (pointCle.find(IMAGEINFOCV_AKAZE_DES)==pointCle.end())
        pointCle.insert(std::make_pair(IMAGEINFOCV_AKAZE_DES,x));
    else
        pointCle[IMAGEINFOCV_AKAZE_DES].clear();
    if (descripteur.find(IMAGEINFOCV_AKAZE_DES)==descripteur.end())
        descripteur.insert(std::make_pair(IMAGEINFOCV_AKAZE_DES,cv::Mat()));
    else
        descripteur[IMAGEINFOCV_AKAZE_DES] =  cv::Mat();
   }

if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getDescriptorChannels() != pOCV->intParam["DescriptorChannels"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setDescriptorChannels(pOCV->intParam["DescriptorChannels"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getDescriptorSize() != pOCV->doubleParam["DescriptorSize"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setDescriptorSize(pOCV->doubleParam["DescriptorSize"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getDescriptorType() != pOCV->intParam["DescriptorType"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setDescriptorType(pOCV->intParam["DescriptorType"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getDiffusivity() != pOCV->intParam["Diffusivity"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setDiffusivity(pOCV->intParam["Diffusivity"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getNOctaveLayers() != pOCV->intParam["NOctaveLayers"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setNOctaveLayers(pOCV->intParam["NOctaveLayers"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getNOctaves() != pOCV->intParam["NOctaves"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setNOctaves(pOCV->intParam["NOctaves"].valeur);
if (pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->getThreshold() != pOCV->doubleParam["Threshold"].valeur)
    pOCV->detecteur["AKAZE"].dynamicCast<cv::AKAZE>()->setThreshold(pOCV->doubleParam["Threshold"].valeur);
if (pOCV->intParam["image_mask"].valeur == 1)
    pOCV->detecteur["AKAZE"]->detectAndCompute(*op[0], *op[0]->MasqueOperateur(), *(op[0]->PointCle(IMAGEINFOCV_AKAZE_DES)), *(op[0]->Descripteur(IMAGEINFOCV_AKAZE_DES)));
else
    pOCV->detecteur["AKAZE"]->detectAndCompute(*op[0], cv::noArray(), *(op[0]->PointCle(IMAGEINFOCV_AKAZE_DES)), *(op[0]->Descripteur(IMAGEINFOCV_AKAZE_DES)));
    if (matches.find(IMAGEINFOCV_AKAZE_DES)!=matches.end())
        matches[IMAGEINFOCV_AKAZE_DES].clear();
    else
    {
        std::vector<cv::DMatch>  x;
        matches.insert(std::make_pair(IMAGEINFOCV_AKAZE_DES,x));
    }

AjoutOpAttribut(pOCV);

r.push_back(this);
return r;
}

std::vector<ImageInfoCV	*>ImageInfoCV::AppariePoint(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{

cv::Ptr<cv::DescriptorMatcher> descriptorMatcher; // brute force

std::map<int, cv::Mat >::iterator it=descripteur.begin();

for (; it != descripteur.end();it++)
{
    if (pOCV->intParam["matcher"].valeur==0) // Brute force matcher
    {
        if (pOCV->intParam["normType"].valeur == -1)
            switch (it->first){
            case IMAGEINFOCV_ORB_DES:
                if (listeOpAttribut.find("orbfeatures2d") != listeOpAttribut.end())
                {
                    if (listeOpAttribut["orbfeatures2d"].intParam["WTA_K"].valeur==3 || listeOpAttribut["orbfeatures2d"].intParam["WTA_K"].valeur==4)
                        descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING2,pOCV->intParam["crossCheck"].valeur).clone();
                    else
                        descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                }
                else
                        descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                break;
            case IMAGEINFOCV_AKAZE_DES:
                if (listeOpAttribut.find("akazefeatures2d") != listeOpAttribut.end())
                {
                    descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                }
                else
                    descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                break;
            case IMAGEINFOCV_AGAST_DES:
                break;
            case IMAGEINFOCV_BLOB_DES:
                break;
            case IMAGEINFOCV_BRISK_DES:
                descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                break;
            case IMAGEINFOCV_KAZE_DES:
                descriptorMatcher = cv::BFMatcher(cv::NORM_HAMMING,pOCV->intParam["crossCheck"].valeur).clone();
                break;
            }
        else 
            descriptorMatcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
    }
    else // FLANN matcher
    {

    }

    descriptorMatcher->match(*op[0]->Descripteur(it->first), *op[1]->Descripteur(it->first), matches[it->first], UMat());
    pointCleApp.insert(make_pair(it->first,*(op[1]->PointCle(it->first))));
}

AjoutOpAttribut(pOCV);
pOCV->imgParam[pOCV->nomOperation + "prec"] = op[1];
std::vector<ImageInfoCV	*> r;
r.push_back(op[0]);
return r;
}

std::vector<ImageInfoCV	*>ImageInfoCV::FindHomography(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
    if (pointCleApp.size()==0 || op[0]!=this || ListePointCleApp()==NULL)
    {
        std::vector<ImageInfoCV	*> r;
        return r;
    }
    std::vector<cv::KeyPoint> *pts1 = PointCle(-1);


    std::map<int, std::vector<cv::KeyPoint> >*lApp=ListePointCleApp();
    std::map<int, std::vector<cv::KeyPoint> >::iterator it=ListePointCleApp()->begin();
    std::vector<cv::Point2f> src,dst;
    for (;it!=ListePointCleApp()->end();it++)
    {
        double max_dist = 0; double min_dist = 100;
        std::vector<cv::DMatch> *m = Appariement(it->first);
          //-- Quick calculation of max and min distances between keypoints
        if (m!=NULL && PointCle(it->first) != NULL && PointCleApp(it->first) != NULL)
        {
            for( int i = 0; i < m->size(); i++ )
            { 
                double dist = (*m)[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }
            pts1 = PointCle(it->first);
            std::vector<cv::KeyPoint> *pts2 = PointCleApp(it->first);
            if (pts2->size()==0)
	            break;
            int fZoomNume, fZoomDeno;
            //wxPoint pos = ClientToScreen(pt);
            for (int i = 0; i < m->size(); i++)
                if ((*m)[i].distance<std::max(2*min_dist,(min_dist+max_dist)/3))
                    {
	                src.push_back((*pts1)[(*m)[i].queryIdx].pt);
	                dst.push_back((*pts2)[(*m)[i].trainIdx].pt);
	                }
        }


    }
    homography =cv::findHomography(src,dst,pOCV->intParam["method"].valeur,pOCV->doubleParam["ransacReprojThreshold"].valeur);


    AjoutOpAttribut(pOCV);
    std::vector<ImageInfoCV	*> r;
    r.push_back(op[0]);
return r;
}


//std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueLucasKanadePyramide(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV)
std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueLucasKanadePyramide(std::vector<ImageInfoCV	*> op,ParametreOperation *pOCV)
{

std::vector<ImageInfoCV	*> r;

if (op.size()<2 || op[0]==NULL || op[1]==NULL)
	return r;
if (op[1]==op[0])
{
    AjoutOpAttribut(pOCV);
    pOCV->imgParam[pOCV->nomOperation + "prec"] = op[1];

    r.push_back(op[1]);
    return r;
}

if (channels()!=op[1]->channels())
	{
	pOCV->opErreur=3;
	pOCV->msgErreur="depth images are different";
	return r;
	}
if (boncoin.size()==0 )
	{
	pOCV->opErreur=1;
	pOCV->msgErreur="No feature to track";
	return r;
	}
if (op[1]->BonCoin(true)==NULL)
	{
	pOCV->opErreur=2;
	pOCV->msgErreur="Unitialized op2 BonCoin";
	return r;
	}
if (op[1]->CoinRef(true)==NULL)
	{
	pOCV->opErreur=3;
	pOCV->msgErreur="Unitialized op2 CoinRef";
	return r;
	}
	
cv::TermCriteria critere(pOCV->intParam["typeCriteria"].valeur,pOCV->intParam["maxCountCriteria"].valeur,pOCV->doubleParam["epsilonCriteria"].valeur);
cv::TermCriteria critere2(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

std::vector<uchar> status;
std::vector<float> err;
for (int i=0;i<op[0]->channels();i++)
	{
    std::vector<cv::Point2f> p1=(*op[0]->BonCoin())[i];
//	calcOpticalFlowPyrLK(*op[0],*op[1],p1,(*op[1]->BonCoin())[i],status,err,cv::Size(10,10),
	//	3,critere2,0,0.0001);
	calcOpticalFlowPyrLK(*op[0],*op[1],p1,(*op[1]->BonCoin())[i],status,err,pOCV->sizeParam["winSize"].valeur,
		pOCV->intParam["maxLevel"].valeur,critere,pOCV->intParam["flag"].valeur,pOCV->doubleParam["minEigThreshold"].valeur);
	int k,l;
	(*op[1]->CoinRef())[i].resize((*op[1]->BonCoin())[i].size());
	for (k=l=0;k<(*op[1]->BonCoin())[i].size();k++)
		{
		if (status[k] || op[1]==op[0])
			{
			(*(op[1]->CoinRef()))[i][l]=boncoin[i][k];
			(*op[1]->BonCoin())[i][l++]=(*op[1]->BonCoin())[i][k];
			}
		}
	(*op[1]->BonCoin())[i].resize(l);
	for (k=l=0;k<(*op[1]->BonCoin())[i].size();k++)
		{
		if (!status[k])
			{
			(*(op[1]->CoinRef()))[i][l++]=boncoin[i][k];
			}
		}
	}
//imSuiv->CloneStat(this);
AjoutOpAttribut(pOCV);
pOCV->imgParam[pOCV->nomOperation + "prec"] = op[1];

r.push_back(op[1]);
return r;
}

//std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueFarnerback(ImageInfoCV	*imPrec,ImageInfoCV	*imSuiv,ParametreOperation *pOCV)
std::vector<ImageInfoCV *>ImageInfoCV::FlotOptiqueFarnerback(std::vector<ImageInfoCV	*>op,ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;

if (op[0]==NULL )
	{
	r.push_back(op[1]);
	return r;
	}
if (op[0]->channels()!=op[1]->channels())
	return r;
if (op[1]->FlotOptique(true)==NULL)
	return r;
if (op[0]->channels()==1)
	{
	calcOpticalFlowFarneback(*op[0], *op[1], op[1]->FlotOptique()[0], pOCV->doubleParam["pyr_scale"].valeur, pOCV->intParam["levels"].valeur, pOCV->sizeParam["winSize"].valeur.width, 
		pOCV->intParam["iterations"].valeur, pOCV->doubleParam["poly_sigma"].valeur, pOCV->intParam["flag"].valeur, pOCV->intParam["flag"].valeur);
	}
else
	{
	std::vector<UMat> d1,d2;
	cv::split( *op[0], d1 );
	cv::split( *op[1], d2 );
	for (int i=0;i<op[0]->channels();i++)
		{
		calcOpticalFlowFarneback(d1[i], d2[i], op[1]->FlotOptique()[i], pOCV->doubleParam["pyr_scale"].valeur, pOCV->intParam["levels"].valeur, pOCV->sizeParam["winSize"].valeur.width,
			pOCV->intParam["iterations"].valeur, pOCV->doubleParam["poly_sigma"].valeur, pOCV->intParam["flag"].valeur, pOCV->intParam["flag"].valeur);
		}
	}
AjoutOpAttribut(pOCV);


r.push_back(op[1]);
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::PhaseCorrelate(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] == NULL)
	{
	r.push_back(op[1]);
	return r;
	}
if (op[0]->channels() != op[1]->channels())
	return r;
if (!op[1]->Ponderation())
	{
	op[1]->Ponderation(true);
	}
UMat im1,im2;
op[0]->convertTo(im1, CV_64F);
op[1]->convertTo(im2, CV_64F);
pOCV->doubleParam["response"]=0;
cv::Point2d shift = cv::phaseCorrelate(im1, im2, *op[1]->Ponderation());
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
r.push_back(op[1]); 
return r;
}


std::vector<ImageInfoCV *> ImageInfoCV::MAJHistoriqueMvt(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
if (op[0] == NULL)
	{
	if (pOCV->imgParam.find(pOCV->nomOperation + "prec") == pOCV->imgParam.end())
		
		{
		throw std::string("First image is NULL in MAJHistoriqueMvt");
		return r;
		}
	op[0] = pOCV->imgParam[pOCV->nomOperation + "prec"];
	}
if (op[1] == NULL)
	{

	throw std::string("Second image is NULL in MAJHistoriqueMvt");
	return r;
	}
if (op[0]->channels() != op[1]->channels() || op[0]->size() != op[1]->size())
{
	throw std::string("Channel and size must be equal");
	return r;
}
if (op[0]->channels() != 1)
{
	throw std::string("Number of channel must be 1");
	return r;
}
ImageInfoCV *mhi=NULL;
if (pOCV->imgParam.find(pOCV->nomOperation + "mhi") == pOCV->imgParam.end())
	{
	mhi=new ImageInfoCV(op[0]->rows,op[0]->cols,CV_32FC1);
	pOCV->imgParam[pOCV->nomOperation + "mhi"] = mhi;
	}
else
	mhi = pOCV->imgParam[pOCV->nomOperation + "mhi"];
UMat *silh1=mhi->Silh(true);
absdiff(*op[1], *op[0], *silh1); 
threshold(*silh1, *silh1, pOCV->doubleParam["thresh"].valeur, pOCV->doubleParam["maxval"].valeur, pOCV->intParam["threshold_type"].valeur);
pOCV->doubleParam["timestamp"].valeur = (double)clock() / CLOCKS_PER_SEC;
cv::motempl::updateMotionHistory(*silh1, *mhi, pOCV->doubleParam["timestamp"].valeur, pOCV->doubleParam["duration"].valeur); // update MHI
pOCV->imgParam[pOCV->nomOperation+"prec"] = op[1];
mhi->ParamOCVUpdateMotionHistory(pOCV);

r.push_back(mhi);
return r;

}





std::vector<ImageInfoCV	*>ImageInfoCV::CalcOrientationMvt(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
if (op[0]->depth()!=CV_32FC1)
	throw std::string("CalcOrientationMvt :image must be single channel floating-point");
if (masqueMOG)
delete masqueMOG;
if (orient)
	delete orient;
masqueMOG = new UMat();
orient = new UMat();	// calculate motion gradient orientation and valid orientation mask
cv::motempl::calcMotionGradient(*op[0], *masqueMOG, *orient, pOCV->doubleParam["delta1"].valeur, pOCV->doubleParam["delta2"].valeur, pOCV->intParam["aperture_size"].valeur);
op[0]->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(this);
return r;
}

std::vector<ImageInfoCV *>ImageInfoCV::SegmenteMvt(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
if (op[0]->depth() != CV_32FC1)
	throw std::string("CalcOrientationMvt :image must be single channel floating-point");
if (!masqueMOG)
	throw std::string("SegmenteMvt : masque is NULL");
if (!orient) 
	throw std::string("SegmenteMvt : orient is NULL");
regionsMvt.clear();
if (segmvt==NULL)
	segmvt  = new cv::UMat();
cv::motempl::segmentMotion(*op[0], *segmvt, regionsMvt, pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur, pOCV->doubleParam["segThresh"].valeur);
if (pOCV->intParam["calcGlobalOrientation"].valeur==1)
	{
	UMat mask;
	op[0]->convertTo(mask, CV_8U, 255. / pOCVUpdateMotionHistory->doubleParam["duration"].valeur, (pOCVUpdateMotionHistory->doubleParam["duration"].valeur - pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur)*255. / pOCVUpdateMotionHistory->doubleParam["duration"].valeur);
	angle.resize(regionsMvt.size());
	for (int i = 0; i < (int)regionsMvt.size(); i++) 
		{
		UMat silh_roi = (*silh)(regionsMvt[i]);
		UMat mhi_roi = (*op[0])(regionsMvt[i]);
		UMat orient_roi = (*orient)(regionsMvt[i]);
		UMat mask_roi = mask(regionsMvt[i]);
		angle[i] = cv::motempl::calcGlobalOrientation(orient_roi, mask_roi, mhi_roi, pOCVUpdateMotionHistory->doubleParam["timestamp"].valeur, pOCVUpdateMotionHistory->doubleParam["duration"].valeur);

		}
	}
op[0]->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(op[0]);
return r;
}


std::vector<ImageInfoCV		*>ImageInfoCV::TransAffine(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
ImageInfoCV *imDst = new ImageInfoCV();
if (homography.empty())
{
    cv::Mat			inter;
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];

    cv::Mat rotation(2, 3, CV_32FC1);
    cv::Mat affinite(2, 3, CV_32FC1);

    srcTri[0] = pOCV->pointParam["src1"].valeur;
    srcTri[1] = pOCV->pointParam["src2"].valeur;
    srcTri[2] = pOCV->pointParam["src3"].valeur;

    dstTri[0] = pOCV->pointParam["dst1"].valeur;
    dstTri[1] = pOCV->pointParam["dst2"].valeur;
    dstTri[2] = pOCV->pointParam["dst3"].valeur;

    affinite = getAffineTransform(srcTri, dstTri);
    cv::warpAffine(*op[0], inter, affinite, pOCV->sizeParam["dsize"].valeur, pOCV->intParam["flags"].valeur, pOCV->intParam["borderMode"].valeur, pOCV->doubleParam["borderValue"].valeur);

    /** Rotating the image after Warp */

    /// Compute a rotation matrix with respect to the center of the image

    /// Get the rotation matrix with the specifications above
    rotation = getRotationMatrix2D(pOCV->pointParam["centre"].valeur, pOCV->doubleParam["angle"].valeur, pOCV->doubleParam["scale"].valeur);
    warpAffine(inter, *imDst, rotation, pOCV->sizeParam["dsize"].valeur, pOCV->intParam["flags"].valeur, pOCV->intParam["borderMode"].valeur, pOCV->doubleParam["borderValue"].valeur);
}

std::vector<ImageInfoCV	*> r;
r.push_back(imDst);
return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::TransPerspective(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV)
{
if (homography.empty())
	throw std::string("You must match and findhomography first");
ImageInfoCV *imDst = new ImageInfoCV();
warpPerspective(*op[0], *imDst,homography, pOCV->sizeParam["dsize"].valeur, pOCV->intParam["flags"].valeur, pOCV->intParam["borderMode"].valeur, pOCV->doubleParam["borderValue"].valeur);
op[0]->AjoutOpAttribut(pOCV);
std::vector<ImageInfoCV	*> r;
r.push_back(imDst);
return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::Dimension(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::resize(*op[0], *imDst, pOCV->sizeParam["dsize"].valeur, pOCV->doubleParam["fx"].valeur, pOCV->doubleParam["fy"].valeur, pOCV->intParam["InterpolationFlags"].valeur);

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::CorrigeAberation(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
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

	cv::undistort(*op[0], *imDst, matriceCamera, coeffDistor, cv::noArray());

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::LogPolar(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::logPolar(*op[0], *imDst, pOCV->pointParam["center"].valeur, pOCV->doubleParam["M"].valeur ,pOCV->intParam["interpolationFlags"].valeur);

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV		*>ImageInfoCV::LinearPolar(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
	ImageInfoCV *imDst = new ImageInfoCV();
	cv::linearPolar(*op[0], *imDst, pOCV->pointParam["center"].valeur, pOCV->doubleParam["M"].valeur ,pOCV->intParam["interpolationFlags"].valeur);

	std::vector<ImageInfoCV	*> r;
	r.push_back(imDst);
	return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::Fond_MOG(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
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
pOCV->ecartFond["MOG"].dynamicCast<cv::bgsegm::BackgroundSubtractorMOG>()->apply(*op[0], *imDst, pOCV->doubleParam["learningRate"].valeur);
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
	op[0]->copyTo(*premierPlan,*imDst);
	r.push_back(premierPlan);
	}
	break;
	}
	
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_MOG2(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
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
pOCV->ecartFond["MOG2"].dynamicCast<cv::BackgroundSubtractorMOG2>()->apply(*op[0], *imDst, pOCV->doubleParam["learningRate"].valeur);
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
	op[0]->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_KNN(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
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
pOCV->ecartFond["KNN"].dynamicCast<cv::BackgroundSubtractorKNN>()->apply(*op[0], *imDst, pOCV->doubleParam["learningRate"].valeur);
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
	op[0]->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::Fond_GMG(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
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
pOCV->ecartFond["GMG"].dynamicCast<cv::bgsegm::BackgroundSubtractorGMG>()->apply(*op[0], *imDst);
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
	op[0]->copyTo(*premierPlan, *imDst);
	r.push_back(premierPlan);
	}
	break;
	}
return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::DetailFeaturesFinder(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV)
{
if (this->pano==NULL)
{
    pano = new Panoramique;
}
pano->is_compose_scale_set = false;
pano->try_cuda = false;
pano->is_work_scale_set = false;
pano->seam_work_aspect = 1;
pano->work_megapix = 0.6;
pano->seam_megapix = 0.1;
pano->compose_megapix = -1;
pano->work_scale = 1;
pano->seam_scale = 1;
pano->compose_scale = 1;

if (pOCV->pano.size()==0)
{
    pOCV->pano.push_back(pano);
}
cv::Ptr<cv::detail::FeaturesFinder> finder;
if (pOCV->intParam["Stitch_descriptor"].valeur==0 )
{
    finder = cv::makePtr<cv::detail::OrbFeaturesFinder>(pOCV->sizeParam["orb_grid_size"].valeur,pOCV->intParam["orb_nfeatures"].valeur,(float)pOCV->doubleParam["orb_scaleFactor"].valeur,pOCV->intParam["orb_nlevels"].valeur);
}
if (pOCV->intParam["Stitch_descriptor"].valeur==1)
{
    finder = cv::makePtr<cv::detail::SurfFeaturesFinder>(pOCV->doubleParam["surf_hess_thresh"].valeur,pOCV->intParam["surf_num_octaves"].valeur,
        pOCV->intParam["surf_num_layers"].valeur,pOCV->intParam["surf_num_octaves_descr"].valeur,pOCV->intParam["surf_num_layers_descr"].valeur);
}
pano->features.resize(pOCV->op.size());
UMat img;
pano->op=pOCV->op;
pano->indOpFenetre=pOCV->indOpFenetre;
pano->tabImg.resize(pano->op.size());
for (int i = 0; i < pOCV->op.size(); ++i)
{
	if (pano->work_megapix < 0)
	{
		img = *pano->op[i];
		pano->work_scale = 1;
		pano->is_work_scale_set = true;
	}
	else
	{
		if (!pano->is_work_scale_set)
		{
			pano->work_scale = cv::min(1.0, sqrt(pano->work_megapix * 1e6 / pano->op[i]->size().area()));
			pano->is_work_scale_set = true;
		}
		cv::resize(*pano->op[i], img, cv::Size(), pano->work_scale, pano->work_scale);
	}
	if (!pano->is_seam_scale_set)
	{
		pano->seam_scale = cv::min(1.0, sqrt(pano->seam_megapix * 1e6 / pano->op[i]->size().area()));
		pano->seam_work_aspect = pano->seam_scale / pano->work_scale;
		pano->is_seam_scale_set = true;
	}
	(*finder)(*pOCV->op[i], pano->features[i]);
    pano->features[i].img_idx = i;
	cv::resize(*pano->op[i], img, cv::Size(), pano->seam_scale, pano->seam_scale);
	pano->tabImg[i] = img.clone();
}
finder->collectGarbage();
std::vector<ImageInfoCV	*> r;
ImageInfoCV *t = new ImageInfoCV(1, 1, pano->op[0]->type());
t->pano = pano;
pano = NULL;
r.push_back(t);
return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::DetailMatchesInfo(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
std::vector<ImageInfoCV	*> r;
r.push_back(this);
if (pano==NULL)
    return r;
cv::detail::BestOf2NearestMatcher matcher(pOCV->intParam["try_use_gpu"].valeur, pOCV->doubleParam["match_conf"].valeur,
    pOCV->intParam["num_matches_thresh1"].valeur,pOCV->intParam["num_matches_thresh2"].valeur);

matcher(pano->features, pano->appariement);
matcher.collectGarbage();

AjoutOpAttribut(pOCV);
r.push_back(this);
return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::LeaveBiggestComponent(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    if (pano==NULL)
        return r;
    pano->bijection = cv::detail::leaveBiggestComponent(pano->features, pano->appariement, pOCV->doubleParam["conf_thresh"].valeur);
    if (pano->bijection.size()<1)
    {
		throw std::string("LeaveBiggestComponent : Cannot match this images!");
		return r;
	}
	std::vector<ImageInfoCV	*> bonnesImages;
	std::vector<UMat> bonnesMat;
	std::vector<int> bonIndices;
    for (size_t i = 0; i < pano->bijection.size(); ++i)
    {
		bonnesMat.push_back(pano->tabImg[pano->bijection[i]]);
		bonnesImages.push_back(pano->op[pano->bijection[i]]);
		bonIndices.push_back(pano->indOpFenetre[pano->bijection[i]]);
    }
	pano->op = bonnesImages;
	pano->tabImg = bonnesMat;
	pano->indOpFenetre = bonIndices;
    r.push_back(this);
    AjoutOpAttribut(pOCV);
    return r;
}
std::vector<ImageInfoCV	*> ImageInfoCV::HomographyBasedEstimator(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    if (pano==NULL)
        return r;
    pano->cameras.clear();
    cv::detail::HomographyBasedEstimator estimator(pOCV->intParam["is_focals_estimated"].valeur);
    
    if (!estimator(pano->features, pano->appariement, pano->cameras))
    {
		throw std::string("HomographyBasedEstimator : Homography estimation failed!");
		return r;
    }

    for (size_t i = 0; i < pano->cameras.size(); ++i)
    {
        cv::Mat R;
        pano->cameras[i].R.convertTo(R, CV_32F);
        pano->cameras[i].R = R;
    }

    
    if (pOCV->intParam["ba_cost_func"].valeur == 0) 
        pano->adjuster = cv::makePtr<cv::detail::BundleAdjusterReproj>();
    else if (pOCV->intParam["ba_cost_func"].valeur == 1) 
        pano->adjuster = cv::makePtr<cv::detail::BundleAdjusterRay>();
    else
    {
		throw std::string("HomographyBasedEstimator : unknown BundleAdjusterReproj!");
		return r;
    }
    pano->adjuster->setConfThresh((float)pOCV->doubleParam["conf_thresh"].valeur);
    cv::Mat_<uchar> refine_mask = cv::Mat::zeros(3, 3, CV_8U);
    if (pOCV->intParam["ba_refine_mask_0"].valeur == 1) refine_mask(0,0) = 1;
    if (pOCV->intParam["ba_refine_mask_2"].valeur == 1) refine_mask(0,2) = 1;
    if (pOCV->intParam["ba_refine_mask_3"].valeur == 1) refine_mask(1,1) = 1;
    if (pOCV->intParam["ba_refine_mask_4"].valeur == 1) refine_mask(1,2) = 1;
    pano->adjuster->setRefinementMask(refine_mask);
    if (!(*pano->adjuster)(pano->features, pano->appariement, pano->cameras))
    {
		throw std::string("HomographyBasedEstimator : Camera parameters adjusting failed!");
		return r;
    }

    // Find median focal length

    std::vector<double> focals;
    for (size_t i = 0; i < pano->cameras.size(); ++i)
    {
        focals.push_back(pano->cameras[i].focal);
    }

    sort(focals.begin(), focals.end());
    if (focals.size() % 2 == 1)
        pano->warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
    else
        pano->warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;
    if (pOCV->intParam["do_wave_correct"].valeur!=0)
    {
        std::vector<cv::Mat> rmats;
        for (size_t i = 0; i < pano->cameras.size(); ++i)
            rmats.push_back(pano->cameras[i].R.clone());
        cv::detail::waveCorrect(rmats, (cv::detail::WaveCorrectKind)pOCV->intParam["wave_correct"].valeur);
        for (size_t i = 0; i < pano->cameras.size(); ++i)
            pano->cameras[i].R = rmats[i];
    }

    AjoutOpAttribut(pOCV);
    r.push_back(this);
    return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::WraperWrap(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
    std::vector<ImageInfoCV	*> r;
    if (pano==NULL)
        return r;
	if (pano->cameras.size() == 0)
		return r;
	if (pano->op.size()<=1)
    {
		throw std::string("WraperWrap : not enough image! ");
		return r;
    }
    
    int num_images=pano->tabImg.size();
    pano->corners.clear();
    pano->masks_warped.clear();
    pano->images_warped.clear();
    pano->sizes.clear();
    pano->masks.clear();
    pano->corners.resize(num_images);
    pano->masks_warped.resize(num_images);
    pano->images_warped.resize(num_images);
	pano->sizes.resize(num_images);
	pano->masks.resize(num_images);

    // Prepare images masks
    for (int i = 0; i < num_images; ++i)
    {
		pano->masks[i].create(pano->tabImg[i].size(), CV_8U);
		pano->masks[i].setTo(cv::Scalar::all(255));
    }

    // Warp images and their masks

#ifdef HAVE_OPENCV_CUDAWARPING
    if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
    {
        if (pOCV->intParam["warp_type"].valeur ==0)
            warper_creator = cv::makePtr<cv::PlaneWarperGpu>();
        else if (pOCV->intParam["warp_type"].valeur ==1)
            warper_creator = cv::makePtr<cv::CylindricalWarperGpu>();
        else if (pOCV->intParam["warp_type"].valeur ==2)
            warper_creator = cv::makePtr<cv::SphericalWarperGpu>();
    }
    else
#endif
    {
        if (pOCV->intParam["warp_type"].valeur ==0)
            pano->warper_creator = cv::makePtr<cv::PlaneWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==1)
			pano->warper_creator = cv::makePtr<cv::CylindricalWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==2)
			pano->warper_creator = cv::makePtr<cv::SphericalWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==3)
			pano->warper_creator = cv::makePtr<cv::FisheyeWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==4)
			pano->warper_creator = cv::makePtr<cv::StereographicWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==5)
			pano->warper_creator = cv::makePtr<cv::CompressedRectilinearWarper>(2.0f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==6)
			pano->warper_creator = cv::makePtr<cv::CompressedRectilinearWarper>(1.5f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==7)
			pano->warper_creator = cv::makePtr<cv::CompressedRectilinearPortraitWarper>(2.0f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==8)
			pano->warper_creator = cv::makePtr<cv::CompressedRectilinearPortraitWarper>(1.5f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==9)
			pano->warper_creator = cv::makePtr<cv::PaniniWarper>(2.0f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==10)
			pano->warper_creator = cv::makePtr<cv::PaniniWarper>(1.5f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==11)
			pano->warper_creator = cv::makePtr<cv::PaniniPortraitWarper>(2.0f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==12)
			pano->warper_creator = cv::makePtr<cv::PaniniPortraitWarper>(1.5f, 1.0f);
        else if (pOCV->intParam["warp_type"].valeur ==13)
			pano->warper_creator = cv::makePtr<cv::MercatorWarper>();
        else if (pOCV->intParam["warp_type"].valeur ==14)
			pano->warper_creator = cv::makePtr<cv::TransverseMercatorWarper>();
    }

	if (!pano->warper_creator)
    {
		throw std::string("WraperWrap : unknown wrapper!");
		return r;
    }

	pano->warper = pano->warper_creator->create(static_cast<float>(pano->warped_image_scale * pano->seam_work_aspect));

    for (int i = 0; i < num_images; ++i)
    {
        cv::Mat_<float> K;
        pano->cameras[i].K().convertTo(K, CV_32F);
        float swa = (float)pano->seam_work_aspect;
        K(0,0) *= swa; K(0,2) *= swa;
        K(1,1) *= swa; K(1,2) *= swa;

		pano->corners[i] = pano->warper->warp(pano->tabImg[i], K, pano->cameras[i].R, cv::INTER_LINEAR, cv::BORDER_REFLECT, pano->images_warped[i]);
		pano->sizes[i] = pano->images_warped[i].size();

		pano->warper->warp(pano->masks[i], K, pano->cameras[i].R, cv::INTER_NEAREST, cv::BORDER_CONSTANT, pano->masks_warped[i]);
    }

    pano->images_warped_f.resize(num_images);
    for (int i = 0; i < num_images; ++i)
		pano->images_warped[i].convertTo(pano->images_warped_f[i], CV_32F);
	AjoutOpAttribut(pOCV);
	r.push_back(this);
	return r;
}


std::vector<ImageInfoCV	*> ImageInfoCV::CorrectionExpo(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
	std::vector<ImageInfoCV	*> r;
	if (pano == NULL)
		return r;
	if (pano->op.size() <= 1)
	{
		throw std::string("WraperWrap : not enough image!");
		return r;
	}
	pano->correcteurExpo = cv::detail::ExposureCompensator::createDefault(pOCV->intParam["expos_comp_type"].valeur);
	pano->correcteurExpo->feed(pano->corners, pano->images_warped, pano->masks_warped);

    if (pano->op[0]->channels() == 1)
        pOCV->intParam["seam_find_type"].valeur = 0;

	if (pOCV->intParam["seam_find_type"].valeur == 0)
		pano->couture = cv::makePtr<cv::detail::NoSeamFinder>();
	else if (pOCV->intParam["seam_find_type"].valeur == 1)
		pano->couture = cv::makePtr<cv::detail::VoronoiSeamFinder>();
	else if (pOCV->intParam["seam_find_type"].valeur == 2)
	{
#ifdef HAVE_OPENCV_CUDALEGACY
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			pano->couture = cv::makePtr<cv::detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
		else
#endif
			pano->couture = cv::makePtr<cv::detail::GraphCutSeamFinder>(cv::detail::GraphCutSeamFinderBase::COST_COLOR);
	}
	else if (pOCV->intParam["seam_find_type"].valeur == 3)
	{
#ifdef HAVE_OPENCV_CUDALEGACY
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			pano->couture = cv::makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
		else
#endif
			pano->couture = cv::makePtr<cv::detail::GraphCutSeamFinder>(cv::detail::GraphCutSeamFinderBase::COST_COLOR_GRAD);
	}
	else if (pOCV->intParam["seam_find_type"].valeur == 4)
		pano->couture = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
	else if (pOCV->intParam["seam_find_type"].valeur == 5)
		pano->couture =cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR_GRAD);
	if (!pano->couture)
	{
		throw std::string("Can't create the seam finder '");
		return r;
	}

	pano->couture->find(pano->images_warped_f, pano->corners, pano->masks_warped);

	// Release unused memory
//	pano->images.clear();
	AjoutOpAttribut(pOCV);
	r.push_back(this);
	return r;
}

std::vector<ImageInfoCV	*> ImageInfoCV::PanoComposition(std::vector< ImageInfoCV *>, ParametreOperation *pOCV)
{
    pano->camerasPano = pano->cameras;
    pano->cornersPano = pano->corners;
    pano->sizesPano = pano->sizes;

	std::vector<ImageInfoCV	*> r;
	bool timelapse = false;
    pano->is_compose_scale_set = false;
    pano->compose_scale = 1;
    pano->warped_image_scalePano = pano->warped_image_scale;
	if (pano == NULL)
		return r;
	if (pano->op.size() <= 1)
	{
		throw std::string("WraperWrap : not enough image!");
		return r;
	}
	UMat img_warped, img_warped_s;
	UMat dilated_mask, seam_mask, mask, mask_warped;
	cv::Ptr<cv::detail::Blender> blender;
	cv::Ptr<cv::detail::Timelapser> timelapser;
	//double compose_seam_aspect = 1;
	double compose_work_aspect = 1;
	UMat img;
    if (pano->op[0]->channels() == 1)
        pOCV->intParam["blend_type"].valeur = 0;
	for (int img_idx = 0; img_idx < pano->op.size(); ++img_idx)
	{
		LOGLN("Compositing image #" << indices[img_idx] + 1);

		// Read image and resize it if necessary
		if (!pano->is_compose_scale_set)
		{
			if (pano->compose_megapix > 0)
				pano->compose_scale = cv::min(1.0, sqrt(pano->compose_megapix * 1e6 / pano->op[img_idx]->size().area()));
			pano->is_compose_scale_set = true;
			// Compute relative scales
			//compose_seam_aspect = compose_scale / seam_scale;
			compose_work_aspect = 1;

			// Update warped image scale
			pano->warped_image_scalePano *= static_cast<float>(compose_work_aspect);
			pano->warper = pano->warper_creator->create(pano->warped_image_scalePano);

			// Update corners and sizes
			for (int i = 0; i < pano->op.size(); ++i)
			{
				// Update intrinsics
                pano->camerasPano[i].focal *= compose_work_aspect;
                pano->camerasPano[i].ppx *= compose_work_aspect;
                pano->camerasPano[i].ppy *= compose_work_aspect;

				// Update corner and size
				cv::Size sz = pano->op[i]->size();
				if (std::abs(pano->compose_scale - 1) > 1e-1)
				{
					sz.width = cvRound(pano->op[i]->size().width * pano->compose_scale);
					sz.height = cvRound(pano->op[i]->size().height * pano->compose_scale);
				}

				UMat K;
                pano->camerasPano[i].K().convertTo(K, CV_32F);
                cv::Rect roi = pano->warper->warpRoi(sz, K, pano->camerasPano[i].R);
				pano->cornersPano[i] = roi.tl();
				pano->sizes[i] = roi.size();
			}
		}
		if (abs(pano->compose_scale - 1) > 1e-1)
			cv::resize(*pano->op[img_idx], img, cv::Size(), pano->compose_scale, pano->compose_scale);
		else
			img = *pano->op[img_idx];
		img = *pano->op[img_idx];
		cv::Size img_size = img.size();

		cv::Mat K;
        pano->camerasPano[img_idx].K().convertTo(K, CV_32F);

		// Warp the current image
        pano->warper->warp(img, K, pano->camerasPano[img_idx].R, cv::INTER_LINEAR, cv::BORDER_REFLECT, img_warped);

		// Warp the current image mask
		mask.create(img_size, CV_8U);
		mask.setTo(cv::Scalar::all(255));
        pano->warper->warp(mask, K, pano->camerasPano[img_idx].R, cv::INTER_NEAREST, cv::BORDER_CONSTANT, mask_warped);

		// Compensate exposure
        if (pano->op[0]->channels()==3)
            pano->correcteurExpo->apply(img_idx, pano->cornersPano[img_idx], img_warped, mask_warped);

		img_warped.convertTo(img_warped_s, CV_16S);
		img_warped.release();
		img.release();
		mask.release();

		dilate(pano->masks_warped[img_idx], dilated_mask, UMat());
		cv::resize(dilated_mask, seam_mask, mask_warped.size());
		cv::bitwise_and(seam_mask , mask_warped,mask_warped);
//		mask_warped = seam_mask & mask_warped;

		if (!blender && !timelapse)
		{
			blender = cv::detail::Blender::createDefault(pOCV->intParam["blend_type"].valeur, pano->try_cuda);
			cv::Size dst_sz = cv::detail::resultRoi(pano->cornersPano, pano->sizes).size();
			float blend_width = sqrt(static_cast<float>(dst_sz.area())) * pOCV->doubleParam["blend_strength"].valeur / 100.f;
			if (blend_width < 1.f)
				blender = cv::detail::Blender::createDefault(cv::detail::Blender::NO, pano->try_cuda);
			else if (pOCV->intParam["blend_type"].valeur == cv::detail::Blender::MULTI_BAND)
			{
				cv::detail::MultiBandBlender* mb = dynamic_cast<cv::detail::MultiBandBlender*>(blender.get());
				mb->setNumBands(static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.));
				LOGLN("Multi-band blender, number of bands: " << mb->numBands());
			}
			else if (pOCV->intParam["blend_type"].valeur == cv::detail::Blender::FEATHER)
			{
				cv::detail::FeatherBlender* fb = dynamic_cast<cv::detail::FeatherBlender*>(blender.get());
				fb->setSharpness(1.f / blend_width);
				LOGLN("Feather blender, sharpness: " << fb->sharpness());
			}
			blender->prepare(pano->cornersPano, pano->sizes);
		}

			blender->feed(img_warped_s, mask_warped, pano->cornersPano[img_idx]);
	}

	if (!timelapse)
	{
		ImageInfoCV * result = new ImageInfoCV();
		UMat result_mask;
        result->pano = pano;
		blender->blend(*result, result_mask);
        this->release();
        this->create(result->rows,result->cols, result->type());
        result->copyTo(*this);
		r.push_back(this);
        AjoutOpAttribut(pOCV);
        return r;

	}




	r.push_back(NULL);
	return r;
}