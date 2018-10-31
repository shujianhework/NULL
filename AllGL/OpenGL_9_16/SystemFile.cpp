#include "stdafx.h"
#include "SystemFile.h"
#include "../Start/Tool.h"
#include "BMPFile.h"
#include <stdlib.h>
#include <stdio.h>
using namespace HHF;
#define OPENFILE(p,m) fopen(p,m);
#define CHECKFILE(hand,Break) if(hand == NULL){Break;}
#define GETSIZE(flg,v)  {fseek(flg,0,SEEK_END);v = ftell(flg);fseek(flg,0,SEEK_SET);}
#define READFILE(flg,data,len) fread(data,sizeof(data[1]),len,flg);
#define CLOSEFILE(hand) if(hand){fclose(hand);hand = NULL;}
#define LOADFIRSH10DATA(buff,p,m,ref){ref = false; auto f = OPENFILE(p, m); if(f){READFILE(f, buff, 10); CLOSEFILE(f); ref = true; }}
#define EQUALFLG(A,B,N) (strlen(B) >= N && N <= 10 && !strncmp(A,B,N))
//#define CHECK
bool SystemFile::init(){
	return true;
}
bool SystemFile::open(std::string path,std::string format){
	if (this->buffer)
		return false;
	memcpy(FileFormat, format.c_str(), format.length());
	auto f = OPENFILE(path.c_str(), "r");
	CHECKFILE(f, return false);
	GETSIZE(f,Filesize);
	buffer = new unsigned char[Filesize];
	READFILE(f, buffer, Filesize);
	CLOSEFILE(f);
	return true;
}
static bool getFormat(std::string path,std::string& formath){
	unsigned int idx = path.find_last_of(".");
	formath = "";
	if (idx == (-1)){
		return false;
	}
	std::string tempstr = path.substr(idx + 1, path.length());//.c_str();
	if (tempstr.length() >= FILEFORMATSIZE)
		return false;
	formath = tempstr;
	std::string lowformat = Tool::ToLow(tempstr.c_str());
	if (lowformat == "txt"){
		return true;
	}
	char arr[10] = { 0 };
	bool ref = false;
	LOADFIRSH10DATA(arr,path.c_str(),"r",ref);
	if (ref == false)
		return false;
	if (lowformat == "bmp"){
		char *p = "BM";
		if (EQUALFLG(arr, p, 2)){
			formath = "bmp";
			return true;
		}
		else
			return false;
	}
	return true;
}
SystemFile* SystemFile::Open(std::string path){
	std::string format = "";
	if (getFormat(path, format) == false)
		return NULL;
	SystemFile* systemfile = NULL;//new SystemFile;
	bool openret = false;
	if (format == "bmp"){
		systemfile = new BMPFile();
	}
	else{
		systemfile = new SystemFile;
	}
	openret = systemfile->open(path, format);
	if (systemfile && openret == false){
		delete systemfile;
		systemfile = NULL;
	}
	return systemfile;
}