// PreprocessingHead.cpp : �������̨Ӧ�ó������ڵ㡣
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
//-1 �򲻿�ͷ�ļ� -2 ��ȡ�汾��ʧ��   -3 ȱ��ͷ�ļ���Ӧfpp�ļ�  -4 д��ͷ�ļ���Ӧcpp�ļ�ʧ�� -5 ��������
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
			printf("ȱ�ٲ���:%s\n", var.c_str());
			for each (auto item in params)
			{
				printf("�������:%s=%s\n", item.first.c_str(), item.second.c_str());
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
	//����д���ļ�
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
	printf("xxx.exe Key=Value:\nKey����:\n");
	printf("\t\tH:���ذ汾�������ļ�,����ͬһĿ¼���ڶ�Ӧtpp�ļ�\n");
	printf("\t\tD:�汾�괮����\n");
	printf("\t\tP:·����ʽ\n");
	printf("\t\tC:�������Ƹ�ʽ\n");
	printf("\t\tCD:����������� �������������ƿ���ֱ������ auto *p = Value ���ɶ���\n");
	printf("\t\th:�鿴����\n");
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

