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
		//���߂���{�b�N�X�̍쐬
		void CreateTraceBox();
		//���߂��鋅�̂̍쐬
		void CreateTraceSphere();
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
