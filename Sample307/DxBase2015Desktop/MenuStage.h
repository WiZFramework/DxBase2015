#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	�p�r: ���j���[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage{
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViews();
		//���j���[�X�v���C�g�A�j���[�V�����̍쐬
		void CreateSSMenu();
	public:
		//�\�z�Ɣj��
		MenuStage() :Stage(){}
		virtual ~MenuStage(){}
		//������
		virtual void Create()override;
		//����
		virtual void Update()override;
	};
}
//endof  basedx11
