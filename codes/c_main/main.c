#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../c_libs/stb_image.h"

#include "methods.h"
#include "utilities.h"

FILE* logging;
int readPBM(struct Image* img,const char* filename);

int getImage(struct Image* img,const char* filename)
{
		int l = strlen(filename);
		if(strcmp(filename+l-4,".pbm")==0)return readPBM(img,filename);
		int n;
		unsigned char* data = stbi_load(filename,&img->width,&img->height,&n,0);
		if(data == NULL)
		{
				printf("Error in reading Img\n");
				fprintf(logging,"Error in reading Img\n");
				exit(1);
		}
		fprintf(logging,"Image Loaded from %s\nWidth:%d Height:%d Channels : %d\n",filename,img->width,img->height,n);
		allocMat(img->channels,img->height*img->width,&img->data);
		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						for(int l = 0;l<img->channels;l++)
						img->data[l][(img->width*j+i)]=data[(img->width*j+i)*n+l]/255.0;
				}
		}
		stbi_image_free(data);

		return 0;
}

int writeImage(struct Image* img,const char* filename)
{
		FILE* f;
		f = fopen(filename,"w");

		if(img->channels==1)fprintf(f,"P2\n");
		else fprintf(f,"P3\n");
		fprintf(f,"%d %d\n",img->width,img->height);
		fprintf(f,"255\n");

		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						for(int l = 0; l<img->channels;l++){
						int x =((*getPixel(img,l, i, j))*255);
						if(x<0)x=-x;//TODO
						fprintf(f,"%d ",x>255?255:x);
						}
				}
				fprintf(f,"\n");
		}
		

		return 0;
}
int readPBM(struct Image* img,const char* filename)
{
		FILE* f;
		f = fopen(filename,"r");
		if(f == NULL)
		{
				printf("Error in reading Img\n");
				fprintf(logging,"Error in reading Img\n");
				exit(1);
		}
		char c[100];
		fgets(c,100,f);
		if(strcmp(c,"P2\n")&&strcmp(c, "P3\n")){fprintf(logging,"this \"%s\" is not supported.\n",c);exit(1);}
		if(strcmp(c, "P3\n")==0)img->channels=3;
		else img->channels=1;
		fscanf(f,"%d %d",&img->width,&img->height);
		fprintf(logging,"Image Loaded from %s\nWidth:%d Height:%d Channels : %d\n",filename,img->width,img->height,1);
		int m;
		fscanf(f,"%d",&m);
		allocMat(img->channels,img->height*img->width,&img->data);
		for(int j= 0; j<img->height;j++)
				for (int i =0; i<img->width; i++) {
						for(int l = 0; l<img->channels;l++){
								int x ;
								fscanf(f,"%d ",&x);
								*getPixel(img,0,i,j)=x/(double)m;
						}
				}
		return 0;
}

void printImage(struct Image* img)
{
		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						printf("%lf ",*getPixel(img,0, i, j));//TODO
				}
				printf("\n");
		}
		
}

void freeImage(struct Image* img)
{
		freeMat(img->channels,img->height*img->width,img->data);
}

void createSVD(struct Image* img, double***U,double***E,double***V,int k)
{
		int n = img->height,m=img->width;
		allocMat(n,n,U);
		allocMat(m,m,V);
		allocMat(n,m,E);

}

void getSVD(struct Image* img,int l, double**U,double**E,double**V,int k)
{
		int n = img->height,m=img->width;
		for(int i = 0; i<n;i++)
				for(int j = 0; j<n;j++)
				{
						if(i==j)U[i][j]=1.0;
						else U[i][j]=0.0;
				}
		for(int i = 0; i<m;i++)
				for(int j = 0; j<m;j++)
				{
						if(i==j)V[i][j]=1;
						else V[i][j]=0;
				}
	
		for(int i = 0; i<n;i++)
				for(int j = 0; j<m;j++)
				{
						E[i][j]=*getPixel(img,l, j, i);
				}
		
		power_met(img, U, E, V,k);
}

void compressSVD(int n,int m,double** U, double** E,double** V,int k)
{
		for(int i = k;i<(n>m?m:n);i++)
		{
				E[i][i]=0;
		}
}



void setImgFromSVD(struct Image*img,int l,double**U,double**E,double**V,int k)
{
		double** C;
		double** C2;
		C2 = (double**)mallocate(sizeof(double*)*img->height);
		allocMat(img->height,img->width,&C);
		for(int i = 0; i<img->height;i++)
		{
				C2[i]=getPixel(img,l, 0, i);
		}
		
		matMul(img->height,k,k,U,E,C);
	matMul(img->height,k,img->width,C,V,E);
		fprintf(logging,"Frobenius Error (Channel: %d) :%lf\n",l+1,FebErr(img->height,img->width,C2,E));
		fprintf(logging,"Relative Error(Channel: %d) :%lf\n",l+1,RelativeErr(img->height,img->width,C2,E));
		fprintf(logging,"Compresion Ratio (Channel: %d) :%lf\n",l+1,CompresionRatio(img->height,img->width,k));
		for(int i = 0; i<img->height;i++)
				for(int j = 0; j<img->width;j++)
				{
						*getPixel(img,l, j, i)=E[i][j];
				}
		freeMat(img->height,img->width,C);
		
		free(C2);


}
void freeSVD(int n,int m,double** U, double** E,double** V)
{
		freeMat(n,n,U);
		freeMat(m,m,V);
		freeMat(n,m,E);
}

void compress(struct Image* img,int k)
{
		int m = img->width, n=img->height;
		double** U=0,**E=0,**V=0;
		createSVD(img, &U, &E, &V, k);
		for(int l = 0; l<img->channels;l++){
				getSVD(img,l,U,E,V,k);
				setImgFromSVD(img,l,U,E,V,k);
		}
		freeSVD(n,m,U,E,V);
}


int main()
{
		char loggingfile[] = "log.txt"; 
		logging = fopen(loggingfile, "a");
		if(logging==NULL) 
		{
		printf("logging in stdout\n");
		logging=stdout;
		}else 
				printf("logging in %s\n",loggingfile);
		clock_t start = clock();
		struct Image img;
		FILE* f;
		f=fopen("init.txt", "r");
		char file[256];
		fscanf(f,"%s",file);
		fscanf(f,"%d",&img.channels);
		if(img.channels!=1&&img.channels!=3)
		{
				fprintf(logging,"channels are not valid(1 = greyscale, 3 = RGB). Falling back to greyscale.\n");
				printf("channels are not valid(1 = greyscale, 3 = RGB). Falling back to greyscale, rewrite it.\n");
				img.channels=1;
				
		}

		getImage(&img,file);
		fscanf(f,"%s",file);
		int k;
		fscanf(f,"%d",&k);
		int ml = img.width>img.height?img.height:img.width;
		if(k>ml)
		{
				fprintf(logging,"k>%d.Mapping k to %d.\n",ml,ml);
				
		}
		fprintf(logging,"Values extracted from the init.txt file.\n");
		fprintf(logging,"k = %d\n",k);
		compress(&img,k);
		writeImage(&img, file);
		freeImage(&img);
		fprintf(logging,"Time taken about %lf seconds.\n",((double)(clock()-start))/ CLOCKS_PER_SEC);
		fclose(f);
		return  0;
}
