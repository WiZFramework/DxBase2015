#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	用途: 卵のアニメーション
	//--------------------------------------------------------------------------------------
	//構築と消滅
	EggAnime::EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_2head.ssae", L"attack1", true),
		m_Span(10.0f)
	{}
	EggAnime::~EggAnime(){}
	//初期化
	void EggAnime::Create(){
		//元となるオブジェクトからアニメーションオブジェクトへの行列の設定
		Matrix4X4 mat;
		mat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0, 0, 0),
			Vector3(0, 0, 0)
			);
		SetToAnimeMatrix(mat);

		//位置の初期化
		auto PtrT = GetComponent<Transform>();
		PtrT->SetPosition(Vector3(0, -25.0f, 0.2f));
		//親クラスのCreateを必ず呼ぶ
		SS5ssae::Create();
		//値は秒あたりのフレーム数
		SetFps(30.0f);
		//スプライトのサイズ（1メートル当たりのピクセル）
		SetSpritePixelParMeter(8.0f);
	}
	void EggAnime::Update(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrT = GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		Pos.x += (m_Span * ElapsedTime);
		if (abs(Pos.x) > 40.0f){
			m_Span *= -1.0f;
		}
		PtrT->SetPosition(Pos);
		//移動方向に向きを変える
		if (m_Span < 0){
			PtrT->SetScale(1, 1, 1);
		}
		else{
			PtrT->SetScale(-1, 1, 1);
		}
		//アニメーションを更新する
		UpdateAnimeTime(ElapsedTime);
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






}
//endof  basedx11
