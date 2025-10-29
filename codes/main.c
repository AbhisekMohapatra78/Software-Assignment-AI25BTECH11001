#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Image{
		double* data;
		int width;
		int height;
};

int getImage(struct Image* img)
{
		int n;
		unsigned char* data = stbi_load("../figs/einstein.jpg",&img->width,&img->height,&n,0);
		if(data == NULL)
		{
				printf("error in reading img");
				return 1;
		}
		printf("Channels : %d\n",n);
		img->data=malloc(sizeof(double)*img->width*img->height);
		for(int j= 0; j<img->height;j++)
		{
				for (int i =0; i<img->width; i++) {
						img->data[(img->width*j+i)]=data[(img->width*j+i)*n]/255.0;
				}
		}
		stbi_image_free(data);

		return 0;
}

double* getPixel(struct Image* img, int x, int y)
{
		return &(img->data[img->width*y+x]);
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

int main()
{
		struct Image img;
		getImage(&img);
		printImage(&img);
		return  0;
}
