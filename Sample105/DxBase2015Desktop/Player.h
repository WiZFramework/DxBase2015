#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		//�ړ��̌����𓾂�
		Vector3 GetAngle();
	public:
		//�\�z�Ɣj��
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player(){}
		//������
		virtual void Create() override;
		//�A�N�Z�T
		//�C�e�̔���
		void StartShellBall();
		//�X�V
		virtual void Update() override;
	};


}
//endof  basedx11
