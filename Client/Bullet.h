#pragma once
#include "Obj.h"
class CBullet :	public CObj
{
private:
	explicit CBullet();
	virtual ~CBullet();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	void		Move_Bullet();

public:
	static CBullet*		Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, int& iCnt);
};


