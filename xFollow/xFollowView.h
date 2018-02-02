
// xFollowView.h : CxFollowView ��Ľӿ�
//

#pragma once

#include "MyListCtrl.h"

class CxFollowDoc;

class CxFollowView : public CView
{
protected: // �������л�����
	CxFollowView();
	DECLARE_DYNCREATE(CxFollowView)

// ����
public:
	CxFollowDoc* GetDocument() const;

public:
	void init();

private:
	CMyListCtrl      m_logList;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CxFollowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // xFollowView.cpp �еĵ��԰汾
inline CxFollowDoc* CxFollowView::GetDocument() const
   { return reinterpret_cast<CxFollowDoc*>(m_pDocument); }
#endif

