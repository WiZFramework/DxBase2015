
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


	//プレイヤーの作成
	void GameStage::CreatePlayer(){
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	//カプセルの作成
	void GameStage::CreateCapsule(){
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"CapsuleGroup");

		//配列の初期化
		vector<Vector3> Vec = {
			{ 10.0f, 0, 10.0f },
			{ 10.0f, 0, -10.0f },
			{ -10.0f, 0, 10.0f },
			{ -10.0f, 0, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<CapsuleObject>(v);
		}
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
			//カプセル作成
			CreateCapsule();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
