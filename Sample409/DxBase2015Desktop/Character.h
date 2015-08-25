#pragma once

#include "stdafx.h"

namespace basedx11{

	struct BallConstantBuffer
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Proj;
		Vector3  m_LightDir;
		float m_Explode;
		BallConstantBuffer() {
			memset(this, 0, sizeof(BallConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class Ball : public GameObject;
	//	�p�r: ���_���ύX�����{�[��
	//--------------------------------------------------------------------------------------
	class Ball : public GameObject{
		Vector3 m_StartPos;
		//�R���X�^���g�o�b�t�@
		shared_ptr<CBuffer<BallConstantBuffer> > m_ConstantBuffer;
		//���_�V�F�[�_
		shared_ptr<VShader<VertexPositionNormalTexture> > m_VirtexShader;
		//�s�N�Z���V�F�[�_�[
		shared_ptr<PShader> m_PixelShader;
		//�W�I���g���V�F�[�_�[
		shared_ptr<GShader> m_GeometryShader;
		//��яo���W��
		float m_Explode;
		//���������ǂ���
		bool m_IsHalfAlpha;
	public:
		//�\�z�Ɣj��
		Ball(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Ball(){}
		//������
		virtual void Create() override;
		//�ω�
		virtual void Update() override;
		//�`��
		virtual void Draw() override;
	};

}
//endof  basedx11
