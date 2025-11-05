#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "power_method.h"
#include "utilities.h"


int getImage(struct Image* img,const char* filename)
{
		int n;
		unsigned char* data = stbi_load(filename,&img->width,&img->height,&n,0);
		if(data == NULL)
		{
				printf("Error in reading Img\n");
				exit(1);
		}
		printf("Image Loaded from %s\nWidth:%d Height:%d Channels : %d\n",filename,img->width,img->height,n);
		img->data=mallocate(sizeof(double)*img->width*img->height);
		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						img->data[(img->width*j+i)]=data[(img->width*j+i)*n]/255.0;
				}
		}
		stbi_image_free(data);

		return 0;
}

int writeImage(struct Image* img,const char* filename)
{
		FILE* f;
		f = fopen(filename,"w");

		fprintf(f,"P2\n");
		fprintf(f,"%d %d\n",img->width,img->height);
		fprintf(f,"255\n");

		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						int x =((*getPixel(img, i, j))*255);
						if(x<0)x=-x;//TODO
						fprintf(f,"%d ",x>255?255:x);
				}
				fprintf(f,"\n");
		}
		

		return 0;
}

void printImage(struct Image* img)
{
		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						printf("%lf ",*getPixel(img, i, j));
				}
				printf("\n");
		}
		
}

void freeImage(struct Image* img)
{
		free(img->data);
}

void getSVD(struct Image* img, double***U,double***E,double***V,int k)
{
		int n = img->height,m=img->width;
		*U = (double**)mallocate(sizeof(double*)*n);
		for(int i = 0; i<n;i++)
				(*U)[i]=(double*)mallocate(sizeof(double)*n);
		for(int i = 0; i<n;i++)
				for(int j = 0; j<n;j++)
				{
						if(i==j)(*U)[i][j]=1.0;
						else (*U)[i][j]=0.0;
				}
		*V = (double**)mallocate(sizeof(double*)*m);
		for(int i = 0; i<m;i++)
				(*V)[i]=(double*)mallocate(sizeof(double)*m);
		for(int i = 0; i<m;i++)
				for(int j = 0; j<m;j++)
				{
						if(i==j)(*V)[i][j]=1;
						else (*V)[i][j]=0;
				}
	
		*E = (double**)mallocate(sizeof(double*)*n);
		for(int i = 0; i<n;i++)
				(*E)[i]=(double*)mallocate(sizeof(double)*m);
		for(int i = 0; i<n;i++)
				for(int j = 0; j<m;j++)
				{
						(*E)[i][j]=*getPixel(img, j, i);
						//if(i==j)(*E)[i][j]=1;
						//else (*E)[i][j]=0;
				}
		
		power_met(img, *U, *E, *V,k);
}

void compressSVD(int n,int m,double** U, double** E,double** V,int k)
{
		for(int i = k;i<(n>m?m:n);i++)
		{
				E[i][i]=0;
		}
}



void setImgFromSVD(struct Image*img,double**U,double**E,double**V,int k)
{
		double** C;
		double** C2;
		C2 = (double**)mallocate(sizeof(double*)*img->height);
		allocMat(img->height,img->width,&C);
		for(int i = 0; i<img->height;i++)
		{
				C2[i]=getPixel(img, 0, i);
		}
		
		matMul(img->height,k,k,U,E,C);
	matMul(img->height,k,img->width,C,V,E);
		printf("Error Frobenius :%lf\n",err(img->height,img->width,C2,E));
		for(int i = 0; i<img->height;i++)
				for(int j = 0; j<img->width;j++)
				{
						*getPixel(img, j, i)=E[i][j];
				}
		freeMat(img->height,img->width,C);
		
		free(C2);


}
void freeSVD(int n,int m,double** U, double** E,double** V)
{
		for(int i = 0; i<n;i++)
		{
				if(U)free(U[i]);
				if(E)free(E[i]);
		}
		for(int j = 0; j<m;j++)
				if(V)free(V[j]);
		free(U);
}

void compress(struct Image* img,int k)
{
		int m = img->width, n=img->height;
		double** U=0,**E=0,**V=0;
		getSVD(img,&U,&E,&V,k);
		//printf("%d\n",U);
		//compress(img, k);
		setImgFromSVD(img,U,E,V,k);
		freeSVD(n,m,U,E,V);
}


int main()
{
		clock_t start = clock();
		struct Image img;
		char file[256];
		printf("Enter image file path (remove spaces in the path and only 255 characters):");
		scanf("%s",file);
		getImage(&img,file);
		printf("Enter output (.pgm) file path (remove spaces in the path and only 255 characters):");
		scanf("%s",file);
		int k;
		printf("enter the value of k:");
		scanf("%d",&k);
		compress(&img,k);
		writeImage(&img, file);
		freeImage(&img);
		printf("Time taken about %lf seconds.\n",((double)(clock()-start))/ CLOCKS_PER_SEC);
		return  0;
}
