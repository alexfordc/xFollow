
// xFollowView.cpp : CxFollowView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "xFollow.h"
#endif

#include "xFollowDoc.h"
#include "xFollowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CxFollowView

IMPLEMENT_DYNCREATE(CxFollowView, CView)

BEGIN_MESSAGE_MAP(CxFollowView, CView)
END_MESSAGE_MAP()

// CxFollowView 构造/析构

CxFollowView::CxFollowView()
{
	// TODO: 在此处添加构造代码

}

CxFollowView::~CxFollowView()
{
}

BOOL CxFollowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CxFollowView 绘制

void CxFollowView::OnDraw(CDC* /*pDC*/)
{
	CxFollowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CxFollowView 诊断

#ifdef _DEBUG
void CxFollowView::AssertValid() const
{
	CView::AssertValid();
}

void CxFollowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxFollowDoc* CxFollowView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxFollowDoc)));
	return (CxFollowDoc*)m_pDocument;
}

#endif //_DEBUG


// CxFollowView 消息处理程序


void CxFollowView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect viewRect;
	GetClientRect(&viewRect);
	if (m_logList.Create(WS_CHILD | WS_VISIBLE | LVS_LIST | LVS_ALIGNLEFT | WS_BORDER | LVS_OWNERDRAWFIXED, viewRect, this, ID_LIST_LOG))
	{
		m_logList.SetBkColor(RGB(30, 30, 30));
		m_logList.ShowWindow(SW_SHOW);
	}
}

void CxFollowView::init()
{
	m_logList.init();
}
