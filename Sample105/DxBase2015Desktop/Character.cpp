#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	�p�r: �C�e
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ShellBall::ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
		const Vector3& JumpVec) :
		GameObject(StagePtr), m_StartPos(StartPos),
		m_NowScale(0.25f, 0.25f, 0.25f), m_JumpVec(JumpVec)
	{}
	ShellBall::~ShellBall(){}
	//������
	void ShellBall::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetRotation(0, 0, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//�W�����v�X�^�[�g
		PtrGravity->StartJump(m_JumpVec);

		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<ShellBall> >(GetThis<ShellBall>());
		//�ŏ��̃X�e�[�g��FiringState�ɐݒ�
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}

	void ShellBall::Refresh(const Vector3& StartPos, const Vector3& JumpVec){
		SetUpdateActive(true);
		SetDrawActive(true);
		m_StartPos = StartPos;
		m_JumpVec = JumpVec;
		//Transform�擾
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetPosition(m_StartPos);
		//�`��R���|�[�l���g
		auto PtrDraw = GetComponent<BasicPNTDraw>();
		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		//�d�͂����o��
		auto PtrGravity = GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		PtrGravity->StartJump(m_JumpVec);

		//���̃X�e�[�g��FiringState�ɐݒ�
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}


	void ShellBall::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
		//�X�e�[�g�}�V�����g�����Ƃ�Update�����𕪎U�ł���
	}

	//���������o����֐�
	//�n�ʂɂ������ǂ���
	bool ShellBall::IsArrivedBaseMotion(){
		//�d�͂����o��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->IsGravityVelocityZero()){
			//�������x��0�Ȃ�true
			return true;
		}
		return false;
	}

	//�����̊J�n
	void ShellBall::ExplodeStartMotion(){
		//Transform�擾
		auto Ptr = GetComponent<Transform>();
		m_NowScale = Vector3(5.0f, 5.0f, 5.0f);
		Ptr->SetScale(m_NowScale);
		//�`��R���|�[�l���g
		auto PtrDraw = GetComponent<BasicPNTDraw>();
		//�������͐�
		PtrDraw->SetDiffuse(Color4(1.0f, 0.0f, 0, 1.0f));
	}

	//�����̉��o(���o�I���ōX�V�ƕ`��𖳌��ɂ���j
	void ShellBall::ExplodeExcuteMotion(){
		//Transform�擾
		auto Ptr = GetComponent<Transform>();
		m_NowScale *= 0.9f;
		if (m_NowScale.x < 0.25f){
			m_NowScale = Vector3(0.25f, 0.25f, 0.25f);
			//�\���ƍX�V�����Ȃ��悤�ɂ���
			//��������Ƃ��̌�A�X�V�y�ѕ`��n�͑S���Ă΂�Ȃ��Ȃ�
			//�ĂэX�V�`�悷�邽�߂ɂ́A�O�����瑀�삪�K�v�i�v���C���[���ĂыN�����j
			SetUpdateActive(false);
			SetDrawActive(false);
		}
		Ptr->SetScale(m_NowScale);
	}


	//--------------------------------------------------------------------------------------
	//	class FiringState : public ObjState<ShellBall>;
	//	�p�r: ���˂��甚���܂ł̃X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<FiringState> FiringState::Instance(){
		static shared_ptr<FiringState> instance;
		if (!instance){
			instance = shared_ptr<FiringState>(new FiringState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void FiringState::Enter(const shared_ptr<ShellBall>& Obj){
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void FiringState::Execute(const shared_ptr<ShellBall>& Obj){
		//�����I�����ǂ����`�F�b�N
		if (Obj->IsArrivedBaseMotion()){
			//�����I���Ȃ�X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(ExplodeState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void FiringState::Exit(const shared_ptr<ShellBall>& Obj){
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	//	class ExplodeState : public ObjState<ShellBall>;
	//	�p�r: �����Œ��̃X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<ExplodeState> ExplodeState::Instance(){
		static shared_ptr<ExplodeState> instance;
		if (!instance){
			instance = shared_ptr<ExplodeState>(new ExplodeState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void ExplodeState::Enter(const shared_ptr<ShellBall>& Obj){
		//�����̊J�n
		Obj->ExplodeStartMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void ExplodeState::Execute(const shared_ptr<ShellBall>& Obj){
		//�������o�̎��s
		Obj->ExplodeExcuteMotion();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void ExplodeState::Exit(const shared_ptr<ShellBall>& Obj){
		//�������Ȃ�
	}



}
//endof  basedx11
