/*!
@file Rigidbody.h
@brief Gravity,Rigidbody,Collision,SteeringComponent�Ƃ��̔h���N���X

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class Gravity : public Component ;
	//	�p�r: �d��
	//--------------------------------------------------------------------------------------
	class Gravity : public Component{
		//on�I�u�W�F�N�g�����݂��L�����ǂ������`�F�b�N����
		bool CheckOnObject(const shared_ptr<GameObject>& OnObject);
		//�q�b�g�����I�u�W�F�N�g�𒲂ׁA���ꂪ�����ɂ�������OnObject�ɐݒ肷��i���̏�ɏ��j
		void HitObjectToOnObject();
	public:
		explicit Gravity(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Gravity();
		//�A�N�Z�T
		const Vector3& GetGravity() const;
		void SetGravity(const Vector3& Gravity);
		void SetGravity(float x, float y, float z);

		const Vector3& GetInvGravity() const;
		void SetInvGravity(const Vector3& InvGravity);
		void SetInvGravity(float x, float y, float z);
		void SetInvGravityDefault();


		const Vector3& GetGravityVelocity() const;
		void SetGravityVelocity(const Vector3& GravityVelocity);
		void SetGravityVelocity(float x, float y, float z);

		bool IsGravityVelocityZero();
		void SetGravityVelocityZero();


		float GetBaseY() const;
		void SetBaseY(float y);

		float GetRayUnderSize() const;
		void SetRayUnderSize(float f);

		void CheckBaseY();

		shared_ptr<GameObject> GetOnObject() const;
		void SetOnObject(const shared_ptr<GameObject>& OnObject);
		void ClearOnObject();


		//�W�����v�̃X�^�[�g
		void StartJump(const Vector3& StartVec,float EscapeSpan = 0.001f);
		void StartJump(float x, float y, float z,float EscapeSpan = 0.001f);

		//����
		virtual void Update()override;
		virtual void Update2()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SteeringComponent : public Component ;
	//	�p�r: ���ǃR���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class SteeringComponent : public Component{
	protected:
		explicit SteeringComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SteeringComponent();
	public:
		float GetWeight() const;
		void SetWeight(float f);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SeekSteering : public SteeringComponent;
	//	�p�r: Seek����
	//--------------------------------------------------------------------------------------
	class SeekSteering : public SteeringComponent{
	public:
		explicit SeekSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SeekSteering();

		//�A�N�Z�T
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ArriveSteering : public SteeringComponent;
	//	�p�r: Arrive����
	//--------------------------------------------------------------------------------------
	class ArriveSteering : public SteeringComponent{
	public:
		explicit ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ArriveSteering();

		//�A�N�Z�T
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		float GetDecl() const;
		void SetDecl(float f);


		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PursuitSteering : public SteeringComponent;
	//	�p�r: Pursuit����
	//--------------------------------------------------------------------------------------
	class PursuitSteering : public SteeringComponent{
	public:
		explicit PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PursuitSteering();

		//�A�N�Z�T
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		const Quaternion& GetTargetQuaternion() const;
		void SetTargetQuaternion(const Quaternion& Qt);
		void SetTargetRotation(const Vector3& rotation);
		void SetTargetRotation(float x, float y, float z);

		const Vector3& GetTargetVelocity() const;
		void SetTargetVelocity(const Vector3& velocity);
		void SetTargetVelocity(float x, float y, float z);


		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class WanderSteering : public SteeringComponent;
	//	�p�r: Wander����
	//--------------------------------------------------------------------------------------
	class WanderSteering : public SteeringComponent{
	public:
		explicit WanderSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~WanderSteering();

		//�A�N�Z�T
		const Vector3& GetWanderTarget() const;
		void SetWanderTarget(const Vector3& target);

		float GetWanderRadius() const;
		void SetWanderRadius(float f);
		float GetWanderDistance() const;
		void SetWanderDistance(float f);
		float GetWanderJitter() const;
		void SetWanderJitter(float f);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class WallAvoidanceSteering : public SteeringComponent;
	//	�p�r: WallAvoidance����(�ǉ��)
	//--------------------------------------------------------------------------------------
	class WallAvoidanceSteering : public SteeringComponent{
	public:
		explicit WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~WallAvoidanceSteering();

		//�A�N�Z�T
		//�������ǂ̔z��
		vector<PLANE>& GetPlaneVec() const;
		void SetPlaneVec(const vector<PLANE>& planevec);
		//Plane�^��n��
		void SetPlaneVec(const vector<Plane>& planevec);

		//�ǂƏՓ˂��Ă��邩
		bool IsWallArribed() const;

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ObstacleAvoidanceSteering : public SteeringComponent;
	//	�p�r: ObstacleAvoidance����(��Q�����)
	//--------------------------------------------------------------------------------------
	class ObstacleAvoidanceSteering : public SteeringComponent{
	public:
		explicit ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ObstacleAvoidanceSteering();

		//�A�N�Z�T
		//��Q���̔z��
		const vector<SPHERE>& GetObstacleSphereVec() const;
		void SetObstacleSphereVec(const vector<SPHERE>& spherevec);
		//���̍���
		float GetRoadWidth() const;
		void SetRoadWidth(float f);
		//����
		float GetRoadHeight() const;
		void SetRoadHeight(float f);


		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	


	//--------------------------------------------------------------------------------------
	//	class FollowPathSteering : public SteeringComponent;
	//	�p�r: FollowPath����(�o�H�Ǐ])
	//--------------------------------------------------------------------------------------
	class FollowPathSteering : public SteeringComponent{
	public:
		explicit FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~FollowPathSteering();

		//�A�N�Z�T
		///�o�H������킷���X�g
		void SetPathList(const list<Vector3>& pathlist);

		float GetDecl() const;
		void SetDecl(float f);

		float GetWaypointSpan() const;
		void SetWaypointSpan(float f);

		bool GetLooped() const;
		bool IsLooped() const;
		void SetLooped(bool b);

		bool IsFinished() const;

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class AlignmentSteering : public SteeringComponent;
	//	�p�r: Alignment����(����)
	//--------------------------------------------------------------------------------------
	class AlignmentSteering : public SteeringComponent{
	public:
		AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~AlignmentSteering();
		//�A�N�Z�T
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CohesionSteering : public SteeringComponent;
	//	�p�r: Cohesion����(����)
	//--------------------------------------------------------------------------------------
	class CohesionSteering : public SteeringComponent{
	public:
		CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~CohesionSteering();
		//�A�N�Z�T
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SeparationSteering : public SteeringComponent;
	//	�p�r: Separation����
	//--------------------------------------------------------------------------------------
	class SeparationSteering : public SteeringComponent{
	public:
		SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~SeparationSteering();
		//�A�N�Z�T
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	�p�r: �����v�Z�t�����ʒu�R���g���[��
	//--------------------------------------------------------------------------------------
	class Rigidbody : public Component {
	public:
		//�\�z�Ɣj��
		explicit Rigidbody(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Rigidbody();
		//�A�N�Z�T
		const Vector3& GetVelocity() const;
		void SetVelocity(const Vector3& Velocity);
		void SetVelocity(float x, float y, float z);

		bool IsVelocityZeroCommand() const;
		void SetVelocityZeroCommand(bool b);

		const Vector3& GetHitVelocity() const;
		void SetHitVelocity(const Vector3& Velocity);
		void SetHitVelocity(float x, float y, float z);


		float GetMaxSpeed() const;
		void SetMaxSpeed(float f);

		const Vector3& GetMinVelocity() const;
		void SetMinVelocity(const Vector3& Velocity);
		void SetMinVelocity(float x, float y, float z);


		const Vector3& GetAngularVelocity() const;
		void SetAngularVelocity(const Vector3& AngularVelocity);
		void SetAngularVelocity(float x, float y, float z);


		float GetMass() const;
		void SetMass(float f);

		float GetReflection() const;			//�����W��
		void SetReflection(float f);

		const Vector3& GetForce() const;
		void SetForce(const Vector3& Force);
		void SetForce(float x, float y, float z);

		float GetFriction() const;
		void SetFriction(float f);

		float GetMaxForce() const;
		void SetMaxForce(float f);

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	};

	class MeshResource;
	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	�p�r: �Փ˔���R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class Collision : public Component {
		friend class Gravity;
		void ResetHitObject();
		bool IsNewDest() const;
		bool NeedAfterCollision() const;
		bool NeedSendEvent() const;
		bool NeedPostEvent() const;
		//���O�O���[�v�ɑ����Ă��邩�𒲂ׂ�
		bool ChkExcludeCollisionObject(const shared_ptr<GameObject>&  CheckGameObject);

		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) = 0;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) = 0;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) = 0;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint) = 0;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal,const Vector3& ClosestPoint) = 0;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject) = 0;
		//on�I�u�W�F�N�g�����݂��L�����ǂ������`�F�b�N����
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject) = 0;
	protected:
		//�\�z�Ɣj��
		explicit Collision(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Collision();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetMeshResource(const shared_ptr<MeshResource>& Ptr);
		//Rigidbody������ꍇ��Update2
		void Update2WithRigidbody2();
		//�����o�������̌W��
		float GetEscapeSpanMin() const;
		void SetEscapeSpanMin(float f);
		float GetEscapeAlignPlus() const;
		void SetEscapeAlignPlus(float f);
	public:
		//�A�N�Z�T
		bool GetFixed() const;
		bool IsFixed() const;
		void SetFixed(bool b);

		bool GetSendEventActive() const;
		bool IsSendEventActive() const;
		void SetSendEventActive(bool b);

		bool GetPostEventActive() const;
		bool IsPostEventActive() const;
		void SetPostEventActive(bool b);

		float GetPostDispatchTime() const;			//Post�̍ۂ̒x������
		void SetPostDispatchTime(float f);


		wstring GetEventString() const;
		void SetEventString(const wstring& str);


		bool GetAutoAfterCollision() const;
		bool IsAutoAfterCollision() const;
		void SetAutoAfterCollision(bool b);

		bool GetNewDestOnly() const;
		bool IsNewDestOnly() const;
		void SetNewDestOnly(bool b);

		shared_ptr<GameObject> GetHitObject() const;
		void SetHitObject(const shared_ptr<GameObject>& Ptr);

		void ClearBothHitObject();

		shared_ptr<GameObject> GetSubDispatchObject() const;
		void SetSubDispatchObject(const shared_ptr<GameObject>& Ptr);

		//���肩�珜�O����I�u�W�F�N�g�O���[�v�̐ݒ�
		void SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group);

		float GetHitTime() const;
		void SetHitTime(float f);

		//on�I�u�W�F�N�g�����݂��L�����ǂ������`�F�b�N����
		//Gravity����Ă΂��
		bool CheckOnObjectBase(const shared_ptr<GameObject>& OnObject);

		//���z�֐�
		virtual Matrix4X4 GetCollisionMatrix() const = 0;

		//����
		virtual void Update()override;
		virtual void Update2()override;
		virtual void Draw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	�p�r: ���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CollisionSphere : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//Gravity����Collision�o�R�ŌĂ΂��
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//�\�z�Ɣj��
		explicit CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionSphere();
		//������
		virtual void Create() override;

		//�A�N�Z�T
		float GetMakedDiameter() const;	//�쐬���̒��a
		void SetMakedDiameter(float f);		//�쐬���̒��a
		float GetMakedRadius() const;	//�쐬���̔��a
		void SetMakedRadius(float f);			//�쐬���̔��a


		SPHERE GetSphere() const;
		SPHERE GetBeforeSphere() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//����
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	�p�r: �J�v�Z���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CollisionCapsule : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//Gravity����Collision�o�R�ŌĂ΂��
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//�\�z�Ɣj��
		explicit CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionCapsule();
		//������
		virtual void Create() override;
		//�A�N�Z�T
		float GetMakedDiameter() const;	//�쐬���̒��a
		void SetMakedDiameter(float f);		//�쐬���̒��a
		float GetMakedRadius() const;	//�쐬���̔��a
		void SetMakedRadius(float f);			//�쐬���̔��a
		float GetMakedHeight() const;	//�쐬���̍���
		void SetMakedHeight(float f);		//�쐬���̍���

		CAPSULE GetCapsule() const;
		CAPSULE GetBeforeCapsule() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//����
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	�p�r: Obb�Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CollisionObb : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//Gravity����Collision�o�R�ŌĂ΂��
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//�\�z�Ɣj��
		explicit CollisionObb(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionObb();
		//������
		virtual void Create() override;

		//�A�N�Z�T
		float GetMakedSize() const;	//�쐬����1�ӂ̒���
		void SetMakedSize(float f);


		OBB GetObb() const;
		OBB GetBeforeObb() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//����
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





}

//end basedx11
