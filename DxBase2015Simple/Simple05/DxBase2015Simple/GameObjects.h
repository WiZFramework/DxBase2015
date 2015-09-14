#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct TextureColor3DConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct TextureColor3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		TextureColor3DConstantBuffer() {
			memset(this, 0, sizeof(TextureColor3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D,TextureColor3DConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D, TextureColor3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTextureColor3D : public VertexShader< VSTextureColor3D, VertexPositionNormalColorTexture>;
	//	用途:  VSTextureColor3D頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSTextureColor3D : public VertexShader<VSTextureColor3D, VertexPositionNormalColorTexture>{
	public:
		//構築
		VSTextureColor3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTextureColor3D : public PixelShader<PSTextureColor3D>;
	//	用途: PSTextureColor3Dピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSTextureColor3D : public PixelShader<PSTextureColor3D>{
	public:
		//構築
		PSTextureColor3D();
	};

	class GameStage;

	//--------------------------------------------------------------------------------------
	//	class GameObject : public Object, public SimpleInterface;
	//	用途: ゲームオブジェクト
	//--------------------------------------------------------------------------------------
	class GameObject : public Object, public SimpleInterface{
		weak_ptr<GameStage> m_GameStgae;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Matrix4X4 m_WorldMatrix;
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//インデックスバッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//インデックスの数
		UINT m_NumIndicis;
		//テクスチャファイル名
		wstring m_TextureFileName;
		//シェーダリソースビュー（テクスチャリソース）
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//リソースビュー
		//メッシュの作成
		void CreateCustomMesh();
		//シェーダリソースビューの作成
		void CreateShaderResView();
	public:
		GameObject(const shared_ptr<GameStage>& GStage,
			const Vector3& StartScale,
			const Vector3& StartRotation,
			const Vector3& StartPos);
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
		const shared_ptr<View>& GetView() const{
			return m_View;
		}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
