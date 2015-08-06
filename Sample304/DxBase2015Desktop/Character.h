#pragma once

#include "stdafx.h"

namespace basedx11{

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
