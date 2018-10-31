#include "stdafx.h"
#include "Textures.h"
#include "BMPFile.h"
//#include "../PublicOtherLibH/png/png.h"
//#pragma comment(lib,"")
#ifdef aaa
typedef LONG_PTR ssize_t;
enum class PixelFormat
{
	//! auto detect the type
	AUTO,
	//! 32-bit texture: BGRA8888
	BGRA8888,
	//! 32-bit texture: RGBA8888
	RGBA8888,
	//! 24-bit texture: RGBA888
	RGB888,
	//! 16-bit texture without Alpha channel
	RGB565,
	//! 8-bit textures used as masks
	A8,
	//! 8-bit intensity texture
	I8,
	//! 16-bit textures used as masks
	AI88,
	//! 16-bit textures: RGBA4444
	RGBA4444,
	//! 16-bit textures: RGB5A1
	RGB5A1,
	//! 4-bit PVRTC-compressed texture: PVRTC4
	PVRTC4,
	//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
	PVRTC4A,
	//! 2-bit PVRTC-compressed texture: PVRTC2
	PVRTC2,
	//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
	PVRTC2A,
	//! ETC-compressed texture: ETC
	ETC,
	//! S3TC-compressed texture: S3TC_Dxt1
	S3TC_DXT1,
	//! S3TC-compressed texture: S3TC_Dxt3
	S3TC_DXT3,
	//! S3TC-compressed texture: S3TC_Dxt5
	S3TC_DXT5,
	//! ATITC-compressed texture: ATC_RGB
	ATC_RGB,
	//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
	ATC_EXPLICIT_ALPHA,
	//! ATITC-compressed texture: ATC_INTERPOLATED_ALPHA
	ATC_INTERPOLATED_ALPHA,
	//! Default texture format: AUTO
	DEFAULT = AUTO,

	NONE = -1
};
typedef struct
{
	const unsigned char * data;
	ssize_t size;
	int offset;
}tImageSource;
bool isPng(const unsigned char * data, ssize_t dataLen);
bool isPng(const unsigned char * data, ssize_t dataLen)
{
	if (dataLen <= 8)
	{
		return false;
	}

	static const unsigned char PNG_SIGNATURE[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };

	return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}
#define CC_BREAK_IF(con) if(con) break;
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

	if ((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))
void premultipliedAlpha(int _width,int _height,unsigned char* _data)
{
	//CCASSERT(_renderFormat == Texture2D::PixelFormat::RGBA8888, "The pixel format should be RGBA8888!");

	unsigned int* fourBytes = (unsigned int*)_data;
	for (int i = 0; i < _width * _height; i++)
	{
		unsigned char* p = _data + i * 4;
		fourBytes[i] = CC_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
	}

	//_hasPremultipliedAlpha = true;
}
bool Load(const unsigned char * data, ssize_t dataLen){
#define PNGSIGSIZE  8
	bool ret = false;
	png_byte        header[PNGSIGSIZE] = { 0 };
	png_structp     png_ptr = 0;
	png_infop       info_ptr = 0;

	do
	{
		// png header len is 8 bytes
		CC_BREAK_IF(dataLen < PNGSIGSIZE);

		// check the data is png or not
		memcpy(header, data, PNGSIGSIZE);
		CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

		// init png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		CC_BREAK_IF(!png_ptr);

		// init png_info
		info_ptr = png_create_info_struct(png_ptr);
		CC_BREAK_IF(!info_ptr);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
		CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

		// set the read call back function
		tImageSource imageSource;
		imageSource.data = (unsigned char*)data;
		imageSource.size = dataLen;
		imageSource.offset = 0;
		png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

		// read png header info

		// read png file info
		png_read_info(png_ptr, info_ptr);
		int _width = png_get_image_width(png_ptr, info_ptr);
		int _height = png_get_image_height(png_ptr, info_ptr);
		png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

		//CCLOG("color type %u", color_type);

		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			bit_depth = 8;
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		// reduce images with 16-bit samples to 8 bits
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
		}

		// Expanded earlier for grayscale, now take care of palette and rgb
		if (bit_depth < 8)
		{
			png_set_packing(png_ptr);
		}
		// update info
		png_read_update_info(png_ptr, info_ptr);
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
		PixelFormat _renderFormat = PixelFormat::NONE;
		switch (color_type)
		{
		case PNG_COLOR_TYPE_GRAY:
			_renderFormat = PixelFormat::I8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			_renderFormat = PixelFormat::AI88;
			break;
		case PNG_COLOR_TYPE_RGB:
			_renderFormat = PixelFormat::RGB888;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			_renderFormat = PixelFormat::RGBA8888;
			break;
		default:
			break;
		}

		// read png data
		png_size_t rowbytes;
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * _height);

		rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		int _dataLen = rowbytes * _height;
		unsigned char* _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
		if (!_data)
		{
			if (row_pointers != nullptr)
			{
				free(row_pointers);
			}
			break;
		}

		for (unsigned short i = 0; i < _height; ++i)
		{
			row_pointers[i] = _data + i*rowbytes;
		}
		png_read_image(png_ptr, row_pointers);

		png_read_end(png_ptr, nullptr);

		// premultiplied alpha for RGBA8888
		if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			premultipliedAlpha(_width,_height,_data);
		}
		else
		{
			//_hasPremultipliedAlpha = false;
		}

		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}

		ret = true;
	} while (0);

	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}
	return ret;
}
#endif
Textures* Textures::__insta = NULL;
Textures::Textures()
{
	tempTextureData.Id = 0;
}


Textures::~Textures()
{
	TextureData *tdata = NULL;
	for (auto& iter : textures){
		for each (auto var in *(iter.second)){
			delete var;
			var = NULL;
		}
		delete iter.second;
		iter.second = NULL;
	}
	textures.clear();	
}
//#define USERSYSTEM
#ifdef USERSYSTEM
Textures::TextureData* Textures::CreateTextureData(TCHAR* path, std::function<void(AUX_RGBImageRec)> back){
	FILE *fp = fopen(path, TEXT("r"));
	if (fp == NULL){
		printf("文件打开失败");
		return NULL;
	}
	fclose(fp);
	AUX_RGBImageRec* ari = auxDIBImageLoad(path);
	back(*ari);
	TextureData *data = new TextureData();
	memcpy(data, &tempTextureData, sizeof(TextureData));
	return data;
}
#else
Textures::TextureData* Textures::CreateTextureData(TCHAR* path, std::function<void(AUX_RGBImageRec)> back){
	FILE *fp = fopen(path, TEXT("r"));
	if (fp == NULL){
		printf("文件打开失败");
		return NULL;
	}
	fclose(fp);

	AUX_RGBImageRec ari = { 0 };// auxDIBImageLoad(path);
	HHF::BMPFile *bmp = (HHF::BMPFile *)HHF::SystemFile::Open(path);
	if (bmp == NULL)
		return NULL;
	ari.data = bmp->getImageData();
	ari.sizeX = bmp->getwidth(); 
	ari.sizeY = bmp->getheight();
	back(ari);
	Sleep(25);
	delete bmp;
	bmp = NULL;
	TextureData *data = new TextureData();
	memcpy(data, &tempTextureData, sizeof(TextureData));
	return data;
}
#endif
unsigned int Textures::GetTextureData(TCHAR *path, TextureFilterType tft){
	return GetTextureData(path,tft, [&](AUX_RGBImageRec image){
		tempTextureData.FilterType = tft;
		glBindTexture(GL_TEXTURE_2D, tempTextureData.Id);
		switch (tft)
		{
		case TFT_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		case TFT_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case TFT_LINEAR_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image.sizeX, image.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	});
}
unsigned int Textures::GetTextureData(TCHAR *path, TextureFilterType tft, std::function<void(AUX_RGBImageRec)> back){
	auto iter = textures.find(path);
	TextureData *tdata = NULL;
	unsigned int ErrorIdFlg = -1;
	if (iter != textures.end()){
		std::list<TextureData*>::iterator iiter = iter->second->begin();
		for (iiter = iter->second->begin();iiter != iter->second->end();iiter++)
		{
			tdata = *iiter;
			if (tdata->FilterType == tft)
				return tdata->Id;
		}
		if (back == nullptr)
			return 0;
		tdata = CreateTextureData(path, back);
		if (tdata->Id == ErrorIdFlg)
			delete tdata;
		else
			iter->second->push_back(tdata);
		return tempTextureData.Id;
	}
	else{
		tdata = CreateTextureData(path, back);
		if (tdata == NULL)
			return -1;
		if (tdata->Id == ErrorIdFlg)
			delete tdata;
		else{
			std::list<TextureData*>* lis = new std::list<TextureData*>();
			lis->push_back(tdata);
			WCHARS wchars = path;
			textures.insert(std::make_pair(wchars, lis));
		}
		return tempTextureData.Id;
	}
}
#if _UNICODE
FILE *fopen(wchar_t *filename, wchar_t *mode)
{
	return ::fopen(TW2C(filename).c_str(), TW2C(mode).c_str());
}
#endif