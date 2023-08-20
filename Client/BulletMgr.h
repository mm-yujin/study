#pragma once

#include "Bullet.h"

class CBulletMgr
{
	DECLARE_SINGLETON(CBulletMgr)

private:
	CBulletMgr();
	~CBulletMgr();

public:
	// ���� �޸� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�
	CObj*		Reuse_Obj(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir);

	// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	void		Collect_Obj(CObj* pObj);

	void		Release();
	
public:
	int		Get_Count() { return m_iCount; }

private:
	list<CObj*>		m_ObjectPool;
	int				m_iCount = 0;	
};

