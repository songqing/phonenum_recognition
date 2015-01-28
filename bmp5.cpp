#include <fstream>
#include <iostream>
using namespace std;
typedef unsigned char var8;
typedef unsigned int uvar32;
typedef unsigned short int uvar16;
typedef  struct  { var8 Blue,Green,Red,Reserved;}  Palette;
int main()
{
	uvar32 Size,Offset,Height,Width,Compress,Reserved,
		InfoHeaderSize,Colors,ImportantColors,DataSize,HResolution,VResolution;
	uvar16 Planes,Bit;
	int i,j,k;
	var8 type[2],imdata[118][28];
	Palette Rgbquad;
	ifstream fin;
	ofstream fout_txt;
	fin.open("test.bmp",ios::binary);  
	fout_txt.open("output.txt",ios::out);
	/*�Զ����ƶ���ʽ�򿪸��ļ���һ��Ҫ�����Ƶ�!*/
	if(!fin)
	{
		cout<<"No this file!\n";
		return 1;
	}
	if(!fout_txt)
	{
		cout<<"open file output.txt error!"<<endl;
		return 1;
	}
	fin.read((char*)&type ,2);
	cout<<"file type: "<<type[0]<<type[1]<<endl;
	fout_txt<<"file type: "<<type[0]<<type[1]<<endl;
	/* ���ֽڵ���������ʶ��λͼ������,ƫ����0�����file type: BM*/
	fin.read((char*)&Size ,sizeof(uvar32));
	cout<<"file size: "<<Size<<endl;
	fout_txt<<"file size: "<<Size<<endl;
	/*�����ļ��Ĵ�С��ƫ����2�����file size: 66614*/
	fin.read((char*)&Reserved,sizeof(uvar32));
	cout<<"Reserved dword: "<<Reserved<<endl;
	fout_txt<<"Reserved dword: "<<Reserved<<endl;
	/*��������������Ϊ0�� ƫ����6,���Reserved dword: 0*/
	fin.read((char*)&Offset  ,sizeof(uvar32));
	cout<<"Offset: "<<Offset<<endl;
	fout_txt<<"Offset: "<<Offset<<endl;
	/*���ļ���ʼ��λͼ���ݿ�ʼ֮�������(bitmap data)֮���ƫ����
	,ƫ����10,���Offset: 1078��λͼ����Ϊ256��256,
	65536��λͼ���ݣ�+1078��ƫ������=66614���ļ���С��*/
	fin.read((char*)&InfoHeaderSize,sizeof(uvar32));
	cout<<"Bitmap Info Header Size: "<<InfoHeaderSize<<endl;
	fout_txt<<"Bitmap Info Header Size: "<<InfoHeaderSize<<endl;
	/*λͼ��Ϣͷ(Bitmap Info Header)�ĳ��ȣ�ƫ����14��
	��� Bitmap Info Header Size: 40*/
	fin.read((char*)&Width,sizeof(uvar32));
	cout<<"Bitmap Width: "<<Width<<endl;
	fout_txt<<"Bitmap Width: "<<Width<<endl;
	/*λͼ�Ŀ�ȣ�������Ϊ��λ,ƫ����18��Bitmap Width: 256*/
	fin.read((char*)&Height,sizeof(uvar32));
	cout<<"Bitmap Height: "<<Height<<endl;
	fout_txt<<"Bitmap Height: "<<Height<<endl;
	/*λͼ�ĸ߶ȣ�������Ϊ��λ,�����ֵ��һ��������˵��ͼ���ǵ���ģ�
	�����ֵ��һ����������˵��ͼ��������ġ�ƫ����22��Bitmap Height: 256*/
	fin.read((char*)&Planes,sizeof(uvar16));
	cout<<"Bitmap Planes: "<<Planes<<endl;
	fout_txt<<"Bitmap Planes: "<<Planes<<endl;
	/*λͼ��λ������ע����ֵ������1��,ƫ����26��Bitmap Planes: 1*/
	fin.read((char*)&Bit,sizeof(uvar16));
	cout<<"Bits per Pixel: "<<Bit<<endl;
	fout_txt<<"Bits per Pixel: "<<Bit<<endl;
	/*ÿ�����ص�λ��,ƫ����28,256ɫӦ��8bit,Bits per Pixel: 8*/
	fin.read((char*)&Compress,sizeof(uvar32));
	cout<<"Compresssion Y or N: "<<Compress<<endl;
	fout_txt<<"Compresssion Y or N: "<<Compress<<endl;
	/*ѹ��˵��0Ϊ��ѹ��,ƫ����30,Compresssion Y or N: 0*/
	if(Compress) { fin.close();return 2;}
	fin.read((char*)&DataSize,sizeof(uvar32));
	cout<<"Data Size: "<<DataSize<<endl;
	fout_txt<<"Data Size: "<<DataSize<<endl;
	/*���ֽ�����ʾ��λͼ���ݵĴ�С,ƫ����34,256*256=65536,
	Data Size: 65536*/
	fin.read((char*)&HResolution,sizeof(uvar32));
	cout<<"HResolution: "<<HResolution<<endl;
	fout_txt<<"HResolution: "<<HResolution<<endl;
	/*������/�ױ�ʾ��ˮƽ�ֱ���,ƫ����38,HResolution: 0*/
	fin.read((char*)&VResolution,sizeof(uvar32));
	cout<<"VResolution: "<<VResolution<<endl;
	fout_txt<<"VResolution: "<<VResolution<<endl;
	/*������/�ױ�ʾ�Ĵ�ֱ�ֱ���,ƫ����42,VResolution: 0*/
	fin.read((char*)&Colors,sizeof(uvar32));
	cout<<"Colors: "<<Colors<<endl;
	fout_txt<<"Colors: "<<Colors<<endl;
	/*λͼʹ�õ���ɫ��,256ɫ,ƫ����46,Colors: 256*/
	fin.read((char*)&ImportantColors,sizeof(uvar32));
	cout<<"ImportantColors: "<<ImportantColors<<endl;
	fout_txt<<"ImportantColors: "<<ImportantColors<<endl;
	/*ָ����Ҫ����ɫ�����������ֵ������ɫ��ʱ�����ߵ���0ʱ����
	��ʾ������ɫ��һ����Ҫ��ƫ����50,ImportantColors: 0*/
	for(i=0;i<256;i++)
	{
		fin.read((char*)&Rgbquad,sizeof(Palette));
		cout<<int(Rgbquad.Blue)<<" "<<int(Rgbquad.Green)<<" "<<int(Rgbquad.Red)
			<<"  "<<int(Rgbquad.Reserved)<<endl;
		fout_txt<<int(Rgbquad.Blue)<<" "<<int(Rgbquad.Green)<<" "<<int(Rgbquad.Red)
			<<"  "<<int(Rgbquad.Reserved)<<endl;
	}
	/*��ɫ��淶�����ڵ�ɫ���е�ÿ�������4���ֽ�������������
	����RGB��ֵ��1�ֽ�������ɫ���� ,1�ֽ�������ɫ���� ,1�ֽ����ں�ɫ����
	1�ֽ���������(����Ϊ0),ƫ������54��54��255��4=1074�����ڲ�ɫͼ��R,G��B
	�и��Եķ���������ڰ�ͼ��R=G=B=�Ҷȼ����������� 0 0 0 0��1 1 1 0��
	255 255 255 0*/

	fin.seekg(Offset,ios::beg);
	for( i=27;i>=0;i--)
	{
		for(j=0;j<(Width+3)/4*4;j++)
		{
			fin.read((char*)&imdata[i][j],sizeof(var8));
			
			
		}
	}
		/*ֱ�Ӱ�65536�����صĻҶȶ�������imdata��,����ǰ���ɫ��ĸ�ʽ��������������
		�� ������ɫ�塣����Heightֵ������,ͼ���õ�,��ʼ�����������ͼ�����һ�еĻ�
		��ֵ��*/
	fin.close();
	fout_txt.close();
	system("pause");
	return 0;
		/*�������ݽ����Matlab��imfinfo()��imread()�Ľ�����Ǻϡ�
		�������ݺ󣬾Ϳ��Խ���ֱ��ͼͳ�ƣ�fft2�����ˡ�^&^*/
}
