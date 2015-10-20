/*!
@file Rigidbody.cpp
@brief Gravity,Rigidbody,Collision,SteeringComponent�Ƃ��̔h���N���X

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Gravity::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl{
		Vector3 m_Gravity;				//�d�͉����x
		Vector3 m_InvGravity;			//�ł������d�͉����x
		Vector3 m_GravityVelocity;		//�d�͉����x�ɂ�錻�݂̑��x
		float m_BaseY;	//�ŉ������n�_
		float m_RayUnderSize;	//���C�������ɑł����ޒ���
		weak_ptr<GameObject> m_SecondParent;	//��2�̐e
	public:
		Impl() :
			m_Gravity(0, -9.8f, 0),
			m_InvGravity(0,0,0),
			m_GravityVelocity(0, 0, 0),
			m_BaseY(0),
			m_RayUnderSize(1.25f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Gravity : public Component ;
	//	�p�r: �d��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Gravity::~Gravity(){}

	//�A�N�Z�T

	const Vector3& Gravity::GetGravity() const{
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const Vector3& gravity){
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravity(float x, float y, float z){
		pImpl->m_Gravity = Vector3(x, y, z);
	}

	const Vector3& Gravity::GetInvGravity() const{
		return pImpl->m_InvGravity;
	}
	void Gravity::SetInvGravity(const Vector3& InvGravity){
		pImpl->m_InvGravity = InvGravity;
	}
	void Gravity::SetInvGravity(float x, float y, float z){
		pImpl->m_InvGravity = Vector3(x, y, z);
	}

	void Gravity::SetInvGravityDefault(){
		pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
	}

	const Vector3& Gravity::GetGravityVelocity() const{
		return pImpl->m_GravityVelocity;
	}

	void Gravity::SetGravityVelocity(const Vector3& GravityVelocity){
		pImpl->m_GravityVelocity = GravityVelocity;
	}
	void Gravity::SetGravityVelocity(float x, float y, float z){
		pImpl->m_GravityVelocity = Vector3(x, y, z);
	}

	bool Gravity::IsGravityVelocityZero(){
		return (pImpl->m_GravityVelocity == Vector3(0, 0, 0));
	}
	void Gravity::SetGravityVelocityZero(){
		pImpl->m_GravityVelocity.Zero();
	}
	float Gravity::GetBaseY() const{
		return pImpl->m_BaseY;
	}
	void Gravity::SetBaseY(float y){
		pImpl->m_BaseY = y;
	}

	float Gravity::GetRayUnderSize() const{
		return pImpl->m_RayUnderSize;
	}
	void Gravity::SetRayUnderSize(float f){
		pImpl->m_RayUnderSize = f;
	}
	shared_ptr<GameObject> Gravity::GetOnObject() const{
		return GetGameObject()->GetComponent<Transform>()->GetParent();
	}
	void Gravity::SetOnObject(const shared_ptr<GameObject>& OnObject){
		if (IsGameObjectActive()){
			if (!OnObject){
				throw BaseException(
					L"�����ȃI�u�W�F�N�g��OnObject�ɐݒ�ł��܂���B",
					L"if (!OnObject)",
					L"Gravity::SetOnObject()"
					);
			}
			GetGameObject()->GetComponent<Transform>()->SetParent(OnObject);
		}
	}

	void Gravity::ClearOnObject(){
		if (IsGameObjectActive() && GetOnObject()){
			GetGameObject()->GetComponent<Transform>()->ClearParent();
		}
	}

	void Gravity::CheckBaseY(){
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				Vector3 Pos = PtrT->GetPosition();
				if (pImpl->m_Gravity.y <= 0){
					if (Pos.y <= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
					}
				}
				else{
					if (Pos.y >= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
					}
				}
				//�ʒu��ݒ�
				PtrT->SetPosition(Pos);
			}
		}
	}

	void Gravity::StartJump(const Vector3& StartVec,float EscapeSpan){
		//����0
		SetInvGravity(0, 0, 0);
		SetGravityVelocity(StartVec);
		//�W�����v���Đe�I�u�W�F�N�g�{�����[�����炾�o�ł��Ȃ��Ƃ��Ή�
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		Vector3 EscapeVec = StartVec;
		EscapeVec *= EscapeSpan;
		Pos += EscapeVec;
		PtrT->SetPosition(Pos);
	}
	void Gravity::StartJump(float x, float y, float z,float EscapeSpan){
		StartJump(Vector3(x, y, z), EscapeSpan);
	}


	//����
	void Gravity::Update(){
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				Vector3 Pos = PtrT->GetPosition();
				//���R���������x���v�Z
				pImpl->m_GravityVelocity += pImpl->m_Gravity * ElapsedTime;
				//�ŏ����̉����x
				pImpl->m_GravityVelocity += pImpl->m_InvGravity * ElapsedTime;
				Pos += pImpl->m_GravityVelocity * ElapsedTime;
				if (pImpl->m_Gravity.y <= 0){
					if (Pos.y <= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						SetInvGravityDefault();
					}
				}
				else{
					if (Pos.y >= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						SetInvGravityDefault();
					}
				}
				//�ʒu��ݒ�
				PtrT->SetPosition(Pos);
			}
		}
	}


	//�q�b�g�����I�u�W�F�N�g�𒲂ׁA���ꂪ�����ɂ�������OnObject�ɐݒ肷��i���̏�ɏ��j
	void Gravity::HitObjectToOnObject(){
		auto PtrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (!PtrCollision){
			//Collision���Ȃ��̂ŏ����͍s��Ȃ�
			return;
		}
		//�e�R���|�[�l���g�̎擾
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//�q�b�g�����I�u�W�F�N�g���擾
		auto HitGameObjectPtr = PtrCollision->GetHitObject();
		if (HitGameObjectPtr){
			if (CheckOnObject(HitGameObjectPtr)){
				if (HitGameObjectPtr != GetOnObject()){
					if (GetOnObject()){
						ClearOnObject();
					}
					SetOnObject(HitGameObjectPtr);
					Vector3 InvGravity(GetGravity() * 1.0f);
					auto ParMat = GetOnObject()->GetComponent<Transform>()->GetWorldMatrix();
					Quaternion Qt = ParMat.QtInMatrix();
					ParMat.Identity();
					ParMat.RotationQuaternion(Qt);
					Vector4 v;
					ParMat.Inverse(&v);
					InvGravity.Transform(ParMat);
					InvGravity.y = -GetGravity().y;
					SetInvGravity(InvGravity);
					SetGravityVelocityZero();
				}
				//�q�b�g�I�u�W�F�N�g�̓N���A
				PtrCollision->SetHitObject(nullptr);
			}
			else{
				ClearOnObject();
				SetInvGravity(0, 0, 0);
			}
		}
		else{
			if (!CheckOnObject(GetOnObject())){
				ClearOnObject();
				SetInvGravity(0, 0, 0);
			}
			else{
				//�p�����ď���Ă���
				Vector3 InvGravity(GetGravity() * 1.0f);
				auto ParMat = GetOnObject()->GetComponent<Transform>()->GetWorldMatrix();
				Quaternion Qt = ParMat.QtInMatrix();
				ParMat.Identity();
				ParMat.RotationQuaternion(Qt);
				Vector4 v;
				ParMat.Inverse(&v);
				InvGravity.Transform(ParMat);
				InvGravity.y = -GetGravity().y;
				SetInvGravity(InvGravity);
				if (InvGravity.x == 0 && InvGravity.z == 0){
					SetGravityVelocityZero();
				}
			}
		}
	}




	//on�I�u�W�F�N�g�����݂��L�����ǂ������`�F�b�N����
	bool Gravity::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		if (!OnObject){
			return false;
		}
		//Collision�ɏ�����C����
		auto PtrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (PtrCollision){
			return PtrCollision->CheckOnObjectBase(OnObject);
		}
		return false;
	}

	//Gravity��Update2�̓I�u�W�F�N�g�ɏ�鏈�����s��
	void Gravity::Update2(){
		if (IsGameObjectActive()){
			//�q�b�g���Ă���I�u�W�F�N�g�������
			//���ꂪ���ɂ���OBB�Ȃ�onObject�ɐݒ�
			HitObjectToOnObject();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SteeringComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct SteeringComponent::Impl{
		float m_Weight;
		Impl() :
			m_Weight(1.0f)
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class SteeringComponent : public Component ;
	//	�p�r: ���ǃR���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	SteeringComponent::SteeringComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	SteeringComponent::~SteeringComponent(){}

	float SteeringComponent::GetWeight() const{ return pImpl->m_Weight; }
	void SteeringComponent::SetWeight(float f){ pImpl->m_Weight = f; }




	//--------------------------------------------------------------------------------------
	//	struct SeekSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct SeekSteering::Impl{
		Vector3 m_TargetPosition;
		Impl() :
			m_TargetPosition(0, 0, 0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SeekSteering : public SteeringComponent;
	//	�p�r: Seek����
	//--------------------------------------------------------------------------------------
	SeekSteering::SeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				//�����I�u�W�F�N�g������΂��̈ʒu�ɏ�����
				pImpl->m_TargetPosition = Vector3(0,0,0);
//				pImpl->m_TargetPosition = PtrT->GetPosition();
			}
		}
	}
	SeekSteering::~SeekSteering(){}

	//�A�N�Z�T

	const Vector3& SeekSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void SeekSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void SeekSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x,y,z);
	}
	//����
	void SeekSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Seek(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
					TransPtr->GetPosition(), RigidPtr->GetMaxSpeed()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ArriveSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ArriveSteering::Impl{
		Vector3 m_TargetPosition;
		float m_Decl;
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_Decl(3.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ArriveSteering : public SteeringComponent;
	//	�p�r: Arrive����
	//--------------------------------------------------------------------------------------
	ArriveSteering::ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ArriveSteering::~ArriveSteering(){}

	//�A�N�Z�T
	const Vector3& ArriveSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void ArriveSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void ArriveSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	float ArriveSteering::GetDecl() const{ return pImpl->m_Decl; }
	void ArriveSteering::SetDecl(float f){ pImpl->m_Decl = f; }


	//����
	void ArriveSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Arrive(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
					TransPtr->GetPosition(), RigidPtr->GetMaxSpeed(), pImpl->m_Decl) *  GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PursuitSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct PursuitSteering::Impl{
		Vector3 m_TargetPosition;	//�ڕW�̈ʒu
		Quaternion m_TargetQuaternion;		//�ڕW�̌���
		Vector3 m_TargetVelocity;	//�ڕW�̑��x
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_TargetQuaternion(),
			m_TargetVelocity(0, 0, 0)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class PursuitSteering : public SteeringComponent;
	//	�p�r: Pursuit����
	//--------------------------------------------------------------------------------------
	PursuitSteering::PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	PursuitSteering::~PursuitSteering(){}

	//�A�N�Z�T
	const Vector3& PursuitSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void PursuitSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void PursuitSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	const Quaternion& PursuitSteering::GetTargetQuaternion() const{
		return pImpl->m_TargetQuaternion;
	}
	void PursuitSteering::SetTargetQuaternion(const Quaternion& Qt){
		pImpl->m_TargetQuaternion = Qt;
	}
	void PursuitSteering::SetTargetRotation(const Vector3& rotation){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(rotation);
	}
	void PursuitSteering::SetTargetRotation(float x, float y, float z){
		SetTargetRotation(Vector3(x, y, z));
	}
	const Vector3& PursuitSteering::GetTargetVelocity() const{
		return pImpl->m_TargetVelocity;
	}
	void PursuitSteering::SetTargetVelocity(const Vector3& velocity){
		pImpl->m_TargetVelocity = velocity;
	}
	void PursuitSteering::SetTargetVelocity(float x, float y, float z){
		SetTargetVelocity(Vector3(x, y, z));
	}

	//����
	void PursuitSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Pursuit(RigidPtr->GetVelocity(), TransPtr->GetPosition(), TransPtr->GetRotation(), RigidPtr->GetMaxSpeed(),
					pImpl->m_TargetVelocity, pImpl->m_TargetPosition, pImpl->m_TargetQuaternion.GetRotation()) *  GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WanderSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct WanderSteering::Impl{
		Vector3 m_WanderTarget;	//�p�j�ڕW�̉�]�W���i���ǂɂ���ď�����������j
		float m_WanderRadius;	//�p�j���a
		float m_WanderDistance;	//�p�j�~�܂ł̋���
		float m_WanderJitter;	//�����_���ψق̍ő�l
		Impl() :
			m_WanderTarget(0,0,0),
			m_WanderRadius(1.5f),
			m_WanderDistance(1.0f),
			m_WanderJitter(0.5f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class WanderSteering : public SteeringComponent;
	//	�p�r: Wander����
	//--------------------------------------------------------------------------------------
	WanderSteering::WanderSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WanderSteering::~WanderSteering(){}
	//�A�N�Z�T
	const Vector3& WanderSteering::GetWanderTarget() const{
		return pImpl->m_WanderTarget;
	}
	void WanderSteering::SetWanderTarget(const Vector3& target){
		pImpl->m_WanderTarget = target;
	}
	float WanderSteering::GetWanderRadius() const{
		return pImpl->m_WanderRadius;
	}
	void WanderSteering::SetWanderRadius(float f){
		pImpl->m_WanderRadius = f;
	}
	float WanderSteering::GetWanderDistance() const{
		return pImpl->m_WanderDistance;
	}
	void WanderSteering::SetWanderDistance(float f){
		pImpl->m_WanderDistance = f;
	}
	float WanderSteering::GetWanderJitter() const{
		return pImpl->m_WanderJitter;
	}
	void WanderSteering::SetWanderJitter(float f){
		pImpl->m_WanderJitter = f;
	}

	void WanderSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Wander(TransPtr->GetWorldMatrix(),
					pImpl->m_WanderRadius, pImpl->m_WanderDistance, pImpl->m_WanderJitter, pImpl->m_WanderTarget) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WallAvoidanceSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct WallAvoidanceSteering::Impl{
		vector<PLANE> m_PlaneVec;//������ׂ��ʂ̔z��
		bool m_WallArrived;	//�ǂƏՓ˂�����
		Impl():
			m_WallArrived(false)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class WallAvoidanceSteering : public SteeringComponent;
	//	�p�r: WallAvoidance����(�ǉ��)
	//--------------------------------------------------------------------------------------
	WallAvoidanceSteering::WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WallAvoidanceSteering::~WallAvoidanceSteering(){}

	//�A�N�Z�T
	vector<PLANE>& WallAvoidanceSteering::GetPlaneVec() const{
		return pImpl->m_PlaneVec;
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<PLANE>& planevec){
		//�K���N���A����
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec){
			pImpl->m_PlaneVec.push_back(v);
		}
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<Plane>& planevec){
		//�K���N���A����
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec){
			PLANE p(v);
			pImpl->m_PlaneVec.push_back(p);
		}
	}


	//�ǂƏՓ˂��Ă��邩
	bool WallAvoidanceSteering::IsWallArribed() const{
		return pImpl->m_WallArrived;
	}


	//����
	void WallAvoidanceSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			pImpl->m_WallArrived = false;
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0,0,0);
				WorkForce = Steering::WallAvoidance(TransPtr->GetWorldMatrix(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_PlaneVec) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
				if (WorkForce.Length() > 0.0f){
					//�ǂɏՓ˂��Ă���
					pImpl->m_WallArrived = true;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ObstacleAvoidanceSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ObstacleAvoidanceSteering::Impl{
		vector<SPHERE> m_ObstacleSphereVec;		//������ׂ���Q����SPHERE�z��
		float m_RoadWidth;
		float m_RoadHeight;
		Impl():
			m_RoadWidth(0.5f),
			m_RoadHeight(0.5f)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class ObstacleAvoidanceSteering : public SteeringComponent;
	//	�p�r: ObstacleAvoidance����(��Q�����)
	//--------------------------------------------------------------------------------------
	ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering(){}
	//�A�N�Z�T
	//��Q���̔z��
	const vector<SPHERE>& ObstacleAvoidanceSteering::GetObstacleSphereVec() const{
		return pImpl->m_ObstacleSphereVec;
	}
	void ObstacleAvoidanceSteering::SetObstacleSphereVec(const vector<SPHERE>& spherevec){
		pImpl->m_ObstacleSphereVec.clear();
		for (auto sp : spherevec){
			pImpl->m_ObstacleSphereVec.push_back(sp);
		}
	}

	//���̍���
	float ObstacleAvoidanceSteering::GetRoadWidth() const{
		return pImpl->m_RoadWidth;
	}
	void ObstacleAvoidanceSteering::SetRoadWidth(float f){
		pImpl->m_RoadWidth = f;
	}
	//����
	float ObstacleAvoidanceSteering::GetRoadHeight() const{
		return pImpl->m_RoadHeight;
	}
	void ObstacleAvoidanceSteering::SetRoadHeight(float f){
		pImpl->m_RoadHeight = f;
	}


	void ObstacleAvoidanceSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0, 0, 0);
				WorkForce = Steering::ObstacleAvoidance(TransPtr->GetWorldMatrix(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(),pImpl->m_RoadWidth,pImpl->m_RoadHeight,
					pImpl->m_ObstacleSphereVec) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct FollowPathSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct FollowPathSteering::Impl{
		Path m_Path;	//�o�H������킷�p�X
		float m_Decl;	//�����l
		float m_WaypointSpan;	//�o�H�̒��S����̋���
		Impl():
			m_Decl(3.0f),
			m_WaypointSpan(2.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class FollowPathSteering : public SteeringComponent;
	//	�p�r: FollowPath����(�o�H�Ǐ])
	//--------------------------------------------------------------------------------------
	FollowPathSteering::FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	FollowPathSteering::~FollowPathSteering(){}

	//�A�N�Z�T
	void FollowPathSteering::SetPathList(const list<Vector3>& pathlist){
		pImpl->m_Path.SetList(pathlist);
	}

	float FollowPathSteering::GetDecl() const{
		return pImpl->m_Decl;
	}
	void FollowPathSteering::SetDecl(float f){
		pImpl->m_Decl = f;
	}

	float FollowPathSteering::GetWaypointSpan() const{
		return pImpl->m_WaypointSpan;
	}
	void FollowPathSteering::SetWaypointSpan(float f){
		pImpl->m_WaypointSpan = f;
	}

	bool FollowPathSteering::GetLooped() const{
		return pImpl->m_Path.GetLooped();
	}
	bool FollowPathSteering::IsLooped() const{
		return pImpl->m_Path.GetLooped();
	}
	void FollowPathSteering::SetLooped(bool b){
		pImpl->m_Path.SetLooped(b);
	}

	bool FollowPathSteering::IsFinished() const{
		return pImpl->m_Path.IsFinished();
	}

	//����
	void FollowPathSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0, 0, 0);
				WorkForce = Steering::FollowPath(pImpl->m_Path,
					pImpl->m_WaypointSpan,
					TransPtr->GetPosition(),
					RigidPtr->GetVelocity(),
					RigidPtr->GetMaxSpeed(),
					pImpl->m_Decl) 
					* GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}

	}

	//--------------------------------------------------------------------------------------
	//	struct AlignmentSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct AlignmentSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class AlignmentSteering : public SteeringComponent;
	//	�p�r: Alignment����(����)
	//--------------------------------------------------------------------------------------
	AlignmentSteering::AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	AlignmentSteering::~AlignmentSteering(){}

	//�A�N�Z�T
	shared_ptr<GameObjectGroup> AlignmentSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
				L"AlignmentSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void AlignmentSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}
	//����
	void AlignmentSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Alignment(GetGameObjectGroup(), GetGameObject()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct CohesionSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CohesionSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class CohesionSteering : public SteeringComponent;
	//	�p�r: Cohesion����(����)
	//--------------------------------------------------------------------------------------
	CohesionSteering::CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	CohesionSteering::~CohesionSteering(){}

	//�A�N�Z�T
	shared_ptr<GameObjectGroup> CohesionSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
				L"CohesionSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void CohesionSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}

	//����
	void CohesionSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Cohesion(GetGameObjectGroup(), GetGameObject(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SeparationSteering::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct SeparationSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SeparationSteering : public SteeringComponent;
	//	�p�r: Separation����
	//--------------------------------------------------------------------------------------
	SeparationSteering::SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	SeparationSteering::~SeparationSteering(){}
	//�A�N�Z�T
	shared_ptr<GameObjectGroup> SeparationSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"�O���[�v�͖����ł�",
				L"�O���[�v�̑��݂��m�F���ĉ�����",
				L"SeparationSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void SeparationSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}

	void SeparationSteering::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Separation(GetGameObjectGroup(), GetGameObject()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct Rigidbody::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Rigidbody::Impl{
		Vector3 m_Velocity;				//���x
		bool m_VelocityZeroCommand;		//Trans�t�H�[�����ω����Ȃ��Ƃ��ɁA���x��0�ɂ��邩�ǂ���
		Vector3 m_HitVelocity;		//�Փ˂����u�Ԃ̑��x
		float m_MaxSpeed;				//�ō����x
		Vector3 m_MinVelocity;			//�Œᑬ�x�iXYZ���w��ł���j
		Vector3 m_AngularVelocity;		//��]

		float m_Mass;					//���ʁi�L���O�����j
		float m_Reflection;						//�����W��
		Vector3 m_Force;				//���݂̃t�H�[�X�i�q�R���|�[�l���g�ŕύX�����j
		float m_Friction;				//���C�W���iForce�̋t�����ɓ����K��0����-1.0�̊ԁj
		float m_MaxForce;				//�ō��t�H�[�X
		Impl() :
			m_Velocity(0, 0, 0),
			m_VelocityZeroCommand(false),
			m_HitVelocity(0,0,0),
			m_MaxSpeed(10.0f),
			m_MinVelocity(0.01f,0.01f,0.01f),
			m_AngularVelocity(0, 0, 0),
			m_Mass(1.0f),
			m_Reflection(1.0f),
			m_Force(0, 0, 0),
			m_Friction(0),
			m_MaxForce(30.0f)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	�p�r: �����ʒu�R���g���[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Rigidbody::Rigidbody(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Rigidbody::~Rigidbody(){}

	//�A�N�Z�T
	
	bool Rigidbody::IsVelocityZeroCommand() const{
		return pImpl->m_VelocityZeroCommand;
	}
	
	void Rigidbody::SetVelocityZeroCommand(bool b){
		pImpl->m_VelocityZeroCommand = b;
	}


	const Vector3& Rigidbody::GetVelocity() const{ return pImpl->m_Velocity; }
	void Rigidbody::SetVelocity(const Vector3& Velocity){ 
		pImpl->m_Velocity = Velocity; 
		pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		if (pImpl->m_Velocity.Length() < pImpl->m_MinVelocity.Length()){
			pImpl->m_Velocity.Zero();
		}
	}
	void Rigidbody::SetVelocity(float x, float y, float z){ 
		SetVelocity(Vector3(x, y, z));
	}

	const Vector3& Rigidbody::GetHitVelocity() const{ return pImpl->m_HitVelocity; }
	void Rigidbody::SetHitVelocity(const Vector3& Velocity){ pImpl->m_HitVelocity = Velocity; }
	void Rigidbody::SetHitVelocity(float x, float y, float z){ pImpl->m_HitVelocity = Vector3(x, y, z); }



	float Rigidbody::GetMaxSpeed() const{ return pImpl->m_MaxSpeed; }
	void Rigidbody::SetMaxSpeed(float f){ pImpl->m_MaxSpeed = f; }

	const Vector3& Rigidbody::GetMinVelocity() const{
		return pImpl->m_MinVelocity;
	}
	void Rigidbody::SetMinVelocity(const Vector3& Velocity){
		pImpl->m_MinVelocity = Velocity;
	}
	void Rigidbody::SetMinVelocity(float x, float y, float z){
		SetMinVelocity(Vector3(x,y,z));
	}


	const Vector3& Rigidbody::GetAngularVelocity() const{ return pImpl->m_AngularVelocity; }
	void Rigidbody::SetAngularVelocity(const Vector3& AngularVelocity){ pImpl->m_AngularVelocity = AngularVelocity; }
	void Rigidbody::SetAngularVelocity(float x, float y, float z){ pImpl->m_AngularVelocity = Vector3(x, y, z); }

	float Rigidbody::GetMass() const{ return pImpl->m_Mass; }
	void Rigidbody::SetMass(float f){ pImpl->m_Mass = f; }

	float Rigidbody::GetReflection() const{
		return pImpl->m_Reflection;
	}
	void Rigidbody::SetReflection(float f){
		pImpl->m_Reflection = f;
	}


	const Vector3& Rigidbody::GetForce() const{ return pImpl->m_Force; }
	void Rigidbody::SetForce(const Vector3& Force){ pImpl->m_Force = Force; }
	void Rigidbody::SetForce(float x, float y, float z){ pImpl->m_Force = Vector3(x, y, z); }

	float Rigidbody::GetFriction() const{
		return pImpl->m_Friction;
	}
	void Rigidbody::SetFriction(float f){
		if (f >= 0){
			pImpl->m_Friction = 0.0f;
		}
		else if (f <= -1.0f){
			pImpl->m_Friction = -1.0f;
		}
		else{
			pImpl->m_Friction = f;
		}
	}


	float Rigidbody::GetMaxForce() const{ return pImpl->m_MaxForce; }
	void Rigidbody::SetMaxForce(float f){ pImpl->m_MaxForce = f; }

	void Rigidbody::Update(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				//�O��̃^�[������̎���
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				//�t�H�[�X���ύX����Ă�����
				if (pImpl->m_Force.Length() > 0){
					//�t�H�[�X��0�ȏ�Ȃ�
					//���ʂ��v�Z���ĉ��������߂�
					Vector3 Accel = pImpl->m_Force / pImpl->m_Mass;
					pImpl->m_Velocity += Accel * ElapsedTime;
				}
				if (pImpl->m_Velocity.Length() >= pImpl->m_MaxSpeed){
					pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
				}
				//�����l�����߂�
				auto  Friction = pImpl->m_Mass * pImpl->m_Friction;
				if (Friction <= -1.0f){
					Friction = -1.0f;
				}
				else if (Friction >= 0.0f){
					Friction = 0.0f;
				}
				Vector3 VFriction = pImpl->m_Velocity * Friction;
				pImpl->m_Velocity += VFriction;
				if (abs(pImpl->m_Velocity.x) < pImpl->m_MinVelocity.x){
					pImpl->m_Velocity.x = 0;
				}
				if (abs(pImpl->m_Velocity.y) < pImpl->m_MinVelocity.y){
					pImpl->m_Velocity.y = 0;
				}
				if (abs(pImpl->m_Velocity.z) < pImpl->m_MinVelocity.z){
					pImpl->m_Velocity.z = 0;
				}
				Vector3 Pos = PtrT->GetPosition();
				Pos += pImpl->m_Velocity * ElapsedTime;
				PtrT->SetPosition(Pos);
				Quaternion Qt = PtrT->GetQuaternion();
				Quaternion QtSpan;
				QtSpan.RotationRollPitchYawFromVector(pImpl->m_AngularVelocity * ElapsedTime);
				Qt *= QtSpan;
				Qt.Normalize();
				PtrT->SetQuaternion(Qt);
				//���x���o�b�N�A�b�v����
				SetHitVelocity(pImpl->m_Velocity);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Collision::Impl{
		bool m_Fixed;		//�Î~�I�u�W�F�N�g���ǂ���
		bool m_SendEventActive;	//�Փ˂����Ƃ���Send�C�x���g�𔭐����邩�ǂ���
		bool m_PostEventActive;	//�Փ˂����Ƃ���Post�C�x���g�𔭐����邩�ǂ���
		float m_PostDispatchTime;		//Post�𔭍s����ꍇ�̒x������
		wstring m_EventString;	//�Փ˂����Ƃ��̃C�x���g�L�[
		bool m_AutoAfterCollision;	//�����㏈�����s�����ǂ���
		bool m_NewDestOnly;	//�V�����Փ˂̂݌㏈������
		weak_ptr<GameObject> m_BeforeHitObject;		//��O�ɏՓ˂�������
		weak_ptr<GameObject> m_HitObject;		//�Փ˂�������
		float m_HitTime;	//�q�b�g�����Ƃ��̑O�^�[������̌o�ߎ���
		weak_ptr<GameObject> m_SubDispatchObject;	//�Փ˂�������m�点��T�u�I�u�W�F�N�g
		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X

		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//���肩�珜�O����O���[�v



		float m_EscapeSpanMin;
		float m_EscapeAlignPlus;
		Impl() :
			m_Fixed(false),
			m_SendEventActive(false),
			m_PostEventActive(false),
			m_PostDispatchTime(0),
			m_EventString(L"CollisionEvent"),
			m_AutoAfterCollision(true),
			m_NewDestOnly(true),
			m_HitTime(0),
			m_EscapeSpanMin(0.15f),
			m_EscapeAlignPlus(1.2f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	�p�r: �Փ˔���R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Collision::Collision(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Collision::~Collision(){}

	//�A�N�Z�T
	shared_ptr<GameObject> Collision::GetHitObject() const{
		if (pImpl->m_HitObject.expired()){
			return nullptr;
		}
		return pImpl->m_HitObject.lock();
	}
	void Collision::SetHitObject(const shared_ptr<GameObject>& Ptr){
		pImpl->m_HitObject = Ptr;
	}

	void Collision::ClearBothHitObject(){
		auto Other = GetHitObject();
		if (Other){
			auto OtherColl = Other->GetComponent<Collision>(false);
			if (OtherColl){
				OtherColl->SetHitObject(nullptr);
			}
		}
		SetHitObject(nullptr);
	}


	void Collision::ResetHitObject(){
		pImpl->m_BeforeHitObject = pImpl->m_HitObject;
		pImpl->m_HitObject.reset();
	}

	shared_ptr<GameObject> Collision::GetSubDispatchObject() const{
		if (pImpl->m_SubDispatchObject.expired()){
			return nullptr;
		}
		return pImpl->m_SubDispatchObject.lock();
	}
	void Collision::SetSubDispatchObject(const shared_ptr<GameObject>& Ptr){
		pImpl->m_SubDispatchObject = Ptr;
	}


	bool Collision::IsNewDest() const{
		if (pImpl->m_HitObject.expired()){
			//�V�����q�b�g���Ȃ����false
			return false;
		}
		else{
			//�V�����q�b�g��������
			if (pImpl->m_BeforeHitObject.expired()){
				//��O���q�b�g���ĂȂ�
				return true;
			}
			if (pImpl->m_BeforeHitObject.lock() == pImpl->m_HitObject.lock()){
				//��O�Ɠ���
				return false;
			}
			else{
				//��O�ƈႤ
				return true;
			}
		}
	}


	bool Collision::NeedAfterCollision() const{
		if (pImpl->m_AutoAfterCollision){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}

	bool Collision::NeedSendEvent() const{
		if (pImpl->m_SendEventActive){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}

	bool Collision::NeedPostEvent() const{
		if (pImpl->m_PostEventActive){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}


	shared_ptr<MeshResource> Collision::GetMeshResource() const{
		if (pImpl->m_MeshResource.expired()){
			return nullptr;
		}
		return pImpl->m_MeshResource.lock();
	}
	void Collision::SetMeshResource(const shared_ptr<MeshResource>& Ptr){
		pImpl->m_MeshResource = Ptr;
	}

	bool Collision::GetFixed() const{
		return pImpl->m_Fixed;
	}
	bool Collision::IsFixed() const{
		return pImpl->m_Fixed;
	}
	void Collision::SetFixed(bool b){
		pImpl->m_Fixed = b;
	}

	bool Collision::GetSendEventActive() const{
		return pImpl->m_SendEventActive;
	}
	bool Collision::IsSendEventActive() const{
		return pImpl->m_SendEventActive;
	}
	void Collision::SetSendEventActive(bool b){
		pImpl->m_SendEventActive = b;
	}

	bool Collision::GetPostEventActive() const{
		return pImpl->m_PostEventActive;
	}
	bool Collision::IsPostEventActive() const{
		return pImpl->m_PostEventActive;
	}
	void Collision::SetPostEventActive(bool b){
		pImpl->m_PostEventActive = b;
	}

	float Collision::GetPostDispatchTime() const{
		return pImpl->m_PostDispatchTime;
	}
	void Collision::SetPostDispatchTime(float f){
		if (f <= 0.0f){
			f = 0.0f;
		}
		pImpl->m_PostDispatchTime = f;
	}
	wstring Collision::GetEventString() const{
		return pImpl->m_EventString;
	}
	void Collision::SetEventString(const wstring& str){
		if (str == L""){
			throw BaseException(
				L"�C�x���g�X�g�����O���󔒂ł�",
				L"��������m�F���ĉ�����",
				L"Collision::SetEventString()"
				);
		}
		pImpl->m_EventString = str;
	}


	bool Collision::GetAutoAfterCollision() const{
		return pImpl->m_AutoAfterCollision;
	}
	bool Collision::IsAutoAfterCollision() const{
		return pImpl->m_AutoAfterCollision;
	}
	void Collision::SetAutoAfterCollision(bool b){
		pImpl->m_AutoAfterCollision = b;
	}

	bool Collision::GetNewDestOnly() const{
		return pImpl->m_NewDestOnly;
	}
	bool Collision::IsNewDestOnly() const{
		return pImpl->m_NewDestOnly;
	}
	void Collision::SetNewDestOnly(bool b){
		pImpl->m_NewDestOnly = b;
	}

	//�����o�������̌W��
	float Collision::GetEscapeSpanMin() const{
		return pImpl->m_EscapeSpanMin;
	}
	void Collision::SetEscapeSpanMin(float f){
		pImpl->m_EscapeSpanMin = f;
	}

	float Collision::GetEscapeAlignPlus() const{
		return pImpl->m_EscapeAlignPlus;
	}
	void Collision::SetEscapeAlignPlus(float f){
		pImpl->m_EscapeAlignPlus = f;
	}

	float Collision::GetHitTime() const{
		return pImpl->m_HitTime;
	}
	void Collision::SetHitTime(float f){
		pImpl->m_HitTime = f;
	}


	//���肩�珜�O����I�u�W�F�N�g�O���[�v�̐ݒ�
	void Collision::SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_ExcludeCollisionGroup = Group;
	}

	//���O�O���[�v�ɑ����Ă��邩�𒲂ׂ�
	bool Collision::ChkExcludeCollisionObject(const shared_ptr<GameObject>&  CheckGameObject){
		//���O�O���[�v�����邩�ǂ���
		if (!pImpl->m_ExcludeCollisionGroup.expired()){
			auto Group = pImpl->m_ExcludeCollisionGroup.lock();
			//���O�O���[�v�̔z��𓾂�
			auto GroupVec = Group->GetGroupVector();
			for (auto ExcludePtr : GroupVec){
				if (!ExcludePtr.expired()){
					//�O���[�v�ɑ�����e�I�u�W�F�N�g�𓾂�
					auto ExcludeObj = ExcludePtr.lock();
					if (ExcludeObj != GetGameObject()){
						//���̃I�u�W�F�N�g���������g�ȊO
						if (ExcludeObj == CheckGameObject){
							return true;
						}
					}
				}
			}
		}
		return false;
	}


	bool Collision::CheckOnObjectBase(const shared_ptr<GameObject>& OnObject){
		//���z�֐��Ăяo��
		return CheckOnObject(OnObject);
	}


	//����
	void Collision::Update(){
		//Collision���L�����ǂ���
		if (!IsUpdateActive()){
			return;
		}
		//Fixed���ǂ���
		if (IsFixed()){
			return;
		}
		//���łɃq�b�g���Ă���
		if (GetHitObject()){
			return;
		}
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			auto GravityPtr = GetGameObject()->GetComponent<Gravity>(false);
			if (PtrT){
				//�O��̃^�[������̎���
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				auto ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();
				for (auto ObjPtr : ObjVec){
					if ((GetGameObject() != ObjPtr) && ObjPtr->IsUpdateActive()){
						if (GravityPtr && GravityPtr->GetOnObject() == ObjPtr){
							//����Ă�I�u�W�F�N�g�Ƃ͔��肵�Ȃ�
							continue;
						}
						if (ChkExcludeCollisionObject(ObjPtr)){
							//���O�I�u�W�F�N�g�Ƃ͂Ƃ͔��肵�Ȃ�
							continue;
						}
						//�����Collision���擾
						auto CollisionPtr = ObjPtr->GetComponent<Collision>(false);
						if (CollisionPtr){
							if (!CollisionPtr->IsUpdateActive()){
								//�����Collision������
								continue;
							}
							if (CollisionPtr->GetHitObject()){
								//�������łɑ��肪�q�b�g���Ă���
								continue;
							}
						}
						auto CollisionSpherePtr = ObjPtr->GetComponent<CollisionSphere>(false);
						auto CollisionCapsulePtr = ObjPtr->GetComponent<CollisionCapsule>(false);
						auto CollisionObbPtr = ObjPtr->GetComponent<CollisionObb>(false);
						if (CollisionSpherePtr){
							CollisionWithSphere(ObjPtr);
						}
						else if (CollisionCapsulePtr){
							CollisionWithCapsule(ObjPtr);
						}
						else if (CollisionObbPtr){
							CollisionWithObb(ObjPtr);
						}
					}
				}
				//����Hit���Ă���A�����HitObject�ɂ������ݒ�
				auto HitObject = GetHitObject();
				if (HitObject){
					auto OtherPtr = HitObject->GetComponent<Collision>();
					if (!OtherPtr->IsFixed()){
						OtherPtr->SetHitObject(GetGameObject());
						OtherPtr->SetHitTime(GetHitTime());
						HitObject->GetComponent<Transform>()->LerpBeforeToNow(GetHitTime());

					}
				}
			}
		}
	}

	//Rigidbody������ꍇ��Update2
	//�Փ˖@�������߂�B
	//�ŋߐړ_�����߂�
	void Collision::Update2WithRigidbody2(){
		if (IsGameObjectActive()){
			auto PtrSrcRigidbody = GetGameObject()->GetComponent<Rigidbody>();
			if (IsFixed() || !GetHitObject()){
				//�����Ȃ����A�Փ˂��ĂȂ���
				return;
			}
			Vector3 HitNormal;	//�Փ˂̖@��
			Vector3 ClosestPoint; //�ŋߐړ_
			GetNormalClosetPointWithHitObject(GetHitObject(), HitNormal, ClosestPoint);
			//�������v�Z
			auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
			if (PtrGrav){
				//�~�ς��������̉����x��0�ɂ���
				PtrGrav->SetGravityVelocityZero();
			}
			//��������
			auto DestGameObject = GetHitObject();
			//Dest����Transform��Rigidbody���擾
			auto PtrDestTrans = DestGameObject->GetComponent<Transform>();
			auto PtrDestRegid = DestGameObject->GetComponent<Rigidbody>();
			auto PtrDestColl = DestGameObject->GetComponent<Collision>();

			Vector3 Reflect = Vector3EX::Reflect(PtrSrcRigidbody->GetHitVelocity(), HitNormal);
			if (!PtrDestColl->IsFixed()){
				////���肪Fix�łȂ���΁A�͂𕪎U����
				Vector3 v1Temp = PtrSrcRigidbody->GetHitVelocity() * (PtrSrcRigidbody->GetMass() - PtrDestRegid->GetMass());
				Vector3 v2Temp = PtrDestRegid->GetHitVelocity() * 2;
				v2Temp *= PtrDestRegid->GetMass();
				Reflect = (v1Temp + v2Temp) / (PtrSrcRigidbody->GetMass() + PtrDestRegid->GetMass());
			}
			//������
			Reflect *= PtrSrcRigidbody->GetReflection();
			PtrSrcRigidbody->SetVelocity(Reflect);
			//////////////////////////////////////
			//�ǂ��o������
			EscapeFromDestObject(DestGameObject, HitNormal, ClosestPoint);

		}
	}
	//�����A�ǂ��o���Ȃǎ��㏈��
	void Collision::Update2(){
		auto PtrSrcGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (GetHitObject() && IsUpdate2Active()){
			//�Փ˂��Ă��Ă���IsUpdate2Active�Ȃ�
			shared_ptr<void> PtrVoid = static_pointer_cast<void>(GetHitObject());
			if (NeedSendEvent()){
				//Send�C�x���g���o��
				SendEvent(GetGameObject(), GetGameObject(), pImpl->m_EventString, PtrVoid);
				if (GetSubDispatchObject()){
					//�T�u�ő��M����I�u�W�F�N�g������Α��M
					SendEvent(GetGameObject(), GetSubDispatchObject(), pImpl->m_EventString, PtrVoid);
				}
			}
			if (NeedPostEvent()){
				//Post�C�x���g���o��
				PostEvent(pImpl->m_PostDispatchTime, GetGameObject(), GetGameObject(), pImpl->m_EventString, PtrVoid);
				if (GetSubDispatchObject()){
					//�T�u�ő��M����I�u�W�F�N�g������Α��M
					PostEvent(pImpl->m_PostDispatchTime, GetGameObject(), GetSubDispatchObject(), pImpl->m_EventString, PtrVoid);
				}
			}
			if (NeedAfterCollision()){
				//�����㏈��
				if (!IsFixed()){
					//Rigidbody������Ώ�����C����
					auto PtrSrcRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
					auto PtrDestRigidbody = GetHitObject()->GetComponent<Rigidbody>(false);
					if (PtrSrcRigidbody && PtrDestRigidbody){
						Update2WithRigidbody2();
					}
				}
			}
		}
		ResetHitObject();
		if (PtrSrcGravity){
			if (PtrSrcGravity->GetOnObject()){
				EscapeFromDestParent(PtrSrcGravity->GetOnObject());
			}
			PtrSrcGravity->CheckBaseY();
		}
	}



	void Collision::Draw(){

		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		auto PtrMeshResource = pImpl->m_MeshResource.lock();
		//�X�e�[�W����J���������o��
		auto PtrCamera = PtrStage->GetTargetCamera();

		if (PtrMeshResource && PtrCamera){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//�X�e�[�^�X�̃|�C���^
			auto RenderStatePtr = PtrStage->GetRenderState();
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

			//�J�����̎擾
			Matrix4X4 View, Proj;
			View = PtrCamera->GetViewMatrix();
			Proj = PtrCamera->GetProjMatrix();

			//�R���X�^���g�o�b�t�@�̐ݒ�
			SimpleConstantBuffer sb;
			sb.m_Model = Matrix4X4EX::Transpose(GetCollisionMatrix());
			sb.m_View = Matrix4X4EX::Transpose(View);
			sb.m_Projection = Matrix4X4EX::Transpose(Proj);
			sb.m_Direction = Vector4(0, -1.0f, 0, 0);

			sb.m_DiffuseColor = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//���_�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			//�T���v���[�͎g�p���Ȃ�
			pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
			pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
			//�e�N�X�`�����g�p���Ȃ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
			pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
			//�e�N�X�`���Ȃ��̃s�N�Z���V�F�[�_�̐ݒ�
			pID3D11DeviceContext->PSSetShader(PSSimplePNT::GetPtr()->GetShader(), nullptr, 0);
			//���_�V�F�[�_�̐ݒ�
			pID3D11DeviceContext->VSSetShader(VSSimplePNT::GetPtr()->GetShader(), nullptr, 0);

			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSSimplePNT::GetPtr()->GetInputLayout());

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�h��Ԃ�
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
			//�����_�����O�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
			//�`��
			pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			//��n��
			Dev->InitializeStates(RenderStatePtr);
		}

	}



	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl{
		float m_MakedDiameter;					//�쐬���̒��a
		Impl() :
			m_MakedDiameter(1.0f)
		{}
		~Impl(){}

	};


	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	�p�r: ���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionSphere::CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionSphere::~CollisionSphere(){}

	void CollisionSphere::Create(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE"));
		SetDrawActive(false);
	}

	//�A�N�Z�T
	float CollisionSphere::GetMakedDiameter() const{
		return pImpl->m_MakedDiameter;
	}
	void CollisionSphere::SetMakedDiameter(float f){
		pImpl->m_MakedDiameter = f;
	}
	float CollisionSphere::GetMakedRadius() const{
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionSphere::SetMakedRadius(float f){
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	SPHERE CollisionSphere::GetSphere() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		//���̃I�u�W�F�N�g��SPHERE���쐬
		SPHERE Ret(MatBase.PosInMatrix(), MatBase.ScaleInMatrix().x * 0.5f);
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const{
		SPHERE Ret = GetSphere();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Ret.m_Center = PtrT->GetBeforeWorldMatrix().PosInMatrix();
		return Ret;
	}

	Matrix4X4 CollisionSphere::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionSphere::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//�����CollisionSphere����������
		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforeSphere = DestCollisionSpherePtr->GetBeforeSphere();

		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestSphere, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
	}

	void CollisionSphere::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//�����CollisionCapsule����������
		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforeCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();

		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforeCapsule.GetCenter();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestCapsule, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}


	}


	void CollisionSphere::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//�����CollisionObb����������
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
	}

	void CollisionSphere::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		//����̌^���m�F
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//���肪����
			SPHERE SrcSphere = GetSphere();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			//Noamal�ɐݒ�
			Normal = SrcSphere.m_Center - DestSphere.m_Center;
			Normal.Normalize();
			//�ŋߐړ_�𓱂��o��
			auto ClosPoint = Normal * DestSphere.m_Radius;
			ClosestPoint = ClosPoint + DestSphere.m_Center;
		}
		else if (DestCollisionCapsulePtr){
			//�����CollisionObb����������
			SPHERE SrcSphere = GetSphere();
			CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
			//�ŋߐړ_�𓾂�
			HitTest::SPHERE_CAPSULE(SrcSphere, DestCapsule, ClosestPoint);
			Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
		}
		else if (DestCollisionObbPtr){
			//�����CollisionObb����������
			SPHERE SrcSphere = GetSphere();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//�ŋߐړ_�𓾂�
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
		}
		else{
			Normal = Vector3(0, 0, 0);
			ClosestPoint = Vector3(0, 0, 0);
		}
	}

	void CollisionSphere::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		SPHERE SrcSphere = GetSphere();
		//�ŋߐړ_����Normal�����ɔ��a�Ԃ�ړ�
		Vector3 EscV = Normal;
		EscV.Normalize();
		float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
		EscV *= EscF;
		SrcSphere.m_Center = ClosestPoint + EscV;
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		PtrTrans->SetPosition(SrcSphere.m_Center);
	}

	void CollisionSphere::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		auto DestCollisionObbPtr = ParentObject->GetComponent<CollisionObb>(false);
		if (DestCollisionObbPtr){
			//�����CollisionObb����������
			SPHERE SrcSphere = GetSphere();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//�ŋߐړ_�𓾂�
			Vector3 ClosestPoint;
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Vector3 Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.0001f, Normal);
			//�ŋߐړ_����Normal�����ɔ��a�Ԃ�ړ�
			float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
			Normal *= EscF;
			SrcSphere.m_Center = ClosestPoint + Normal;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcSphere.m_Center);
		}
	}

	//Gravity����Ă΂��
	bool CollisionSphere::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//���̃I�u�W�F�N�g��OBB�ȊO�͐ݒ�ł��Ȃ�
			return false;
		}
		//������Sphere
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();

		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y -= SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y -= SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;

			//��ɏ���Ă邩�ǂ���������
			//���C��ł�����ł݂�
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		else{
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y += SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y += SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;
			//���ɏ���Ă邩�ǂ���������
			//���C��ł�����ł݂�
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		return false;
	}



	//--------------------------------------------------------------------------------------
	//	struct CollisionCapsule::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionCapsule::Impl{
		float m_MakedDiameter;			//�쐬���̒��a
		float m_MakedHeight;			//�쐬������
		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	�p�r: �J�v�Z���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionCapsule::CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionCapsule::~CollisionCapsule(){}

	//������
	void CollisionCapsule::Create(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_CAPSULE"));
		SetDrawActive(false);
	}

	//�A�N�Z�T
	float CollisionCapsule::GetMakedDiameter() const{
		return pImpl->m_MakedDiameter;
	}
	void CollisionCapsule::SetMakedDiameter(float f){
		pImpl->m_MakedDiameter = f;
	}
	float CollisionCapsule::GetMakedRadius() const{
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionCapsule::SetMakedRadius(float f){
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	float CollisionCapsule::GetMakedHeight() const{
		return pImpl->m_MakedHeight;
	}
	void CollisionCapsule::SetMakedHeight(float f){
		pImpl->m_MakedHeight = f;
	}

	CAPSULE CollisionCapsule::GetCapsule() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		//���̃I�u�W�F�N�g��CAPSULE���쐬
		CAPSULE Ret(pImpl->m_MakedDiameter * 0.5f, Vector3(0, pImpl->m_MakedHeight * -0.5f, 0),
			Vector3(0, pImpl->m_MakedHeight * 0.5f, 0),
			PtrT->GetWorldMatrix());
		return Ret;
	}

	CAPSULE CollisionCapsule::GetBeforeCapsule() const{
		CAPSULE Ret = GetCapsule();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Ret.SetCenter(PtrT->GetBeforeWorldMatrix().PosInMatrix());
		return Ret;
	}
	Matrix4X4 CollisionCapsule::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedHeight, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionCapsule::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//�����CollisionSphere����������
		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforeSphere = DestCollisionSpherePtr->GetBeforeSphere();

		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		//Capsule��Src�Ȃ̂ŁASphere���𓮂��������ɂ���
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcCapsule, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}


	}
	void CollisionCapsule::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//�����CollisionCapsule����������
		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforeCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();

		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforeCapsule.GetCenter();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestCapsule, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}


	}
	void CollisionCapsule::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//�����CollisionCapsule����������
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();

		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}

	}

	void CollisionCapsule::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		//Src����Capsule
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//�����CollisionSphere����������
			CAPSULE SrcCapsle = GetCapsule();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			Vector3 RetVec;
			//�ŋߐړ_�𓾂�
			float t;
			HitTest::ClosetPtPointSegment(DestSphere.m_Center, SrcCapsle.m_PointA, SrcCapsle.m_PointB, t, ClosestPoint);
			Normal = SrcCapsle.GetCenter() - ClosestPoint;
			Normal.Normalize();
		}
		else if (DestCollisionCapsulePtr){
			//�����DestCollisionCapsulePtr����������
			CAPSULE SrcCapsle = GetCapsule();
			CAPSULE DestCapsle = DestCollisionCapsulePtr->GetCapsule();
			Vector3 RetVecSrc, RetVecDest;
			//�ŋߐړ_�𓾂�
			HitTest::CAPSULE_CAPSULE(SrcCapsle, DestCapsle, RetVecSrc, RetVecDest);
			//�Փ˖@���𓾂�
			Normal = RetVecSrc - RetVecDest;
			Normal.Normalize();
			//Dest���̍ŋߐړ_�𓾂�
			ClosestPoint = Normal;
			ClosestPoint *= DestCapsle.m_Radius;
			ClosestPoint += RetVecDest;
		}
		else if (DestCollisionObbPtr){
			//�����DestCollisionObbPtr����������
			CAPSULE SrcCapsle = GetCapsule();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			int flg;
			ClosestPoint = HitTest::ClosestPtCapsuleOBB(SrcCapsle, DestObb, flg);

			if (flg < 0){
				Normal = SrcCapsle.m_PointA - ClosestPoint;
				Normal.Normalize();
			}
			else if (flg > 0){
				Normal = SrcCapsle.m_PointB - ClosestPoint;
				Normal.Normalize();
			}
			else{
				float t;
				Vector3 RetVec;
				HitTest::ClosetPtPointSegment(ClosestPoint, SrcCapsle.m_PointB, SrcCapsle.m_PointA, t, RetVec);
				Normal = RetVec - ClosestPoint;
				Normal.Normalize();
			}
		}
		else{
			Normal = Vector3(0, 0, 0);
			ClosestPoint = Vector3(0, 0, 0);
		}
	}

	void CollisionCapsule::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		CAPSULE SrcCapsule = GetCapsule();
		//�ŋߐړ_����Normal������EscF�Ԃ�ړ�
		Vector3 EscV = Normal;
		EscV.Normalize();
		float EscF = SrcCapsule.m_Radius * GetEscapeSpanMin();
		EscV *= EscF;
		Vector3 Center = SrcCapsule.GetCenter();
		Center += EscV;
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		PtrTrans->SetPosition(Center);
	}

	void CollisionCapsule::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		auto DestCollisionObbPtr = ParentObject->GetComponent<CollisionObb>(false);
		if (DestCollisionObbPtr){
			//�����CollisionObb����������
			CAPSULE SrcCapsule = GetCapsule();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			int flg;
			HitTest::ClosestPtCapsuleOBB(SrcCapsule, DestObb, flg);
			if (flg >= 0){
				//�����̋��̂̂ݑΉ�
				return;
			}

			SPHERE SrcSphere;
			SrcSphere.m_Center = SrcCapsule.m_PointA;
			SrcSphere.m_Radius = SrcCapsule.m_Radius;
			//�ŋߐړ_�𓾂�
			Vector3 ClosestPoint;
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Vector3 Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.0001f, Normal);
			//�ŋߐړ_����Normal�����ɔ��a�Ԃ�ړ�
			float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
			Normal *= EscF;
			SrcSphere.m_Center = ClosestPoint + Normal;
			Vector3 Span = SrcSphere.m_Center - SrcCapsule.m_PointA;
			SrcCapsule.m_PointA = SrcSphere.m_Center;
			SrcCapsule.m_PointB += Span;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcCapsule.GetCenter());
		}
	}

	//Gravity����Ă΂��
	bool CollisionCapsule::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//���̃I�u�W�F�N�g��OBB�ȊO�͐ݒ�ł��Ȃ�
			return false;
		}
		//������Capsule
		CAPSULE SrcSCapsule = GetCapsule();
		//����Sphere�Ŕ���
		SPHERE SrcSphere;
		SrcSphere.m_Center = SrcSCapsule.m_PointA;
		SrcSphere.m_Radius = SrcSCapsule.m_Radius;
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();

		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y -= SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y -= SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;

			//��ɏ���Ă邩�ǂ���������
			//���C��ł�����ł݂�
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		else{
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y += SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y += SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;
			//���ɏ���Ă邩�ǂ���������
			//���C��ł�����ł݂�
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}


		return false;
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionObb::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionObb::Impl{
		float m_Size;					//�쐬���̃T�C�Y
		Impl() :
			m_Size(1.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	�p�r: Obb�Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionObb::CollisionObb(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionObb::~CollisionObb(){}

	//������
	void CollisionObb::Create(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_CUBE"));
		SetDrawActive(false);
	}


	//�A�N�Z�T
	float CollisionObb::GetMakedSize() const{
		return pImpl->m_Size;
	}
	void CollisionObb::SetMakedSize(float f){
		pImpl->m_Size = f;
	}

	OBB CollisionObb::GetObb() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}
	OBB CollisionObb::GetBeforeObb() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetBeforeWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}
	Matrix4X4 CollisionObb::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionObb::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//�����CollisionSphere����������

		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforSphere = DestCollisionSpherePtr->GetBeforeSphere();


		Vector3 DestVelocity = DestSphere.m_Center - DestBeforSphere.m_Center;
		//OBB��Src�Ȃ̂ŁASphere���𓮂��������ɂ���
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforSphere, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
	}

	void CollisionObb::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//�����CollisionSphere����������

		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();


		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforCapsule.GetCenter();
		//OBB��Src�Ȃ̂ŁACapsule���𓮂��������ɂ���
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforCapsule, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
			//�q�b�g�n�_�܂Ŗ߂�
			PtrT->LerpBeforeToNow(HitTime);
			//�q�b�g�I�u�W�F�N�g��ݒ�
			//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
			//�ݒ肳���
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
	}



	void CollisionObb::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�O�̏Փ˂�Transform���ς���Ă�\��������̂ŁA���[�v���Ŏ擾
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//�����CollisionObb����������
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;

		if (SrcObb.m_Size.Length() <= DestObb.m_Size.Length()){
			Vector3 SpanVelocity = SrcVelocity - DestVelocity;
			SpanVelocity /= ElapsedTime;
			float HitTime = 0;
			if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
				//�q�b�g�n�_�܂Ŗ߂�
				PtrT->LerpBeforeToNow(HitTime);
				//�q�b�g�I�u�W�F�N�g��ݒ�
				//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
				//�ݒ肳���
				SetHitTime(HitTime);
				SetHitObject(DestObj);
			}
		}
		else{
			Vector3 SpanVelocity = DestVelocity - SrcVelocity;
			SpanVelocity /= ElapsedTime;
			float HitTime = 0;
			if (HitTest::CollisionTestObbObb(DestBeforeObb, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
				//�q�b�g�n�_�܂Ŗ߂�
				PtrT->LerpBeforeToNow(HitTime);
				//�q�b�g�I�u�W�F�N�g��ݒ�
				//�����Ďc������肷�邪�A�ŏI�I�ɁA���Before�n�_�ɋ߂��I�u�W�F�N�g��
				//�ݒ肳���
				SetHitTime(HitTime);
				SetHitObject(DestObj);
			}
		}
	}

	void CollisionObb::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//�����CollisionSphere����������
			OBB SrcObb = GetObb();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			//�ŋߐړ_�𓾂�
			HitTest::SPHERE_OBB(DestSphere, SrcObb, ClosestPoint);
			//�@����Dest���i���̑��j�̖@�����o���ċt�����ɂ���
			Normal = DestSphere.m_Center - ClosestPoint;
			Normal *= -1.0f;
			Normal.Normalize();
		}
		else if (DestCollisionCapsulePtr){
			//�����DestCollisionCapsulePtr����������
			OBB SrcObb = GetObb();
			CAPSULE DestCapsle = DestCollisionCapsulePtr->GetCapsule();
			HitTest::CAPSULE_OBB(DestCapsle, SrcObb, ClosestPoint);
			float t;
			Vector3 RetVec;
			HitTest::ClosetPtPointSegment(ClosestPoint, DestCapsle.m_PointA, DestCapsle.m_PointB, t, RetVec);
			Normal = ClosestPoint - RetVec;
			Normal *= -1.0f;
			Normal.Normalize();
		}
		else if (DestCollisionObbPtr){
			//�����DestCollisionObbPtr����������
			OBB SrcObb = GetObb();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//�ŋߐړ_�𓾂�
			HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, ClosestPoint);
			Normal = SrcObb.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.5f, Normal);
		}
	}

	void CollisionObb::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		auto DestCollisionSpherePtr = DestGameObject->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestGameObject->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestGameObject->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			OBB SrcObb = GetObb();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			float BaseF = Vector3EX::Length(ClosestPoint - DestSphere.m_Center);
			float NewF = DestSphere.m_Radius + (DestSphere.m_Radius * GetEscapeSpanMin());
			float MoveF = NewF - BaseF;
			Vector3 MoveV = Normal * MoveF;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center + MoveV);
		}
		else if (DestCollisionCapsulePtr){
			OBB SrcObb = GetObb();
			SrcObb.m_Size *= GetEscapeAlignPlus();
			CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
			Vector3 EscapeSpan;
			EscapeSpan = Normal;
			EscapeSpan.Normalize();
			EscapeSpan *= (SrcObb.GetMinSize() * GetEscapeSpanMin());
			Vector3 RetVec;
			int count = 0;
			while (HitTest::CAPSULE_OBB(DestCapsule, SrcObb, RetVec)){
				SrcObb.m_Center += EscapeSpan;
				EscapeSpan *= GetEscapeAlignPlus();
				count++;
				if (count > 30){
					break;
				}
			}
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center);
		}
		else if (DestCollisionObbPtr){
			//�����DestCollisionObbPtr����������
			OBB SrcObb = GetObb();
			SrcObb.m_Size *= GetEscapeAlignPlus();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			Vector3 EscapeSpan;
			EscapeSpan = Normal;
			EscapeSpan.Normalize();
			EscapeSpan *= (SrcObb.GetMinSize() * GetEscapeSpanMin());
			Vector3 RetVec;
			int count = 0;
			while (HitTest::OBB_OBB(SrcObb, DestObb)){
				SrcObb.m_Center += EscapeSpan;
				EscapeSpan *= GetEscapeAlignPlus();
				count++;
				if (count > 30){
					break;
				}
			}
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center);
		}
	}

	void CollisionObb::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		//OBB��OBB�ɏ��`�͖��Ή�
	}

	//Gravity����Ă΂��
	bool CollisionObb::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//���̃I�u�W�F�N�g��OBB�ȊO�͐ݒ�ł��Ȃ�
			return false;
		}
		//������OBB
		OBB SrcObb = GetObb();
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			//SrcObb�����ӂ̂S�_�����߂�
			//���S����̑��΋����ō쐬����
			vector<Vector3> BottomPoints = {
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
			};
			for (auto p : BottomPoints){
				Vector3 StartPoint = p + Vector3(0, 0.1f, 0);
				StartPoint.Transform(DestObb.GetRotMatrix());
				StartPoint += SrcObb.m_Center;
				Vector3 EndPoint = p + Vector3(0, -0.1f, 0);
				EndPoint.Transform(DestObb.GetRotMatrix());
				EndPoint += SrcObb.m_Center;
				//��ɏ���Ă邩�ǂ���������
				//���C��ł�����ł݂�
				if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
					return true;
				}
			}
		}
		else{
			//SrcObb�����ӂ̂S�_�����߂�
			//���S����̑��΋����ō쐬����
			vector<Vector3> BottomPoints = {
				Vector3(SrcObb.m_Size.x, SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(SrcObb.m_Size.x, SrcObb.m_Size.y, -SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, SrcObb.m_Size.y, -SrcObb.m_Size.z),
			};
			for (auto p : BottomPoints){
				Vector3 StartPoint = p + Vector3(0, 0.1f, 0);
				StartPoint.Transform(DestObb.GetRotMatrix());
				StartPoint += SrcObb.m_Center;
				Vector3 EndPoint = p + Vector3(0, 0.1f, 0);
				EndPoint.Transform(DestObb.GetRotMatrix());
				EndPoint += SrcObb.m_Center;
				//��ɏ���Ă邩�ǂ���������
				//���C��ł�����ł݂�
				if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
					return true;
				}
			}

		}
		return false;
	}


}

//end basedx11

