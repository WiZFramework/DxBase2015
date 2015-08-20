#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	�p�r: ���̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	EggAnime::EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_2head.ssae", L"attack1", true),
		m_Span(10.0f)
	{}
	EggAnime::~EggAnime(){}
	//������
	void EggAnime::Create(){
		//���ƂȂ�I�u�W�F�N�g����A�j���[�V�����I�u�W�F�N�g�ւ̍s��̐ݒ�
		Matrix4X4 mat;
		mat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0, 0, 0),
			Vector3(0, 0, 0)
			);
		SetToAnimeMatrix(mat);

		//�ʒu�̏�����
		auto PtrT = GetComponent<Transform>();
		PtrT->SetPosition(Vector3(0, -25.0f, 0.2f));
		//�e�N���X��Create��K���Ă�
		SS5ssae::Create();
		//�l�͕b������̃t���[����
		SetFps(30.0f);
		//�X�v���C�g�̃T�C�Y�i1���[�g��������̃s�N�Z���j
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
		//�ړ������Ɍ�����ς���
		if (m_Span < 0){
			PtrT->SetScale(1, 1, 1);
		}
		else{
			PtrT->SetScale(-1, 1, 1);
		}
		//�A�j���[�V�������X�V����
		UpdateAnimeTime(ElapsedTime);
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

		//��������������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}






}
//endof  basedx11
