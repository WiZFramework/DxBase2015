#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
#include "GameObject2.h"
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

	GameObject2::GameObject2(const shared_ptr<GameStage2>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos)
	{
		m_Quaternion.RotationRollPitchYawFromVector(StartRotation);
		m_TextureFileName = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
	}

	//���b�V���̍쐬
	void GameObject2::CreateCustomMesh(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//��8�ʑ̂̍쐬
		VertexUtil::CreateOctahedron(1.0f, vertices, indices);
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
	void GameObject2::CreateShaderResView(){
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

	void GameObject2::Create(){
		//���b�V���̍쐬
		CreateCustomMesh();
		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		CreateShaderResView();
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void GameObject2::Update(){
		//�Q�[���X�e�[�W�������Ȃ烊�^�[��
		if (m_GameStgae.expired()){
			return;
		}
		//�R���g���[���̎擾
		auto CntrolStateVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntrolStateVec[0].bConnected){
			if (CntrolStateVec[0].wPressedButtons & XINPUT_GAMEPAD_X){
				//X�{�^���������ꂽ�u�ԂȂ�
				//�Q�[���X�e�[�W�̍쐬
				auto Stage = Object::CreateObject<GameStage>();
				//���̃Q�[���X�e�[�W�Ƃ��Đݒ�
				//SetNextStage()�֐��͎���UpdateApp()�ŗL���ɂȂ�B
				App::GetApp()->SetNextStage(Stage);
				//�X�e�[�W��ύX�����ꍇ�͕K��������Update�I��
				return;
			}
		}

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion SpanQt;
		SpanQt.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		m_Quaternion *= SpanQt;
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}




	void GameObject2::Draw(){
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
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
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
		pID3D11DeviceContext->PSSetShaderResources(0, 1, m_ShaderResView.GetAddressOf());
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSTexture3D::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBTexture3D::GetPtr()->GetBuffer();
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




	void GameStage2::Create(){
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

		auto Obj = Object::CreateObject<GameObject2>(
			GetThis<GameStage2>(),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 2.0f)
			);
		m_GameObjectVec.push_back(Obj);
		//�����������
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

	}

	void GameStage2::Update(){
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

	void GameStage2::Draw(){
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
