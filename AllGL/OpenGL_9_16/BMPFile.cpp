#include "stdafx.h"
#include "BMPFile.h"
using namespace HHF;
BMPFile::BMPFile()
{
	TextData = NULL;
}
BMPFile::~BMPFile()
{
}
static void ColorTableMemSwap(unsigned char*& p, int l,int sigsize = 4){
	unsigned char temp = 0;
	for (int i = 0; i < l; i++)
	{ 
		temp = p[i * sigsize + 0];
		p[i * sigsize + 0] = p[i * sigsize + 2];
		p[i * sigsize + 2] = temp;
	}
}
bool BMPFile::open(std::string path, std::string format){
	if (false == __super::open(path, format))
		return false;
	char arr[100] = { 0 };
	memcpy(&arr[2], this->GetBuffer(), 98);
	BitMapFileHeader *bmfh = (BitMapFileHeader*)arr;
	BitMapFileInfoHead *bmfih = (BitMapFileInfoHead*)(&arr[sizeof(BitMapFileHeader)]);
	int bmfihoffset = sizeof(BitMapFileHeader)+sizeof(BitMapFileInfoHead)-2;
	width = bmfih->biWidth - 1;
	height = bmfih->biHeight - 1;
	if (bmfh->bfOffBits != bmfihoffset){
		RGBQuand *pl = (RGBQuand*)(GetBuffer() + bmfihoffset);
		unsigned int RGBSize = (bmfh->bfOffBits - bmfihoffset) / sizeof(RGBQuand);
		unsigned char *databegin = (unsigned char*)GetBuffer() + bmfh->bfOffBits;
		unsigned int size = (bmfih->biWidth*bmfih->biHeight) * 3;
		TextData = new unsigned char[size];
		if (bmfih->biBitCount == 1){
			for (int i = 0; i < bmfih->biWidth*bmfih->biHeight; i++){
				unsigned char *p = &TextData[i * 3];
				unsigned char idx = databegin[i / 8];
				unsigned char flg = idx;

				p[0] = flg;
				p[1] = flg;
				p[2] = flg;
			}
		}
		else if (bmfih->biBitCount == 4){
			unsigned char* temppl = (unsigned char*)pl;
			ColorTableMemSwap(temppl, 0xf);
			for (int i = 0; i < bmfih->biWidth*bmfih->biHeight; i++){
				unsigned char *p = &TextData[i * 3];
				unsigned char idx = databegin[i / 2];
				if (i % 2)
					idx = (idx >> 4) & 0xf;
				else
					idx = idx & 0xf;
				memcpy(p, &pl[idx], 3);
			}
		}
		else if (bmfih->biBitCount == 8){
			unsigned char* temppl = (unsigned char*)pl;
			ColorTableMemSwap(temppl, 256);
			for (int i = 0; i < bmfih->biWidth*bmfih->biHeight; i++){
				unsigned char *p = &TextData[i * 3];
				memcpy(p, &pl[databegin[i]], 3);
			}
		}
		else{
			return false;
		}
		return true;
	}
	TextData = (unsigned char*)GetBuffer() + bmfh->bfOffBits;
	ColorTableMemSwap(TextData, bmfih->biWidth*bmfih->biHeight,3);
	return true;
}
bool BMPFile::close(){
	if (TextData){
		if (TextData < GetBuffer() || (TextData > GetBuffer() + this->Filesize))
		{
			delete[] TextData;
			TextData = NULL;
		}
	}
	return __super::close(); 
}