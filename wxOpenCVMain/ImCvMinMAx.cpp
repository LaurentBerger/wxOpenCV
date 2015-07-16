#include "ImageInfo.h"

void ImageInfoCV::ExtremumLoc(ImageInfoCV *mask )
{
    cv::ocl::setUseOpenCL(false);

if (minIm.size()==0 )
	{
	minIm.resize(channels());			/*< Minimimum pour chaque plan de l'image */
	maxIm.resize(channels());			/*< Maximimum pour chaque plan de l'image */
	locMin.resize(channels())	;	/*< Position du miminmu pour chaque plan */
	locMax.resize(channels())	;	/*< Position du miminmu pour chaque plan */
	}
if (channels()==1)
	{
	if (mask==NULL)
		minMaxLoc((cv::UMat)*this,&minIm[0],&maxIm[0],&locMin[0],&locMax[0]);
	else
		minMaxLoc((cv::UMat)*this,&minIm[0],&maxIm[0],&locMin[0],&locMax[0],*mask);
	}
else  
	{
        std::vector<UMat> plan;
        split(*this,plan);

        for (int i = 0; i < channels(); i++)
        {
            if (mask==NULL)
		        minMaxLoc(plan[i],&minIm[i],&maxIm[i]);//,&locMin[i],&locMax[i]);
	        else
		        minMaxLoc(plan[i],&minIm[i],&maxIm[i],&locMin[i],&locMax[i],*mask);

        }
    }
    cv::ocl::setUseOpenCL(true);
}

