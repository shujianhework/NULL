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
			unsigned long biSize;//���ṹ��С
			long biWidth;//ͼƬ��
			long biHeight;//ͼƬ��
			unsigned short biPlanes;//Ŀ���豸����
			unsigned short biBitCount;//ÿ������λ�� ���Ե���ɫ���
			unsigned long biCompression;//ѹ������  bmp �� 0
			unsigned long biSizeImage;//λͼ��С 
			long biXPelsPerMeter;//λͼˮƽ�ֱ���
			long biYPelsPerMeter;//λͼ��ֱ�ֱ���
			unsigned long biClrUsed;//λͼʵ��ʹ�õ���ɫ���е���ɫ��
			unsigned long biClrImportant;//λͼ��ʾ��������Ҫ����ɫ��
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