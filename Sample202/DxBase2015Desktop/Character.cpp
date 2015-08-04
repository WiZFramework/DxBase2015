#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class SeekCamera : public GameObject;
	//	�p�r: �ǂ�������J�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekCamera::SeekCamera(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SeekCamera::~SeekCamera(){}

	//������
	void SeekCamera::Create(){
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		PtrSeek->SetWeight(5.0f);
		PtrSeek->SetUpdateActive(false);

		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		PtrArrive->SetWeight(5.0f);
		PtrArrive->SetUpdateActive(false);


		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		//�g�p���Ă���̂�Camera�ł���
		auto PtrCamera = dynamic_pointer_cast<Camera>(GetStage()->GetCamera(0));
		//LookAtCamera�ł���
		auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (!PtrLookAtCamera && PtrCamera){
			//Camera���g�p���Ă�Ȃ�I�u�W�F�N�g�J�����ɐݒ�
			PtrCamera->SetCameraObject(GetThis<SeekCamera>());
		}
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SeekCamera> >(GetThis<SeekCamera>());
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->SetCurrentState(SeekCameraFarState::Instance());
		//���������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<SeekCamera>());
	}

	void SeekCamera::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}



	//���[�V��������������֐��Q

	//�J�����̌������ʒu�𓾂�
	Vector3 SeekCamera::GetCameraTargetMotion(bool IsFar){
		Vector3 CameraTarget(0, 1.25f, -4.0f);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			CameraTarget = PtrPlayer->GetComponent<Transform>()->GetPosition();
			CameraTarget.y += 1.0f;

			auto Rot = PtrPlayer->GetComponent<Transform>()->GetRotation();
			Vector3 RotVec(sin(Rot.y), 0, cos(Rot.y));
			RotVec.Normalize();

			if (IsFar){
				RotVec *= -1.0f;
			}
			else{
				RotVec *= -4.0f;
			}
			CameraTarget += RotVec;

			//�g�p���Ă���̂�Camera�ł���
			auto PtrCamera = dynamic_pointer_cast<Camera>(GetStage()->GetCamera(0));
			//LookAtCamera�ł���
			auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
			if (!PtrLookAtCamera && PtrCamera){
				//Camera���g�p���Ă�Ȃ�J�����̒��ړ_���v���C���[�ɐݒ�
				PtrCamera->SetAt(PtrPlayer->GetComponent<Transform>()->GetPosition());
			}
		}
		return CameraTarget;
	}

	//�v���C���[�Ƃ̋����𓾂�
	float SeekCamera::GetToPlayerLemngth(){
		Vector3 PlayerPos(0, 0.25, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		}
		auto LenVec = PlayerPos - GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(LenVec);
	}



	void  SeekCamera::SeekStartMoton(){
		auto Target = GetCameraTargetMotion(true);
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Target);

	}
	bool  SeekCamera::SeekUpdateMoton(){
		auto Target = GetCameraTargetMotion(true);
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(Target);
		if (GetToPlayerLemngth() <= 5.0f){
			return true;
		}
		return false;
	}
	void  SeekCamera::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekCamera::ArriveStartMoton(){
		auto Target = GetCameraTargetMotion(false);
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Target);
	}
	bool  SeekCamera::ArriveUpdateMoton(){
		auto Target = GetCameraTargetMotion(false);
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Target);
		if (GetToPlayerLemngth() > 5.0f){
			return true;
		}
		return false;
	}
	void  SeekCamera::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
	}



	//--------------------------------------------------------------------------------------
	//	class SeekCameraFarState : public ObjState<SeekCamera>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<SeekCameraFarState> SeekCameraFarState::Instance(){
		static shared_ptr<SeekCameraFarState> instance(new SeekCameraFarState);
		return instance;
	}
	void SeekCameraFarState::Enter(const shared_ptr<SeekCamera>& Obj){
		Obj->SeekStartMoton();
	}
	void SeekCameraFarState::Execute(const shared_ptr<SeekCamera>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(SeekCameraNearState::Instance());
		}
	}
	void SeekCameraFarState::Exit(const shared_ptr<SeekCamera>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class SeekCameraNearState : public ObjState<SeekCamera>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<SeekCameraNearState> SeekCameraNearState::Instance(){
		static shared_ptr<SeekCameraNearState> instance(new SeekCameraNearState);
		return instance;
	}
	void SeekCameraNearState::Enter(const shared_ptr<SeekCamera>& Obj){
		Obj->ArriveStartMoton();
	}
	void SeekCameraNearState::Execute(const shared_ptr<SeekCamera>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(SeekCameraFarState::Instance());
		}
	}
	void SeekCameraNearState::Exit(const shared_ptr<SeekCamera>& Obj){
		Obj->ArriveEndMoton();
	}




	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject(){}

	//������
	void SeekObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->SetCurrentState(FarState::Instance());
		//���������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<SeekObject>());
	}

	//���[�e�B���e�B�֐��Q
	Vector3 SeekObject::GetPlayerPosition() const{
		//�����v���C���[��������������ĂȂ��ꍇ�ɂ́AVector3(0,m_BaseY,0)��Ԃ�
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const{
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}

	//���[�V��������������֐��Q
	void  SeekObject::SeekStartMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(GetPlayerPosition());

	}
	bool  SeekObject::SeekUpdateMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() <= m_StateChangeSize){
			return true;
		}
		return false;
	}
	void  SeekObject::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekObject::ArriveStartMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(GetPlayerPosition());
	}
	bool  SeekObject::ArriveUpdateMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() > m_StateChangeSize){
			//�v���C���[�Ƃ̋��������ȏ�Ȃ�true
			return true;
		}
		return false;
	}
	void  SeekObject::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		//Arrive�R���|�[�l���g�𖳌��ɂ���
		PtrArrive->SetUpdateActive(false);
	}

	//����
	void SeekObject::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}
	void SeekObject::Update3(){
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f){
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
		//���y��m_BaseY
		auto Pos = PtrTransform->GetPosition();
		Pos.y = m_BaseY;
		PtrTransform->SetPosition(Pos);
	}
	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance(){
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->SeekStartMoton();
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance(){
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveStartMoton();
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveEndMoton();
	}




	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//������
	void FixedBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//��������������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox(){}

	//������
	void MoveBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);


		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}



	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	�p�r: ��Q���J�v�Z��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	CapsuleObject::~CapsuleObject(){}

	//������
	void CapsuleObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Capsule�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionCapsule>();
		CollPtr->SetFixed(true);
		//�R���W�����̃��C�A�t���[���\��
		CollPtr->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}


	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject(){}
	//������
	void SphereObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphere�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//�R���W�����̃��C�A�t���[���\��
		CollPtr->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");
	}


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	�p�r: ���ƏՓ˔��肷��X�N�G�A
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HitTestSquare::HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos)
	{}
	HitTestSquare::~HitTestSquare(){}

	void HitTestSquare::Create(){
		auto TrancePtr = GetComponent<Transform>();
		TrancePtr->SetScale(m_StartScale);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(m_StartRotation);
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(m_StartPos);
		auto DrawComp = AddComponent<BasicPNTDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SQUARE");


		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<HitTestSquare>());

	}

	PLANE HitTestSquare::GetPLANE() const{
		auto TrasnsPtr = GetComponent<Transform>();
		//�\�ʏ��3�̓_���g����PLANE���쐬
		//1�ڂ̓_�͒��S
		Vector3 Point0 = TrasnsPtr->GetPosition();
		//2�ڂ�-0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point1(-0.5f, -0.5f, 0);
		Point1.Transform(TrasnsPtr->GetWorldMatrix());
		//3�ڂ�0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point2(0.5f, -0.5f, 0);
		Point2.Transform(TrasnsPtr->GetWorldMatrix());
		//3�_���g���Ėʂ��쐬
		PLANE ret(Point0, Point1, Point2);
		return ret;
	}

	COLRECT HitTestSquare::GetCOLRECT() const{
		auto TrasnsPtr = GetComponent<Transform>();
		COLRECT rect(1.0f, 1.0f, TrasnsPtr->GetWorldMatrix());
		return rect;
	}


	//�_�Ƃ̍ŋߐړ_��Ԃ�
	void HitTestSquare::ClosestPtPoint(const Vector3& Point, Vector3& Ret){
		COLRECT rect = GetCOLRECT();
		Vector3 d = Point - rect.m_Center;
		Ret = rect.m_Center;
		for (int i = 0; i < 2; i++){
			float dist = Vector3EX::Dot(d, rect.m_Rot[i]);
			if (dist > rect.m_UVec[i]){
				dist = rect.m_UVec[i];
			}
			if (dist < -rect.m_UVec[i]){
				dist = -rect.m_UVec[i];
			}
			Ret += rect.m_Rot[i] * dist;
		}
	}


	//�_�Ƃ̋�����Ԃ��i�߂�l���}�C�i�X�Ȃ痠���j
	float HitTestSquare::GetDistPointPlane(const Vector3& Point) const{
		PLANE pl = GetPLANE();
		return (Vector3EX::Dot(Point, pl.m_Normal) - pl.m_DotValue) / Vector3EX::Dot(pl.m_Normal, pl.m_Normal);
	}

	//�q�b�g�e�X�g�����čŋߐړ_�ƁASquare�̖@����Ԃ�
	bool HitTestSquare::HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal){
		PLANE pl = GetPLANE();
		Normal = pl.m_Normal;
		//�l�p�`�Ƃ̍ŋߐړ_�𓾂�
		ClosestPtPoint(Sp.m_Center, RetVec);
		//�ŋߐړ_�����a�ȉ��Ȃ�Փ˂��Ă���
		if (Vector3EX::Length(Sp.m_Center - RetVec) <= Sp.m_Radius){
			return true;
		}
		return false;
	}



}
//endof  basedx11
