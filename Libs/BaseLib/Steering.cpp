#include "stdafx.h"


namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct Steering;
	//	�p�r: ���Ǌ֘A���[�e�B���e�B
	//	��static�Ăяo��������
	//--------------------------------------------------------------------------------------
	//�X�^�e�B�b�N�����o
		bool Steering::AccumulateForce(Vector3& Force, const Vector3& ForceToAdd, float MaxForce){
		//���݂̗͂̒����𓾂�
		float MagnitudeSoFar = Force.Length();
		//�ő�l�Ƃ̍������߂�
		float magnitudeRemaining = MaxForce - MagnitudeSoFar;
		//����0�ȉ��i�܂�ő�l�𒴂��Ă�����j
		//�ǉ����Ȃ��Ń��^�[��
		if (magnitudeRemaining <= 0.0f){
			return false;
		}
		//�ǉ�����͂̑傫�������߂�
		float MagnitudeToAdd = ForceToAdd.Length();
		//�͂̒ǉ�
		if (MagnitudeToAdd < magnitudeRemaining){
			Force += ForceToAdd;
		}
		else{
			Force += (Vector3EX::Normalize(ForceToAdd) * MagnitudeToAdd);
		}
		//�ǉ����ꂽ�w�W��Ԃ�  
		return true;
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::Seek(const Vector3& Velocity, const Vector3& Target, const Vector3& Pos, float MaxSpeed){
		Vector3 DesiredVelocity
			= Vector3EX::Normalize(Target - Pos) * MaxSpeed;
		return (DesiredVelocity - Velocity);
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::Flee(const Vector3& Velocity, const Vector3& Target,
		const Vector3& Pos, float MaxSpeed, float PanicDistance){
		float PanicDistanceSq = PanicDistance * PanicDistance;
		if (Vector3EX::LengthSq(Pos - Target) > PanicDistanceSq){
			return Vector3(0, 0, 0);
		}
		Vector3 DesiredVelocity
			= Vector3EX::Normalize(Pos - Target) * MaxSpeed;
		return (DesiredVelocity - Velocity);
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::Arrive(const Vector3& Velocity, const Vector3& Target, const Vector3& Pos, float MaxSpeed, float Decl){
		Vector3 ToTarget = Target - Pos;
		float dist = ToTarget.Length();
		if (dist > 0){
			const float DecelerationTweaker = 0.3f;
			float speed = dist / (Decl * DecelerationTweaker);
			speed = Util::Minimum(speed, MaxSpeed);
			Vector3 DesiredVelocity = ToTarget * speed / dist;
			return (DesiredVelocity - Velocity);
		}
		return Vector3(0, 0, 0);
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::Pursuit(const Vector3& Velocity, const Vector3& Pos, const Vector3& Rot, float MaxSpeed,
		const Vector3& TargetVelocity, const Vector3& Target, const Vector3& TargetRot){
		Vector3 ToEvader = Target - Pos;
		double RelativeHeading = Rot.Dot(TargetRot);
		if ((ToEvader.Dot(Rot) > 0) &&
			(RelativeHeading < -0.95))  //acos(0.95)=18 degs
		{
			return Steering::Seek(Velocity, Target, Pos, MaxSpeed);
		}
		float LookAheadTime = ToEvader.Length() /
			(MaxSpeed + TargetVelocity.Length());
		return Steering::Seek(Velocity, Target + TargetVelocity * LookAheadTime, Pos, MaxSpeed);
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::Wander(const Matrix4X4 Matrix,
		float WanderRadius, float WanderDistance, float WanderJitter, Vector3& WanderTarget){
		WanderTarget += Vector3(
			(Util::RandZeroToOne(true) * 2.0f - 1.0f)  * WanderJitter,
			0,
			(Util::RandZeroToOne(true) * 2.0f - 1.0f)  * WanderJitter
			);
		WanderTarget.Normalize();
		WanderTarget *= WanderRadius;
		Vector3 wander_target = WanderTarget + Vector3(WanderDistance, 0, 0);
		wander_target.Transform(Matrix);
		return wander_target - Matrix.PosInMatrix();
	}

	struct ObstacleAvoidanceSphere{
		SPHERE m_Sp;
		float Len;
		ObstacleAvoidanceSphere(const SPHERE& Sp, float len) :
			m_Sp(Sp), Len(len){}
	};

	//--------------------------------------------------------------------------------------
	bool SortSphereObstacleAvoidanceHandle(ObstacleAvoidanceSphere& Left, ObstacleAvoidanceSphere& Right){
		return Left.Len < Right.Len;
	}

	//--------------------------------------------------------------------------------------
	Vector3 Steering::ObstacleAvoidance(const Matrix4X4 Matrix,
		const Vector3& Velocity, float MaxSpeed, float Width, float Height,
		const vector<SPHERE>& SphereVec){
		//���݂̑��x�ƈʒu�Ɠ�������A�Փ˔���OBB���쐬����
		Vector3 Scale(Width, Height, Velocity.Length());
		Matrix4X4 ObbMat;
		ObbMat.DefTransformation(Scale, Matrix.QtInMatrix(), Matrix.PosInMatrix() + Velocity / 2.0f);
		OBB Obb(Vector3(1.0f, 1.0f, 1.0f), ObbMat);
		vector<ObstacleAvoidanceSphere> ChangeVec;
		for (size_t i = 0; i < SphereVec.size(); i++){
			float len = Vector3EX::Length(SphereVec[i].m_Center - Matrix.PosInMatrix());
			ObstacleAvoidanceSphere Sp(SphereVec[i], len);
			ChangeVec.push_back(Sp);
		}
		//������ύX�\�[�g
		std::sort(ChangeVec.begin(), ChangeVec.end(), SortSphereObstacleAvoidanceHandle);
		//�߂����Ɍ������ĉ����ƏՓ˂��Ă�����A�^�[�Q�b�g������SEEK
		for (size_t i = 0; i < ChangeVec.size(); i++){
			Vector3 RetVec;
			if (HitTest::SPHERE_OBB(ChangeVec[i].m_Sp, Obb, RetVec)){
				//�i�s������OBB�ƏՓ˂���
				//OBB�i�s�����̐�����RetVec�Ƃ̍ŋߐړ_�����߂�
				float t;
				Vector3 d;
				HitTest::ClosetPtPointSegment(RetVec, Matrix.PosInMatrix(), Matrix.PosInMatrix() + Velocity, t, d);
				//�ޔ�������v�Z����
				Vector3 AvoidanceVec = (d - RetVec);
				//���K��
				AvoidanceVec.Normalize();
				AvoidanceVec *= (Width + MaxSpeed);
				return AvoidanceVec;
			}
		}
		return Vector3(0, 0, 0);
	}

	struct AvoidanceSegment{
		Vector3 m_PointA;
		Vector3 m_PointB;
		AvoidanceSegment(){}
		AvoidanceSegment(const Vector3& pa, const Vector3& pb) :
			m_PointA(pa),
			m_PointB(pb)
		{
		}
	};

	//--------------------------------------------------------------------------------------
	Vector3 Steering::WallAvoidance(const Matrix4X4 Matrix,
		const Vector3& Velocity, float MaxSpeed, const vector<PLANE>& PlaneVec){
		//�܂��G�o�ɂȂ�����z����쐬
		float Len = Velocity.Length() * 0.5f;
		vector<AvoidanceSegment> Segments;
		Segments.push_back(AvoidanceSegment(Vector3(0, 0, Len), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(cos(XM_PIDIV4) * Len, 0, sin(XM_PIDIV4) * Len), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(-cos(XM_PIDIV4) * Len, 0, sin(XM_PIDIV4) * Len), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(Len, 0, 0), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(-Len, 0, 0), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(cos(XM_PIDIV4) * Len, 0, -sin(XM_PIDIV4) * Len), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(-cos(XM_PIDIV4) * Len, 0, -sin(XM_PIDIV4) * Len), Vector3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(Vector3(0, 0, -Len), Vector3(0, 0, 0)));
		for (size_t i = 0; i < Segments.size(); i++){
			//�G�o�̊e���_�ɍs����v�Z���āA���[���h���W�ɕϊ�
			Segments[i].m_PointA.Transform(Matrix);
			Segments[i].m_PointB.Transform(Matrix);
			//�����ƕǂ̏Փ˔���
			Vector3 RetVec;
			float t;
			for (size_t j = 0; j < PlaneVec.size(); j++){
				if (HitTest::InsidePtPlane(Matrix.PosInMatrix(), PlaneVec[j])){
					return PlaneVec[j].m_Normal * MaxSpeed;
				}
				if (HitTest::SEGMENT_PLANE(Segments[i].m_PointA, Segments[i].m_PointB, PlaneVec[j], t, RetVec)){
					//�����Ɩʂ��Փ˂��Ă���
					//�ʂ̖@���̕����Ɍ��݂̑��x�Ńt�H�[�X��Ԃ�
					return PlaneVec[j].m_Normal * Velocity.Length();
				}
			}
		}
		return Vector3(0, 0, 0);
	}



	//--------------------------------------------------------------------------------------
	Vector3 Steering::FollowPath(Path& rPath,
		float WaypointSeekDist,
		const Vector3& Pos, const Vector3& Velocity,
		float MaxSpeed, float Decl){
		Vector3 Dis = Pos - rPath.GetCurWaypoint();
		float DisSq = Dis.LengthSq();
		float WaypointSeekDistSq = WaypointSeekDist * WaypointSeekDist;

		if (DisSq < WaypointSeekDistSq){
			rPath.SetNextWaypoint();
		}
		if (!rPath.IsFinished()){
			return Seek(Velocity, rPath.GetCurWaypoint(), Pos, MaxSpeed);
		}
		else{
			return Arrive(Velocity, rPath.GetCurWaypoint(), Pos, MaxSpeed, Decl);
		}
	}

	Vector3 Steering::Separation(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj){
		Vector3 SteeringForce(0, 0, 0);
		auto Vec = Group->GetGroupVector();
		for (auto Ptr : Vec){
			if (!Ptr.expired()){
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj){
					PtrObj->GetComponent<Transform>();
					Vector3 ToAgent
						= MyObj->GetComponent<Transform>()->GetPosition()
						- PtrObj->GetComponent<Transform>()->GetPosition();
					SteeringForce += Vector3EX::Normalize(ToAgent) / ToAgent.Length();
				}
			}
		}
		return SteeringForce;
	}


	

	//--------------------------------------------------------------------------------------
	//	static Vector3 Alignment(
	//	const shared_ptr<GameObjectGroup>& Group,	//�ݒ肷��O���[�v�̔z��
	//	const shared_ptr<GameObject>& MyObj				//�������g
	//	);
	//	�p�r: ����s��
	//	�߂�l: �t�H�[�X
	//--------------------------------------------------------------------------------------
	Vector3 Steering::Alignment(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj){
		Vector3 AverageHeading(0, 0, 0);
		auto Vec = Group->GetGroupVector();
		int count = 0;
		for (auto Ptr : Vec){
			if (!Ptr.expired()){
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj){
					auto PtrT = PtrObj->GetComponent<Transform>();
					AverageHeading += QuaternionEX::GetRotation(PtrT->GetQuaternion());
					count++;
				}
			}
		}
		if (count > 0){
			AverageHeading /= (float)count;
			auto PtrT = MyObj->GetComponent<Transform>();
			AverageHeading -= QuaternionEX::GetRotation(PtrT->GetQuaternion());
		}
		return AverageHeading;
	}

	//--------------------------------------------------------------------------------------
	//	static Vector3 Cohesion(
	//	const shared_ptr<GameObjectGroup>& Group,	//�ݒ肷��O���[�v�̔z��
	//	const shared_ptr<GameObject>& MyObj			//�������g
	//	const Vector3& Velocity,	//���݂̑��x
	//	float MaxSpeed				//�ō����x
	//	);
	//	�p�r: �����s��
	//	�߂�l: �t�H�[�X
	//--------------------------------------------------------------------------------------
	Vector3 Steering::Cohesion(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj, 
		const Vector3& Velocity, float MaxSpeed){
		auto Vec = Group->GetGroupVector();
		Vector3 SteeringForce(0, 0, 0);
		//�d�S
		Vector3 CenterOfMass(0, 0, 0);
		int count = 0;
		for (auto Ptr : Vec){
			if (!Ptr.expired()){
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj){
					auto PtrT = PtrObj->GetComponent<Transform>();
					CenterOfMass += PtrT->GetPosition();
					count++;
				}
			}
		}
		if (count > 0){
			CenterOfMass /= (float)count;
			auto PtrT = MyObj->GetComponent<Transform>();
			SteeringForce = Seek(Velocity, CenterOfMass, PtrT->GetPosition(), MaxSpeed);
			SteeringForce.Normalize();
		}
		return SteeringForce;
	}

}
//endof  basedx11
