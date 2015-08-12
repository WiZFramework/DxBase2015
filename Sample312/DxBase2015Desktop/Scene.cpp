#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	�p�r: �V�[���N���X
	//--------------------------------------------------------------------------------------
	void Scene::Create(){
		try{
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<MenuStage>();
		}
		catch (...){
			throw;
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"ToGame"){
			ResetActiveStage<GameStage>();
		}

		if (event->m_MsgStr == L"ToMenu"){
			ResetActiveStage<MenuStage>();
		}
	}


}
//end basedx11
