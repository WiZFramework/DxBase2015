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


	//--------------------------------------------------------------------------------------
	//	class ObjectResource : public Object, public SimpleInterface;
	//	�p�r: �I�u�W�F�N�g���\�[�X
	//--------------------------------------------------------------------------------------
	class ObjectResource : public Object, public SimpleInterface{
	protected:
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�C���f�b�N�X�̐�
		UINT m_NumIndicis;
		ObjectResource(){}
		virtual ~ObjectResource(){}
	public:
		const ComPtr<ID3D11Buffer>& GetVertexBuffer() const{
			return m_VertexBuffer;
		}
		const ComPtr<ID3D11Buffer>& GetIndexBuffer() const{
			return m_IndexBuffer;
		}
		UINT GetNumVertices() const{
			return m_NumVertices;
		}
		UINT GetNumIndicis() const{
			return m_NumIndicis;
		}
		virtual void Update()override{}
		virtual void Draw()override{}
	};

	//--------------------------------------------------------------------------------------
	//	class CubeResource : public ObjectResource;
	//	�p�r: �����̃��\�[�X
	//--------------------------------------------------------------------------------------
	class CubeResource : public ObjectResource{
	public:
		CubeResource(){}
		virtual ~CubeResource(){}
		virtual void Create()override;
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public Object, public SimpleInterface;
	//	�p�r: �e�N�X�`�����\�[�X
	//--------------------------------------------------------------------------------------
	class TextureResource : public Object, public SimpleInterface{
	protected:
		//�e�N�X�`���t�@�C����
		wstring m_TextureFileName;
		//�V�F�[�_���\�[�X�r���[�i�e�N�X�`�����\�[�X�j
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//���\�[�X�r���[

		void CreateTextureBase();
	public:
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResView() const{
			return m_ShaderResView;
		}
		TextureResource(){}
		TextureResource(const wstring& str) :
			m_TextureFileName(str)
		{}
		virtual ~TextureResource(){}
		void CreateTexture(const wstring& str);

		virtual void Create()override;
		virtual void Update()override{}
		virtual void Draw()override{}

	};


	class GameStage;

	//--------------------------------------------------------------------------------------
	//	class CubeObject : public Object, public SimpleInterface;
	//	�p�r: �����̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class CubeObject : public Object, public SimpleInterface{
		weak_ptr<GameStage> m_GameStgae;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Matrix4X4 m_WorldMatrix;
		float m_Span;
		bool m_IsHit;
	public:
		CubeObject(const shared_ptr<GameStage>& GStage,
			const Vector3& StartScale,
			const Vector3& StartRotation,
			const Vector3& StartPos,
			float Span);
		virtual ~CubeObject(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Update2();
		virtual void Draw()override;
		//�Փˊ֘A
		OBB GetOBB();
		void SetHit();
	};





	//--------------------------------------------------------------------------------------
	//	class GameStage : public StageBase;
	//	�p�r: �Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	class GameStage : public StageBase{
		vector<shared_ptr<CubeObject>> m_CubeObjectVec;
		shared_ptr<View> m_View;
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//�f�t�H���g�̃����_�����O�^�[�Q�b�g
		//������
		shared_ptr<StringSprite> m_StringSprite;
		//�����̃��\�[�X
		shared_ptr<CubeResource> m_CubeResource;
		//�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_TextureResource;
	public:
		GameStage(){}
		virtual ~GameStage(){}
		const shared_ptr<View>& GetView() const{
			return m_View;
		}
		const shared_ptr<CubeResource>& GetCubeResource() const{
			return m_CubeResource;
		}
		const shared_ptr<TextureResource>& GetTextureResource() const{
			return m_TextureResource;
		}
		const vector<shared_ptr<CubeObject>>& GetCubeObjectVec(){
			return m_CubeObjectVec;
		}



		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
