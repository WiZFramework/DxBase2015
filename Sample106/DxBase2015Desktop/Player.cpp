
#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr){}

	//������
	void Player::Create(){
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//��������
		SetAlphaActive(true);
		//0�Ԗڂ̃r���[�̃J�����𓾂�
		//LookAtCamera�ł���
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��DefaultState�ɐݒ�
		m_StateMachine->SetCurrentState(DefaultState::Instance());
		//DefaultState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<Player>());
	}

	//�ړ��̌����𓾂�
	Vector3 Player::GetAngle(){
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 && CntlVec[0].fThumbLY != 0){
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetCamera(0);
				//�i�s�����̌������v�Z
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}

	//�X�V
	void Player::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Player::Update2(){
		//�Փ˔���𓾂�
		auto PtrCollision = GetComponent<CollisionSphere>();
		if (PtrCollision->GetHitObject()){
			auto PtrShell = dynamic_pointer_cast<ShellBall>(PtrCollision->GetHitObject());
			if (PtrShell){
				//�C�e�ɓ���������X�e�[�g�ύX
				GetStateMachine()->ChangeState(ShellHitState::Instance());
			}
		}
	}

	//�ړ����Č������ړ������ɂ���
	void Player::MoveRotationMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//���݈ʒu�����o��
		auto Pos = PtrTransform->GetPosition();
		//�ړ����������Z�B
		Pos += Angle * (ElapsedTime * 10.0f);
		PtrTransform->SetPosition(Pos);
		//��]�̌v�Z
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f){
			//�x�N�g����Y����]�ɕϊ�
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else{
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);
	}

	//�C�e�ƏՓ˂����u�Ԃ̏���
	void Player::ShellHitMotion(){
		//�Փ˔���𓾂�
		auto PtrCollision = GetComponent<CollisionSphere>();
		//�Փ˂���
		if (PtrCollision->GetHitObject()){
			auto ShellPtr = dynamic_pointer_cast<ShellBall>(PtrCollision->GetHitObject());
			if (ShellPtr){
				//���肪�C�e������
				//�X�R�A�I�u�W�F�N�g�ɃC�x���g���o
				auto PtrScoreObject = GetStage()->GetSharedGameObject<ScoreObject>(L"ScoreObject");
				PostEvent(0, GetThis<Player>(), PtrScoreObject, L"PlayerHit");
			}
			//�����Transform�𓾂�B
			auto PtrOtherTrans = PtrCollision->GetHitObject()->GetComponent<Transform>();
			//����̏ꏊ�𓾂�
			auto OtherPos = PtrOtherTrans->GetPosition();

			//Transform�𓾂�B
			auto PtrTrans = GetComponent<Transform>();
			//�ꏊ�𓾂�
			auto Pos = PtrTrans->GetPosition();

			//��ԕ������v�Z����
			Pos -= OtherPos;
			Pos.Normalize();
			Pos.y = 0;
			Pos *= 6.0f;
			Pos += Vector3(0, 6.0f, 0);

			//�Փ˂��Ȃ��ɂ���i�̂��ɕ����j
			PtrCollision->SetHitObject(nullptr);
			PtrCollision->SetUpdateActive(false);

			//�d�͂𓾂�
			auto PtrGravity = GetComponent<Gravity>();
			//�W�����v�X�^�[�g
			PtrGravity->StartJump(Pos);
		}
	}

	//�C�e�ƏՓ˂�����̏���
	//�����I��������true��Ԃ�
	bool Player::ShellHitMoveMotion(){
		//�d�͂𓾂�
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->IsGravityVelocityZero()){
			//�����I��
			//�Փ˔���𓾂�
			auto PtrCollision = GetComponent<CollisionSphere>();
			//�Փ˂�L���ɂ���
			PtrCollision->SetUpdateActive(true);
			return true;
		}
		return false;
	}

	//A�{�^���ŃW�����v����ǂ����𓾂�
	bool Player::IsJumpMotion(){
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//A�{�^���������ꂽ�u�ԂȂ�C�e����
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				return true;
			}
		}
		return false;
	}

	//A�{�^���ŃW�����v����u�Ԃ̏���
	void Player::JumpMotion(){
		auto PtrTrans = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		if (PtrTrans->GetParent()){
			//�e��������A�A�N�V�����R���|�[�l���g�̈ړ��A�N�V������T��
			//�ړ��{�b�N�X�ɏ���Ă���ꍇ�A���̊������W�����v�ɉ��Z����
			auto ActionPtr = PtrTrans->GetParent()->GetComponent<Action>(false);
			if (ActionPtr){
				JumpVec += ActionPtr->GetVelocity();
			}
		}
		PtrGravity->StartJump(JumpVec);
	}
	//A�{�^���ŃW�����v���Ă���Ԃ̏���
	//�W�����v�I��������true��Ԃ�
	bool Player::JumpMoveMotion(){
		//�d�͂𓾂�
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->IsGravityVelocityZero()){
			//�����I��
			return true;
		}
		return false;
	}


	//B�{�^���ŖC�e�𔭎˂���ǂ����𓾂�
	bool Player::IsShellThrowMotion(){
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//B�{�^���������ꂽ�u�ԂȂ�C�e����
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
				return true;
			}
		}
		return false;
	}
	//B�{�^���ŖC�e�𔭎˂��鏈��
	void Player::ShellThrowMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//�C�e�̒ǉ�
		auto PtrTrans = GetComponent<Transform>();
		//�v���C���[�̌����𓾂�
		auto PlayerAngle = PtrTrans->GetRotation();
		Vector3 ShellSpeed(sin(PlayerAngle.y), 0, cos(PlayerAngle.y));
		ShellSpeed *= 10.0f;
		//�v���C���[�̈ړ��X�s�[�h�𓾂�
		//�ړ����������Z�B
		Vector3 Velo = PtrTrans->GetPosition() - PtrTrans->GetBeforeWorldPosition();
		Velo /= ElapsedTime;
		Velo.y = 0;
		//�ړ��X�s�[�h�����Z
		ShellSpeed += Velo;
		//�ł��グ�̏�����̏����x��ǉ��i�l�͌Œ�j
		ShellSpeed += Vector3(0.0f, 4.0f, 0);
		//�O���[�v���ɋ󂫂�����΂��̃I�u�W�F�N�g���ė��p
		//�����łȂ���ΐV�K�ɍ쐬
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		for (auto Ptr : ShellVec){
			//Ptr��weak_ptr�Ȃ̂ŗL�����`�F�b�N���K�v
			if (!Ptr.expired()){
				auto ShellPtr = dynamic_pointer_cast<ShellBall>(Ptr.lock());
				if (ShellPtr){
					if ((!ShellPtr->IsUpdateActive()) && (!ShellPtr->IsDrawActive())){
						ShellPtr->Refresh(PtrTrans->GetPosition(), ShellSpeed,false);
						return;
					}
				}
			}
		}
		//�����܂ŗ�����󂫂��Ȃ��������ƂɂȂ�
		//�C�e�̒ǉ�
		auto Sh = GetStage()->AddGameObject<ShellBall>(PtrTrans->GetPosition(), ShellSpeed,false);
		//�O���[�v�ɒǉ�
		Group->IntoGroup(Sh);
	}


	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<DefaultState> DefaultState::Instance(){
		static shared_ptr<DefaultState> instance;
		if (!instance){
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void DefaultState::Enter(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DefaultState::Execute(const shared_ptr<Player>& Obj){
		Obj->MoveRotationMotion();
		if (Obj->IsShellThrowMotion()){
			Obj->ShellThrowMotion();
		}
		if (Obj->IsJumpMotion()){
			//Jump�{�^���ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DefaultState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	//	class ShellHitState : public ObjState<Player>;
	//	�p�r: �C�e�����������Ƃ��̏���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<ShellHitState> ShellHitState::Instance(){
		static shared_ptr<ShellHitState> instance;
		if (!instance){
			instance = shared_ptr<ShellHitState>(new ShellHitState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void ShellHitState::Enter(const shared_ptr<Player>& Obj){
		//�Փ˂����u�Ԃ̏���
		Obj->ShellHitMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void ShellHitState::Execute(const shared_ptr<Player>& Obj){
		if (Obj->ShellHitMoveMotion()){
			//�����I���Ȃ�X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void ShellHitState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: A�{�^���ŃW�����v�����Ƃ��̏���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<JumpState> JumpState::Instance(){
		static shared_ptr<JumpState> instance;
		if (!instance){
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void JumpState::Enter(const shared_ptr<Player>& Obj){
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		Obj->JumpMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void JumpState::Execute(const shared_ptr<Player>& Obj){
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		//�W�����v�����C�e���ˉ\
		if (Obj->IsShellThrowMotion()){
			Obj->ShellThrowMotion();
		}
		if (Obj->JumpMoveMotion()){
			//�����I���Ȃ�X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void JumpState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}



}
//endof  basedx11
