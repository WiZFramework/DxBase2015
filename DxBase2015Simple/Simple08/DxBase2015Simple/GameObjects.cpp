#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBTexture3D : public ConstantBuffer<CBTexture3D,Texture3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBTexture3D, CBTexture3D::Deleter> CBTexture3D::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>;
	//	�p�r:  VSTexture3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSTexture3D, VSTexture3D::Deleter> VSTexture3D::m_Ptr;
	//�\�z
	VSTexture3D::VSTexture3D() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSTexture3D.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSTexture3D : public PixelShader<PSTexture3D>;
	//	�p�r: PSTexture3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<PSTexture3D, PSTexture3D::Deleter> PSTexture3D::m_Ptr;
	//�\�z
	PSTexture3D::PSTexture3D() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSTexture3D.cso")
	{
	}


	//--------------------------------------------------------------------------------------
	//	class CubeResource : public ObjectResource;
	//	�p�r: �����̃��\�[�X
	//--------------------------------------------------------------------------------------
	void CubeResource::Create(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//�����̂̍쐬
		VertexUtil::CreateCube(1.0f, vertices, indices);
		//���_�o�b�t�@�̍쐬�i���_�͕ύX�ł��Ȃ��j
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());
		//�C���f�b�N�X�o�b�t�@�̍쐬
		VertexUtil::CreateIndexBuffer(m_IndexBuffer, indices);
		//�C���f�b�N�X���̐ݒ�
		m_NumIndicis = static_cast<UINT>(indices.size());
	}


	//--------------------------------------------------------------------------------------
	//	class TextureResource : public Object, public SimpleInterface;
	//	�p�r: �e�N�X�`�����\�[�X
	//--------------------------------------------------------------------------------------
	void TextureResource::CreateTextureBase(){
		//�e�N�X�`���쐬
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(m_TextureFileName.c_str(), 0, &metadata, image),
			L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
			m_TextureFileName,
			L"TextureResource::CreateTextureBase()"
			);
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();
		//�~���[�e�b�N�X
		std::mutex Mutex;
		Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
		{
			// �摜����V�F�[�_���\�[�XView�̍쐬
			return ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
				L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
				L"if( FAILED( CreateShaderResourceView() ) )",
				L"TextureResource::CreateTextureBase()"
				);
		});
	}

	void TextureResource::Create(){
		if (m_TextureFileName != L""){
			CreateTextureBase();
		}
	}
	void TextureResource::CreateTexture(const wstring& str){
		if (str != L""){
			m_TextureFileName = str;
			CreateTextureBase();
		}
	}


	//--------------------------------------------------------------------------------------
	//	class CubeObject : public Object, public SimpleInterface;
	//	�p�r: �����̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const shared_ptr<GameStage>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos,
		float Span) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos),
		m_Span(Span),
		m_IsHit(false)
	{
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void CubeObject::Create(){
	}
	void CubeObject::Update(){
		m_IsHit = false;
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Position.x += m_Span * ElapsedTime;
		if (abs(m_Position.x) > 5.0f){
			m_Span *= -1.0f;
		}
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}
	void CubeObject::Update2(){
		//�Q�[���X�e�[�W�������Ȃ烊�^�[��
		if (m_GameStgae.expired()){
			return;
		}
		if (m_IsHit){
			return;
		}
		auto GameStgaePtr = m_GameStgae.lock();
		//OBB���쐬
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), m_WorldMatrix);
		auto CubeVec = GameStgaePtr->GetCubeObjectVec();
		for (auto C : CubeVec){
			if (C != GetThis<CubeObject>()){
				OBB OtherObb = C->GetOBB();
				//�g���l�����ۂ��������Ȃ�OBB��OBB����
				if (HitTest::OBB_OBB(obb, OtherObb)){
					SetHit();
					C->SetHit();
				}
			}
		}
	}
	OBB CubeObject::GetOBB(){
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), m_WorldMatrix);
		return obb;
	}

	void CubeObject::SetHit(){
		m_Span *= -1.0f;
		m_IsHit = true;
	}


	void CubeObject::Draw(){
		//�Q�[���X�e�[�W�������Ȃ烊�^�[��
		if (m_GameStgae.expired()){
			return;
		}
		auto GameStgaePtr = m_GameStgae.lock();
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = Dev->GetRenderState();
		auto Stage = m_GameStgae.lock();
		auto ViewPtr = Stage->GetView();
		//�r���[����J���������o��
		auto PtrCamera = ViewPtr->GetCamera();
		//�J�����̎擾
		Matrix4X4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		Texture3DConstantBuffer cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.Model = Matrix4X4EX::Transpose(m_WorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//���C�g�̐ݒ�
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = ViewPtr->GetMultiLight()->GetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBTexture3D::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, 
			GameStgaePtr->GetCubeResource()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(
			GameStgaePtr->GetCubeResource()->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�h��Ԃ������i����̉摜��JPEG�Ȃ̂œ����ɂ��Ȃ��j
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		//���j�A�T���v���[��ݒ�
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//�e�N�X�`����ݒ�
		pID3D11DeviceContext->PSSetShaderResources(0, 1, 
			GameStgaePtr->GetTextureResource()->GetShaderResView().GetAddressOf());
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSTexture3D::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBTexture3D::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(
			GameStgaePtr->GetCubeResource()->GetNumIndicis(), 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(
			GameStgaePtr->GetCubeResource()->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);
	}










	void GameStage::Create(){
		m_View = Object::CreateObject<View>();
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		m_View->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = m_View->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 2.0f));
		auto PtrLight = m_View->GetMultiLight();
		PtrLight->SetDefaultLighting();
		//�f�t�H���g�̃����_�����O�^�[�Q�b�g���쐬
		m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();

		//�����̃��\�[�X�̍쐬
		m_CubeResource = Object::CreateObject<CubeResource>();
		wstring TextureFile = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		//�e�N�X�`�����\�[�X�̍쐬
		m_TextureResource = Object::CreateObject<TextureResource>(TextureFile);

		
		auto Obj = Object::CreateObject<CubeObject>(
			GetThis<GameStage>(),
			Vector3(1.0f,1.0f,1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(-2.0f, 0.0f, 2.0f),
			2.0f
			);
		m_CubeObjectVec.push_back(Obj);

		Obj = Object::CreateObject<CubeObject>(
			GetThis<GameStage>(),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(2.0f, 0.0f, 2.0f),
			-2.0f
			);
		m_CubeObjectVec.push_back(Obj);



		//�����������
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


	}

	void GameStage::Update(){
		//�r���[�̕ω�
		m_View->Update();
		//�I�u�W�F�N�g�̕ω�
		for (auto Obj : m_CubeObjectVec){
			Obj->Update();
		}
		//�Փ˔���p��Update2
		for (auto Obj : m_CubeObjectVec){
			Obj->Update2();
		}

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		//������̕ω�
		m_StringSprite->SetText(FPS);
		m_StringSprite->Update();
	}

	void GameStage::Draw(){
		m_DefaultRenderTarget->ClearViews();
		m_DefaultRenderTarget->StartRenderTarget();
		for (auto Obj : m_CubeObjectVec){
			Obj->Draw();
		}
		//������̕`��
		m_StringSprite->Draw();
		m_DefaultRenderTarget->EndRenderTarget();
	}




}


//end basedx11
