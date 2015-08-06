#include "stdafx.h"
#include "Project.h"

namespace basedx11{

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

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
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

	MoveBox::~MoveBox(){}

	//������
	void MoveBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);


		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSprite::NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0){
	}
	NumberSprite::~NumberSprite(){}

	//������
	void NumberSprite::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(2.0f, 2.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		auto PtrSprite = AddComponent<Sprite>(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"NUMBER_TX");
		//1���[�g��������̃s�N�Z����
		//�ȉ���640*480�s�N�Z���̏ꍇ�B����20,�c15���[�g���Ƃ�������
		PtrSprite->SetPixelParMeter(32.0f);
		//��������
		SetAlphaActive(true);
		//���S���_
		PtrSprite->SetCoordinate(Sprite::Coordinate::m_CenterZeroPlusUpY);
		//�e�������Ƃ�UV�l���܂ޒ��_�f�[�^��z�񉻂��Ă���
		for (size_t i = 0; i < 10; i++){
			float from = ((float)i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//���㒸�_
				VertexPositionColorTexture(
				Vector3(-0.5f, 0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 0)
				),
				//�E�㒸�_
				VertexPositionColorTexture(
				Vector3(0.5f, 0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 0)
				),
				//�������_
				VertexPositionColorTexture(
				Vector3(-0.5f, -0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 1.0f)
				),
				//�E�����_
				VertexPositionColorTexture(
				Vector3(0.5f, -0.5f, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 1.0f)
				),
			};
			m_NumberBurtexVec.push_back(NumVirtex);
		}


	}

	void NumberSprite::Update(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		size_t Num = (size_t)m_TotalTime;
		Num = Num % 10;

		auto PtrSprite = GetComponent<Sprite>();
		PtrSprite->UpdateVirtexBuffer(m_NumberBurtexVec[Num]);
	}

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BarSprite::BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_MaxTime(30.0f), m_LastTime(m_MaxTime){
	}
	BarSprite::~BarSprite(){}

	//������
	void BarSprite::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		auto PtrSprite = AddComponent<Sprite>(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"BAR_TX");
		//1���[�g��������̃s�N�Z����
		PtrSprite->SetPixelParMeter(32.0f);
		SetAlphaActive(true);

		//���_����
		PtrSprite->SetCoordinate(Sprite::Coordinate::m_LeftBottomZeroPlusUpY);


		vector<VertexPositionColorTexture> Virtex =
		{
			//���㒸�_
			VertexPositionColorTexture(
			Vector3(-0.5f, 4.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 0)
			),
			//�E�㒸�_
			VertexPositionColorTexture(
			Vector3(0.5f, 4.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 0)
			),
			//�������_
			VertexPositionColorTexture(
			Vector3(-0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f)
			),
			//�E�����_
			VertexPositionColorTexture(
			Vector3(0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f)
			),
		};
		PtrSprite->UpdateVirtexBuffer(Virtex);




	}

	//�ω�
	void BarSprite::Update(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_LastTime -= ElapsedTime;
		if (m_LastTime < 0.0f){
			m_LastTime = m_MaxTime;
		}
		float BarVirtexHeight = m_LastTime * 4.0f / m_MaxTime;
		float UVHeight = 1.0f - (m_LastTime / m_MaxTime);
		auto PtrSprite = GetComponent<Sprite>();
		vector<VertexPositionColorTexture> Virtex =
		{
			//���㒸�_
			VertexPositionColorTexture(
			Vector3(-0.5f, BarVirtexHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, UVHeight)
			),
			//�E�㒸�_
			VertexPositionColorTexture(
			Vector3(0.5f, BarVirtexHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, UVHeight)
			),
			//�������_
			VertexPositionColorTexture(
			Vector3(-0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f)
			),
			//�E�����_
			VertexPositionColorTexture(
			Vector3(0.5f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f)
			),
		};
		PtrSprite->UpdateVirtexBuffer(Virtex);

	}




}
//endof  basedx11
