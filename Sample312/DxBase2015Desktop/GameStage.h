#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	�p�r: ���j���[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage{
		//�r���[�̍쐬
		void CreateViews();
		//������̍쐬
		void CreateString();
	public:
		//�\�z�Ɣj��
		MenuStage() :Stage(){}
		virtual ~MenuStage(){}
		//������
		virtual void Create()override;
		//�X�V
		virtual void Update() override;
	};


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
