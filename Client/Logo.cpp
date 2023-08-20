#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Device.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release_Scene();
}

HRESULT CLogo::Ready_Scene()
{
	
	InitializeCriticalSection(&m_Crt);


	/*DWORD		dwID[3];

	for(3)
	m_hThread[i] = (HANDLE)_beginthreadex(nullptr,  // 보안 속성(핸들의 상속 여부, nullptr이면 상속에서 제외)
										0,		 // 기본값 스택 사이즈(1바이트)
										ImgLoadThreadFunc, // 쓰레드로 동작시키고자 하는 함수의 이름(__stdcall 규약)
										this,  // 쓰레드 함수로 전달하고자하는 데이터 주소
										0,	// 쓰레드의 생성 및 실행을 조절하기 위한 flag, 기본값 01
										&dwID[i]);	// 쓰레드의 id 반환값*/


	int	iTemp = 0;

	m_hThread= (HANDLE)_beginthreadex(nullptr,  // 보안 속성(핸들의 상속 여부, nullptr이면 상속에서 제외)
									 0,		 // 기본값 스택 사이즈(1바이트)
									 ImgLoadThreadFunc, // 쓰레드로 동작시키고자 하는 함수의 이름(__stdcall 규약)
									 this,  // 쓰레드 함수로 전달하고자하는 데이터 주소
									 0,	// 쓰레드의 생성 및 실행을 조절하기 위한 flag, 기본값 0
									 nullptr);	// 쓰레드의 id 반환값

	return S_OK;
}

void CLogo::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::STAGE);
		return;
	}
}

void CLogo::Late_Update_Scene()
{
}

void CLogo::Render_Scene()
{
	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Logo");

	float	fX = pTexInfo->tImgInfo.Width / 2.f;
	float	fY = pTexInfo->tImgInfo.Height / 2.f;

	D3DXMATRIX	matTrans;
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 로딩 중인 파일 경로 출력
	wstring		wstrFullPath = CTextureMgr::Get_Instance()->Get_String();

	D3DXMatrixTranslation(&matTrans, 50.f, 500.f, 0.f);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		wstrFullPath.c_str(),
		wstrFullPath.length(),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 0, 255, 0));

}

void CLogo::Release_Scene()
{
}

size_t CLogo::ImgLoadThreadFunc(void * lpParam)
{
	CLogo*		pLogo = reinterpret_cast<CLogo*>(lpParam);

	EnterCriticalSection(&(pLogo->Get_Crt()));

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/JusinLogo.png", TEX_SINGLE, L"Logo")))
	{
		ERR_MSG(L"Logo Image Insert failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ReadImgPath failed");
		return E_FAIL;
	}

	CTextureMgr::Get_Instance()->Set_String(L"로딩 완료");

	LeaveCriticalSection(&(pLogo->Get_Crt()));
	// _endthreadex(0);

	return 0;
}
