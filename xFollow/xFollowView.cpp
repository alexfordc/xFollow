
// xFollowView.cpp : CxFollowView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CxFollowView ����/����

CxFollowView::CxFollowView()
{
	// TODO: �ڴ˴���ӹ������

}

CxFollowView::~CxFollowView()
{
}

BOOL CxFollowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CxFollowView ����

void CxFollowView::OnDraw(CDC* /*pDC*/)
{
	CxFollowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CxFollowView ���

#ifdef _DEBUG
void CxFollowView::AssertValid() const
{
	CView::AssertValid();
}

void CxFollowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxFollowDoc* CxFollowView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxFollowDoc)));
	return (CxFollowDoc*)m_pDocument;
}

#endif //_DEBUG


// CxFollowView ��Ϣ�������


void CxFollowView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
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
