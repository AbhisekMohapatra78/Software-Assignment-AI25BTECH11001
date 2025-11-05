#include"power_method.h"
#include "utilities.h"

void power_met(struct Image* img,double** u,double** e,double** v,int k)
{
		double** A;
		allocMat(img->width, img->width, &A);
		for (int i = 0; i<img->width; i++) {
		for (int j = 0; j<img->width; j++) {
		A[i][j]=0;
		for (int l=0; l<img->height; l++) {
				A[i][j]+=e[l][j]*e[l][i];
		}
		}
		}

		double** W0;
		double** V;
		allocMat(img->width, k, &W0);
		allocMat(img->width, k, &V);
		for(int i = 0; i<img->width;i++)
				for(int j = 0; j<k;j++)
				{
						W0[i][j]=((double)rand())/ RAND_MAX;
				}
		
		orthogonalize(img->width, k, W0, V);
		for(int _ = 0; _<40;_++){
				matMul(img->width, img->width, k, A, V, W0);
				orthogonalize(img->width, k, W0, V);
		}
		transposeMat(img->width, k, V, v);
		freeMat(img->width, k, W0);

		double** W;
		allocMat(img->height, k, &W);
		matMul(img->height, img->width, k, e, V, W);
		QR(img->height, k, W, u,e);
		freeMat(img->height, k, W);
		for(int i = 0; i<k;i++)
				for(int j = 0; j<k;j++)
						if(i!=j)e[i][j]=0;
	//	printMat(k,k,e);
}




