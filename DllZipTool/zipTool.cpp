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
// ����˵��: ʵ��ѹ���ļ��в��� 
// ����˵��: [in]�� pFilePath         Ҫ��ѹ�����ļ��� 
//                         mZipFileFullPath  ѹ������ļ�����·�� 
// ����ֵ: �������������·���FALSE,ѹ���ɹ��󷵻�TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_PackFiles(CString pFilePath, CString mZipFileFullPath)
{
	//�������� 
	if ((pFilePath == L"") || (mZipFileFullPath == L""))
	{
		//·���쳣���� 
		return FALSE;
	}

	CString zipSavePath = mZipFileFullPath;
	CString c = "\\";
	CString z = ".zip";

	//�ж�·���Ƿ����
	DWORD dwAttrib = GetFileAttributes(pFilePath);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib)
	{
		string path = CT2CA(pFilePath.GetBuffer(0));
		const char* tempPath = path.c_str();
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(tempPath, &FindFileData);
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//���ļ���
			if (zipSavePath.Right(4).MakeLower() != L".zip")   //���·��������.zip,������
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
					//ZIP�ļ���ŵ��ļ��в����ڴ����� 
					wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
					if (FALSE == CreatedMultipleDirectory(temp))
					{
						//����Ŀ¼ʧ�� 
						return FALSE;
					}
				}
			}


			//����ļ��е����� 
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

			//����ZIP�ļ� 
			hz = CreateZip(zipSavePath, 0);
			if (hz == 0)
			{
				//����Zip�ļ�ʧ�� 
				return FALSE;
			}

			//�ݹ��ļ���,����ȡ���ʼۼ���ZIP�ļ� 
			BrowseFile(pFilePath);
		}
		else
		{
			//���ļ�
			if (zipSavePath.Right(4).MakeLower() != L".zip")   //���·��������.zip,������
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
					//ZIP�ļ���ŵ��ļ��в����ڴ����� 
					wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
					if (FALSE == CreatedMultipleDirectory(temp))
					{
						//����Ŀ¼ʧ�� 
						return FALSE;
					}
				}
			}

			hz = CreateZip(zipSavePath, 0);
			if (hz == 0)
			{
				//����Zip�ļ�ʧ�� 
				return FALSE;
			}
			CString strPath;
			strPath.Format(L"%s", pFilePath);
			CString fileNameUnzip = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\') - 1);    //Ҫѹ�����ļ���
			ZipAdd(hz, fileNameUnzip, pFilePath);
		}
		//�ر�ZIP�ļ� 
		CloseZip(hz);
	}
	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//ѹ��ʧ��(δ����ѹ������ļ�) 
		return FALSE;
	}
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ʵ��ѹ���ļ��в��� 
// ����˵��: [in]�� pFilePath         Ҫ��ѹ�����ļ��� ��
// ѹ������ԭ·�����ļ�������
// ����ֵ: �������������·���FALSE,ѹ���ɹ��󷵻�TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_PackFiles(CString pFilePath)
{
	//�������� 
	if (pFilePath == L"")
	{
		//·���쳣���� 
		return FALSE;
	}
	//�ж�·���Ƿ����
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

			//����ļ��е����� 
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
			//����ZIP�ļ� 
			hz = CreateZip(zipFileFullPath, 0);
			if (hz == 0)
			{
				//����Zip�ļ�ʧ�� 
				return FALSE;
			}

			//�ݹ��ļ���,����ȡ���ʼۼ���ZIP�ļ� 
			BrowseFile(pFilePath);
			//�ر�ZIP�ļ� 
			CloseZip(hz);
			CFileFind tFFind;
			if (!tFFind.FindFile(zipFileFullPath))
			{
				//ѹ��ʧ��(δ����ѹ������ļ�) 
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			//���ļ�
			CString zipFileFullPath = ZipTool::getCurrentPath(pFilePath);
			zipFileFullPath.Append(c);
			zipFileFullPath.Append(ZipTool::getFileName(pFilePath));
			zipFileFullPath.Append(z);
			hz = CreateZip(zipFileFullPath, 0);
			if (hz == 0)
			{
				//����Zip�ļ�ʧ�� 
				return FALSE;
			}
			CString strPath;
			strPath.Format(L"%s", pFilePath);
			CString fileNameUnzip = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\') - 1);    //Ҫѹ�����ļ���
			ZipAdd(hz, fileNameUnzip, pFilePath);
			//�ر�ZIP�ļ� 
			CloseZip(hz);
			CFileFind tFFind;
			if (!tFFind.FindFile(zipFileFullPath))
			{
				//ѹ��ʧ��(δ����ѹ������ļ�) 
				return FALSE;
			}
			return TRUE;
		}
	}
}



///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ��ѹ���ļ��� 
// ����˵��: [in]�� mUnPackPath     ��ѹ���ļ���ŵ�·�� 
//                mZipFileFullPath  ZIP�ļ���·�� 
// ����ֵ: 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath)
{
	//�������� 
	if ((mUnPackPath == L"") || (mZipFileFullPath.Right(4).MakeLower() != L".zip") || (mZipFileFullPath == L""))
	{
		//·���쳣���� 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//ѹ��ʧ��(δ����ѹ���ļ�) 
		return FALSE;
	}

	//�����ѹ����·�������� ��ͼ������ 
	CString tZipFilePath = mUnPackPath;
	if (!ZipTool::FolderExist(tZipFilePath))
	{
		//��ѹ���ŵ��ļ��в����� ������ 
		wchar_t* temp = tZipFilePath.GetBuffer(tZipFilePath.GetLength());
		if (FALSE == CreatedMultipleDirectory(temp))
		{
			//����Ŀ¼ʧ�� 
			return FALSE;
		}
	}

	//��ZIP�ļ� 
	hz = OpenZip(mZipFileFullPath, 0);
	if (hz == 0)
	{
		//��Zip�ļ�ʧ�� 
		return FALSE;
	}

	zr = SetUnzipBaseDir(hz, mUnPackPath);
	if (zr != ZR_OK)
	{
		//��Zip�ļ�ʧ�� 
		CloseZip(hz);
		return FALSE;
	}

	zr = GetZipItem(hz, -1, &ze);
	if (zr != ZR_OK)
	{
		//��ȡZip�ļ�����ʧ�� 
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
			//��ȡZip�ļ�����ʧ�� 
			CloseZip(hz);
			return FALSE;
		}
	}

	CloseZip(hz);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ��ѹ���ļ��� 
// ����˵��: [in]�� mUnPackPath     ��ѹ���ļ���ŵ�·�� 
// ��ѹ����·����ԭ·��һ�£��ļ���������
// ����ֵ: 
///////////////////////////////////////////////////////////////////////////// 
BOOL ZipTool::Zip_UnPackFiles(CString mZipFileFullPath)
{
	//�������� 
	if ((mZipFileFullPath == L"") || (mZipFileFullPath.Right(4).MakeLower() != L".zip"))
	{
		//·���쳣���� 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//ѹ��ʧ��(δ����ѹ���ļ�) 
		return FALSE;
	}

	//��ZIP�ļ� 
	hz = OpenZip(mZipFileFullPath, 0);
	if (hz == 0)
	{
		//��Zip�ļ�ʧ�� 
		return FALSE;
	}

	zr = SetUnzipBaseDir(hz, ZipTool::getCurrentPath(mZipFileFullPath));
	if (zr != ZR_OK)
	{
		//��Zip�ļ�ʧ�� 
		CloseZip(hz);
		return FALSE;
	}

	zr = GetZipItem(hz, -1, &ze);
	if (zr != ZR_OK)
	{
		//��ȡZip�ļ�����ʧ�� 
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
			//��ȡZip�ļ�����ʧ�� 
			CloseZip(hz);
			return FALSE;
		}
	}

	CloseZip(hz);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ���ָ�����ļ����Ƿ���� 
// ����˵��: [in]��strPath �����ļ��� (�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
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
		//������ڲ��������ļ��� 
		rValue = TRUE;
	}

	FindClose(hFind);
	return rValue;
}

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: �����ļ��� 
// ����˵��: [in]��strFile �������ļ���(�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
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
			//�����һ����Ŀ¼���õݹ��������һ���� 
			CString strPath = ff.GetFilePath();

			CString subPath;
			GetRelativePath(strPath, subPath);
			//���ļ���ӵ�ZIP�ļ� 
			ZipAddFolder(hz, subPath);
			BrowseFile(strPath);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath();
			CString subPath;

			GetRelativePath(strPath, subPath);
			//���ļ���ӵ�ZIP�ļ� 
			ZipAdd(hz, subPath, strPath);
		}
	}

	//�ر� 
	ff.Close();
}

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ��ȡ���·�� 
// ����˵��: [in]��pFullPath ��ǰ�ļ�������·�� [out] : ����������·�� 
///////////////////////////////////////////////////////////////////////////// 
void ZipTool::GetRelativePath(CString& pFullPath, CString& pSubString)
{
	pSubString = pFullPath.Right(pFullPath.GetLength() - this->m_FolderPath.GetLength() + this->m_FolderName.GetLength());
}

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: �����༶Ŀ¼ 
// ����˵��: [in]�� ·���ַ��� 
// ����ֵ: BOOL �ɹ�True ʧ��False 
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
// ����˵��: ��ȡ��ǰ·��
// ����˵��: [in]��FullPath ��ǰ�ļ�������·�� 
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
// ����˵��: ��ȡ�ļ�������
// ����˵��: [in]��FullPath ��ǰ�ļ�������·��
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
// ����˵��: ��ȡ�ļ�����
// ����˵��: [in]��FullPath ��ǰ�ļ�������·��
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