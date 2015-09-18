#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBTextureSkin3D : public ConstantBuffer<CBTextureSkin3D, TextureSkin3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBTextureSkin3D, CBTextureSkin3D::Deleter> CBTextureSkin3D::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSTextureSkin3D : public VertexShader<VSTextureSkin3D, VertexPositionNormalTextureSkinning>;
	//	�p�r:  VSTextureSkin3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSTextureSkin3D, VSTextureSkin3D::Deleter> VSTextureSkin3D::m_Ptr;
	//�\�z
	VSTextureSkin3D::VSTextureSkin3D() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSTextureSkin3D.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSTextureSkin3D : public PixelShader<PSTextureSkin3D>;
	//	�p�r: PSTextureSkin3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<PSTextureSkin3D, PSTextureSkin3D::Deleter> PSTextureSkin3D::m_Ptr;
	//�\�z
	PSTextureSkin3D::PSTextureSkin3D() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSTextureSkin3D.cso")
	{
	}

	GameObject::GameObject(const shared_ptr<GameStage>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos),
		m_LoopTime(4.0f)
	{
		m_Quaternion.RotationRollPitchYawFromVector(StartRotation);
	}

	struct VertexPositionNormalTextureSkinningPOD{
		float position[3];
		float normal[3];
		float textureCoordinate[2];
		uint32_t indices[4];
		float weights[4];
	};

	struct MaterialExPOD{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		float m_Diffuse[4];
		//! �X�y�L�����[�i���ˌ��j
		float m_Specular[4];
		//! �A���r�G���g�i���F�j
		float m_Ambient[4];
		//! �G�~�b�V�u�i���ˌ��j
		float m_Emissive[4];
	};

	struct	BonePOD
	{
		//!��{�|�[�Y�ւ̍s��
		float	m_BindPose[4][4];
		//!���݂̍s��
		float	m_CurrentPose[4][4];
		//!�v�Z���ꂽ���݂̍s��
		float	m_ConbinedPose[4][4];
	};

	struct	MatrixPOD
	{
		float	m_Mat[4][4];
	};




	//���b�V���f�[�^�̓ǂݍ���
	void GameObject::ReadMesh(vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Matrix4X4>& bonematrix){
		auto MeshFileName = App::GetApp()->m_wstrRelativeDataPath + L"Chara_R.bin";
		BinaryReader Reader(MeshFileName);
		//�w�b�_�̓ǂݍ���
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0"){
			throw BaseException(
				L"�f�[�^�`�����Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		//���_�̓ǂݍ���
		auto blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::SkinedVertex){
			throw BaseException(
				L"���_(�X�L�����b�V��)�̃w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTextureSkinningPOD);
		auto pVertex = Reader.ReadArray<VertexPositionNormalTextureSkinningPOD>((size_t)VerTexSize);
		for (UINT i = 0; i < VerTexSize; i++){
			VertexPositionNormalTextureSkinning v;
			v.position.x = pVertex[i].position[0];
			v.position.y = pVertex[i].position[1];
			v.position.z = pVertex[i].position[2];
			v.normal.x = pVertex[i].normal[0];
			v.normal.y = pVertex[i].normal[1];
			v.normal.z = pVertex[i].normal[2];
			v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
			v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
			for (int j = 0; j < 4; j++){
				v.indices[j] = pVertex[i].indices[j];
				v.weights[j] = pVertex[i].weights[j];
			}
			vertices.push_back(v);
		}

		//�C���f�b�N�X�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index){
			throw BaseException(
				L"�C���f�b�N�X�̃w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++){
			indices.push_back(pIndex[i]);
		}

		//�}�e���A���̓ǂݍ���
		//�}�e���A�����̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount){
			throw BaseException(
				L"�}�e���A�����̃w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++){
			//�e�N�X�`���t�@�C�������ϒ��Ȃ̂Œ��ӁB
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material){
				throw BaseException(
					L"�}�e���A���̃w�b�_���Ⴂ�܂�",
					MeshFileName,
					L"GameObject::ReadMesh()"
					);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!�J�n�C���f�b�N�X
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!�`��C���f�b�N�X�J�E���g
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! �f�t�B�[�Y�i���̂̐F�j
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! �X�y�L�����[�i���ˌ��j
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! �A���r�G���g�i���F�j
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! �G�~�b�V�u�i���ˌ��j
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			TextureFileStr = App::GetApp()->m_wstrRelativeDataPath + TextureFileStr;
			ToM.m_ShaderResView = CreateShaderResView(TextureFileStr);
			materials.push_back(ToM);
		}
		//�{�[����
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::BoneCount){
			throw BaseException(
				L"�{�[�����̃w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		m_BoneCount = blockHeader.m_Size;
		//�{�[���A�j���[�V�����s��
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::AnimeMatrix){
			throw BaseException(
				L"�A�j���[�V�����s��̃w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		auto MatrixSize = blockHeader.m_Size / sizeof(MatrixPOD);
		auto pAnimeMatrix = Reader.ReadArray<MatrixPOD>((size_t)MatrixSize);
		for (UINT i = 0; i < MatrixSize; i++){
			//�{�[���P�ʂł͂Ȃ��s��P�ʂœǂݍ���
			Matrix4X4 mat;
			for (int u = 0; u < 4; u++){
				for (int v = 0; v < 4; v++){
					mat(u, v) = pAnimeMatrix->m_Mat[u][v];
				}
			}
			bonematrix.push_back(mat);
			pAnimeMatrix++;
		}
		m_SampleCount = MatrixSize / m_BoneCount;

		//End
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End){
			throw BaseException(
				L"�I���w�b�_���Ⴂ�܂�",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}



	}


	//���b�V���̍쐬
	void GameObject::CreateCustomMesh(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionNormalTextureSkinning> vertices;
		vector<uint16_t> indices;
		ReadMesh(vertices, indices, m_Materials, m_SampleMatrix);
		//���_�o�b�t�@�̍쐬�i���_�͕ύX�ł��Ȃ��j
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());
		//�C���f�b�N�X�o�b�t�@�̍쐬
		VertexUtil::CreateIndexBuffer(m_IndexBuffer, indices);
		//�C���f�b�N�X���̐ݒ�
		m_NumIndicis = static_cast<UINT>(indices.size());
		//�V�F�[�_�[�ɓn���{�[���s��̏�����
		m_LocalBonesMatrix.resize(m_BoneCount);
		for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++){
			m_LocalBonesMatrix[i] = m_SampleMatrix[m_BoneCount * 15 + i];
		}

	}

	//�V�F�[�_���\�[�X�r���[�̍쐬
	ComPtr<ID3D11ShaderResourceView> GameObject::CreateShaderResView(const wstring& TextureFileName){
		//�e�N�X�`���쐬
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(TextureFileName.c_str(), 0, &metadata, image),
			L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
			TextureFileName,
			L"GameObject::CreateShaderResView()"
			);
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();
		//�~���[�e�b�N�X
		std::mutex Mutex;
		ComPtr<ID3D11ShaderResourceView> ResView;
		Util::DemandCreate(ResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
		{
			// �摜����V�F�[�_���\�[�XView�̍쐬
			return ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
				L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
				L"if( FAILED( CreateShaderResourceView() ) )",
				L"Texture::Impl::Impl()"
				);
		});
		return ResView;
	}

	void GameObject::Create(){
		//���b�V���̍쐬
		CreateCustomMesh();
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void GameObject::InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out){
		Vector3 Scale1, Pos1;
		Quaternion Qt1;
		m1.Decompose(&Scale1, &Qt1, &Pos1);
		Qt1.Normalize();

		Vector3 Scale2, Pos2;
		Quaternion Qt2;

		m2.Decompose(&Scale2, &Qt2, &Pos2);
		Qt2.Normalize();

		Vector3 ScaleOut, PosOut;
		Quaternion QtOut;

		ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
		PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
		QtOut = QuaternionEX::Slerp(Qt1, Qt2, t);
		out.DefTransformation(ScaleOut, QtOut, PosOut);
	}


	void GameObject::CalucLocalBonesMatrix(float TgtTime){
		//1�̃T���v���ɗv���鎞�Ԃ�}��
		auto OneSampleTime = m_LoopTime / m_SampleCount;
		auto FLOATTgtSample = TgtTime / OneSampleTime;
		UINT UITgtSample = (UINT)FLOATTgtSample;
		if (UITgtSample >= (m_SampleCount - 1)){
			UITgtSample = m_SampleCount - 1;
			//�Ō�̃T���v����\��
			for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++){
				m_LocalBonesMatrix[i] = m_SampleMatrix[m_BoneCount * UITgtSample + i];
			}
		}
		else{
			//�T���v���ƃT���v���̊Ԃ̊������v�Z
			FLOATTgtSample -= (float)UITgtSample;
			UINT UINextSample = UITgtSample + 1;
			for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++){
				InterpolationMatrix(
					m_SampleMatrix[m_BoneCount * UITgtSample + i],
					m_SampleMatrix[m_BoneCount * UINextSample + i], 
					FLOATTgtSample, m_LocalBonesMatrix[i]);
			}
		}
	}


	void GameObject::Update(){
		//�T���v����ύX����
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		static float NowTime = 0.0f;
		NowTime += ElapsedTime;
		if (NowTime >= m_LoopTime){
			NowTime = 0.0f;
		}
		CalucLocalBonesMatrix(NowTime);
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
		TextureSkin3DConstantBuffer cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.Model = Matrix4X4EX::Transpose(m_WorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//���C�g�̐ݒ�
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = ViewPtr->GetMultiLight()->GetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//�{�[���̐ݒ�
		size_t BoneSz = m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++){
			Matrix4X4 mat = m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBTextureSkin3D::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
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
		pID3D11DeviceContext->VSSetShader(VSTextureSkin3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTextureSkin3D::GetPtr()->GetShader(), nullptr, 0);
		//���j�A�T���v���[��ݒ�
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		for (auto& m : m_Materials){
			//�e�N�X�`����ݒ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_ShaderResView.GetAddressOf());
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSTextureSkin3D::GetPtr()->GetInputLayout());
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBTextureSkin3D::GetPtr()->GetBuffer();
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�����_�����O�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//�`��
			pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			//�����_�����O�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//�`��
			pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex,0);
		}
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
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -1.0f, 2.0f)
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
