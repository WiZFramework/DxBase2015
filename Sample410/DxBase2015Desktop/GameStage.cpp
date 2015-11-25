
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	//リソースの作成
	void GameStage::CreateResourses(){
		wstring strTexture = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);


		
	}

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
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
	}

	//プレートの作成
	void GameStage::CreatePlate(){
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		//変化コンポーネント（Transform）の取得
		auto TrancePtr = Ptr->GetComponent<Transform>();
		//スケーリングの設定
		TrancePtr->SetScale(200.0f, 200.0f, 1.0f);
		//回転の設定
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		TrancePtr->SetQuaternion(Qt);
		//位置の設定
		TrancePtr->SetPosition(0, 0, 0);
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<SimplePNTDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);
	}



	//描画マネージャの作成
	void GameStage::CreateDrawBoxManager(){
		auto Ptr = AddGameObject<DrawBoxManager>(1000,L"DEFAULT_CUBE", L"SKY_TX");
		//シェアオブジェクトにマネージャーを追加
		SetSharedGameObject(L"DrawBoxManager", Ptr);

		Ptr = AddGameObject<DrawBoxManager>(2000, L"DEFAULT_DODECAHEDRON", L"WALL_TX");
		//シェアオブジェクトに分割用のマネージャーを追加
		SetSharedGameObject(L"DrawSplitBoxManager", Ptr);

	}

	//ボックスの作成
	void GameStage::CreateFixedBox(){
		for (int i = 0; i < 30; i++){
			for (int j = 0; j < 30; j++){
				float x = (float)(i - 15) / 2.0f;
				float z = (float)(j + 2) / 2.0f;
				AddGameObject<FixedBox>(Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 0, 0), Vector3(x, 0.25f, z));
			}
		}
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer(){
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	//初期化
	void GameStage::Create(){
		try{
			//リソースの作成
			CreateResourses();
			//ビュー類を作成する
			CreateViews();
			//プレートを作成する
			CreatePlate();
			//ボックスの作成
			CreateFixedBox();
			//描画マネージャの作成
			CreateDrawBoxManager();
			//プレーヤーの作成
			CreatePlayer();
			//影を細かくする(デフォルト64)
			Shadowmap::SetViewWidth(16.0f);
			Shadowmap::SetViewHeight(16.0f);
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
