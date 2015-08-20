#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViews();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�㉺�ړ����Ă���{�b�N�X�̍쐬
		void CreateMoveBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//������
		virtual void Create()override;
	};

	//--------------------------------------------------------------------------------------
	//	class ChildGameStage : public Stage;
	//	�p�r: �q���Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class ChildGameStage : public Stage{
		//�r���[�̍쐬
		void CreateViews();
		//�i���o�[�X�v���C�g�̍쐬
		void CreateNumberSprite();
		//�o�[�X�v���C�g�̍쐬
		void CreateBerSprite();
	public:
		//�\�z�Ɣj��
		ChildGameStage() :Stage(){}
		virtual ~ChildGameStage(){}
		//������
		virtual void Create()override;
	};

}
//endof  basedx11
