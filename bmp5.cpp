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
	/*以二进制读方式打开该文件，一定要二进制的!*/
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
	/* 两字节的内容用来识别位图的类型,偏移量0，结果file type: BM*/
	fin.read((char*)&Size ,sizeof(uvar32));
	cout<<"file size: "<<Size<<endl;
	fout_txt<<"file size: "<<Size<<endl;
	/*整个文件的大小，偏移量2，结果file size: 66614*/
	fin.read((char*)&Reserved,sizeof(uvar32));
	cout<<"Reserved dword: "<<Reserved<<endl;
	fout_txt<<"Reserved dword: "<<Reserved<<endl;
	/*保留，必须设置为0， 偏移量6,结果Reserved dword: 0*/
	fin.read((char*)&Offset  ,sizeof(uvar32));
	cout<<"Offset: "<<Offset<<endl;
	fout_txt<<"Offset: "<<Offset<<endl;
	/*从文件开始到位图数据开始之间的数据(bitmap data)之间的偏移量
	,偏移量10,结果Offset: 1078。位图数据为256＊256,
	65536（位图数据）+1078（偏移量）=66614（文件大小）*/
	fin.read((char*)&InfoHeaderSize,sizeof(uvar32));
	cout<<"Bitmap Info Header Size: "<<InfoHeaderSize<<endl;
	fout_txt<<"Bitmap Info Header Size: "<<InfoHeaderSize<<endl;
	/*位图信息头(Bitmap Info Header)的长度，偏移量14，
	结果 Bitmap Info Header Size: 40*/
	fin.read((char*)&Width,sizeof(uvar32));
	cout<<"Bitmap Width: "<<Width<<endl;
	fout_txt<<"Bitmap Width: "<<Width<<endl;
	/*位图的宽度，以象素为单位,偏移量18，Bitmap Width: 256*/
	fin.read((char*)&Height,sizeof(uvar32));
	cout<<"Bitmap Height: "<<Height<<endl;
	fout_txt<<"Bitmap Height: "<<Height<<endl;
	/*位图的高度，以象素为单位,如果该值是一个正数，说明图像是倒向的，
	如果该值是一个负数，则说明图像是正向的。偏移量22，Bitmap Height: 256*/
	fin.read((char*)&Planes,sizeof(uvar16));
	cout<<"Bitmap Planes: "<<Planes<<endl;
	fout_txt<<"Bitmap Planes: "<<Planes<<endl;
	/*位图的位面数（注：该值将总是1）,偏移量26，Bitmap Planes: 1*/
	fin.read((char*)&Bit,sizeof(uvar16));
	cout<<"Bits per Pixel: "<<Bit<<endl;
	fout_txt<<"Bits per Pixel: "<<Bit<<endl;
	/*每个象素的位数,偏移量28,256色应该8bit,Bits per Pixel: 8*/
	fin.read((char*)&Compress,sizeof(uvar32));
	cout<<"Compresssion Y or N: "<<Compress<<endl;
	fout_txt<<"Compresssion Y or N: "<<Compress<<endl;
	/*压缩说明0为不压缩,偏移量30,Compresssion Y or N: 0*/
	if(Compress) { fin.close();return 2;}
	fin.read((char*)&DataSize,sizeof(uvar32));
	cout<<"Data Size: "<<DataSize<<endl;
	fout_txt<<"Data Size: "<<DataSize<<endl;
	/*用字节数表示的位图数据的大小,偏移量34,256*256=65536,
	Data Size: 65536*/
	fin.read((char*)&HResolution,sizeof(uvar32));
	cout<<"HResolution: "<<HResolution<<endl;
	fout_txt<<"HResolution: "<<HResolution<<endl;
	/*用象素/米表示的水平分辨率,偏移量38,HResolution: 0*/
	fin.read((char*)&VResolution,sizeof(uvar32));
	cout<<"VResolution: "<<VResolution<<endl;
	fout_txt<<"VResolution: "<<VResolution<<endl;
	/*用象素/米表示的垂直分辨率,偏移量42,VResolution: 0*/
	fin.read((char*)&Colors,sizeof(uvar32));
	cout<<"Colors: "<<Colors<<endl;
	fout_txt<<"Colors: "<<Colors<<endl;
	/*位图使用的颜色数,256色,偏移量46,Colors: 256*/
	fin.read((char*)&ImportantColors,sizeof(uvar32));
	cout<<"ImportantColors: "<<ImportantColors<<endl;
	fout_txt<<"ImportantColors: "<<ImportantColors<<endl;
	/*指定重要的颜色数。当该域的值等于颜色数时（或者等于0时），
	表示所有颜色都一样重要。偏移量50,ImportantColors: 0*/
	for(i=0;i<256;i++)
	{
		fin.read((char*)&Rgbquad,sizeof(Palette));
		cout<<int(Rgbquad.Blue)<<" "<<int(Rgbquad.Green)<<" "<<int(Rgbquad.Red)
			<<"  "<<int(Rgbquad.Reserved)<<endl;
		fout_txt<<int(Rgbquad.Blue)<<" "<<int(Rgbquad.Green)<<" "<<int(Rgbquad.Red)
			<<"  "<<int(Rgbquad.Reserved)<<endl;
	}
	/*调色板规范。对于调色板中的每个表项，这4个字节用下述方法来
	描述RGB的值：1字节用于蓝色分量 ,1字节用于绿色分量 ,1字节用于红色分量
	1字节用于填充符(设置为0),偏移量由54到54＋255＊4=1074，对于彩色图像R,G，B
	有各自的分量，对与黑白图像，R=G=B=灰度级。输出结果有 0 0 0 0，1 1 1 0…
	255 255 255 0*/

	fin.seekg(Offset,ios::beg);
	for( i=27;i>=0;i--)
	{
		for(j=0;j<(Width+3)/4*4;j++)
		{
			fin.read((char*)&imdata[i][j],sizeof(var8));
			
			
		}
	}
		/*直接把65536个像素的灰度读入数组imdata中,由于前面调色板的格式，读出的数据无
		需 索引调色板。由于Height值大于零,图像倒置的,开始读入的数据是图像最后一行的灰
		度值。*/
	fin.close();
	fout_txt.close();
	system("pause");
	return 0;
		/*以上数据结果与Matlab的imfinfo()和imread()的结果相吻合。
		读出数据后，就可以进行直方图统计，fft2运算了。^&^*/
}
