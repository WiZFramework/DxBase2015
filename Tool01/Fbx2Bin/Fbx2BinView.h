
// Fbx2BinView.h : CFbx2BinView �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "resource.h"
#include "DxWindow.h"
#include "afxwin.h"


class CFbx2BinView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CFbx2BinView();
	DECLARE_DYNCREATE(CFbx2BinView)

public:
	enum{ IDD = IDD_FBX2BIN_FORM };
	CDxWindow m_DxWindow;
	UINT_PTR m_TimerID;
	BOOL m_IsFbxReaded;	//FBX���ǂ܂ꂽ���ǂ���

// ����
public:
	CFbx2BinDoc* GetDocument() const;

// ����
public:
	void SetSkinedActive(BOOL b);

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnInitialUpdate(); // �\�z��ɏ��߂ČĂяo����܂��B

// ����
public:
	virtual ~CFbx2BinView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

#ifndef _DEBUG  // Fbx2BinView.cpp �̃f�o�b�O �o�[�W����
inline CFbx2BinDoc* CFbx2BinView::GetDocument() const
   { return reinterpret_cast<CFbx2BinDoc*>(m_pDocument); }
#endif

