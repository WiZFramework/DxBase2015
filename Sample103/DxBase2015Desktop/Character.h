#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	�p�r: �A�N�V��������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		ActionObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~ActionObject();
		//������
		virtual void Create() override;
		virtual void Update2() override;
	};


}
//endof  basedx11
