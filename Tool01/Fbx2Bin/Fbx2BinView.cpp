
// Fbx2BinView.cpp : CFbx2BinView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "Fbx2Bin.h"
#endif

#include "Fbx2BinDoc.h"
#include "Fbx2BinView.h"
#include "Project.h"
using namespace basedx11;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFbx2BinView

IMPLEMENT_DYNCREATE(CFbx2BinView, CFormView)

BEGIN_MESSAGE_MAP(CFbx2BinView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_READFILE, &CFbx2BinView::OnClickedButtonReadfile)
	ON_BN_CLICKED(IDC_BUTTON_SAVEFILE, &CFbx2BinView::OnClickedButtonSavefile)
	ON_BN_CLICKED(IDC_BUTTON_SAVEFILE_SKIN, &CFbx2BinView::OnClickedButtonSavefileSkin)
	ON_BN_CLICKED(IDC_BUTTON_RUN_SKIN, &CFbx2BinView::OnClickedButtonRunSkin)
END_MESSAGE_MAP()

// CFbx2BinView コンストラクション/デストラクション

CFbx2BinView::CFbx2BinView()
	: CFormView(CFbx2BinView::IDD)
	, m_Directory(_T(""))
	, m_FbxFileName(_T(""))
	, m_TimerID(0)
	, m_Scale(_T("1.0"))
	, m_MeshID(_T("0"))
	, m_BinFileName(_T(""))
	, m_BinFileHeader(_T("BDV1.0"))
	, m_BinFileNameExt(_T(".bin"))
	, m_IsScaling(TRUE)
	,m_IsFbxReaded(FALSE)
{
	// TODO: 構築コードをここに追加します。

}

CFbx2BinView::~CFbx2BinView()
{
	App::DeleteApp();
}

void CFbx2BinView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIRECTORY, m_Directory);
	DDX_Text(pDX, IDC_EDIT_FBXFILENAME, m_FbxFileName);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_Scale);
	DDX_Text(pDX, IDC_EDIT_MESHID, m_MeshID);
	DDX_Text(pDX, IDC_EDIT_BINFILENAME, m_BinFileName);
	DDX_Text(pDX, IDC_EDIT_BINFILEHEADER, m_BinFileHeader);
	DDV_MaxChars(pDX, m_BinFileHeader, 15);
	DDX_Text(pDX, IDC_EDIT_BINFILENAME_EXT, m_BinFileNameExt);
	DDX_Check(pDX, IDC_CHECK_IS_SCALING, m_IsScaling);
	DDX_Control(pDX, IDC_EDIT_ANIME_START, m_AnimeSampleStart);
	DDX_Control(pDX, IDC_EDIT_ANIME_END, m_AnimeSampleEnd);
	DDX_Control(pDX, IDC_EDIT_ANIME_SAMPLE, m_AnimeSampleRate);
	DDX_Control(pDX, IDC_BUTTON_SAVEFILE_SKIN, m_SaveFileSkinButton);
	DDX_Control(pDX, IDC_BUTTON_RUN_SKIN, m_RunSkin);
	DDX_Control(pDX, IDC_EDIT_ANIME_SAMPLE_SPAN, m_SampleSpan);
}

BOOL CFbx2BinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
}

void CFbx2BinView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CFbx2BinView::SetSkinedActive(BOOL b){
	m_AnimeSampleStart.EnableWindow(b);
	m_AnimeSampleEnd.EnableWindow(b);
	m_AnimeSampleRate.EnableWindow(b);
	m_RunSkin.EnableWindow(b);
	m_SampleSpan.EnableWindow(b);
	m_SaveFileSkinButton.EnableWindow(b);
}



// CFbx2BinView 診断

#ifdef _DEBUG
void CFbx2BinView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFbx2BinView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFbx2BinDoc* CFbx2BinView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFbx2BinDoc)));
	return (CFbx2BinDoc*)m_pDocument;
}
#endif //_DEBUG


// CFbx2BinView メッセージ ハンドラー


int CFbx2BinView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	auto ClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH));
	CRect r(0, 0, 640, 480);
	r += CPoint(256, 16);
	m_DxWindow.Create(ClassName, L"", WS_CHILD | WS_VISIBLE, r, this, 0);



	try{
		////アプリケーションクラスの構築
		App::GetApp(lpCreateStruct->hInstance, m_DxWindow.GetSafeHwnd(), false, 640, 480);
		//シーンの追加登録
		auto PtrScene = App::GetApp()->AddScene<Scene>();
		PtrScene->SetView(this);


		m_TimerID = SetTimer(1, 10, NULL);
	}
	catch (...){
		if (m_TimerID != 0){
			KillTimer(m_TimerID);
		}
		m_TimerID = 0;
		AfxMessageBox(L"エラーです。終了します。");
		return -1;
	}


	return 0;
}


void CFbx2BinView::OnDestroy()
{
	CFormView::OnDestroy();
	if (m_TimerID != 0){
		KillTimer(m_TimerID);
	}
	m_TimerID = 0;
}


void CFbx2BinView::OnTimer(UINT_PTR nIDEvent)
{
	try{
		App::GetApp()->Update();
		if (App::GetApp()->Draw()){
			//フロイントバッファに転送
			App::GetApp()->Present(1, 0);
		}
	}
	catch (...){
		if (m_TimerID != 0){
			KillTimer(m_TimerID);
		}
		m_TimerID = 0;
		AfxMessageBox(L"エラーが発生したので終了します。");
		PostQuitMessage(0);
	}

	CFormView::OnTimer(nIDEvent);
}


void CFbx2BinView::OnClickedButtonReadfile()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(TRUE);
	m_IsFbxReaded = FALSE;
	if (m_Directory == L"" || m_FbxFileName == L"" || m_Scale == L""){
		AfxMessageBox(L"ディレクトリ名かファイル名か縮尺率が空白です。");
		return;
	}
	//縮尺率が浮動小数点になってるかチェック
	float Scale = (float)_wtof(m_Scale);
	if (Scale <= 0.0f){
		AfxMessageBox(L"縮尺率が無効です。0以下になってませんか？");
		return;
	}
	int MashID = _wtoi(m_MeshID);
	if (MashID < 0){
		AfxMessageBox(L"メッシュIDが無効です。0未満になってませんか？");
		return;
	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//ファイルが間違いなく存在するかチェック
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_FbxFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode == 0xFFFFFFFF){
		AfxMessageBox(L"ファイル名見つかりません。");
		return;
	}
	if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
		AfxMessageBox(L"ファイルが無効です。ディレクトリではありませんか？");
		return;
	}
	else{
		//OK
		auto gstage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto PtrFbxObject = gstage->GetSharedGameObject<FbxMeshObject>(L"FbxMeshObject");
		auto Data = make_shared<FbxMeshObjData>();
		Data->m_DataDir = App::GetApp()->m_wstrRelativeDataPath + StrDir;
		Data->m_FbxFileName = m_FbxFileName;
		Data->m_MeshIndex = (size_t)MashID;
		Data->m_Scale = Scale;
		gstage->PostEvent(0, gstage, PtrFbxObject, L"DataReset", SharedToVoid(Data));

		auto DotIndex = m_FbxFileName.ReverseFind(L'.');
		if (DotIndex < 0){
			m_BinFileName = m_FbxFileName + m_BinFileNameExt;
		}
		else{
			m_BinFileName = m_FbxFileName.Mid(0, DotIndex) + m_BinFileNameExt;
		}
		UpdateData(FALSE);
		m_IsFbxReaded = TRUE;
	}
}


void CFbx2BinView::OnClickedButtonSavefile()
{
	if (!m_IsFbxReaded){
		AfxMessageBox(L"FBXが読み込まれてません。\r\nすでにモデルが表示されている場合は、もう一度読み込んでみてください。");
		return;
	}
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(TRUE);
	if (m_Directory == L"" || m_BinFileName == L"" || m_BinFileHeader == L""){
		AfxMessageBox(L"ディレクトリ名か保存ファイル名かヘッダが空白です。");
		return;
	}
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"スケーリング保存されるようにチェックされてますが、スケーリングが空白です。");
			return;
		}
		//縮尺率が浮動小数点になってるかチェック
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"縮尺率が無効です。0以下になってませんか？");
			return;
		}

	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//ファイルが存在するかチェック
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_BinFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode != 0xFFFFFFFF){
		//ファイルが存在した
		if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
			AfxMessageBox(L"ファイル名がディレクトリではありませんか？");
			return;
		}
		else{
			int i = AfxMessageBox(L"ファイルがありますが上書きしますか？", MB_OKCANCEL);
			if (i != IDOK){
				return;
			}
		}
	}
	//OK
	auto gstage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
	auto PtrFbxObject = gstage->GetSharedGameObject<FbxMeshObject>(L"FbxMeshObject");
	auto Data = make_shared<FbxMeshObjSaveData>();
	Data->m_DataDir = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	Data->m_BinFileName = m_BinFileName;
	Data->m_Header = m_BinFileHeader;
	Data->m_Scale = Scale;
	gstage->PostEvent(0, gstage, PtrFbxObject, L"DataSave", SharedToVoid(Data));

	wstring str = Data->m_DataDir + Data->m_BinFileName;
	str  +=  L"\r\nに保存しました。";
	AfxMessageBox(str.c_str());


	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}






void CFbx2BinView::OnClickedButtonSavefileSkin()
{
	if (!m_IsFbxReaded){
		AfxMessageBox(L"FBXが読み込まれてません。\r\nすでにモデルが表示されている場合は、もう一度読み込んでみてください。");
		return;
	}
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(TRUE);
	if (m_Directory == L"" || m_BinFileName == L"" || m_BinFileHeader == L""){
		AfxMessageBox(L"ディレクトリ名か保存ファイル名かヘッダが空白です。");
		return;
	}
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"スケーリング保存されるようにチェックされてますが、スケーリングが空白です。");
			return;
		}
		//縮尺率が浮動小数点になってるかチェック
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"縮尺率が無効です。0以下になってませんか？");
			return;
		}

	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//ファイルが存在するかチェック
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_BinFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode != 0xFFFFFFFF){
		//ファイルが存在した
		if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
			AfxMessageBox(L"ファイル名がディレクトリではありませんか？");
			return;
		}
		else{
			int i = AfxMessageBox(L"ファイルがありますが上書きしますか？", MB_OKCANCEL);
			if (i != IDOK){
				return;
			}
		}
	}
	//OK
	auto gstage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
	auto PtrFbxObject = gstage->GetSharedGameObject<FbxMeshObject>(L"FbxMeshObject");
	auto Data = make_shared<FbxMeshObjSaveData>();
	Data->m_DataDir = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	Data->m_BinFileName = m_BinFileName;
	Data->m_Header = m_BinFileHeader;
	Data->m_Scale = Scale;
	CString str;
	m_AnimeSampleStart.GetWindowTextW(str);
	Data->m_StartFrame = (UINT)_wtoi(str);
	m_AnimeSampleEnd.GetWindowTextW(str);
	Data->m_FrameCount = (UINT)_wtoi(str);
	if (Data->m_FrameCount == 0){
		AfxMessageBox(L"フレーム数が0個になってます");
		return;
	}
	m_AnimeSampleRate.GetWindowTextW(str);
	Data->m_FrameParSecond = (float)_wtof(str);
	if (Data->m_FrameParSecond < 1.0f){
		AfxMessageBox(L"秒あたりのフレーム数が1.0未満は指定できません。");
		return;
	}
	m_SampleSpan.GetWindowTextW(str);
	Data->m_SampleSpan = (float)_wtof(str);
	if (Data->m_SampleSpan < 0.01f){
		AfxMessageBox(L"秒あたりのサンプル数は0.01未満は指定できません。");
		return;
	}



	gstage->PostEvent(0, gstage, PtrFbxObject, L"DataSaveSkin", SharedToVoid(Data));

	wstring message = Data->m_DataDir + Data->m_BinFileName;
	message += L"\r\nに保存しました。";
	AfxMessageBox(message.c_str());

}


void CFbx2BinView::OnClickedButtonRunSkin()
{
	if (!m_IsFbxReaded){
		AfxMessageBox(L"FBXが読み込まれてません。\r\nすでにモデルが表示されている場合は、もう一度読み込んでみてください。");
		return;
	}
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(TRUE);
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"スケーリング保存されるようにチェックされてますが、スケーリングが空白です。");
			return;
		}
		//縮尺率が浮動小数点になってるかチェック
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"縮尺率が無効です。0以下になってませんか？");
			return;
		}

	}
	//OK
	auto gstage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
	auto PtrFbxObject = gstage->GetSharedGameObject<FbxMeshObject>(L"FbxMeshObject");
	auto Data = make_shared<FbxMeshObjSaveData>();
	Data->m_DataDir = L"";
	Data->m_BinFileName = L"";
	Data->m_Header = L"";
	Data->m_Scale = Scale;
	CString str;
	m_AnimeSampleStart.GetWindowTextW(str);
	Data->m_StartFrame = (UINT)_wtoi(str);
	m_AnimeSampleEnd.GetWindowTextW(str);
	Data->m_FrameCount = (UINT)_wtoi(str);
	if (Data->m_FrameCount == 0){
		AfxMessageBox(L"フレーム数が0個になってます");
		return;
	}
	m_AnimeSampleRate.GetWindowTextW(str);
	Data->m_FrameParSecond = (float)_wtof(str);
	if (Data->m_FrameParSecond < 1.0f){
		AfxMessageBox(L"秒あたりのフレーム数が1.0未満は指定できません。");
		return;
	}
	gstage->PostEvent(0, gstage, PtrFbxObject, L"RunSkin", SharedToVoid(Data));
}



