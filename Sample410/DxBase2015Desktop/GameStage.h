#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//プレートの作成
		void CreatePlate();
		//ボックスの作成
		void CreateFixedBox();
		//描画マネージャの作成
		void CreateDrawBoxManager();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void Create()override;
	};
}
//endof  basedx11
