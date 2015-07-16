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
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();

		if (Key.m_bPushKeyTbl['F'] || Key.m_bPushKeyTbl[VK_LBUTTON]){
			Pos.x -=  ElapsedTime;
		}
		else if (Key.m_bPushKeyTbl['G'] || Key.m_bPushKeyTbl[VK_RBUTTON]){
			Pos.x += ElapsedTime;
		}

		//�����A�L�[���������u�Ԃ��}�E�X���N���b�N�����u�Ԃ��Ƃ��Ă���ꍇ�́A
		//��L���R�����g�ɂ��ȉ���L���ɂ���
		/*
		if (Key.m_bPressedKeyTbl['F'] || Key.m_bPressedKeyTbl[VK_LBUTTON]){
			Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bPressedKeyTbl['G'] || Key.m_bPressedKeyTbl[VK_RBUTTON]){
			Pos.x += ElapsedTime * 5.0f;
		}
		*/

		//�����A�L�[�𗣂����u�Ԃ��}�E�X���A�b�v�����u�Ԃ��Ƃ��Ă���ꍇ�́A
		//��L���R�����g�ɂ��ȉ���L���ɂ���
		/*
		if (Key.m_bUpKeyTbl['F'] || Key.m_bUpKeyTbl[VK_LBUTTON]){
			Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bUpKeyTbl['G'] || Key.m_bUpKeyTbl[VK_RBUTTON]){
			Pos.x += ElapsedTime * 5.0f;
		}
		*/

		PtrTransform->SetPosition(Pos);
	}



}
//endof  basedx11
