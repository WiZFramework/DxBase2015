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
		m_Span(1.0f),
		m_StartPos(StartPos)
	{
	}
	Box::~Box(){}

	//������
	void Box::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������
		SetAlphaActive(true);
	}

	//�X�V
	void Box::Update(){
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		//�ʒu�̎擾
		auto Pos = PtrTransform->GetPosition();
		//�l��ω�������
		Pos.x += m_Span * ElapsedTime;
		if (abs(Pos.x) > 3.0f){
			//�������E3.0�𒴂����甽�]
			m_Span *= -1.0f;
		}
		//�V�����ʒu��ݒ�
		PtrTransform->SetPosition(Pos);
	}



}
//endof  basedx11
