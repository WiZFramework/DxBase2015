#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//������
		virtual void Create() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxManager : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X�̃}�l�[�W���i�`�悷��j
	//--------------------------------------------------------------------------------------
	class FixedBoxManager : public GameObject{
		//���_�ƃC���f�b�N�X���s��ƁA�i���o�[�ɍ��킹�ĕύX����
		void ChangeVertex(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, const Matrix4X4& Mat, size_t Num);
		//���\�[�X
		shared_ptr<CommonMeshResource> m_BoxesRes;
	public:
		//�\�z�Ɣj��
		FixedBoxManager(shared_ptr<Stage>& StagePtr);
		virtual ~FixedBoxManager();
		//������
		virtual void Create() override;
	};


}
//endof  basedx11
