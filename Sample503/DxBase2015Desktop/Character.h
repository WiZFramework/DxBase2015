#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	class Jump
	//	�p�r: �W�����v�̌v�Z��Ԃ��֐��I�u�W�F�N�g�e���v���[�g
	//--------------------------------------------------------------------------------------
	template<typename T>
	class Jump{
		T m_T;
	public:
		float operator()(){
			return m_T();
		}
	};

	//--------------------------------------------------------------------------------------
	//	class ShortType;
	//	�p�r: �V���[�g�W�����v�̌v�Z��Ԃ��֐��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ShortType{
	public:
		float operator()(){
			return 2.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class MediumType;
	//	�p�r: �~�f�B�A���W�����v�̌v�Z��Ԃ��֐��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class MediumType{
	public:
		float operator()(){
			return 4.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class class LongType;
	//	�p�r: �����O�W�����v�̌v�Z��Ԃ��֐��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class LongType{
	public:
		float operator()(){
			return 6.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class LongLongType;
	//	�p�r: �����O�����O�W�����v�̌v�Z��Ԃ��֐��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class LongLongType{
	public:
		float operator()(){
			return 8.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		Vector3 m_StartPos;
		float m_YRot;
		float m_RotationSpeed;
	public:
		//�\�z�Ɣj��
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update() override;
	};



}
//endof  basedx11
