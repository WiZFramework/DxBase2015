
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

		//FBX���b�V���t�@�C������FBX�t�@�C���S�̂̃I�u�W�F�N�g�̍쐬�o�^
		//�Ԃ��L����
		//auto DataDir = App::GetApp()->m_wstrRelativeDataPath + L"Chara_R\\";
		//auto PtrFbxScene = App::GetApp()->RegisterFbxScene(L"CHARA_R_FBX", DataDir, L"Chara_R.fbx");
		//auto PtrFbxMesh = App::GetApp()->RegisterFbxMesh(L"CHARA_R_MESH", PtrFbxScene, 0);
		//PtrFbxMesh->AddAnimation("wait", 5, 4, true, 10);
		//PtrFbxMesh->AddAnimation("walk", 13, 4, true, 10);
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

	//���b�V���I�u�W�F�N�g�̍쐬
	void GameStage::CreateFbxMeshObject(){
		auto Ptr = AddGameObject<FbxMeshObject>();
		//�V�F�A�z���FbxMeshObject��ǉ�
		SetSharedGameObject(L"FbxMeshObject", Ptr);
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
			CreateFbxMeshObject();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
