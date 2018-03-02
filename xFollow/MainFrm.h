
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "../xFollowCenter/Interface/ICFollowCenter.h"
#include "../xFollowCenter/Interface/IFollowCenter.h"

class CMainFrame : public CFrameWnd, public IFollowCenterSpi
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

private:
	virtual void initRsp(bool successed, int errorID);
	virtual void startRsp(bool successed, int errorID);
	virtual void stopRsp();

	IFollowCenter*        m_followCenter;

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStart();
};


