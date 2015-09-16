
// Fbx2Bin.h : Fbx2Bin アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CFbx2BinApp:
// このクラスの実装については、Fbx2Bin.cpp を参照してください。
//

class CFbx2BinApp : public CWinApp
{
public:
	CFbx2BinApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFbx2BinApp theApp;
