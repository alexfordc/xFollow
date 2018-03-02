#pragma once

#include "afxcmn.h"
#include <windows.h>
#include <map>
#include <tuple>
#include <atomic>
#include <list>
#include <mutex>
#include <thread>

#include "../XNYSTools/Interface/ILogStream.h"
#include "../Include/X_MyLog.h"

#define WM_MYMSG WM_USER+8

class Clog : public ILogStream
{
public:
	Clog();
	virtual ~Clog();

public:
	void init(HWND hwnd);
	virtual void logBuffer(int logType, const char* buf, int len);

private:
	HWND     m_hwnd;
};

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)
public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

	void init();
	void exit();

private:
	void setFontSelf(int nHeight, LPCTSTR lpszFacename);
	Clog      m_log;
	bool      m_isInited;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// 	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	DECLARE_MESSAGE_MAP()

private:
	afx_msg LRESULT MyMsgHandler(WPARAM wParam, LPARAM lParam);
public:
// 	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
// 	virtual void PreSubclassWindow();
};

