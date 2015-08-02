#pragma once

#include "stdafx.h"

namespace basedx11{


	template <typename T>
	class MyStateMachine;

	//--------------------------------------------------------------------------------------
	//	class MyObjState;
	//	�p�r: �X�e�[�g�����e���v���[�g�N���X(���ۃN���X)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class MyObjState{
	public:
		//--------------------------------------------------------------------------------------
		//	MyObjState();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		MyObjState(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~MyObjState();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~MyObjState(){}
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
	//	class MyStateMachine;
	//	�p�r: �X�e�[�g�}�V�������e���v���[�g�N���X(���ۃN���X)
	//	���X�e�[�g���Ǘ�����
	//--------------------------------------------------------------------------------------
	template <typename T>
	class MyStateMachine
	{
	private:
		//���̃X�e�[�g�}�V�������I�[�i�[
		weak_ptr<T> m_Owner;
		//���݂̃X�e�[�g
		weak_ptr< MyObjState<T> > m_CurrentState;
		//��O�̃X�e�[�g
		weak_ptr< MyObjState<T> > m_PreviousState;
	public:
		//--------------------------------------------------------------------------------------
		//	MyStateMachine(
		//	const shared_ptr<T>& owner	//���̃X�e�[�g�}�V����ێ�����I�[�i�[
		//	);
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		explicit MyStateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
		{}
		//--------------------------------------------------------------------------------------
		//	virtual ~MyStateMachine();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~MyStateMachine(){}
		//--------------------------------------------------------------------------------------
		//	�p�r: Set�A�N�Z�b�T
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void SetCurrentState(const shared_ptr< MyObjState<T> >& s){ m_CurrentState = s; }
		void SetPreviousState(const shared_ptr< MyObjState<T> >& s){ m_PreviousState = s; }
		//--------------------------------------------------------------------------------------
		//	�p�r: Get�A�N�Z�b�T
		//	�߂�l: MyObjState<T>*
		//--------------------------------------------------------------------------------------
		shared_ptr< MyObjState<T> >  GetCurrentState() const {
			if (!m_CurrentState.expired()){
				return m_CurrentState.lock();
			}
			return nullptr;
		}
		shared_ptr< MyObjState<T> >  GetPreviousState()const {
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
		//	const shared_ptr< MyObjState<T> >& NewState	//�V�����X�e�[�g
		//�@);
		// �p�r: �X�e�[�g��ύX����
		// �߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void  ChangeState(const shared_ptr< MyObjState<T> >& NewState){
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
		//const MyObjState<T>& st	//���ׂ�X�e�[�g
		//)const;
		//�p�r: �J�����g�X�e�[�g���w�肵���X�e�[�g�ɂȂ��Ă邩�ǂ������`�F�b�N
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		bool IsInState(const shared_ptr< MyObjState<T> >& st)const{
			if (m_CurrentState.expired()){
				return false;
			}
			return typeid(m_CurrentState.lock()) == typeid(st);
		}
	};



	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViews();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//������
		virtual void Create()override;
	};
}
//endof  basedx11
