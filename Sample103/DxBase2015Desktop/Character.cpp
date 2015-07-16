#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionObject::ActionObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos){
	}
	ActionObject::~ActionObject(){}


	//初期化
	void ActionObject::Create(){
		//Transformだけは追加しなくても取得できる
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(m_StartPos);
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);

		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();


		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(0, 0.0f, -2.0f));
		PtrAction->AddMoveBy(2.0f, Vector3(-4.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(0, 0, 2.0f));
		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//テクスチャの登録
		PtrDraw->SetTextureResource(L"TRACE_TX");


		//透過処理する
		SetAlphaActive(true);
	}

	void ActionObject::Update2(){
		//何かと衝突していたら
		auto ColPtr = GetComponent<CollisionSphere>();
		if (ColPtr->GetHitObject()){
			auto PtrTrans = GetComponent<Transform>();
			auto Pos = PtrTrans->GetPosition();

			auto OtherPtr = ColPtr->GetHitObject()->GetComponent<Transform>();
			auto OtherPos = OtherPtr->GetPosition();

			auto Span  = Pos - OtherPos;
			auto Span2 = Pos - OtherPos;

			//衝突してないと思われる位置まで移動
			Span.Normalize();
			Span.y = 0.0f;
			Span *= 0.125f;
			Pos += Span;
			Pos.y = 0.125f;
			PtrTrans->SetPosition(Pos);
			//衝突後移動方向の計算
			Span2.Normalize();
			Span2 *= 3.0f;
			Span2.y = 0.0f;


			//アクションの取得
			auto PtrAction = GetComponent<Action>();
			//現在のすべてのアクションをクリア
			PtrAction->AllActionClear();
			//アクションの設定し直し
			PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
			PtrAction->AddMoveBy(1.0f, Span2);
			//ループする
			PtrAction->SetLooped(true);
			//アクション開始
			PtrAction->Run();

		}
	}


}
//endof  basedx11
