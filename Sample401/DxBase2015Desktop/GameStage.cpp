
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	//ビュー類の作成
	void GameStage::CreateViews(){
		//最初にデフォルトのレンダリングターゲット類を作成する
		CreateDefaultRenderTargets();
		//マルチビューコンポーネントの取得
		auto PtrMultiView = GetComponent<MultiView>();
		//マルチビューにビューの追加
		auto PtrView = PtrMultiView->AddView();
		//ビューの矩形を設定（ゲームサイズ全体）
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		//最初のビューにパラメータの設定
		PtrView->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.0f, 1.0f);
	}

	//三角形オブジェクトの作成
	void GameStage::CreateTriangleObject(){
		//三角形オブジェクトを作成する
		AddGameObject<TriangleObject>();
	}

	//初期化
	void GameStage::Create(){
		try{
			//ビュー類を作成する
			CreateViews();
			//三角形オブジェクトの作成
			CreateTriangleObject();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
