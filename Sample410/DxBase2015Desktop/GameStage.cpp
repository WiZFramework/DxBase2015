
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
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, Color4(0.0f, 0.125f, 0.3f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = PtrView->GetCamera();
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



	//�`��}�l�[�W���̍쐬
	void GameStage::CreateDrawBoxManager(){
		auto Ptr = AddGameObject<DrawBoxManager>(1000,L"DEFAULT_CUBE", L"SKY_TX");
		//�V�F�A�I�u�W�F�N�g�Ƀ}�l�[�W���[��ǉ�
		SetSharedGameObject(L"DrawBoxManager", Ptr);

		Ptr = AddGameObject<DrawBoxManager>(2000, L"DEFAULT_DODECAHEDRON", L"WALL_TX");
		//�V�F�A�I�u�W�F�N�g�ɕ����p�̃}�l�[�W���[��ǉ�
		SetSharedGameObject(L"DrawSplitBoxManager", Ptr);

	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox(){
		for (int i = 0; i < 30; i++){
			for (int j = 0; j < 30; j++){
				float x = (float)(i - 15) / 2.0f;
				float z = (float)(j + 2) / 2.0f;
				AddGameObject<FixedBox>(Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 0, 0), Vector3(x, 0.25f, z));
			}
		}
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
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//�`��}�l�[�W���̍쐬
			CreateDrawBoxManager();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�e���ׂ�������(�f�t�H���g64)
			Shadowmap::SetViewWidth(16.0f);
			Shadowmap::SetViewHeight(16.0f);
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
