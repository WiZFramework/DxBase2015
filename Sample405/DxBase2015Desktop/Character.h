#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��{�b�N�X
	//--------------------------------------------------------------------------------------
	class CustomDrawBox : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//�R���X�^���g�o�b�t�@�\����
		struct ConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Vector4 Param;	//�ėp�p�����[�^

		};
		//�R���X�^���g�o�b�t�@
		shared_ptr<CBuffer<ConstantBuffer>> m_ConstantBuffer;
		//���b�V�����\�[�X
		shared_ptr<CommonMeshResource> m_MeshResource;
		//���_�V�F�[�_
		shared_ptr< VShader<VertexPositionNormalTexture> > m_VirtexShader;
		//�s�N�Z���V�F�[�_�[
		shared_ptr<PShader> m_PixelShader;
		//���b�V�����\�[�X�̍쐬
		void CreateMeshResource();
		//���_�̕ύX
		void UpdateMeshResource();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawBox();
		//������
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() override;
	};

}
//endof  basedx11
