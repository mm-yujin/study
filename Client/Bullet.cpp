#include "stdafx.h"
#include "Bullet.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "BulletMgr.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release();
}

HRESULT CBullet::Initialize(void)
{
	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"ElectronicShock";
	m_tFrame = { 0.f, 8.f };

	return S_OK;
}

int CBullet::Update(void)
{
	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + __super::m_vScroll.x,
		m_tInfo.vPos.y + __super::m_vScroll.y,
		m_tInfo.vPos.z);

	m_tInfo.matWorld = matTrans;

	if (0.f >= m_tInfo.vPos.x + __super::m_vScroll.x ||
		WINCX <= m_tInfo.vPos.x + __super::m_vScroll.x ||
		0.f >= m_tInfo.vPos.y + __super::m_vScroll.y ||
		WINCY <= m_tInfo.vPos.y + __super::m_vScroll.y)
	{
		CBulletMgr::Get_Instance()->Collect_Obj(this);
		return OBJ_DEAD;
	}
	
	return OBJ_NOEVENT;
}

void CBullet::Late_Update(void)
{
	Move_Bullet();
}

void CBullet::Render(void)
{
	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture
	(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	float		fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float		fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	
												nullptr, 
												&D3DXVECTOR3(fCenterX, fCenterY, 0.f), 
												nullptr, 
												D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBullet::Release(void)
{
}

void CBullet::Move_Bullet()
{
	m_tInfo.vPos += m_tInfo.vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
}

CBullet* CBullet::Create(const D3DXVECTOR3& vPos, 
	const D3DXVECTOR3& vDir, 
	int& iCnt)
{
	CBullet*		pInstance = new CBullet;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->Set_Pos(vPos);
	pInstance->Set_Dir(vDir);
	++iCnt;

	return pInstance;
}
