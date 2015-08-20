#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	用途: アクションする配置オブジェクト
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//構築と破棄
		ActionObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~ActionObject();
		//初期化
		virtual void Create() override;
		virtual void Update2() override;
	};


}
//endof  basedx11
