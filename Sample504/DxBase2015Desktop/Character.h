#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle{
	public:
		//�\�z�Ɣj��
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//--------------------------------------------------------------------------------------
		//	void InsertSpark(
		//	const Vector3& Pos,	//���o�ʒu�i�G�~�b�^�[�ʒu�j
		//	const Color4& Col	//�F
		//	);
		//�p�r: �X�p�[�N����o����
		//�߂�l: �Ȃ��B
		//--------------------------------------------------------------------------------------
		void InsertSpark(const Vector3& Pos, const Color4& Col);
	};
	//�v���C���[�̐錾
	class Player;
	//--------------------------------------------------------------------------------------
	//	class AssignInterface;
	//	�p�r: �U�蕪������C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class AssignInterface{
	protected:
		AssignInterface(){}
		virtual ~AssignInterface(){}
	public:
		virtual void AssignAction(const shared_ptr<Player>& ply) = 0;

	};


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject,public AssignInterface;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject, public AssignInterface{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//������
		virtual void Create() override;
		//����
		virtual void AssignAction(const shared_ptr<Player>& ply)override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject, public AssignInterface;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject, public AssignInterface{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~MoveBox();
		//������
		virtual void Create() override;
		//����
		virtual void AssignAction(const shared_ptr<Player>& ply)override;
	};


}
//endof  basedx11
