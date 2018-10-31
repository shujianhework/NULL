#pragma once
#include "SystemFile.h"
namespace HHF{
	class BMPFile :public SystemFile
	{
	private:
		struct BitMapFileHeader{
			unsigned short bfType;
			unsigned long bfSize;
			unsigned short bfRetain1;
			unsigned short bfRetain2;
			unsigned long bfOffBits;
		};
		struct BitMapFileInfoHead{
			unsigned long biSize;//本结构大小
			long biWidth;//图片宽
			long biHeight;//图片高
			unsigned short biPlanes;//目标设备级别
			unsigned short biBitCount;//每个像素位数 属性的颜色深度
			unsigned long biCompression;//压缩类型  bmp 是 0
			unsigned long biSizeImage;//位图大小 
			long biXPelsPerMeter;//位图水平分辨率
			long biYPelsPerMeter;//位图垂直分辨率
			unsigned long biClrUsed;//位图实际使用的颜色标中的颜色数
			unsigned long biClrImportant;//位图显示过程中重要的颜色数
		};
		struct RGBQuand{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
		unsigned long width;
		unsigned long height;
		unsigned char *TextData;
	protected:
		virtual bool open(std::string path, std::string format);
	public:
		BMPFile();
		virtual bool close();
		virtual ~BMPFile();
		virtual unsigned char *getImageData(){
			return this->TextData;
		}
		inline unsigned long getwidth(){
			return width;
		}
		inline unsigned long getheight(){
			return height;
		}
	};
}