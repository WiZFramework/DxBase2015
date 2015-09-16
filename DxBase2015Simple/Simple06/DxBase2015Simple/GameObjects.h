#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Texture3DConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct Texture3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Texture3DConstantBuffer() {
			memset(this, 0, sizeof(Texture3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTexture3D : public ConstantBuffer<CBTexture3D,Texture3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBTexture3D : public ConstantBuffer<CBTexture3D, Texture3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>;
	//	�p�r:  VSTexture3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>{
	public:
		//�\�z
		VSTexture3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTexture3D : public PixelShader<PSTexture3D>;
	//	�p�r: PSTexture3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSTexture3D : public PixelShader<PSTexture3D>{
	public:
		//�\�z
		PSTexture3D();
	};

	class GameStage;

	struct MaterialEx{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Color4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Color4 m_Specular;
		//! �A���r�G���g�i���F�j
		Color4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Color4 m_Emissive;
		//�V�F�[�_���\�[�X�r���[�i�e�N�X�`�����\�[�X�j
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;
	};

	enum class BlockType{
		Vertex,
		Index,
		Material,
		MaterialCount
	};

	struct BlockHeader{
		BlockType m_Type;
		UINT m_Size;
	};



	//--------------------------------------------------------------------------------------
	//	class GameObject : public Object, public SimpleInterface;
	//	�p�r: �Q�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class GameObject : public Object, public SimpleInterface{
		weak_ptr<GameStage> m_GameStgae;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Matrix4X4 m_WorldMatrix;
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�C���f�b�N�X�̐�
		UINT m_NumIndicis;
		//�}�e���A���̔z��
		vector<MaterialEx> m_Materials;
		//���b�V���f�[�^�̓ǂݍ���
		void ReadMesh(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials);
		//���b�V���̍쐬
		void CreateCustomMesh();
		//�V�F�[�_���\�[�X�r���[�̍쐬
		ComPtr<ID3D11ShaderResourceView> CreateShaderResView(const wstring& TextureFileName);
	public:
		GameObject(const shared_ptr<GameStage>& GStage,
			const Vector3& StartScale,
			const Vector3& StartRotation,
			const Vector3& StartPos);
		virtual ~GameObject(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


	//--------------------------------------------------------------------------------------
	//	class GameStage : public StageBase;
	//	�p�r: �Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	class GameStage : public StageBase{
		vector<shared_ptr<GameObject>> m_GameObjectVec;
		shared_ptr<View> m_View;
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//�f�t�H���g�̃����_�����O�^�[�Q�b�g
		//������
		shared_ptr<StringSprite> m_StringSprite;
	public:
		GameStage(){}
		virtual ~GameStage(){}
		const shared_ptr<View>& GetView() const{
			return m_View;
		}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
