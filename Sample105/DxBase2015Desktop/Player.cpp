
#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr){}

	//初期化
	void Player::Create(){
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRedit = AddComponent<Rigidbody>();
		//Seek操舵
		AddComponent<SeekSteering>();
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.125f);
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//透明処理
		SetAlphaActive(true);
		//0番目のビューのカメラを得る
		//LookAtCameraである
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
	}

	//移動の向きを得る
	Vector3 Player::GetAngle(){
		Vector3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 && CntlVec[0].fThumbLY != 0){
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetCamera(0);
				//進行方向の向きを計算
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化
				Angle.Normalize();
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void Player::StartShellBall(){
		//砲弾の追加
		auto PtrTrans = GetComponent<Transform>();
		//プレイヤーの向きを得る
		auto PlayerAngle = PtrTrans->GetRotation();
		Vector3 ShellSpeed(sin(PlayerAngle.y), 0, cos(PlayerAngle.y));
		ShellSpeed *= 10.0f;
		//プレイヤーの移動スピードを得る
		auto Velo = GetComponent<Rigidbody>()->GetVelocity();
		Velo.y = 0;
		//移動スピードを加算
		ShellSpeed += Velo;
		//打ち上げの上向きの初速度を追加（値は固定）
		ShellSpeed += Vector3(0.0f,8.0f, 0);
		//グループ内に空きがあればそのオブジェクトを再利用
		//そうでなければ新規に作成
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		for (auto Ptr : ShellVec){
			//Ptrはweak_ptrなので有効性チェックが必要
			if (!Ptr.expired()){
				auto ShellPtr = dynamic_pointer_cast<ShellBall>(Ptr.lock());
				if (ShellPtr){
					if ((!ShellPtr->IsUpdateActive()) && (!ShellPtr->IsDrawActive())){
						ShellPtr->Refresh(PtrTrans->GetPosition(), ShellSpeed);
						return;
					}
				}
			}
		}
		//ここまで来たら空きがなかったことになる
		//砲弾の追加
		auto Sh = GetStage()->AddGameObject<ShellBall>(PtrTrans->GetPosition(), ShellSpeed);
		//グループに追加
		Group->IntoGroup(Sh);
	}

	//更新
	void Player::Update(){
		//向きの取得
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//現在位置を取り出す
		auto Pos = PtrTransform->GetPosition();
		//移動方向を加算。
		//移動方向だけがわかればいいので、
		//Angleは正規化されてて良い
		Pos += Angle;



		//Seek操舵
		auto PtrSeek = GetComponent<SeekSteering>();
		//加算された方向に追いかける
		PtrSeek->SetTargetPosition(Pos);
		//回転の計算
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f){
			//ベクトルをY軸回転に変換
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else{
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);

		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//Aボタンが押された瞬間なら砲弾発射
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				//砲弾の追加
				StartShellBall();
			}
		}

		//文字列をとりだす
		auto PtrString = GetComponent<StringSprite>();
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		wstring str(L"砲弾グループの砲弾数: ");
		str += Util::UintToWStr(ShellVec.size());
		PtrString->SetText(str);


	}



}
//endof  basedx11
