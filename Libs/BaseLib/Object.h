#pragma once
#include "stdafx.h"

namespace basedx11{

	struct Event;
	class App;

	//--------------------------------------------------------------------------------------
	//	class ShapeInterface;
	//	用途: UpdateとDrawのインターフェイス
	//--------------------------------------------------------------------------------------
	class ShapeInterface{
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	ShapeInterface();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~ShapeInterface();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface(){}
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
		//	virtual void Update2();
		/*!
		@breaf Update2処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update2() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Update3();
		/*!
		@breaf Update3処理.
		この関数のみデフォルト定義しておく
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update3(){}
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
	protected:
		//構築と破棄
		Object(){}
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
			return Ptr;
		}

		//初期化を行う（仮想関数）
		//＊thisポインタが必要なオブジェクトはこの関数を多重定義して、構築する
		virtual void PreCreate(){}
		virtual void Create(){}


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
		Event(float DispatchTime,const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
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

	class Stage;
	class SceneBase;

	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	用途: イベント配送クラス
	//--------------------------------------------------------------------------------------
	class EventDispatcher{
	public:
		//構築と破棄
		explicit EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr);
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

}
//end basedx11
