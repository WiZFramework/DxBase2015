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

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.5f);

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
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			auto PtrGravity = GetComponent<Gravity>();
			if (PtrGravity->GetGravityVelocity().Length() <= 0){
				//�n�ʂɂ���Ƃ��̂݃W�����v�ł���
				//A�{�^���������ꂽ�u�ԂȂ珬�W�����v
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
					//�W�����v�̓V���[�g�^�C�v
					Jump<ShortType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//B�{�^���������ꂽ�u�ԂȂ璆�W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
					//�W�����v�̓~�f�B�A���^�C�v
					Jump<MediumType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//Y�{�^���������ꂽ�u�ԂȂ��W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y){
					//�W�����v�̓����O�^�C�v
					Jump<LongType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//X�{�^���������ꂽ�u�ԂȂ���W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X){
					//�W�����v�̓����O�����O�^�C�v
					Jump<LongLongType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_YRot(0),
		m_RotationSpeed(0.0f)
	{}
	RollingTorus::~RollingTorus(){}
	//������
	void RollingTorus::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.5f);

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
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			auto PtrGravity = GetComponent<Gravity>();
			if (PtrGravity->GetGravityVelocity().Length() <= 0){
				//�n�ʂɂ���Ƃ��̂݃W�����v�ł���
				//A�{�^���������ꂽ�u�ԂȂ珬�W�����v
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
					Jump<ShortType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 5.0f;
				}
				//B�{�^���������ꂽ�u�ԂȂ璆�W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
					Jump<MediumType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 10.0f;
				}
				//Y�{�^���������ꂽ�u�ԂȂ��W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y){
					Jump<LongType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 15.0f;
				}
				//X�{�^���������ꂽ�u�ԂȂ���W�����v
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X){
					Jump<LongLongType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 20.0f;
				}
			}
		}

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
