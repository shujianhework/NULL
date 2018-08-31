#include "stdafx.h"
#include "Textures.h"

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
Textures::TextureData* Textures::CreateTextureData(WCHAR* path, std::function<void(AUX_RGBImageRec)> back){
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
unsigned int Textures::GetTextureData(WCHAR *path, TextureFilterType tft){
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
unsigned int Textures::GetTextureData(WCHAR *path, TextureFilterType tft, std::function<void(AUX_RGBImageRec)> back){
	auto iter = textures.find(path);
	TextureData *tdata = NULL;
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
		if (tdata->Id == 0)
			delete tdata;
		else
			iter->second->push_back(tdata);
		return tempTextureData.Id;
	}
	else{
		tdata = CreateTextureData(path, back);
		if (tdata->Id == 0)
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