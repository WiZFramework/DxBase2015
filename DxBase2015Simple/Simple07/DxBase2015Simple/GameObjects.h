#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct TextureSkin3DConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct TextureSkin3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		XMVECTOR Bones[3 * 72];
		TextureSkin3DConstantBuffer() {
			memset(this, 0, sizeof(TextureSkin3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTextureSkin3D : public ConstantBuffer<CBTextureSkin3D, TextureSkin3DConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBTextureSkin3D : public ConstantBuffer<CBTextureSkin3D, TextureSkin3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTextureSkin3D : public VertexShader<VSTextureSkin3D, VertexPositionNormalTextureSkinning>;
	//	用途:  VSTextureSkin3D頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSTextureSkin3D : public VertexShader<VSTextureSkin3D, VertexPositionNormalTextureSkinning>{
	public:
		//構築
		VSTextureSkin3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTextureSkin3D : public PixelShader<PSTextureSkin3D>;
	//	用途: PSTextureSkin3Dピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSTextureSkin3D : public PixelShader<PSTextureSkin3D>{
	public:
		//構築
		PSTextureSkin3D();
	};

	class GameStage;

	struct MaterialEx{
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		Color4 m_Diffuse;
		//! スペキュラー（反射光）
		Color4 m_Specular;
		//! アンビエント（環境色）
		Color4 m_Ambient;
		//! エミッシブ（放射光）
		Color4 m_Emissive;
		//シェーダリソースビュー（テクスチャリソース）
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;
	};

	enum class BlockType{
		Vertex,
		Index,
		Material,
		MaterialCount,
		SkinedVertex,
		BoneCount,
		AnimeMatrix,
		End = 100
	};

	struct BlockHeader{
		BlockType m_Type;
		UINT m_Size;
	};

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
		//マテリアルの配列
		vector<MaterialEx> m_Materials;
		//ボーンの数
		UINT m_BoneCount;
		//サンプリング数
		UINT m_SampleCount;
		//サンプリングされたボーン行列
		vector<Matrix4X4> m_SampleMatrix;
		//シェーダに渡すボーン行列
		vector<Matrix4X4> m_LocalBonesMatrix;
		//サンプルをひと回りするのに要する時間（秒）
		float m_LoopTime;
		//メッシュデータの読み込み
		void ReadMesh(vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, 
			vector<MaterialEx>& materials,vector<Matrix4X4>& bonematrix);
		//メッシュの作成
		void CreateCustomMesh();
		//シェーダリソースビューの作成
		ComPtr<ID3D11ShaderResourceView> CreateShaderResView(const wstring& TextureFileName);
		//補間処理
		void InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out);
		void CalucLocalBonesMatrix(float TgtTime);
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
