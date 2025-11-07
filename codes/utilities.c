#include "utilities.h"
double* getPixel(struct Image* img, int x, int y)
{
		return &(img->data[img->width*y+x]);
}
void matMul(int n , int k, int m, double** A, double**B,double**C)
{
for (int i = 0; i<n; i++) {
for (int j = 0; j<m; j++) {
C[i][j]=0;
for (int l=0; l<k; l++) {
C[i][j]+=A[i][l]*B[l][j];
}
}
}
}
void matMulVec(int n , int m,double** A,double* v,double* u)
{
		for(int i = 0; i<n;i++)
		{
				u[i]=0;
				for (int j = 0; j<m; j++) {
						u[i]+=A[i][j]*v[j];
				}
		}
}
void* mallocate(size_t x)
{
		void* u = malloc(x);
		if(u==0){printf("not able to allocate\n");abort();}
		return u;
}
void vecMulMat(int n , int m,double* v,double** A,double* u){
		for(int i = 0; i<n;i++)
		{
				u[i]=0;
				for (int j = 0; j<n; j++) {
						u[i]+=A[j][i]*v[j];
				}
		}

};
void QR(int n, int m, double** A, double**B,double**C)
{
		for(int i = 0; i<m;i++)
				for(int j =0;j<m;j++)
				{
						if(i==j)C[i][j]=1;
						else C[i][j]=0;
				}
		for(int i = 0; i<m;i++)
		{
				double x = 0;
				for(int j = 0; j<n;j++)
				{
						x+=A[j][i]*A[j][i];
				}
				x=sqrt(x);
				if(x==0)printf("vector is 0.\n");
				for(int j = 0; j<n;j++)
						B[j][i]=A[j][i]/x;
				for(int j = 0; j<m;j++)
				{
						C[j][i]*=x;
				}

				for(int j = i+1;j<m;j++)
				{
						double dot = 0;
						for(int k = 0; k<n;k++)
								dot+=B[k][i]*A[k][j];
						for(int k = 0; k<n;k++)
								A[k][j]-=dot*B[k][i];
						for(int k = 0; k<m;k++)
						{
								C[k][j]+=dot*C[k][i];
						}


				}
		}
}
void orthogonalize(int n, int m, double** A, double**B)
{
		for(int i = 0; i<m;i++)
		{
				double x = 0;
				for(int j = 0; j<n;j++)
				{
						x+=A[j][i]*A[j][i];
				}
				x=sqrt(x);
				if(x==0)printf("vector is 0.\n");
				for(int j = 0; j<n;j++)
				{
						B[j][i]=A[j][i]/x;
				}

				for(int j = i+1;j<m;j++)
				{
						double dot = 0;
						for(int k = 0; k<n;k++)
								dot+=B[k][i]*A[k][j];
						for(int k = 0; k<n;k++)
								A[k][j]-=dot*B[k][i];

				}
		}
}
void allocMat(int n, int m,double*** A)
{
		
		*A = (double**)mallocate(sizeof(double*)*n);
		double* dat = mallocate(sizeof(double)*n*m);
		for(int i =0 ; i<n;i++)
		{
				(*A)[i] = dat+(i*m);
		}
}
void freeMat(int n, int m, double** A)
{
		
		free(A[0]);
		
		free(A);
}

void printMat(int n, int m, double** A)
{
		printf("[\n");
		for(int i = 0; i<n;i++)
		{
		printf("[ ");
		for(int j = 0; j<m;j++)
		{
				printf(" %lf ,",A[i][j]);
		}
		printf(" ]\n");
		}
}
void transposeMat(int n , int m, double** A,double**B)
{
		for(int i = 0; i<n;i++)
				for(int j = 0; j<m;j++)
						B[j][i]=A[i][j];
}
void transposeMatSwap(int n , double** A)
{
		for(int i = 0; i<n-1;i++)
				for(int j = i+1; j<n;j++)
				{double t=A[i][j];A[i][j]=A[j][i];A[j][i]=t;}
}
double err(int n,int m, double** actual,double** expected)
{
		double e = 0;
		for (int i = 0; i<n; i++) {
				for (int j = 0; j<m; j++) {
						double v = actual[i][j]-expected[i][j];
						if(v<0)v=-v;
						e+=v;
				}
		}
		return sqrt(e);
}
double dotVec(int n, double* v, double* u)
{
		double s=0;
		for(int i = 0; i<n;i++)
		{
				s+=v[i]*u[i];
		}
		return s;
}
void copyMat(int n, int m, double** source, double** dest)
{
		for(int i = 0; i<n;i++)
				for(int j = 0; j<m;j++)
						dest[i][j]=source[i][j];
}
void normalize(int n,double* A)
{
		double s=0;
		for(int i = 0; i<n;i++)
				s+=A[i]*A[i];
		s=sqrt(s);
		for(int i = 0; i<n;i++)
				A[i]/=s;
}
