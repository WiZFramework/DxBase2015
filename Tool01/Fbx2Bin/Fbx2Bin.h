
// Fbx2Bin.h : Fbx2Bin �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CFbx2BinApp:
// ���̃N���X�̎����ɂ��ẮAFbx2Bin.cpp ���Q�Ƃ��Ă��������B
//

class CFbx2BinApp : public CWinApp
{
public:
	CFbx2BinApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFbx2BinApp theApp;
