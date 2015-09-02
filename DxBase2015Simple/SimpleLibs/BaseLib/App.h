/*! 
@file App.h
@brief アプリケーションクラス。入力機器等

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
	XBoxコントローラーのステータス.
	XINPUT_STATEからのエイリアス.
	*/
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		//!< ボタン状態
		BYTE bLeftTrigger;	//!< 左トリガ
		BYTE bRightTrigger;	//!< 右トリガ

		float fThumbRX;	//!< 右スティックXのflort変換
		float fThumbRY;	//!< 右スティックYのflort変換
		float fThumbLX;	//!< 左スティックXのflort変換
		float fThumbLY;	//!< 左スティックYのflort変換
		WORD wNowUpdateButtons;	//!< 状態が変わったときにフラグが立つボタン
		WORD wPressedButtons;	//!< 押された瞬間を所持するボタン
		WORD wReleasedButtons;	//!< 離された瞬間を所持するボタン
		WORD wLastButtons;	//!< 一つ前のボタン
		bool bConnected;	//!< 接続しているかどうか
	};
	//--------------------------------------------------------------------------------------
	//	struct KEYBOARD_STATE;
	/*!
	キーボードのステータス.
	マウスのステータスも取得する
	*/
	//--------------------------------------------------------------------------------------
	struct KEYBOARD_STATE{
		static const DWORD MAX_KEYVCODE = 256;
		bool m_bPushKeyTbl[MAX_KEYVCODE];	//!< 現在のキーボードの状態
		bool m_bLastKeyTbl[MAX_KEYVCODE];	//!< 一つ前のキーボードの状態
		bool m_bPressedKeyTbl[MAX_KEYVCODE];	//!< 押された瞬間のキーボード
		bool m_bUpKeyTbl[MAX_KEYVCODE];		//!< 離された瞬間のキーボード
		bool m_KeyMessageActive;	//!<何かのキーイベントが発生
		Point2D<int> m_MouseClientPoint;
		//--------------------------------------------------------------------------------------
		//	KEYBOARD_STATE();
		/*!
		コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		KEYBOARD_STATE() :
			m_KeyMessageActive{ false },
			m_MouseClientPoint{ 0, 0 }
		{
			//キーボードテーブルの初期化
			::ZeroMemory(&m_bLastKeyTbl, sizeof(m_bLastKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
		}
		//--------------------------------------------------------------------------------------
		//	bool IsMouseEnabled(
		//			HWND hWnd,	//ウインドウのハンドル。
		//			vector<DWORD>& UseKeyVec	//使用するキーコード配列
		//	);
		/*!
		@breaf	マウス使用でマウスがクライアント領域にあるかどうかのチェック
		@param	HWND hWnd,	ウインドウのハンドル。
		@param	vector<DWORD>& UseKeyVec: 使用するキーコード配列
		@return マウス使用でマウスがクライアント領域にあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsMouseEnabled(HWND hWnd, vector<DWORD>& UseKeyVec){
			//マウスポイントは0,0に初期化
			m_MouseClientPoint = { 0, 0 };
			//マウス利用にかかわらず、マウスポインタ座標は設定
			POINT p;
			::GetCursorPos(&p);
			if (::ScreenToClient(hWnd, &p)){
				//クライアント座標に変換できたときのみ、マウス座標を設定
				m_MouseClientPoint = { p.x, p.y };
			}
			else{
				return false;
			}

			//UseKeyVecにマウスがあった場合
			vector<DWORD> MouseTemp = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
			bool MouseChk = false;
			for (auto chk : MouseTemp){
				auto it = find(UseKeyVec.begin(), UseKeyVec.end(), chk);
				if (it != UseKeyVec.end()){
					MouseChk = true;
					break;
				}
			}
			//マウス使用で座標がクライアント領域内ならtrue
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
		//			HWND hWnd,	//ウインドウのハンドル。Escキー入力時にメッセージを送るため
		//			vector<DWORD>& UseKeyVec	//使用するキーコード配列
		//	);
		/*!
		@breaf	キーステートを得る
		@param	HWND hWnd,	ウインドウのハンドル。Escキー入力時にメッセージを送るため
		@param	vector<DWORD>& UseKeyVec: 使用するキーコード配列
		@return キー入力があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetKeyState(HWND hWnd, vector<DWORD>& UseKeyVec){
			m_KeyMessageActive = false;
			//一つ前にコピー
			CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
			//キーボードの状態を初期化
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
			//マウスのチェック
			bool MouseEnabled = IsMouseEnabled(hWnd, UseKeyVec);
			//自分自身にフォーカスがない場合はfalse
			if (::GetFocus() != hWnd){
				return false;
			}
			SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
			if (ret & 0x8000){
				//Escキーは無条件にfalse
				return false;
			}
			size_t sz = UseKeyVec.size();
			for (size_t i = 0; i < sz; i++){
				ret = GetAsyncKeyState((int)UseKeyVec[i]);
				if (UseKeyVec[i] == VK_LBUTTON || UseKeyVec[i] == VK_RBUTTON || UseKeyVec[i] == VK_MBUTTON){
					if (!MouseEnabled){
						//マウスが無効ならUsedに入っていても無視
						continue;
					}
				}
				if (ret & 0x8000){
					m_bPushKeyTbl[UseKeyVec[i]] = true;
					m_KeyMessageActive = true;	//メッセージをゲームが受け取る
					if (!m_bLastKeyTbl[UseKeyVec[i]]){
						//前回押されてなくて今回押された
						m_bPressedKeyTbl[UseKeyVec[i]] = true;
					}
				}
				else{
					//キーは押されてない
					if (m_bLastKeyTbl[UseKeyVec[i]]){
						//前回押されていて今回押されてない
						m_bUpKeyTbl[UseKeyVec[i]] = true;
						m_KeyMessageActive = true;	//メッセージをゲームが受け取る
					}
				}
			}
			return m_KeyMessageActive;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class InputDevice;
	/*!
	入力機器
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
		@breaf コンストラクタ
		@param なし
		@return	なし
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
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		~InputDevice(){}
		//--------------------------------------------------------------------------------------
		//	bool ResetInputState(
		//		HWND hWnd,	//ウインドウのハンドル。Escキー入力時にメッセージを送るため
		//		vector<DWORD>& UseKeyVec	//使用するキーの配列
		//	);
		/*!
		@breaf マウスとキーボードの状態を得る
		@param HWND hWnd,	ウインドウのハンドル。Escキー入力時にメッセージを送るため
		@param vector<DWORD>& UseKeyVec	使用するキー
		@return	マウスかキーボードメッセージがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec){
			bool Ret1 = m_KeyState.GetKeyState(hWnd, UseKeyVec);
			return Ret1;
		}
		//--------------------------------------------------------------------------------------
		//	void ResetControlerState();
		/*!
		@breaf コントローラーの状態をm_Stateに取得する
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState(){
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//一つ前の状態を保存
				m_State[i].wLastButtons = m_State[i].Gamepad.wButtons;
				// XInputから現在のステータスを得る
				XINPUT_STATE State;
				dwResult = XInputGetState(i, &State);
				if (dwResult == ERROR_SUCCESS){
					m_State[i].dwPacketNumber = State.dwPacketNumber;
					m_State[i].Gamepad = State.Gamepad;
					m_State[i].bConnected = true;
					//ボタンの設定
					//コピー
					m_State[i].wButtons = m_State[i].Gamepad.wButtons;
					//更新ボタン
					m_State[i].wNowUpdateButtons = m_State[i].Gamepad.wButtons;
					m_State[i].bLeftTrigger = m_State[i].Gamepad.bLeftTrigger;
					m_State[i].bRightTrigger = m_State[i].Gamepad.bRightTrigger;
					//前回より変化しているボタンのみフラグを立てる
					//XOR演算により実装
					m_State[i].wNowUpdateButtons ^= m_State[i].wLastButtons;
					//押された瞬間をセット
					m_State[i].wPressedButtons = m_State[i].wNowUpdateButtons & m_State[i].wButtons;
					//離された瞬間をセット
					m_State[i].wReleasedButtons = m_State[i].wNowUpdateButtons & (m_State[i].wButtons ^ 0xffff);
					//レバーのデッドゾーンの設定
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
		//	アクセサ
		//!	キーステートの取得
		const KEYBOARD_STATE& GetKeyState() const{ return m_KeyState; }
		//!	コントローラステートの取得
		const vector<CONTROLER_STATE>& GetControlerVec()const { return m_State; }
	};

	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	/*!
	オーディオマネージャクラス
	*/
	//--------------------------------------------------------------------------------------
	class AudioManager{
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	AudioManager();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AudioManager();
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioManager();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioManager();
		//--------------------------------------------------------------------------------------
		//	void CreateDeviceIndependentResources();
		/*!
		@breaf オーディオの初期化
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateDeviceIndependentResources();
		//--------------------------------------------------------------------------------------
		//	IXAudio2* GetMusicEngine()const;
		/*!
		@breaf ミュージックエンジンの取得
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetMusicEngine()const;
		//--------------------------------------------------------------------------------------
		//	IXAudio2* GetMusicEngine()const;
		/*!
		@breaf サウンドエフェクトエンジンの取得
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetSoundEffectEngine()const;
		//--------------------------------------------------------------------------------------
		//	SuspendAudio();
		/*!
		@breaf オーディオの中断
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SuspendAudio();
		//--------------------------------------------------------------------------------------
		//	ResumeAudio();
		/*!
		@breaf オーディオの再開
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResumeAudio();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		//ムーブ禁止
		AudioManager(const AudioManager&&) = delete;
		AudioManager& operator=(const AudioManager&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	class App;
	/*!
		アプリケーションクラス
	*/
	//--------------------------------------------------------------------------------------
	class App{
		// デリーター
		struct AppDeleter
		{
			void operator()(App *p) { delete p; }
		};
		static unique_ptr<App, AppDeleter> m_App;		//Singletonで利用する自分自身のポインタ
		HINSTANCE m_hInstance;							//モジュールのインスタンス
		HWND m_hWnd;									//メインウインドウのハンドル
		bool m_FullScreen;								//フルスクリーンかどうか
		UINT m_GameWidth;								//ゲーム盤幅(ピクセル)
		UINT m_GameHeight;								//ゲーム盤高さ(ピクセル)
		shared_ptr<DeviceResources> m_DeviceResources;	//デバイス
		shared_ptr<StageBase> m_StageBase;				//ゲームステージ
		shared_ptr<StageBase> m_NextStageBase;			//次のゲームステージ
		StepTimer m_Timer;								//タイマー
		//オーディオマネージャ
		unique_ptr<AudioManager> m_AudioManager;
		//イベント配送クラス
		shared_ptr<EventDispatcher> m_EventDispatcher;	
		//構築と破棄
		App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height);
		virtual ~App(){}
	public:
		//! シングルトン構築とアクセサ
		static unique_ptr<App, AppDeleter>& GetApp(HINSTANCE hInstance = 0, HWND hWnd = 0,
			bool FullScreen = false, UINT Width = 0, UINT Height = 0);
		//! 強制破棄
		static void DeleteApp();
		//アクセサ
		//! アプリケーションのインスタンス取得
		HINSTANCE GetHInstance() const { return m_hInstance; }
		//! ウインドウのハンドルの取得
		HWND GetHWnd() const { return m_hWnd; }
		//! オーディオマネージャの取得
		unique_ptr<AudioManager>& GetAudioManager();
		//!　イベント配送オブジェクトの取得
		shared_ptr<EventDispatcher> GetEventDispatcher();
		//! フルスクリーンかどうか
		bool IsFullScreen() const { return m_FullScreen; }
		bool GetFullScreen() const { return m_FullScreen; }
		//! ゲーム盤の幅の取得
		UINT GetGameWidth() const { return m_GameWidth; }
		//! ゲーム盤の高さの取得
		UINT GetGameHeight() const { return m_GameHeight; }
		//! デバイスリソースの取得
		shared_ptr<DeviceResources> GetDeviceResources() const { return m_DeviceResources; }
		//! デバイスリソースの設定
		void SetDeviceResources(const shared_ptr<DeviceResources>& ptr){ m_DeviceResources = ptr; }
		//!ゲームステージの取得
		shared_ptr<StageBase> GetStageBase() const { return m_StageBase; }
		//!ゲームステージの取得テンプレート
		template<typename T>
		shared_ptr<T> GetStage() const { 
			shared_ptr<T> Ret = dynamic_pointer_cast<T>(m_StageBase);
			if (Ret){
				return Ret;
			}
			return nullptr; 
		}
		//!次のステージの設定
		void SetNextStage(const shared_ptr<StageBase>& NextStage){
			m_NextStageBase = NextStage;
		}
		//! タイマーの取得(設定アクセサはない)
		const StepTimer& GetStepTimer() const { return m_Timer; }
		//! 前回のターンからの経過時間を得る
		float GetElapsedTime(){ 
			double d = m_Timer.GetElapsedSeconds();
			//0除算回避のため1000分の1未満は返らないようにする
			if (d < 0.001){
				d = 0.001;
			}
			return (float)d; 
		}
		//操作
		//! 更新
		void UpdateApp();
		//! 描画
		bool DrawApp();
		//! フロントバッファに転送する
		virtual void Present(UINT SyncInterval, UINT Flags);
		//--------------------------------------------------------------------------------------
		//	公開変数
		//--------------------------------------------------------------------------------------
		wstring		m_wstrModulePath;		//!< モジュール名フルパス
		wstring		m_wstrDir;				//!< モジュールがあるディレクトリ
		wstring		m_wstrDataPath;			//!< 絶対パスのメディアディレクトリ
		wstring		m_wstrShadersPath;		//!< 絶対パスのシェーダディレクトリ
		wstring		m_wstrRelativeDataPath;	//!< 相対パスのメディアディレクトリ
		wstring		m_wstrRelativeShadersPath;	//!< 相対パスのシェーダディレクトリ
		map<wstring, wstring> m_ConfigMap;		//!< 汎用マップ（各アプリケションで使用できる）
	private:
		//コピー禁止
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		//ムーブ禁止
		App(const App&&) = delete;
		App& operator=(const App&&) = delete;

	};

}

//end basedx11
