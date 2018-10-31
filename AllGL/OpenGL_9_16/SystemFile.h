#pragma once
#include <iostream>
//å«º£ ÎÄ¼þ hanhaifile
namespace HHF{
#define FILEFORMATSIZE 25
	class SystemFile
	{
		std::string filepath;
		unsigned char *buffer;
		unsigned char FileFormat[FILEFORMATSIZE];
	protected:
		unsigned long long Filesize;
		virtual bool open(std::string path, std::string format);
	public:
		SystemFile() :filepath(""){
			memset(FileFormat, 0, FILEFORMATSIZE);
			Filesize = 0;
			buffer = NULL;
		}
		virtual ~SystemFile(){
			close();
		}
		virtual bool init();
		static SystemFile* Open(std::string path);
		unsigned char *GetBuffer(){
			return buffer;
		}
		virtual bool close(){
			if (buffer)
				delete buffer;
			buffer = NULL;
			Filesize = 0;
			return true;
		}
	};
}