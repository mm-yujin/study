#pragma once

#include "Include.h"
#include "TextureMgr.h"

class CDevice;
class CMainGame
{
public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(void);
	void		Release(void);

private:
	CDevice*			m_pGraphicDev = nullptr;

public:
	CMainGame();
	~CMainGame();
};

// ±íÀÌ ¿ì¼± Å½»ö, ³Êºñ ¿ì¼± Å½»ö