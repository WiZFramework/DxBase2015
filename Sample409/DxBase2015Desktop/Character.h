#pragma once

#include "stdafx.h"

namespace basedx11{

	struct BallConstantBuffer
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Proj;
		Vector3  m_LightDir;
		float m_Explode;
		BallConstantBuffer() {
			memset(this, 0, sizeof(BallConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class Ball : public GameObject;
	//	用途: 頂点が変更されるボール
	//--------------------------------------------------------------------------------------
	class Ball : public GameObject{
		Vector3 m_StartPos;
		//コンスタントバッファ
		shared_ptr<CBuffer<BallConstantBuffer> > m_ConstantBuffer;
		//頂点シェーダ
		shared_ptr<VShader<VertexPositionNormalTexture> > m_VirtexShader;
		//ピクセルシェーダー
		shared_ptr<PShader> m_PixelShader;
		//ジオメトリシェーダー
		shared_ptr<GShader> m_GeometryShader;
		//飛び出す係数
		float m_Explode;
		//半透明かどうか
		bool m_IsHalfAlpha;
	public:
		//構築と破棄
		Ball(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Ball(){}
		//初期化
		virtual void Create() override;
		//変化
		virtual void Update() override;
		//描画
		virtual void Draw() override;
	};

}
//endof  basedx11
