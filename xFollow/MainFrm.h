
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "../xFollowCenter/Interface/ICFollowCenter.h"
#include "../xFollowCenter/Interface/IFollowCenter.h"

class CMainFrame : public CFrameWnd, public IFollowCenterSpi
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

private:
	virtual void initRsp(bool successed, int errorID);
	virtual void startRsp(bool successed, int errorID);
	virtual void stopRsp();

	IFollowCenter*        m_followCenter;

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStart();
};


