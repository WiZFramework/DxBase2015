#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct SimpleDirectConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct SimpleDirectConstantBuffer
	{
		Color4 m_DiffuseColor;
		SimpleDirectConstantBuffer() {
			memset(this, 0, sizeof(SimpleDirectConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect, SimpleDirectConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	用途: VSSimpleDirect頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>{
	public:
		//構築
		VSSimpleDirect();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	用途: PSSimpleピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSSimpleDirect : public PixelShader<PSSimpleDirect>{
	public:
		//構築
		PSSimpleDirect();
	};



	//--------------------------------------------------------------------------------------
	//	class GameObject : public Object, public SimpleInterface;
	//	用途: ゲームオブジェクト
	//--------------------------------------------------------------------------------------
	class GameObject : public Object, public SimpleInterface{
		//頂点バッファ
		//バッファ関連
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		UINT m_NumVertices;				//頂点の数
	public:
		GameObject()
		{}
		virtual ~GameObject(){}
		virtual void Create()override;
		virtual void Update()override{}
		virtual void Draw()override;
	};


	//--------------------------------------------------------------------------------------
	//	class GameStage : public StageBase;
	//	用途: ゲームステージ
	//--------------------------------------------------------------------------------------
	class GameStage : public StageBase{
		vector<shared_ptr<GameObject>> m_GameObjectVec;
		shared_ptr<View> m_View;
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//デフォルトのレンダリングターゲット
		//文字列
		shared_ptr<StringSprite> m_StringSprite;
	public:
		GameStage(){}
		virtual ~GameStage(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
