#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Object;
	//	�p�r: ���ׂẴI�u�W�F�N�g�̊��N���X�i���ۃN���X�j
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
	//	�p�r: Impl�\����
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl{
		weak_ptr<SceneBase> m_SceneBase;
		//�C�x���g�̃L���[
		list< shared_ptr<Event> > m_PriorityQ;
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//�C�x���g
		//	);
		//�p�r: �C�x���g�̑��M
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl(const shared_ptr<SceneBase>& SceneBasePtr) :m_SceneBase(SceneBasePtr){}
		~Impl(){}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event){
		if (m_SceneBase.expired()){
			//�V�[��������
			m_PriorityQ.clear();
			return;
		}
		auto SceneBasePtr = m_SceneBase.lock();
		if (event->m_ReceiverKey != L""){
			//�L�[������������
			if (auto Vec = SceneBasePtr->GetActiveStage()->GetSharedObjectGroup(event->m_ReceiverKey, false)){
				//�z�񂪂�����
				for (size_t i = 0; i < Vec->size(); i++){
					Vec->at(i)->OnEvent(event);
				}
			}
			//�����ăV�F�A�I�u�W�F�N�g�������L�[������Α���
			if (auto Ptr = SceneBasePtr->GetActiveStage()->GetSharedObject(event->m_ReceiverKey, false)){
				Ptr->OnEvent(event);
			}
		}
		else if (!event->m_Receiver.expired()){
			//�󂯎肪�L��
			auto RePtr = event->m_Receiver.lock();
			RePtr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	�p�r: �C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EventDispatcher::EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr):
		pImpl(new Impl(SceneBasePtr))
	{}
	EventDispatcher::~EventDispatcher(){}


	//�C�x���g��POST�i�L���[�ɓ����j
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(Delay,Sender, Receiver, MsgStr, Info);
		//�L���[�ɂ��߂�
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
	void EventDispatcher::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(0.0f,Sender, Receiver, MsgStr, Info);
		//���M
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::DispatchDelayedEvwnt(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()){
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f){
				(*it)->m_DispatchTime = 0.0f;
				//���b�Z�[�W�̑��M
				pImpl->Discharge(*it);
				//�L���[����폜
				it = pImpl->m_PriorityQ.erase(it);
				//�폜��̃C�e���[�^���u�Ō�v��
				//�Ƃ��̓��[�v�𔲂���
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

