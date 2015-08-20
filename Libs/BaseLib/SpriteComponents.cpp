#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Sprite::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Sprite::Impl{
		vector<VertexPositionColorTexture> m_BackupVertex;//�o�b�N�A�b�v�̒��_
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
		Vector2 m_MakedSize;			//�쐬���̃T�C�Y
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;

		weak_ptr<TextureResource> m_TextureResource;	//�e�N�X�`�����\�[�X
		bool m_SamplerWrap;

		bool m_ZBufferUse;

		bool m_AlphaBlendSrcOne;	//����������SRC_ONE�ݒ�

		float m_PixelParMeter;	//1���[�g��������̃s�N�Z��
		Sprite::Coordinate m_Coordinate;	//���W�n
		//�~���[�e�b�N�X
		std::mutex Mutex;

		Impl(const Vector2& MakedSize, const Color4& Color) :
			m_NumVertices(0),
			m_NumIndicis(0),
			m_MakedSize(MakedSize),
			m_LeftTop(Color),	//�����͓����F
			m_RightTop(Color),
			m_LeftBottom(Color),
			m_RightBottom(Color),
			m_SamplerWrap(false),
			m_ZBufferUse(false),
			m_AlphaBlendSrcOne(false),
			m_PixelParMeter(32.0f),
			m_Coordinate(Sprite::Coordinate::m_LeftTopZeroPlusDownY)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Sprite : public DrawComponent;
	//	�p�r: Sprite�R���|�[�l���g
	//�@��Sprite�͒��_�ύX����{�Ȃ̂ŁA�e�R���|�[�l���g�Ńo�b�t�@���쐬����
	//--------------------------------------------------------------------------------------
	Sprite::Sprite(const shared_ptr<GameObject>& GameObjectPtr,
		const Color4& Color,const Vector2& Size) :
		Component(GameObjectPtr),
		pImpl(new Impl(Size, Color))

	{
		try{
			//�C���f�b�N�X���쐬���邽�߂̔z��
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

			//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�o�b�t�@�̍쐬
			Util::DemandCreate(pImpl->m_VertexBuffer, pImpl->Mutex, [&](ID3D11Buffer** pResult)
			{
				//���_�o�b�t�@�̍쐬
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, pImpl->m_BackupVertex, pResult);
			});
			pImpl->m_NumVertices = static_cast<UINT>(pImpl->m_BackupVertex.size());

			Util::DemandCreate(pImpl->m_IndexBuffer, pImpl->Mutex, [&](ID3D11Buffer** pResult)
			{
				//�C���f�b�N�X�o�b�t�@�̍쐬
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//�C���f�b�N�X���̐ݒ�
			pImpl->m_NumIndicis = static_cast<UINT>(indices.size());
		}
		catch (...){
			throw;
		}
	}
	Sprite::~Sprite(){}

	//�A�N�Z�T
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
					L"�e�N�X�`�����\�[�X��������܂���",
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
					L"�e�N�X�`���L�[���󔒂ł�",
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

	//Z�o�b�t�@���g�����ǂ���
	bool Sprite::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	bool Sprite::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void Sprite::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool Sprite::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool Sprite::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void Sprite::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	//1���[�^������̃s�N�Z����
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
		//Transform�̍s��j����Sprite�s��ɕϊ�����
		auto SpriteMatrix = PtrT->GetWorldMatrix();
		//�X�P�[�����O�̕ύX
		//�X�N���[������2.0�Ɋ��Z����
		auto SpriteScale1 = PtrT->GetWorldMatrix().ScaleInMatrix();
		SpriteScale1.x = 2.0f * SpriteScale1.x;
		SpriteScale1.y = 2.0f * SpriteScale1.y;
		SpriteScale1.z = 1.0f;
		Vector3 SpriteScale2(1.0f / GameWidth, 1.0f / GameHeight,1.0f);

		//�|�W�V�������擾
		auto SpritePos = PtrT->GetWorldMatrix().PosInMatrix();
		//�P�ʂ̕ύX
		SpritePos.x = 2.0f * SpritePos.x / GameWidth;
		//�P�ʂ̕ύX
		SpritePos.y = 2.0f * SpritePos.y / GameHeight;

		//���W���̕ύX
		switch (pImpl->m_Coordinate){
		case Coordinate::m_LeftBottomZeroPlusUpY:
			SpritePos.x -= 1.0f;
			SpritePos.y -= 1.0f;
			break;
		case Coordinate::m_LeftTopZeroPlusDownY:
			SpritePos.x -= 1.0f;
			SpritePos.y -= 1.0f;
			//���������v���X�ɂ���
			SpritePos.y *= -1.0f;
			break;
		case Coordinate::m_CenterZeroPlusUpY:
			break;
		}
		//Z��0����1.0�̊Ԃɂ���
		if (SpritePos.z <= 0.0f){
			SpritePos.z = 0.0f;
		}
		if (SpritePos.z >= 1.0f){
			SpritePos.z = 1.0f;
		}
		Matrix4X4 RotMatrix;
		//��]���s������
		//�䂪�܂Ȃ��悤�ɂ���
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
		//�e�N�X�`���͕K�{
		auto PtrTextureResource = GetTextureResource(false);
		if (PtrTextureResource){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//�X�e�[�^�X�̃|�C���^
			auto RenderStatePtr = PtrStage->GetRenderState();

			//�R���X�^���g�o�b�t�@�̐ݒ�
			SpriteConstantBuffer sb;
			sb.mTransformMatrix = ShaderMatrix;

			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionColorTexture);
			UINT offset = 0;
			//���_�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(pImpl->m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
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
			//�T���v���[��ݒ�
			pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
			//�e�N�X�`����ݒ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
			//�e�N�X�`������̃s�N�Z���V�F�[�_�̐ݒ�
			pID3D11DeviceContext->PSSetShader(PSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//���_�V�F�[�_�̐ݒ�
			pID3D11DeviceContext->VSSetShader(VSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSSpritePCT::GetPtr()->GetInputLayout());
			if (pImpl->m_ZBufferUse){
				//�f�v�X�X�e���V���g�p
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//�f�v�X�X�e���V���͎g�p���Ȃ�
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			}
			//�X�e�[�^�X�̐ݒ�
			//�A���t�@�u�����h
			//�A���t�@EX�ɂ��U�蕪���͖���
			if (GetGameObject()->GetAlphaActive()){
				if (IsAlphaBlendSrcOne()){
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
				else{
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
			}
			else{
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xFFFFFFFF);
			}
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer =  nullptr;

			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			if (PtrGameObject->IsAlphaActive()){
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//�`��
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
			}
			else{
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(pImpl->m_NumIndicis, 0, 0);
			}

			//��n��
			Dev->InitializeStates(RenderStatePtr);

		}
	}

	void Sprite::UpdateVirtexBuffer(const vector<VertexPositionColorTexture>& NewBuffer){
		if (NewBuffer.size() < 4){
			// Map���s
			throw BaseException(
				L"�ύX���钸�_�̐�������܂���",
				L"if (NewBuffer.size() < 4)",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = pImpl->m_VertexBuffer.Get();

		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//UV�l�̕ύX
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		vertices[0] = NewBuffer[0];
		vertices[1] = NewBuffer[1];
		vertices[2] = NewBuffer[2];
		vertices[3] = NewBuffer[3];
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// �e�L�X�g �����_�����O�Ɋ֘A���郊�\�[�X�B
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		Color4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAlignment m_TextAlignment;
		bool m_KeyInput;	//�L�[���͂��邩�ǂ���
		size_t m_Caret;	//�L�����b�g�ʒu
		float m_CaretCounter;	//�L�����b�g�̃J�E���^
		float m_CaretSpeed;		//�L�����b�g�̓_�ő��x(0�œ_�ł��Ȃ�)
		bool m_CaretState;		//�L�����b�g��`�悷�邩�ǂ����i�_�ł̕\����ԁj
		Color4 m_CaretColor;	//�L�����b�g�̐F
		Color4 m_BackColor;		//�w�i�F
		Point2D<float> m_BackTextMargin;	//�w�i�F��h��Ԃ��e�L�X�g�̃}�[�W��(���E�Ə㉺)



		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_CaretBrush;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;


		Impl():
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(128.0f),
			m_TextBlockHeight(32.0f),
			m_FontName(L"�l�r�S�V�b�N"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{16.0f,16.0f},
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_KeyInput(false),
			m_Caret(0),
			m_CaretCounter(0),
			m_CaretSpeed(0.5f),
			m_CaretState(true),
			m_CaretColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_BackColor(Color4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f,0.0f)
			{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	�p�r: StringSprite�R���|�[�l���g
	//	������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()){
		try{
			ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
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
				L"�t�H���g�쐬�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
				);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_Brush
				),
				L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_CaretBrush
				),
				L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_BackBrush
				),
				L"�o�b�N�O���E���h�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);


		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}
	//�A�N�Z�T
	void StringSprite::SetFont(const wstring& FontName, float FontSize){
		ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize,
			L"ja",
			&pImpl->m_textFormat
			),
			L"�t�H���g�쐬�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);
		//�t�H���g�̍쐬�ɐ��������̂Œl��ݒ�
		pImpl->m_FontName = FontName;
		pImpl->m_FoneSize = FontSize;

		ThrowIfFailed(
			pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
			L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"StringSprite::SetFont()"
			);


	}
	StringSprite::TextAlignment StringSprite::GetTextAlignment() const{
		return pImpl->m_TextAlignment;
	}
	void StringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment){
		pImpl->m_TextAlignment = Alignment;
	}


	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}


	//�Ō���ɒǉ�
	void StringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		if (IsKeyInput()){
			pImpl->m_Caret = TempText.size();
		}
		SetText(TempText);
	}
	void StringSprite::InsertText(const wstring& str){
		if (!IsKeyInput()){
			return;
		}
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
	void StringSprite::InsertText(const wstring& str, size_t CaretIndex){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_Caret = CaretIndex;
		InsertText(str);
	}


	const Color4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFontColor(const Color4& Col){
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_Brush
			),
			L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetFontColor()"
			);

	}

	const Color4& StringSprite::GetBackColor() const{
		return pImpl->m_BackColor;
	}
	void StringSprite::SetBackColor(const Color4& Col){
		pImpl->m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_BackBrush
			),
			L"�o�b�N�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	Point2D<float> StringSprite::GetBackTextMargin() const{
		return pImpl->m_BackTextMargin;
	}
	void StringSprite::SetBackTextMargin(Point2D<float> p){
		pImpl->m_BackTextMargin = p;
	}

	const Color4& StringSprite::GetCaretColor() const{
		return pImpl->m_CaretColor;
	}
	void StringSprite::SetCaretColor(const Color4& Col){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_CaretBrush
			),
			L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	float StringSprite::GetCaretSpeed() const{
		if (!IsKeyInput()){
			return 0.0f;
		}
		return pImpl->m_CaretSpeed;
	}
	void StringSprite::SetCaretSpeed(float f){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretSpeed = f;
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

	bool StringSprite::GetKeyInput() const{
		return pImpl->m_KeyInput;
	}
	bool StringSprite::IsKeyInput() const{
		return pImpl->m_KeyInput;

	}
	void StringSprite::SetKeyInput(bool b){
		pImpl->m_KeyInput = b;
	}

	bool StringSprite::IsFocus(){
		if (!IsKeyInput()){
			return false;
		}
		shared_ptr<InputTextManager> Manager = GetStage()->GetInputTextManager();
		if (Manager){
			auto Ptr = GetThis<StringSprite>();
			if (Manager->GetFocusInputString() == Ptr){
				return true;
			}
		}
		return false;
	}
	void StringSprite::SetFocus(bool b){
		if (!IsKeyInput()){
			return;
		}
		shared_ptr<InputTextManager> Manager = GetStage()->GetInputTextManager();
		if (Manager){
			if (b){
				Manager->SetFocusInputString(GetThis<StringSprite>());
			}
			else{
				Manager->SetFocusInputString(nullptr);
			}
		}
	}



	ComPtr<IDWriteTextLayout>& StringSprite::GetTextLayout()const{
		return pImpl->m_textLayout;
	}
	ComPtr<IDWriteTextFormat>&	StringSprite::GetTextFormat()const{
		return pImpl->m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringSprite::GetDriteTextMetrics() const{
		return pImpl->m_textMetrics;
	}

	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}

	Rect2D<float> StringSprite::GetTextRect() const{
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringSprite::SetTextRect(const Rect2D<float>& rect){
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}



	void StringSprite::OnKeyDown(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring TempText = GetText();
		switch (wParam){
		case VK_BACK:
			if (pImpl->m_Caret > 0 && TempText.size() > 0){
				pImpl->m_Caret--;
				wstring LeftStr = TempText.substr(0, pImpl->m_Caret);
				wstring RightStr = TempText.substr(pImpl->m_Caret + 1);
				TempText = LeftStr + RightStr;
				SetText(TempText);
			}
			break;
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
	void StringSprite::OnChar(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring str(L"");
		switch (wParam){
		case VK_RIGHT:
		case VK_LEFT:
		case VK_BACK:
			break;
		default:
			str += (wchar_t)wParam;
			InsertText(str);
			break;
		}
	}


	void StringSprite::Update(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
			pImpl->m_text.c_str(),
			(uint32)pImpl->m_text.length(),
			pImpl->m_textFormat.Get(),
			pImpl->m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
			pImpl->m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
			&pImpl->m_textLayout
			),
			L"�t�H���g���C�A�E�g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextLayout()",
			L"StringSprite::Update()"
			);
		ThrowIfFailed(
			pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
			L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringSprite::Update()"
			);

	}



	void StringSprite::Draw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2D1_RECT_F BackRectangle = D2D1::RectF(
			pImpl->m_StartPosition.x, pImpl->m_StartPosition.y,
			pImpl->m_StartPosition.x + pImpl->m_TextBlockWidth,
			pImpl->m_StartPosition.y + pImpl->m_TextBlockHeight
			);
		BackRectangle.left -= pImpl->m_BackTextMargin.x;
		BackRectangle.top -= pImpl->m_BackTextMargin.y;
		BackRectangle.right += pImpl->m_BackTextMargin.x;
		BackRectangle.bottom += pImpl->m_BackTextMargin.y;

		D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//�o�b�N�O���E���h�̕`��
		D2DDeviceContext->FillRectangle(&BackRectangle,pImpl->m_BackBrush.Get());


		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			pImpl->m_StartPosition.x,
			pImpl->m_StartPosition.y
			);

		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (pImpl->m_TextAlignment){
		case TextAlignment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAlignment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAlignment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			pImpl->m_textFormat->SetTextAlignment(Alignment),
			L"�e�L�X�g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringSprite::Draw()"
			);

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			pImpl->m_textLayout.Get(),
			pImpl->m_Brush.Get()
			);

		//���͂ł��ăt�H�[�J�X������΁A�L�����b�g�\��
		if (IsKeyInput() && IsFocus()){
			pImpl->m_CaretCounter += ElapsedTime;
			if (pImpl->m_CaretSpeed == 0.0f){
				pImpl->m_CaretCounter = 0;
				pImpl->m_CaretState = true;
			}
			else{
				if (pImpl->m_CaretCounter >= pImpl->m_CaretSpeed){
					pImpl->m_CaretCounter = 0;
					if (pImpl->m_CaretState){
						pImpl->m_CaretState = false;
					}
					else{
						pImpl->m_CaretState = true;
					}
				}
			}
			if (pImpl->m_CaretState){
				float x, y;
				DWRITE_HIT_TEST_METRICS m;
				pImpl->m_textLayout->HitTestTextPosition(pImpl->m_Caret, false, &x, &y, &m);

				ComPtr<IDWriteTextLayout>       m_CaretLayout;
				ThrowIfFailed(
					DWriteFactory->CreateTextLayout(
					L"|",
					(uint32)1,
					pImpl->m_textFormat.Get(),
					pImpl->m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
					pImpl->m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
					&m_CaretLayout
					),
					L"�L�����b�g���C�A�E�g�ݒ�Ɏ��s���܂����B",
					L"DWriteFactory->CreateTextLayout()",
					L"StringSprite::Draw()"
					);
				ThrowIfFailed(
					pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
					L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
					L"pImpl->m_textLayout->GetMetrics()",
					L"StringSprite::Draw()"
					);

				D2DDeviceContext->DrawTextLayout(
					D2D1::Point2F(x, y),
					m_CaretLayout.Get(),
					pImpl->m_CaretBrush.Get()
					);
			}

		}
		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)){
				throw BaseException(
					L"�������\���ł��܂���ł���",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"StringSprite::Draw()"
					);
			}
		}
		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());
	}


}

//end basedx11
