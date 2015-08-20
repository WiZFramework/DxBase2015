#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct AudioManager::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct AudioManager::Impl{
		bool m_audioAvailable;
		ComPtr<IXAudio2>    m_musicEngine;
		ComPtr<IXAudio2>    m_soundEffectEngine;
		IXAudio2MasteringVoice* m_musicMasteringVoice;
		IXAudio2MasteringVoice* m_soundEffectMasteringVoice;
		Impl() :
			m_audioAvailable{false}
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	//--------------------------------------------------------------------------------------
	AudioManager::AudioManager() :
		pImpl(new Impl)
	{
	}
	AudioManager::~AudioManager(){}

	void AudioManager::CreateDeviceIndependentResources()
	{
		UINT32 flags = 0;

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_musicEngine, flags),
			L"���y�p�I�[�f�B�I�G���W���̏������Ɏ��s���܂���",
			L"XAudio2Create(&m_musicEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);

#if defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
		debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
		debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
		pImpl->m_musicEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		HRESULT hr = pImpl->m_musicEngine->CreateMasteringVoice(&pImpl->m_musicMasteringVoice);
		if (FAILED(hr))
		{
			throw BaseException(
				L"���y�̃}�X�^�����O�{�C�X�̏������Ɏ��s���܂���",
				L"m_musicEngine->CreateMasteringVoice(&m_musicMasteringVoice)",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
			// Unable to create an audio device
			pImpl->m_audioAvailable = false;
			return;
		}

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_soundEffectEngine, flags),
			L"�T�E���h�p�I�[�f�B�I�G���W���̏������Ɏ��s���܂���",
			L"XAudio2Create(&m_soundEffectEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);
#if defined(_DEBUG)
		pImpl->m_soundEffectEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		ThrowIfFailed(
			pImpl->m_soundEffectEngine->CreateMasteringVoice(&pImpl->m_soundEffectMasteringVoice),
			L"�T�E���h�p�}�X�^�����O�{�C�X�̏������Ɏ��s���܂���",
			L"m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);
		pImpl->m_audioAvailable = true;
	}

	IXAudio2* AudioManager::GetMusicEngine()const
	{
		return pImpl->m_musicEngine.Get();
	}

	IXAudio2* AudioManager::GetSoundEffectEngine()const
	{
		return pImpl->m_soundEffectEngine.Get();
	}

	void AudioManager::SuspendAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			pImpl->m_musicEngine->StopEngine();
			pImpl->m_soundEffectEngine->StopEngine();
		}
	}

	void AudioManager::ResumeAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			ThrowIfFailed(
				pImpl->m_musicEngine->StartEngine(),
				L"���y�p�G���W���̃X�^�[�g�Ɏ��s���܂���",
				L"m_musicEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
			ThrowIfFailed(
				pImpl->m_soundEffectEngine->StartEngine(),
				L"�T�E���h�p�G���W���̃X�^�[�g�Ɏ��s���܂���",
				L"m_soundEffectEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
		}
	}




	//--------------------------------------------------------------------------------------
	//	class App;
	//	�p�r: �A�v���P�[�V�����N���X
	//--------------------------------------------------------------------------------------
	//static�ϐ�����
	unique_ptr<App, App::AppDeleter> App::m_App;
	//�\�z
	App::App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height) :
		m_hInstance{ hInstance },
		m_hWnd{ hWnd },
		m_FullScreen{ FullScreen },
		m_GameWidth{ Width },
		m_GameHeight{ Height },
		m_Timer()
	{
		try{
			//�f�o�C�X���\�[�X�̍\�z
			m_DeviceResources = shared_ptr<DeviceResources>(new DeviceResources(hWnd, FullScreen, Width, Height));
			//��f�B���N�g���̐ݒ�
			//���΃p�X�ɂ���ƁA�t�@�C���_�C�A���O�ŃJ�����g�p�X�������̂�
			//��΃p�X�w��
			wchar_t Modulebuff[MAX_PATH];
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];

			::ZeroMemory(Modulebuff, sizeof(Modulebuff));
			::ZeroMemory(Drivebuff, sizeof(Drivebuff));
			::ZeroMemory(Dirbuff, sizeof(Dirbuff));
			::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
			::ZeroMemory(Extbuff, sizeof(Extbuff));

			//���W���[�����i�v���O�����t�@�C�����j�𓾂�
			if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))){
				throw BaseException(
					L"���W���[�����擾�ł��܂���B",
					L"if(!::GetModuleFileName())",
					L"App::App()"
					);
			}
			m_wstrModulePath = Modulebuff;
			//���W���[��������A�e�u���b�N�ɕ�����
			_wsplitpath_s(Modulebuff,
				Drivebuff, _MAX_DRIVE,
				Dirbuff, _MAX_DIR,
				FileNamebuff, _MAX_FNAME,
				Extbuff, _MAX_EXT);

			//�h���C�u���̎擾
			m_wstrDir = Drivebuff;
			//�f�B���N�g�����̎擾
			m_wstrDir += Dirbuff;
			//media�f�B���N�g����T��
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"media";
			//�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
			DWORD RetCode;
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF){
				//���s����
				m_wstrDataPath = m_wstrDir;
				m_wstrDataPath += L"..\\media";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF){
					//�Ăю��s����
					throw BaseException(
						L"media�f�B���N�g�����m�F�ł��܂���B",
						L"if(RetCode == 0xFFFFFFFF)",
						L"App::App()"
						);
				}
				else{
					m_wstrDataPath += L"\\";
					//���΃p�X�̐ݒ�
					m_wstrRelativeDataPath = L"..\\media\\";
				}
			}
			else{
				m_wstrDataPath += L"\\";
				//���΃p�X�̐ݒ�
				m_wstrRelativeDataPath = L"media\\";
			}
			m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
			m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";

			//�I�[�f�B�I�}�l�[�W���̎擾
			GetAudioManager();

			//�����̏�����
			srand((unsigned)time(nullptr));


		}
		catch (...){
			throw;
		}
	}

	//�V���O���g���\�z�ƃA�N�Z�T
	unique_ptr<App, App::AppDeleter>& App::GetApp(HINSTANCE hInstance, HWND hWnd,
		bool FullScreen, UINT Width, UINT Height){
		try{
			if (m_App.get() == 0){
				//�������g�̍\�z
				m_App.reset(new App(hInstance, hWnd, FullScreen, Width, Height));
			}
			return m_App;
		}
		catch (...){
			throw;
		}

	}
	//�����j��
	void App::DeleteApp(){
		if (m_App.get()){
			m_App.reset();
		}
	}

	//FBX�}�l�[�W���̎擾
	unique_ptr<FbxManager, App::FbxManagerDeleter>& App::GetFbxManager(){
		try{
			if (m_pFbxManager.get() == 0){
				m_pFbxManager.reset(FbxManager::Create());
				FbxIOSettings *ios = FbxIOSettings::Create(m_pFbxManager.get(), IOSROOT);
				m_pFbxManager->SetIOSettings(ios);

			}
			return m_pFbxManager;
		}
		catch (...){
			throw;
		}
	}

	// �I�[�f�B�I�}�l�[�W���̎擾
	unique_ptr<AudioManager>& App::GetAudioManager(){
		try{
			if (m_AudioManager.get() == 0){
				m_AudioManager.reset(new AudioManager);
				m_AudioManager->CreateDeviceIndependentResources();

			}
			return m_AudioManager;
		}
		catch (...){
			throw;
		}
	}



	void App::SetSceneBase(const shared_ptr<SceneBase>& ptr){
		if (m_SceneBase.get()){
			m_SceneBase.reset();
		}
		m_SceneBase = ptr;
	}

	void App::RegisterResource(const wstring& Key,const shared_ptr<BaseResource>& ResObj){
		try{
			if (Key == L""){
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::RegisterResource()"
					);
			}
			if (!ResObj){
				throw BaseException(
					L"���\�[�X���s��ł�",
					L"if(!pResObj)",
					L"App::RegisterResource()"
					);
			}
			map<wstring, shared_ptr<BaseResource> >::iterator it;
			//�d���|�C���^�̌���
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++){
				if (it->second == ResObj){
					//�|�C���^���d�����Ă��Ă��A�L�[�������Ȃ�
					//���łɓo�^����Ă���̂Ń��^�[��
					if (it->first == Key){
						return;
					}
					wstring keyerr = Key;
					throw BaseException(
						L"���łɂ��̃��\�[�X�͓o�^����Ă��܂�",
						keyerr,
						L"App::RegisterResource()"
						);
				}
			}
			//�d���L�[�̌���
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//�w��̖��O����������
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"���łɂ��̃L�[�͓o�^����Ă��܂�",
					keyerr,
					L"App::RegisterResource()"
					);
			}
			else{
				//������Ȃ�
				//���\�[�X�y�A�̒ǉ�
				m_ResMap[Key] = ResObj;
			}
		}
		catch (...){
			throw;
		}
	}

	//�e�N�X�`���̓o�^(�����L�[�̃e�N�X�`�����Ȃ���΃t�@�C�����ō쐬���A�o�^)
	//�������O�̃e�N�X�`��������΂��̃|�C���^��Ԃ�
	shared_ptr<TextureResource> App::RegisterTexture(const wstring& Key, const wstring& TextureFileName, const wstring& TexType){
		if (CheckResource<TextureResource>(Key)){
			return GetResource<TextureResource>(Key);
		}
		//
		auto PtrTexture = Object::CreateObject<TextureResource>(TextureFileName, TexType);
		RegisterResource(Key, PtrTexture);
		return PtrTexture;
	}

	//--------------------------------------------------------------------------------------
	//	shared_ptr<AudioResource> RegisterWav(
	//		const wstring& Key, // ���\�[�X�L�[
	//		const wstring& WavFileName	//Wav�t�@�C����
	//	);
	shared_ptr<AudioResource> App::RegisterWav(const wstring& Key, const wstring& WavFileName){
		if (CheckResource<AudioResource>(Key)){
			return GetResource<AudioResource>(Key);
		}
		//
		auto PtrWav = Object::CreateObject<AudioResource>(WavFileName);
		RegisterResource(Key, PtrWav);
		return PtrWav;
	}



	//--------------------------------------------------------------------------------------
	//	shared_ptr<FbxSceneResource> RegisterFbxScene(
	//		const wstring& Key,			//�L�[
	//		const wstring& BaseDir.		//��f�B���N�g��
	//		const wstring& FbxFileName	//FbxFile��
	//	);
	//--------------------------------------------------------------------------------------
	shared_ptr<FbxSceneResource> App::RegisterFbxScene(const wstring& Key, const wstring& BaseDir, const wstring& FbxFileName){
		try{
			if (CheckResource<FbxSceneResource>(Key)){
				return GetResource<FbxSceneResource>(Key);
			}
			auto PtrFbxScene = FbxSceneResource::CreateFbxScene(BaseDir, FbxFileName);
			RegisterResource(Key, PtrFbxScene);
			return PtrFbxScene;
		}
		catch (...){
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	shared_ptr<FbxMeshResource> RegisterFbxMesh(
	//		const wstring& Key,			//�L�[
	//		const shared_ptr<FbxSceneResource>& FbxSceneRes		//���̃��b�V�����܂܂��V�[�����\�[�X
	//		UINT MeshID					//���b�V���̃V�[������ID
	//	);
	//--------------------------------------------------------------------------------------
	shared_ptr<FbxMeshResource> App::RegisterFbxMesh(const wstring& Key, const shared_ptr<FbxSceneResource>& FbxSceneRes, UINT MeshID){
		try{
			if (CheckResource<FbxMeshResource>(Key)){
				return GetResource<FbxMeshResource>(Key);
			}
			auto PtrFbxMesh = FbxSceneRes->GetFbxMeshResource(MeshID);
			RegisterResource(Key, PtrFbxMesh);
			return PtrFbxMesh;
		}
		catch (...){
			throw;
		}
	}
	void App::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		m_SceneBase->OnMessage(message,wParam, lParam);
	}
	//����
	void App::Update(){
		//���͋@���Ԃ����Z�b�g
		m_InputDevice.ResetControlerState();
		// �V�[�� �I�u�W�F�N�g���X�V���܂��B
		m_Timer.Tick([&]()
		{
			m_SceneBase->Update();
		});
	}
	bool App::Draw(){
		// ����X�V�O�Ƀ����_�����O�͍s��Ȃ��B
		if (m_Timer.GetFrameCount() == 0)
		{
			return false;
		}
		m_SceneBase->Draw();
		return true;
	}
	void App::Present(UINT SyncInterval, UINT Flags){
		// �o�b�N�o�b�t�@����t�����g�o�b�t�@�ɓ]��
		GetDeviceResources()->GetSwapChain()->Present(SyncInterval, Flags);
	}




}



//end basedx11
