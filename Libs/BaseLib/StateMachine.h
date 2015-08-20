#pragma once

#include "stdafx.h"

namespace basedx11{


	template <typename T>
	class StateMachine;

	//--------------------------------------------------------------------------------------
	//	class ObjState;
	//	�p�r: �X�e�[�g�����e���v���[�g�N���X(���ۃN���X)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class ObjState{
	public:
		//--------------------------------------------------------------------------------------
		//	ObjState();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		ObjState(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~ObjState();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~ObjState(){}
		//--------------------------------------------------------------------------------------
		//	virtual void Enter(
		//	const shared_ptr<T>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g�ɓ������Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Execute(
		//	const shared_ptr<T>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: Update�̂Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Exit(
		//	const shared_ptr<T>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g���o��Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<T>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	//	template <typename T>
	//	class StateMachine;
	//	�p�r: �X�e�[�g�}�V�������e���v���[�g�N���X(���ۃN���X)
	//	���X�e�[�g���Ǘ�����
	//--------------------------------------------------------------------------------------
	template <typename T>
	class StateMachine
	{
	private:
		//���̃X�e�[�g�}�V�������I�[�i�[
		weak_ptr<T> m_Owner;
		//���݂̃X�e�[�g
		weak_ptr< ObjState<T> > m_CurrentState;
		//��O�̃X�e�[�g
		weak_ptr< ObjState<T> > m_PreviousState;
	public:
		//--------------------------------------------------------------------------------------
		//	StateMachine(
		//	const shared_ptr<T>& owner	//���̃X�e�[�g�}�V����ێ�����I�[�i�[
		//	);
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		explicit StateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
		{}
		//--------------------------------------------------------------------------------------
		//	virtual ~StateMachine();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~StateMachine(){}
		//--------------------------------------------------------------------------------------
		//	�p�r: Set�A�N�Z�b�T
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void SetCurrentState(const shared_ptr< ObjState<T> >& s){ m_CurrentState = s; }
		void SetPreviousState(const shared_ptr< ObjState<T> >& s){ m_PreviousState = s; }
		//--------------------------------------------------------------------------------------
		//	�p�r: Get�A�N�Z�b�T
		//	�߂�l: ObjState<T>*
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> >  GetCurrentState() const {
			if (!m_CurrentState.expired()){
				return m_CurrentState.lock();
			}
			return nullptr; 
		}
		shared_ptr< ObjState<T> >  GetPreviousState()const {
			if (!m_pPreviousState.expired()){
				return m_pPreviousState.lock();
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		//	void  Update() const
		//	�p�r: �X�e�[�g���X�V����
		//	�߂�l: �Ȃ�
		//	�������o�͕ύX���Ȃ��̂�const�֐��Ŗ��Ȃ�
		//--------------------------------------------------------------------------------------
		void Update() const{
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				auto Ptr = m_CurrentState.lock();
				Ptr->Execute(m_Owner.lock());
			}
		}
		//--------------------------------------------------------------------------------------
		//	void  ChangeState(
		//	const shared_ptr< ObjState<T> >& NewState	//�V�����X�e�[�g
		//�@);
		// �p�r: �X�e�[�g��ύX����
		// �߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void  ChangeState(const shared_ptr< ObjState<T> >& NewState){
			//���̃X�e�[�g��ۑ�
			m_PreviousState = m_CurrentState;
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				//���̃X�e�[�g�ɏI����m�点��
				auto Ptr = m_CurrentState.lock();
				Ptr->Exit(m_Owner.lock());
			}
			//�V�����X�e�[�g���J�����g�ɐݒ�
			m_CurrentState = NewState;
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				//���̃X�e�[�g�ɏI����m�点��
				auto Ptr = m_CurrentState.lock();
				Ptr->Enter(m_Owner.lock());
			}
		}
		//--------------------------------------------------------------------------------------
		//	void RevertToPreviousState();
		// �p�r: �X�e�[�g����O�̃X�e�[�g�ɖ߂�
		// �߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void RevertToPreviousState(){
			ChangeState(m_pPreviousState);
		}
		//--------------------------------------------------------------------------------------
		//bool IsInState(
		//const ObjState<T>& st	//���ׂ�X�e�[�g
		//)const;
		//�p�r: �J�����g�X�e�[�g���w�肵���X�e�[�g�ɂȂ��Ă邩�ǂ������`�F�b�N
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		bool IsInState(const shared_ptr< ObjState<T> >& st)const{
			if (m_CurrentState.expired()){
				return false;
			}
			return typeid(m_CurrentState.lock()) == typeid(st);
		}
	};

	//--------------------------------------------------------------------------------------
	//	class ObjState;
	//	�p�r: �X�e�[�g�����e���v���[�g�N���X(���ۃN���X)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class EventState{
	public:
		//--------------------------------------------------------------------------------------
		//	EventState();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		EventState(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~EventState();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~EventState(){}
		//--------------------------------------------------------------------------------------
		//	virtual void Enter(
		//	const shared_ptr<T>& Obj,		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	const shared_ptr<Event>& event				//����ꂽ�C�x���g�i�ǉ����͂��̒��ɓ����Ă���j
		//	) = 0;
		//	�p�r: �X�e�[�g�ɓ������Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<T>& Obj, const shared_ptr<Event>& event) = 0;
	};



	//--------------------------------------------------------------------------------------
	//	template <typename T>
	//	class EventMachine;
	//	�p�r: �C�x���g�}�V�������e���v���[�g�N���X(���ۃN���X)
	//	���C�x���g�X�e�[�g���Ǘ�����
	//--------------------------------------------------------------------------------------
	template <typename T>
	class EventMachine
	{
	private:
		//���̃X�e�[�g�}�V�������I�[�i�[
		weak_ptr<T> m_Owner;
		//�C�x���g������ƃC�x���g�X�e�[�g�̑g�ݍ��킹��}�b�v
		map<wstring, weak_ptr< EventState<T> > >  m_EventStateMap;
	public:
		//--------------------------------------------------------------------------------------
		//	EventMachine(
		//	const shared_ptr<T>& owner	//���̃C�x���g�}�V����ێ�����I�[�i�[
		//	);
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		explicit EventMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
		{}
		//--------------------------------------------------------------------------------------
		//	virtual ~EventMachine();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~EventMachine(){}
		//--------------------------------------------------------------------------------------
		//	�p�r: Add�A�N�Z�b�T
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void AddEventState(const wstring& key,const shared_ptr< EventState<T> >& s){
			auto it = m_EventStateMap.find(key);
			if (it != m_EventStateMap.end()){
				//���łɂ���
				throw BaseException(
					L"���łɂ��̃C�x���g�͓o�^����Ă��܂��B",
					key,
					L"EventMachine::AddEventState()"
					);
			}
			m_EventStateMap[key] = s;
		}
		//--------------------------------------------------------------------------------------
		//	void HandleEvent(
		//	const shared_ptr<Event>& event	//�C�x���g
		//	);
		//	�p�r: �C�x���g���C�x���g�X�e�[�g�ɐU�蕪����
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void HandleEvent(const shared_ptr<Event>& event){
			auto it = m_EventStateMap.find(event->m_MsgStr);
			if (it != m_EventStateMap.end()){
				auto Ptr = it->second;
				if (!Ptr.expired() && !m_Owner.expired()){
					auto Ptr2 = Ptr.lock();
					Ptr2->Enter(m_Owner.lock(), event);
				}
			}
		}
	};





}
//end basedx11

