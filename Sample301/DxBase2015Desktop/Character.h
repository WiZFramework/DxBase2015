#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject{
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//���[�e�B���e�B�֐��Q
		//�v���C���[�̈ʒu��Ԃ�
		Vector3 GetPlayerPosition() const;
		//�v���C���[�܂ł̋�����Ԃ�
		float GetPlayerLength() const;
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void Create() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const{
			return m_StateMachine;
		}
		//���[�V��������������֐��Q
		void  SeekStartMoton();
		bool  SeekUpdateMoton();
		void  SeekEndMoton();

		void  ArriveStartMoton();
		bool  ArriveUpdateMoton();
		void  ArriveEndMoton();

		//����
		virtual void Update() override;
		virtual void Update3() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState(){}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState(){}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//������
		virtual void Create() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~MoveBox();
		//������
		virtual void Create() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	�p�r: ��Q���J�v�Z��
	//--------------------------------------------------------------------------------------
	class CapsuleObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CapsuleObject();
		//������
		virtual void Create() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SphereObject();
		//������
		virtual void Create() override;
		//����
	};


	//�Փ˔���Ɏg�p����RECT
	struct COLRECT{
		Vector3 m_Center;     //���S�_�̍��W
		Vector3 m_Rot[2];  //XY �̊e���W���̌X����\�������x�N�g��
		float m_UVec[2];     //XY���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
		COLRECT(){}
		//���̊�{�傫���A�c�̊�{�傫�����ƕϊ��s���^����RECT������������
		COLRECT(float BaseXSize, float BaseYSize, const Matrix4X4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11, Matrix._12, Matrix._13);
			Vector3 VecY(Matrix._21, Matrix._22, Matrix._23);
			m_UVec[0] = BaseXSize * VecX.Length() * 0.5f;
			m_UVec[1] = BaseYSize * VecY.Length() * 0.5f;
			//��]�𓾂�
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
		}
	};


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	�p�r: ���ƏՓ˔��肷��X�N�G�A
	//--------------------------------------------------------------------------------------
	class HitTestSquare : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~HitTestSquare();
		//������
		virtual void Create() override;
		//���ʂ�Ԃ�
		PLANE GetPLANE() const;
		//����p��RECT��Ԃ�
		COLRECT GetCOLRECT() const;

		//�_�Ƃ̍ŋߐړ_��Ԃ�
		void ClosestPtPoint(const Vector3& Point, Vector3& Ret);

		//�_�Ƃ̋�����Ԃ��i�߂�l���}�C�i�X�Ȃ痠���j
		float GetDistPointPlane(const Vector3& Point) const;
		//�q�b�g�e�X�g�����čŋߐړ_�ƁASquare�̖@����Ԃ�
		bool HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal);

		//����
		virtual void Update() override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NumberSprite : public GameObject{
		Vector3 m_StartPos;
		vector< vector<VertexPositionColorTexture> > m_NumberBurtexVec;
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~NumberSprite();
		//������
		virtual void Create() override;
		//�ω�
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class NumberSquare : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NumberSquare : public GameObject{
		weak_ptr<SeekObject> m_SeekObject;
		//���̃I�u�W�F�N�g�݂̂Ŏg�p����X�N�G�A���b�V��
		shared_ptr<CommonMeshResource> m_SquareMeshResource;
		//�w�ԍ�
		size_t m_Number;
	public:
		//�\�z�Ɣj��
		NumberSquare(shared_ptr<Stage>& StagePtr, const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number);
		virtual ~NumberSquare();
		//������
		virtual void Create() override;
		//�ω�
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BarSprite : public GameObject{
		Vector3 m_StartPos;
		vector< vector<VertexPositionColorTexture> > m_BarBurtexVec;
		//�ő厞�ԁi�������ԁj
		float m_MaxTime;
		//�c�莞��
		float m_LastTime;
	public:
		//�\�z�Ɣj��
		BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BarSprite();
		//������
		virtual void Create() override;
		//�ω�
		virtual void Update() override;
	};




}
//endof  basedx11
