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

// 메모리 단편화 : 빈번한 메모리 블럭을 할당, 해제하면서 발생하는 메모리 조각나는 현상

// - 내부 단편화 : 실제 필요한 메모리보다 초과되는 크기로 할당되어 메모리가 남는 현상 (ex) 구조체
// - 외부 단편화 : 빈번한 메모리 할당, 해제 시 메모리가 중간 중간 비게 되는데 빈 공간의 메모리를 사용 할 수 없게 되는 현상 (ex) 디스크 조각 모음

