#pragma once
#include <list>
#include <tuple>
#include <string>
#include <map>
#include <Windows.h>
#include <functional>
#include "../Start/Tool.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <stdio.h>
#include <stdlib.h>
#if _UNICODE
#define WCHARS std::wstring
FILE *fopen(wchar_t *filename, wchar_t *mode);
#else
#define WCHARS std::string
#endif
enum TextureFilterType
{
	TFT_NEAREST,
	TFT_LINEAR,
	TFT_LINEAR_NEAREST
};
class Textures
{
private:
	struct TextureData
	{
		TextureFilterType FilterType;
		unsigned int Id;
	};
private:
	std::map<WCHARS, std::list<TextureData*>*> textures;
	TextureData tempTextureData;
	static Textures* __insta;
private:
	Textures();
	~Textures();
	TextureData* CreateTextureData(WCHAR* path, std::function<void(AUX_RGBImageRec)> back);
	unsigned int GetTextureData(WCHAR* path, TextureFilterType tft, std::function<void(AUX_RGBImageRec)> back);
public:
	unsigned int GetTextureData(WCHAR *path, TextureFilterType tft);
	static Textures* getInstance(){
		__insta = __insta ? __insta : new Textures;
		return __insta;
	}
};

