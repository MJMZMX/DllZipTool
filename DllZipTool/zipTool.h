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
	HZIP hz;          //Zip�ļ���� 
	ZRESULT zr;    //��������ֵ 
	ZIPENTRY ze;  //Zip�ļ���� 
	CString m_FolderPath;     //folder·�� 
	CString  m_FolderName;  //folder��Ҫ��ѹ�����ļ����� 
private:
	//ʵ�ֱ����ļ��� 
	void BrowseFile(CString &strFile);

	//��ȡ���·�� 
	void GetRelativePath(CString& pFullPath, CString& pSubString);

	//����·�� 
	BOOL CreatedMultipleDirectory(wchar_t* direct);
	//��ȡ��ǰ·��
	CString getCurrentPath(CString fullPath);
	//
	CString getFileName(CString fullPath);

	CString getFolderName(CString fullPath);

	BOOL FolderExist(CString& strPath);


public:
	//ѹ���ļ��нӿ� 
	_DLL_API BOOL Zip_PackFiles(CString pFilePath, CString mZipFileFullPath);
	_DLL_API BOOL Zip_PackFiles(CString pFilePath);

	//��ѹ���ļ��нӿ� 
	_DLL_API BOOL Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath);
	_DLL_API BOOL Zip_UnPackFiles(CString mZipFileFullPath);
};