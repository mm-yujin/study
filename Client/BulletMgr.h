#pragma once

#include "Bullet.h"

class CBulletMgr
{
	DECLARE_SINGLETON(CBulletMgr)

private:
	CBulletMgr();
	~CBulletMgr();

public:
	// 현재 메모리 풀에 있는 오브젝트를 재사용하는 함수
	CObj*		Reuse_Obj(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir);

	// 사용을 마친 오브젝트를 회수하는 함수
	void		Collect_Obj(CObj* pObj);

	void		Release();
	
public:
	int		Get_Count() { return m_iCount; }

private:
	list<CObj*>		m_ObjectPool;
	int				m_iCount = 0;	
};

