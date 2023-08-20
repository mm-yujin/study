#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	CTextureMgr();
	~CTextureMgr();

public:
	const wstring&			Get_String() { return m_wstrFullPath; }
	void			Set_String(wstring wstrFullPath) { m_wstrFullPath = wstrFullPath; }

public:
	const TEXINFO * Get_Texture(const TCHAR * pObjKey = L"", const TCHAR * pStateKey = L"", const int & iCnt = 0) ;
	HRESULT Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey = L"", const int & iCnt = 0) ;
	HRESULT	ReadImgPath(const wstring& wstrFilePath);

	void Release() ;
	
private:
	map<wstring, CTexture*>		m_mapTex;
	wstring						m_wstrFullPath;


};

