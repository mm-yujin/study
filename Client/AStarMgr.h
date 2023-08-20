#pragma once

#include "Include.h"
#include "MyTerrain.h"

class CAStarMgr
{
	DECLARE_SINGLETON(CAStarMgr)

private:
	CAStarMgr();
	~CAStarMgr();

public:
	list<TILE*>&		Get_BestList() { return m_BestList; }

public:
	void		AStar_Start(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);
	void		Release();
public:
	int			Get_TileIdx(const D3DXVECTOR3& vPos);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	bool		Make_BestList(int iStartIdx, int iGoalIdx);
	bool		Make_Route(int iStartIdx, int iGoalIdx);

private:
	bool		Check_Close(int iIndex);
	bool		Check_Open(int iIndex);

private:
	list<int>		m_CloseList;
	list<int>		m_OpenList;
	list<TILE*>		m_BestList;

	int				m_iStartIdx = 0;
};

