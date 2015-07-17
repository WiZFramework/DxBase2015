#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		Vector3 m_StartPos;
		float m_YRot;
		float m_RotationSpeed;
	public:
		//�\�z�Ɣj��
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//������
		virtual void Create() override;
		virtual void Update() override;
	};


}
//endof  basedx11
