#include "stdafx.h"
#include "Player.h"
#include "..\Tool\TextureMgr.h"
#include "Device.h"
#include "AStarMgr.h"
#include "TimeMgr.h"
#include "Bullet.h"
#include "ObjMgr.h"
#include "BulletMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Dash";

	m_tFrame = { 0.f, 10.f };

	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		CAStarMgr::Get_Instance()->AStar_Start(m_tInfo.vPos, ::Get_Mouse() - m_vScroll);
	}

	Fire_Bullet();
	Move_Route();

	D3DXMATRIX	matScale, matTrans;
		
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x, 
		m_tInfo.vPos.y + m_vScroll.y, 
		m_tInfo.vPos.z);
		
	m_tInfo.matWorld = matScale * matTrans;

	return 0;
}

void CPlayer::Late_Update(void)
{
	__super::Move_Frame();
}

void CPlayer::Render(void)
{
	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
	
	float		fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float		fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 텍스처 객체 주소
		nullptr, // 출력할 이미지 영역에 대한 렉트 구조체 주소, null인경우 이미지의 0, 0기준으로 출력
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 vector3 구조체 포인터, nullptr인 경우 0, 0이 중심 좌표
		nullptr, // 위치 좌표에 따른 vector3 구조체 포인어
		D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG

	system("cls");
	cout << "총알 개수 : " << CBulletMgr::Get_Instance()->Get_Count() << endl;

#endif

}

void CPlayer::Release(void)
{
}

void CPlayer::Move_Route()
{
	list<TILE*>&	BestList = CAStarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		D3DXVECTOR3	vDir = BestList.front()->vPos - m_tInfo.vPos;

		float	fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_tInfo.vPos += vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (3.f >= fDistance)
			BestList.pop_front();
	}

}

void CPlayer::Fire_Bullet()
{
	m_fFireItv += CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (m_fFireItv < 3.f)
		return;

	m_fFireItv = 0.f;

	D3DXMATRIX		matRotZ, matTrans, matWorld;

	D3DXVECTOR3		vDir{ 1.f, 0.f,0.f };

	for (m_fFireAngle = 0.f; m_fFireAngle < 360.f; ++m_fFireAngle)
	{
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fFireAngle));
		D3DXMatrixTranslation(&matTrans, 
			m_tInfo.vPos.x,
			m_tInfo.vPos.y,
			0.f);

		matWorld = matRotZ * matTrans;
		D3DXVec3TransformNormal(&vDir, &vDir, &matWorld);

		// CObj*		pBullet = CBullet::Create(m_tInfo.vPos, vDir, m_iCount);

		CObj*	pBullet = CBulletMgr::Get_Instance()->Reuse_Obj(m_tInfo.vPos, vDir);

		CObjMgr::Get_Instance()->Add_Object(CObjMgr::EFFECT, pBullet);
	}


}
