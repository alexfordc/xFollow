#include "stdafx.h"
#include "MyListCtrl.h"

#include <dirent.h>
#include <string>

Clog::Clog()
	: m_hwnd(nullptr)
{

}

Clog::~Clog()
{

}

void Clog::init( HWND hwnd )
{
	m_hwnd = hwnd;
}

void Clog::logBuffer( int logType, const char* buf, int len )
{
	char* log = new char[len + 1];
	sprintf_s(log, len, buf);
	PostMessage(m_hwnd, WM_MYMSG, (WPARAM)log, (LPARAM)logType);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
/*
	ON_WM_DROPFILES()
*/
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyListCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnNMCustomdraw)

	ON_MESSAGE(WM_MYMSG, MyMsgHandler)
	ON_WM_MEASUREITEM_REFLECT()
// 	ON_WM_DRAWITEM() // 这个只会在report模式下生效
	ON_WM_SIZE()
// 	ON_WM_PAINT()
END_MESSAGE_MAP()

CMyListCtrl::CMyListCtrl()
	: m_isInited(false)
{
}

CMyListCtrl::~CMyListCtrl()
{
}

void CMyListCtrl::init()
{
	if (!m_isInited)
	{
// 		this->ShowScrollBar(SB_VERT,TRUE);
		m_isInited = true;
		m_log.init(this->m_hWnd);

		setFontSelf(100, "Consolas"); //Consolas //fixedsys
		XNYSTools::ILog::logStream((uintptr_t)&m_log);
	}
}

void CMyListCtrl::exit()
{
	m_isInited = false;
}

/*
void CMyListCtrl::OnDropFiles( HDROP hDropInfo )
{
	char szFilePathName[_MAX_PATH+1] = {0};
	std::string fileVersion, buildTime;
	int errorCode = 0;

	//得到文件个数      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);   

	std::string fileName;
	std::lock_guard<std::mutex> lck(m_mutex);
	for (UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex) 
	{   
		//　得到文件名   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);
	}      

	//完成拖放操作
	DragFinish(hDropInfo);

	CListCtrl::OnDropFiles(hDropInfo);
}
*/

void CMyListCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CMyListCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CMyListCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CMyListCtrl::OnRButtonDown( UINT nFlags, CPoint point )
{
	CListCtrl::OnRButtonDown(nFlags, point);
}

void CMyListCtrl::OnRButtonUp( UINT nFlags, CPoint point )
{
	CListCtrl::OnRButtonUp(nFlags, point);
}

void CMyListCtrl::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem != -1)
	{
/*
		m_row = pNMItemActivate->iItem;
		m_colum = pNMItemActivate->iSubItem;

		//DWORD dwPos = GetMessagePos();
		//CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu; //定义下面要用到的cmenu对象
		menu.LoadMenu(IDR_POP_MENU); //装载自定义的右键菜单
		CMenu* pPopup = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单

		CPoint point1;//定义一个用于确定光标位置的位置
		GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标

		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point1.x, point1.y, this);//在指定位置显示弹出菜单 GetParent()
*/

	}

	*pResult = 0;
}

void CMyListCtrl::OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = 0;
	// TODO: Add your control notification handler code here
	//指定列表项绘制前后发送消息
	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	} else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) {
		pLVCD->clrTextBk = RGB(30, 30, 30);
		int type = this->GetItemData(pLVCD->nmcd.dwItemSpec);
		if(type == 1) {
			pLVCD->clrText = RGB(0, 230, 0);
		} else if(type == 2) {
			pLVCD->clrText = RGB(230, 230, 230);
		} else if(type == 3) {
			pLVCD->clrText = RGB(230, 230, 0);
		} else if(type == 4) {
			pLVCD->clrText = RGB(230, 0, 0);
		}
		*pResult = CDRF_DODEFAULT;
	}
}

LRESULT CMyListCtrl::MyMsgHandler( WPARAM wParam, LPARAM lParam )
{
	char* log = (char*)wParam;
	int row = this->GetItemCount();
	this->InsertItem(row, log);
	//this->SetItemText(save->lvi.iItem, ++save->lvi.iSubItem, save->version.c_str());
	this->SetItemData(row, lParam);
// 	this->EnsureVisible(row, FALSE);

	delete log;

	return 0;
}

//设置字体和大小
void CMyListCtrl::setFontSelf(int nHeight, LPCTSTR lpszFacename)
{
	CRect rcWindow;
	GetWindowRect(rcWindow);
	WINDOWPOS pos;
	pos.hwnd = this->m_hWnd;
	pos.cx = rcWindow.Width();
	pos.cy = rcWindow.Height();
	pos.flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE;
	SendMessage(WM_WINDOWPOSCHANGED,0,(LPARAM)&pos);

	CFont* pfont = new CFont; //在头文件中声明
	pfont->CreatePointFont(nHeight, lpszFacename);
	this->SetFont(pfont, true);
}

void CMyListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMeasureItemStruct->itemHeight = 200; // 你想要的高度
}


/*
void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);      
	LVITEM lvi = {0};   
	lvi.mask = LVIF_STATE;//|LVIF_IMAGE;   
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ;   
	lvi.iItem = lpDrawItemStruct->itemID;   
	BOOL bGet = GetItem(&lvi);   

	//高亮显示    
	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED)   
		&& ((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS))));  
	//画文本背景   
	CRect rcBack = lpDrawItemStruct->rcItem;   
	pDC->SetBkMode(TRANSPARENT);   
	if( bHighlight ) //如果被选中  
	{   
		pDC->SetTextColor(RGB(255,255,255));              //文本颜色  
		pDC->FillRect(rcBack, &CBrush(RGB(90,162,100)));  //行背景色  
	}   
	else   
	{   
		pDC->SetTextColor(RGB(0,0,0));                    //文本颜色  
		pDC->FillRect(rcBack, &CBrush(RGB(255,255,255))); //行背景色  
	}   

	//绘制文本  
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)   
	{   
		//得到列数  
		int nCollumn = GetHeaderCtrl()->GetItemCount();  
		//循环处理  
		CString szText;   
		for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)   
		{    
			CRect rcItem;   
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem ))  
			{   
				continue;   
			}  

			szText = GetItemText( lpDrawItemStruct->itemID, i );  
			rcItem.left += 5; rcItem.right -= 1;   
			pDC->DrawText(szText, lstrlen(szText), &rcItem,    
				DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);    
		}   
	}   
}

void CMyListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, LVS_OWNERDRAWFIXED);

	CListCtrl::PreSubclassWindow();
}
*/


void CMyListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CMyListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrl::OnPaint()
}
