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
			ResetActiveStage<GameStage>();
		}
		catch (...){
			throw;
		}
	}

}
//end basedx11
