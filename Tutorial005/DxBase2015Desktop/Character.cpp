#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	�p�r: �����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveObject::MoveObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, const Vector3& StartVelocity, bool Type) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StartVelocity(StartVelocity),
		m_Type(Type)
	{
	}
	MoveObject::~MoveObject(){}

	//������
	void MoveObject::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		if (m_Type){
			//Sphere�̏Փ˔��������
			auto PtrSphere = AddComponent<CollisionSphere>();
		}
		else{
			//OBB�̏Փ˔��������
			auto PtrObb = AddComponent<CollisionObb>();
		}

		//�����v�Z������
		auto PtrRigit = AddComponent<Rigidbody>();
		PtrRigit->SetVelocity(m_StartVelocity);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		if (m_Type){
			ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		}
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		if (m_Type){
			PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		}
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//�����L��
		SetAlphaActive(true);
	}

	//�X�V
	void MoveObject::Update2(){
		auto PtrRigit = GetComponent<Rigidbody>();
		auto Velo = PtrRigit->GetVelocity();
		//���������ő��xy���O�ȊO�ɂȂ�ꍇ������̂Œ���
		Velo.y = 0;
		PtrRigit->SetVelocity(Velo);
	}

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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

	//������
	void FixedBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetOwnShadowActive(true);
	}



}
//endof  basedx11
