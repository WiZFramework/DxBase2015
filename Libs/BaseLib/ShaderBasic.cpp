#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBShadow, CBShadow::Deleter> CBShadow::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	�p�r: VSShadowmap���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSShadowmap, VSShadowmap::Deleter> VSShadowmap::m_Ptr;
	//�\�z
	VSShadowmap::VSShadowmap():
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	�p�r: VSShadowmapBone���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSShadowmapBone, VSShadowmapBone::Deleter> VSShadowmapBone::m_Ptr;
	//�\�z
	VSShadowmapBone::VSShadowmapBone() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmapBone.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	�p�r: PSShadowmap�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSShadowmap, PSShadowmap::Deleter> PSShadowmap::m_Ptr;
	//�\�z
	PSShadowmap::PSShadowmap() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSShadowmap.cso")
	{}


	//--------------------------------------------------------------------------------------
	//	class CBSprite : public ConstantBuffer<CBSprite,SpriteConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBSprite, CBSprite::Deleter> CBSprite::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSpritePCT : public VertexShader<VSSpritePCT>;
	//	�p�r: VSSpritePCT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSSpritePCT, VSSpritePCT::Deleter> VSSpritePCT::m_Ptr;
	//�\�z
	VSSpritePCT::VSSpritePCT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSpritePCT.cso"){}
	//--------------------------------------------------------------------------------------
	//	class PSSpritePCT : public PixelShader<PSSpritePCT>;
	//	�p�r: PSSpritePCT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSpritePCT, PSSpritePCT::Deleter> PSSpritePCT::m_Ptr;
	//�\�z
	PSSpritePCT::PSSpritePCT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSpritePCT.cso")
	{}


	//SimpleVertexShader

	//--------------------------------------------------------------------------------------
	//	class CBSimple : public ConstantBuffer<CBSimple>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBSimple, CBSimple::Deleter> CBSimple::m_Ptr;


	//--------------------------------------------------------------------------------------
	//	class VSSimplePNT : public VertexShader<VSSimplePNT>;
	//	�p�r: VSSimplePNT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSSimplePNT, VSSimplePNT::Deleter> VSSimplePNT::m_Ptr;
	//�\�z
	VSSimplePNT::VSSimplePNT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimplePNT.cso"){}



	//SimplePixelShader


	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT : public PixelShader<PSSimplePNT>;
	//	�p�r: PSSimplePNT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimplePNT, PSSimplePNT::Deleter> PSSimplePNT::m_Ptr;
	//�\�z
	PSSimplePNT::PSSimplePNT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimplePNT.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>;
	//	�p�r: PSSimplePNT2�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimplePNT2, PSSimplePNT2::Deleter> PSSimplePNT2::m_Ptr;
	//�\�z
	PSSimplePNT2::PSSimplePNT2() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimplePNT2.cso")
	{}



	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBSimpleDirect, CBSimpleDirect::Deleter> CBSimpleDirect::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	�p�r: VSSimpleDirect���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSSimpleDirect, VSSimpleDirect::Deleter> VSSimpleDirect::m_Ptr;
	//�\�z
	VSSimpleDirect::VSSimpleDirect() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleDirect.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	�p�r: PSSimple�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimpleDirect, PSSimpleDirect::Deleter> PSSimpleDirect::m_Ptr;
	//�\�z
	PSSimpleDirect::PSSimpleDirect() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleDirect.cso")
	{
	}


	//--------------------------------------------------------------------------------------

	// BasicVertexShader

	//--------------------------------------------------------------------------------------
	//	class CBBasic : public ConstantBuffer<CBBasic,BasicConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBBasic, CBBasic::Deleter> CBBasic::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>;
	//	�p�r: VSBasicPNT ���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSBasicPNT, VSBasicPNT::Deleter> VSBasicPNT::m_Ptr;
	//�\�z
	VSBasicPNT::VSBasicPNT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSBasicPNT.cso"){}


	//BasicPixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT : public PixelShader<PSBasicPNT>;
	//	�p�r: PSBasicPNT�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSBasicPNT, PSBasicPNT::Deleter> PSBasicPNT::m_Ptr;
	//�\�z
	PSBasicPNT::PSBasicPNT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNT.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>;
	//	�p�r: PSBasicPNT2�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSBasicPNT2, PSBasicPNT2::Deleter> PSBasicPNT2::m_Ptr;
	//�\�z
	PSBasicPNT2::PSBasicPNT2() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNT2.cso")
	{}




	//--------------------------------------------------------------------------------------
	//	�X�L�����b�V���p
	//--------------------------------------------------------------------------------------

	//�R���X�^���g�o�b�t�@

	//--------------------------------------------------------------------------------------
	//	class CBBasicBone : public ConstantBuffer<CBBasicBone,BasicBoneConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBBasicBone, CBBasicBone::Deleter> CBBasicBone::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow,BasicBoneShadowConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBBasicBoneShadow, CBBasicBoneShadow::Deleter> CBBasicBoneShadow::m_Ptr;


	// VertexShader
	//--------------------------------------------------------------------------------------
	//	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>;
	//	�p�r: VSBasicPNTBone ���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSBasicPNTBone, VSBasicPNTBone::Deleter> VSBasicPNTBone::m_Ptr;
	//�\�z
	VSBasicPNTBone::VSBasicPNTBone() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSBasicPNTBone.cso"){}

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>;
	//	�p�r: PSBasicPNTBone�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<PSBasicPNTBone, PSBasicPNTBone::Deleter> PSBasicPNTBone::m_Ptr;
	//�\�z
	PSBasicPNTBone::PSBasicPNTBone() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNTBone.cso")
	{}


}
//end basedx11
