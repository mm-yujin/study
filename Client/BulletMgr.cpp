#include "stdafx.h"
#include "BulletMgr.h"

IMPLEMENT_SINGLETON(CBulletMgr)

CBulletMgr::CBulletMgr()
{
}

CBulletMgr::~CBulletMgr()
{
	Release();
}

CObj * CBulletMgr::Reuse_Obj(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vDir)
{
	CObj*		pBullet = nullptr;

	if (m_ObjectPool.empty())
	{
		pBullet = CBullet::Create(vPos, vDir, m_iCount);
	}
	else
	{
		pBullet = m_ObjectPool.front();
		m_ObjectPool.pop_front();
	}

	if (nullptr == pBullet)
		return pBullet;

	pBullet->Set_Pos(vPos);
	pBullet->Set_Dir(vDir);

	return pBullet;
}

void CBulletMgr::Collect_Obj(CObj * pObj)
{
	if (nullptr == pObj)
		return;

	m_ObjectPool.push_back(pObj);
}

void CBulletMgr::Release()
{
	for_each(m_ObjectPool.begin(), m_ObjectPool.end(), CDeleteObj());
	m_ObjectPool.clear();
}
