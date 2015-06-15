#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	�p�r: �����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class MoveObject : public GameObject{
		Vector3 m_StartPos;
		Vector3 m_StartVelocity;
		bool m_Type;
	public:
		//�\�z�Ɣj��
		MoveObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, const Vector3& StartVelocity, bool Type);
		virtual ~MoveObject();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update2() override;
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



}
//endof  basedx11
