#pragma once
#include "stdafx.h"

namespace basedx11{

	class App;
	struct Event;

	//--------------------------------------------------------------------------------------
	//	class SimpleInterface;
	//	用途: SimpleなUpdateとDrawのインターフェイス
	//--------------------------------------------------------------------------------------
	class SimpleInterface{
	protected:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	SimpleInterface();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		SimpleInterface(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~SimpleInterface();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimpleInterface(){}
	public:
		//操作
		//--------------------------------------------------------------------------------------
		//	virtual void Update() = 0;
		/*!
		@breaf Update処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Draw() = 0;
		/*!
		@breaf Draw処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw() = 0;
	};



	//--------------------------------------------------------------------------------------
	//	class Object;
	//	用途: すべてのオブジェクトの基底クラス（抽象クラス）
	//--------------------------------------------------------------------------------------
	class Object : public std::enable_shared_from_this<Object>{
		//クリエイト済みかどうか
		//Create関数が呼び出し後にtrueになる
		bool m_Created;
		void SetCreated(bool b){
			m_Created = b;
		}
	protected:
		//構築と破棄
		Object():m_Created(false){}
		virtual ~Object(){}
	public:
		//thisポインタ取得
		template<typename T>
		std::shared_ptr<T> GetThis(){
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr){
				return Ptr;
			}
			else{
				throw BaseException(
					L"thisをT型にキャストできません",
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"Object::GetThis()"
					);
			}
		}
		//オブジェクト作成（static関数）
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateObject(Ts&&... params){
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//仮想関数呼び出し
			Ptr->PreCreate();
			Ptr->Create();
			Ptr->SetCreated(true);
			return Ptr;
		}

		//初期化を行う（仮想関数）
		//＊thisポインタが必要なオブジェクトはこの関数を多重定義して、構築する
		virtual void PreCreate(){}
		virtual void Create(){}
		//クリエイト済みかどうか
		bool IsCreated(){
			return m_Created;
		}
		// イベントのPOST（キューに入れる）
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//指定のダイナミッククラスに送る
		template<typename T>
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					PostEvent(DispatchTime, Sender, TgtPtr, MsgStr, Info);
				}
			}
		}
		// イベントのSEND（キューに入れずにそのまま送る）
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//指定のダイナミッククラスに送る
		template<typename T>
		void SendEvent(const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					SendEvent(Sender, TgtPtr, MsgStr, Info);
				}
			}
		}

		//イベントを受け取る（仮想関数）
		//＊オブジェクトはイベントを受け取る場合にこの関数を多重定義する
		virtual void OnEvent(const shared_ptr<Event>& event){}
	private:
		//コピー禁止
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		//ムーブ禁止
		Object(const Object&&) = delete;
		Object& operator=(const Object&&) = delete;

	};

	//--------------------------------------------------------------------------------------
	//	struct Event;
	//	用途: イベント構造体
	//--------------------------------------------------------------------------------------
	struct Event{
		//遅延時間（SendEventの場合は常に0）
		float m_DispatchTime;
		//このメッセージを送ったオブジェクト
		weak_ptr<Object> m_Sender;
		//受け取るオブジェクト（nullptrの場合はアクティブステージ内すべてもしくはキーワードで識別するオブジェクト）
		weak_ptr<Object> m_Receiver;
		//受け取るオブジェクトの参照名もしくは配列名
		wstring m_ReceiverKey;
		//メッセージ文字列
		wstring m_MsgStr;
		//追加情報をもつオブジェクトのポインタ
		shared_ptr<void> m_Info;
		//構築と破棄
		Event(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(Receiver),
			m_ReceiverKey(L""),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		Event(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(),
			m_ReceiverKey(ReceiverKey),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		~Event(){}
	};

	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	用途: イベント配送クラス
	//--------------------------------------------------------------------------------------
	class EventDispatcher{
	public:
		//構築と破棄
		explicit EventDispatcher();
		virtual ~EventDispatcher();

		//イベントのPOST（キューに入れる）
		void PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//イベントのSEND（キューに入れずにそのまま送る）
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		//	void DispatchDelayedEvwnt();
		//	用途: POSTイベントの送信(メインループで呼ばれる)
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void DispatchDelayedEvwnt();

		//--------------------------------------------------------------------------------------
		//	void ClearEventQ();
		//	用途:キューにたまっているメッセージを削除する
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void ClearEventQ();

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
		//ムーブ禁止
		EventDispatcher(const EventDispatcher&&) = delete;
		EventDispatcher& operator=(const EventDispatcher&&) = delete;
	};



	//--------------------------------------------------------------------------------------
	//	class StageBase : public Object, public SimpleInterface;
	//	用途: ステージの親クラス
	//--------------------------------------------------------------------------------------
	class StageBase : public Object, public SimpleInterface{
	protected:
		StageBase() :Object(), SimpleInterface(){}
		virtual ~StageBase(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public SimpleInterface ;
	//	用途: カメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Camera : public Object, public SimpleInterface{
	public:
		//構築と破棄
		explicit Camera();
		virtual ~Camera();
		//アクセサ
		const Vector3& GetEye() const;
		void SetEye(const Vector3& Eye);
		void SetEye(float x, float y, float z);

		const Vector3& GetAt() const;
		void SetAt(const Vector3& At);
		void SetAt(float x, float y, float z);

		const Vector3& GetUp() const;
		void SetUp(const Vector3& Up);
		void SetUp(float x, float y, float z);

		bool IsPers()const;
		bool GetPers()const;
		void SetPers(bool b);

		float GetFovY() const;
		void SetFovY(float f);

		float GetAspect() const;
		void SetAspect(float f);

		float GetWidth() const;
		void SetWidth(float f);

		float GetHeight() const;
		void SetHeight(float f);

		float GetNear() const;
		void SetNear(float f);

		float GetFar() const;
		void SetFar(float f);

		void SetViewPort(const D3D11_VIEWPORT& v);

		const Matrix4X4& GetViewMatrix() const;
		const Matrix4X4& GetProjMatrix() const;
		//操作
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public SimpleInterface ;
	//	用途: ライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Light : public Object, public SimpleInterface{
	public:
		explicit Light();
		Light(
			const Vector3& Directional,
			const Color4& DiffuseColor,
			const Color4& SpecularColor
			);

		virtual ~Light();
		//アクセサ
		const Vector3& GetDirectional() const;
		void SetDirectional(const Vector3& Directional);
		void SetDirectional(float x, float y, float z);
		void SetPositionToDirectional(const Vector3& Position);
		void SetPositionToDirectional(float x, float y, float z);

		const Color4& GetDiffuseColor()const;
		void SetDiffuseColor(const Color4& col);
		void SetDiffuseColor(float r, float g, float b, float a);

		const Color4& GetSpecularColor() const;
		void SetSpecularColor(const Color4& col);
		void SetSpecularColor(float r, float g, float b, float a);

		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public SimpleInterface ;
	//	用途: マルチライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class MultiLight : public Object, public SimpleInterface{
	public:
		//構築と破棄
		explicit MultiLight();
		virtual ~MultiLight();
		//アクセサ
		//Lightの数
		size_t GetLightCount() const;
		//Lightの取得
		shared_ptr<Light> GetLight(size_t Index)const;
		//Lightのセット
		void SetLight(size_t Index, shared_ptr<Light>& Ptr);
		//配列の参照
		vector< shared_ptr<Light> >& GetLightVec();
		//Lightの追加
		shared_ptr<Light> AddLight();
		//Lightの削除
		void RemoveLight(size_t Index);
		//デフォルトのライティングの設定
		void SetDefaultLighting();

		//操作
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public SimpleInterface;
	//	用途: ビュークラス（ビューポートとライト、カメラを管理、コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class View : public Object, public SimpleInterface{
		//テンプレートから呼ばれるサブ関数
		void ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr);
	public:
		//構築と破棄
		View();
		virtual ~View();
		//アクセサ
		const D3D11_VIEWPORT& GetViewPort()const;
		const D3D11_VIEWPORT* GetViewPortPtr()const;
		void SetViewPort(const D3D11_VIEWPORT& v);
		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& c);
		shared_ptr<Camera> GetCamera() const;
		void SetCamera(shared_ptr<Camera>& Ptr);

		shared_ptr<MultiLight> GetMultiLight() const;
		void SetMultiLight(shared_ptr<MultiLight>& Lights);

		shared_ptr<MultiLight> SetDefaultLighting();


		//操作
		shared_ptr<Light> AddLight();

		template<typename CameraType, typename LightType>
		void ResetParamaters(const Rect2D<float>& ViewPortSize, Color4& Col, size_t LightCount, float MinDepth = 0.0f, float MaxDepth = 1.0f){
			D3D11_VIEWPORT vp;
			ZeroMemory(&vp, sizeof(vp));
			vp.MinDepth = MinDepth;
			vp.MaxDepth = MaxDepth;
			vp.TopLeftX = ViewPortSize.left;
			vp.TopLeftY = ViewPortSize.top;
			vp.Width = ViewPortSize.Width();
			vp.Height = ViewPortSize.Height();
			//カメラを差し替える
			auto CameraPtr = Object::CreateObject<CameraType>();
			//ライトを差し替える
			auto MultiLightPtr = Object::CreateObject<LightType>();
			for (size_t i = 0; i < LightCount; i++){
				MultiLightPtr->AddLight();
			}
			//サブ関数を使って設定
			ResetParamatersSub(vp, Col,
				dynamic_pointer_cast<Camera>(CameraPtr),
				dynamic_pointer_cast<MultiLight>(MultiLightPtr));
		}
		//操作
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Object, public SimpleInterface;
	//	用途: StringSprite
	//	文字列表示オブジェクト
	//--------------------------------------------------------------------------------------
	class StringSprite : public Object, public SimpleInterface{
	public:
		explicit StringSprite();
		virtual ~StringSprite();
		//アクセサ
		enum TextAlignment{
			m_Left,		//左合わせ
			m_Center,	//中央合わせ
			m_Right		//右合わせ
		};
		//アクセサ
		bool GetKeyInput() const;
		bool IsKeyInput() const;
		void SetKeyInput(bool b);


		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);
		void InsertText(const wstring& str);
		void InsertText(const wstring& str, size_t CaretIndex);


		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);

		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		const Color4& GetCaretColor() const;
		void SetCaretColor(const Color4& Col);

		float GetCaretSpeed() const;
		void SetCaretSpeed(float f);



		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//操作
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public Object;
	/*!
	オーディオリソース（wav）クラス
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public Object{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ<br />
		ウェブファイル名を渡す
		@param const wstring& FileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf サウンドデータの取得
		@param なし
		@return	このウェブリソースのサウンドデータ
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf ウェブフォーマットの取得
		@param なし
		@return	このウェブリソースのWAVEFORMATEXウェブフォーマットのポインタ
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	用途: マルチオーディオ
	//--------------------------------------------------------------------------------------
	class MultiAudioObject : public Object{
	public:
		//構築と破棄
		MultiAudioObject();
		virtual ~MultiAudioObject();
		//アクセサ
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey, bool ExceptionActive = true) const;
		//操作
		shared_ptr<AudioResource> AddAudioResource(const wstring& FileName,const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}
//end basedx11
