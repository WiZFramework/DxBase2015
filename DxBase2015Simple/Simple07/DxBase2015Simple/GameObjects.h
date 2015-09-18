#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct TextureSkin3DConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct TextureSkin3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		XMVECTOR Bones[3 * 72];
		TextureSkin3DConstantBuffer() {
			memset(this, 0, sizeof(TextureSkin3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTextureSkin3D : public ConstantBuffer<CBTextureSkin3D, TextureSkin3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBTextureSkin3D : public ConstantBuffer<CBTextureSkin3D, TextureSkin3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTextureSkin3D : public VertexShader<VSTextureSkin3D, VertexPositionNormalTextureSkinning>;
	//	�p�r:  VSTextureSkin3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSTextureSkin3D : public VertexShader<VSTextureSkin3D, VertexPositionNormalTextureSkinning>{
	public:
		//�\�z
		VSTextureSkin3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTextureSkin3D : public PixelShader<PSTextureSkin3D>;
	//	�p�r: PSTextureSkin3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSTextureSkin3D : public PixelShader<PSTextureSkin3D>{
	public:
		//�\�z
		PSTextureSkin3D();
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
		MaterialCount,
		SkinedVertex,
		BoneCount,
		AnimeMatrix,
		End = 100
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
		//�{�[���̐�
		UINT m_BoneCount;
		//�T���v�����O��
		UINT m_SampleCount;
		//�T���v�����O���ꂽ�{�[���s��
		vector<Matrix4X4> m_SampleMatrix;
		//�V�F�[�_�ɓn���{�[���s��
		vector<Matrix4X4> m_LocalBonesMatrix;
		//�T���v�����ЂƉ�肷��̂ɗv���鎞�ԁi�b�j
		float m_LoopTime;
		//���b�V���f�[�^�̓ǂݍ���
		void ReadMesh(vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, 
			vector<MaterialEx>& materials,vector<Matrix4X4>& bonematrix);
		//���b�V���̍쐬
		void CreateCustomMesh();
		//�V�F�[�_���\�[�X�r���[�̍쐬
		ComPtr<ID3D11ShaderResourceView> CreateShaderResView(const wstring& TextureFileName);
		//��ԏ���
		void InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out);
		void CalucLocalBonesMatrix(float TgtTime);
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
