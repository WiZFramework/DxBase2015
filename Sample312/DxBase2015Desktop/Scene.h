#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	�p�r: �V�[���N���X
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	public:
		//�\�z�Ɣj��
		Scene(){}
		~Scene(){}
		//�A�N�Z�T
		//����
		virtual void Create()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;
	};
}
//end basedx11
