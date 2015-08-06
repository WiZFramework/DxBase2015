
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
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
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"bar.jpg";
		App::GetApp()->RegisterTexture(L"BAR_TX", strTexture);

	}

	//�r���[�ނ̍쐬
	void GameStage::CreateViews(){
		//�ŏ��Ƀf�t�H���g�̃����_�����O�^�[�Q�b�g�ނ��쐬����
		CreateDefaultRenderTargets();
		//�}���`�r���[�R���|�[�l���g�̎擾
		auto PtrMultiView = GetComponent<MultiView>();
		//�}���`�r���[�Ƀr���[�̒ǉ�1
		auto PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�i�Q�[���T�C�Y�������j
		Rect2D<float> rect(0, 0, (float)320, (float)480);
		//�ŏ��̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.2f, 1.0f);
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

		//�}���`�r���[�Ƀr���[�̒ǉ�2
		PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�i�Q�[���T�C�Y�E�����j
		rect = Rect2D<float>(0, 0, (float)320, (float)480);
		rect += Point2D<float>(320.0f, 0);

		//2�Ԗڂ̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.2f, 1.0f);
		PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}

	//�v���[�g�̍쐬
	void GameStage::CreatePlate(){
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		//�ω��R���|�[�l���g�iTransform�j�̎擾
		auto TrancePtr = Ptr->GetComponent<Transform>();
		//�X�P�[�����O�̐ݒ�
		TrancePtr->SetScale(200.0f, 200.0f, 1.0f);
		//��]�̐ݒ�
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		TrancePtr->SetQuaternion(Qt);
		//�ʒu�̐ݒ�
		TrancePtr->SetPosition(0, 0, 0);
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<SimplePNTDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);
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


	//������
	void GameStage::Create(){
		try{
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ނ��쐬����
			CreateViews();
			//�v���[�g���쐬����
			CreatePlate();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�q���X�e�[�W�̍쐬
			AddChileStage<ChildGameStage>();

		}
		catch (...){
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	class ChildGameStage : public Stage;
	//	�p�r: �q���Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//�r���[�̍쐬
	void ChildGameStage::CreateViews(){
		//�ŏ��Ƀf�t�H���g�̃����_�����O�^�[�Q�b�g�ނ��쐬����
		CreateDefaultRenderTargets();


		//�}���`�r���[�R���|�[�l���g�̎擾
		auto PtrMultiView = GetComponent<MultiView>();
		//�}���`�r���[�Ƀr���[�̒ǉ�
		auto PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�
		Rect2D<float> rect(0, 0, (float)640, (float)480);

		//0�Ԗڂ̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.0f, 0.0f, 0.0f), 1, 0.0f, 0.2f);
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f,0.0f, -10.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}

	//�i���o�[�X�v���C�g�̍쐬
	void ChildGameStage::CreateNumberSprite(){
		//�z��̏�����
		vector<Vector3> Vec = {
			{ -6.0f, -5.5f, 0 },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<NumberSprite>(v);
		}
	}

	//�o�[�X�v���C�g�̍쐬
	void ChildGameStage::CreateBerSprite(){
		AddGameObject<BarSprite>(Vector3(1.0f, 1.0f, 0));
	}

	void ChildGameStage::Create(){
		CreateViews();
		CreateNumberSprite();
		CreateBerSprite();
	}





}
//endof  basedx11
