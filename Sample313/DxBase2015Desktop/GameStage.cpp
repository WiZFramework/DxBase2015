
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

		wstring MediaPath = App::GetApp()->m_wstrRelativeDataPath;
		//CSV�t�@�C���̌���
		wstring CsvFilename = MediaPath + L"GameStage.csv";
		//���[�J�����CSV�t�@�C���N���X���쐬
		CsvFile GameStageCsv(CsvFilename);
		if (!GameStageCsv.ReadCsv()){
			//�t�@�C���͑��݂��Ȃ�����
			// ���������s
			throw BaseException(
				L"CSV�t�@�C��������܂���B",
				CsvFilename,
				L"GameStage::CreateSeekObject()"
				);
		}
		//�ǂ��|����I�u�W�F�N�g�f�[�^��ǂݍ���
		//�����_���𗘗p
		//�킩��₷���悤�ɃC�����C���ł͏����Ȃ�
		//�ȉ��́Awstring�������Ɏ��bool��Ԃ������_��
		//--------------------------------------------------------
		auto func = [](const wstring& str)->bool{
			//1�s���R���}��؂�̕�����ɂȂ��Ă���̂ŕ�������
			vector< wstring > StrVec;
			Util::WStrToTokenVector(StrVec, str, L',');
			if (StrVec[0] == L"SeekObject" && StrVec[1] == L"1" && StrVec[2] == L"Release"){
				return true;
			}
			else{
				return false;
			}
		};
		//--------------------------------------------------------
		//���[�N�p�̕�����̔z��
		vector<wstring> SeekObjectVec;
		//csv�̒�����A�����i�����_���Ō����j�Œ��o
		GameStageCsv.GetSelect(SeekObjectVec, func);

		//��L�̍\���́A�ȉ��̂悤�ɃC�����C���ŏ�����
		/*
		GameStageCsv.GetSelect(SeekObjectVec,
			[](const wstring& str)->bool{
				//1�s���R���}��؂�̕�����ɂȂ��Ă���̂ŕ�������
				vector< wstring > StrVec;
				Util::WStrToTokenVector(StrVec, str, L',');
				if (StrVec[0] == L"SeekObject" && StrVec[1] == L"1" && StrVec[2] == L"Release"){
					return true;
				}
				else{
					return false;
				}
			}
		);
		*/
		
		/*
		//�L���v�`�����̃����_���Ƃ��Ĉȉ��̂悤�ɂ�������

		wstring StageStr = L"1";
		wstring ReleaseStr = L"Release";
		auto func2 = [&](const wstring& str)->bool{
			//1�s���R���}��؂�̕�����ɂȂ��Ă���̂ŕ�������
			vector< wstring > StrVec;
			Util::WStrToTokenVector(StrVec, str, L',');
			if (StrVec[0] == L"SeekObject" && StrVec[1] == StageStr && StrVec[2] == ReleaseStr){
				return true;
			}
			else{
				return false;
			}
		};
		//�L���v�`���������_�����g����select
		GameStageCsv.GetSelect2(SeekObjectVec, func2);
		*/
		/*
		//�L���v�`�����̃����_������x�ɏ����ꍇ

		wstring StageStr = L"1";
		wstring ReleaseStr = L"Release";
		//�L���v�`���������_�����g����select
		GameStageCsv.GetSelect2(SeekObjectVec, 
			[&](const wstring& str)->bool{
				//1�s���R���}��؂�̕�����ɂȂ��Ă���̂ŕ�������
				vector< wstring > StrVec;
				Util::WStrToTokenVector(StrVec, str, L',');
				if (StrVec[0] == L"SeekObject" && StrVec[1] == StageStr && StrVec[2] == ReleaseStr){
					return true;
				}
				else{
					return false;
				}
			}
		);
		*/

		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto LineStr : SeekObjectVec){
			//1�s���R���}��؂�̕�����ɂȂ��Ă���̂ŕ�������
			vector< wstring > LineVec;
			Util::WStrToTokenVector(LineVec, LineStr, L',');
			//���������z�񂩂�f�[�^�����o��
			AddGameObject<SeekObject>(Vector3(LineVec[3], LineVec[4], LineVec[5]));
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
