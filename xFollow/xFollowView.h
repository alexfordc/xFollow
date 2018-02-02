
// xFollowView.h : CxFollowView 类的接口
//

#pragma once

#include "MyListCtrl.h"

class CxFollowDoc;

class CxFollowView : public CView
{
protected: // 仅从序列化创建
	CxFollowView();
	DECLARE_DYNCREATE(CxFollowView)

// 特性
public:
	CxFollowDoc* GetDocument() const;

public:
	void init();

private:
	CMyListCtrl      m_logList;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CxFollowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // xFollowView.cpp 中的调试版本
inline CxFollowDoc* CxFollowView::GetDocument() const
   { return reinterpret_cast<CxFollowDoc*>(m_pDocument); }
#endif

