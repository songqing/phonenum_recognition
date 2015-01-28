#include <stdio.h>
#include "BmpRot.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "time.h"
using namespace std;

#define random(x) (rand()%x)
#define PI 3.14159//圆周率宏定义
//#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度
int phonenum=0;
int countNum[10]={0};
int posPrev=0;

//变量定义
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256色调色板
BITMAPINFOHEADER strInfo;

//char strFile[LENGTH_NAME_BMP];//bmp文件名
int width,height;//图片的宽度和高度
int widthTemp;
IMAGEDATA **imagedata=NULL;//动态分配存储原图片的像素信息的二维数组
IMAGEDATA **imagedataTemp=NULL;//动态分配存储原图片的像素信息的二维数组

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
void getNumArray()
{
	FILE* fpw_txt;
	int i,j;
	//寻找单个数字的二维数组表示
	fpw_txt=fopen("num/num1/1_5.txt","w");
	//fprintf(fpw_txt,"9 11 16\n");
	//fprintf(fpw_txt,"width=10, height=16.\n");
	for(i=8;i<=23;i++)
	{
		for(j=1;j<=12;j++)
		{
			fprintf(fpw_txt,"%d\t",imagedata[i][j]);
		}
		fprintf(fpw_txt,"\n");

	}
	fclose(fpw_txt);
}
int readBmp(const char* filename)
{
	//cout<<"请输入所要读取的文件名:"<<endl;
	//cin>>strFile;
	FILE *fpi;
	int i,j;
	fpi=fopen(filename,"rb");

	widthTemp=0;

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
		imagedata= (IMAGEDATA**)malloc(height*width*sizeof(IMAGEDATA));
		for(i=0;i<height;i++)
			imagedata[i]=(IMAGEDATA*)malloc(width*sizeof(IMAGEDATA));
		

		//temp imagedata array for 28 * 28
		imagedataTemp= (IMAGEDATA**)malloc(28*28*sizeof(IMAGEDATA));
		for(i=0;i<28;i++)
			imagedataTemp[i]=(IMAGEDATA*)malloc(28*sizeof(IMAGEDATA));

		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		for(i=height-1;i>=0;i--)
			fread(imagedata[i],sizeof(char),width,fpi);

		//getNumArray();  //info of num
		fclose(fpi);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}
	return 1;

}

int readSmallBmp()
{
	//cout<<"请输入所要读取的文件名:"<<endl;
	//cin>>strFile;
	FILE *fpi;
	int i,j;
	fpi=fopen("small.bmp","rb");

	widthTemp=0;

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
		showBmpHead(strHead);//显示文件头
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		showBmpInforHead(strInfo);//显示文件信息头

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
		imagedata= (IMAGEDATA**)malloc(height*width*sizeof(IMAGEDATA));
		for(i=0;i<height;i++)
			imagedata[i]=(IMAGEDATA*)malloc(width*sizeof(IMAGEDATA));
		

		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		for(i=height-1;i>=0;i--)
			fread(imagedata[i],sizeof(char),width,fpi);

		//getNumArray();  //info of num
		fclose(fpi);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}
	return 1;

}

int saveBmp()
{
	FILE* fpw;
	int i;

	//保存bmp图片
	if((fpw=fopen("small11111.bmp","wb"))==NULL)
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
	return 1;
}
int saveBmpFinal()
{
	FILE* fpw, *fout2;
	int i,j;
	//保存bmp图片
	const char* filecreate="result/num0000.bmp";
	char filecreateTemp[]="result/num0000.bmp";
	int phonenumTemp=phonenum++;
	i=0;
	while(phonenumTemp>0)
	{
		filecreateTemp[13-i++]=phonenumTemp%10 + '0';
		phonenumTemp/=10;
	}
	filecreate=filecreateTemp;
	if((fpw=fopen(filecreate,"wb"))==NULL)  //file increase by 1
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
	int nTemp=0;
	int lineNum=0;
	int randnum1,randnum2;
	srand((int)time(0));
	const char* fileNumName="num/num3/3_1.txt";
	char filenameTemp[]="num/num3/3_1.txt";

	while(countTemp++<11)
	{
		//fseek(fout2,0,SEEK_SET);
		//fscanf(fout2,"%d %d %d",&n1,&n2,&n3);
		
		randnum1=random(10);
		randnum2=random(4)+1;
		if(countTemp==1)
			randnum1=1;
			
		filenameTemp[7]=filenameTemp[9]=randnum1+'0';
		filenameTemp[11]=randnum2+'0';

		fileNumName=filenameTemp;
		fout2=fopen(fileNumName,"r");   //create two random numbers
		j=1;
		
		fscanf(fout2,"%d",&lineNum);

		for(i=8;i<=23;i++)
		{
			j=(countTemp-1)*10+1;
			for(;j<=130;j++)
			{
				k++;

				if(k%(lineNum+1)==0)   //k % num : num needs to change, store in the file
				{
					k=0;
					cout<<endl;
					break;
				}
				fscanf(fout2,"%d",&nTemp);
				//if(i==8)cout<<nTemp<<"\t";
				imagedata[i][j]=nTemp;
			}
		}
		//i=height-1-23;
		fclose(fout2);
	}
	for(i=height-1;i>=0;i--)
		fwrite(imagedata[i],sizeof(char),width,fpw);
	fclose(fpw);
	//fclose(fout2);
	return 1;
}

int createBmp()
{
	FILE* fpw, *fout2;
	int i,j;
	//保存bmp图片
	if((fpw=fopen("new11.bmp","wb"))==NULL)
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

	fout2=fopen("num/num3/3_1.txt","r");
	j=1;

	while(countTemp++<11)
	{
		fseek(fout2,0,SEEK_SET);
		//fscanf(fout2,"%d %d %d",&n1,&n2,&n3);

		for(i=8;i<=23;i++)
		{
			j=(countTemp-1)*10+1;
			for(;j<=130;j++)
			{
				k++;
				if(k%12==0)
				{
					k=0;
					break;
				}
				fscanf(fout2,"%d",&nTemp);
				imagedata[i][j]=nTemp;
			}
		}
		//i=height-1-23;
	}
	for(i=height-1;i>=0;i--)
		fwrite(imagedata[i],sizeof(char),width,fpw);
	fclose(fpw);
	fclose(fout2);
	return 1;
}

int createBmpRandom()
{
	FILE* fpw, *fout2,*fpPos;
	int i,j;
	//保存bmp图片
	const char* filecreate="result/num0000.bmp";
	char filecreateTemp[]="result/num0000.bmp";
	int phonenumTemp=phonenum++;
	i=0;
	while(phonenumTemp>0)
	{
		filecreateTemp[13-i++]=phonenumTemp%10 + '0';
		phonenumTemp/=10;
	}
	filecreate=filecreateTemp;
	if((fpw=fopen(filecreate,"wb"))==NULL)  //file increase by 1
	{
		cout<<"create the bmp file error!"<<endl;
		return NULL;
	}
	//
	//store the each num pos into a txt
	const char* filenumPos="pos/num0000.txt";
	char filenumPosTemp[]="pos/num0000.txt";
	phonenumTemp=phonenum-1;
	i=0;
	while(phonenumTemp>0)
	{
		filenumPosTemp[10-i++]=phonenumTemp%10 + '0';
		phonenumTemp/=10;
	}
	filenumPos=filenumPosTemp;
	if((fpPos=fopen(filenumPos,"w"))==NULL)  //file increase by 1
	{
		cout<<"create the txt file error!"<<endl;
		return NULL;
	}

	//write file head
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
	
	//num and pos is random
	int countTemp=0;
	int k=0;
	int nTemp=0;
	int lineNum=0;
	int randnum1,randnum2,randnumrow,randnumcol;
	
	const char* fileNumName="num/num3/3_1.txt";
	char filenameTemp[]="num/num3/3_1.txt";
	posPrev=0;
	while(countTemp++<11)
	{
		//fseek(fout2,0,SEEK_SET);
		//fscanf(fout2,"%d %d %d",&n1,&n2,&n3);
		
		//num is random
		randnum1=random(10);
		randnum2=random(4)+1;
		if(countTemp==1)
			randnum1=1;
			
		filenameTemp[7]=filenameTemp[9]=randnum1+'0';
		filenameTemp[11]=randnum2+'0';

		fileNumName=filenameTemp;
		fout2=fopen(fileNumName,"r");   //create two random numbers
		j=1;
		
		
		randnumcol=random(6);
		
		fscanf(fout2,"%d",&lineNum);

		randnumrow=random(4);
		//for(i=6+randnumrow;i<=21+randnumrow;i++)   //row num is random
		for(i=8;i<=23;i++)   //row num is random
		{
			if(countTemp==1)
				j=6;
			else j=posPrev+randnumcol-3;

			//j=(countTemp-1)*10+randnumcol-1;   //column num is random
			//j=(countTemp-1)*10+1;
			for(;j<=140;j++)
			{
				k++;

				if(k%(lineNum+1)==0)   //k % num : num needs to change, store in the file
				{
					k=0;
					
					//cout<<endl;
					break;
				}
				fscanf(fout2,"%d",&nTemp);
				//if(i==8)cout<<nTemp<<"\t";
				if((int)imagedata[i][j]!=31)
					if(nTemp-(30-(int)(imagedata[i][j]))<0)
						imagedata[i][j]=0;
					else
						imagedata[i][j]=nTemp-(30-(int)(imagedata[i][j]));
				else
					imagedata[i][j]=nTemp;
			}
		}

		//write the num's center pos into the txt file
		fprintf(fpPos,"%d\t%d\t",randnum1,(j-1-lineNum/2));

		posPrev=j;
		//i=height-1-23;
		
		fclose(fout2);
	}
	

	for(i=height-1;i>=0;i--)
		fwrite(imagedata[i],sizeof(char),width,fpw);
	fclose(fpw);
	fclose(fpPos);
	if(posPrev<105 || posPrev>115)
	{
		phonenum--;
	}
	//fclose(fout2);
	return 1;
}

int createSingleNum(int curPic)
{
	FILE* fpw, *fout2,*fpPos,*fplarge;
	int i,j;
	int phonenumTemp=curPic;

	//store the each num pos into a txt
	const char* filenumPos="pos/num0000.txt";
	char filenumPosTemp[]="pos/num0000.txt";
	//phonenumTemp=phonenum-1;
	i=0;
	while(phonenumTemp>0)
	{
		filenumPosTemp[10-i++]=phonenumTemp%10 + '0';
		phonenumTemp/=10;
	}
	filenumPos=filenumPosTemp;
	if((fpPos=fopen(filenumPos,"r"))==NULL)  //file increase by 1
	{
		cout<<"open the txt file error!"<<endl;
		return NULL;
	}

	//open the matching bmg file
	//
	const char* filenumPic="result/num0000.bmp";
	char filenumPicTemp[]="result/num0000.bmp";
	phonenumTemp=curPic;
	i=0;
	while(phonenumTemp>0)
	{
		filenumPicTemp[13-i++]=phonenumTemp%10 + '0';
		phonenumTemp/=10;
	}
	filenumPic=filenumPicTemp;
	
	if((fplarge=fopen(filenumPic,"rb"))==NULL)  //file increase by 1
	{
		cout<<"open the bmp file error!"<<endl;
		return NULL;
	}
	widthTemp=0;

	if(fplarge != NULL){
		//先读取文件类型
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fplarge);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
			return NULL;
		}
		//读取bmp文件的文件头和信息头
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fplarge);
		//showBmpHead(strHead);//显示文件头
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fplarge);
		//showBmpInforHead(strInfo);//显示文件信息头

		//读取调色板
		for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fplarge);
			fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fplarge);
			fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fplarge);
			fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fplarge);
		}

		width = strInfo.biWidth;
		widthTemp=width;
		height = strInfo.biHeight;
		//图像每一行的字节数必须是4的整数倍
		width = (width  + 3) / 4 * 4;
		
		
		//for(i=0;i<height;i++)
			//free(imagedata[i]);
		//free(imagedata);
		//imagedata= (IMAGEDATA**)malloc(height*width*sizeof(IMAGEDATA));
		//for(i=0;i<height;i++)
			//imagedata[i]=(IMAGEDATA*)malloc(width*sizeof(IMAGEDATA));
		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		for(i=height-1;i>=0;i--)
			fread(imagedata[i],sizeof(char),width,fplarge);

		//getNumArray();  //info of num
		fclose(fplarge);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}
	
	//readBmp(filenumPic);  //get the 2D array imagedata

	int iNum;
	const char* filecreate="numpic/num0/0_0000.bmp";
	char filecreateTemp[]="numpic/num0/0_0000.bmp";
	int numvalue=0,numpos=0;
	int linesmall,rowsmall;
	for(iNum=0;iNum<11;iNum++)  //get the 11 num's real value and pos
	{
		fscanf(fpPos,"%d\t%d\t",&numvalue,&numpos);

		//保存bmp图片
		i=0;
		phonenumTemp=countNum[numvalue]++;
		while(phonenumTemp>0)
		{
			filecreateTemp[17-i++]=phonenumTemp%10 + '0';
			phonenumTemp/=10;
		}
		filecreateTemp[10]=filecreateTemp[12]=numvalue+'0';
		filecreate=filecreateTemp;
		if((fpw=fopen(filecreate,"wb"))==NULL)  //file increase by 1
		{
			cout<<"create the bmp file error!"<<endl;
			return NULL;
		}
		//
		//write file head
		WORD bfType_w=0x4d42;
		fwrite(&bfType_w,1,sizeof(WORD),fpw);
		//fpw +=2;
		fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);
		strInfo.biWidth = 28;
		strInfo.biHeight = 28;
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
		
		//initialize the temp imagedata array
		for(i=0;i<28;i++)
			for(j=0;j<28;j++)
				imagedataTemp[i][j]=31;

		//put data into the new bmp file from the big bmp file
		i=6;j=5;
		for(linesmall=8;linesmall<=23;linesmall++)
		{
			j=5;
			for(rowsmall=numpos-7;rowsmall<=numpos+7;rowsmall++)
			{
				imagedataTemp[i][j]=imagedata[linesmall][rowsmall];
				j++;
			}
			i++;
		}

		for(i=27;i>=0;i--)
			fwrite(imagedataTemp[i],sizeof(char),28,fpw);
		
		fclose(fpw);
	}

	fclose(fpPos);
	//fclose(fout2);
	return 1;
}

int main(){
	srand((int)time(0));
	readBmp("model.bmp");
	//readSmallBmp();
	//saveBmp();
	int i;
	/*
	for(i=0;i<10;i++)
		saveBmpFinal();  //pos is not random
	*/
	for(i=0;i<10000;i++)
		createBmpRandom();  //pos is random
	//
	//cout<<phonenum<<endl;
	for(i=0;i<phonenum;i++)
		createSingleNum(i);
	//释放内存
	//delete[] imagedata;

	//system("pause");
	return 0;
}