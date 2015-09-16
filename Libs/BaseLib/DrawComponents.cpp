#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl{
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース

		Matrix4X4 m_MeshToTransform;

		Impl():
			m_MeshToTransform()
		{}
		~Impl(){}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(64.0f);
	float Shadowmap::Impl::m_ViewHeight(64.0f);


	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	用途: シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap(){}

	float Shadowmap::GetLightHeight(){ return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear(){ return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar(){ return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth(){ return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight(){ return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f){ Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f){ Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f){ Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f){ Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f){ Impl::m_ViewHeight = f; }


	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	const Matrix4X4& Shadowmap::GetMeshToTransform() const{
		return pImpl->m_MeshToTransform;
	}
	void Shadowmap::SetMeshToTransform(const Matrix4X4& Mat){
		pImpl->m_MeshToTransform = Mat;
	}



	void Shadowmap::Draw(){
		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			//メッシュリソースの取得
			auto PtrMeshResource = GetMeshResource();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrMeshResource && PtrLight && PtrCamera){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();


				//ライトの取得
				Matrix4X4 LightView, LightProj;

				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightAt + (LightDir * GetLightHeight());

				//ライトのビューと射影を計算
				LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
				LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

				ShadowConstantBuffer Cb;

				Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				bool IsSkin = false;
				bool IsSkinStride = false;
				if (PtrMeshResource->IsSkining()){
					auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
					if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()){
						if (pLocalBoneVec){
							//ボーンの設定
							size_t BoneSz = pLocalBoneVec->size();
							UINT cb_count = 0;
							for (size_t b = 0; b < BoneSz; b++){
								Matrix4X4 mat = (*pLocalBoneVec)[b].m_ConbinedPose;
								mat.Transpose();
								Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
								Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
								Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
								cb_count += 3;
							}
							IsSkin = true;
						}
					}
					IsSkinStride = true;
				}


				//これより描画処理
				//コンスタントバッファの更新
				pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				if (IsSkin){
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
					//ストライドとオフセット
					UINT stride = sizeof(VertexPositionNormalTextureSkinning);
					UINT offset = 0;
					//頂点バッファをセット
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
					//ストライドとオフセット
					UINT stride = sizeof(VertexPositionNormalTexture);
					if (IsSkinStride){
						stride = sizeof(VertexPositionNormalTextureSkinning);
					}
					UINT offset = 0;
					//頂点バッファをセット
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				}



				//インデックスバッファのセット
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//ピクセルシェーダはセットしない！
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//ジオメトリシェーダの設定（使用しない）
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//コンスタントバッファをピクセルシェーダにセット
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//描画
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SimpleDirectDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SimpleDirectDraw::Impl{
		Color4 m_Diffuse;	//デフィーズ
		//背面をクリアするためのバッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		UINT m_NumVertices;				//頂点の数
		D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;	//描画方法
		Impl() :
			m_Diffuse(0.5f, 0.5f, 0.5f, 1.0f),
			m_NumVertices(0),
			m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class SimpleDirectDraw : public DrawComponent;
	//	用途: SimpleDirectDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	SimpleDirectDraw::SimpleDirectDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SimpleDirectDraw::~SimpleDirectDraw(){}
	//アクセサ
	const Color4& SimpleDirectDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void SimpleDirectDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }

	D3D11_PRIMITIVE_TOPOLOGY SimpleDirectDraw::GetPrimitiveTopology() const{
		return pImpl->m_PrimitiveTopology;
	}
	void SimpleDirectDraw::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY t){
		pImpl->m_PrimitiveTopology = t;
	}


	//頂点配列を渡して頂点バッファを作成する
	void SimpleDirectDraw::CreateVertex(const vector<VertexPosition>& Verteces){
		try{
			VertexUtil::CreateVertexBuffer(pImpl->m_VertexBuffer, Verteces);
			//頂点数の設定
			pImpl->m_NumVertices = static_cast<UINT>(Verteces.size());
		}
		catch (...){
			throw;
		}
	}

	void SimpleDirectDraw::Draw(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			//もしステージがnullだったら、オブジェクトがステージの可能性がある
			PtrStage = dynamic_pointer_cast<Stage>(PtrGameObject);
			if (!PtrStage){
				return;
			}
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto RenderStatePtr = PtrStage->GetRenderState();

		//コンスタントバッファの準備
		SimpleDirectConstantBuffer sb;
		sb.m_DiffuseColor = GetDiffuse();
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(GetPrimitiveTopology());

		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//コンスタントバッファの設定(ピクセルシェーダのみでよい)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());

		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルは使用しない
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//
		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);

		//描画
		pD3D11DeviceContext->Draw(pImpl->m_NumVertices, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);

	}






	//--------------------------------------------------------------------------------------
	//	struct SimplePNTDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SimplePNTDraw::Impl{
		Color4 m_Diffuse;	//デフィーズ
		bool m_OwnShadowActive;
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャリソース
		ID3D11SamplerState* m_pSamplerState;	//サンプラーステート（オプション）
		bool m_ZBufferUse;		//Zバッファを有効にするかどうか
		bool m_AlphaBlendSrcOne;	//透明処理のSRC_ONE設定
		size_t m_Virsion;		//シェーダーのバージョン
		Impl() :
			m_Diffuse(0.5f, 0.5f, 0.5f, 1.0f),
			m_OwnShadowActive(false),
			m_pSamplerState(nullptr),
			m_ZBufferUse(true),
			m_AlphaBlendSrcOne(false),
			m_Virsion(2)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SimplePNTDraw : public Component;
	//	用途: SimplePNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	SimplePNTDraw::SimplePNTDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SimplePNTDraw::~SimplePNTDraw(){}
	//アクセサ
	const Color4& SimplePNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void SimplePNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }

	bool SimplePNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool SimplePNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void SimplePNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	bool SimplePNTDraw::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;

	}
	bool SimplePNTDraw::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void SimplePNTDraw::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool SimplePNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool SimplePNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void SimplePNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	size_t SimplePNTDraw::GetShaderVirsion() const{
		return pImpl->m_Virsion;
	}
	void SimplePNTDraw::SetShaderVirsion(size_t v){
		if (v <= 1){
			v = 1;
		}
		else if (v >= 2){
			v = 2;
		}
		pImpl->m_Virsion = v;
	}



	void SimplePNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}


	shared_ptr<MeshResource> SimplePNTDraw::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"SimplePNTDraw::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void SimplePNTDraw::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"SimplePNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void SimplePNTDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	shared_ptr<TextureResource> SimplePNTDraw::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"SimplePNTDraw::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void SimplePNTDraw::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"SimplePNTDraw::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void SimplePNTDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void SimplePNTDraw::Draw(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		auto PtrMeshResource = GetMeshResource();
		//テクスチャは必須ではない
		auto PtrTextureResource = GetTextureResource(false);

		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージから0番目のライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);

		if (PtrMeshResource && PtrCamera && PtrLight){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//ステータスのポインタ
			auto RenderStatePtr = PtrStage->GetRenderState();
			//シャドウマップのレンダラーターゲット
			auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

			//カメラの取得
			Matrix4X4 View, Proj;
			View = PtrCamera->GetViewMatrix();
			Proj = PtrCamera->GetProjMatrix();

			//コンスタントバッファの設定
			SimpleConstantBuffer sb;
			sb.m_Model = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());
			sb.m_View = Matrix4X4EX::Transpose(View);
			sb.m_Projection = Matrix4X4EX::Transpose(Proj);
			sb.m_Direction = PtrLight->GetDirectional();
			sb.m_DiffuseColor = pImpl->m_Diffuse;
			if (IsOwnShadowActive()){
				//ライトの取得
				Matrix4X4 LightView, LightProj, LightViewProj;
				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightDir;
				LightEye *= Shadowmap::GetLightHeight();
				LightEye = LightAt + LightEye;

				//ライトのビューと射影を計算
				LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
				LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
					Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
				LightViewProj = LightView * LightProj;
				Matrix4X4 LWMatrix = PtrT->GetWorldMatrix() * LightViewProj;
				//コンスタントバッファに設定
				sb.m_LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
				sb.m_ActiveFlg.y = 1;
				sb.m_LPos = LightEye;
				sb.m_LPos.w = 0;
				sb.m_LView = Matrix4X4EX::Transpose(LightView);
				sb.m_LProjection = Matrix4X4EX::Transpose(LightProj);
				sb.m_EyePos = PtrCamera->GetEye();
				sb.m_EyePos.w = 0;
			}
			if (PtrTextureResource){
				sb.m_ActiveFlg.x = 1;
			}


			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);


			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//頂点バッファの設定
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			if (PtrTextureResource){
				//リニアサンプラーを設定
				ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
				if (pImpl->m_pSamplerState){
					samplerState = pImpl->m_pSamplerState;
				}
				pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
				//テクスチャを設定
				pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
			}
			else{
				//サンプラーは使用しない
				pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
				//テクスチャも使用しない
				pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
			}

			if (GetShaderVirsion() >= 2){
				//ピクセルシェーダの設定
				pID3D11DeviceContext->PSSetShader(PSSimplePNT2::GetPtr()->GetShader(), nullptr, 0);
			}
			else{
				//ピクセルシェーダの設定
				pID3D11DeviceContext->PSSetShader(PSSimplePNT::GetPtr()->GetShader(), nullptr, 0);
			}

			//頂点シェーダの設定
			pID3D11DeviceContext->VSSetShader(VSSimplePNT::GetPtr()->GetShader(), nullptr, 0);

			//インプットレイアウトの設定
			pID3D11DeviceContext->IASetInputLayout(VSSimplePNT::GetPtr()->GetInputLayout());

			if (IsOwnShadowActive()){
				//シャドウマップのリソースビューを取得
				ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
				pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//シャドウ用にリニアサンプラーを取得
				ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetComparisonLinear();
				if (pImpl->m_Virsion <= 1){
					//バージョンが低い場合はテクスチャと同じサンプラーを使用
					pShadowSamplerState = RenderStatePtr->GetLinearClamp();
				}
				//シャドウ用リニアサンプラーを設定
				pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
			}
			else{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
				pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
			}

			if (pImpl->m_ZBufferUse){
				//デプスステンシルは使用する
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//デプスステンシルは使用しない
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);

			}


			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			if (PtrGameObject->IsAlphaActive()){
				if (IsAlphaBlendSrcOne()){
					//透明処理
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
				else{
					//半透明処理
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//描画
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			}
			else{
				//塗りつぶし
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates(RenderStatePtr);


		}
	}



	//--------------------------------------------------------------------------------------
	//	struct BasicPNTDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct BasicPNTDraw::Impl{
		Color4 m_Diffuse;	//デフィーズ
		Color4 m_Emissive;	//エミッシブ
		Color4 m_SpecularAndPower;	//スペキュラーとパワー
		bool m_OwnShadowActive;		//自身への影投影
		bool m_CullNone;		//両面描画かどうか
		bool m_ZBufferUse;		//Zバッファを有効にするかどうか
		bool m_AlphaBlendSrcOne;	//透明処理のSRC_ONE設定
		size_t m_Virsion;		//シェーダーのバージョン
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャリソース
		bool m_TextureOnlyNoLight;				//テクスチャオンリーかどうか
		ID3D11SamplerState* m_pSamplerState;	//サンプラーステート（オプション）
		Impl() :
			m_Diffuse(0.7f, 0.7f, 0.7f, 1.0f),
			m_Emissive(0.4f, 0.4f, 0.4f, 1.0f),
			m_SpecularAndPower(0.4f, 0.4f, 0.4f, 1.0f),
			m_OwnShadowActive(false),
			m_CullNone(false),
			m_ZBufferUse(true),
			m_AlphaBlendSrcOne(false),
			m_Virsion(1),
			m_TextureOnlyNoLight(false),
			m_pSamplerState(nullptr)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class BasicPNTDraw : public Component;
	//	用途: BasicPNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BasicPNTDraw::BasicPNTDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicPNTDraw::~BasicPNTDraw(){}

	//アクセサ
	const Color4& BasicPNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void BasicPNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }
	const Color4& BasicPNTDraw::GetEmissive() const { return pImpl->m_Emissive; }
	void BasicPNTDraw::SetEmissive(const Color4& c){ pImpl->m_Emissive = c; }
	const Color4& BasicPNTDraw::GetSpecularAndPower() const { return pImpl->m_SpecularAndPower; }
	void BasicPNTDraw::SetSpecularAndPower(const Color4& c){ pImpl->m_SpecularAndPower = c; }

	bool BasicPNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool BasicPNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void BasicPNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	size_t BasicPNTDraw::GetShaderVirsion() const{
		return pImpl->m_Virsion;
	}
	void BasicPNTDraw::SetShaderVirsion(size_t v){
		if (v <= 1){
			v = 1;
		}
		else if (v >= 2){
			v = 2;
		}
		pImpl->m_Virsion = v;
	}


	bool BasicPNTDraw::GetCullNone() const{
		return pImpl->m_CullNone;
	}
	bool BasicPNTDraw::IsCullNone() const{
		return pImpl->m_CullNone;
	}
	void BasicPNTDraw::SetCullNone(bool b){
		pImpl->m_CullNone = b;
	}

	bool BasicPNTDraw::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;

	}
	bool BasicPNTDraw::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void BasicPNTDraw::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool BasicPNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool BasicPNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void BasicPNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	void BasicPNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}
	ID3D11SamplerState* BasicPNTDraw::GetSamplerState() const{
		if (IsGameObjectActive()){
			if (pImpl->m_pSamplerState){
				return pImpl->m_pSamplerState;
			}
			else{
				auto PtrGameObject = GetGameObject();
				auto PtrStage = PtrGameObject->GetStage();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();
				return RenderStatePtr->GetLinearClamp();
			}
		}
		else{
			return nullptr;
		}
	}





	shared_ptr<MeshResource> BasicPNTDraw::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"BasicPNTDraw::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicPNTDraw::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"BasicPNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void BasicPNTDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}


	shared_ptr<TextureResource> BasicPNTDraw::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"BasicPNTDraw::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicPNTDraw::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"BasicPNTDraw::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void BasicPNTDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void BasicPNTDraw::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool BasicPNTDraw::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool BasicPNTDraw::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}



	void BasicPNTDraw::Draw(){

		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();
			//マルチライトを取り出す
			auto Lights = PtrStage->GetTargetMultiLight();
			//ステージから0番目のライトを取り出す(シャドウ用)
			auto PtrLight = PtrStage->GetTargetLight(0);
			//メッシュリソースの取得
			auto PtrMeshResource = GetMeshResource();
			//テクスチャの取得(nullの場合有)
			auto PtrTextureResource = GetTextureResource(false);
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();
				//シャドウマップのレンダラーターゲット
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				//カメラの取得
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = PtrT->GetWorldMatrix() * View * Proj;
				TransWorldViewProj.Transpose();

				//コンスタントバッファの設定
				BasicConstantBuffer Cb;

				Cb.diffuseColor = pImpl->m_Diffuse;
				Cb.emissiveColor = pImpl->m_Emissive;
				Cb.specularColorAndPower = pImpl->m_SpecularAndPower;
				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				Cb.m_ActiveFlg.z = lightcount;

				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, PtrT->GetWorldMatrix());
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;


				if (IsOwnShadowActive()){
					//ライトの取得
					Matrix4X4 LightView, LightProj, LightViewProj;
					Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
					Vector3 LightAt = PtrCamera->GetAt();
					Vector3 LightEye = LightDir;
					LightEye *= Shadowmap::GetLightHeight();
					LightEye = LightAt + LightEye;
					//ライトのビューと射影を計算
					LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
					LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
						Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
					LightViewProj = LightView * LightProj;
					Matrix4X4 LWMatrix = PtrT->GetWorldMatrix() * LightViewProj;
					Cb.LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
					Cb.m_ActiveFlg.y = 1;

					Cb.m_LPos = LightEye;
					Cb.m_LPos.w = 0;
					Cb.m_LView = Matrix4X4EX::Transpose(LightView);
					Cb.m_LProjection = Matrix4X4EX::Transpose(LightProj);
					Cb.m_EyePos = PtrCamera->GetEye();
					Cb.m_EyePos.w = 0;


				}
				if (PtrTextureResource){
					Cb.m_ActiveFlg.x = 1;
					if (IsTextureOnlyNoLight()){
						Cb.m_ActiveFlg.z = 0;
						Cb.m_ActiveFlg.w = 1;
					}
					else{
						Cb.m_ActiveFlg.w = 0;
					}
				}

				//コンスタントバッファの更新
				pID3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalTexture);
				UINT offset = 0;
				//頂点バッファの設定
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//インデックスバッファのセット
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//頂点シェーダの設定
				pID3D11DeviceContext->VSSetShader(VSBasicPNT::GetPtr()->GetShader(), nullptr, 0);

				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };
				if (PtrTextureResource){
					//テクスチャを設定
					pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
					//リニアサンプラーを設定
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						samplerState = pImpl->m_pSamplerState;
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
				}

				if (GetShaderVirsion() >= 2){
					//ピクセルシェーダの設定
					pID3D11DeviceContext->PSSetShader(PSBasicPNT2::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//ピクセルシェーダの設定
					pID3D11DeviceContext->PSSetShader(PSBasicPNT::GetPtr()->GetShader(), nullptr, 0);
				}

				//インプットレイアウトの設定
				pID3D11DeviceContext->IASetInputLayout(VSBasicPNT::GetPtr()->GetInputLayout());

				if (IsOwnShadowActive()){
					//シャドウマップのリソースビューを取得
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウ用にリニアサンプラーを取得
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetComparisonLinear();
					if (pImpl->m_Virsion <= 1){
						//バージョンが低い場合はテクスチャと同じサンプラーを使用
						pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					}
					//シャドウ用リニアサンプラーを設定
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
				}

				if (pImpl->m_ZBufferUse){
					//デプスステンシルは使用する
					pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
				}
				else{
					//デプスステンシルは使用しない
					pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);

				}
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (PtrGameObject->IsAlphaActive()){
					if (IsAlphaBlendSrcOne()){
						//透明処理
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
					}
					else{
						//半透明処理
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					}
					//レンダリングステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
					//レンダリングステート
					if (pImpl->m_CullNone){
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
					}
					else{
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				}
				else{
					//塗りつぶし
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
					//レンダリングステート
					if (pImpl->m_CullNone){
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
					}
					else{
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				}
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}
		}
	}




	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTDraw::Impl{
		Color4 m_Diffuse;	//デフィーズ
		Color4 m_Emissive;	//エミッシブ
		Color4 m_SpecularAndPower;	//スペキュラーとパワー
		bool m_OwnShadowActive;		//自身への影投影
		weak_ptr<FbxMeshResource> m_FbxMeshResource;	//FBXメッシュリソース
		Matrix4X4 m_MeshToTransform;
		bool m_TextureOnlyNoLight;				//テクスチャオンリーかどうか
		bool m_AlphaBlendSrcOne;	//透明処理のSRC_ONE設定
		ID3D11SamplerState* m_pSamplerState;	//サンプラーステート（オプション）
		Impl() :
			m_Diffuse(0.7f, 0.7f, 0.7f, 1.0f),
			m_Emissive(0.4f, 0.4f, 0.4f, 1.0f),
			m_SpecularAndPower(0.4f, 0.4f, 0.4f, 1.0f),
			m_OwnShadowActive(false),
			m_MeshToTransform(),
			m_TextureOnlyNoLight(false),
			m_AlphaBlendSrcOne(false),
			m_pSamplerState(nullptr)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTDraw : public Component;
	//	用途: BasicFbxPNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BasicFbxPNTDraw::BasicFbxPNTDraw(const shared_ptr<GameObject>& GameObjectPtr):
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicFbxPNTDraw::~BasicFbxPNTDraw(){}

	//アクセサ
	const Color4& BasicFbxPNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void BasicFbxPNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }
	const Color4& BasicFbxPNTDraw::GetEmissive() const { return pImpl->m_Emissive; }
	void BasicFbxPNTDraw::SetEmissive(const Color4& c){ pImpl->m_Emissive = c; }
	const Color4& BasicFbxPNTDraw::GetSpecularAndPower() const { return pImpl->m_SpecularAndPower; }
	void BasicFbxPNTDraw::SetSpecularAndPower(const Color4& c){ pImpl->m_SpecularAndPower = c; }

	bool BasicFbxPNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool BasicFbxPNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void BasicFbxPNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	bool BasicFbxPNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool BasicFbxPNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void BasicFbxPNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	void BasicFbxPNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}
	ID3D11SamplerState* BasicFbxPNTDraw::GetSamplerState() const{
		return pImpl->m_pSamplerState;
	}


	shared_ptr<FbxMeshResource> BasicFbxPNTDraw::GetFbxMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_FbxMeshResource.expired()){
			return pImpl->m_FbxMeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"Fbxメッシュリソースが見つかりません",
					L"if (pImpl->m_FbxMeshResource.expired())",
					L"BasicFbxPNTDraw::GetFbxMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicFbxPNTDraw::SetFbxMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"Fbxメッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"BasicFbxPNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_FbxMeshResource = App::GetApp()->GetResource<FbxMeshResource>(ResKey);
			//ボーンがあれば読み込む
			SetBoneVec();
		}
		catch (...){
			throw;
		}
	}
	void BasicFbxPNTDraw::SetFbxMeshResource(const shared_ptr<FbxMeshResource>& FbxMeshResourcePtr){
		pImpl->m_FbxMeshResource = FbxMeshResourcePtr;
		//ボーンがあれば読み込む
		SetBoneVec();
	}

	const Matrix4X4& BasicFbxPNTDraw::GetMeshToTransform() const{
		return pImpl->m_MeshToTransform;
	}
	void BasicFbxPNTDraw::SetMeshToTransform(const Matrix4X4& Mat){
		pImpl->m_MeshToTransform = Mat;
	}

	void BasicFbxPNTDraw::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool BasicFbxPNTDraw::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool BasicFbxPNTDraw::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}



	//操作
	void BasicFbxPNTDraw::Draw(){

		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();
			//マルチライトを取り出す
			auto Lights = PtrStage->GetTargetMultiLight();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//メッシュリソースの取得
			auto PtrMeshResource = GetFbxMeshResource();
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();
				//シャドウマップのレンダラーターゲット
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				//カメラの取得
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = RealWorldMatrix * View * Proj;
				TransWorldViewProj.Transpose();

				//コンスタントバッファの設定
				BasicConstantBuffer Cb;

				Cb.diffuseColor = pImpl->m_Diffuse;
				Cb.emissiveColor = pImpl->m_Emissive;
				Cb.specularColorAndPower = pImpl->m_SpecularAndPower;


				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				Cb.m_ActiveFlg.z = lightcount;

				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(RealWorldMatrix);

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, RealWorldMatrix);
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;


				if (IsOwnShadowActive()){
					//ライトの取得
					Matrix4X4 LightView, LightProj, LightViewProj;
					Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
					Vector3 LightAt = PtrCamera->GetAt();
					Vector3 LightEye = LightDir;
					LightEye *= Shadowmap::GetLightHeight();
					LightEye = LightAt + LightEye;
					//ライトのビューと射影を計算
					LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
					LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
						Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
					LightViewProj = LightView * LightProj;
					Matrix4X4 LWMatrix = RealWorldMatrix * LightViewProj;
					Cb.LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
					Cb.m_ActiveFlg.y = 1;
				}
				Cb.m_ActiveFlg.x = 1;
				if (IsTextureOnlyNoLight()){
					Cb.m_ActiveFlg.z = 0;
					Cb.m_ActiveFlg.w = 1;
				}
				else{
					Cb.m_ActiveFlg.w = 0;
				}


				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalTexture);
				//ボーンが登録されていたらボーンを探す
				if (PtrMeshResource->IsSkining()){
					stride = sizeof(VertexPositionNormalTextureSkinning);
				}


				UINT offset = 0;
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };


				if (IsOwnShadowActive()){
					//シャドウマップのリソースビューを取得
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウ用にリニアサンプラーを取得(テクスチャと同じもの)
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						pShadowSamplerState = pImpl->m_pSamplerState;
					}
					//シャドウ用リニアサンプラーを設定
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
				}

				////////////////////////////


				for (auto Mat : PtrMeshResource->GetMaterialVec()){

					Cb.diffuseColor = Mat.m_Diffuse;
					Cb.emissiveColor = Mat.m_Emissive;
					Cb.specularColorAndPower = Mat.m_Specular;

					//頂点バッファの設定

					auto VBuffer = PtrMeshResource->GetVertexBuffer();
					auto IndBff = PtrMeshResource->GetIndexBuffer();

					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//インデックスバッファのセット
					pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


					//テクスチャを設定
					pID3D11DeviceContext->PSSetShaderResources(0, 1, Mat.m_Texture->GetShaderResourceView().GetAddressOf());
					//リニアサンプラーを設定
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						samplerState = pImpl->m_pSamplerState;
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
					//頂点シェーダの設定
					pID3D11DeviceContext->VSSetShader(VSBasicPNT::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pID3D11DeviceContext->IASetInputLayout(VSBasicPNT::GetPtr()->GetInputLayout());
					//ピクセルシェーダの設定
					pID3D11DeviceContext->PSSetShader(PSBasicPNT::GetPtr()->GetShader(), nullptr, 0);

					//コンスタントバッファの更新
					pID3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


					if (PtrGameObject->IsAlphaActive()){
						if (IsAlphaBlendSrcOne()){
							//透明処理
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
						}
						else{
							//半透明処理
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						}
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}
					else{
						//塗りつぶし
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}

				}


				////////////////////////////
				//後始末
				Dev->InitializeStates(RenderStatePtr);
			}
		}

	}


	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTBoneDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTBoneDraw::Impl{
		string m_sCurrentAnimationName;	//現在処理対象になっているアニメーションの名前
		float m_CurrentTime;			//アニメーションの現在の経過時間（秒）
		bool m_IsAnimeEnd;				//現在のアニメーションが終了したかどうか
		vector< Bone > m_vecLocalBones;	//各オブジェクトごとにボーンを所持しておく
		Impl():
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	用途: BasicFbxPNTBoneDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BasicFbxPNTBoneDraw::BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BasicFbxPNTDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicFbxPNTBoneDraw::~BasicFbxPNTBoneDraw(){}

	//ボーン行列をリソースから読み込む
	void BasicFbxPNTBoneDraw::SetBoneVec(){
		auto PtrMeshResource = GetFbxMeshResource();
		if (PtrMeshResource->GetNumBones() > 0){
			//ローカルボーンのオリジナルからのコピー
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();
		}
		else{
			throw BaseException(
				L"ボーンが見つかりません",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::SetBoneVec()"
				);
		}
	}


	//アクセサ
	//各オブジェクトごとのボーン
	const vector< Bone >& BasicFbxPNTBoneDraw::GetVecLocalBones() const{
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BasicFbxPNTBoneDraw::GetVecLocalBonesPtr() const{
		return &pImpl->m_vecLocalBones;
	}


	const string& BasicFbxPNTBoneDraw::GetCurrentAnimation() const{
		return pImpl->m_sCurrentAnimationName;
	}
	void BasicFbxPNTBoneDraw::SetCurrentAnimation(const string& AnemationName, float StartTime){
		auto PtrMeshResource = GetFbxMeshResource();
		//指定のアニメーションがあるかどうかチェック
		//無ければ例外が出る
		AnimationData animData = PtrMeshResource->GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
	}

	//現在のアニメーションが終了しているかどうか
	bool BasicFbxPNTBoneDraw::IsTargetAnimeEnd() const{
		return pImpl->m_IsAnimeEnd;
	}

	//指定したIDのボーンの現在の行列を取得する
	void BasicFbxPNTBoneDraw::GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const{
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			Matrix4X4 MeshMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();
			if (pImpl->m_vecLocalBones.size() <= BoneId){
				throw BaseException(
					L"ボーンIDが範囲外です",
					L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
					L"BasicFbxPNTBoneDraw::GetBoneMatrix()"
					);
			}
			Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
		}
	}

	//指定したIDのボーンの現在のローカル行列を取得する（親子関係を構築するなど用）
	void BasicFbxPNTBoneDraw::GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const{
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			Matrix4X4 MeshMatrix = GetMeshToTransform();// *PtrT->GetWorldMatrix();
			if (pImpl->m_vecLocalBones.size() <= BoneId){
				throw BaseException(
					L"ボーンIDが範囲外です",
					L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
					L"BasicFbxPNTBoneDraw::GetLocalBoneMatrix()"
					);
			}
			Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
		}
	}



	void BasicFbxPNTBoneDraw::Update(){
		//アニメーションを更新する
		//メッシュリソースの取得
		auto PtrMeshResource = GetFbxMeshResource();
		//ボーンが登録されていたらボーンを探す
		if (PtrMeshResource->IsSkining()){
			pImpl->m_CurrentTime += App::GetApp()->GetElapsedTime();
			//アニメーションデータの取得
			//チェックするだけなのでテンポラリでよい
			AnimationData animData = PtrMeshResource->GetAnimationData(pImpl->m_sCurrentAnimationName);
			//ボーンを現在の時間に更新する
			//戻り値は終了してるかどうか
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
		}
	}


	void  BasicFbxPNTBoneDraw::Draw(){

		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();
			//マルチライトを取り出す
			auto Lights = PtrStage->GetTargetMultiLight();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//メッシュリソースの取得
			auto PtrMeshResource = GetFbxMeshResource();
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();
				//シャドウマップのレンダラーターゲット
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };

				//コンスタントバッファの設定
				BasicBoneShadowConstantBuffer CbShadow;

				if (IsOwnShadowActive()){
					//シャドウマップのリソースビューを取得
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウ用にリニアサンプラーを取得(テクスチャと同じもの)
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					if (GetSamplerState()){
						pShadowSamplerState = GetSamplerState();
					}
					//シャドウ用リニアサンプラーを設定
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
					CbShadow.m_ActiveFlg.y = 1;
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
				}


				//カメラの取得
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = RealWorldMatrix * View * Proj;
				TransWorldViewProj.Transpose();

				//コンスタントバッファの設定
				BasicBoneConstantBuffer Cb;

				Cb.diffuseColor = GetDiffuse();
				Cb.emissiveColor = GetEmissive();
				Cb.specularColorAndPower = GetSpecularAndPower();

				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				CbShadow.m_ActiveFlg.z = lightcount;

				if (IsTextureOnlyNoLight()){
					CbShadow.m_ActiveFlg.z = 0;
					CbShadow.m_ActiveFlg.w = 1;
				}
				else{
					CbShadow.m_ActiveFlg.w = 0;
				}


				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(RealWorldMatrix);

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, RealWorldMatrix);
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;

				//ボーンの設定
				size_t BoneSz = pImpl->m_vecLocalBones.size();
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++){
					Matrix4X4 mat = pImpl->m_vecLocalBones[b].m_ConbinedPose;
					mat.Transpose();
					Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}


				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalTextureSkinning);
				UINT offset = 0;
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//ID3D11ShaderResourceView* pNull[1] = { 0 };
				//ID3D11SamplerState* pNullSR[1] = { 0 };

				////////////////////////////

				for (auto Mat : PtrMeshResource->GetMaterialVec()){


					Cb.diffuseColor = Mat.m_Diffuse;
					Cb.emissiveColor = Mat.m_Emissive;
					Cb.specularColorAndPower = Mat.m_Specular;

					//頂点バッファの設定

					auto VBuffer = PtrMeshResource->GetVertexBuffer();
					auto IndBff = PtrMeshResource->GetIndexBuffer();

					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//インデックスバッファのセット
					pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


					//テクスチャを設定
					pID3D11DeviceContext->PSSetShaderResources(0, 1, Mat.m_Texture->GetShaderResourceView().GetAddressOf());
					//リニアサンプラーを設定
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (GetSamplerState()){
						samplerState = GetSamplerState();
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
					//頂点シェーダの設定
					pID3D11DeviceContext->VSSetShader(VSBasicPNTBone::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pID3D11DeviceContext->IASetInputLayout(VSBasicPNTBone::GetPtr()->GetInputLayout());
					//ピクセルシェーダの設定
					pID3D11DeviceContext->PSSetShader(PSBasicPNTBone::GetPtr()->GetShader(), nullptr, 0);

					//コンスタントバッファの更新
					pID3D11DeviceContext->UpdateSubresource(CBBasicBone::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					pID3D11DeviceContext->UpdateSubresource(CBBasicBoneShadow::GetPtr()->GetBuffer(), 0, nullptr, &CbShadow, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBBasicBone::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

					pConstantBuffer = CBBasicBoneShadow::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pConstantBuffer);



					if (PtrGameObject->IsAlphaActive()){
						if (IsAlphaBlendSrcOne()){
							//透明処理
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
						}
						else{
							//半透明処理
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						}
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}
					else{
						//塗りつぶし
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						//レンダリングステート
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//描画
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}

				}


				////////////////////////////
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}
		}
	}




}

//end basedx11
