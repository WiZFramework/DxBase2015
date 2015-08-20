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
		m_Span(3.0f)
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
		//�����L��
		SetAlphaActive(true);
		//���Έړ��A�N�V����������
		auto PtrMoveBy = AddComponent<MoveBy>();
		PtrMoveBy->SetParams(2.0f, Vector3(m_Span, 0, 0));
		//�A�N�V�������s�J�n
		PtrMoveBy->Run();
	}

	//�X�V
	void Box::Update2(){
		//���Έړ��A�N�V���������o��
		auto PtrMoveBy = GetComponent<MoveBy>();
		if (PtrMoveBy->IsArrived()){
			//�������Ă�����ړ��������t�]�����āAMoveBy��������
			m_Span *= -1.0f;
			PtrMoveBy->SetParams(2.0f, Vector3(m_Span, 0, 0));
			//�A�N�V�������s�J�n
			PtrMoveBy->Run();
		}
	}




}
//endof  basedx11
