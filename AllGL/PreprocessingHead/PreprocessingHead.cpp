// PreprocessingHead.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <Windows.h>
//-1 打不开头文件 -2 获取版本号失败   -3 缺少头文件对应fpp文件  -4 写入头文件对应cpp文件失败 -5 参数错误
bool ArgvHandler(int argc, _TCHAR* argv[], std::vector<std::string> AllKeys, std::map<std::string, std::string> &params){
	std::string tempstr = "";
	params.clear();
	int index = -1;
	for (int i = 0; i < argc; i++)
	{
		tempstr = argv[i];
		if (tempstr.length() > 0){
			index = tempstr.find("=");
			if (index > 0){
				if(argv[i][index - 1] == '\\')
					params.insert(std::make_pair(tempstr,""));
				else{
					params.insert(std::make_pair(tempstr.substr(0, index), tempstr.substr(index + 1, tempstr.length() - index - 1)));
				}
			}
			else{
				params.insert(std::make_pair(tempstr, tempstr));
			}
		}
	}
	for each (auto var in AllKeys)
	{
		if (params.find(var) == params.end()){
			return false;
		}
	}
	return true;
}
bool ArgvHandler(int argc, _TCHAR* argv[], std::map<std::string, std::string> &params){
	std::vector<std::string> AllKeys;
	return ArgvHandler(argc, argv,AllKeys , params);
}
bool CheckKeys(std::vector<std::string> AllKeys, std::map<std::string, std::string> &params){

	for each (auto var in AllKeys)
	{
		if (params.find(var) == params.end()){
			printf("缺少参数:%s\n", var.c_str());
			for each (auto item in params)
			{
				printf("传入参数:%s=%s\n", item.first.c_str(), item.second.c_str());
			}
			return false;
		}
	}
	return true;
}
int Work(std::map<std::string, std::string> &params){
	int Version = -1;
	std::string tempstr = "#define "+params["D"]+" ";
	const char *strVersionDefine = tempstr.c_str();
	const int VersionDefinelen = strlen(strVersionDefine);
	const char *HeadFileName = params["H"].c_str();
	const char *PathFormat = params["P"].c_str();
	const char *ClassNameFormat = params["C"].c_str();
	const char* ClassDefineType = params["CD"].c_str();
	FILE *f = fopen(HeadFileName, "r");
	if (f == NULL)
		return -1;
	char data[4096] = "";
	while (!feof(f))
	{
		fgets(data, 4096, f);
		if (strlen(data) < 200 && strncmp(data, strVersionDefine, VersionDefinelen) == 0){
			char strVersion[200] = "";
			int len = strlen(data) - VersionDefinelen;
			strncpy(strVersion, &data[VersionDefinelen], len);
			std::stringstream ss;
			ss << strVersion;
			ss >> Version;
			break;
		}
	}
	fclose(f);
	f = NULL;
	if (Version < 0)
		return -2;
	std::vector<std::string> datalist;
	tempstr = "#include ";
	datalist.push_back("");
	datalist.push_back(tempstr + "\"" + HeadFileName + "\"\n");
	char ChildPath[1024] = "";
	sprintf(ChildPath, PathFormat, Version);
	datalist.push_back(tempstr + "\"" + ChildPath + "\"\n");
	ChildPath[0] = '\0';
	sprintf(ChildPath, ClassNameFormat, Version);
	tempstr = "#define ";
	tempstr = tempstr + ClassDefineType + " new ";
	datalist.push_back(tempstr + ChildPath + "()\n");
	tempstr = HeadFileName;
	tempstr = tempstr.substr(0, tempstr.length() - 1) + "fpp";
	f = fopen(tempstr.c_str(), "r");
	if (f == NULL)
		return -3;
	while (!feof(f))
	{
		fgets(data, 4096, f);
		if (strcmp(data, "#include \"stdafx.h\"\n") == 0)
			datalist[0] = data;
		else
			datalist.push_back(data);
	}
	fclose(f);
	//重新写入文件
	tempstr = HeadFileName;
	tempstr = tempstr.substr(0, tempstr.length() - 1) + "cpp";
	f = fopen(tempstr.c_str(), "w");
	if (f == NULL)
		return -4;
	for (int i = 0; i < datalist.size(); i++)
	{
		fwrite(datalist[i].c_str(), sizeof(char), datalist[i].length(), f);
	}
	fclose(f);
	return 0;
}
void ShowHelp(){
	printf("xxx.exe Key=Value:\nKey介绍:\n");
	printf("\t\tH:加载版本宏所在文件,并且同一目录存在对应tpp文件\n");
	printf("\t\tD:版本宏串名称\n");
	printf("\t\tP:路径格式\n");
	printf("\t\tC:类型名称格式\n");
	printf("\t\tCD:定义宏类名称 后面根据这个名称可以直接是用 auto *p = Value 生成对象\n");
	printf("\t\th:查看帮助\n");
}
int _tmain(int argc, _TCHAR* argv[])
{
	std::map<std::string, std::string> params;
	if (ArgvHandler(argc, argv, params) == false){
		ShowHelp();
		Sleep(2000);
		return -5;
	}
	if (params["h"] != "")
	{
		ShowHelp();
		Sleep(3000);
		return 0;
	}
	std::vector<std::string> BXKeys = { "H","D","P","C","CD" };
	if (CheckKeys(BXKeys, params) == false){
		ShowHelp();
		Sleep(6000);
		return -5;
	}
	return Work(params);
}

