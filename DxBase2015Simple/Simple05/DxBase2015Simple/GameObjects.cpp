#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D,TextureColor3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBTextureColor3D, CBTextureColor3D::Deleter> CBTextureColor3D::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSTextureColor3D : public VertexShader< VSTextureColor3D, VertexPositionNormalColorTexture>;
	//	�p�r:  VSTextureColor3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSTextureColor3D, VSTextureColor3D::Deleter> VSTextureColor3D::m_Ptr;
	//�\�z
	VSTextureColor3D::VSTextureColor3D() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSTextureColor3D.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSTextureColor3D : public PixelShader<PSTextureColor3D>;
	//	�p�r: PSTextureColor3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSTextureColor3D, PSTextureColor3D::Deleter> PSTextureColor3D::m_Ptr;
	//�\�z
	PSTextureColor3D::PSTextureColor3D() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSTextureColor3D.cso")
	{
	}

	GameObject::GameObject(const shared_ptr<GameStage>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos)
	{
		m_Quaternion.RotationRollPitchYawFromVector(StartRotation);
		m_TextureFileName = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
	}

	//���b�V���̍쐬
	void GameObject::CreateCustomMesh(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//��8�ʑ̂̍쐬
		VertexUtil::CreateOctahedron(1.0f, vertices, indices);
		//���_��ύX���邽�߂̔z��
		vector<VertexPositionNormalColorTexture> set_vertices;
		for (auto V : vertices){
			Color4 Col;
			if (V.position.y > 0){
				Col = Color4(1.0f, 1.0f, 0.0f, 0.0f);
			}
			else if (V.position.y < 0){
				Col = Color4(0.0f, 1.0f, 1.0f, 0.0f);
			}
			else{
				if (V.position.x == 0.0f){
					Col = Color4(1.0f, 1.0f, 0.0f, 1.0f);
				}
				else if (V.position.x > 0.0f){
					Col = Color4(0.0f, 0.0f, 1.0f, 1.0f);
				}
				else{
					Col = Color4(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			set_vertices.push_back(
				VertexPositionNormalColorTexture(V.position, V.normal, Col,V.textureCoordinate)
				);
		}
		//���_�o�b�t�@�̍쐬�i���_�͕ύX�ł��Ȃ��j
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, set_vertices);

		//���_�o�b�t�@�̍쐬�i���_�͕ύX�ł��Ȃ��j
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());
		//�C���f�b�N�X�o�b�t�@�̍쐬
		VertexUtil::CreateIndexBuffer(m_IndexBuffer, indices);
		//�C���f�b�N�X���̐ݒ�
		m_NumIndicis = static_cast<UINT>(indices.size());
	}

	//�V�F�[�_���\�[�X�r���[�̍쐬
	void GameObject::CreateShaderResView(){
		//�e�N�X�`���쐬
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(m_TextureFileName.c_str(), 0, &metadata, image),
			L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
			m_TextureFileName,
			L"GameObject::CreateShaderResView()"
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
				L"Texture::Impl::Impl()"
				);
		});
	}

	void GameObject::Create(){
		//���b�V���̍쐬
		CreateCustomMesh();
		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		CreateShaderResView();
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void GameObject::Update(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion SpanQt;
		SpanQt.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		m_Quaternion *= SpanQt;
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}




	void GameObject::Draw(){
		//�Q�[���X�e�[�W�������Ȃ烊�^�[��
		if (m_GameStgae.expired()){
			return;
		}
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
		TextureColor3DConstantBuffer cb1;
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
		pID3D11DeviceContext->UpdateSubresource(CBTextureColor3D::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalColorTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//��������
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSTextureColor3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTextureColor3D::GetPtr()->GetShader(), nullptr, 0);
		//���j�A�T���v���[��ݒ�
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//�e�N�X�`����ݒ�
		pID3D11DeviceContext->PSSetShaderResources(0, 1, m_ShaderResView.GetAddressOf());
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSTextureColor3D::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBTextureColor3D::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
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
		
		auto Obj = Object::CreateObject<GameObject>(
			GetThis<GameStage>(),
			Vector3(1.0f,1.0f,1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 2.0f)
			);
		m_GameObjectVec.push_back(Obj);
		//�����������
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

	}

	void GameStage::Update(){
		//�r���[�̕ω�
		m_View->Update();
		//�I�u�W�F�N�g�̕ω�
		for (auto Obj : m_GameObjectVec){
			Obj->Update();
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
		for (auto Obj : m_GameObjectVec){
			Obj->Draw();
		}
		//������̕`��
		m_StringSprite->Draw();
		m_DefaultRenderTarget->EndRenderTarget();
	}




}


//end basedx11
