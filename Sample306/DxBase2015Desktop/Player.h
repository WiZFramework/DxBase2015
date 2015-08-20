#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class Player :  public SS5ssae;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public SS5ssae{
		shared_ptr< StateMachine<Player> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		//�ړ��̕����𓾂�
		float GetMoveX();
		//�ō����x
		float m_MaxSpeed;
		//������
		float m_Decel;
		//����
		float m_Mass;
		//Z���[���̈ʒu�̔z��
		vector<float> m_ZRail;
		size_t m_ZRailIndex;
		//��O�̃��[��
		size_t m_ZRailBeforIndex;
		//�ۉe���쐬���邽�߂̃��b�V��
		shared_ptr<CommonMeshResource> m_ShadowResource;
		//�f�[�^�ƃQ�[���Ƃ̕ϊ��s��
		Matrix4X4 m_ToAnimeMatrixLeft;
		Matrix4X4 m_ToAnimeMatrixRight;
	public:
		//�\�z�Ɣj��
		Player(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir);
		virtual ~Player(){}
		//������
		virtual void Create() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<Player> > GetStateMachine() const{
			return m_StateMachine;
		}
		//���[�V��������������֐��Q
		//�ړ����Č������ړ������ɂ���
		//�ړ�������Ԃ�
		float MoveRotationMotion();

		void AnimeChangeMotion(const wstring& key, bool looped);
		void LoopedAnimeUpdateMotion();

		//A�{�^���ŃW�����v����ǂ����𓾂�
		bool IsJumpMotion();
		//�W�����v�X�^�[�g����
		void JumpStartMotion();
		//�W�����v���Ă���Ԃ̏���
		//�W�����v�I��������true��Ԃ�
		bool JumpMotion();

		//B�{�^���ŃA�^�b�N����ǂ����𓾂�
		bool IsAttackMotion();
		//B�{�^���ŃA�^�b�N���鏈��
		bool AttackMotion();

		//���X�e�B�b�N��Z���[����ύX���邩�ǂ���
		bool IsRailChangeMotion();
		//Z���[���ύX���J�n����
		void RailChangeStartMotion();
		//Z���[���ύX���X�V����
		bool RailChangeMotion();
		//Z���[���������I�ɖ߂�
		void RailChangeBeforStartMotion();
		//Z���[���ύX���I������
		void RailChangeEndMotion();

		//�X�V
		virtual void Update() override;
		virtual void Update2() override;
	};

	//--------------------------------------------------------------------------------------
	//	class WaitState : public ObjState<Player>;
	//	�p�r: �ҋ@���
	//--------------------------------------------------------------------------------------
	class WaitState : public ObjState<Player>
	{
		WaitState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<WaitState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	�p�r: �����ړ�
	//--------------------------------------------------------------------------------------
	class WalkState : public ObjState<Player>
	{
		WalkState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<WalkState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class RailChangeState : public ObjState<Player>;
	//	�p�r: ���[���ύX���
	//--------------------------------------------------------------------------------------
	class RailChangeState : public ObjState<Player>
	{
		RailChangeState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<RailChangeState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	class JumpState : public ObjState<Player>
	{
		JumpState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<JumpState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class RunState : public ObjState<Player>;
	//	�p�r: ����A�j���[�V����
	//--------------------------------------------------------------------------------------
	class RunState : public ObjState<Player>
	{
		RunState(){}
	public:
		static shared_ptr<RunState> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class AttackState : public ObjState<Player>;
	//	�p�r: �A�^�b�N�A�j���[�V����
	//--------------------------------------------------------------------------------------
	class AttackState : public ObjState<Player>
	{
		AttackState(){}
	public:
		static shared_ptr<AttackState> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//endof  basedx11
