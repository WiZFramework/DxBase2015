#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	�p�r: �C�e
	//--------------------------------------------------------------------------------------
	class ShellBall : public GameObject{
		Vector3 m_StartPos;
		Vector3 m_NowScale;
		Vector3 m_JumpVec;
		shared_ptr< StateMachine<ShellBall> >  m_StateMachine;	//�X�e�[�g�}�V�[��
	public:
		//�\�z�Ɣj��
		ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
			const Vector3& JumpVec);
		virtual ~ShellBall();
		//�A�N�Z�T
		shared_ptr< StateMachine<ShellBall> > GetStateMachine() const{
			return m_StateMachine;
		}
		//������
		virtual void Create() override;
		virtual void Update() override;
		//�ď�����
		void Refresh(const Vector3& StartPos, const Vector3& JumpVec);
		//���������o����֐�
		//�n�ʂɂ������ǂ���
		bool IsArrivedBaseMotion();
		//�����̊J�n
		void ExplodeStartMotion();
		//�����̉��o(���o�I���ōX�V�ƕ`��𖳌��ɂ���j
		void ExplodeExcuteMotion();

	};

	//--------------------------------------------------------------------------------------
	//	class FiringState : public ObjState<ShellBall>;
	//	�p�r: ���˂��甚���܂ł̃X�e�[�g
	//--------------------------------------------------------------------------------------
	class FiringState : public ObjState<ShellBall>
	{
		FiringState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<FiringState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<ShellBall>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<ShellBall>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<ShellBall>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class ExplodeState : public ObjState<ShellBall>;
	//	�p�r: �����Œ��̃X�e�[�g
	//--------------------------------------------------------------------------------------
	class ExplodeState : public ObjState<ShellBall>
	{
		ExplodeState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<ExplodeState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<ShellBall>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<ShellBall>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<ShellBall>& Obj)override;
	};



}
//endof  basedx11
