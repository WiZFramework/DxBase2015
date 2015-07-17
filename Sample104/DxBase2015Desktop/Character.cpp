#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
		GameObject(StagePtr), m_StartPos(StartPos), m_YRot(0), m_RotationSpeed(20.0f)
	{}
	RollingTorus::~RollingTorus(){}
	//������
	void RollingTorus::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);
		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_TORUS");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_TORUS");

		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));


	}
	void RollingTorus::Update(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_YRot += (m_RotationSpeed * ElapsedTime);
		if (m_RotationSpeed > 0){
			m_RotationSpeed -= 0.05f;
		}
		auto Ptr = GetComponent<Transform>();
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);

	}


}
//endof  basedx11
