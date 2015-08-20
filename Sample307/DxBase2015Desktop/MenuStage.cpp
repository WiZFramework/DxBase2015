
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//���\�[�X�̍쐬
	void MenuStage::CreateResourses(){
		wstring CursorWav = App::GetApp()->m_wstrRelativeDataPath + L"Sounds\\cursor.wav";
		App::GetApp()->RegisterWav(L"Cursor", CursorWav);
		wstring DecideSE = App::GetApp()->m_wstrRelativeDataPath + L"Sounds\\decide.wav";
		App::GetApp()->RegisterWav(L"Decide", DecideSE);
	}

	//�r���[�ނ̍쐬
	void MenuStage::CreateViews(){
		//�ŏ��Ƀf�t�H���g�̃����_�����O�^�[�Q�b�g�ނ��쐬����
		CreateDefaultRenderTargets();
		//�}���`�r���[�R���|�[�l���g�̎擾
		auto PtrMultiView = GetComponent<MultiView>();
		//�}���`�r���[�Ƀr���[�̒ǉ�
		auto PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�i�Q�[���T�C�Y�S�́j
		Rect2D<float> rect(0, 0, 640, 360);
		Point2D<float> offset(0, 70);
		//�ŏ��̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<Camera, MultiLight>(rect + offset, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 0.0f, -1.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetDirectional(0.0f, 0.0f, 0.5f);
	}

	//���j���[�X�v���C�g�A�j���[�V�����̍쐬
	void MenuStage::CreateSSMenu(){
		AddGameObject<StageSelectAnimation>( App::GetApp()->m_wstrRelativeDataPath + L"StageSelect\\", L"Opening" ) ;
	}

	//������
	void MenuStage::Create(){
		try{
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ނ��쐬����
			CreateViews();
			//���j���[�X�v���C�g�A�j���[�V�����̍쐬
			CreateSSMenu();
		}
		catch (...){
			throw;
		}
	}

	//����
	void MenuStage::Update(){
		//	: �R���g���[�����̎擾.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();

		//	: B�{�^���������ꂽ��.
		if ( CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B ) {
			PostEvent( 0.0f, GetThis<MenuStage>(), App::GetApp()->GetSceneBase(), L"ToGame" ) ;
			
		}
	}

}
//endof  basedx11