#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y)
	{
	}
	SeekObject::~SeekObject(){}

	//初期化
	void SeekObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//最初のステートをSeekFarStateに設定
		m_StateMachine->SetCurrentState(SeekFarState::Instance());
		//初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<SeekObject>());
	}
	//アクセサ
	shared_ptr< StateMachine<SeekObject> > SeekObject::GetStateMachine()const {
		return m_StateMachine;
	}
	//ステート関数群
	Vector3 SeekObject::GetPlayerPosition() const{
		//もしプレイヤーが初期化化されてない場合には、Vector3(0,m_BaseY,0)を返す
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const{
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}
	bool SeekObject::SetIfNearVelocity(){
		auto PtrRighd = GetComponent<Rigidbody>();
		if (PtrRighd->GetVelocity().Length() < 0.2f){
			//速度が0.2f未満なら、速度を0にする
			PtrRighd->SetVelocity(Vector3(0, 0, 0));
			return true;
		}
		else{
			return false;
		}
	}


	//操作
	void SeekObject::Update(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}
	void SeekObject::Update3(){
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f){
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
		//常にyはm_BaseY
		auto Pos = PtrTransform->GetPosition();
		Pos.y = m_BaseY;
		PtrTransform->SetPosition(Pos);
	}
	//--------------------------------------------------------------------------------------
	//	class SeekFarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<SeekFarState> test;

	shared_ptr<SeekFarState> SeekFarState::Instance(){
		static shared_ptr<SeekFarState> instance(new SeekFarState);
		return instance;
	}
	void SeekFarState::Enter(const shared_ptr<SeekObject>& Obj){
		//5メートルより離れている
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void SeekFarState::Execute(const shared_ptr<SeekObject>& Obj){
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() <= 5.0f){
			Obj->GetStateMachine()->ChangeState(ArriveNearState::Instance());
			return;
			//本来ここでreturnはいらないが、
			//ChangeState()呼び出し後はすぐにステートを出ることを強調した
		}
	}
	void SeekFarState::Exit(const shared_ptr<SeekObject>& Obj){
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	//--------------------------------------------------------------------------------------
	//	class ArriveNearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<ArriveNearState> ArriveNearState::Instance(){
		static shared_ptr<ArriveNearState> instance(new ArriveNearState);
		return instance;
	}
	void ArriveNearState::Enter(const shared_ptr<SeekObject>& Obj){
		//5メートル以下に近づいている
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void ArriveNearState::Execute(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->SetIfNearVelocity()){
			//速度を止めたらArriveも無効にする
			//そのためSeekFarStateに移らないと動き出さない
			PtrArrive->SetUpdateActive(false);
		}
		if (Obj->GetPlayerLength() > 5.0f){
			Obj->GetStateMachine()->ChangeState(SeekFarState::Instance());
			return;
			//本来ここでreturnはいらないが、
			//ChangeState()呼び出し後はすぐにステートを出ることを強調した
		}
	}
	void ArriveNearState::Exit(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
	}



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//初期化
	void FixedBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//		auto PtrDraw = AddComponent<SimplePNTDraw>();
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox(){}

	//初期化
	void MoveBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);


		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();



		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}



	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	CapsuleObject::~CapsuleObject(){}

	//初期化
	void CapsuleObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Capsuleの衝突判定をつける
		auto CollPtr = AddComponent<CollisionCapsule>();
		CollPtr->SetFixed(true);
		//コリジョンのワイアフレーム表示
		CollPtr->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}


	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject(){}
	//初期化
	void SphereObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphereの衝突判定をつける
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//コリジョンのワイアフレーム表示
		CollPtr->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");
	}


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	用途: 球と衝突判定するスクエア
	//--------------------------------------------------------------------------------------
	//構築と破棄
	HitTestSquare::HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos)
	{}
	HitTestSquare::~HitTestSquare(){}

	void HitTestSquare::Create(){
		auto TrancePtr = GetComponent<Transform>();
		TrancePtr->SetScale(m_StartScale);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(m_StartRotation);
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(m_StartPos);
		auto DrawComp = AddComponent<BasicPNTDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SQUARE");


		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<HitTestSquare>());

	}

	PLANE HitTestSquare::GetPLANE() const{
		auto TrasnsPtr = GetComponent<Transform>();
		//表面上に3つの点を使ってPLANEを作成
		//1つ目の点は中心
		Vector3 Point0 = TrasnsPtr->GetPosition();
		//2つ目は-0.5,-0.5,0の点をワールド変換したもの
		Vector3 Point1(-0.5f, -0.5f, 0);
		Point1.Transform(TrasnsPtr->GetWorldMatrix());
		//3つ目は0.5,-0.5,0の点をワールド変換したもの
		Vector3 Point2(0.5f, -0.5f, 0);
		Point2.Transform(TrasnsPtr->GetWorldMatrix());
		//3点を使って面を作成
		PLANE ret(Point0, Point1, Point2);
		return ret;
	}

	COLRECT HitTestSquare::GetCOLRECT() const{
		auto TrasnsPtr = GetComponent<Transform>();
		COLRECT rect(1.0f, 1.0f, TrasnsPtr->GetWorldMatrix());
		return rect;
	}


	//点との最近接点を返す
	void HitTestSquare::ClosestPtPoint(const Vector3& Point, Vector3& Ret){
		COLRECT rect = GetCOLRECT();
		Vector3 d = Point - rect.m_Center;
		Ret = rect.m_Center;
		for (int i = 0; i < 2; i++){
			float dist = Vector3EX::Dot(d, rect.m_Rot[i]);
			if (dist > rect.m_UVec[i]){
				dist = rect.m_UVec[i];
			}
			if (dist < -rect.m_UVec[i]){
				dist = -rect.m_UVec[i];
			}
			Ret += rect.m_Rot[i] * dist;
		}
	}


	//点との距離を返す（戻り値がマイナスなら裏側）
	float HitTestSquare::GetDistPointPlane(const Vector3& Point) const{
		PLANE pl = GetPLANE();
		return (Vector3EX::Dot(Point, pl.m_Normal) - pl.m_DotValue) / Vector3EX::Dot(pl.m_Normal, pl.m_Normal);
	}

	//ヒットテストをして最近接点と、Squareの法線を返す
	bool HitTestSquare::HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal){
		PLANE pl = GetPLANE();
		Normal = pl.m_Normal;
		//四角形との最近接点を得る
		ClosestPtPoint(Sp.m_Center, RetVec);
		//最近接点が半径以下なら衝突している
		if (Vector3EX::Length(Sp.m_Center - RetVec) <= Sp.m_Radius){
			return true;
		}
		return false;
	}



}
//endof  basedx11
