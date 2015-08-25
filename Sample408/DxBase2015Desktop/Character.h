#pragma once

#include "stdafx.h"

namespace basedx11{

	//カスタムシャドウマップ用コンスタントバッファ構造体
	struct CustomShadowmapConstantBuffer
	{
		XMMATRIX mWorld[3];
		XMMATRIX mView;
		XMMATRIX mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap,CustomShadowmapConstantBuffer>;
	//	用途: カスタムコンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap, CustomShadowmapConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>;
	//	用途: VSCustomShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>{
	public:
		//構築
		VSCustomShadowmap();
	};
	//--------------------------------------------------------------------------------------
	//	class GSCustomShadowmap : public GeometryShader<GSCustomShadowmap>;
	//	用途: CustomShadowmapジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	class GSCustomShadowmap : public GeometryShader<GSCustomShadowmap>{
	public:
		//構築
		GSCustomShadowmap();
	};

	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	用途: カスタムシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap{
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CustomShadowmap();
		//操作
		virtual void Draw();

	};


	//カスタム描画コンスタントバッファ構造体
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World[3];
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//汎用パラメータ
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBCustomDraw : public ConstantBuffer<CBCustomDraw,CustomDrawConstantBuffer>;
	//	用途: カスタムコンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBCustomDraw : public ConstantBuffer<CBCustomDraw, CustomDrawConstantBuffer>{
	};

	//--------------------------------------------------------------------------------------
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>;
	//	用途: VSCustomDraw頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>{
	public:
		//構築
		VSCustomDraw();
	};
	//--------------------------------------------------------------------------------------
	//	class GSCustomDraw : public GeometryShader<GSCustomDraw>;
	//	用途: GSCustomDrawジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	class GSCustomDraw : public GeometryShader<GSCustomDraw>{
	public:
		//構築
		GSCustomDraw();
	};
	//--------------------------------------------------------------------------------------
	//	class PSCustomDraw : public PixelShader<PSCustomDraw>;
	//	用途: PSCustomDrawピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSCustomDraw : public PixelShader<PSCustomDraw>{
	public:
		//構築
		PSCustomDraw();
	};





	//--------------------------------------------------------------------------------------
	//	class CustomDrawOctahedron : public GameObject;
	//	独自のシェーダで描画する8面体
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//インデックスバッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//インデックスの数
		UINT m_NumIndicis;
		//バックアップ用の頂点配列
		vector<VertexPositionNormalColor> m_BackupVirtex;
		//メッシュの作成
		void CreateCustomMesh();
		//頂点の変更
		void UpdateCustomMesh();
		//経過時間
		float m_TotalTime;
	public:
		//構築と破棄
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawOctahedron();
		//アクセサ
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

		//初期化
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() override;
	};

}
//endof  basedx11
