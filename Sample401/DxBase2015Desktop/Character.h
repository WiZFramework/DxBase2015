#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class TriangleObject : public GameObject;
	//	�p�r: �O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class TriangleObject : public GameObject{
		//�o�b�t�@�֘A
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
	public:
		//�\�z�Ɣj��
		TriangleObject(const shared_ptr<Stage>& StagePtr);
		virtual ~TriangleObject();
		//������
		virtual void Create() override;
		//�`��
		virtual void Draw()override;
	};

}
//endof  basedx11
