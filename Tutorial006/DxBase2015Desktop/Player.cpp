
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
		//	PtrDraw->SetOwnShadowActive(true);

		//透明処理
		SetAlphaActive(true);
		//0番目のビューのカメラを得る
		//LookAtCameraである
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//最初のステートをDefaultStateに設定
		m_StateMachine->SetCurrentState(DefaultState::Instance());
		//DefaultStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<Player>());
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

	void Player::StartJump(){
		auto PtrTrans = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		if (PtrTrans->GetParent()){
			//親がいたら、アクションコンポーネントの移動アクションを探す
			//移動ボックスに乗っている場合、その慣性をジャンプに加算する
			auto ActionPtr = PtrTrans->GetParent()->GetComponent<Action>(false);
			if (ActionPtr){
				JumpVec += ActionPtr->GetVelocity();
			}
		}
		PtrGravity->StartJump(JumpVec);
	}


	//更新
	void Player::Update(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();

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



	}

	void Player::Update2(){
		auto ColPtr = GetComponent<CollisionSphere>();
		if (ColPtr->GetHitObject() && GetStateMachine()->GetCurrentState() == JumpState::Instance()){
			GetStateMachine()->ChangeState(DefaultState::Instance());

		}
	}


	void Player::Update3(){

		auto PtrTransform = GetComponent<Transform>();
		auto Coll = GetComponent<CollisionSphere>();
		auto Sp = Coll->GetSphere();
		//平面との交差を判定
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		for (auto Ptr : Group->GetGroupVector()){
			if (!Ptr.expired()){
				auto SquarePtr = dynamic_pointer_cast< HitTestSquare>(Ptr.lock());
				if (SquarePtr){
					Vector3 RetVec, Normal;
					if (SquarePtr->HitTestSphere(Sp, RetVec, Normal)){
						//ヒットしたので位置をヒット位置に修正する
						Vector3 SetPos = RetVec + -Normal * Sp.m_Radius;
						PtrTransform->SetPosition(SetPos);
						auto PtrGrav = GetComponent<Gravity>();
						//蓄積した落下の加速度を0にする
						PtrGrav->SetGravityVelocityZero();
						auto PtrRidit = GetComponent<Rigidbody>();
						//反発
						Vector3 Refalct = Vector3EX::Reflect(PtrRidit->GetVelocity(), Normal);
						PtrRidit->SetVelocity(Refalct);

					}
				}
			}
		}

		auto Pos = GetComponent<Transform>()->GetWorldMatrix().PosInMatrix();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring GravStr(L"Gravity:\t");
		auto Grav = GetComponent<Gravity>()->GetGravity();
		GravStr += L"X=" + Util::FloatToWStr(Grav.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(Grav.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(Grav.z, 6, Util::FloatModify::Fixed) + L"\n";


		wstring GravityStr(L"GravityVelocity:\t");
		auto GravityVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravityStr += L"X=" + Util::FloatToWStr(GravityVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Y=" + Util::FloatToWStr(GravityVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Z=" + Util::FloatToWStr(GravityVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring InvGravityStr(L"InvGravity: ");
		auto InvGravity = GetComponent<Gravity>()->GetInvGravity();
		InvGravityStr += L"X=" + Util::FloatToWStr(InvGravity.x, 6, Util::FloatModify::Fixed) + L", ";
		InvGravityStr += L"Y=" + Util::FloatToWStr(InvGravity.y, 6, Util::FloatModify::Fixed) + L", ";
		InvGravityStr += L"Z=" + Util::FloatToWStr(InvGravity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring OnObjectStr(L"OnObject: ");
		auto OnObject = GetComponent<Gravity>()->GetOnObject();
		if (OnObject){
			OnObjectStr += Util::UintToWStr((UINT)OnObject.get()) + L"\n";
		}
		else{
			OnObjectStr += L"NULL\n";
		}
		wstring statestr = L"JUMP: ";
		if (m_StateMachine->GetCurrentState() == DefaultState::Instance()){
			statestr = L"DEFAULT\n";
		}

		wstring str = PositionStr + RididStr + GravStr + GravityStr + InvGravityStr + OnObjectStr + statestr;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);


	}



	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	用途: 通常移動
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	static DefaultState* Instance();
	//	用途: インスタンスの取得
	//	戻り値: DefaultStateのインスタンス
	//--------------------------------------------------------------------------------------
	shared_ptr<DefaultState> DefaultState::Instance(){
		static shared_ptr<DefaultState> instance;
		if (!instance){
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//--------------------------------------------------------------------------------------
	//	virtual void Execute(
	//	const shared_ptr<Player>& Obj		//ステートを保持するオブジェクト
	//	) = 0;
	//	用途: Updateのときに実行される
	//	戻り値: なし（純粋仮想関数）
	//--------------------------------------------------------------------------------------
	void DefaultState::Execute(const shared_ptr<Player>& Obj){
		//コントローラの取得
		//コントローラはApp内なのでステートから呼んでも問題ない。
		//
		//ただし、ステートクラスにはメンバ変数は設けないこと！（シングルトンのため）
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//Aボタンが押された瞬間ならステート変更
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				Obj->GetStateMachine()->ChangeState(JumpState::Instance());
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	用途: ジャンプ状態
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	static shared_ptr<JumpState> Instance();
	//	用途: インスタンスの取得
	//	戻り値: DefaultStateのインスタンス
	//--------------------------------------------------------------------------------------
	shared_ptr<JumpState> JumpState::Instance(){
		static shared_ptr<JumpState> instance;
		if (!instance){
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//--------------------------------------------------------------------------------------
	//	virtual void JumpState::Enter(
	//	const shared_ptr<Player>& Obj	//ステートを保持するオブジェクト
	//	) = 0;
	//	用途: ステートに入ったときに実行される
	//	戻り値: なし（純粋仮想関数）
	//--------------------------------------------------------------------------------------
	void JumpState::Enter(const shared_ptr<Player>& Obj){
		Obj->StartJump();
	}
	//--------------------------------------------------------------------------------------
	//	virtual void JumpState::Execute(
	//	const shared_ptr<Player>& Obj		//ステートを保持するオブジェクト
	//	) = 0;
	//	用途: Updateのときに実行される
	//	戻り値: なし（純粋仮想関数）
	//--------------------------------------------------------------------------------------
	void JumpState::Execute(const shared_ptr<Player>& Obj){
		auto PtrTransform = Obj->GetComponent<Transform>();
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0 || PtrTransform->GetParent()){
			//通常状態に戻る
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}


}
//endof  basedx11
