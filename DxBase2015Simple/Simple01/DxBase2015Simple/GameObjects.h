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
	//	用途: ゲームステージ
	//--------------------------------------------------------------------------------------
	class GameObject : public Object, public SimpleInterface{
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//メッシュの作成
		void CreateCustomMesh();
		//頂点の変更
		void UpdateCustomMesh();
		//経過時間
		float m_TotalTime;
	public:
		GameObject() :
			m_TotalTime(0)
		{}
		virtual ~GameObject(){}
		virtual void Create()override;
		virtual void Update()override;
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
