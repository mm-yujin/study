#pragma once
#include "Obj.h"
class CMyTerrain :	public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	vector<TILE*>&		Get_VecTile() { return m_vecTile; }
	vector<list<TILE*>>&	Get_Adj() { return m_vecAdj; }

private:
	void		Load_Tile(const TCHAR* pFilePath);
	void		Ready_Adj();

private:
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>>	m_vecAdj;

};

