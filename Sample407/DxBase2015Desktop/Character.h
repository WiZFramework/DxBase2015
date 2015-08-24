#pragma once

#include "stdafx.h"

namespace basedx11{

	//�J�X�^���`��R���X�^���g�o�b�t�@�\����
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
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
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionColor>;
	//	�p�r: VSCustomDraw���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionColor>{
	public:
		//�\�z
		VSCustomDraw();
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
	//	class CustomDrawVortex : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��Q����
	//--------------------------------------------------------------------------------------
	class CustomDrawVortex : public GameObject{
		Vector3 m_StartPos;
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�o�b�N�A�b�v�p�̒��_�z��
		vector<VertexPositionColor> m_BackupVirtex;
		//���b�V���̍쐬
		void CreateCustomMesh();
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawVortex(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CustomDrawVortex();
		//������
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() override;
	};

}
//endof  basedx11
