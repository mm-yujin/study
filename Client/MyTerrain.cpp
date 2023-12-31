#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	Load_Tile(L"../Data/Map.dat");

	Ready_Adj();

	return S_OK;
}

int CMyTerrain::Update(void)
{
	if (0.f > ::Get_Mouse().x)
		m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < ::Get_Mouse().x)
		m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > ::Get_Mouse().y)
		m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < ::Get_Mouse().y)
		m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR			szBuf[MIN_STR] = L"";
	int				iIndex = 0;

	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iMaxX = WINCX / TILECX;
	int		iMaxY = WINCY / (TILECY / 2);


	for (int i = iCullY; i < iCullY + iMaxY; ++i)
	{
		for (int j = iCullX; j < iCullX + iMaxX; ++j)
		{
			iIndex = i * TILEX + j;

			if(0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,	// x축으로 스크롤 값
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,	// y축으로 스크롤 값
				m_vecTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			float	fX = pTexInfo->tImgInfo.Width / 2.f;
			float	fY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, // 출력할 이미지 컴객체
				nullptr, // 출력할 이미지 영역에 대한 RECT구조체 포인터, NULL인 경우 이미지의 0,0 기준으로 출력
				&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 중심 축에 대한 VEC3 구조체 포인터, NULL인 경우 0, 0이 중심 좌표
				nullptr, // 위치 좌표에 대한 VEC3구조체 포인터, NULL인 경우 스크린 상 0, 0 좌표 출력
				D3DCOLOR_ARGB(255, 255, 255, 255));// 출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색상이 반영(0xffffffff를 넘겨주면 원본 색상 유지)

			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
				szBuf,
				lstrlen(szBuf),
				nullptr,
				0,
				D3DCOLOR_ARGB(255, 255, 255, 255));	
		}
	}

	
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CMyTerrain::Load_Tile(const TCHAR* pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	TILE*	pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);

	}
	CloseHandle(hFile);
}

void CMyTerrain::Ready_Adj()
{
	m_vecAdj.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;

			// 좌 상단 이웃
			// 맨 윗줄		맨 왼쪽 줄

			if ((0 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// 홀 -> 짝 20 감소, 짝 -> 홀 21 감소
				if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
				else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
				}
			}

			// 우 상단 이웃
			// 맨 윗줄		맨 오른쪽 줄

			if ((0 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// 홀 -> 짝 19 감소, 짝 -> 홀 20 감소
				if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				}
				else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			// 좌 하단 이웃
			// 맨 아랫 줄		맨 왼쪽 줄

			if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// 홀 -> 짝 20 증가, 짝 -> 홀 19 증가
				if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
				else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
				}
			}

			// 우 하단 이웃
			// 맨 아랫 줄		맨 오른쪽 줄

			if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// 홀 -> 짝 21 증가, 짝 -> 홀 20 증가
				if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
				}
				else if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}				
			}
		}
	}
}
