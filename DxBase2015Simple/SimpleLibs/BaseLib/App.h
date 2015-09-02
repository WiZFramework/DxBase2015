/*! 
@file App.h
@brief �A�v���P�[�V�����N���X�B���͋@�퓙

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basedx11{

	class DeviceResources;
	class RenderTarget;
	class StageBase;
	class EventDispatcher;

	//--------------------------------------------------------------------------------------
	//	struct CONTROLER_STATE : public XINPUT_STATE;
	/*!
	XBox�R���g���[���[�̃X�e�[�^�X.
	XINPUT_STATE����̃G�C���A�X.
	*/
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		//!< �{�^�����
		BYTE bLeftTrigger;	//!< ���g���K
		BYTE bRightTrigger;	//!< �E�g���K

		float fThumbRX;	//!< �E�X�e�B�b�NX��flort�ϊ�
		float fThumbRY;	//!< �E�X�e�B�b�NY��flort�ϊ�
		float fThumbLX;	//!< ���X�e�B�b�NX��flort�ϊ�
		float fThumbLY;	//!< ���X�e�B�b�NY��flort�ϊ�
		WORD wNowUpdateButtons;	//!< ��Ԃ��ς�����Ƃ��Ƀt���O�����{�^��
		WORD wPressedButtons;	//!< �����ꂽ�u�Ԃ���������{�^��
		WORD wReleasedButtons;	//!< �����ꂽ�u�Ԃ���������{�^��
		WORD wLastButtons;	//!< ��O�̃{�^��
		bool bConnected;	//!< �ڑ����Ă��邩�ǂ���
	};
	//--------------------------------------------------------------------------------------
	//	struct KEYBOARD_STATE;
	/*!
	�L�[�{�[�h�̃X�e�[�^�X.
	�}�E�X�̃X�e�[�^�X���擾����
	*/
	//--------------------------------------------------------------------------------------
	struct KEYBOARD_STATE{
		static const DWORD MAX_KEYVCODE = 256;
		bool m_bPushKeyTbl[MAX_KEYVCODE];	//!< ���݂̃L�[�{�[�h�̏��
		bool m_bLastKeyTbl[MAX_KEYVCODE];	//!< ��O�̃L�[�{�[�h�̏��
		bool m_bPressedKeyTbl[MAX_KEYVCODE];	//!< �����ꂽ�u�Ԃ̃L�[�{�[�h
		bool m_bUpKeyTbl[MAX_KEYVCODE];		//!< �����ꂽ�u�Ԃ̃L�[�{�[�h
		bool m_KeyMessageActive;	//!<�����̃L�[�C�x���g������
		Point2D<int> m_MouseClientPoint;
		//--------------------------------------------------------------------------------------
		//	KEYBOARD_STATE();
		/*!
		�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		KEYBOARD_STATE() :
			m_KeyMessageActive{ false },
			m_MouseClientPoint{ 0, 0 }
		{
			//�L�[�{�[�h�e�[�u���̏�����
			::ZeroMemory(&m_bLastKeyTbl, sizeof(m_bLastKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
		}
		//--------------------------------------------------------------------------------------
		//	bool IsMouseEnabled(
		//			HWND hWnd,	//�E�C���h�E�̃n���h���B
		//			vector<DWORD>& UseKeyVec	//�g�p����L�[�R�[�h�z��
		//	);
		/*!
		@breaf	�}�E�X�g�p�Ń}�E�X���N���C�A���g�̈�ɂ��邩�ǂ����̃`�F�b�N
		@param	HWND hWnd,	�E�C���h�E�̃n���h���B
		@param	vector<DWORD>& UseKeyVec: �g�p����L�[�R�[�h�z��
		@return �}�E�X�g�p�Ń}�E�X���N���C�A���g�̈�ɂ����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsMouseEnabled(HWND hWnd, vector<DWORD>& UseKeyVec){
			//�}�E�X�|�C���g��0,0�ɏ�����
			m_MouseClientPoint = { 0, 0 };
			//�}�E�X���p�ɂ�����炸�A�}�E�X�|�C���^���W�͐ݒ�
			POINT p;
			::GetCursorPos(&p);
			if (::ScreenToClient(hWnd, &p)){
				//�N���C�A���g���W�ɕϊ��ł����Ƃ��̂݁A�}�E�X���W��ݒ�
				m_MouseClientPoint = { p.x, p.y };
			}
			else{
				return false;
			}

			//UseKeyVec�Ƀ}�E�X���������ꍇ
			vector<DWORD> MouseTemp = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
			bool MouseChk = false;
			for (auto chk : MouseTemp){
				auto it = find(UseKeyVec.begin(), UseKeyVec.end(), chk);
				if (it != UseKeyVec.end()){
					MouseChk = true;
					break;
				}
			}
			//�}�E�X�g�p�ō��W���N���C�A���g�̈���Ȃ�true
			if (MouseChk){
				RECT rc;
				::GetClientRect(hWnd, &rc);
				if (::PtInRect(&rc, p)){
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		//	bool GetKeyState(
		//			HWND hWnd,	//�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		//			vector<DWORD>& UseKeyVec	//�g�p����L�[�R�[�h�z��
		//	);
		/*!
		@breaf	�L�[�X�e�[�g�𓾂�
		@param	HWND hWnd,	�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		@param	vector<DWORD>& UseKeyVec: �g�p����L�[�R�[�h�z��
		@return �L�[���͂������true
		*/
		//--------------------------------------------------------------------------------------
		bool GetKeyState(HWND hWnd, vector<DWORD>& UseKeyVec){
			m_KeyMessageActive = false;
			//��O�ɃR�s�[
			CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
			//�L�[�{�[�h�̏�Ԃ�������
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
			//�}�E�X�̃`�F�b�N
			bool MouseEnabled = IsMouseEnabled(hWnd, UseKeyVec);
			//�������g�Ƀt�H�[�J�X���Ȃ��ꍇ��false
			if (::GetFocus() != hWnd){
				return false;
			}
			SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
			if (ret & 0x8000){
				//Esc�L�[�͖�������false
				return false;
			}
			size_t sz = UseKeyVec.size();
			for (size_t i = 0; i < sz; i++){
				ret = GetAsyncKeyState((int)UseKeyVec[i]);
				if (UseKeyVec[i] == VK_LBUTTON || UseKeyVec[i] == VK_RBUTTON || UseKeyVec[i] == VK_MBUTTON){
					if (!MouseEnabled){
						//�}�E�X�������Ȃ�Used�ɓ����Ă��Ă�����
						continue;
					}
				}
				if (ret & 0x8000){
					m_bPushKeyTbl[UseKeyVec[i]] = true;
					m_KeyMessageActive = true;	//���b�Z�[�W���Q�[�����󂯎��
					if (!m_bLastKeyTbl[UseKeyVec[i]]){
						//�O�񉟂���ĂȂ��č��񉟂��ꂽ
						m_bPressedKeyTbl[UseKeyVec[i]] = true;
					}
				}
				else{
					//�L�[�͉�����ĂȂ�
					if (m_bLastKeyTbl[UseKeyVec[i]]){
						//�O�񉟂���Ă��č��񉟂���ĂȂ�
						m_bUpKeyTbl[UseKeyVec[i]] = true;
						m_KeyMessageActive = true;	//���b�Z�[�W���Q�[�����󂯎��
					}
				}
			}
			return m_KeyMessageActive;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class InputDevice;
	/*!
	���͋@��
	*/
	//--------------------------------------------------------------------------------------
	class InputDevice{
		static const DWORD MAX_CONTROLLERS = 4;
		vector<CONTROLER_STATE> m_State;
		KEYBOARD_STATE m_KeyState;
	public:
		//--------------------------------------------------------------------------------------
		//	InputDevice();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		InputDevice() :
			m_State(MAX_CONTROLLERS),
			m_KeyState()
		{
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++){
				::ZeroMemory(&m_State[i], sizeof(CONTROLER_STATE));
			}
		}
		//--------------------------------------------------------------------------------------
		//	~InputDevice();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		~InputDevice(){}
		//--------------------------------------------------------------------------------------
		//	bool ResetInputState(
		//		HWND hWnd,	//�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		//		vector<DWORD>& UseKeyVec	//�g�p����L�[�̔z��
		//	);
		/*!
		@breaf �}�E�X�ƃL�[�{�[�h�̏�Ԃ𓾂�
		@param HWND hWnd,	�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		@param vector<DWORD>& UseKeyVec	�g�p����L�[
		@return	�}�E�X���L�[�{�[�h���b�Z�[�W�������true
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec){
			bool Ret1 = m_KeyState.GetKeyState(hWnd, UseKeyVec);
			return Ret1;
		}
		//--------------------------------------------------------------------------------------
		//	void ResetControlerState();
		/*!
		@breaf �R���g���[���[�̏�Ԃ�m_State�Ɏ擾����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState(){
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//��O�̏�Ԃ�ۑ�
				m_State[i].wLastButtons = m_State[i].Gamepad.wButtons;
				// XInput���猻�݂̃X�e�[�^�X�𓾂�
				XINPUT_STATE State;
				dwResult = XInputGetState(i, &State);
				if (dwResult == ERROR_SUCCESS){
					m_State[i].dwPacketNumber = State.dwPacketNumber;
					m_State[i].Gamepad = State.Gamepad;
					m_State[i].bConnected = true;
					//�{�^���̐ݒ�
					//�R�s�[
					m_State[i].wButtons = m_State[i].Gamepad.wButtons;
					//�X�V�{�^��
					m_State[i].wNowUpdateButtons = m_State[i].Gamepad.wButtons;
					m_State[i].bLeftTrigger = m_State[i].Gamepad.bLeftTrigger;
					m_State[i].bRightTrigger = m_State[i].Gamepad.bRightTrigger;
					//�O����ω����Ă���{�^���̂݃t���O�𗧂Ă�
					//XOR���Z�ɂ�����
					m_State[i].wNowUpdateButtons ^= m_State[i].wLastButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_State[i].wPressedButtons = m_State[i].wNowUpdateButtons & m_State[i].wButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_State[i].wReleasedButtons = m_State[i].wNowUpdateButtons & (m_State[i].wButtons ^ 0xffff);
					//���o�[�̃f�b�h�]�[���̐ݒ�
					if ((m_State[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbLX = 0;
						m_State[i].Gamepad.sThumbLY = 0;
					}
					if (m_State[i].Gamepad.sThumbLX >= 0){
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)SHRT_MAX;
					}
					else{
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbLY >= 0){
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)SHRT_MAX;
					}
					else{
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)(SHRT_MAX + 1);
					}

					if ((m_State[i].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbRX = 0;
						m_State[i].Gamepad.sThumbRY = 0;
					}
					if (m_State[i].Gamepad.sThumbRX >= 0){
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)SHRT_MAX;
					}
					else{
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbRY >= 0){
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)SHRT_MAX;
					}
					else{
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)(SHRT_MAX + 1);
					}
				}
				else{
					m_State[i].bConnected = false;
				}
			}
		}
		//	�A�N�Z�T
		//!	�L�[�X�e�[�g�̎擾
		const KEYBOARD_STATE& GetKeyState() const{ return m_KeyState; }
		//!	�R���g���[���X�e�[�g�̎擾
		const vector<CONTROLER_STATE>& GetControlerVec()const { return m_State; }
	};

	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	/*!
	�I�[�f�B�I�}�l�[�W���N���X
	*/
	//--------------------------------------------------------------------------------------
	class AudioManager{
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	AudioManager();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AudioManager();
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioManager();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioManager();
		//--------------------------------------------------------------------------------------
		//	void CreateDeviceIndependentResources();
		/*!
		@breaf �I�[�f�B�I�̏�����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateDeviceIndependentResources();
		//--------------------------------------------------------------------------------------
		//	IXAudio2* GetMusicEngine()const;
		/*!
		@breaf �~���[�W�b�N�G���W���̎擾
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetMusicEngine()const;
		//--------------------------------------------------------------------------------------
		//	IXAudio2* GetMusicEngine()const;
		/*!
		@breaf �T�E���h�G�t�F�N�g�G���W���̎擾
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetSoundEffectEngine()const;
		//--------------------------------------------------------------------------------------
		//	SuspendAudio();
		/*!
		@breaf �I�[�f�B�I�̒��f
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SuspendAudio();
		//--------------------------------------------------------------------------------------
		//	ResumeAudio();
		/*!
		@breaf �I�[�f�B�I�̍ĊJ
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ResumeAudio();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		//���[�u�֎~
		AudioManager(const AudioManager&&) = delete;
		AudioManager& operator=(const AudioManager&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	class App;
	/*!
		�A�v���P�[�V�����N���X
	*/
	//--------------------------------------------------------------------------------------
	class App{
		// �f���[�^�[
		struct AppDeleter
		{
			void operator()(App *p) { delete p; }
		};
		static unique_ptr<App, AppDeleter> m_App;		//Singleton�ŗ��p���鎩�����g�̃|�C���^
		HINSTANCE m_hInstance;							//���W���[���̃C���X�^���X
		HWND m_hWnd;									//���C���E�C���h�E�̃n���h��
		bool m_FullScreen;								//�t���X�N���[�����ǂ���
		UINT m_GameWidth;								//�Q�[���Օ�(�s�N�Z��)
		UINT m_GameHeight;								//�Q�[���Ս���(�s�N�Z��)
		shared_ptr<DeviceResources> m_DeviceResources;	//�f�o�C�X
		shared_ptr<StageBase> m_StageBase;				//�Q�[���X�e�[�W
		shared_ptr<StageBase> m_NextStageBase;			//���̃Q�[���X�e�[�W
		StepTimer m_Timer;								//�^�C�}�[
		//�I�[�f�B�I�}�l�[�W��
		unique_ptr<AudioManager> m_AudioManager;
		//�C�x���g�z���N���X
		shared_ptr<EventDispatcher> m_EventDispatcher;	
		//�\�z�Ɣj��
		App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height);
		virtual ~App(){}
	public:
		//! �V���O���g���\�z�ƃA�N�Z�T
		static unique_ptr<App, AppDeleter>& GetApp(HINSTANCE hInstance = 0, HWND hWnd = 0,
			bool FullScreen = false, UINT Width = 0, UINT Height = 0);
		//! �����j��
		static void DeleteApp();
		//�A�N�Z�T
		//! �A�v���P�[�V�����̃C���X�^���X�擾
		HINSTANCE GetHInstance() const { return m_hInstance; }
		//! �E�C���h�E�̃n���h���̎擾
		HWND GetHWnd() const { return m_hWnd; }
		//! �I�[�f�B�I�}�l�[�W���̎擾
		unique_ptr<AudioManager>& GetAudioManager();
		//!�@�C�x���g�z���I�u�W�F�N�g�̎擾
		shared_ptr<EventDispatcher> GetEventDispatcher();
		//! �t���X�N���[�����ǂ���
		bool IsFullScreen() const { return m_FullScreen; }
		bool GetFullScreen() const { return m_FullScreen; }
		//! �Q�[���Ղ̕��̎擾
		UINT GetGameWidth() const { return m_GameWidth; }
		//! �Q�[���Ղ̍����̎擾
		UINT GetGameHeight() const { return m_GameHeight; }
		//! �f�o�C�X���\�[�X�̎擾
		shared_ptr<DeviceResources> GetDeviceResources() const { return m_DeviceResources; }
		//! �f�o�C�X���\�[�X�̐ݒ�
		void SetDeviceResources(const shared_ptr<DeviceResources>& ptr){ m_DeviceResources = ptr; }
		//!�Q�[���X�e�[�W�̎擾
		shared_ptr<StageBase> GetStageBase() const { return m_StageBase; }
		//!�Q�[���X�e�[�W�̎擾�e���v���[�g
		template<typename T>
		shared_ptr<T> GetStage() const { 
			shared_ptr<T> Ret = dynamic_pointer_cast<T>(m_StageBase);
			if (Ret){
				return Ret;
			}
			return nullptr; 
		}
		//!���̃X�e�[�W�̐ݒ�
		void SetNextStage(const shared_ptr<StageBase>& NextStage){
			m_NextStageBase = NextStage;
		}
		//! �^�C�}�[�̎擾(�ݒ�A�N�Z�T�͂Ȃ�)
		const StepTimer& GetStepTimer() const { return m_Timer; }
		//! �O��̃^�[������̌o�ߎ��Ԃ𓾂�
		float GetElapsedTime(){ 
			double d = m_Timer.GetElapsedSeconds();
			//0���Z����̂���1000����1�����͕Ԃ�Ȃ��悤�ɂ���
			if (d < 0.001){
				d = 0.001;
			}
			return (float)d; 
		}
		//����
		//! �X�V
		void UpdateApp();
		//! �`��
		bool DrawApp();
		//! �t�����g�o�b�t�@�ɓ]������
		virtual void Present(UINT SyncInterval, UINT Flags);
		//--------------------------------------------------------------------------------------
		//	���J�ϐ�
		//--------------------------------------------------------------------------------------
		wstring		m_wstrModulePath;		//!< ���W���[�����t���p�X
		wstring		m_wstrDir;				//!< ���W���[��������f�B���N�g��
		wstring		m_wstrDataPath;			//!< ��΃p�X�̃��f�B�A�f�B���N�g��
		wstring		m_wstrShadersPath;		//!< ��΃p�X�̃V�F�[�_�f�B���N�g��
		wstring		m_wstrRelativeDataPath;	//!< ���΃p�X�̃��f�B�A�f�B���N�g��
		wstring		m_wstrRelativeShadersPath;	//!< ���΃p�X�̃V�F�[�_�f�B���N�g��
		map<wstring, wstring> m_ConfigMap;		//!< �ėp�}�b�v�i�e�A�v���P�V�����Ŏg�p�ł���j
	private:
		//�R�s�[�֎~
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		//���[�u�֎~
		App(const App&&) = delete;
		App& operator=(const App&&) = delete;

	};

}

//end basedx11
