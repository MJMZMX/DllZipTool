//#include "stdafx.h"
#include <iostream>
#include <direct.h> 
#include <vector>
//#include <string>
#include "zipTool.h"


int main()
{

	return 0;
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 实现压缩文件夹操作 
// 参数说明: [in]： pFilePath         要被压缩的文件夹 
//                         mZipFileFullPath  压缩后的文件名和路径 
// 返回值: 参数有误的情况下返回FALSE,压缩成功后返回TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_PackFiles(CString pFilePath, CString mZipFileFullPath)
{
	//参数错误 
	if ((pFilePath == L"") || (mZipFileFullPath == L""))
	{
		//路径异常返回 
		return FALSE;
	}

	CString zipSavePath = mZipFileFullPath;
	CString c = "\\";
	CString z = ".zip";

	//判断路径是否存在
	DWORD dwAttrib = GetFileAttributes(pFilePath);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib)
	{
		string path = CT2CA(pFilePath.GetBuffer(0));
		const char* tempPath = path.c_str();
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(tempPath, &FindFileData);
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//是文件夹
			if (zipSavePath.Right(4).MakeLower() != L".zip")   //输出路径不包含.zip,增加它
			{
				if (zipSavePath.Right(1) == L"\\")
				{
					zipSavePath.Append(pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1));
					zipSavePath.Append(z);
				}
				else
				{
					zipSavePath.Append(c);
					zipSavePath.Append(pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1));
					zipSavePath.Append(z);
				}

				CString tZipFilePath = zipSavePath.Left(zipSavePath.ReverseFind('\\') + 1);
				if (!ZipTool::FolderExist(tZipFilePath))
				{
					//ZIP文件存放的文件夹不存在创建它 
					wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
					if (FALSE == CreatedMultipleDirectory(temp))
					{
						//创建目录失败 
						return FALSE;
					}
				}
			}


			//获得文件夹的名字 
			if (pFilePath.Right(1) == L"\\")
			{
				this->m_FolderPath = pFilePath.Left(pFilePath.GetLength() - 1);
				m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1);
			}
			else
			{
				this->m_FolderPath = pFilePath;
				m_FolderName = pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1);
			}

			//创建ZIP文件 
			hz = CreateZip(zipSavePath, 0);
			if (hz == 0)
			{
				//创建Zip文件失败 
				return FALSE;
			}

			//递归文件夹,将获取的问价加入ZIP文件 
			BrowseFile(pFilePath);
		}
		else
		{
			//是文件
			if (zipSavePath.Right(4).MakeLower() != L".zip")   //输出路径不包含.zip,增加它
			{
				if (zipSavePath.Right(1) == L"\\")
				{
					zipSavePath.Append(ZipTool::getFileName(pFilePath));
					zipSavePath.Append(z);
				}
				else
				{
					zipSavePath.Append(c);
					zipSavePath.Append(ZipTool::getFileName(pFilePath));
					//CString nameTemp = pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1);
					//zipSavePath.Append(nameTemp.Left(nameTemp.ReverseFind('.')));
					zipSavePath.Append(z);
				}


				CString tZipFilePath = zipSavePath.Left(zipSavePath.ReverseFind('\\') + 1);
				if (!ZipTool::FolderExist(tZipFilePath))
				{
					//ZIP文件存放的文件夹不存在创建它 
					wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
					if (FALSE == CreatedMultipleDirectory(temp))
					{
						//创建目录失败 
						return FALSE;
					}
				}
			}

			hz = CreateZip(zipSavePath, 0);
			if (hz == 0)
			{
				//创建Zip文件失败 
				return FALSE;
			}
			CString strPath;
			strPath.Format(L"%s", pFilePath);
			CString fileNameUnzip = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\') - 1);    //要压缩的文件名
			ZipAdd(hz, fileNameUnzip, pFilePath);
		}
		//关闭ZIP文件 
		CloseZip(hz);
	}
	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//压缩失败(未发现压缩后的文件) 
		return FALSE;
	}
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 实现压缩文件夹操作 
// 参数说明: [in]： pFilePath         要被压缩的文件夹 ，
// 压缩后存放原路径，文件名不变
// 返回值: 参数有误的情况下返回FALSE,压缩成功后返回TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_PackFiles(CString pFilePath)
{
	//参数错误 
	if (pFilePath == L"")
	{
		//路径异常返回 
		return FALSE;
	}
	//判断路径是否存在
	DWORD dwAttrib = GetFileAttributes(pFilePath);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib)
	{
		string path = CT2CA(pFilePath.GetBuffer(0));
		const char* tempPath = path.c_str();
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(tempPath, &FindFileData);
		CString z = ".zip";
		CString c = "\\";
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{

			//获得文件夹的名字 
			if (pFilePath.Right(1) == L"\\")
			{
				this->m_FolderPath = pFilePath.Left(pFilePath.GetLength() - 1);
				m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1);
			}
			else
			{
				this->m_FolderPath = pFilePath;
				m_FolderName = pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1);
			}

			CString zipFileFullPath = ZipTool::getCurrentPath(pFilePath);
			zipFileFullPath.Append(c);
			zipFileFullPath.Append(ZipTool::getFolderName(pFilePath));
			zipFileFullPath.Append(z);
			//创建ZIP文件 
			hz = CreateZip(zipFileFullPath, 0);
			if (hz == 0)
			{
				//创建Zip文件失败 
				return FALSE;
			}

			//递归文件夹,将获取的问价加入ZIP文件 
			BrowseFile(pFilePath);
			//关闭ZIP文件 
			CloseZip(hz);
			CFileFind tFFind;
			if (!tFFind.FindFile(zipFileFullPath))
			{
				//压缩失败(未发现压缩后的文件) 
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			//是文件
			CString zipFileFullPath = ZipTool::getCurrentPath(pFilePath);
			zipFileFullPath.Append(c);
			zipFileFullPath.Append(ZipTool::getFileName(pFilePath));
			zipFileFullPath.Append(z);
			hz = CreateZip(zipFileFullPath, 0);
			if (hz == 0)
			{
				//创建Zip文件失败 
				return FALSE;
			}
			CString strPath;
			strPath.Format(L"%s", pFilePath);
			CString fileNameUnzip = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\') - 1);    //要压缩的文件名
			ZipAdd(hz, fileNameUnzip, pFilePath);
			//关闭ZIP文件 
			CloseZip(hz);
			CFileFind tFFind;
			if (!tFFind.FindFile(zipFileFullPath))
			{
				//压缩失败(未发现压缩后的文件) 
				return FALSE;
			}
			return TRUE;
		}
	}
}



///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 解压缩文件夹 
// 参数说明: [in]： mUnPackPath     解压后文件存放的路径 
//                mZipFileFullPath  ZIP文件的路径 
// 返回值: 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath)
{
	//参数错误 
	if ((mUnPackPath == L"") || (mZipFileFullPath.Right(4).MakeLower() != L".zip") || (mZipFileFullPath == L""))
	{
		//路径异常返回 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//压缩失败(未发现压缩文件) 
		return FALSE;
	}

	//如果解压缩的路径不存在 试图创建它 
	CString tZipFilePath = mUnPackPath;
	if (!ZipTool::FolderExist(tZipFilePath))
	{
		//解压后存放的文件夹不存在 创建它 
		wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
		if (FALSE == CreatedMultipleDirectory(temp))
		{
			//创建目录失败 
			return FALSE;
		}
	}

	//打开ZIP文件 
	hz = OpenZip(mZipFileFullPath, 0);
	if (hz == 0)
	{
		//打开Zip文件失败 
		return FALSE;
	}

	zr = SetUnzipBaseDir(hz, mUnPackPath);
	if (zr != ZR_OK)
	{
		//打开Zip文件失败 
		CloseZip(hz);
		return FALSE;
	}

	zr = GetZipItem(hz, -1, &ze);
	if (zr != ZR_OK)
	{
		//获取Zip文件内容失败 
		CloseZip(hz);
		return FALSE;
	}

	int numitems = ze.index;
	for (int i = 0; i<numitems; i++)
	{
		zr = GetZipItem(hz, i, &ze);
		zr = UnzipItem(hz, i, ze.name);

		if (zr != ZR_OK)
		{
			//获取Zip文件内容失败 
			CloseZip(hz);
			return FALSE;
		}
	}

	CloseZip(hz);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 解压缩文件夹 
// 参数说明: [in]： mUnPackPath     解压后文件存放的路径 
// 解压后存放路径与原路径一致，文件夹名不变
// 返回值: 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_UnPackFiles(CString mZipFileFullPath)
{
	//参数错误 
	if ((mZipFileFullPath == L"") || (mZipFileFullPath.Right(4).MakeLower() != L".zip"))
	{
		//路径异常返回 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//压缩失败(未发现压缩文件) 
		return FALSE;
	}

	//打开ZIP文件 
	hz = OpenZip(mZipFileFullPath, 0);
	if (hz == 0)
	{
		//打开Zip文件失败 
		return FALSE;
	}

	zr = SetUnzipBaseDir(hz, ZipTool::getCurrentPath(mZipFileFullPath));
	if (zr != ZR_OK)
	{
		//打开Zip文件失败 
		CloseZip(hz);
		return FALSE;
	}

	zr = GetZipItem(hz, -1, &ze);
	if (zr != ZR_OK)
	{
		//获取Zip文件内容失败 
		CloseZip(hz);
		return FALSE;
	}

	int numitems = ze.index;
	for (int i = 0; i<numitems; i++)
	{
		zr = GetZipItem(hz, i, &ze);
		zr = UnzipItem(hz, i, ze.name);

		if (zr != ZR_OK)
		{
			//获取Zip文件内容失败 
			CloseZip(hz);
			return FALSE;
		}
	}

	CloseZip(hz);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 检查指定的文件夹是否存在 
// 参数说明: [in]：strPath 检查的文件夹 (此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::FolderExist(CString& strPath)
{
	CString sCheckPath = strPath;

	if (sCheckPath.Right(1) != L"\\")
		sCheckPath += L"\\";

	sCheckPath += L"*.*";

	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;

	HANDLE hFind = FindFirstFile(sCheckPath, &wfd);

	if ((hFind != INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) || (wfd.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE))
	{
		//如果存在并类型是文件夹 
		rValue = TRUE;
	}

	FindClose(hFind);
	return rValue;
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
void ZipTool::BrowseFile(CString &strFile)
{
	CFileFind ff;
	CString szDir = strFile;

	if (szDir.Right(1) != L"\\")
		szDir += L"\\";

	szDir += L"*.*";

	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath();

			CString subPath;
			GetRelativePath(strPath, subPath);
			//将文件添加到ZIP文件 
			ZipAddFolder(hz, subPath);
			BrowseFile(strPath);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath();
			CString subPath;

			GetRelativePath(strPath, subPath);
			//将文件添加到ZIP文件 
			ZipAdd(hz, subPath, strPath);
		}
	}

	//关闭 
	ff.Close();
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取相对路径 
// 参数说明: [in]：pFullPath 当前文件的完整路径 [out] : 解析后的相对路径 
///////////////////////////////////////////////////////////////////////////// 
void ZipTool::GetRelativePath(CString& pFullPath, CString& pSubString)
{
	pSubString = pFullPath.Right(pFullPath.GetLength() - this->m_FolderPath.GetLength() + this->m_FolderName.GetLength());
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 创建多级目录 
// 参数说明: [in]： 路径字符串 
// 返回值: BOOL 成功True 失败False 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::CreatedMultipleDirectory(wchar_t* direct)
{
	std::wstring Directoryname = direct;

	if (Directoryname[Directoryname.length() - 1] != '\\')
	{
		Directoryname.append(1, '\\');
	}
	std::vector< std::wstring> vpath;
	std::wstring strtemp;
	BOOL  bSuccess = FALSE;
	for (int i = 0; i < Directoryname.length(); i++)
	{
		if (Directoryname[i] != '\\')
		{
			strtemp.append(1, Directoryname[i]);
		}
		else
		{
			vpath.push_back(strtemp);
			strtemp.append(1, '\\');
		}
	}
	std::vector<std::wstring>::const_iterator vIter;
	for (vIter = vpath.begin(); vIter != vpath.end(); vIter++)
	{
		bSuccess = CreateDirectory(vIter->c_str(), NULL) ? TRUE : FALSE;
	}
	return bSuccess;
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取当前路径
// 参数说明: [in]：FullPath 当前文件的完整路径 
///////////////////////////////////////////////////////////////////////////// 
CString ZipTool::getCurrentPath(CString fullPath)
{
	if (!fullPath.IsEmpty())
	{
		return fullPath.Left(fullPath.ReverseFind('\\'));
	}
	else
		return "";
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取文件夹名称
// 参数说明: [in]：FullPath 当前文件的完整路径
///////////////////////////////////////////////////////////////////////////// 
CString ZipTool::getFolderName(CString fullPath)
{
	if (!fullPath.IsEmpty())
	{
		if (fullPath.Right(1) == L"\\")
		{
			CString pathTemp = fullPath.Left(fullPath.GetLength() - 1);
			return pathTemp.Right(pathTemp.GetLength() - pathTemp.ReverseFind('\\') - 1);
		}
		else
		{
			return fullPath.Right(fullPath.GetLength() - fullPath.ReverseFind('\\') - 1);
		}
	}
	else
		return "";
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取文件名称
// 参数说明: [in]：FullPath 当前文件的完整路径
///////////////////////////////////////////////////////////////////////////// 
CString ZipTool::getFileName(CString fullPath) {

	CString pathTemp = fullPath.Right(fullPath.GetLength() - fullPath.ReverseFind('\\') - 1);
	if (!pathTemp.IsEmpty())
	{
		if (pathTemp.Find('.') >0)
		{
			return pathTemp.Left(pathTemp.ReverseFind('.'));
		}
		else
		{
			return pathTemp;
		}
	}
	else
		return "";
}