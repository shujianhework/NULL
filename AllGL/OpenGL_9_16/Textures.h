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
#if _UNICODE
#define WCHARS std::wstring
FILE *fopen(wchar_t *filename, wchar_t *mode){
	return ::fopen(TW2C(filename).c_str(), TW2C(mode).c_str());
}
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
private:
	Textures();
	~Textures();
	TextureData* CreateTextureData(WCHAR* path, std::function<void(AUX_RGBImageRec)> back);
	unsigned int GetTextureData(WCHAR* path, TextureFilterType tft, std::function<void(AUX_RGBImageRec)> back = nullptr);
public:
	unsigned int GetTextureData(WCHAR *path, TextureFilterType tft);
	static Textures* getInstance();
};

