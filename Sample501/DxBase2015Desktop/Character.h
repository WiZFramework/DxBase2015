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
		float m_InStartTime;	//���ˌ�̌o�ߎ���
		bool m_IsEnemyBall;		//�G���̖C�e���ǂ���
		shared_ptr< StateMachine<ShellBall> >  m_StateMachine;	//�X�e�[�g�}�V�[��
	public:
		//�\�z�Ɣj��
		ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
			const Vector3& JumpVec, bool IsEnemyBall);
		virtual ~ShellBall();
		//�A�N�Z�T
		shared_ptr< StateMachine<ShellBall> > GetStateMachine() const{
			return m_StateMachine;
		}
		//������
		virtual void Create() override;
		virtual void Update() override;
		//�ď�����
		void Refresh(const Vector3& StartPos, 
			const Vector3& JumpVec, bool IsEnemyBall);
		//���������o����֐�
		//���ˌ���̎��Ԃ���������Փ˂��A�N�e�B�u�ɂ���
		void HitTestCheckMotion();
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


	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//	�p�r: �G�L�����̐e�N���X
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject{
		shared_ptr< StateMachine<Enemy> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		//�C�e�̔���
		void StartShellBall();
	protected:
		Vector3 m_StartPos;
		//�\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Enemy();
		//�h���N���X�́A�K�����̉��z�֐�����������
		virtual void ExtMotionMethod() = 0;
		virtual void ShellHitMotionMethod() = 0;

	public:
		//�A�N�Z�T
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const{
			return m_StateMachine;
		}
		//������
		virtual void Create() override;
		virtual void Update() override;
		virtual void Update2() override;

		//���[�V��������������֐��Q

		//���̊֐��̓��\�b�h�Ăяo���֐��Ƃ��Ď�������
		void ExtMotion();
		//�C�e�ƏՓ˂����u�Ԃ̏���
		void ShellHitMotion();

		//�m���ɂ��C�e�𔭎˂��郂�[�V����
		void ShellThrowMotion();
		//�C�e�ƏՓ˂�����̏���
		//�����I��������true��Ԃ�
		bool ShellHitMoveMotion();
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyDefaultState : public ObjState<Enemy>;
	//	�p�r: �ʏ���
	//--------------------------------------------------------------------------------------
	class EnemyDefaultState : public ObjState<Enemy>
	{
		EnemyDefaultState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<EnemyDefaultState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyShellHitState : public ObjState<Enemy>;
	//	�p�r: �C�e�����������Ƃ��̏���
	//--------------------------------------------------------------------------------------
	class EnemyShellHitState : public ObjState<Enemy>
	{
		EnemyShellHitState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<EnemyShellHitState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};




	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public Enemy;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	class RollingTorus : public Enemy{
		float m_YRot;
		const float m_MaxRotationSpeed;
		float m_RotationSpeed;
	protected:
		//�h���N���X�́A�K�����̉��z�֐�����������
		virtual void ExtMotionMethod()override;
		virtual void ShellHitMotionMethod()override;
	public:
		//�\�z�Ɣj��
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//������
		virtual void Create() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ColoringOctahedron : public Enemy;
	//	�p�r: �F���ς��8�ʑ�
	//--------------------------------------------------------------------------------------
	class ColoringOctahedron : public Enemy{
	protected:
		//�h���N���X�́A�K�����̉��z�֐�����������
		virtual void ExtMotionMethod()override;
		virtual void ShellHitMotionMethod()override;
	public:
		//�\�z�Ɣj��
		ColoringOctahedron(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~ColoringOctahedron();
		//������
		virtual void Create() override;
	};






	//--------------------------------------------------------------------------------------
	//	class ScoreObject : public GameObject;
	//	�p�r: �X�R�A��\������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ScoreObject : public GameObject{
		//�C�x���g�}�V�[��
		shared_ptr< EventMachine<ScoreObject> >  m_EventMachine;
		//�|�C���g
		size_t m_Point;
		//�G��(�g�[���X)�̃|�C���g
		size_t m_EnemyPoint;

	public:
		//�\�z�Ɣj��
		ScoreObject(const shared_ptr<Stage>& StagePtr);
		virtual ~ScoreObject();
		//������
		virtual void Create() override;
		virtual void Update() override;
		//�C�x���g�̃n���h��
		virtual void OnEvent(const shared_ptr<Event>& event)override;
		//�C�x���g�����������Ƃ��̉��o�֐�
		//�|�C���g�̉��Z
		void AddPointMotion(size_t sz);
		//�G�|�C���g�̉��Z
		void AddEnemyPointMotion(size_t sz);
	};


	//--------------------------------------------------------------------------------------
	//	class TorusHitEvent : public EventState<ScoreObject>;
	//	�p�r: �g�[���X���C�e�ɓ��������C�x���g
	//--------------------------------------------------------------------------------------
	class TorusHitEvent : public EventState<ScoreObject>
	{
		TorusHitEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<TorusHitEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event)override;
	};

	//--------------------------------------------------------------------------------------
	//	class PlayerHitEvent : public EventState<ScoreObject>;
	//	�p�r: �v���C���[���C�e�ɓ��������C�x���g
	//--------------------------------------------------------------------------------------
	class PlayerHitEvent : public EventState<ScoreObject>
	{
		PlayerHitEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<PlayerHitEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event)override;
	};





}
//endof  basedx11
