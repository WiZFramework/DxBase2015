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
		//�ǂ�������I�u�W�F�N�g�̍쐬
		void CreateSeekObject();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//������
		virtual void Create()override;
	};
}
//endof  basedx11
