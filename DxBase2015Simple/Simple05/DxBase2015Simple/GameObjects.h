#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct TextureColor3DConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct TextureColor3DConstantBuffer
	{
		Matrix4X4 Model;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		TextureColor3DConstantBuffer() {
			memset(this, 0, sizeof(TextureColor3DConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D,TextureColor3DConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D, TextureColor3DConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSTextureColor3D : public VertexShader< VSTextureColor3D, VertexPositionNormalColorTexture>;
	//	�p�r:  VSTextureColor3D���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSTextureColor3D : public VertexShader<VSTextureColor3D, VertexPositionNormalColorTexture>{
	public:
		//�\�z
		VSTextureColor3D();
	};

	//--------------------------------------------------------------------------------------
	//	class PSTextureColor3D : public PixelShader<PSTextureColor3D>;
	//	�p�r: PSTextureColor3D�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSTextureColor3D : public PixelShader<PSTextureColor3D>{
	public:
		//�\�z
		PSTextureColor3D();
	};

	class GameStage;

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
		//�e�N�X�`���t�@�C����
		wstring m_TextureFileName;
		//�V�F�[�_���\�[�X�r���[�i�e�N�X�`�����\�[�X�j
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//���\�[�X�r���[
		//���b�V���̍쐬
		void CreateCustomMesh();
		//�V�F�[�_���\�[�X�r���[�̍쐬
		void CreateShaderResView();
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
