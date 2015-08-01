#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	�p�r: �C�e
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ShellBall::ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
		const Vector3& JumpVec, bool IsEnemyBall) :
		GameObject(StagePtr), m_StartPos(StartPos),
		m_NowScale(0.25f, 0.25f, 0.25f), m_JumpVec(JumpVec),
		m_InStartTime(0), m_IsEnemyBall(IsEnemyBall)
	{}
	ShellBall::~ShellBall(){}
	//������
	void ShellBall::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetRotation(0, 0, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCollision = AddComponent<CollisionSphere>();
		//�Փ˂͖����ɂ��Ă���
		PtrCollision->SetUpdateActive(false);
		//�C�e�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		//�C�e���m�͏Փ˂��Ȃ��悤�ɂ��Ă���
		PtrCollision->SetExcludeCollisionGroup(Group);


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
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 1.0f, 0, 1.0f));
		}

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<ShellBall> >(GetThis<ShellBall>());
		//�ŏ��̃X�e�[�g��FiringState�ɐݒ�
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}

	void ShellBall::Refresh(const Vector3& StartPos, const Vector3& JumpVec, bool IsEnemyBall){
		SetUpdateActive(true);
		SetDrawActive(true);
		m_StartPos = StartPos;
		m_JumpVec = JumpVec;
		m_InStartTime = 0;
		m_IsEnemyBall = IsEnemyBall;
		//Transform�擾
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetPosition(m_StartPos);
		//�`��R���|�[�l���g
		auto PtrDraw = GetComponent<BasicPNTDraw>();
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 1.0f, 0, 1.0f));
		}
		//�Փ˔�����Ăяo��
		auto PtrCollision = GetComponent<CollisionSphere>();
		//�Փ˂͖����ɂ��Ă���
		PtrCollision->SetUpdateActive(false);

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
	//���ˌ���̎��Ԃ���������Փ˂��A�N�e�B�u�ɂ���
	void ShellBall::HitTestCheckMotion(){
		//�Փ˔�����Ăяo��
		auto PtrCollision = GetComponent<CollisionSphere>();
		if (PtrCollision->IsUpdateActive()){
			//���ɏՓ˂͗L��
			return;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_InStartTime += ElapsedTime;
		if (m_InStartTime > 0.5f){
			//�Փ˂�L���ɂ���
			PtrCollision->SetUpdateActive(true);
		}
	}


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
		//�������̐F
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 0.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 0.0f, 1.0f, 1.0f));
		}

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
		//�o�ߎ��Ԃɂ���ďՓ˂�L���ɂ���
		Obj->HitTestCheckMotion();
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



	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//	�p�r: �G�L�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos)
	{}
	Enemy::~Enemy(){}
	//������
	void Enemy::Create(){

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<Enemy> >(GetThis<Enemy>());
		//�ŏ��̃X�e�[�g��DefaultState�ɐݒ�
		m_StateMachine->SetCurrentState(EnemyDefaultState::Instance());
		//DefaultState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<Enemy>());

	}

	void Enemy::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy::Update2(){
		//�Փ˔���𓾂�
		auto PtrCollision = GetComponent<CollisionSphere>();
		if (PtrCollision->GetHitObject()){
			//����������X�e�[�g�ύX
			GetStateMachine()->ChangeState(EnemyShellHitState::Instance());
		}
	}

	//���[�V��������������֐��Q

	//���̊֐��̓��\�b�h�Ăяo���֐��Ƃ��Ď�������
	void Enemy::ExtMotion(){
		//���z�֐��Ăяo��
		//���̌Ăяo���Ŕh���N���X�̉��z�֐������s�����
		ExtMotionMethod();
	}
	//�C�e�ƏՓ˂����u�Ԃ̏���
	void Enemy::ShellHitMotion(){
		//���z�֐��Ăяo��
		//���̌Ăяo���Ŕh���N���X�̉��z�֐������s�����
		ShellHitMotionMethod();
	}


	//�C�e�𔭎˂���֐�
	//ShellThrowMotion()����Ă΂��
	void Enemy::StartShellBall(){
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto ShellAngle = PlayerPos - Pos;
		float len = ShellAngle.Length();
		ShellAngle.y = 0;
		ShellAngle.Normalize();
		ShellAngle *= len;
		//�ł��グ�̏�����̏����x��ǉ��i�l�͌Œ�j
		ShellAngle += Vector3(0.0f, 5.0f, 0);
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
						ShellPtr->Refresh(Pos, ShellAngle,true);
						return;
					}
				}
			}
		}
		//�����܂ŗ�����󂫂��Ȃ��������ƂɂȂ�
		//�C�e�̒ǉ�
		auto Sh = GetStage()->AddGameObject<ShellBall>(Pos, ShellAngle,true);
		//�O���[�v�ɒǉ�
		Group->IntoGroup(Sh);
	}


	//�m���ɂ��C�e�𔭎˂��郂�[�V����
	void Enemy::ShellThrowMotion(){
		//100����1�̊m���ŖC�e�𔭎�
		if (Util::DivProbability(100)){
			StartShellBall();
		}
	}

	//�C�e�ƏՓ˂�����̏���
	//�����I��������true��Ԃ�
	bool Enemy::ShellHitMoveMotion(){
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



	//--------------------------------------------------------------------------------------
	//	class EnemyDefaultState : public ObjState<RollingTorus>;
	//	�p�r: �ʏ���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<EnemyDefaultState> EnemyDefaultState::Instance(){
		static shared_ptr<EnemyDefaultState> instance;
		if (!instance){
			instance = shared_ptr<EnemyDefaultState>(new EnemyDefaultState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void EnemyDefaultState::Enter(const shared_ptr<Enemy>& Obj){
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void EnemyDefaultState::Execute(const shared_ptr<Enemy>& Obj){
		Obj->ExtMotion();
		Obj->ShellThrowMotion();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void EnemyDefaultState::Exit(const shared_ptr<Enemy>& Obj){
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	//	class EnemyShellHitState : public ObjState<RollingTorus>;
	//	�p�r: �C�e�����������Ƃ��̏���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<EnemyShellHitState> EnemyShellHitState::Instance(){
		static shared_ptr<EnemyShellHitState> instance;
		if (!instance){
			instance = shared_ptr<EnemyShellHitState>(new EnemyShellHitState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void EnemyShellHitState::Enter(const shared_ptr<Enemy>& Obj){
		Obj->ShellHitMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void EnemyShellHitState::Execute(const shared_ptr<Enemy>& Obj){
		Obj->ExtMotion();
		if (Obj->ShellHitMoveMotion()){
			//�����I���Ȃ�X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(EnemyDefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void EnemyShellHitState::Exit(const shared_ptr<Enemy>& Obj){
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public Enemy;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
		Enemy(StagePtr, StartPos),
		m_YRot(0),
		m_MaxRotationSpeed(20.0f),
		m_RotationSpeed(0.0f)
	{}
	RollingTorus::~RollingTorus(){}

	//������
	void RollingTorus::Create(){

		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCollision = AddComponent<CollisionSphere>();
		//�G�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�G���m�͏Փ˂��Ȃ��悤�ɂ��Ă���
		PtrCollision->SetExcludeCollisionGroup(Group);

		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_TORUS");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_TORUS");
		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.25f);
		//�W�����v�X�^�[�g
		PtrGravity->StartJump(Vector3(0, 4.0f, 0));

		Enemy::Create();
	}


	//�h���N���X�́A�K�����̉��z�֐�����������
	void RollingTorus::ExtMotionMethod(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_YRot += (m_RotationSpeed * ElapsedTime);
		if (m_RotationSpeed > 0){
			m_RotationSpeed -= 0.05f;
		}
		auto Ptr = GetComponent<Transform>();
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
	}

	void RollingTorus::ShellHitMotionMethod(){
		//�Փ˔���𓾂�
		auto PtrCollision = GetComponent<CollisionSphere>();
		//�q�b�g�����Ƃ��݂̂��̊֐��͌Ă΂��̂�
		//PtrCollision->GetHitObject()�ɂ͕K���l�������Ă��邪
		//�ꉞ�A�m�F���Ă���
		if (PtrCollision->GetHitObject()){
			auto ShellPtr = dynamic_pointer_cast<ShellBall>(PtrCollision->GetHitObject());
			if (ShellPtr){
				//���肪�C�e������
				//���肪�v���C���[�̉\��������̂ŁA�`�F�b�N����
				//�X�R�A�I�u�W�F�N�g�ɃC�x���g���o
				auto PtrScoreObject = GetStage()->GetSharedGameObject<ScoreObject>(L"ScoreObject");
				PostEvent(0, GetThis<Enemy>(), PtrScoreObject, L"EnemyHit");
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
			Pos *= 4.0f;
			Pos += Vector3(0, 4.0f, 0);

			//��]�J�n
			m_RotationSpeed = m_MaxRotationSpeed;
			//�Փ˂𑊎���܂߂Ȃ��ɂ���
			PtrCollision->ClearBothHitObject();
			//�Փ˂͖����ɂ��Ă���
			PtrCollision->SetUpdateActive(false);

			//�d�͂𓾂�
			auto PtrGravity = GetComponent<Gravity>();
			//�W�����v�X�^�[�g
			PtrGravity->StartJump(Pos);

		}
	}



	//--------------------------------------------------------------------------------------
	//	class ColoringOctahedron : public Enemy;
	//	�p�r: �F���ς��8�ʑ�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ColoringOctahedron::ColoringOctahedron(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
		Enemy(StagePtr, StartPos)
	{}
	ColoringOctahedron::~ColoringOctahedron(){}
	//������
	void ColoringOctahedron::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCollision = AddComponent<CollisionSphere>();
		//�G�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�G���m�͏Փ˂��Ȃ��悤�ɂ��Ă���
		PtrCollision->SetExcludeCollisionGroup(Group);

		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_OCTAHEDRON");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_OCTAHEDRON");
		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.25f);
		//�W�����v�X�^�[�g
		PtrGravity->StartJump(Vector3(0, 4.0f, 0));

		Enemy::Create();
	}

	//�h���N���X�́A�K�����̉��z�֐�����������
	void ColoringOctahedron::ExtMotionMethod(){
		//�d�͂��Ƃ肾��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() > 0){
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			auto PtrDraw = GetComponent<BasicPNTDraw>();
			auto Col = PtrDraw->GetDiffuse();
			Col.z += ElapsedTime;
			if (Col.z >= 1.0f){
				Col.z = 0;
			}
			Col.y -= ElapsedTime;
			if (Col.y <= 0.0f){
				Col.y = 1.0f;
			}

			PtrDraw->SetDiffuse(Col);
		}
	}

	void ColoringOctahedron::ShellHitMotionMethod(){
		//�Փ˔���𓾂�
		auto PtrCollision = GetComponent<CollisionSphere>();
		//�q�b�g�����Ƃ��݂̂��̊֐��͌Ă΂��̂�
		//PtrCollision->GetHitObject()�ɂ͕K���l�������Ă��邪
		//�ꉞ�A�m�F���Ă���
		if (PtrCollision->GetHitObject()){
			auto ShellPtr = dynamic_pointer_cast<ShellBall>(PtrCollision->GetHitObject());
			if (ShellPtr){
				//���肪�C�e������
				//���肪�v���C���[�̉\��������̂ŁA�`�F�b�N����
				//�X�R�A�I�u�W�F�N�g�ɃC�x���g���o
				auto PtrScoreObject = GetStage()->GetSharedGameObject<ScoreObject>(L"ScoreObject");
				PostEvent(0, GetThis<Enemy>(), PtrScoreObject, L"EnemyHit");
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
			Pos *= 4.0f;
			Pos += Vector3(0, 4.0f, 0);

			//�F�̏�����
			auto PtrDraw = GetComponent<BasicPNTDraw>();
			PtrDraw->SetDiffuse(Color4(1.0f,0,0,1.0f));

			//�Փ˂𑊎���܂߂Ȃ��ɂ���
			PtrCollision->ClearBothHitObject();
			//�Փ˂͖����ɂ��Ă���
			PtrCollision->SetUpdateActive(false);

			//�d�͂𓾂�
			auto PtrGravity = GetComponent<Gravity>();
			//�W�����v�X�^�[�g
			PtrGravity->StartJump(Pos);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class ScoreObject : public GameObject;
	//	�p�r: �X�R�A��\������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ScoreObject::ScoreObject(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr), m_Point(0), m_EnemyPoint(0)
	{}
	ScoreObject::~ScoreObject(){}
	//������
	void ScoreObject::Create(){
		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));
		//�C�x���g�}�V���̍\�z
		m_EventMachine = make_shared< EventMachine<ScoreObject>>(GetThis<ScoreObject>());
		//�C�x���g�L�[�ƃC�x���g�X�e�[�g�����ѕt����
		m_EventMachine->AddEventState(L"EnemyHit", TorusHitEvent::Instance());
		m_EventMachine->AddEventState(L"PlayerHit", PlayerHitEvent::Instance());

	}
	void ScoreObject::Update(){
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		wstring str(L"�|�C���g: ");
		str += Util::UintToWStr(m_Point);
		str += L"\n�G�|�C���g: ";
		str += Util::UintToWStr(m_EnemyPoint);
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		str += L"\n�C�e�O���[�v�̖C�e��: ";
		str += Util::UintToWStr(ShellVec.size());

		PtrString->SetText(str);
	}

	//�C�x���g�̃n���h��
	void ScoreObject::OnEvent(const shared_ptr<Event>& event){
		//�n���h���֐��Ăяo��
		//����ŃC�x���g���U�蕪������
		m_EventMachine->HandleEvent(event);
	}
	//�|�C���g�̉��Z
	void ScoreObject::AddPointMotion(size_t sz){
		m_Point += sz;
	}
	//�G�|�C���g�̉��Z
	void ScoreObject::AddEnemyPointMotion(size_t sz){
		m_EnemyPoint += sz;
	}




	//--------------------------------------------------------------------------------------
	//	class TorusHitEvent : public EventState<ScoreObject>;
	//	�p�r: �g�[���X���C�e�ɓ��������C�x���g
	//--------------------------------------------------------------------------------------
	//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
	shared_ptr<TorusHitEvent> TorusHitEvent::Instance(){
		static shared_ptr<TorusHitEvent> instance;
		if (!instance){
			instance = shared_ptr<TorusHitEvent>(new TorusHitEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void TorusHitEvent::Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event){
		//�|�C���g���Z
		Obj->AddPointMotion(1);
	}

	//--------------------------------------------------------------------------------------
	//	class PlayerHitEvent : public EventState<ScoreObject>;
	//	�p�r: �v���C���[���C�e�ɓ��������C�x���g
	//--------------------------------------------------------------------------------------
	//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
	shared_ptr<PlayerHitEvent> PlayerHitEvent::Instance(){
		static shared_ptr<PlayerHitEvent> instance;
		if (!instance){
			instance = shared_ptr<PlayerHitEvent>(new PlayerHitEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void PlayerHitEvent::Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event){
		//�G�|�C���g���Z
		Obj->AddEnemyPointMotion(1);
	}





}
//endof  basedx11
