#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Texture3DConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct Texture3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Texture3DConstantBuffer() {
			memset(this, 0, sizeof(Texture3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTexture3D : public ConstantBuffer<CBTexture3D,Texture3DConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBTexture3D : public ConstantBuffer<CBTexture3D, Texture3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>;
	//	用途:  VSTexture3D頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>{
	public:
		//構築
		VSTexture3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTexture3D : public PixelShader<PSTexture3D>;
	//	用途: PSTexture3Dピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSTexture3D : public PixelShader<PSTexture3D>{
	public:
		//構築
		PSTexture3D();
	};


	//--------------------------------------------------------------------------------------
	//	class ObjectResource : public Object, public SimpleInterface;
	//	用途: オブジェクトリソース
	//--------------------------------------------------------------------------------------
	class ObjectResource : public Object, public SimpleInterface{
	protected:
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//インデックスバッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//インデックスの数
		UINT m_NumIndicis;
		ObjectResource(){}
		virtual ~ObjectResource(){}
	public:
		const ComPtr<ID3D11Buffer>& GetVertexBuffer() const{
			return m_VertexBuffer;
		}
		const ComPtr<ID3D11Buffer>& GetIndexBuffer() const{
			return m_IndexBuffer;
		}
		UINT GetNumVertices() const{
			return m_NumVertices;
		}
		UINT GetNumIndicis() const{
			return m_NumIndicis;
		}
		virtual void Update()override{}
		virtual void Draw()override{}
	};

	//--------------------------------------------------------------------------------------
	//	class CubeResource : public ObjectResource;
	//	用途: 立方体リソース
	//--------------------------------------------------------------------------------------
	class CubeResource : public ObjectResource{
	public:
		CubeResource(){}
		virtual ~CubeResource(){}
		virtual void Create()override;
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public Object, public SimpleInterface;
	//	用途: テクスチャリソース
	//--------------------------------------------------------------------------------------
	class TextureResource : public Object, public SimpleInterface{
	protected:
		//テクスチャファイル名
		wstring m_TextureFileName;
		//シェーダリソースビュー（テクスチャリソース）
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//リソースビュー

		void CreateTextureBase();
	public:
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResView() const{
			return m_ShaderResView;
		}
		TextureResource(){}
		TextureResource(const wstring& str) :
			m_TextureFileName(str)
		{}
		virtual ~TextureResource(){}
		void CreateTexture(const wstring& str);

		virtual void Create()override;
		virtual void Update()override{}
		virtual void Draw()override{}

	};


	class GameStage;

	//--------------------------------------------------------------------------------------
	//	class CubeObject : public Object, public SimpleInterface;
	//	用途: 立方体オブジェクト
	//--------------------------------------------------------------------------------------
	class CubeObject : public Object, public SimpleInterface{
		weak_ptr<GameStage> m_GameStgae;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Matrix4X4 m_WorldMatrix;
		float m_Span;
		bool m_IsHit;
	public:
		CubeObject(const shared_ptr<GameStage>& GStage,
			const Vector3& StartScale,
			const Vector3& StartRotation,
			const Vector3& StartPos,
			float Span);
		virtual ~CubeObject(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Update2();
		virtual void Draw()override;
		//衝突関連
		OBB GetOBB();
		void SetHit();
	};





	//--------------------------------------------------------------------------------------
	//	class GameStage : public StageBase;
	//	用途: ゲームステージ
	//--------------------------------------------------------------------------------------
	class GameStage : public StageBase{
		vector<shared_ptr<CubeObject>> m_CubeObjectVec;
		shared_ptr<View> m_View;
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//デフォルトのレンダリングターゲット
		//文字列
		shared_ptr<StringSprite> m_StringSprite;
		//立方体リソース
		shared_ptr<CubeResource> m_CubeResource;
		//テクスチャリソース
		shared_ptr<TextureResource> m_TextureResource;
	public:
		GameStage(){}
		virtual ~GameStage(){}
		const shared_ptr<View>& GetView() const{
			return m_View;
		}
		const shared_ptr<CubeResource>& GetCubeResource() const{
			return m_CubeResource;
		}
		const shared_ptr<TextureResource>& GetTextureResource() const{
			return m_TextureResource;
		}
		const vector<shared_ptr<CubeObject>>& GetCubeObjectVec(){
			return m_CubeObjectVec;
		}



		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
