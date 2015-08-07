
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

	//固定のボックスの作成
	void GameStage::CreateFixedBox(){
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//上下移動しているボックスの作成
	void GameStage::CreateMoveBox(){
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//透過するボックスの作成
	void GameStage::CreateTraceBox(){
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(4.0f, 0.5f, 4.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.0f, 2.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<TraceBox>(v[0], v[1], v[2]);
		}
	}
	//透過する球体の作成
	void GameStage::CreateTraceSphere(){
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(2.0f, 2.0f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.0f, 2.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<TraceSphere>(v[0], v[1], v[2]);
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
			//固定のボックスの作成
			CreateFixedBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//透過するボックスの作成
			CreateTraceBox();
			//透過する球体の作成
			CreateTraceSphere();
			//プレーヤーの作成
			CreatePlayer();


		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
