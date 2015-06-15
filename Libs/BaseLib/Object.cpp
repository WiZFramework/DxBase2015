#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Object;
	//	用途: すべてのオブジェクトの基底クラス（抽象クラス）
	//--------------------------------------------------------------------------------------
	void Object::PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void Object::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}



	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	用途: Impl構造体
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl{
		weak_ptr<SceneBase> m_SceneBase;
		//イベントのキュー
		list< shared_ptr<Event> > m_PriorityQ;
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//イベント
		//	);
		//用途: イベントの送信
		//戻り値: なし
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl(const shared_ptr<SceneBase>& SceneBasePtr) :m_SceneBase(SceneBasePtr){}
		~Impl(){}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event){
		if (m_SceneBase.expired()){
			//シーンが無効
			m_PriorityQ.clear();
			return;
		}
		auto SceneBasePtr = m_SceneBase.lock();
		if (event->m_ReceiverKey != L""){
			//キーがあった処理
			if (auto Vec = SceneBasePtr->GetActiveStage()->GetSharedObjectGroup(event->m_ReceiverKey, false)){
				//配列があった
				for (size_t i = 0; i < Vec->size(); i++){
					Vec->at(i)->OnEvent(event);
				}
			}
			//続いてシェアオブジェクトも同じキーがあれば送る
			if (auto Ptr = SceneBasePtr->GetActiveStage()->GetSharedObject(event->m_ReceiverKey, false)){
				Ptr->OnEvent(event);
			}
		}
		else if (!event->m_Receiver.expired()){
			//受け手が有効
			auto RePtr = event->m_Receiver.lock();
			RePtr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	用途: イベント配送クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	EventDispatcher::EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr):
		pImpl(new Impl(SceneBasePtr))
	{}
	EventDispatcher::~EventDispatcher(){}


	//イベントのPOST（キューに入れる）
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//イベントの作成 
		auto Ptr = make_shared<Event>(Delay,Sender, Receiver, MsgStr, Info);
		//キューにためる
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	//イベントのSEND（キューに入れずにそのまま送る）
	void EventDispatcher::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//イベントの作成 
		auto Ptr = make_shared<Event>(0.0f,Sender, Receiver, MsgStr, Info);
		//送信
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::DispatchDelayedEvwnt(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()){
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f){
				(*it)->m_DispatchTime = 0.0f;
				//メッセージの送信
				pImpl->Discharge(*it);
				//キューから削除
				it = pImpl->m_PriorityQ.erase(it);
				//削除後のイテレータが「最後」の
				//ときはループを抜ける
				if (it == pImpl->m_PriorityQ.end()){
					break;
				}
			}
			else{
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ(){
		pImpl->m_PriorityQ.clear();
	}






}
//endof  basedx11

