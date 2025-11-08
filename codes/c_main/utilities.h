#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
struct Image{
		double** data;
		int width;
		int height;
		int channels;
};
double* getPixel(struct Image* img,int n, int x, int y);
void* mallocate(size_t x);

void matMul(int n , int k, int m, double** A, double**B,double**C);
void matMulVec(int n , int m,double** A,double* v,double* u);
void vecMulMat(int n , int m,double* v,double** A,double* u);
double dotVec(int n, double* v, double* u);
void normalize(int n,double* A);


void allocMat(int n, int m,double*** A);
void freeMat(int n, int m, double** A);
void printMat(int n, int m, double** A);
void copyMat(int n, int m, double** source, double** dest);

void transposeMatSwap(int n , double** A);
void transposeMat(int n , int m, double** A,double**B);
void orthogonalize(int n, int m, double** A, double**B);
void QR(int n, int m, double** A, double**B,double**C);

double FebErr(int n,int m, double** actual,double** expected);
double RelativeErr(int n,int m, double** actual,double** expected);
double CompresionRatio(int n,int m,double k);
