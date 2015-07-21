
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

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRedit = AddComponent<Rigidbody>();
		//���т��т��������ɂȂ�悤�A���ʂ�������B
		PtrRedit->SetMass(0.5f);
		//Seek����
		AddComponent<SeekSteering>();
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
				//���K��
				Angle.Normalize();
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
		auto ColPtr = GetComponent<CollisionSphere>();
		if (ColPtr->GetHitObject() && GetStateMachine()->GetCurrentState() == JumpState::Instance()){
			GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}


	//���[�V��������������֐��Q
	//�ړ����Č������ړ������ɂ���
	void Player::MoveRotationMotion(){
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();

		//���݈ʒu�����o��
		auto Pos = PtrTransform->GetPosition();
		//�ړ����������Z�B
		//�ړ������������킩��΂����̂ŁA
		//Angle�͐��K������Ăėǂ�
		Pos += Angle;
		//Seek����
		auto PtrSeek = GetComponent<SeekSteering>();
		//���Z���ꂽ�����ɒǂ�������
		PtrSeek->SetTargetPosition(Pos);
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

	//A�{�^���ŃW�����v����ǂ����𓾂�
	bool Player::IsJumpMotion(){
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//A�{�^���������ꂽ�u�ԂȂ�W�����v
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
		PtrGravity->StartJump(JumpVec,0.01f);
	}
	//A�{�^���ŃW�����v���Ă���Ԃ̏���
	//�W�����v�I��������true��Ԃ�
	bool Player::JumpMoveMotion(){
		auto PtrTransform = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0 || PtrTransform->GetParent()){
			return true;
		}
		return false;
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
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
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
		if (Obj->JumpMoveMotion()){
			//�ʏ��Ԃɖ߂�
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void JumpState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}





/*

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

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRedit = AddComponent<Rigidbody>();
		//Seek����
		AddComponent<SeekSteering>();
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
				//���K��
				Angle.Normalize();
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void Player::StartJump(){
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


	//�X�V
	void Player::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();

		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();

		//���݈ʒu�����o��
		auto Pos = PtrTransform->GetPosition();
		//�ړ����������Z�B
		//�ړ������������킩��΂����̂ŁA
		//Angle�͐��K������Ăėǂ�
		Pos += Angle;
		//Seek����
		auto PtrSeek = GetComponent<SeekSteering>();
		//���Z���ꂽ�����ɒǂ�������
		PtrSeek->SetTargetPosition(Pos);
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

	void Player::Update2(){
		auto ColPtr = GetComponent<CollisionSphere>();
		if (ColPtr->GetHitObject() && GetStateMachine()->GetCurrentState() == JumpState::Instance()){
			GetStateMachine()->ChangeState(DefaultState::Instance());

		}
	}





	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	static DefaultState* Instance();
	//	�p�r: �C���X�^���X�̎擾
	//	�߂�l: DefaultState�̃C���X�^���X
	//--------------------------------------------------------------------------------------
	shared_ptr<DefaultState> DefaultState::Instance(){
		static shared_ptr<DefaultState> instance;
		if (!instance){
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//--------------------------------------------------------------------------------------
	//	virtual void Execute(
	//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
	//	) = 0;
	//	�p�r: Update�̂Ƃ��Ɏ��s�����
	//	�߂�l: �Ȃ��i�������z�֐��j
	//--------------------------------------------------------------------------------------
	void DefaultState::Execute(const shared_ptr<Player>& Obj){
		//�R���g���[���̎擾
		//�R���g���[����App���Ȃ̂ŃX�e�[�g����Ă�ł����Ȃ��B
		//
		//�������A�X�e�[�g�N���X�ɂ̓����o�ϐ��݂͐��Ȃ����ƁI�i�V���O���g���̂��߁j
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//A�{�^���������ꂽ�u�ԂȂ�X�e�[�g�ύX
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				Obj->GetStateMachine()->ChangeState(JumpState::Instance());
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	static shared_ptr<JumpState> Instance();
	//	�p�r: �C���X�^���X�̎擾
	//	�߂�l: DefaultState�̃C���X�^���X
	//--------------------------------------------------------------------------------------
	shared_ptr<JumpState> JumpState::Instance(){
		static shared_ptr<JumpState> instance;
		if (!instance){
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//--------------------------------------------------------------------------------------
	//	virtual void JumpState::Enter(
	//	const shared_ptr<Player>& Obj	//�X�e�[�g��ێ�����I�u�W�F�N�g
	//	) = 0;
	//	�p�r: �X�e�[�g�ɓ������Ƃ��Ɏ��s�����
	//	�߂�l: �Ȃ��i�������z�֐��j
	//--------------------------------------------------------------------------------------
	void JumpState::Enter(const shared_ptr<Player>& Obj){
		Obj->StartJump();
	}
	//--------------------------------------------------------------------------------------
	//	virtual void JumpState::Execute(
	//	const shared_ptr<Player>& Obj		//�X�e�[�g��ێ�����I�u�W�F�N�g
	//	) = 0;
	//	�p�r: Update�̂Ƃ��Ɏ��s�����
	//	�߂�l: �Ȃ��i�������z�֐��j
	//--------------------------------------------------------------------------------------
	void JumpState::Execute(const shared_ptr<Player>& Obj){
		auto PtrTransform = Obj->GetComponent<Transform>();
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0 || PtrTransform->GetParent()){
			//�ʏ��Ԃɖ߂�
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}

*/


}
//endof  basedx11
