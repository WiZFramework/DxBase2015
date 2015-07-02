#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
		float m_Span;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update() override;
		virtual void Update2() override;
	};


}
//endof  basedx11
