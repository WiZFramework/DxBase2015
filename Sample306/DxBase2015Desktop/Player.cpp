
#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_3head.ssae", L"wait"),
		m_MaxSpeed(40.0f),	//�ō����x
		m_Decel(0.95f),	//�����l
		m_Mass(1.0f),	//����
		m_ZRailIndex(0),
		m_ZRailBeforIndex(0)
	{
		m_ZRail = { 0, 2.0f, 4.0f, 6.0f, 8.0f };
		m_ToAnimeMatrixLeft.DefTransformation(
			Vector3(0.1f, 0.1f, 0.1f),
			Vector3(0, 0, 0),
			Vector3(0, -0.55f, 0.0f)
			);
		m_ToAnimeMatrixRight.DefTransformation(
			Vector3(-0.1f, 0.1f, 0.1f),
			Vector3(0, 0, 0),
			Vector3(0, -0.55f, 0.0f)
			);

	}

	//������
	void Player::Create(){
		//�ۉe�i�����ڂɍ쐬����j
		//���_�͕ύX�ł��Ȃ��Ă悢
		m_ShadowResource = CommonMeshResource::CreateSphere(0.8f,18);

		//���ƂȂ�I�u�W�F�N�g����A�j���[�V�����I�u�W�F�N�g�ւ̍s��̐ݒ�
		SetToAnimeMatrix(m_ToAnimeMatrixLeft);

		auto PtrT = GetComponent<Transform>();
		PtrT->SetPosition(Vector3(0, 0.5f, m_ZRail[m_ZRailIndex]));
		//�e�N���X�̃N���G�C�g���Ă�
		SS5ssae::Create();
		//�l�͕b������̃t���[����
		SetFps(30.0f);

		//Rigidbody������
		auto PtrRedit = AddComponent<Rigidbody>();
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.5f);
		//�Փ˔��������(OBB)
		auto PtrCol = AddComponent<CollisionObb>();
		//CollisionSphere���g���ꍇ�́AOBB�𖳌��ɂ��Ĉȉ���L���ɂ���
		//auto PtrCol = AddComponent<CollisionSphere>();
		//�R���W������\������ꍇ�͈ȉ���L��
		//PtrCol->SetDrawActive(true);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(m_ShadowResource);

		//Action������
		//���C���[�ύX�p
		auto PtrAction = AddComponent<Action>();
		//�A�N�V�����͖����ɂ��Ă���
		PtrAction->SetUpdateAllActive(false);


		//0�Ԗڂ̃r���[�̃J�����𓾂�
		//MyCamera�ł���
		auto PtrCamera = dynamic_pointer_cast<MyCamera>(GetStage()->GetCamera(0));
		//LookAtCamera�ł���
//		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//Camera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetToTargetLerp(0.1f);
		}

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��WaitState�ɐݒ�
		m_StateMachine->SetCurrentState(WaitState::Instance());
		//WaitState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<Player>());
	}

	//�ړ��̕����𓾂�
	float Player::GetMoveX(){
		float MoveX = 0;
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0){
				//�R���g���[���̌����v�Z
				MoveX = CntlVec[0].fThumbLX;
			}
		}
		return MoveX;
	}


	//�X�V
	void Player::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Player::Update2(){
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		wstring str = FPS;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
		auto ColPtr = GetComponent<Collision>();
		//���݂̃X�e�[�g��RailChangeState��������
		if (GetStateMachine()->GetCurrentState() == RailChangeState::Instance()){
			if (ColPtr->GetHitObject()){
				//�����ƏՓ˂����A�����I�ɖ߂����
				RailChangeBeforStartMotion();
				ColPtr->ClearBothHitObject();
				return;
			}
		}
		if (ColPtr->GetHitObject() && GetStateMachine()->GetCurrentState() == JumpState::Instance()){
			GetStateMachine()->ChangeState(WaitState::Instance());
		}
	}


	//���[�V��������������֐��Q
	//�ړ����Č������ړ������ɂ���
	//�ړ�������Ԃ�
	float  Player::MoveRotationMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto MoveX = GetMoveX();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbody�����o��
		auto PtrRedit = GetComponent<Rigidbody>();
		//���݂̑��x�����o��
		auto Velo = PtrRedit->GetVelocity().x;
		//�ړI�n���ō����x���|���ċ��߂�
		auto Target = MoveX * m_MaxSpeed;
		//�ړI�n�Ɍ��������߂ɗ͂̂�����������v�Z����
		//Force�̓t�H�[�X�ł���
		auto Force = Target - Velo;
		//�����x�����߂�
		auto Accel = Force / m_Mass;
		//�^�[�����Ԃ��|�������̂𑬓x�ɉ��Z����
		Velo += (Accel * ElapsedTime);
		//��������
		Velo *= m_Decel;
		//���x��ݒ肷��
		Vector3 VecVelo(Velo, 0, 0);
		PtrRedit->SetVelocity(VecVelo);
		////��]�̌v�Z
		//���ƂȂ�I�u�W�F�N�g����A�j���[�V�����I�u�W�F�N�g�ւ̍s��̐ݒ�
		//Transform�̃X�P�[�����O��-1�ɂ���ƏՓ˔��肪���܂������Ȃ��̂�
		//SpriteStdio�̕��������ύX����
		if (MoveX >= 0.0f){
			SetToAnimeMatrix(m_ToAnimeMatrixRight);
		}
		else{
			SetToAnimeMatrix(m_ToAnimeMatrixLeft);
		}
		//MoveX��Ԃ�
		return MoveX;
	}

	void Player::AnimeChangeMotion(const wstring& key, bool looped){
		ChangeAnimation(key);
		SetLooped(looped);
	}

	void Player::LoopedAnimeUpdateMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		UpdateAnimeTime(ElapsedTime);
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
	void Player::JumpStartMotion(){
		auto PtrTrans = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec, 0.05f);
		auto PtrCol = GetComponent<Collision>();
		//�e����������؂藣��
//		PtrTrans->SetParent(nullptr);
	}
	//A�{�^���ŃW�����v���Ă���Ԃ̏���
	//�W�����v�I��������true��Ԃ�
	bool Player::JumpMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		//�W�����v�͒n�ʂɒ����܂ŃA�j���[�V�����ύX�͂��Ȃ�
		UpdateAnimeTime(ElapsedTime);
		auto PtrTransform = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrTransform->GetPosition().y == PtrGravity->GetBaseY() || PtrTransform->GetParent()){
			//��ԉ��ɂ������A�e�ɏ�����Ƃ�
			//�W�����v�I��
			return true;
		}
		return false;
	}


	//B�{�^���ŃA�^�b�N����ǂ����𓾂�
	bool Player::IsAttackMotion(){
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
	//B�{�^���ŃA�^�b�N���鏈��
	bool Player::AttackMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		UpdateAnimeTime(ElapsedTime);
		if (IsAnimeEnd()){
			return true;
		}
		return false;
	}

	//���X�e�B�b�N��Z���[����ύX���邩�ǂ���
	bool Player::IsRailChangeMotion(){
		auto PtrTrans = GetComponent<Transform>();
		auto PtrGra = GetComponent<Gravity>();
		if (PtrTrans->GetPosition().y > PtrGra->GetBaseY()){
			//�����̏�ɏ���Ă鎞�͕ύX�ł��Ȃ�
			return false;
		}
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (abs(CntlVec[0].fThumbLY) > 0.9f){
				//�R���g���[���̌���Y
				if (CntlVec[0].fThumbLY > 0){
					//���݂̃��[���[����ԉ��Ȃ�
					//0��Ԃ�
					if (m_ZRail.size() <= m_ZRailIndex + 1){
						return false;
					}
					else{
						m_ZRailBeforIndex = m_ZRailIndex;
						m_ZRailIndex++;
					}
					return true;
				}
				else if (CntlVec[0].fThumbLY < 0){
					//���݂̃��[������Ԏ�O�Ȃ�
					//0��Ԃ�
					if (m_ZRailIndex == 0){
						return false;
					}
					else{
						m_ZRailBeforIndex = m_ZRailIndex;
						m_ZRailIndex--;
					}
					return true;
				}
			}
		}
		return false;
	}

	//Z���[���ύX���J�n����
	void Player::RailChangeStartMotion(){
		//Rigidbody���擾
		auto PtrRedit = GetComponent<Rigidbody>();
		//�����ɂ���
		PtrRedit->SetUpdateAllActive(false);
		//�d�͂��擾
		auto PtrGravity = GetComponent<Gravity>();
		//�����ɂ���
		PtrGravity->SetUpdateAllActive(false);
		//�Փ˔�����擾
		auto PtrCol = GetComponent<Collision>();
		if (PtrCol->GetHitObject()){
			//�����Փ˂��Ă�����
			//������Ƒ���̏Փˏ����N���A����
			PtrCol->ClearBothHitObject();
		}

		auto PtrTrans = GetComponent<Transform>();
		auto NowPos = PtrTrans->GetPosition();

		//Action���擾����
		auto PtrAction = AddComponent<Action>();
		PtrAction->SetUpdateAllActive(true);
		//�o�^����Ă���A�N�V�������N���A����
		PtrAction->AllActionClear();
		PtrAction->AddMoveTo(1.0f, Vector3(NowPos.x, NowPos.y, m_ZRail[m_ZRailIndex]));
		//���[�v���Ȃ�
		PtrAction->SetLooped(false);
		PtrAction->Run();
	}

	//Z���[���ύX���X�V����
	bool Player::RailChangeMotion(){
		//Action�͏���Ɏ��s�����̂ŁA�������I��������ǂ��������`�F�b�N����
		//Action���擾����
		auto PtrAction = AddComponent<Action>();
		if (PtrAction->IsArrived()){
			return true;
		}
		return false;
	}

	//Z���[���������I�ɖ߂�
	void Player::RailChangeBeforStartMotion(){
		auto PtrTrans = GetComponent<Transform>();
		auto NowPos = PtrTrans->GetPosition();
		//Action���擾����
		auto PtrAction = AddComponent<Action>();
		PtrAction->SetUpdateAllActive(true);
		//�o�^����Ă���A�N�V�������N���A����
		PtrAction->AllActionClear();
		auto temp = m_ZRailIndex;
		m_ZRailIndex = m_ZRailBeforIndex;
		m_ZRailBeforIndex = temp;
		PtrAction->AddMoveTo(0.5f, Vector3(NowPos.x, NowPos.y, m_ZRail[m_ZRailIndex]));
		//���[�v���Ȃ�
		PtrAction->SetLooped(false);
		PtrAction->Run();
	}


	//Z���[���ύX���I������
	void Player::RailChangeEndMotion(){
		//Action���擾����
		auto PtrAction = AddComponent<Action>();
		//�����ɂ���
		PtrAction->SetUpdateAllActive(false);
		//Rigidbody���擾
		auto PtrRedit = GetComponent<Rigidbody>();
		PtrRedit->SetVelocity(0, 0, 0);
		//�L���ɂ���
		PtrRedit->SetUpdateAllActive(true);
		//�d�͂��擾
		auto PtrGravity = GetComponent<Gravity>();
		//�L���ɂ���
		PtrGravity->SetUpdateAllActive(true);
		//�Փ˔�����擾
		auto PtrCol = GetComponent<Collision>();
		//�L���ɂ���
		PtrCol->SetUpdateAllActive(true);

	}





	//--------------------------------------------------------------------------------------
	//	class WaitState : public ObjState<Player>;
	//	�p�r: �ҋ@���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<WaitState> WaitState::Instance(){
		static shared_ptr<WaitState> instance;
		if (!instance){
			instance = shared_ptr<WaitState>(new WaitState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void WaitState::Enter(const shared_ptr<Player>& Obj){
		Obj->AnimeChangeMotion(L"wait", true);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void WaitState::Execute(const shared_ptr<Player>& Obj){
		//�A�j���[�V�����X�V
		Obj->LoopedAnimeUpdateMotion();
		
		if (Obj->IsRailChangeMotion()){
			//Z���[���ύX
			Obj->GetStateMachine()->ChangeState(RailChangeState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX > 0.9f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(RunState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		else if (AbsMoveX > 0.0f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void WaitState::Exit(const shared_ptr<Player>& Obj){
	}


	//--------------------------------------------------------------------------------------
	//	class RailChangeState : public ObjState<Player>;
	//	�p�r: Z���[���ύX���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<RailChangeState> RailChangeState::Instance(){
		static shared_ptr<RailChangeState> instance;
		if (!instance){
			instance = shared_ptr<RailChangeState>(new RailChangeState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void RailChangeState::Enter(const shared_ptr<Player>& Obj){
		//Z���[���ύX�̓A�j��
		Obj->AnimeChangeMotion(L"charge", true);
		Obj->RailChangeStartMotion();

	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void RailChangeState::Execute(const shared_ptr<Player>& Obj){
		//�A�j���[�V�����X�V
		Obj->LoopedAnimeUpdateMotion();
		if (Obj->RailChangeMotion()){
			//�������I�������X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void RailChangeState::Exit(const shared_ptr<Player>& Obj){
		Obj->RailChangeEndMotion();
	}



	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<WalkState> WalkState::Instance(){
		static shared_ptr<WalkState> instance;
		if (!instance){
			instance = shared_ptr<WalkState>(new WalkState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void WalkState::Enter(const shared_ptr<Player>& Obj){
		Obj->AnimeChangeMotion(L"walk", true);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void WalkState::Execute(const shared_ptr<Player>& Obj){
		//�A�j���[�V�����X�V
		Obj->LoopedAnimeUpdateMotion();
		
		if (Obj->IsRailChangeMotion()){
			//Z���[���ύX
			Obj->GetStateMachine()->ChangeState(RailChangeState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}

		
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX > 0.9f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(RunState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		else if (AbsMoveX == 0.0f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		if (Obj->IsJumpMotion()){
			//Jump�{�^���ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void WalkState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}





	//--------------------------------------------------------------------------------------
	//	class RunState : public ObjState<Player>;
	//	�p�r: ����A�j���[�V����
	//--------------------------------------------------------------------------------------
	shared_ptr<RunState> RunState::Instance(){
		static shared_ptr<RunState> instance;
		if (!instance){
			instance = shared_ptr<RunState>(new RunState);
		}
		return instance;
	}
	void RunState::Enter(const shared_ptr<Player>& Obj){
		Obj->AnimeChangeMotion(L"run",true);
	}
	void  RunState::Execute(const shared_ptr<Player>& Obj){
		//�A�j���[�V�����X�V
		Obj->LoopedAnimeUpdateMotion();
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX <= 0.9f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		else if (AbsMoveX == 0.0f){
			//���ȏ�̃X�s�[�h�ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		if (Obj->IsAttackMotion()){
			Obj->GetStateMachine()->ChangeState(AttackState::Instance());
			//�ύX��͈ȍ~�̏����͍s��Ȃ�
			return;
		}
		if (Obj->IsJumpMotion()){
			//Jump�{�^���ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	void  RunState::Exit(const shared_ptr<Player>& Obj){
		//�X�e�[�g�I�������͉������Ȃ�
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
		Obj->AnimeChangeMotion(L"jump_all", false);
		Obj->JumpStartMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void JumpState::Execute(const shared_ptr<Player>& Obj){
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (Obj->JumpMotion()){
			if (AbsMoveX > 0.9f){
				//�����Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(RunState::Instance());
			}
			if (AbsMoveX == 0.0f){
				//�҂���Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			}
			else{
				//������Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			}
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void JumpState::Exit(const shared_ptr<Player>& Obj){
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	//	class AttackState : public ObjState<Player>;
	//	�p�r: �A�^�b�N�A�j���[�V����
	//--------------------------------------------------------------------------------------
	shared_ptr<AttackState> AttackState::Instance(){
		static shared_ptr<AttackState> instance;
		if (!instance){
			instance = shared_ptr<AttackState>(new AttackState);
		}
		return instance;
	}
	void AttackState::Enter(const shared_ptr<Player>& Obj){
		Obj->AnimeChangeMotion(L"attack2", false);
		Obj->SetFps(40.0f);
	}
	void  AttackState::Execute(const shared_ptr<Player>& Obj){
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (Obj->AttackMotion()){
			if (AbsMoveX > 0.9f){
				//�����Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(RunState::Instance());
			}
			if (AbsMoveX == 0.0f){
				//�҂���Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			}
			else{
				//������Ԃɖ߂�
				Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			}
		}
	}
	void  AttackState::Exit(const shared_ptr<Player>& Obj){
		Obj->SetFps(30.0f);
	}



}
//endof  basedx11
