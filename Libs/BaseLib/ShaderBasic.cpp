#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBShadow, CBShadow::Deleter> CBShadow::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	用途: VSShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSShadowmap, VSShadowmap::Deleter> VSShadowmap::m_Ptr;
	//構築
	VSShadowmap::VSShadowmap():
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSShadowmapBone頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSShadowmapBone, VSShadowmapBone::Deleter> VSShadowmapBone::m_Ptr;
	//構築
	VSShadowmapBone::VSShadowmapBone() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmapBone.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	用途: PSShadowmapピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSShadowmap, PSShadowmap::Deleter> PSShadowmap::m_Ptr;
	//構築
	PSShadowmap::PSShadowmap() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSShadowmap.cso")
	{}


	//--------------------------------------------------------------------------------------
	//	class CBSprite : public ConstantBuffer<CBSprite,SpriteConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBSprite, CBSprite::Deleter> CBSprite::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSpritePCT : public VertexShader<VSSpritePCT>;
	//	用途: VSSpritePCT頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSSpritePCT, VSSpritePCT::Deleter> VSSpritePCT::m_Ptr;
	//構築
	VSSpritePCT::VSSpritePCT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSpritePCT.cso"){}
	//--------------------------------------------------------------------------------------
	//	class PSSpritePCT : public PixelShader<PSSpritePCT>;
	//	用途: PSSpritePCTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSpritePCT, PSSpritePCT::Deleter> PSSpritePCT::m_Ptr;
	//構築
	PSSpritePCT::PSSpritePCT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSpritePCT.cso")
	{}


	//SimpleVertexShader

	//--------------------------------------------------------------------------------------
	//	class CBSimple : public ConstantBuffer<CBSimple>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBSimple, CBSimple::Deleter> CBSimple::m_Ptr;


	//--------------------------------------------------------------------------------------
	//	class VSSimplePNT : public VertexShader<VSSimplePNT>;
	//	用途: VSSimplePNT頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSSimplePNT, VSSimplePNT::Deleter> VSSimplePNT::m_Ptr;
	//構築
	VSSimplePNT::VSSimplePNT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimplePNT.cso"){}



	//SimplePixelShader


	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT : public PixelShader<PSSimplePNT>;
	//	用途: PSSimplePNTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimplePNT, PSSimplePNT::Deleter> PSSimplePNT::m_Ptr;
	//構築
	PSSimplePNT::PSSimplePNT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimplePNT.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSSimplePNT2 : public PixelShader<PSSimplePNT2>;
	//	用途: PSSimplePNT2ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimplePNT2, PSSimplePNT2::Deleter> PSSimplePNT2::m_Ptr;
	//構築
	PSSimplePNT2::PSSimplePNT2() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimplePNT2.cso")
	{}



	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBSimpleDirect, CBSimpleDirect::Deleter> CBSimpleDirect::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	用途: VSSimpleDirect頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSSimpleDirect, VSSimpleDirect::Deleter> VSSimpleDirect::m_Ptr;
	//構築
	VSSimpleDirect::VSSimpleDirect() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleDirect.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	用途: PSSimpleピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimpleDirect, PSSimpleDirect::Deleter> PSSimpleDirect::m_Ptr;
	//構築
	PSSimpleDirect::PSSimpleDirect() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleDirect.cso")
	{
	}


	//--------------------------------------------------------------------------------------

	// BasicVertexShader

	//--------------------------------------------------------------------------------------
	//	class CBBasic : public ConstantBuffer<CBBasic,BasicConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBBasic, CBBasic::Deleter> CBBasic::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSBasicPNT : public VertexShader<VSBasicPNT, VertexPositionNormalTexture>;
	//	用途: VSBasicPNT 頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSBasicPNT, VSBasicPNT::Deleter> VSBasicPNT::m_Ptr;
	//構築
	VSBasicPNT::VSBasicPNT() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSBasicPNT.cso"){}


	//BasicPixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT : public PixelShader<PSBasicPNT>;
	//	用途: PSBasicPNTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSBasicPNT, PSBasicPNT::Deleter> PSBasicPNT::m_Ptr;
	//構築
	PSBasicPNT::PSBasicPNT() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNT.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNT2 : public PixelShader<PSBasicPNT2>;
	//	用途: PSBasicPNT2ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSBasicPNT2, PSBasicPNT2::Deleter> PSBasicPNT2::m_Ptr;
	//構築
	PSBasicPNT2::PSBasicPNT2() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNT2.cso")
	{}




	//--------------------------------------------------------------------------------------
	//	スキンメッシュ用
	//--------------------------------------------------------------------------------------

	//コンスタントバッファ

	//--------------------------------------------------------------------------------------
	//	class CBBasicBone : public ConstantBuffer<CBBasicBone,BasicBoneConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBBasicBone, CBBasicBone::Deleter> CBBasicBone::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class CBBasicBoneShadow : public ConstantBuffer<CBBasicBoneShadow,BasicBoneShadowConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBBasicBoneShadow, CBBasicBoneShadow::Deleter> CBBasicBoneShadow::m_Ptr;


	// VertexShader
	//--------------------------------------------------------------------------------------
	//	class VSBasicPNTBone : public VertexShader<VSBasicPNTBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSBasicPNTBone 頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSBasicPNTBone, VSBasicPNTBone::Deleter> VSBasicPNTBone::m_Ptr;
	//構築
	VSBasicPNTBone::VSBasicPNTBone() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSBasicPNTBone.cso"){}

	//PixelShader

	//--------------------------------------------------------------------------------------
	//	class PSBasicPNTBone : public PixelShader<PSBasicPNTBone>;
	//	用途: PSBasicPNTBoneピクセルシェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<PSBasicPNTBone, PSBasicPNTBone::Deleter> PSBasicPNTBone::m_Ptr;
	//構築
	PSBasicPNTBone::PSBasicPNTBone() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSBasicPNTBone.cso")
	{}


}
//end basedx11
