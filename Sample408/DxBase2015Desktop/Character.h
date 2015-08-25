#pragma once

#include "stdafx.h"

namespace basedx11{

	//�J�X�^���V���h�E�}�b�v�p�R���X�^���g�o�b�t�@�\����
	struct CustomShadowmapConstantBuffer
	{
		XMMATRIX mWorld[3];
		XMMATRIX mView;
		XMMATRIX mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap,CustomShadowmapConstantBuffer>;
	//	�p�r: �J�X�^���R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap, CustomShadowmapConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>;
	//	�p�r: VSCustomShadowmap���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>{
	public:
		//�\�z
		VSCustomShadowmap();
	};
	//--------------------------------------------------------------------------------------
	//	class GSCustomShadowmap : public GeometryShader<GSCustomShadowmap>;
	//	�p�r: CustomShadowmap�W�I���g���V�F�[�_
	//--------------------------------------------------------------------------------------
	class GSCustomShadowmap : public GeometryShader<GSCustomShadowmap>{
	public:
		//�\�z
		GSCustomShadowmap();
	};

	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	�p�r: �J�X�^���V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap{
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CustomShadowmap();
		//����
		virtual void Draw();

	};


	//�J�X�^���`��R���X�^���g�o�b�t�@�\����
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World[3];
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//�ėp�p�����[�^
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBCustomDraw : public ConstantBuffer<CBCustomDraw,CustomDrawConstantBuffer>;
	//	�p�r: �J�X�^���R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBCustomDraw : public ConstantBuffer<CBCustomDraw, CustomDrawConstantBuffer>{
	};

	//--------------------------------------------------------------------------------------
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>;
	//	�p�r: VSCustomDraw���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>{
	public:
		//�\�z
		VSCustomDraw();
	};
	//--------------------------------------------------------------------------------------
	//	class GSCustomDraw : public GeometryShader<GSCustomDraw>;
	//	�p�r: GSCustomDraw�W�I���g���V�F�[�_
	//--------------------------------------------------------------------------------------
	class GSCustomDraw : public GeometryShader<GSCustomDraw>{
	public:
		//�\�z
		GSCustomDraw();
	};
	//--------------------------------------------------------------------------------------
	//	class PSCustomDraw : public PixelShader<PSCustomDraw>;
	//	�p�r: PSCustomDraw�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSCustomDraw : public PixelShader<PSCustomDraw>{
	public:
		//�\�z
		PSCustomDraw();
	};





	//--------------------------------------------------------------------------------------
	//	class CustomDrawOctahedron : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��8�ʑ�
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�C���f�b�N�X�̐�
		UINT m_NumIndicis;
		//�o�b�N�A�b�v�p�̒��_�z��
		vector<VertexPositionNormalColor> m_BackupVirtex;
		//���b�V���̍쐬
		void CreateCustomMesh();
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawOctahedron();
		//�A�N�Z�T
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

		//������
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() override;
	};

}
//endof  basedx11
