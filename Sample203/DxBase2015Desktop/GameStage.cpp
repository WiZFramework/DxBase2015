
#include "stdafx.h"
#include "Project.h"


namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class MyCamera : public Camera ;
	//	�p�r: LookAt�J���������C�u��������O��������
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MyCamera::MyCamera() :
		Camera(),
		m_ToTargetLerp(1.0f)
	{
	}
	MyCamera::~MyCamera(){}
	//�A�N�Z�T
	shared_ptr<GameObject> MyCamera::GetTargetObject() const{
		if (!m_TargetObject.expired()){
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj){
		m_TargetObject = Obj;
	}

	float MyCamera::GetToTargetLerp() const{
		return m_ToTargetLerp;
	}
	void MyCamera::SetToTargetLerp(float f){
		m_ToTargetLerp = f;
	}

	void MyCamera::Update(){
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (CntlVec[0].bConnected){
			Vector3 NewAt(0, 0, 0);
			auto TargetPtr = GetTargetObject();
			if (TargetPtr){
				//�ڎw�������ꏊ
				Vector3 ToAt = TargetPtr->GetComponent<Transform>()->GetPosition();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);
			}
			//�X�e�b�v1�A�����_�ƈʒu�̕ύX
			Vector3 Span = GetAt() - GetEye();
			Vector3 NewEye = NewAt - Span;
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v�Q�A�Y�[���̕ύX
			//�J�����ʒu�ƒ����_�̊Ԃ̃x�N�g�����Z�o
			Span = GetAt() - GetEye();
			//���K��
			Span.Normalize();
			//�ω��l�̌���
			Span = Span * ElapsedTime * 10.0f;

			Vector3 NewArm = GetAt() - GetEye();
			//D�p�b�h��
			//�J����������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN){
				//�J�����ʒu������
				NewEye = NewEye - Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() > (GetFar() * 0.1f)){
					NewEye = NewEye + Span;
					NewArm = NewAt - NewEye;
				}
			}
			//D�p�b�h��
			//�J���������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP){
				//�J�����ʒu�����
				NewEye = NewEye + Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() < GetNear() * 2.0f){
					NewEye = NewEye - Span;
					NewArm = NewAt - NewEye;
				}
			}
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v3�p�x�̕ύX
			//���݂�At��Eye�̊p�x�𓾂�
			Vector3 ArmInv = GetEye() - GetAt();
			//�E�X�e�B�b�NX����
			FLOAT AngleY = 0;
			//�E�X�e�B�b�NY����
			FLOAT AngleX = 0;
			FLOAT AngleZ = 0;
			if (CntlVec[0].fThumbRX != 0){
				AngleY = -CntlVec[0].fThumbRX * ElapsedTime;
			}
			if (CntlVec[0].fThumbRY != 0){
				AngleX = CntlVec[0].fThumbRY * ElapsedTime;
				AngleZ = CntlVec[0].fThumbRY * ElapsedTime;
			}
			if (ArmInv.z > 0){
				AngleX *= -1.0f;
			}
			if (ArmInv.x < 0){
				AngleZ *= -1.0f;
			}
			Quaternion QtSpan(AngleX, AngleY, AngleZ);
			QtSpan.Normalize();
			//��]��v�Z�̍s����쐬
			Matrix4X4 Mat, Mat2;
			Mat.STRTransformation(
				Vector3(1.0f, 1.0f, 1.0f),
				ArmInv,
				QtSpan);
			Mat2.TranslationFromVector(GetAt());
			Mat *= Mat2;
			NewEye = Mat.PosInMatrix();
			if (NewEye.y < 0.5f){
				NewEye.y = 0.5f;
			}
			//�J���������ȏ�A�ォ�王���ɂȂ�Ȃ悤�ɒ���
			ArmInv = NewEye - GetAt();
			ArmInv.Normalize();
			float y2 = ArmInv.y * ArmInv.y;
			float x2 = ArmInv.x * ArmInv.x;
			float z2 = ArmInv.z * ArmInv.z;
			if (y2 <= (x2 + z2)){
				SetEye(NewEye);
			}

		}
		Camera::Update();
	}


	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//���\�[�X�̍쐬
	void GameStage::CreateResourses(){
		wstring strTexture = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}

	//�r���[�ނ̍쐬
	void GameStage::CreateViews(){
		//�ŏ��Ƀf�t�H���g�̃����_�����O�^�[�Q�b�g�ނ��쐬����
		CreateDefaultRenderTargets();
		//�}���`�r���[�R���|�[�l���g�̎擾
		auto PtrMultiView = GetComponent<MultiView>();
		//�}���`�r���[�Ƀr���[�̒ǉ�
		auto PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�i�Q�[���T�C�Y�S�́j
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		//�ŏ��̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<MyCamera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.0f, 1.0f);
		//0�Ԗڂ̃r���[�̃J�����𓾂�
		auto PtrCamera = GetCamera(0);
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}

	//�v���[�g�̍쐬
	void GameStage::CreatePlate(){
		auto Ptr = AddGameObject<GameObject>();
		auto TrancePtr = Ptr->GetComponent<Transform>();
		TrancePtr->SetScale(200.0f, 200.0f, 1.0f);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(0, 0, 0);
		auto DrawComp = Ptr->AddComponent<SimplePNTDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
	}



	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject(){
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<SeekObject>(v);
		}
	}

	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox(){
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox(){
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}
	//�v���C���[�̍쐬
	void GameStage::CreatePlayer(){
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	//�J�v�Z���̍쐬
	void GameStage::CreateCapsule(){
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
			{ 20.0f, 0, -20.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<CapsuleObject>(v);
		}
	}

	//�q�b�g���鋅�̂̍쐬
	void GameStage::CreateSphere(){
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, -25.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<SphereObject>(v);
		}
	}



	//�Փ˂���X�N�G�A�̍쐬
	void GameStage::CreateHitTestSquare(){
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"SquareGroup");
		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 1.0f, 1.0f),
			Vector3(0, 0.0f, 0.0f),
			Vector3(0.0f, 0.5f, 10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -15.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(20.0f, 0.4f, -15.0f)
			);
	}


	//������
	void GameStage::Create(){
		try{
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ނ��쐬����
			CreateViews();
			//�v���[�g�̍쐬
			CreatePlate();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//�J�v�Z���쐬
			CreateCapsule();
			//���̍쐬
			CreateSphere();
			//�Փ˂���X�N�G�A�̍쐬
			CreateHitTestSquare();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
