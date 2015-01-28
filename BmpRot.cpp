#include <stdio.h>
#include "BmpRot.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.14159//Բ���ʺ궨��
#define LENGTH_NAME_BMP 30//bmpͼƬ�ļ�������󳤶�

using namespace std;

int countNum[10]={0};

//��������
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256ɫ��ɫ��
BITMAPINFOHEADER strInfo;

//��ʾλͼ�ļ�ͷ��Ϣ
void showBmpHead(BITMAPFILEHEADER pBmpHead){
	cout<<"λͼ�ļ�ͷ:"<<endl;
	cout<<"�ļ���С:"<<pBmpHead.bfSize<<endl;
	cout<<"������_1:"<<pBmpHead.bfReserved1<<endl;
	cout<<"������_2:"<<pBmpHead.bfReserved2<<endl;
	cout<<"ʵ��λͼ���ݵ�ƫ���ֽ���:"<<pBmpHead.bfOffBits<<endl<<endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
	cout<<"λͼ��Ϣͷ:"<<endl;
	cout<<"�ṹ��ĳ���:"<<pBmpInforHead.biSize<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biWidth<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biHeight<<endl;
	cout<<"biPlanesƽ����:"<<pBmpInforHead.biPlanes<<endl;
	cout<<"biBitCount������ɫλ��:"<<pBmpInforHead.biBitCount<<endl;
	cout<<"ѹ����ʽ:"<<pBmpInforHead.biCompression<<endl;
	cout<<"biSizeImageʵ��λͼ����ռ�õ��ֽ���:"<<pBmpInforHead.biSizeImage<<endl;
	cout<<"X����ֱ���:"<<pBmpInforHead.biXPelsPerMeter<<endl;
	cout<<"Y����ֱ���:"<<pBmpInforHead.biYPelsPerMeter<<endl;
	cout<<"ʹ�õ���ɫ��:"<<pBmpInforHead.biClrUsed<<endl;
	cout<<"��Ҫ��ɫ��:"<<pBmpInforHead.biClrImportant<<endl;
}


int main(){
	char strFile[LENGTH_NAME_BMP];//bmp�ļ���
	IMAGEDATA **imagedata = NULL;//��̬����洢ԭͼƬ��������Ϣ�Ķ�ά����
	IMAGEDATA *imagedataRot = NULL;//��̬����洢��ת���ͼƬ��������Ϣ�Ķ�ά����
	int width,height;//ͼƬ�Ŀ�Ⱥ͸߶�
	int i,j;
	
	//cout<<"��������Ҫ��ȡ���ļ���:"<<endl;
	//cin>>strFile;
	FILE *fpi,*fpw,*fpw_txt,*fout2;
	fpi=fopen("test.bmp","rb");
	
	int widthTemp=0;

	if(fpi != NULL){
		//�ȶ�ȡ�ļ�����
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fpi);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
			return NULL;
		}
		//��ȡbmp�ļ����ļ�ͷ����Ϣͷ
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);
		//showBmpHead(strHead);//��ʾ�ļ�ͷ
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		//showBmpInforHead(strInfo);//��ʾ�ļ���Ϣͷ

		//��ȡ��ɫ��
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
		//ͼ��ÿһ�е��ֽ���������4��������
		width = (width  + 3) / 4 * 4;
		//imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		imagedata= (IMAGEDATA**)malloc(height*width);
		for(i=0;i<height;i++)
			imagedata[i]=(IMAGEDATA*)malloc(width);
		//imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
		
		//fseek(fpi,54,SEEK_SET);
		//����ͼƬ����������
		for(i=height-1;i>=0;i--)
			fread(imagedata[i],sizeof(char),width,fpi);

		/*
		//Ѱ�ҵ������ֵĶ�ά�����ʾ
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
	//����bmpͼƬ
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
	//�����ɫ������
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
	
	//����bmpͼƬ
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
	//�����ɫ������
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
	
	//�ͷ��ڴ�
	//delete[] imagedata;
	//delete[] imagedataRot;
	system("pause");
	return 0;
}