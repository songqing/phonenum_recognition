#include <stdio.h>
#include "BmpRot.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.14159//圆周率宏定义
#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度

using namespace std;

int countNum[10]={0};

//变量定义
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256色调色板
BITMAPINFOHEADER strInfo;

//显示位图文件头信息
void showBmpHead(BITMAPFILEHEADER pBmpHead){
	cout<<"位图文件头:"<<endl;
	cout<<"文件大小:"<<pBmpHead.bfSize<<endl;
	cout<<"保留字_1:"<<pBmpHead.bfReserved1<<endl;
	cout<<"保留字_2:"<<pBmpHead.bfReserved2<<endl;
	cout<<"实际位图数据的偏移字节数:"<<pBmpHead.bfOffBits<<endl<<endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
	cout<<"位图信息头:"<<endl;
	cout<<"结构体的长度:"<<pBmpInforHead.biSize<<endl;
	cout<<"位图宽:"<<pBmpInforHead.biWidth<<endl;
	cout<<"位图高:"<<pBmpInforHead.biHeight<<endl;
	cout<<"biPlanes平面数:"<<pBmpInforHead.biPlanes<<endl;
	cout<<"biBitCount采用颜色位数:"<<pBmpInforHead.biBitCount<<endl;
	cout<<"压缩方式:"<<pBmpInforHead.biCompression<<endl;
	cout<<"biSizeImage实际位图数据占用的字节数:"<<pBmpInforHead.biSizeImage<<endl;
	cout<<"X方向分辨率:"<<pBmpInforHead.biXPelsPerMeter<<endl;
	cout<<"Y方向分辨率:"<<pBmpInforHead.biYPelsPerMeter<<endl;
	cout<<"使用的颜色数:"<<pBmpInforHead.biClrUsed<<endl;
	cout<<"重要颜色数:"<<pBmpInforHead.biClrImportant<<endl;
}


int main(){
	char strFile[LENGTH_NAME_BMP];//bmp文件名
	IMAGEDATA **imagedata = NULL;//动态分配存储原图片的像素信息的二维数组
	IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组
	int width,height;//图片的宽度和高度
	int i,j;
	
	//cout<<"请输入所要读取的文件名:"<<endl;
	//cin>>strFile;
	FILE *fpi,*fpw,*fpw_txt,*fout2;
	fpi=fopen("test.bmp","rb");
	
	int widthTemp=0;

	if(fpi != NULL){
		//先读取文件类型
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fpi);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
			return NULL;
		}
		//读取bmp文件的文件头和信息头
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);
		//showBmpHead(strHead);//显示文件头
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		//showBmpInforHead(strInfo);//显示文件信息头

		//读取调色板
		for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);
		}

		width = strInfo.biWidth;
		widthTemp=width;
		height = strInfo.biHeight;
		//图像每一行的字节数必须是4的整数倍
		width = (width  + 3) / 4 * 4;
		//imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		imagedata= (IMAGEDATA**)malloc(height*width);
		for(i=0;i<height;i++)
			imagedata[i]=(IMAGEDATA*)malloc(width);
		//imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
		
		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		for(i=height-1;i>=0;i--)
			fread(imagedata[i],sizeof(char),width,fpi);

		/*
		//寻找单个数字的二维数组表示
		fpw_txt=fopen("num/num9/9_1.txt","w");
		fprintf(fpw_txt,"9 11 16\n");
		//fprintf(fpw_txt,"width=10, height=16.\n");
		for(i=height-1-23;i<=height-1-8;i++)
		{
			for(j=97;j<=107;j++)

			{

				fprintf(fpw_txt,"%d\t",*(imagedata+j+i*width));
			}
			fprintf(fpw_txt,"\n");
			
		}
		*/

		fclose(fpi);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}

/*
	//保存bmp图片
	if((fpw=fopen("temp111.bmp","wb"))==NULL)
	{
		cout<<"create the bmp file error!"<<endl;
		return NULL;
	}
	WORD bfType_w=0x4d42;
	fwrite(&bfType_w,1,sizeof(WORD),fpw);
	//fpw +=2;
	fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);
	strInfo.biWidth = widthTemp;
	strInfo.biHeight = height;
	fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);
	//保存调色板数据
	for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
	{
		fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
	}

	//width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
	for(i=height-1;i>=0;i--)
		fwrite(imagedata[i],sizeof(char),width,fpw);
	fclose(fpw);
*/
	
	//保存bmp图片
	if((fpw=fopen("create111111.bmp","wb"))==NULL)
	{
		cout<<"create the bmp file error!"<<endl;
		return NULL;
	}
	WORD bfType_w=0x4d42;
	fwrite(&bfType_w,1,sizeof(WORD),fpw);
	//fpw +=2;
	fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);
	strInfo.biWidth = widthTemp;
	strInfo.biHeight = height;
	fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);
	//保存调色板数据
	for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
	{
		fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
	}

	//width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		imagedata[i][j]=31;
	
	int countTemp=0;
	int k=0;
	int n1=0,n2=0,n3=0,nTemp=0;
	fout2=fopen("num/num2/2_1.txt","r");
	
	
	j=1;
	
	while(countTemp++<11)
	{
		fseek(fout2,0,SEEK_SET);
			fscanf(fout2,"%d %d %d",&n1,&n2,&n3);
			cout<<n1<<" "<<n2<<" "<<n3<<" ";
		for(i=8;i<=23;i++)
		{
			j=(countTemp-1)*10+1;
			for(;j<=130;j++)
			{
				k++;
				if(k%11==0)
				{
					k=0;
					break;
				}
				fscanf(fout2,"%d",&nTemp);
				imagedata[28-1-i][j]=nTemp;
			}
		}
		//i=height-1-23;
	}
	for(i=0;i<height;i++)
		fwrite(imagedata[i],sizeof(char),width,fpw);
	fclose(fpw);
	
	//释放内存
	//delete[] imagedata;
	//delete[] imagedataRot;
	system("pause");
	return 0;
}