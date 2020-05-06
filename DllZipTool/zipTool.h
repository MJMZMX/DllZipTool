#pragma once

#ifndef _DLL_API  
#define _DLL_API _declspec(dllexport)  
#else  
#define _DLL_API _declspec(dllimport)  
#endif


#include <afx.h>
#include "zip.h"
#include "unzip.h"

using namespace std;

class ZipTool
{

private:
	HZIP hz;          //Zip文件句柄 
	ZRESULT zr;    //操作返回值 
	ZIPENTRY ze;  //Zip文件入口 
	CString m_FolderPath;     //folder路径 
	CString  m_FolderName;  //folder将要被压缩的文件夹名 
private:
	//实现遍历文件夹 
	void BrowseFile(CString &strFile);

	//获取相对路径 
	void GetRelativePath(CString& pFullPath, CString& pSubString);

	//创建路径 
	BOOL CreatedMultipleDirectory(wchar_t* direct);
	//获取当前路径
	CString getCurrentPath(CString fullPath);
	//
	CString getFileName(CString fullPath);

	CString getFolderName(CString fullPath);

	BOOL FolderExist(CString& strPath);


public:
	//压缩文件夹接口 
	_DLL_API BOOL Zip_PackFiles(CString pFilePath, CString mZipFileFullPath);
	_DLL_API BOOL Zip_PackFiles(CString pFilePath);

	//解压缩文件夹接口 
	_DLL_API BOOL Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath);
	_DLL_API BOOL Zip_UnPackFiles(CString mZipFileFullPath);
};