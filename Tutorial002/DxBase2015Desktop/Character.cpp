#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	Box::~Box(){}

	//初期化
	void Box::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//描画コンポーネント
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透過処理
		SetAlphaActive(true);
	}


}
//endof  basedx11
