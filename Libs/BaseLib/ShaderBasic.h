#pragma once

#include "stdafx.h"

namespace basedx11{

	//Shadow

	struct ShadowConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMVECTOR Bones[3 * 72];	//Bone用
		ShadowConstantBuffer() {
			memset(this, 0, sizeof(ShadowConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow,ShadowConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBShadow : public ConstantBuffer<CBShadow, ShadowConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	用途: VSShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>{
	public:
		//構築
		VSShadowmap();
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSShadowmapBone頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>{
	public:
		//構築
		VSShadowmapBone();
	};

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	用途: PSShadowmapピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSShadowmap : public PixelShader<PSShadowmap>{
	public:
		//構築
		PSShadowmap();
	};


	//--------------------------------------------------------------------------------------
	//スプライト

	struct SpriteConstantBuffer
	{
		//転置してない行列
		XMMATRIX mTransformMatrix;
	};

	//--------------------------------------------------------------------------------------
	//	class CBSprite : public ConstantBuffer<CBSprite,SpriteConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBSprite : public ConstantBuffer<CBSprite, SpriteConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSSpritePCT : public VertexShader<VSSpritePCT>;
	//	用途: VSSpritePCT頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSSpritePCT : public VertexShader<VSSpritePCT, VertexPositionColorTexture>{
	public:
		//構築
		VSSpritePCT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSpritePCT : public PixelShader<PSSpritePCT>;
	//	用途: PSSpritePCTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSSpritePCT : public PixelShader<PSSpritePCT>{
	public:
		//構築
		PSSpritePCT();
	};

	//Simple
	//
	//--------------------------------------------------------------------------------------
	//	struct SimpleConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct SimpleConstantBuffer
	{
		Matrix4X4 m_Model;
		Matrix4X4 m_View;
		Matrix4X4 m_Projection;
		Vector4 m_Direction;
		Color4 m_DiffuseColor;
		Matrix4X4 m_LightWorldViewProj;   // ライトビューの正射影行列（影用）
		XMUINT4 m_ActiveFlg;			//テクスチャと影がアクティブかどうか
		Vector4 m_LPos;					//ライトの位置
		Matrix4X4 m_LView;				//ライトビュー
		Matrix4X4 m_LProjection;			//ライトプロジェクション
		Vector4 m_EyePos;					//カメラの位置
		//x=Texture,y=Shadow
		SimpleConstantBuffer() {
			memset(this, 0, sizeof(SimpleConstantBuffer));
		};
	};


	//--------------------------------------------------------------------------------------
	//	class CBSimple : public ConstantBuffer<CBSimple,SimpleConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBSimple : public ConstantBuffer<CBSimple, SimpleConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSSimplePNT : public VertexShader<VSSimplePNT, VertexPositionNormalTexture>;
	//	用途: VSSimplePNT頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSSimplePNT : public VertexShader<VSSimplePNT, VertexPositionNormalTexture>{
	public:
		//構築
		VSSimplePNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT : public PixelShader<PSSimplePNT>;
	//	用途: PSSimplePNTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSSimplePNT : public PixelShader<PSSimplePNT>{
	public:
		//構築
		PSSimplePNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>;
	//	用途: PSSimplePNT2ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>{
	public:
		//構築
		PSSimplePNT2();
	};



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

	struct BasicConstantBuffer
	{
		XMVECTOR diffuseColor;
		XMVECTOR emissiveColor;
		XMVECTOR specularColorAndPower;

		XMVECTOR lightDirection[ShaderResource::m_LightMax];
		XMVECTOR lightDiffuseColor[ShaderResource::m_LightMax];
		XMVECTOR lightSpecularColor[ShaderResource::m_LightMax];

		XMVECTOR eyePosition;

		XMVECTOR fogColor;
		XMVECTOR fogVector;

		XMMATRIX world;
		XMVECTOR worldInverseTranspose[3];
		XMMATRIX worldViewProj;

		XMMATRIX LightWorldViewProj;   // ライトビューの正射影行列

		XMUINT4 m_ActiveFlg;			//テクスチャと影がアクティブかどうか
										//x=Texture,y=Shadow,z=lightcount

		Vector4 m_LPos;					//ライトの位置
		Matrix4X4 m_LView;				//ライトビュー
		Matrix4X4 m_LProjection;			//ライトプロジェクション
		Vector4 m_EyePos;					//カメラの位置


		BasicConstantBuffer() {
			memset(this, 0, sizeof(BasicConstantBuffer));
			diffuseColor = g_XMOne;
			emissiveColor = g_XMZero;
			specularColorAndPower = g_XMZero;
			static const XMVECTORF32 defaultLightDirection = { 0, -1, 0, 0 };
			for (int i = 0; i < 3; i++)
			{
				lightDirection[i] = defaultLightDirection;
				lightDiffuseColor[i] = g_XMZero;
				lightSpecularColor[i] = g_XMZero;
			}
		};
	};


	//コンスタントバッファ

	//--------------------------------------------------------------------------------------
	//	class CBBasic : public ConstantBuffer<CBBasic,BasicConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBBasic : public ConstantBuffer<CBBasic, BasicConstantBuffer>{
	public:
	};

	// VertexShader

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>;
	//	用途: VSBasicPNT 頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>{
	public:
		//構築
		VSBasicPNT();
	};

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT : public PixelShader<PSBasicPNT>;
	//	用途: PSBasicPNTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSBasicPNT : public PixelShader<PSBasicPNT>{
	public:
		//構築
		PSBasicPNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>;
	//	用途: PSBasicPNT2ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>{
	public:
		//構築
		PSBasicPNT2();
	};



	//--------------------------------------------------------------------------------------
	//	スキンメッシュ用
	//--------------------------------------------------------------------------------------

	struct BasicBoneConstantBuffer
	{
		XMVECTOR diffuseColor;
		XMVECTOR emissiveColor;
		XMVECTOR specularColorAndPower;

		XMVECTOR lightDirection[ShaderResource::m_LightMax];
		XMVECTOR lightDiffuseColor[ShaderResource::m_LightMax];
		XMVECTOR lightSpecularColor[ShaderResource::m_LightMax];

		XMVECTOR eyePosition;

		XMVECTOR fogColor;
		XMVECTOR fogVector;

		XMMATRIX world;
		XMVECTOR worldInverseTranspose[3];
		XMMATRIX worldViewProj;

		XMVECTOR Bones[3 * 72];

		BasicBoneConstantBuffer() {
			memset(this, 0, sizeof(BasicBoneConstantBuffer));
			diffuseColor = g_XMOne;
			emissiveColor = g_XMZero;
			specularColorAndPower = g_XMZero;
			static const XMVECTORF32 defaultLightDirection = { 0, -1, 0, 0 };
			for (int i = 0; i < ShaderResource::m_LightMax; i++)
			{
				lightDirection[i] = defaultLightDirection;
				lightDiffuseColor[i] = g_XMZero;
				lightSpecularColor[i] = g_XMZero;
			}
		};
	};

	struct BasicBoneShadowConstantBuffer{
		XMUINT4 m_ActiveFlg;			//影がアクティブかどうか,ライトを無効にするかどうか
		//y=Shadow,w=NoLight
		BasicBoneShadowConstantBuffer(){
			memset(this, 0, sizeof(BasicBoneShadowConstantBuffer));
		}
	};


	//コンスタントバッファ

	//--------------------------------------------------------------------------------------
	//	class CBBasicBone : public ConstantBuffer<CBBasicBone,BasicBoneConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBBasicBone : public ConstantBuffer<CBBasicBone, BasicBoneConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow,BasicBoneShadowConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow, BasicBoneShadowConstantBuffer>{
	public:
	};


	
	// VertexShader

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSBasicPNTBone 頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>{
	public:
		//構築
		VSBasicPNTBone();
	};

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>;
	//	用途: PSBasicPNTBoneピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>{
	public:
		//構築
		PSBasicPNTBone();
	};






}

//end basedx11