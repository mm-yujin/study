#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;



private: 
	void		Move_Route();
	void		Fire_Bullet();

private:
	float		m_fFireItv = 0.f;
	float		m_fFireAngle = 0.f;
	int			m_iCount = 0;

};

