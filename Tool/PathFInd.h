#pragma once
#include "afxwin.h"

#include "Include.h"

// CPathFInd ��ȭ �����Դϴ�.

class CPathFInd : public CDialog
{
	DECLARE_DYNAMIC(CPathFInd)

public:
	CPathFInd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFInd();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CListBox m_ListBox;
	list<IMGPATH*>		m_PathList;



};
