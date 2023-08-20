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

// �޸� ����ȭ : ����� �޸� ���� �Ҵ�, �����ϸ鼭 �߻��ϴ� �޸� �������� ����

// - ���� ����ȭ : ���� �ʿ��� �޸𸮺��� �ʰ��Ǵ� ũ��� �Ҵ�Ǿ� �޸𸮰� ���� ���� (ex) ����ü
// - �ܺ� ����ȭ : ����� �޸� �Ҵ�, ���� �� �޸𸮰� �߰� �߰� ��� �Ǵµ� �� ������ �޸𸮸� ��� �� �� ���� �Ǵ� ���� (ex) ��ũ ���� ����

