
#include "stdafx.h"
#include "Project.h"


namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	用途: メニューステージクラス
	//--------------------------------------------------------------------------------------
	//ビューの作成
	void MenuStage::CreateViews(){
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

		//0番目のビューのカメラを得る
		auto PtrCamera = GetCamera(0);
		PtrCamera->SetEye(Vector3(0.0f, 0.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
	}

	//文字列の作成
	void MenuStage::CreateString(){
		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"Bボタンでゲームを開始します");
		Rect2D<float> rect(0, 0, 200, 200);
		rect += Point2D<float>(200,200);
		PtrString->SetTextRect(rect);
	}

	//初期化
	void MenuStage::Create(){
		CreateViews();
		CreateString();
	}

	//操作
	void MenuStage::Update(){
		//コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//Bボタンが押されたら.
		if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
			//イベント送出
			PostEvent(0.0f, GetThis<MenuStage>(), App::GetApp()->GetSceneBase(), L"ToGame");
		}
	}



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

		//0番目のビューのカメラを得る
		auto PtrCamera = GetCamera(0);
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}

	//プレートの作成
	void GameStage::CreatePlate(){
		auto Ptr = AddGameObject<GameObject>();
		auto TrancePtr = Ptr->GetComponent<Transform>();
		TrancePtr->SetScale(200.0f, 200.0f, 1.0f);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(0, 0, 0);
		auto DrawComp = Ptr->AddComponent<SimplePNTDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
	}



	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject(){
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<SeekObject>(v);
		}
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
	//プレイヤーの作成
	void GameStage::CreatePlayer(){
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	//カプセルの作成
	void GameStage::CreateCapsule(){
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
			{ 20.0f, 0, -20.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<CapsuleObject>(v);
		}
	}

	//ヒットする球体の作成
	void GameStage::CreateSphere(){
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, -25.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<SphereObject>(v);
		}
	}



	//衝突するスクエアの作成
	void GameStage::CreateHitTestSquare(){
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"SquareGroup");
		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 1.0f, 1.0f),
			Vector3(0, 0.0f, 0.0f),
			Vector3(0.0f, 0.5f, 10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -15.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(20.0f, 0.4f, -15.0f)
			);
	}


	//初期化
	void GameStage::Create(){
		try{
			//リソースの作成
			CreateResourses();
			//ビュー類を作成する
			CreateViews();
			//プレートの作成
			CreatePlate();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//固定のボックスの作成
			CreateFixedBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//カプセル作成
			CreateCapsule();
			//球体作成
			CreateSphere();
			//衝突するスクエアの作成
			CreateHitTestSquare();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
