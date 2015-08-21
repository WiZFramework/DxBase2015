
#include "stdafx.h"
#include "Project.h"


namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	Labyrinth ��`��
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//�e�萔
	//--------------------------------------------------------------------------------------
	int Labyrinth::m_Dirtable[24][4] = {
		0, 1, 2, 3, 0, 1, 3, 2, 0, 2, 1, 3, 0, 2, 3, 1, 0, 3, 1, 2, 0, 3, 2, 1,
		1, 0, 2, 3, 1, 0, 3, 2, 1, 2, 0, 3, 1, 2, 3, 0, 1, 3, 0, 2, 1, 3, 2, 0,
		2, 0, 1, 3, 2, 0, 3, 1, 2, 1, 0, 3, 2, 1, 3, 0, 2, 3, 0, 1, 2, 3, 1, 0,
		3, 0, 1, 2, 3, 0, 2, 1, 3, 1, 0, 2, 3, 1, 2, 0, 3, 2, 0, 1, 3, 2, 1, 0 };

	int Labyrinth::m_Dx[4] = { 2, 0, -2, 0 };
	int Labyrinth::m_Dy[4] = { 0, 2, 0, -2 };

	//--------------------------------------------------------------------------------------
	//	void Labyrinth::Add(
	//	int i,		//X��
	//	int j		//Y��
	//	);
	//	�p�r: �T�u�֐��A�T�C�g�ɉ�����
	//	�߂�l: �Ȃ��B
	//--------------------------------------------------------------------------------------
	void Labyrinth::Add(int i, int j){
		m_XX[m_Nsite] = i;
		m_YY[m_Nsite] = j;
		m_Nsite++;
	}

	//--------------------------------------------------------------------------------------
	//	int Labyrinth::Select(
	//	int* i,	//X��
	//	int* j		//Y��
	//	);
	//	�p�r: �T�u�֐��A�T�C�g�𗐐��őI��
	//	�߂�l: �T�C�g�̔ԍ��B�T�C�g���s������0
	//--------------------------------------------------------------------------------------
	int Labyrinth::Select(int* i, int* j){
		int r;
		if (m_Nsite == 0)
			return 0;
		m_Nsite--;
		r = (int)(m_Nsite * (rand() / (RAND_MAX + 1.0)));
		*i = m_XX[r];
		m_XX[r] = m_XX[m_Nsite];
		*j = m_YY[r];
		m_YY[r] = m_YY[m_Nsite];
		return 1;
	}

	//--------------------------------------------------------------------------------------
	//	Labyrinth::Labyrinth(
	//	DWORD MaxX,		//�����̑傫���i�����j
	//	DWORD MaxY		//�c���̑傫���i�����j
	//	);
	//	�p�r: �R���X�g���N�^�B���H���쐬����
	//	�߂�l: �Ȃ��i���s���͗�O��throw�j
	//--------------------------------------------------------------------------------------
	Labyrinth::Labyrinth(DWORD  MaxX, DWORD  MaxY)
		:
		m_MaxX(MaxX), m_MaxY(MaxY),
		m_MaxSite(MaxX *MaxY / 4),
		m_Map(MaxX + 1, vector<int>(MaxY + 1)),	//�񎟌��z��̃C�j�V�����C�U
		m_XX(m_MaxSite), m_YY(m_MaxSite), //1�����z��̃C�j�V�����C�U
		m_Nsite(0){
		try{
			int i, j, i1, j1, d, t, *tt;
			for (i = 0; i <= m_MaxX; i++){
				for (j = 0; j <= m_MaxY; j++){
					m_Map[i][j] = 1;
				}
			}
			for (i = 3; i <= m_MaxX - 3; i++){
				for (j = 3; j <= m_MaxY - 3; j++){
					m_Map[i][j] = 0;
				}
			}
			m_Map[2][3] = 0;
			m_Map[m_MaxX - 2][m_MaxY - 3] = 0;
			for (i = 4; i <= m_MaxX - 4; i += 2){
				Add(i, 2);
				Add(i, m_MaxY - 2);
			}
			for (j = 4; i <= m_MaxY - 4; j += 2){
				Add(2, j);
				Add(m_MaxX - 2, j);
			}
			while (Select(&i, &j)){
				for (;;){
					tt = m_Dirtable[(int)(24 * (rand() / (RAND_MAX * 1.0)))];
					for (d = 3; d >= 0; d--){
						t = tt[d];
						i1 = i + m_Dx[t];
						j1 = j + m_Dy[t];
						if (m_Map[i1][j1] == 0){
							break;
						}
					}
					if (d < 0){
						break;
					}
					m_Map[(i + i1) / 2][(j + j1) / 2] = 1;
					i = i1;
					j = j1;
					m_Map[i][j] = 1;
					Add(i, j);
				}
			}
		}
		catch (...){
			//�ăX���[
			throw;
		}
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

	//�f�[�^�}�b�v��XZ����ւ�����
	void GameStage::ChangeVecXZ(const vector< vector<int> >& SrcMap, vector< vector<int> >& DestMap){
		DestMap.clear();
		size_t count = 0;
		size_t maxx = SrcMap[0].size();
		while (count < maxx){
			vector<int> ZLine(SrcMap.size());
			for (size_t i = 0; i < SrcMap.size(); i++){
				ZLine[i] = SrcMap[i][count];
			}
			DestMap.push_back(ZLine);
			count++;
		}
	}

	//�Œ�{�b�N�X��Add����
	void GameStage::SetFixedBox(vector< vector<int> >& DataMap, bool XLine){
		for (size_t i = 0; i < DataMap.size(); i++){
			auto& line = DataMap[i];
			int Size = 0;
			int Start = 0;
			int MiniSize = 0;
			if (XLine){
				MiniSize = 1;
			}
			for (size_t j = 0; j < line.size(); j++){
				if (line[j]){
					if (Size == 0){
						Start = j;
					}
					Size++;
				}
				else{
					if (Size > MiniSize){
						if (XLine){
							Vector3 Pos = Vector3(Start + (((float)Size) / 2.0f) - 0.5f, 0.5f, i * 1.0f);
							Pos.x *= m_Piece.x;
							Pos.y *= m_Piece.y;
							Pos.z *= m_Piece.z;

							Pos -= Vector3(((float)line.size() - 1) / 2.0f * m_Piece.x, 0, ((float)DataMap.size() - 1) / 2.0f * m_Piece.z);

							AddGameObject<FixedBox>(
								Vector3((float)Size * m_Piece.x, m_Piece.y, 1.0f * m_Piece.z),
								Vector3(0.0f, 0.0f, 0.0f),
								Pos
								);
							for (int k = 0; k < Size; k++){
								line[Start + k] = 0;
							}
						}
						else{
							Vector3 Pos = Vector3(i * 1.0f, 0.5f, Start + (((float)Size) / 2.0f) - 0.5f);
							Pos.x *= m_Piece.x;
							Pos.y *= m_Piece.y;
							Pos.z *= m_Piece.z;
							Pos -= Vector3(((float)DataMap.size() - 1) / 2.0f * m_Piece.x, 0, ((float)line.size() - 1) / 2.0f * m_Piece.z);

							AddGameObject<FixedBox>(
								Vector3(1.0f * m_Piece.x, m_Piece.y, (float)Size * m_Piece.z),
								Vector3(0.0f, 0.0f, 0.0f),
								Pos
								);
						}
					}
					Size = 0;
				}
			}
			if (Size > MiniSize){
				if (XLine){
					Vector3 Pos = Vector3(Start + (((float)Size) / 2.0f) - 0.5f, 0.5f, (i)* 1.0f);
					Pos.x *= m_Piece.x;
					Pos.y *= m_Piece.y;
					Pos.z *= m_Piece.z;
					Pos -= Vector3(((float)line.size() - 1) / 2.0f * m_Piece.x, 0, ((float)DataMap.size() - 1) / 2.0f * m_Piece.z);
					AddGameObject<FixedBox>(
						Vector3((float)Size * m_Piece.x, m_Piece.y, 1.0f * m_Piece.z),
						Vector3(0.0f, 0.0f, 0.0f),
						Pos
						);
					for (int k = 0; k < Size; k++){
						line[Start + k] = 0;
					}
				}
				Size = 0;
			}
		}
	}


	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox(){
		//�}�l�[�W������Q�Ƃł���悤�ɃO���[�v���쐬
		CreateSharedObjectGroup(L"FIXED_BOX");
		//���H�̍쐬
		Labyrinth labyrinth(m_MapSizeX, m_MapSizeZ);
		//���H�}�b�v�̎擾
		auto& LabMap = labyrinth.GetMap();
		//����
		vector< vector<int> > DataMap;
		ChangeVecXZ(LabMap, DataMap);
		//��ԊO�̕ǂɐڂ��Ă���ʘH���쐬
		for (size_t i = 1; i < DataMap.size() - 1; i++){
			auto line = DataMap[i];
			for (size_t j = 1; j < line.size() - 1; j++){
				if (i == 1 || i == (DataMap.size() - 2)){
					DataMap[i][j] = 0;
				}
				if (j == 1 || j == (line.size() - 2)){
					DataMap[i][j] = 0;
				}

			}
		}
		//�����̃Z�b�g
		SetFixedBox(DataMap, true);
		//�c��
		vector< vector<int> > DataMap2;
		ChangeVecXZ(DataMap, DataMap2);
		//�c���̃Z�b�g
		SetFixedBox(DataMap2, false);
		//�{�b�N�X�`��}�l�[�W���̍쐬
		AddGameObject<FixedBoxManager>();
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
			//�v���[���[�̍쐬
			CreatePlayer();


		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
