#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class TriangleObject : public GameObject;
	//	用途: 三角形オブジェクト
	//--------------------------------------------------------------------------------------
	class TriangleObject : public GameObject{
		//バッファ関連
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		UINT m_NumVertices;				//頂点の数
	public:
		//構築と破棄
		TriangleObject(const shared_ptr<Stage>& StagePtr);
		virtual ~TriangleObject();
		//初期化
		virtual void Create() override;
		//描画
		virtual void Draw()override;
	};

}
//endof  basedx11
