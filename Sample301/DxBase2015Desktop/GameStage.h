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
		//�ǂ�������I�u�W�F�N�g�̍쐬
		void CreateSeekObject();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�㉺�ړ����Ă���{�b�N�X�̍쐬
		void CreateMoveBox();
		//�J�v�Z���̍쐬
		void CreateCapsule();
		//�q�b�g���鋅�̂̍쐬
		void CreateSphere();
		//�Փ˂���X�N�G�A�̍쐬
		void CreateHitTestSquare();
		//�i���o�[�X�v���C�g�̍쐬
		void CreateNumberSprite();
		//�o�[�X�v���C�g�̍쐬
		void CreateBerSprite();
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
