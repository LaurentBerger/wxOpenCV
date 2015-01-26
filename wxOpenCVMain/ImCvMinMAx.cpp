#include "ImageInfo.h"

void ImageInfoCV::ExtremumLoc(ImageInfoCV *mask )
{
if (minIm==NULL )
	{
	minIm = new double[channels()];			/*< Minimimum pour chaque plan de l'image */
	maxIm = new double[channels()];			/*< Maximimum pour chaque plan de l'image */
	locMin = new cv::Point[channels()]	;	/*< Position du miminmu pour chaque plan */
	locMax = new cv::Point[channels()]	;	/*< Position du miminmu pour chaque plan */
	}
if (channels()==1)
	{
	if (mask==NULL)
		minMaxLoc((cv::Mat)*this,minIm,maxIm,locMin,locMax);
	else
		minMaxLoc((cv::Mat)*this,minIm,maxIm,locMin,locMax,*mask);
	}
else  
	{
	switch(depth()){
	case CV_8U :
		{
		cv::Vec3b p=at<cv::Vec3b>(0,0);
		for (int i=0;i<channels();i++)
			{
			minIm[i]=255;
			maxIm[i]=0;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			unsigned char *d=(unsigned char*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
	case CV_8S :
		{
		cv::Vec3b p=at<cv::Vec3b>(0,0);
		for (int i=0;i<channels();i++)
			{
			minIm[i]=127;
			maxIm[i]=-128;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			char *d=(char*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
	case CV_16S :
		{
		for (int i=0;i<channels();i++)
			{
			minIm[i]=32767;
			maxIm[i]=-32768;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			short *d=(short*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
	case CV_16U :
		{
		for (int i=0;i<channels();i++)
			{
			minIm[i]=32767;
			maxIm[i]=-32768;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			unsigned short *d=(unsigned short*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
	case CV_32F:
		{
		for (int i=0;i<channels();i++)
			{
			minIm[i]=FLT_MAX;
			maxIm[i]=FLT_MIN;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			float *d=(float*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
	case CV_64F:
		{
		cv::Vec3f p=at<cv::Vec3f>(0,0);
		for (int i=0;i<channels();i++)
			{
			minIm[i]=DBL_MAX;
			maxIm[i]=DBL_MIN;
			locMin[i] = cv::Point(0,0);
			locMax[i] = cv::Point(0,0);
			}
		for (int i=0;i<rows;i++)		
			{
			double *d=(double*)ptr(i);
			for (int j=0;j<cols;j++)
				{
				
				for (int k=0;k<channels();k++)
					{
					if (d[k]<minIm[k])
						{
						minIm[k]= d[k];
						locMin[k]= cv::Point(i,j);
						}
					if (d[k]>maxIm[k])
						{
						maxIm[k]= d[k];
						locMax[k]= cv::Point(i,j);
						}
					}
				}
			}
		}
		break;
		}

	}
}

