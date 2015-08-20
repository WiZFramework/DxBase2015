#pragma once

#include "stdafx.h"

namespace basedx11{

	//Shadow

	struct ShadowConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMVECTOR Bones[3 * 72];	//Bone�p
		ShadowConstantBuffer() {
			memset(this, 0, sizeof(ShadowConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow,ShadowConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBShadow : public ConstantBuffer<CBShadow, ShadowConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	�p�r: VSShadowmap���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>{
	public:
		//�\�z
		VSShadowmap();
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	�p�r: VSShadowmapBone���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>{
	public:
		//�\�z
		VSShadowmapBone();
	};

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	�p�r: PSShadowmap�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSShadowmap : public PixelShader<PSShadowmap>{
	public:
		//�\�z
		PSShadowmap();
	};


	//--------------------------------------------------------------------------------------
	//�X�v���C�g

	struct SpriteConstantBuffer
	{
		//�]�u���ĂȂ��s��
		XMMATRIX mTransformMatrix;
	};

	//--------------------------------------------------------------------------------------
	//	class CBSprite : public ConstantBuffer<CBSprite,SpriteConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBSprite : public ConstantBuffer<CBSprite, SpriteConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSSpritePCT : public VertexShader<VSSpritePCT>;
	//	�p�r: VSSpritePCT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSSpritePCT : public VertexShader<VSSpritePCT, VertexPositionColorTexture>{
	public:
		//�\�z
		VSSpritePCT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSpritePCT : public PixelShader<PSSpritePCT>;
	//	�p�r: PSSpritePCT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSSpritePCT : public PixelShader<PSSpritePCT>{
	public:
		//�\�z
		PSSpritePCT();
	};

	//Simple
	//
	//--------------------------------------------------------------------------------------
	//	struct SimpleConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct SimpleConstantBuffer
	{
		Matrix4X4 m_Model;
		Matrix4X4 m_View;
		Matrix4X4 m_Projection;
		Vector4 m_Direction;
		Color4 m_DiffuseColor;
		Matrix4X4 m_LightWorldViewProj;   // ���C�g�r���[�̐��ˉe�s��i�e�p�j
		XMUINT4 m_ActiveFlg;			//�e�N�X�`���Ɖe���A�N�e�B�u���ǂ���
		Vector4 m_LPos;					//���C�g�̈ʒu
		Matrix4X4 m_LView;				//���C�g�r���[
		Matrix4X4 m_LProjection;			//���C�g�v���W�F�N�V����
		Vector4 m_EyePos;					//�J�����̈ʒu
		//x=Texture,y=Shadow
		SimpleConstantBuffer() {
			memset(this, 0, sizeof(SimpleConstantBuffer));
		};
	};


	//--------------------------------------------------------------------------------------
	//	class CBSimple : public ConstantBuffer<CBSimple,SimpleConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBSimple : public ConstantBuffer<CBSimple, SimpleConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSSimplePNT : public VertexShader<VSSimplePNT, VertexPositionNormalTexture>;
	//	�p�r: VSSimplePNT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSSimplePNT : public VertexShader<VSSimplePNT, VertexPositionNormalTexture>{
	public:
		//�\�z
		VSSimplePNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT : public PixelShader<PSSimplePNT>;
	//	�p�r: PSSimplePNT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSSimplePNT : public PixelShader<PSSimplePNT>{
	public:
		//�\�z
		PSSimplePNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>;
	//	�p�r: PSSimplePNT2�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>{
	public:
		//�\�z
		PSSimplePNT2();
	};



	//--------------------------------------------------------------------------------------
	//	struct SimpleDirectConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
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
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect, SimpleDirectConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	�p�r: VSSimpleDirect���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>{
	public:
		//�\�z
		VSSimpleDirect();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	�p�r: PSSimple�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSSimpleDirect : public PixelShader<PSSimpleDirect>{
	public:
		//�\�z
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

		XMMATRIX LightWorldViewProj;   // ���C�g�r���[�̐��ˉe�s��

		XMUINT4 m_ActiveFlg;			//�e�N�X�`���Ɖe���A�N�e�B�u���ǂ���
										//x=Texture,y=Shadow,z=lightcount

		Vector4 m_LPos;					//���C�g�̈ʒu
		Matrix4X4 m_LView;				//���C�g�r���[
		Matrix4X4 m_LProjection;			//���C�g�v���W�F�N�V����
		Vector4 m_EyePos;					//�J�����̈ʒu


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


	//�R���X�^���g�o�b�t�@

	//--------------------------------------------------------------------------------------
	//	class CBBasic : public ConstantBuffer<CBBasic,BasicConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBBasic : public ConstantBuffer<CBBasic, BasicConstantBuffer>{
	public:
	};

	// VertexShader

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>;
	//	�p�r: VSBasicPNT ���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>{
	public:
		//�\�z
		VSBasicPNT();
	};

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT : public PixelShader<PSBasicPNT>;
	//	�p�r: PSBasicPNT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSBasicPNT : public PixelShader<PSBasicPNT>{
	public:
		//�\�z
		PSBasicPNT();
	};

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>;
	//	�p�r: PSBasicPNT2�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>{
	public:
		//�\�z
		PSBasicPNT2();
	};



	//--------------------------------------------------------------------------------------
	//	�X�L�����b�V���p
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
		XMUINT4 m_ActiveFlg;			//�e���A�N�e�B�u���ǂ���,���C�g�𖳌��ɂ��邩�ǂ���
		//y=Shadow,w=NoLight
		BasicBoneShadowConstantBuffer(){
			memset(this, 0, sizeof(BasicBoneShadowConstantBuffer));
		}
	};


	//�R���X�^���g�o�b�t�@

	//--------------------------------------------------------------------------------------
	//	class CBBasicBone : public ConstantBuffer<CBBasicBone,BasicBoneConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBBasicBone : public ConstantBuffer<CBBasicBone, BasicBoneConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow,BasicBoneShadowConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow, BasicBoneShadowConstantBuffer>{
	public:
	};


	
	// VertexShader

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>;
	//	�p�r: VSBasicPNTBone ���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>{
	public:
		//�\�z
		VSBasicPNTBone();
	};

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>;
	//	�p�r: PSBasicPNTBone�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>{
	public:
		//�\�z
		PSBasicPNTBone();
	};






}

//end basedx11