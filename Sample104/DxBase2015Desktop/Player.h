#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		shared_ptr< StateMachine<Player> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		//�ړ��̌����𓾂�
		Vector3 GetAngle();
	public:
		//�\�z�Ɣj��
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player(){}
		//������
		virtual void Create() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<Player> > GetStateMachine() const{
			return m_StateMachine;
		}
		//�W�����v�J�n
		void StartJump();
		//�X�V
		virtual void Update() override;
		virtual void Update2() override;
	};

	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<Player>
	{
		//--------------------------------------------------------------------------------------
		//	DefaultState(){}
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//	���V���O���g���̂��߁AInstance()�֐�����ČĂяo��
		//--------------------------------------------------------------------------------------
		DefaultState(){}
	public:
		//--------------------------------------------------------------------------------------
		//	static DefaultState* Instance();
		//	�p�r: �C���X�^���X�̎擾
		//	�߂�l: DefaultState�̃C���X�^���X
		//--------------------------------------------------------------------------------------
		static shared_ptr<DefaultState> Instance();
		//--------------------------------------------------------------------------------------
		//	virtual void Enter(
		//	const shared_ptr<Player>& Obj	//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g�ɓ������Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<Player>& Obj)override{}
		//--------------------------------------------------------------------------------------
		//	virtual void Execute(
		//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: Update�̂Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//--------------------------------------------------------------------------------------
		//	virtual void Exit(
		//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g���o��Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<Player>& Obj)override{}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	class JumpState : public ObjState<Player>
	{
		//--------------------------------------------------------------------------------------
		//	JumpState(){}
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//	���V���O���g���̂��߁AInstance()�֐�����ČĂяo��
		//--------------------------------------------------------------------------------------
		JumpState(){}
	public:
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<JumpState> Instance();
		//	�p�r: �C���X�^���X�̎擾
		//	�߂�l: DefaultState�̃C���X�^���X
		//--------------------------------------------------------------------------------------
		static shared_ptr<JumpState> Instance();
		//--------------------------------------------------------------------------------------
		//	virtual void Enter(
		//	const shared_ptr<Player>& Obj	//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g�ɓ������Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//--------------------------------------------------------------------------------------
		//	virtual void Execute(
		//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: Update�̂Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//--------------------------------------------------------------------------------------
		//	virtual void Exit(
		//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
		//	) = 0;
		//	�p�r: �X�e�[�g���o��Ƃ��Ɏ��s�����
		//	�߂�l: �Ȃ��i�������z�֐��j
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<Player>& Obj)override{}
	};


}
//endof  basedx11
