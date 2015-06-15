#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
	public:
		//構築と破棄
		Box(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//初期化
		virtual void Create() override;
		//更新
		virtual void Update() override;
	};


}
//endof  basedx11
