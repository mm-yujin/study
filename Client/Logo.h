#pragma once

#include "Scene.h"

class CLogo : public CScene
{
public:
	CLogo();
	virtual ~CLogo();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	CRITICAL_SECTION		Get_Crt() { return m_Crt; }

private:
	static size_t	__stdcall ImgLoadThreadFunc(void* lpParam);

private:
	CRITICAL_SECTION			m_Crt;
	HANDLE						m_hThread;

};

