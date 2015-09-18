#pragma once
#include "stdafx.h"

class CFbx2BinView;

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	�p�r: �V�[���N���X
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		CFbx2BinView* m_pView;
	public:
		//�\�z�Ɣj��
		Scene(){}
		~Scene(){}
		//�A�N�Z�T
		void SetView(CFbx2BinView* pView){
			m_pView = pView;
		}
		CFbx2BinView* GetView() const{
			return m_pView;
		}
		//����
		virtual void Create()override;
	};
}
//end basedx11
