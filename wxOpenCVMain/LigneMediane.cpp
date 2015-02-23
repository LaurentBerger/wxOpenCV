#include "ImageInfo.h"

ImageInfoCV		*ImageInfoCV::LigneMediane (ImageInfoCV *im1,Parametre &paramOCV)
{

if (depth()!=CV_32F && depth()!=CV_8U)
	{
	ImageInfoCV	*imRes = new ImageInfoCV();
	return imRes;
	}
long			i,j;
unsigned char 	*ptc;

ImageInfoCV	*imRes=new ImageInfoCV(rows,cols,(CV_32F<<3)&(channels()<-1));
float *ptf;
for (int k=0;k<channels();k++)
	for (int i=0;i<rows;i++)
		{
		ptf=(float*)imRes->ptr(k,i);
		for (int j=0;j<cols;j++,ptf++)
			*ptf=0;
		}

for (int c=0;c<channels();c++)
	if (depth()==CV_32F)
		{
		for (int i=1;i<rows-1;i++)
			{
			ptf =(float*)imRes->ptr(i)+1*channels()+c;
			for (int j=1;j<cols-1;j++,ptf+=channels())
				if ((*((float*)ptr(i,j)+c)!=0)&&(*ptf==0))
					{
					float 	k= *((float*)ptr(i,j)+c);
					if ((*((float*)ptr(i+1,j+1)+c)==k)&&(*((float*)ptr(i+1,j)+c)==k-1)&&
						(*((float*)ptr(i,j-1)+c)==k-1))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,*imRes);
							}
						if (!MaxLocal(c,i+1,j+1))
							{
							ptf[(cols+1)*channels()] = *((float*)ptr(i+1,j+1)+c);
							SuiviChemin(c,i+1,j+1,*imRes);
							}
						}
					else if ((*((float*)ptr(i+1,j-1)+c)==k)&&(*((float*)ptr(i,j-1)+c)==k-1)&&
								(*((float*)ptr(i+1,j)+c)==k-1))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,*imRes);
							}
						if (!MaxLocal(c,i+1,j-1))
							{
							ptf[(cols-1)*channels()] = *((float*)ptr(i+1,j-1)+c);
							SuiviChemin(c,i+1,j-1,*imRes);
							}
						}
					if (MaxLocal(c,i,j))
						*ptf = k;
					else
						{
						long 	z=0;
				
						if (k==*((float*)ptr(i+1,j)+c))
							z++;
						if (k==*((float*)ptr(i-1,j)+c))
							z++;
						if (k==*((float*)ptr(i,j-1)+c))
							z++;
						if (k==*((float*)ptr(i,j+1)+c))
							z++;
						if (z==4)
							{
							*ptc = k;
							if ((!MaxLocal(c,i-1,j-1))&&(k+1==*((float*)ptr(i-1,j-1)+c)))
								{
								ptf[(-cols-1)*channels()] = *((float*)ptr(i-1,j-1)+c);
								SuiviChemin(c,i-1,j-1,*imRes);
								}
							if ((!MaxLocal(c,i-1,j+1))&&(k+1==*((float*)ptr(i-1,j+1)+c)))
								{
								ptf[(-cols+1)*channels()] = *((float*)ptr(i-1,j+1)+c);
								SuiviChemin(c,i-1,j+1,*imRes);
								}
							if ((!MaxLocal(c,i+1,j-1))&&(k+1==*((float*)ptr(i+1,j-1)+c)))
								{
								ptf[(cols-1)*channels()] = *((float*)ptr(i+1,j-1)+c);
								SuiviChemin(c,i+1,j-1,*imRes);
								}
							if ((!MaxLocal(c,i+1,j+1))&&(k+1==*((float*)ptr(i+1,j+1)+c)))
								{
								ptf[(cols+1)*channels()] = *((float*)ptr(i+1,j+1)+c);
								SuiviChemin(c,i+1,j+1,*imRes);
								}
							}
						else if (z==3)
							{
							*ptc = (unsigned char)k;
							SuiviChemin(c,i,j,*imRes);
							}
						}
					}
				}
			
		}
	else
		{
		for (int i=1;i<rows-1;i++)
			{
			ptf =(float*)imRes->ptr(i)+1*c;
			for (int j=1;j<cols-1;j++,ptf+=c)
				if ((*((float*)ptr(i,j)+c)!=0)&&(*ptf==0))
					{
					float 	k= *(ptr(i,j)+c);
					if ((*(ptr(i+1,j+1)+c)==k)&&(*(ptr(i+1,j)+c)==k-1)&&
						(*(ptr(i,j-1)+c)==k-1))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,*imRes);
							}
						if (!MaxLocal(c,i+1,j+1))
							{
							ptf[(cols+1)*channels()] = *(ptr(i+1,j+1)+c);
							SuiviChemin(c,i+1,j+1,*imRes);
							}
						}
					else if ((*(ptr(i+1,j-1)+c)==k)&&(*(ptr(i,j-1)+c)==k-1)&&
								(*(ptr(i+1,j)+c)==k-1))
						{
						if (!MaxLocal(c,i,j))
							{
							*ptf = k;
							SuiviChemin(c,i,j,*imRes);
							}
						if (!MaxLocal(c,i+1,j-1))
							{
							ptf[(cols-1)*channels()] = *(ptr(i+1,j-1)+c);
							SuiviChemin(c,i+1,j-1,*imRes);
							}
						}
					if (MaxLocal(c,i,j))
						*ptf = k;
					else
						{
						long 	z=0;
				
						if (k==*(ptr(i+1,j)+c))
							z++;
						if (k==*(ptr(i-1,j)+c))
							z++;
						if (k==*(ptr(i,j-1)+c))
							z++;
						if (k==*(ptr(i,j+1)+c))
							z++;
						if (z==4)
							{
							*ptc = k;
							if ((!MaxLocal(c,i-1,j-1))&&(k+1==*(ptr(i-1,j-1)+c)))
								{
								ptf[(-cols-1)*channels()] = *(ptr(i-1,j-1)+c);
								SuiviChemin(c,i-1,j-1,*imRes);
								}
							if ((!MaxLocal(c,i-1,j+1))&&(k+1==*(ptr(i-1,j+1)+c)))
								{
								ptf[(-cols+1)*channels()] = *(ptr(i-1,j+1)+c);
								SuiviChemin(c,i-1,j+1,*imRes);
								}
							if ((!MaxLocal(c,i+1,j-1))&&(k+1==*(ptr(i+1,j-1)+c)))
								{
								ptf[(cols-1)*channels()] = *(ptr(i+1,j-1)+c);
								SuiviChemin(c,i+1,j-1,*imRes);
								}
							if ((!MaxLocal(c,i+1,j+1))&&(k+1==*(ptr(i+1,j+1)+c)))
								{
								ptf[(cols+1)*channels()] = *((float*)ptr(i+1,j+1)+c);
								SuiviChemin(c,i+1,j+1,*imRes);
								}
							}
						else if (z==3)
							{
							*ptc = (unsigned char)k;
							SuiviChemin(c,i,j,*imRes);
							}
						}
					}
				}
			
		}
return imRes;
}


bool ImageInfoCV::MaxLocal(int c,int i,int j)
{
if (depth()==CV_32F)
	{
	float	k=*((float*)ptr(i,j)+c);

	if ((k>=*((float*)ptr(i-1,j-1)+c))&&((k>=*((float*)ptr(i-1,j)+c))&&
		(k>=*((float*)ptr(i-1,j+1)+c))&&(k>=*((float*)ptr(i,j-1)+c))&&
		(k>=*((float*)ptr(i,j+1)+c))&&(k>=*((float*)ptr(i+1,j-1)+c))&&
		(k>=*((float*)ptr(i+1,j)+c))&&(k>=*((float*)ptr(i+1,j+1)+c))))
		return 1;
	else
		return 0;
	}
else if (depth()==CV_8U)
	{
	int	k=*(ptr(i,j)+c);

	if ((k>=*(ptr(i-1,j-1)+c))&&((k>=*(ptr(i-1,j)+c))&&
		(k>=*(ptr(i-1,j+1)+c))&&(k>=*(ptr(i,j-1)+c))&&
		(k>=*(ptr(i,j+1)+c))&&(k>=*(ptr(i+1,j-1)+c))&&
		(k>=*(ptr(i+1,j)+c))&&(k>=*(ptr(i+1,j+1)+c))))
		return 1;
	else
		return 0;
	}return 0;
}

bool ImageInfoCV::MaxLocal(int i,int j)
{
if (depth()==CV_32F)
	{
	float	k=*((float*)ptr(i,j));

	if ((k>=*((float*)ptr(i-1,j-1)))&&((k>=*((float*)ptr(i-1,j)))&&
		(k>=*((float*)ptr(i-1,j+1)))&&(k>=*((float*)ptr(i,j-1)))&&
		(k>=*((float*)ptr(i,j+1)))&&(k>=*((float*)ptr(i+1,j-1)))&&
		(k>=*((float*)ptr(i+1,j)))&&(k>=*((float*)ptr(i+1,j+1)))))
		return 1;
	else
		return 0;
	}
else if (depth()==CV_8U)
	{
	int	k=*(ptr(i,j));

	if ((k>=*(ptr(i-1,j-1)))&&((k>=*(ptr(i-1,j)))&&
		(k>=*(ptr(i-1,j+1)))&&(k>=*(ptr(i,j-1)))&&
		(k>=*(ptr(i,j+1)))&&(k>=*(ptr(i+1,j-1)))&&
		(k>=*(ptr(i+1,j)))&&(k>=*(ptr(i+1,j+1)))))
		return 1;
	else
		return 0;
	}return 0;
}

	
	
void	ImageInfoCV::SuiviChemin(int i,int j,ImageInfoCV &im)
{
char	fini;
int	k,l,q;

fini = 0;
if (depth()==CV_32F)
	{
	q=*((float*)ptr(i,j));
	k=i;
	l=j;
	while (!fini)
		{
		if (*((float*)ptr(k-1,l))==q+1)
			fini = TraiterMediane(k,l,-1,0,q,im);
		else if (*((float*)ptr(k+1,l))==q+1)
			fini = TraiterMediane(k,l,1,0,q,im);
		else if (*((float*)ptr(k,l-1))==q+1)
			fini = TraiterMediane(k,l,0,-1,q,im);
		else if (*((float*)ptr(k,l+1))==q+1)
			fini = TraiterMediane(k,l,0,1,q,im);
		else if (*((float*)ptr(k-1,l-1))==q+1)
			fini = TraiterMediane(k,l,-1,-1,q,im);
		else if (*((float*)ptr(k-1,l+1))==q+1)
			fini = TraiterMediane(k,l,-1,1,q,im);
		else if (*((float*)ptr(k+1,l-1))==q+1)
			fini = TraiterMediane(k,l,1,-1,q,im);
		else if (*((float*)ptr(i+1,j+1))==q+1)
			fini = TraiterMediane(k,l,1,1,q,im);
		}
	}
else
	{
	q=*(ptr(i,j));
	k=i;
	l=j;
	while (!fini)
		{
		if (*(ptr(k-1,l))==q+1)
			fini = TraiterMediane(k,l,-1,0,q,im);
		else if (*(ptr(k+1,l))==q+1)
			fini = TraiterMediane(k,l,1,0,q,im);
		else if (*(ptr(k,l-1))==q+1)
			fini = TraiterMediane(k,l,0,-1,q,im);
		else if (*(ptr(k,l+1))==q+1)
			fini = TraiterMediane(k,l,0,1,q,im);
		else if (*(ptr(k-1,l-1))==q+1)
			fini = TraiterMediane(k,l,-1,-1,q,im);
		else if (*(ptr(k-1,l+1))==q+1)
			fini = TraiterMediane(k,l,-1,1,q,im);
		else if (*(ptr(k+1,l-1))==q+1)
			fini = TraiterMediane(k,l,1,-1,q,im);
		else if (*(ptr(i+1,j+1))==q+1)
			fini = TraiterMediane(k,l,1,1,q,im);
		}
	}
	
}

void	ImageInfoCV::SuiviChemin(int c,int i,int j,ImageInfoCV &im)
{
char	fini;
int	k,l,q;

fini = 0;
if (depth()==CV_32F)
	{
	q=*((float*)ptr(i,j)+c);
	k=i;
	l=j;
	while (!fini)
		{
		if (*((float*)ptr(k-1,l)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,0,q,im);
		else if (*((float*)ptr(k+1,l)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,0,q,im);
		else if (*((float*)ptr(k,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,0,-1,q,im);
		else if (*((float*)ptr(k,l+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,0,1,q,im);
		else if (*((float*)ptr(k-1,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,-1,q,im);
		else if (*((float*)ptr(k-1,l+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,1,q,im);
		else if (*((float*)ptr(k+1,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,-1,q,im);
		else if (*((float*)ptr(i+1,j+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,1,q,im);
		}
	}
else
	{
	q=*(ptr(i,j)+c);
	k=i;
	l=j;
	while (!fini)
		{
		if (*(ptr(k-1,l)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,0,q,im);
		else if (*(ptr(k+1,l)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,0,q,im);
		else if (*(ptr(k,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,0,-1,q,im);
		else if (*(ptr(k,l+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,0,1,q,im);
		else if (*(ptr(k-1,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,-1,q,im);
		else if (*(ptr(k-1,l+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,-1,1,q,im);
		else if (*(ptr(k+1,l-1)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,-1,q,im);
		else if (*(ptr(i+1,j+1)+c)==q+1)
			fini = TraiterMediane(c,k,l,1,1,q,im);
		}
	}
	
}


bool	ImageInfoCV::TraiterMediane(int c,int &r,int &s,int dr,int ds,int &q,ImageInfoCV &im)
{
float *pt=(float *)im.ptr(0);

pt =pt+(r+dr)*cols+ds+s;

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

bool	ImageInfoCV::TraiterMediane(int &r,int &s,int dr,int ds,int &q,ImageInfoCV &im)
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




	
	


	
	

		

