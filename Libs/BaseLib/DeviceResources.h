/*!
@file DeviceResources.h
@brief Dx11�f�o�C�X���\�[�X

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{
	class RenderState;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX11�f�o�C�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	DeviceResources(
		//		HWND hWnd,		//�E�C���h�E�̃n���h�� 
		//		bool isFullScreen, //�t���X�N���[�����ǂ���
		//		UINT Width,			//��
		//		UINT Height			//����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param HWND hWnd,	�E�C���h�E�̃n���h��
		@param bool isFullScreen, �t���X�N���[�����ǂ���
		@param UINT Width,		��
		@param UINT Height		����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		//	virtual ~DeviceResources();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//D3D�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ID3D11Device2*	GetD3DDevice() const;
		/*!
		@breaf D3D11Device2�C���^�[�t�F�C�X�̎擾
		@param �Ȃ�
		@return	D3D11Device2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Device2* GetD3DDevice() const;
		ID3D11DeviceContext2* GetD3DDeviceContext() const;
		IDXGISwapChain1* GetSwapChain() const;
		D3D_FEATURE_LEVEL GetFeatureLevel() const;

		// D2D �A�N�Z�T
		ID2D1Factory2* GetD2DFactory() const;
		ID2D1Device1* GetD2DDevice() const;
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
//		ID2D1Bitmap1* GetD2DTargetBitmap() const;
		IDWriteFactory2* GetDWriteFactory() const;
		IWICImagingFactory2* GetWicImagingFactory() const;
		void InitializeStates(const shared_ptr<RenderState>& StatePtr);	//��n��

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;
		//���[�u�֎~
		DeviceResources(const DeviceResources&&) = delete;
		DeviceResources& operator=(const DeviceResources&&) = delete;
	};

}

//end basedx11
