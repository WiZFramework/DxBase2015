#include "stdafx.h"
#include "Project.h"

namespace basedx11{

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
