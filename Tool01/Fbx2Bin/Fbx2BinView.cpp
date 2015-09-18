
// Fbx2BinView.cpp : CFbx2BinView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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

// CFbx2BinView �R���X�g���N�V����/�f�X�g���N�V����

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
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

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
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

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



// CFbx2BinView �f�f

#ifdef _DEBUG
void CFbx2BinView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFbx2BinView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFbx2BinDoc* CFbx2BinView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFbx2BinDoc)));
	return (CFbx2BinDoc*)m_pDocument;
}
#endif //_DEBUG


// CFbx2BinView ���b�Z�[�W �n���h���[


int CFbx2BinView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	auto ClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH));
	CRect r(0, 0, 640, 480);
	r += CPoint(256, 16);
	m_DxWindow.Create(ClassName, L"", WS_CHILD | WS_VISIBLE, r, this, 0);



	try{
		////�A�v���P�[�V�����N���X�̍\�z
		App::GetApp(lpCreateStruct->hInstance, m_DxWindow.GetSafeHwnd(), false, 640, 480);
		//�V�[���̒ǉ��o�^
		auto PtrScene = App::GetApp()->AddScene<Scene>();
		PtrScene->SetView(this);


		m_TimerID = SetTimer(1, 10, NULL);
	}
	catch (...){
		if (m_TimerID != 0){
			KillTimer(m_TimerID);
		}
		m_TimerID = 0;
		AfxMessageBox(L"�G���[�ł��B�I�����܂��B");
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
			//�t���C���g�o�b�t�@�ɓ]��
			App::GetApp()->Present(1, 0);
		}
	}
	catch (...){
		if (m_TimerID != 0){
			KillTimer(m_TimerID);
		}
		m_TimerID = 0;
		AfxMessageBox(L"�G���[�����������̂ŏI�����܂��B");
		PostQuitMessage(0);
	}

	CFormView::OnTimer(nIDEvent);
}


void CFbx2BinView::OnClickedButtonReadfile()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	m_IsFbxReaded = FALSE;
	if (m_Directory == L"" || m_FbxFileName == L"" || m_Scale == L""){
		AfxMessageBox(L"�f�B���N�g�������t�@�C�������k�ڗ����󔒂ł��B");
		return;
	}
	//�k�ڗ������������_�ɂȂ��Ă邩�`�F�b�N
	float Scale = (float)_wtof(m_Scale);
	if (Scale <= 0.0f){
		AfxMessageBox(L"�k�ڗ��������ł��B0�ȉ��ɂȂ��Ă܂��񂩁H");
		return;
	}
	int MashID = _wtoi(m_MeshID);
	if (MashID < 0){
		AfxMessageBox(L"���b�V��ID�������ł��B0�����ɂȂ��Ă܂��񂩁H");
		return;
	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//�t�@�C�����ԈႢ�Ȃ����݂��邩�`�F�b�N
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_FbxFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode == 0xFFFFFFFF){
		AfxMessageBox(L"�t�@�C����������܂���B");
		return;
	}
	if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
		AfxMessageBox(L"�t�@�C���������ł��B�f�B���N�g���ł͂���܂��񂩁H");
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
		AfxMessageBox(L"FBX���ǂݍ��܂�Ă܂���B\r\n���łɃ��f�����\������Ă���ꍇ�́A������x�ǂݍ���ł݂Ă��������B");
		return;
	}
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	if (m_Directory == L"" || m_BinFileName == L"" || m_BinFileHeader == L""){
		AfxMessageBox(L"�f�B���N�g�������ۑ��t�@�C�������w�b�_���󔒂ł��B");
		return;
	}
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"�X�P�[�����O�ۑ������悤�Ƀ`�F�b�N����Ă܂����A�X�P�[�����O���󔒂ł��B");
			return;
		}
		//�k�ڗ������������_�ɂȂ��Ă邩�`�F�b�N
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"�k�ڗ��������ł��B0�ȉ��ɂȂ��Ă܂��񂩁H");
			return;
		}

	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//�t�@�C�������݂��邩�`�F�b�N
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_BinFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode != 0xFFFFFFFF){
		//�t�@�C�������݂���
		if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
			AfxMessageBox(L"�t�@�C�������f�B���N�g���ł͂���܂��񂩁H");
			return;
		}
		else{
			int i = AfxMessageBox(L"�t�@�C��������܂����㏑�����܂����H", MB_OKCANCEL);
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
	str  +=  L"\r\n�ɕۑ����܂����B";
	AfxMessageBox(str.c_str());


	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}






void CFbx2BinView::OnClickedButtonSavefileSkin()
{
	if (!m_IsFbxReaded){
		AfxMessageBox(L"FBX���ǂݍ��܂�Ă܂���B\r\n���łɃ��f�����\������Ă���ꍇ�́A������x�ǂݍ���ł݂Ă��������B");
		return;
	}
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	if (m_Directory == L"" || m_BinFileName == L"" || m_BinFileHeader == L""){
		AfxMessageBox(L"�f�B���N�g�������ۑ��t�@�C�������w�b�_���󔒂ł��B");
		return;
	}
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"�X�P�[�����O�ۑ������悤�Ƀ`�F�b�N����Ă܂����A�X�P�[�����O���󔒂ł��B");
			return;
		}
		//�k�ڗ������������_�ɂȂ��Ă邩�`�F�b�N
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"�k�ڗ��������ł��B0�ȉ��ɂȂ��Ă܂��񂩁H");
			return;
		}

	}
	wstring StrDir = m_Directory;
	if (m_Directory.Right(0) != L"\\"){
		StrDir += L"\\";
	}
	//�t�@�C�������݂��邩�`�F�b�N
	wstring ChkSrr = App::GetApp()->m_wstrRelativeDataPath + StrDir;
	ChkSrr += m_BinFileName;

	DWORD RetCode;
	RetCode = GetFileAttributes(ChkSrr.c_str());
	if (RetCode != 0xFFFFFFFF){
		//�t�@�C�������݂���
		if (RetCode & FILE_ATTRIBUTE_DIRECTORY){
			AfxMessageBox(L"�t�@�C�������f�B���N�g���ł͂���܂��񂩁H");
			return;
		}
		else{
			int i = AfxMessageBox(L"�t�@�C��������܂����㏑�����܂����H", MB_OKCANCEL);
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
		AfxMessageBox(L"�t���[������0�ɂȂ��Ă܂�");
		return;
	}
	m_AnimeSampleRate.GetWindowTextW(str);
	Data->m_FrameParSecond = (float)_wtof(str);
	if (Data->m_FrameParSecond < 1.0f){
		AfxMessageBox(L"�b������̃t���[������1.0�����͎w��ł��܂���B");
		return;
	}
	m_SampleSpan.GetWindowTextW(str);
	Data->m_SampleSpan = (float)_wtof(str);
	if (Data->m_SampleSpan < 0.01f){
		AfxMessageBox(L"�b������̃T���v������0.01�����͎w��ł��܂���B");
		return;
	}



	gstage->PostEvent(0, gstage, PtrFbxObject, L"DataSaveSkin", SharedToVoid(Data));

	wstring message = Data->m_DataDir + Data->m_BinFileName;
	message += L"\r\n�ɕۑ����܂����B";
	AfxMessageBox(message.c_str());

}


void CFbx2BinView::OnClickedButtonRunSkin()
{
	if (!m_IsFbxReaded){
		AfxMessageBox(L"FBX���ǂݍ��܂�Ă܂���B\r\n���łɃ��f�����\������Ă���ꍇ�́A������x�ǂݍ���ł݂Ă��������B");
		return;
	}
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	float Scale = 1.0f;
	if (m_IsScaling){
		if (m_Scale == L""){
			AfxMessageBox(L"�X�P�[�����O�ۑ������悤�Ƀ`�F�b�N����Ă܂����A�X�P�[�����O���󔒂ł��B");
			return;
		}
		//�k�ڗ������������_�ɂȂ��Ă邩�`�F�b�N
		Scale = (float)_wtof(m_Scale);
		if (Scale <= 0.0f){
			AfxMessageBox(L"�k�ڗ��������ł��B0�ȉ��ɂȂ��Ă܂��񂩁H");
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
		AfxMessageBox(L"�t���[������0�ɂȂ��Ă܂�");
		return;
	}
	m_AnimeSampleRate.GetWindowTextW(str);
	Data->m_FrameParSecond = (float)_wtof(str);
	if (Data->m_FrameParSecond < 1.0f){
		AfxMessageBox(L"�b������̃t���[������1.0�����͎w��ł��܂���B");
		return;
	}
	gstage->PostEvent(0, gstage, PtrFbxObject, L"RunSkin", SharedToVoid(Data));
}



