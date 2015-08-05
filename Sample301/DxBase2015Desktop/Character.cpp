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
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(5.0f)
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
		m_StateMachine->SetCurrentState(FarState::Instance());
		//初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<SeekObject>());
	}

	//ユーティリティ関数群
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

	//モーションを実装する関数群
	void  SeekObject::SeekStartMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(GetPlayerPosition());

	}
	bool  SeekObject::SeekUpdateMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() <= m_StateChangeSize){
			return true;
		}
		return false;
	}
	void  SeekObject::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekObject::ArriveStartMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(GetPlayerPosition());
	}
	bool  SeekObject::ArriveUpdateMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() > m_StateChangeSize){
			//プレイヤーとの距離が一定以上ならtrue
			return true;
		}
		return false;
	}
	void  SeekObject::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		//Arriveコンポーネントを無効にする
		PtrArrive->SetUpdateActive(false);
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
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance(){
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->SeekStartMoton();
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance(){
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveStartMoton();
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveEndMoton();
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

		//反発を実装する場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

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

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	NumberSprite::NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0){
	}
	NumberSprite::~NumberSprite(){}

	//初期化
	void NumberSprite::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(2.0f, 2.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//スプライトをつける
		auto PtrSprite = AddComponent<Sprite>(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"NUMBER_TX");
		//1メートル当たりのピクセル数
		//以下は640*480ピクセルの場合。横幅20,縦15メートルということ
		PtrSprite->SetPixelParMeter(32.0f);
		//透明処理
		SetAlphaActive(true);
		//中心原点
		PtrSprite->SetCoordinate(Sprite::Coordinate::m_CenterZeroPlusUpY);
		//各数字ごとにUV値を含む頂点データを配列化しておく
		for (size_t i = 0; i < 10; i++){
			float from = ((float)i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//左上頂点
				VertexPositionColorTexture(
				Vector3(-0.5f, 0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 0)
				),
				//右上頂点
				VertexPositionColorTexture(
				Vector3(0.5f, 0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 0)
				),
				//左下頂点
				VertexPositionColorTexture(
				Vector3(-0.5f, -0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 1.0f)
				),
				//右下頂点
				VertexPositionColorTexture(
				Vector3(0.5f, -0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 1.0f)
				),
			};
			m_NumberBurtexVec.push_back(NumVirtex);
		}


	}

	void NumberSprite::Update(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		size_t Num = (size_t)m_TotalTime;
		Num = Num % 10;

		auto PtrSprite = GetComponent<Sprite>();
		PtrSprite->UpdateVirtexBuffer(m_NumberBurtexVec[Num]);
	}


	//--------------------------------------------------------------------------------------
	//	class NumberSquare : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	NumberSquare::NumberSquare(shared_ptr<Stage>& StagePtr, const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare(){}

	//初期化
	void NumberSquare::Create(){
		if (!m_SeekObject.expired()){
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成
			m_SquareMeshResource = CommonMeshResource::CreateSquare(1.0f, true);

			auto DrawComp = AddComponent<BasicPNTDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"NUMBER_TX");
			DrawComp->SetTextureOnlyNoLight(true);
			SetAlphaActive(true);

		}
	}

	//変化
	void NumberSquare::Update(){
		if (!m_SeekObject.expired()){
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetCamera(0);

			Quaternion Qt;
			//向きをビルボードにする
			Qt.Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			//向きをフェイシングにする場合は以下のようにする
			//	Qt.Facing(Pos - PtrCamera->GetEye());
			//向きをフェイシングYにする場合は以下のようにする
			//	Qt.FacingY(Pos - PtrCamera->GetEye());
			//向きをシークオブジェクトと同じにする場合は以下のようにする
			// Qt = SeekTransPtr->GetQuaternion();

			PtrTransform->SetQuaternion(Qt);

			vector<VertexPositionNormalTexture> BackupVec;
			//リソースのバックアップをコピー
			//std::は必要ないが、確認のため記述
			std::copy(
				m_SquareMeshResource->GetBackupVertices().begin(),
				m_SquareMeshResource->GetBackupVertices().end(),
				back_inserter(BackupVec)
				);
			//上記は以下のようにも書ける
			//for (size_t i = 0; i < m_SquareMeshResource->GetBackupVertices().size(); i++){
			//	BackupVec.push_back(m_SquareMeshResource->GetBackupVertices().at(i));
			//}


			//UV値の変更
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//左上頂点
			BackupVec[0].textureCoordinate = Vector2(from, 0);
			//右上頂点
			BackupVec[1].textureCoordinate = Vector2(to, 0);
			//左下頂点
			BackupVec[2].textureCoordinate = Vector2(from, 1.0f);
			//右下頂点
			BackupVec[3].textureCoordinate = Vector2(to, 1.0f);
			//変更した頂点にアップデート
			m_SquareMeshResource->UpdateVirtexBuffer(BackupVec);


		}
	}

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BarSprite::BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_MaxTime(30.0f), m_LastTime(m_MaxTime){
	}
	BarSprite::~BarSprite(){}

	//初期化
	void BarSprite::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//スプライトをつける
		auto PtrSprite = AddComponent<Sprite>(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"BAR_TX");
		//1メートル当たりのピクセル数
		PtrSprite->SetPixelParMeter(32.0f);
		SetAlphaActive(true);

		//原点左下
		PtrSprite->SetCoordinate(Sprite::Coordinate::m_LeftBottomZeroPlusUpY);


		vector<VertexPositionColorTexture> Virtex =
		{
			//左上頂点
			VertexPositionColorTexture(
			Vector3(-0.5f, 4.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 0)
			),
			//右上頂点
			VertexPositionColorTexture(
			Vector3(0.5f, 4.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 0)
			),
			//左下頂点
			VertexPositionColorTexture(
			Vector3(-0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f)
			),
			//右下頂点
			VertexPositionColorTexture(
			Vector3(0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f)
			),
		};
		PtrSprite->UpdateVirtexBuffer(Virtex);




	}

	//変化
	void BarSprite::Update(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_LastTime -= ElapsedTime;
		if (m_LastTime < 0.0f){
			m_LastTime = m_MaxTime;
		}
		float BarVirtexHeight = m_LastTime * 4.0f / m_MaxTime;
		float UVHeight = 1.0f - (m_LastTime / m_MaxTime);
		auto PtrSprite = GetComponent<Sprite>();
		vector<VertexPositionColorTexture> Virtex =
		{
			//左上頂点
			VertexPositionColorTexture(
			Vector3(-0.5f, BarVirtexHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, UVHeight)
			),
			//右上頂点
			VertexPositionColorTexture(
			Vector3(0.5f, BarVirtexHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, UVHeight)
			),
			//左下頂点
			VertexPositionColorTexture(
			Vector3(-0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f)
			),
			//右下頂点
			VertexPositionColorTexture(
			Vector3(0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f)
			),
		};
		PtrSprite->UpdateVirtexBuffer(Virtex);

	}



}
//endof  basedx11
