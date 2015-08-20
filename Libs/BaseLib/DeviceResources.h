/*!
@file DeviceResources.h
@brief Dx11デバイスリソース

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{
	class RenderState;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX11デバイスクラス
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	DeviceResources(
		//		HWND hWnd,		//ウインドウのハンドル 
		//		bool isFullScreen, //フルスクリーンかどうか
		//		UINT Width,			//幅
		//		UINT Height			//高さ
		//	);
		/*!
		@breaf コンストラクタ
		@param HWND hWnd,	ウインドウのハンドル
		@param bool isFullScreen, フルスクリーンかどうか
		@param UINT Width,		幅
		@param UINT Height		高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		//	virtual ~DeviceResources();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//D3Dアクセサ
		//--------------------------------------------------------------------------------------
		//	ID3D11Device2*	GetD3DDevice() const;
		/*!
		@breaf D3D11Device2インターフェイスの取得
		@param なし
		@return	D3D11Device2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Device2* GetD3DDevice() const;
		ID3D11DeviceContext2* GetD3DDeviceContext() const;
		IDXGISwapChain1* GetSwapChain() const;
		D3D_FEATURE_LEVEL GetFeatureLevel() const;

		// D2D アクセサ
		ID2D1Factory2* GetD2DFactory() const;
		ID2D1Device1* GetD2DDevice() const;
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
//		ID2D1Bitmap1* GetD2DTargetBitmap() const;
		IDWriteFactory2* GetDWriteFactory() const;
		IWICImagingFactory2* GetWicImagingFactory() const;
		void InitializeStates(const shared_ptr<RenderState>& StatePtr);	//後始末

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;
		//ムーブ禁止
		DeviceResources(const DeviceResources&&) = delete;
		DeviceResources& operator=(const DeviceResources&&) = delete;
	};

}

//end basedx11
