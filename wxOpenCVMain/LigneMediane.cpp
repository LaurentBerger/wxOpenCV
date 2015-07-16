#include "ImageInfo.h"

cv::Mat mThis;

std::vector<ImageInfoCV		*>ImageInfoCV::LigneMediane (std::vector<ImageInfoCV *> op,ParametreOperation *paramOCV)
{
std::vector<ImageInfoCV	*> r;

if (depth()!=CV_32F && depth()!=CV_8U)
	{
	return r;
	}
cv::Mat mThis = getMat(cv::ACCESS_RW);
cv::Mat	imRes(rows,cols,CV_MAKETYPE(CV_32F,channels()));
float *ptf;
for (int i=0;i<rows;i++)
	{
	ptf=(float*)imRes.ptr(i);
	for (int j=0;j<cols;j++)
		for (int k=0;k<channels();k++,ptf++)
			*ptf=0;
		}

for (int c=0;c<channels();c++)
	{
	ptf=(float*)mThis.ptr(0)+c;
	for (int j=0;j<cols;j++,ptf+=channels())
		*ptf=0;
	ptf=(float*)mThis.ptr(rows-1)+c;
	for (int j=0;j<cols;j++,ptf+=channels())
		*ptf=0;
	for (int i=0;i<rows;i++)
		{
		*((float*)mThis.ptr(i,0)+c)=0;
		*((float*)mThis.ptr(i,cols-1)+c)=0;
		}
	}

for (int c=0;c<channels();c++)
	if (depth()==CV_32F)
		{
		for (int i=1;i<rows-1;i++)
			{
			ptf =(float*)imRes.ptr(i)+1*channels()+c;
			for (int j=1;j<cols-1;j++,ptf+=channels())
				if ((*((float*)mThis.ptr(i,j)+c)!=0)&&(*ptf==0))
					{
					float 	k= *((float*)mThis.ptr(i,j)+c);
					if ((*((float*)mThis.ptr(i+1,j+1)+c)==k)&&(*((float*)mThis.ptr(i+1,j)+c)>k)&&
						(*((float*)mThis.ptr(i,j+1)+c)>k))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,imRes);
							}
						if (!MaxLocal(c,i+1,j+1))
							{
							ptf[(cols+1)*channels()] = *((float*)mThis.ptr(i+1,j+1)+c);
							SuiviChemin(c,i+1,j+1,imRes);
							}
						}
					else if ((*((float*)mThis.ptr(i+1,j-1)+c)==k)&&(*((float*)mThis.ptr(i,j-1)+c)>k)&&
								(*((float*)mThis.ptr(i+1,j)+c)>k))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,imRes);
							}
						if (!MaxLocal(c,i+1,j-1))
							{
							ptf[(cols-1)*channels()] = *((float*)mThis.ptr(i+1,j-1)+c);
							SuiviChemin(c,i+1,j-1,imRes);
							}
						}
					if (MaxLocal(c,i,j))
						*ptf = k;
					else
						{
						long 	z=0;
				
						if (k==*((float*)mThis.ptr(i+1,j)+c))
							z++;
						if (k==*((float*)mThis.ptr(i-1,j)+c))
							z++;
						if (k==*((float*)mThis.ptr(i,j-1)+c))
							z++;
						if (k==*((float*)mThis.ptr(i,j+1)+c))
							z++;
						if (z==4)
							{
							*ptf = k;
							if ((!MaxLocal(c,i-1,j-1))&&(k+1==*((float*)mThis.ptr(i-1,j-1)+c)))
								{
								ptf[(-cols-1)*channels()] = *((float*)mThis.ptr(i-1,j-1)+c);
								SuiviChemin(c,i-1,j-1,imRes);
								}
							if ((!MaxLocal(c,i-1,j+1))&&(k+1==*((float*)mThis.ptr(i-1,j+1)+c)))
								{
								ptf[(-cols+1)*channels()] = *((float*)mThis.ptr(i-1,j+1)+c);
								SuiviChemin(c,i-1,j+1,imRes);
								}
							if ((!MaxLocal(c,i+1,j-1))&&(k+1==*((float*)mThis.ptr(i+1,j-1)+c)))
								{
								ptf[(cols-1)*channels()] = *((float*)mThis.ptr(i+1,j-1)+c);
								SuiviChemin(c,i+1,j-1,imRes);
								}
							if ((!MaxLocal(c,i+1,j+1))&&(k+1==*((float*)mThis.ptr(i+1,j+1)+c)))
								{
								ptf[(cols+1)*channels()] = *((float*)mThis.ptr(i+1,j+1)+c);
								SuiviChemin(c,i+1,j+1,imRes);
								}
							}
						else if (z==3)
							{
							*ptf = (unsigned char)k;
							SuiviChemin(c,i,j,imRes);
							}
						}
					}
				}
			
		}
	else
		{
		for (int i=1;i<rows-1;i++)
			{
			ptf =(float*)imRes.ptr(i)+1*c;
			for (int j=1;j<cols-1;j++,ptf+=c)
				if ((*((float*)mThis.ptr(i,j)+c)!=0)&&(*ptf==0))
					{
					float 	k= *(mThis.ptr(i,j)+c);
					if ((*(mThis.ptr(i+1,j+1)+c)==k)&&(*(mThis.ptr(i+1,j)+c)>k)&&
						(*(mThis.ptr(i,j-1)+c)>k))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,imRes);
							}
						if (!MaxLocal(c,i+1,j+1))
							{
							ptf[(cols+1)*channels()] = *(mThis.ptr(i+1,j+1)+c);
							SuiviChemin(c,i+1,j+1,imRes);
							}
						}
					else if ((*(mThis.ptr(i+1,j-1)+c)==k)&&(*(mThis.ptr(i,j-1)+c)>k)&&
								(*(mThis.ptr(i+1,j)+c)>k))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,imRes);
							}
						if (!MaxLocal(c,i+1,j-1))
							{
							ptf[(cols-1)*channels()] = *(mThis.ptr(i+1,j-1)+c);
							SuiviChemin(c,i+1,j-1,imRes);
							}
						}
					if (MaxLocal(c,i,j))
						*ptf = k;
					else
						{
						long 	z=0;
				
						if (k==*(mThis.ptr(i+1,j)+c))
							z++;
						if (k==*(mThis.ptr(i-1,j)+c))
							z++;
						if (k==*(mThis.ptr(i,j-1)+c))
							z++;
						if (k==*(mThis.ptr(i,j+1)+c))
							z++;
						if (z==4)
							{
							*ptf = k;
							if ((!MaxLocal(c,i-1,j-1))&&(k+1==*(mThis.ptr(i-1,j-1)+c)))
								{
								ptf[(-cols-1)*channels()] = *(mThis.ptr(i-1,j-1)+c);
								SuiviChemin(c,i-1,j-1,imRes);
								}
							if ((!MaxLocal(c,i-1,j+1))&&(k+1==*(mThis.ptr(i-1,j+1)+c)))
								{
								ptf[(-cols+1)*channels()] = *(mThis.ptr(i-1,j+1)+c);
								SuiviChemin(c,i-1,j+1,imRes);
								}
							if ((!MaxLocal(c,i+1,j-1))&&(k+1==*(mThis.ptr(i+1,j-1)+c)))
								{
								ptf[(cols-1)*channels()] = *(mThis.ptr(i+1,j-1)+c);
								SuiviChemin(c,i+1,j-1,imRes);
								}
							if ((!MaxLocal(c,i+1,j+1))&&(k+1==*(mThis.ptr(i+1,j+1)+c)))
								{
								ptf[(cols+1)*channels()] = *((float*)mThis.ptr(i+1,j+1)+c);
								SuiviChemin(c,i+1,j+1,imRes);
								}
							}
						else if (z==3)
							{
							*ptf = k;
							SuiviChemin(c,i,j,imRes);
							}
						}
					}
				}
			
		}
mThis.release();
ImageInfoCV *res = new ImageInfoCV(rows,cols,CV_MAKETYPE(CV_32F,channels()));
imRes.copyTo(*res);
r.push_back(res);
return r;
}


bool ImageInfoCV::MaxLocal(int c,int i,int j)
{
if (depth()==CV_32F)
	{
	float	k=*((float*)mThis.ptr(i,j)+c);

	if ((k>=*((float*)mThis.ptr(i-1,j-1)+c))&&((k>=*((float*)mThis.ptr(i-1,j)+c))&&
		(k>=*((float*)mThis.ptr(i-1,j+1)+c))&&(k>=*((float*)mThis.ptr(i,j-1)+c))&&
		(k>=*((float*)mThis.ptr(i,j+1)+c))&&(k>=*((float*)mThis.ptr(i+1,j-1)+c))&&
		(k>=*((float*)mThis.ptr(i+1,j)+c))&&(k>=*((float*)mThis.ptr(i+1,j+1)+c))))
		return 1;
	else
		return 0;
	}
else if (depth()==CV_8U)
	{
	int	k=*(mThis.ptr(i,j)+c);

	if ((k>=*(mThis.ptr(i-1,j-1)+c))&&((k>=*(mThis.ptr(i-1,j)+c))&&
		(k>=*(mThis.ptr(i-1,j+1)+c))&&(k>=*(mThis.ptr(i,j-1)+c))&&
		(k>=*(mThis.ptr(i,j+1)+c))&&(k>=*(mThis.ptr(i+1,j-1)+c))&&
		(k>=*(mThis.ptr(i+1,j)+c))&&(k>=*(mThis.ptr(i+1,j+1)+c))))
		return 1;
	else
		return 0;
	}return 0;
}

bool ImageInfoCV::MaxLocal(int i,int j)
{
if (depth()==CV_32F)
	{
	float	k=*((float*)mThis.ptr(i,j));

	if ((k>=*((float*)mThis.ptr(i-1,j-1)))&&((k>=*((float*)mThis.ptr(i-1,j)))&&
		(k>=*((float*)mThis.ptr(i-1,j+1)))&&(k>=*((float*)mThis.ptr(i,j-1)))&&
		(k>=*((float*)mThis.ptr(i,j+1)))&&(k>=*((float*)mThis.ptr(i+1,j-1)))&&
		(k>=*((float*)mThis.ptr(i+1,j)))&&(k>=*((float*)mThis.ptr(i+1,j+1)))))
		return 1;
	else
		return 0;
	}
else if (depth()==CV_8U)
	{
	int	k=*(mThis.ptr(i,j));

	if ((k>=*(mThis.ptr(i-1,j-1)))&&((k>=*(mThis.ptr(i-1,j)))&&
		(k>=*(mThis.ptr(i-1,j+1)))&&(k>=*(mThis.ptr(i,j-1)))&&
		(k>=*(mThis.ptr(i,j+1)))&&(k>=*(mThis.ptr(i+1,j-1)))&&
		(k>=*(mThis.ptr(i+1,j)))&&(k>=*(mThis.ptr(i+1,j+1)))))
		return 1;
	else
		return 0;
	}return 0;
}

	
	
void	ImageInfoCV::SuiviChemin(int i,int j,cv::Mat &im)
{
char	fini;
int	k,l,q;

fini = 0;
if (depth()==CV_32F)
	{
	q=*((float*)mThis.ptr(i,j));
	k=i;
	l=j;
	while (!fini)
		{
		if (*((float*)mThis.ptr(k-1,l))>q)
			fini = TraiterMediane(k,l,-1,0,q,im);
		else if (*((float*)mThis.ptr(k+1,l))>q)
			fini = TraiterMediane(k,l,1,0,q,im);
		else if (*((float*)mThis.ptr(k,l-1))>q)
			fini = TraiterMediane(k,l,0,-1,q,im);
		else if (*((float*)mThis.ptr(k,l+1))>q)
			fini = TraiterMediane(k,l,0,1,q,im);
		else if (*((float*)mThis.ptr(k-1,l-1))>q)
			fini = TraiterMediane(k,l,-1,-1,q,im);
		else if (*((float*)mThis.ptr(k-1,l+1))>q)
			fini = TraiterMediane(k,l,-1,1,q,im);
		else if (*((float*)mThis.ptr(k+1,l-1))>q)
			fini = TraiterMediane(k,l,1,-1,q,im);
		else if (*((float*)mThis.ptr(k+1,l+1))>q)
			fini = TraiterMediane(k,l,1,1,q,im);
		}
	}
else
	{
	q=*(mThis.ptr(i,j));
	k=i;
	l=j;
	while (!fini)
		{
		if (*(mThis.ptr(k-1,l))>q)
			fini = TraiterMediane(k,l,-1,0,q,im);
		else if (*(mThis.ptr(k+1,l))>q)
			fini = TraiterMediane(k,l,1,0,q,im);
		else if (*(mThis.ptr(k,l-1))>q)
			fini = TraiterMediane(k,l,0,-1,q,im);
		else if (*(mThis.ptr(k,l+1))>q)
			fini = TraiterMediane(k,l,0,1,q,im);
		else if (*(mThis.ptr(k-1,l-1))>q)
			fini = TraiterMediane(k,l,-1,-1,q,im);
		else if (*(mThis.ptr(k-1,l+1))>q)
			fini = TraiterMediane(k,l,-1,1,q,im);
		else if (*(mThis.ptr(k+1,l-1))>q)
			fini = TraiterMediane(k,l,1,-1,q,im);
		else if (*(mThis.ptr(i+1,j+1))>q)
			fini = TraiterMediane(k,l,1,1,q,im);
		}
	}
	
}

void	ImageInfoCV::SuiviChemin(int c,int i,int j,cv::Mat &im)
{
char	fini;
int	k,l,q;

fini = 0;
if (depth()==CV_32F)
	{
	q=*((float*)mThis.ptr(i,j)+c);
	k=i;
	l=j;
	while (!fini)
		{
		if (*((float*)mThis.ptr(k-1,l)+c)>q)
			fini = TraiterMediane(c,k,l,-1,0,q,im);
		else if (*((float*)mThis.ptr(k+1,l)+c)>q)
			fini = TraiterMediane(c,k,l,1,0,q,im);
		else if (*((float*)mThis.ptr(k,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,0,-1,q,im);
		else if (*((float*)mThis.ptr(k,l+1)+c)>q)
			fini = TraiterMediane(c,k,l,0,1,q,im);
		else if (*((float*)mThis.ptr(k-1,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,-1,-1,q,im);
		else if (*((float*)mThis.ptr(k-1,l+1)+c)>q)
			fini = TraiterMediane(c,k,l,-1,1,q,im);
		else if (*((float*)mThis.ptr(k+1,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,1,-1,q,im);
		else if (*((float*)mThis.ptr(k+1,l+1)+c)>q)
			fini = TraiterMediane(c,k,l,1,1,q,im);
		}
	}
else
	{
	q=*(mThis.ptr(i,j)+c);
	k=i;
	l=j;
	while (!fini)
		{
		if (*(mThis.ptr(k-1,l)+c)>q)
			fini = TraiterMediane(c,k,l,-1,0,q,im);
		else if (*(mThis.ptr(k+1,l)+c)>q)
			fini = TraiterMediane(c,k,l,1,0,q,im);
		else if (*(mThis.ptr(k,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,0,-1,q,im);
		else if (*(mThis.ptr(k,l+1)+c)>q)
			fini = TraiterMediane(c,k,l,0,1,q,im);
		else if (*(mThis.ptr(k-1,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,-1,-1,q,im);
		else if (*(mThis.ptr(k-1,l+1)+c)>q)
			fini = TraiterMediane(c,k,l,-1,1,q,im);
		else if (*(mThis.ptr(k+1,l-1)+c)>q)
			fini = TraiterMediane(c,k,l,1,-1,q,im);
		else if (*(mThis.ptr(i+1,j+1)+c)>q)
			fini = TraiterMediane(c,k,l,1,1,q,im);
		}
	}
	
}


bool	ImageInfoCV::TraiterMediane(int c,int &r,int &s,int dr,int ds,int &q,cv::Mat &im)
{
float *pt=(float *)im.ptr(r+dr,s+ds)+c;



if ((MaxLocal(c,r+dr,s+ds))||(*pt!=0))
	return 1;
else
	{
	*pt = (q+1);
	r = r + dr;
	s = s + ds;
	q = q + 1;
	return 0;
	}
}

bool	ImageInfoCV::TraiterMediane(int &r,int &s,int dr,int ds,int &q,cv::Mat &im)
{
float *pt=(float *)im.ptr(0);

pt =pt+(r+dr)*cols+ds+s;

if ((MaxLocal(r+dr,s+ds))||(*pt!=0))
	return 1;
else
	{
	*pt = (q+1);
	r = r + dr;
	s = s + ds;
	q = q + 1;
	return 0;
	}
}




	
	


	
	

		

