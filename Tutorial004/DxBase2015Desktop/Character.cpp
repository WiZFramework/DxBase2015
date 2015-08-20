#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_Span(2.0f)
	{
	}
	Box::~Box(){}

	//������
	void Box::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		//OBB�̏Փ˔��������
		auto PtrObb = AddComponent<CollisionObb>();
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//�����L��
		SetAlphaActive(true);
	}

	//�X�V
	void Box::Update(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		Pos.x += m_Span * ElapsedTime;
		if (abs(Pos.x) > 10.0f){
			m_Span *= -1.0f;
		}
		PtrTransform->SetPosition(Pos);
	}

	void Box::Update2(){
		auto CollisionPtr = GetComponent<Collision>();
		if (CollisionPtr->GetHitObject()){
			//�����Փ˂����甽�Ε����Ɉړ�
			m_Span *= -1.0f;
		}
	}




}
//endof  basedx11
