#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		//移動の向きを得る
		Vector3 GetAngle();
	public:
		//構築と破棄
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player(){}
		//初期化
		virtual void Create() override;
		//アクセサ
		//砲弾の発射
		void StartShellBall();
		//更新
		virtual void Update() override;
	};


}
//endof  basedx11
