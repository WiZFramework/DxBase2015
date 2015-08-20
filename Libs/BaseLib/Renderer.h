#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class RenderState;
	//	�p�r: �����_�����O�X�e�[�g�N���X
	//--------------------------------------------------------------------------------------
	class RenderState{
	public:
		//--------------------------------------------------------------------------------------
		//	RenderState();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		RenderState();
		//�f�X�g���N�^
		virtual ~RenderState();

		//--------------------------------------------------------------------------------------
		//	�p�r: �u�����h�X�e�[�g�A�N�Z�b�T
		//	�߂�l: �u�����h�X�e�[�g�̃|�C���^
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetOpaque()const;
		ID3D11BlendState* GetAlphaBlend()const;
		ID3D11BlendState* GetAlphaBlendEx()const;
		ID3D11BlendState* GetAdditive()const;
		ID3D11BlendState* GetNonPremultiplied()const;
		//--------------------------------------------------------------------------------------
		//	�p�r: �f�v�X�X�e���V���X�e�[�g�A�N�Z�b�T
		//	�߂�l: �f�v�X�X�e���V���X�e�[�g�̃|�C���^
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilState* GetDepthNone()const;
		ID3D11DepthStencilState* GetDepthDefault()const;
		ID3D11DepthStencilState* GetDepthRead()const;
		//--------------------------------------------------------------------------------------
		//	�p�r: ���X�^���C�U�X�e�[�g�A�N�Z�b�T
		//	�߂�l: ���X�^���C�U�X�e�[�g�̃|�C���^
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullNone()const;
		ID3D11RasterizerState* GetCullNoneScissor()const;
		ID3D11RasterizerState* GetCullFront()const;
		ID3D11RasterizerState* GetCullFrontScissor()const;
		ID3D11RasterizerState* GetCullBack()const;
		ID3D11RasterizerState* GetCullBackScissor()const;
		ID3D11RasterizerState* GetWireframe()const;
		ID3D11RasterizerState* GetWireframeScissor()const;
		//--------------------------------------------------------------------------------------
		//	�p�r: �T���v���[�X�e�[�g�A�N�Z�b�T
		//	�߂�l: �T���v���[�X�e�[�g�̃|�C���^
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetPointWrap()const;
		ID3D11SamplerState* GetPointClamp()const;
		ID3D11SamplerState* GetLinearWrap()const;
		ID3D11SamplerState* GetLinearClamp()const;
		ID3D11SamplerState* GetAnisotropicWrap()const;
		ID3D11SamplerState* GetAnisotropicClamp()const;
		ID3D11SamplerState* GetComparisonLinear()const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		RenderState(const RenderState&) = delete;
		RenderState& operator=(const RenderState&) = delete;
		//���[�u�֎~
		RenderState(const RenderState&&) = delete;
		RenderState& operator=(const RenderState&&) = delete;
	};



	//--------------------------------------------------------------------------------------
	//	class RenderTarget;
	//	�p�r: �����_�[�^�[�Q�b�g���N���X
	//--------------------------------------------------------------------------------------
	class RenderTarget{
	protected:
		//�\�z�Ɣj��
		RenderTarget(){}
		virtual ~RenderTarget(){}
	public:
		//�����_�����O�^�[�Q�b�g���J�n����
		//	���������z�֐�
		virtual void ClearViews() = 0;
		//	���������z�֐�
		virtual void StartRenderTarget() = 0;
		//�����_�����O�^�[�Q�b�g���I������
		//	���������z�֐�
		virtual void EndRenderTarget() = 0;
	private:
		//�R�s�[�֎~
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		//���[�u�֎~
		RenderTarget(const RenderTarget&&) = delete;
		RenderTarget& operator=(const RenderTarget&&) = delete;
	};

	class View;
	class Stage;

	//--------------------------------------------------------------------------------------
	//	class ShadowMapRenderTarget: public RenderTarget;
	//	�p�r: �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	class ShadowMapRenderTarget : public RenderTarget{
	public:
		//�\�z�Ɣj��
		ShadowMapRenderTarget();
		virtual ~ShadowMapRenderTarget();

		//�A�N�Z�T
		float GetShadowMapDimension() const;
		ID3D11ShaderResourceView* GetShaderResourceView() const;
		ID3D11Texture2D*		GetDepthStencil() const;
		ID3D11DepthStencilView*	GetDepthStencilView() const;

		//����
		//�����_�����O�^�[�Q�b�g���N���A����
		virtual void ClearViews()override;
		//�����_�����O�^�[�Q�b�g���J�n����
		virtual void StartRenderTarget()override;
		//�����_�����O�^�[�Q�b�g���I������
		virtual void EndRenderTarget()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	�p�r: �f�t�H���g�̃����_�[�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	class DefaultRenderTarget : public RenderTarget{
	public:
		//�\�z�Ɣj��
		explicit DefaultRenderTarget(const shared_ptr<Stage>& stage);
		virtual ~DefaultRenderTarget();

		//�A�N�Z�T
		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11Texture2D*		GetDepthStencil() const;
		ID3D11DepthStencilView*	GetDepthStencilView() const;
		ID2D1Bitmap1*			GetD2DTargetBitmap() const;

		//����
		//�X�N���[���S�̂����ŃN���A����
		virtual void ClearViews()override;
		//�����_�����O�^�[�Q�b�g���J�n����
		virtual void StartRenderTarget()override;
		//�����_�����O�^�[�Q�b�g���I������
		virtual void EndRenderTarget()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





}
//end basedx11
