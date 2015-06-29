#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Sprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Sprite::Impl{
		vector<VertexPositionColorTexture> m_BackupVertex;//バックアップの頂点
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
		Vector2 m_MakedSize;			//作成時のサイズ
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;

		weak_ptr<TextureResource> m_TextureResource;	//テクスチャリソース
		bool m_SamplerWrap;

		bool m_ZBufferUse;

		float m_PixelParMeter;	//1メートルあたりのピクセル
		Sprite::Coordinate m_Coordinate;	//座標系
		//ミューテックス
		std::mutex Mutex;

		Impl(const Vector2& MakedSize, const Color4& Color) :
			m_NumVertices(0),
			m_NumIndicis(0),
			m_MakedSize(MakedSize),
			m_LeftTop(Color),	//初期は同じ色
			m_RightTop(Color),
			m_LeftBottom(Color),
			m_RightBottom(Color),
			m_SamplerWrap(false),
			m_ZBufferUse(false),
			m_PixelParMeter(32.0f),
			m_Coordinate(Sprite::Coordinate::m_LeftTopZeroPlusDownY)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Sprite : public DrawComponent;
	//	用途: Spriteコンポーネント
	//　＊Spriteは頂点変更が基本なので、各コンポーネントでバッファを作成する
	//--------------------------------------------------------------------------------------
	Sprite::Sprite(const shared_ptr<GameObject>& GameObjectPtr,
		const Color4& Color,const Vector2& Size) :
		Component(GameObjectPtr),
		pImpl(new Impl(Size, Color))

	{
		try{
			//インデックスを作成するための配列
			vector<uint16_t> indices;

			Vector2 HelfSize = Size / 2.0f;
			XMFLOAT4 col(Color.x, Color.y, Color.z, Color.w);

			pImpl->m_BackupVertex.push_back(VertexPositionColorTexture(XMFLOAT3(-HelfSize.x, HelfSize.y, 0), col, XMFLOAT2(0.0f, 0.0f)));
			pImpl->m_BackupVertex.push_back(VertexPositionColorTexture(XMFLOAT3(HelfSize.x, HelfSize.y, 0), col, XMFLOAT2(1.0f, 0.0f)));
			pImpl->m_BackupVertex.push_back(VertexPositionColorTexture(XMFLOAT3(-HelfSize.x, -HelfSize.y, 0), col, XMFLOAT2(0.0f, 1.0f)));
			pImpl->m_BackupVertex.push_back(VertexPositionColorTexture(XMFLOAT3(HelfSize.x, -HelfSize.y, 0), col, XMFLOAT2(1.0f, 1.0f)));

			indices.push_back((uint16_t)0);
			indices.push_back((uint16_t)1);
			indices.push_back((uint16_t)2);
			indices.push_back((uint16_t)1);
			indices.push_back((uint16_t)3);
			indices.push_back((uint16_t)2);

			//配列をもとに頂点とインデックスを作成
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//バッファの作成
			Util::DemandCreate(pImpl->m_VertexBuffer, pImpl->Mutex, [&](ID3D11Buffer** pResult)
			{
				//頂点バッファの作成
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, pImpl->m_BackupVertex, pResult);
			});
			pImpl->m_NumVertices = static_cast<UINT>(pImpl->m_BackupVertex.size());

			Util::DemandCreate(pImpl->m_IndexBuffer, pImpl->Mutex, [&](ID3D11Buffer** pResult)
			{
				//インデックスバッファの作成
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//インデックス数の設定
			pImpl->m_NumIndicis = static_cast<UINT>(indices.size());
		}
		catch (...){
			throw;
		}
	}
	Sprite::~Sprite(){}

	//アクセサ
	Sprite::Coordinate Sprite::GetCoordinate()const{
		return pImpl->m_Coordinate;
	}
	void Sprite::SetCoordinate(Sprite::Coordinate c){
		pImpl->m_Coordinate = c;
	}


	ComPtr<ID3D11Buffer>& Sprite::GetVertexBuffer() const{
		return pImpl->m_VertexBuffer;
	}
	vector<VertexPositionColorTexture>& Sprite::GetBackupVertex() const{
		return pImpl->m_BackupVertex;
	}
	ComPtr<ID3D11Buffer>& Sprite::GetIndexBuffer() const{
		return pImpl->m_IndexBuffer;
	}
	UINT Sprite::GetNumVertices() const{
		return pImpl->m_NumVertices;
	}
	UINT Sprite::GetNumIndicis() const{
		return pImpl->m_NumIndicis;
	}



	shared_ptr<TextureResource> Sprite::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"Sprite::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void Sprite::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"Sprite::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Sprite::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}
	bool Sprite::IsSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	bool Sprite::GetSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	void Sprite::SetSamplerWrap(bool b){
		pImpl->m_SamplerWrap = b;
	}

	//Zバッファを使うかどうか
	bool Sprite::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	bool Sprite::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void Sprite::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}


	//1メータあたりのピクセル数
	float Sprite::GetPixelParMeter() const{
		return pImpl->m_PixelParMeter;
	}
	void Sprite::SetPixelParMeter(float f){
		pImpl->m_PixelParMeter = f;
	}

	void Sprite::CalcSpriteCoordinate(Matrix4X4& Retmat){

		auto PtrGameObject = GetGameObject();
		auto GameWidth = PtrGameObject->GetStage()->GetTargetViewPort().Width / pImpl->m_PixelParMeter;
		auto GameHeight = PtrGameObject->GetStage()->GetTargetViewPort().Height / pImpl->m_PixelParMeter;

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//Transformの行列jからSprite行列に変換する
		auto SpriteMatrix = PtrT->GetWorldMatrix();
		//スケーリングの変更
		//スクリーン幅が2.0に換算する
		auto SpriteScale1 = PtrT->GetWorldMatrix().ScaleInMatrix();
		SpriteScale1.x = 2.0f * SpriteScale1.x;
		SpriteScale1.y = 2.0f * SpriteScale1.y;
		SpriteScale1.z = 1.0f;
		Vector3 SpriteScale2(1.0f / GameWidth, 1.0f / GameHeight,1.0f);

		//ポジションを取得
		auto SpritePos = PtrT->GetWorldMatrix().PosInMatrix();
		//単位の変更
		SpritePos.x = 2.0f * SpritePos.x / GameWidth;
		//単位の変更
		SpritePos.y = 2.0f * SpritePos.y / GameHeight;

		//座標軸の変更
		switch (pImpl->m_Coordinate){
		case Coordinate::m_LeftBottomZeroPlusUpY:
			SpritePos.x -= 1.0f;
			SpritePos.y -= 1.0f;
			break;
		case Coordinate::m_LeftTopZeroPlusDownY:
			SpritePos.x -= 1.0f;
			SpritePos.y -= 1.0f;
			//下向きをプラスにする
			SpritePos.y *= -1.0f;
			break;
		case Coordinate::m_CenterZeroPlusUpY:
			break;
		}
		//Zは0から1.0の間にする
		if (SpritePos.z <= 0.0f){
			SpritePos.z = 0.0f;
		}
		if (SpritePos.z >= 1.0f){
			SpritePos.z = 1.0f;
		}
		Matrix4X4 RotMatrix;
		//回転を先行させる
		//ゆがまないようにする
		RotMatrix.DefTransformation(
			SpriteScale1,
			PtrT->GetWorldMatrix().QtInMatrix(),
			Vector3(0, 0, 0)
			);
		Matrix4X4 OtherMatrix;
		OtherMatrix.DefTransformation(
			SpriteScale2,
			Vector3(0, 0, 0),
			SpritePos
			);
		Retmat = RotMatrix * OtherMatrix;



	}



	void Sprite::Draw(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			return;
		}
		Matrix4X4 ShaderMatrix;
		CalcSpriteCoordinate(ShaderMatrix);
		//テクスチャは必須
		auto PtrTextureResource = GetTextureResource(false);
		if (PtrTextureResource){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//ステータスのポインタ
			auto RenderStatePtr = PtrStage->GetRenderState();

			//コンスタントバッファの設定
			SpriteConstantBuffer sb;
			sb.mTransformMatrix = ShaderMatrix;

			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionColorTexture);
			UINT offset = 0;
			//頂点バッファの設定
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(pImpl->m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			ID3D11SamplerState* samplerState;
			if (IsSamplerWrap()){
				samplerState = RenderStatePtr->GetLinearWrap();
			}
			else{
				samplerState = RenderStatePtr->GetLinearClamp();
			}
			//サンプラーを設定
			pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
			//テクスチャありのピクセルシェーダの設定
			pID3D11DeviceContext->PSSetShader(PSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//頂点シェーダの設定
			pID3D11DeviceContext->VSSetShader(VSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pID3D11DeviceContext->IASetInputLayout(VSSpritePCT::GetPtr()->GetInputLayout());
			if (pImpl->m_ZBufferUse){
				//デプスステンシル使用
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//デプスステンシルは使用しない
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			}
			//ステータスの設定
			//アルファブレンド
			if (GetGameObject()->GetAlphaActive()){
				if (GetGameObject()->GetAlphaExActive()){
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				else{
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
			}
			else{
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xFFFFFFFF);
			}
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer =  nullptr;

			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			if (PtrGameObject->IsAlphaActive()){
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//描画
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
			}
			else{
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
			}

			//後始末
			Dev->InitializeStates(RenderStatePtr);

		}
	}

	void Sprite::UpdateVirtexBuffer(const vector<VertexPositionColorTexture>& NewBuffer){
		if (NewBuffer.size() < 4){
			// Map失敗
			throw BaseException(
				L"変更する頂点の数が足りません",
				L"if (NewBuffer.size() < 4)",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = pImpl->m_VertexBuffer.Get();

		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//UV値の変更
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		vertices[0] = NewBuffer[0];
		vertices[1] = NewBuffer[1];
		vertices[2] = NewBuffer[2];
		vertices[3] = NewBuffer[3];
		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// テキスト レンダリングに関連するリソース。
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		Color4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAligment m_TextAligment;



		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;
		Impl():
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(640.0f),
			m_TextBlockHeight(480.0f),
			m_FontName(L"ＭＳゴシック"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{16.0f,16.0f},
			m_TextAligment(StringSprite::TextAligment::m_Left)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()){
		try{
			ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// デバイスに依存するリソースを作成します。
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
				pImpl->m_FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				pImpl->m_FoneSize,
				L"ja",
				&pImpl->m_textFormat
				),
				L"フォント作成に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"フォントアラインメント設定に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"フォントステートブロック設定に失敗しました。",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
				);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_whiteBrush
				),
				L"フォントブラシ設定に失敗しました。",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);
		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}

	//アクセサ


	//アクセサ
	void StringSprite::CreateFont(const wstring& FontName, float FontSize){
	}
	StringSprite::TextAligment StringSprite::GetTextAligment() const{
		return pImpl->m_TextAligment;
	}
	void StringSprite::SetTextAligment(StringSprite::TextAligment Aligment){
		pImpl->m_TextAligment = Aligment;
	}
	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}
	const Color4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFoneColor(const Color4& Col){
		pImpl->m_FontColor = Col;
	}
	const wstring& StringSprite::GetFontName() const{
		return pImpl->m_FontName;
	}
	float StringSprite::GetFontSize() const{
		return pImpl->m_FoneSize;
	}

	float StringSprite::GetTextBlockWidth() const{
		return pImpl->m_TextBlockWidth;
	}
	void StringSprite::SetTextBlockWidth(float f){
		pImpl->m_TextBlockWidth = f;
	}
	float StringSprite::GetTextBlockHeight() const{
		return pImpl->m_TextBlockHeight;
	}
	void StringSprite::SetTextBlockHeight(float f){
		pImpl->m_TextBlockHeight = f;
	}


	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}




	void StringSprite::Update(){
		//if (pImpl->m_text.size() <= 0){
		//	return;
		//}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
			pImpl->m_text.c_str(),
			(uint32)pImpl->m_text.length(),
			pImpl->m_textFormat.Get(),
			pImpl->m_TextBlockWidth, // 入力テキストの最大幅。
			pImpl->m_TextBlockHeight, // 入力テキストの最大高さ。
			&pImpl->m_textLayout
			),
			L"フォントレイアウト設定に失敗しました。",
			L"DWriteFactory->CreateTextLayout()",
			L"StringSprite::Update()"
			);
		ThrowIfFailed(
			pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
			L"テキストメトリクス取得に失敗しました。",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringSprite::Update()"
			);

	}



	void StringSprite::Draw(){
		if (pImpl->m_text.size() <= 0){
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		// 右下隅に配置
		//D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		//	640.0f - pImpl->m_textMetrics.layoutWidth,
		//	480.0f - pImpl->m_textMetrics.height
		//	);

		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			pImpl->m_StartPosition.x,
			pImpl->m_StartPosition.y
			);


		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (pImpl->m_TextAligment){
		case TextAligment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAligment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAligment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			pImpl->m_textFormat->SetTextAlignment(Alignment),
			L"テキストアラインメント設定に失敗しました。",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringSprite::Draw()"
			);



		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			pImpl->m_textLayout.Get(),
			pImpl->m_whiteBrush.Get()
			);

		// D2DERR_RECREATE_TARGET をここで無視します。このエラーは、デバイスが失われたことを示します。
		// これは、Present に対する次回の呼び出し中に処理されます。
		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			//DX::ThrowIfFailed(hr);
		}

		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());

	}


	//--------------------------------------------------------------------------------------
	//	struct InputStringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct InputStringSprite::Impl{
		bool m_KeyInput;	//キー入力するかどうか
		size_t m_Caret;	//キャレット
		Impl() :
			m_KeyInput(true),
			m_Caret(0)
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class InputStringSprite : publicStringSprite;
	//	用途: InputStringSpriteコンポーネント
	//	入力付文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	InputStringSprite::InputStringSprite(const shared_ptr<GameObject>& GameObjectPtr):
		StringSprite(GameObjectPtr),
		pImpl(new Impl())
	{}
	InputStringSprite::~InputStringSprite(){}

	bool InputStringSprite::GetKeyInput() const{
		return pImpl->m_KeyInput;
	}
	bool InputStringSprite::IsKeyInput() const{
		return pImpl->m_KeyInput;

	}
	void InputStringSprite::SetKeyInput(bool b){
		pImpl->m_KeyInput = b;
	}

	bool InputStringSprite::IsFocus(){
		auto Manager = GetGameObject()->GetStage()->GetInputTextManager();
		if (Manager){
			auto Ptr = GetThis<InputStringSprite>();
			if (Manager->GetFocusInputString() == Ptr){
				return true;
			}
		}
		return false;
	}
	void InputStringSprite::SetFocus(bool b){
		shared_ptr<InputTextManager> Manager;// = GetGameObject()->GetStage()->GetInputTextManager();
		auto ParStage = dynamic_pointer_cast<Stage>(GetGameObject());
		if (ParStage){
			Manager = ParStage->GetInputTextManager();
		}
		else{
			Manager = GetGameObject()->GetStage()->GetInputTextManager();
		}
		if (Manager){
			if (b){
				Manager->SetFocusInputString(GetThis<InputStringSprite>());
			}
			else{
				Manager->SetFocusInputString(nullptr);
			}
		}
	}

	size_t InputStringSprite::GetCaret() const{
		if (pImpl->m_Caret >= GetText().size()){
			pImpl->m_Caret = GetText().size();
		}
		return pImpl->m_Caret;
	}
	void InputStringSprite::SetCaret(size_t CaretIndex){
		if (CaretIndex >= GetText().size()){
			CaretIndex = GetText().size();
		}
		pImpl->m_Caret = CaretIndex;
	}

	//最後尾に追加
	void InputStringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		pImpl->m_Caret = TempText.size();
		SetText(TempText);
	}
	void InputStringSprite::InsertText(const wstring& str){
		wstring TempText = GetText();
		if (pImpl->m_Caret >= TempText.size()){
			pImpl->m_Caret = TempText.size();
			AddText(str);
		}
		else{
			TempText.insert(pImpl->m_Caret, str);
			pImpl->m_Caret += str.size();
			SetText(TempText);
		}
	}
	void InputStringSprite::InsertText(const wstring& str, size_t CaretIndex){
		pImpl->m_Caret = CaretIndex;
		InsertText(str);
	}

	void InputStringSprite::OnKeyDown(WPARAM wParam, LPARAM lParam){
		wstring TempText = GetText();
		switch (wParam){
			case VK_RIGHT:
				if (pImpl->m_Caret < TempText.size()){
					pImpl->m_Caret++;
				}
			break;
			case VK_LEFT:
				if (pImpl->m_Caret > 0){
					pImpl->m_Caret--;
				}
			break;
			default:
			break;
		}
	}

	void InputStringSprite::OnChar(WPARAM wParam, LPARAM lParam){
		wstring str(L"");
		str += (wchar_t)wParam;
		InsertText(str);
	}


}

//end basedx11
