#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class Path;
	//	�p�r: �o�H�N���X
	//--------------------------------------------------------------------------------------
	class Path{
		//�o�H�̃|�C���g�̃��X�g
		list<Vector3> m_WayPoints;
		//���݃C�e���[�^�[
		list<Vector3>::iterator m_CurWaypoint;
		//���[�v���邩�ǂ���
		bool m_Looped;
	public:
		//--------------------------------------------------------------------------------------
		//	Path();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		Path() :
			m_Looped(false){
		}
		//--------------------------------------------------------------------------------------
		//	virtual ~Path();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		virtual ~Path(){
			m_WayPoints.clear();
		}
		//--------------------------------------------------------------------------------------
		//�A�N�Z�b�T
		//--------------------------------------------------------------------------------------
		bool GetLooped()const{ return m_Looped; }
		list<Vector3>* GetPathPtr(){ return &m_WayPoints; }
		Vector3 GetCurWaypoint()const{
			if (m_CurWaypoint == m_WayPoints.end()){
				return m_WayPoints.back();
			}
			else{
				return *m_CurWaypoint;
			}
		}
		void SetLooped(bool b){ m_Looped = b; }
		//--------------------------------------------------------------------------------------
		//	bool IsFinished();
		//	�p�r: �Ō�܂ōs���Ă邩�ǂ���
		//	�߂�l: �Ō�܂ōs���Ă����true
		//--------------------------------------------------------------------------------------
		bool IsFinished(){
			return !(m_CurWaypoint != m_WayPoints.end());
		}
		//--------------------------------------------------------------------------------------
		//	void Clear();
		//	�p�r: ���X�g���N���A����
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Clear(){ m_WayPoints.clear(); }
		//--------------------------------------------------------------------------------------
		//	void SetList(
		//	const list<Vector3> new_path	//�V�������X�g
		//	);
		//	�p�r: ���X�g�������ւ���
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void SetList(const list<Vector3>& new_path){
			Clear();
			auto it = new_path.begin();
			while (it != new_path.end()){
				m_WayPoints.push_back(*it);
				it++;
			}
			m_CurWaypoint = m_WayPoints.begin();
		}
		//--------------------------------------------------------------------------------------
		//	void SetNextWaypoint();
		//	�p�r: ���̃|�C���g�ɐi�߂�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void SetNextWaypoint(){
			if (m_WayPoints.size() <= 0){
				throw BaseException(
					L"���X�g����ł��B",
					L"if(m_WayPoints.size() <= 0)",
					L"Path::SetNextWaypoint()"
					);
			}
			if (m_CurWaypoint == m_WayPoints.end()){
				return;
			}
			if (++m_CurWaypoint == m_WayPoints.end()){
				if (m_Looped){
					m_CurWaypoint = m_WayPoints.begin();
				}
			}
		}
	};




	class GameObject;
	class GameObjectGroup;

	//--------------------------------------------------------------------------------------
	//	struct Steering;
	//	�p�r: ���Ǌ֘A���[�e�B���e�B
	//	��static�Ăяo��������
	//--------------------------------------------------------------------------------------
	struct Steering{
		Steering(){}
		~Steering(){}
		//--------------------------------------------------------------------------------------
		//	static  bool AccumulateForce(
		//	Vector3& Force,
		//	const Vector3& ForceToAdd,
		//	float MaxForce
		//	);
		//	�p�r: ���Ǘ͂����Z����
		//	�߂�l: ���Z������true
		//	��Force�Ɍv�Z���ʂ����Z����
		//--------------------------------------------------------------------------------------
		static bool AccumulateForce(Vector3& Force, const Vector3& ForceToAdd, float MaxForce);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Seek(
		//	const Vector3& Velocity,	//���݂̑��x
		//	const Vector3& Target,		//�ڕW
		//	const Vector3& Pos,			//���݂̈ʒu
		//	float MaxSpeed				//�ō����x
		//	);
		//	�p�r: �T���s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Seek(const Vector3& Velocity, const Vector3& Target, const Vector3& Pos, float MaxSpeed);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Flee(
		//	const Vector3& Velocity,	//���݂̑��x
		//	const Vector3& Target,		//�ڕW
		//	const Vector3& Pos,			//���݂̈ʒu
		//	float MaxSpeed,				//�ō����x
		//	float PanicDistance			//�����o�����������͈͂�1��
		//	);
		//	�p�r: �����s��
		//	�߂�l: �t�H�[�X
		//	���^�[�Q�b�g�� PanicDistance * PanicDistance�̋����͈͂ɓ������瓦���o��
		//--------------------------------------------------------------------------------------
		static Vector3 Flee(const Vector3& Velocity, const Vector3& Target,
			const Vector3& Pos, float MaxSpeed, float PanicDistance);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Arrive(
		//	const Vector3& Velocity,	//���݂̑��x
		//	const Vector3& Target,		//�ڕW
		//	const Vector3& Pos,			//���݂̈ʒu
		//	float MaxSpeed				//�ō����x
		//	float Decl					//�����l�i1.0����3.0�̊Ԓ��x�j
		//	);
		//	�p�r: �����s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Arrive(const Vector3& Velocity, const Vector3& Target,
			const Vector3& Pos, float MaxSpeed, float Decl);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Pursuit(
		//	const Vector3& Velocity,	//���݂̑��x
		//	const Vector3& Pos,			//���݂̈ʒu
		//	const Vector3& Rot,			//���݂̌���
		//	float MaxSpeed,				//�ō����x
		//	const Vector3& TargetVelocity,	//�ڕW�̌��݂̑��x
		//	const Vector3& Target,		//�ڕW
		//	const Vector3& TargetRot	//�ڕW�̌���
		//	);
		//	�p�r: �ǐՍs��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Pursuit(const Vector3& Velocity, const Vector3& Pos, const Vector3& Rot, float MaxSpeed,
			const Vector3& TargetVelocity, const Vector3& Target, const Vector3& TargetRot);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Wander(
		//	const Matrix4X4 Matrix,		//���݂̍s��
		//	float WanderRadius,	//�p�j���锼�a
		//	float WanderDistance,	//�p�j���锼�a�܂ł̋���
		//	float WanderJitter,	//�����_���ψق̍ő�l
		//	Vector3& WanderTarget	//�p�j�̃^�[�Q�b�g�ϐ��i��]�l�j
		//	);
		//	�p�r: �p�j�s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Wander(const Matrix4X4 Matrix,
			float WanderRadius, float WanderDistance, float WanderJitter, Vector3& WanderTarget);
		//--------------------------------------------------------------------------------------
		//	static Vector3 ObstacleAvoidance(
		//	const Matrix4X4 Matrix,		//���݂̍s��
		//	const Vector3& Velocity,	//���݂̑��x
		//	float MaxSpeed,				//�ō����x
		//	float Width,				//������铹��
		//	float Height,				//������鍂��
		//	const vector<SPHERE>& SphereVec	//������ׂ����̂̔z��
		//	);
		//	�p�r: ��Q������s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 ObstacleAvoidance(const Matrix4X4 Matrix,
			const Vector3& Velocity, float MaxSpeed, float Width, float Height,
			const vector<SPHERE>& SphereVec);
		//--------------------------------------------------------------------------------------
		//	static Vector3 WallAvoidance(
		//	const Matrix4X4 Matrix,		//���݂̍s��
		//	const Vector3& Velocity,	//���݂̑��x
		//	float MaxSpeed,				//�ō����x
		//	const vector<PLANE>& PlaneVec	//������ׂ����̂̔z��
		//	);
		//	�p�r: �ǉ���s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 WallAvoidance(const Matrix4X4 Matrix,
			const Vector3& Velocity, float MaxSpeed, const vector<PLANE>& PlaneVec);
		//--------------------------------------------------------------------------------------
		//	static Vector3 FollowPath(
		//	Path& rPath,				//�p�X
		//	float WaypointSeekDist,		//Seek�J�n�܂ł̋���
		//	const Vector3& Pos,			//���݂̈ʒu
		//	const Vector3& Velocity,	//���݂̑��x
		//	float MaxSpeed,				//�ō����x
		//	float Decl					//�����l�i1.0����3.0�̊Ԓ��x�j
		//	);
		//	�p�r: �o�H�s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 FollowPath(Path& rPath,
			float WaypointSeekDist,
			const Vector3& Pos, const Vector3& Velocity,
			float MaxSpeed, float Decl);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Separation(
		//	const shared_ptr<GameObjectGroup>& Group,	//�ݒ肷��O���[�v�̔z��
		//	const shared_ptr<GameObject>& MyObj				//�������g
		//	);
		//	�p�r: �����s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Separation(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Alignment(
		//	const shared_ptr<GameObjectGroup>& Group,	//�ݒ肷��O���[�v�̔z��
		//	const shared_ptr<GameObject>& MyObj				//�������g
		//	);
		//	�p�r: ����s��
		//	�߂�l: �t�H�[�X
		//--------------------------------------------------------------------------------------
		static Vector3 Alignment(const shared_ptr<GameObjectGroup>& Group,const shared_ptr<GameObject>& MyObj);
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
		static Vector3 Cohesion(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj, 
			const Vector3& Velocity, float MaxSpeed);




	};

}
//endof  basedx11
