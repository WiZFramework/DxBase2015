#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MyCamera : public Camera ;
	//	用途: LookAtカメラをライブラリから外したもの
	//--------------------------------------------------------------------------------------
	class MyCamera : public Camera{
		weak_ptr<GameObject> m_TargetObject;	//目標となるオブジェクト
		float m_ToTargetLerp;	//目標を追いかける際の補間値
	public:
		//構築と破棄
		explicit MyCamera();
		virtual ~MyCamera();
		//アクセサ
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);

		//操作
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//固定のボックスの作成
		void CreateFixedBox();
		//プレートの作成
		void CreatePlate();
		//プレイヤーの作成
		void CreatePlayer();
		//Eggの作成
		void CreateEgg();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void Create()override;
		//操作
		virtual void Update()override;
	};
}
//endof  basedx11
