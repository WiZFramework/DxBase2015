#pragma once
#include "stdafx.h"

class CFbx2BinView;

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	用途: シーンクラス
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		CFbx2BinView* m_pView;
	public:
		//構築と破棄
		Scene(){}
		~Scene(){}
		//アクセサ
		void SetView(CFbx2BinView* pView){
			m_pView = pView;
		}
		CFbx2BinView* GetView() const{
			return m_pView;
		}
		//操作
		virtual void Create()override;
	};
}
//end basedx11
