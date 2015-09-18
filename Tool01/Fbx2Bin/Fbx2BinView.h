
// Fbx2BinView.h : CFbx2BinView クラスのインターフェイス
//

#pragma once

#include "resource.h"
#include "DxWindow.h"
#include "afxwin.h"


class CFbx2BinView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CFbx2BinView();
	DECLARE_DYNCREATE(CFbx2BinView)

public:
	enum{ IDD = IDD_FBX2BIN_FORM };
	CDxWindow m_DxWindow;
	UINT_PTR m_TimerID;
	BOOL m_IsFbxReaded;	//FBXが読まれたかどうか

// 属性
public:
	CFbx2BinDoc* GetDocument() const;

// 操作
public:
	void SetSkinedActive(BOOL b);

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。

// 実装
public:
	virtual ~CFbx2BinView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_Directory;
	CString m_FbxFileName;
	afx_msg void OnClickedButtonReadfile();
	CString m_Scale;
	CString m_MeshID;
	CString m_BinFileName;
	CString m_BinFileHeader;
	afx_msg void OnClickedButtonSavefile();
	CString m_BinFileNameExt;
	BOOL m_IsScaling;

	CEdit m_AnimeSampleStart;
	CEdit m_AnimeSampleEnd;
	CEdit m_AnimeSampleRate;
	afx_msg void OnClickedButtonSavefileSkin();
	CButton m_SaveFileSkinButton;
	CButton m_RunSkin;
	afx_msg void OnClickedButtonRunSkin();
	CEdit m_SampleSpan;
};

#ifndef _DEBUG  // Fbx2BinView.cpp のデバッグ バージョン
inline CFbx2BinDoc* CFbx2BinView::GetDocument() const
   { return reinterpret_cast<CFbx2BinDoc*>(m_pDocument); }
#endif

