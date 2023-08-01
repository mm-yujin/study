#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";	// ��� ��θ� ������ �迭
	TCHAR	szCurDirPath[MAX_PATH] = L""; // ������ ���� ��θ� ������ �迭

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	//szCurDirPath���� strFullPath�� ���� ��� ��θ� ���ؼ� szRelativePath�� ����, ��, ���� ����̺� �������� ����

	PathRelativePathTo(szRelativePath, 
		szCurDirPath, 
		FILE_ATTRIBUTE_DIRECTORY, 
		strFullPath.GetString(), 
		FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtration(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	// ���Ͽ� ��� �� ���� ���� Ŭ����
	CFileFind		Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : �־��� ��ο� ������ �ִ��� ������ ������ �Ǵ��ϴ� �Լ�, �������� ������ FALSE �����ϸ� TRUE�� ����
	// ���� ������ �ִٴ� ���� �˰� �Ǿ��� �� ���Ͽ� �����ϰ� ���� ���� �ݵ�� FindNextFile �Լ��� ȣ���ؾ߸� �Ѵ�.

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ��� ���� ������ �ִ��� ���� �Ǵ�, ���̻� ã���� ���ų� ������ ������ ��� �ڵ� 0 ����
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtration(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szPathBuf);

			// L"D:\\����ȯ\\139��\\Frame139\\Texture\\Stage\\Effect\\BossMultiAttack"
			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : ���� �̸��� ������ �Լ�
			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			// wstrTextureName = L"BossMultiAttack0"

			// substr(����, ��) : ���ۿ��� ������ �ش��ϴ� ���ڿ��� �߶�
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);
			// L"D:\\����ȯ\\139��\\Frame139\\Texture\\Stage\\Effect\\BossMultiAttack"
			
			// PathCombine(1, 2, 3) : 2�� 3�� �̾�ٿ� 1�� ����, 2�� 3���̿� \\�� �ڵ����� ���Ե�ä 1�� ����
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// L"D:\\����ȯ\\139��\\Frame139\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			//	PathFindFileName : ���ϸ��� ã�ų� ���� ������ �������� ã�� �Լ�, ���� ������ �����ϴ� ������ STATEKEY��
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			bContinue = false;
		}

	}


}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	int				iCount = 0;
	CFileFind		Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
