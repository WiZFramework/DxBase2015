#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Labyrinth;
	//	�p�r: ���H�N���X
	//--------------------------------------------------------------------------------------
	class Labyrinth{
		//�e�萔
		static int m_Dirtable[24][4];	//�����\
		static int m_Dx[4];				//�ψʃx�N�g��
		static int m_Dy[4];				//�ψʃx�N�g��
		//���H�̃T�C�Y�֘A
		int m_MaxX;			//���H�̉��T�C�Y�i�����j
		int m_MaxY;			//���H�̏c�T�C�Y�i�����j
		const int m_MaxSite;		//�ő�T�C�g��
		//���I�ɐ��������z��֘A
		vector< vector<int> > m_Map;
		//�񎟌��z��B�C�j�V�����C�U����
		vector<int> m_XX;
		vector<int> m_YY;
		int m_Nsite;	//�o�^�T�C�g��
		//--------------------------------------------------------------------------------------
		//	void Add(
		//	int i,		//X��
		//	int j		//Y��
		//	);
		//	�p�r: �T�u�֐��A�T�C�g�ɉ�����
		//	�߂�l: �Ȃ��B
		//--------------------------------------------------------------------------------------
		void Add(int i, int j);
		//--------------------------------------------------------------------------------------
		//	int Select(
		//	int* i,	//X��
		//	int* j		//Y��
		//	);
		//	�p�r: �T�u�֐��A�T�C�g�𗐐��őI��
		//	�߂�l: �T�C�g�̔ԍ��B�T�C�g���s������0
		//--------------------------------------------------------------------------------------
		int Select(int* i, int* j);
	public:
		//--------------------------------------------------------------------------------------
		//	Labyrinth(
		//	DWORD  MaxX,		//�����̑傫���i�����j
		//	DWORD  MaxY		//�c���̑傫���i�����j
		//	);
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ��i���s���͗�O��throw�j
		//--------------------------------------------------------------------------------------
		Labyrinth(DWORD MaxX, DWORD MaxY);
		//--------------------------------------------------------------------------------------
		//	~Labyrinth();
		//	�p�r: �f�X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		~Labyrinth(){
			//�������Ȃ��B�z���vector���J������
		}
		//--------------------------------------------------------------------------------------
		//	vector< vector<int> >& GetMap();
		//	�p�r: ���H�̃}�b�v�𓾂�
		//	�߂�l: ���H�̃}�b�v�i�T�C�Y�͌Ăяo�����Œ��ӂ��Ďg�p����j
		//--------------------------------------------------------------------------------------
		vector< vector<int> >& GetMap(){
			return m_Map;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//�s�[�X�̑傫��
		const Vector3 m_Piece;
		//���H��X������Z�����T�C�Y�i6�ȏ�̋����ɂ���j
		int m_MapSizeX;
		int m_MapSizeZ;
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViews();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�f�[�^�}�b�v��XZ����ւ�����
		void ChangeVecXZ(const vector< vector<int> >& SrcMap, vector< vector<int> >& DestMap);
		//�Œ�{�b�N�X��Add����
		void SetFixedBox(vector< vector<int> >& DataMap, bool XLine);
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :
			Stage(), m_Piece(2.0f, 1.0f, 2.0f),
			m_MapSizeX(30),
			m_MapSizeZ(30)
		{}
		virtual ~GameStage(){}
		//�A�N�Z�T
		const Vector3& GetPiece() const{
			return m_Piece;
		}
		//������
		virtual void Create()override;
	};
}
//endof  basedx11
