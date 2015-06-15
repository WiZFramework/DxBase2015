/*!
@file Components.h
@brief �R���|�[�l���g�i�X�V�n�j

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	//	class Component :public Object, public ShapeInterface;
	/*!
	�R���|�[�l���g�e�N���X
	*/
	//--------------------------------------------------------------------------------------
	class Component :public Object, public ShapeInterface{
	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	explicit Component(
		//		const shared_ptr<GameObject>& GameObjectPtr	//���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		//	);
		/*!
		@breaf �v���e�N�g�R���X�g�N�^
		@param const shared_ptr<GameObject>& GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit Component(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	virtual ~Component();
		/*!
		@breaf �v���e�N�g�f�X�g�N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Component();
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	shared_ptr<GameObject> GetGameObject() const;
		/*!
		@breaf �Q�[���I�u�W�F�N�g�̎擾
		@param �Ȃ�
		@return	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		//	bool IsGameObjectActive() const;
		/*!
		@breaf ���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g���L�����ǂ������擾
		@param �Ȃ�
		@return	�Q�[���I�u�W�F�N�g���L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsGameObjectActive() const;
		//--------------------------------------------------------------------------------------
		//	void AttachGameObject(
		//		const shared_ptr<GameObject>& GameObjectPtr	//�V�����Q�[���I�u�W�F�N�g
		//	);
		/*!
		@breaf �Q�[���I�u�W�F�N�g��ݒ肷��
		@param const shared_ptr<GameObject>& GameObjectPtr	�V�����Q�[���I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	bool IsUpdateActive() const;
		/*!
		@breaf Update���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		//	bool GetUpdateActive() const;
		/*!
		@breaf Update���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		//	void SetUpdateActive(
		//		bool b	//�A�N�e�B�u���ǂ���
		//	);
		/*!
		@breaf Update���A�N�e�B�u���ǂ�����ݒ肷��
		@param bool b	�A�N�e�B�u���ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);

		//--------------------------------------------------------------------------------------
		//	bool IsUpdate2Active() const;
		/*!
		@breaf Update2���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdate2Active() const;
		//--------------------------------------------------------------------------------------
		//	bool GetUpdate2Active() const;
		/*!
		@breaf Update2���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdate2Active() const;
		//--------------------------------------------------------------------------------------
		//	void SetUpdate2Active(
		//		bool b	//�A�N�e�B�u���ǂ���
		//	);
		/*!
		@breaf Update2���A�N�e�B�u���ǂ�����ݒ肷��
		@param bool b	�A�N�e�B�u���ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdate2Active(bool b);

		//--------------------------------------------------------------------------------------
		//	bool IsDrawActive() const;
		/*!
		@breaf Draw���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const;
		//--------------------------------------------------------------------------------------
		//	bool GetDrawActive() const;
		/*!
		@breaf Draw���A�N�e�B�u���ǂ����𓾂�
		@param �Ȃ�
		@return	�A�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetDrawActive() const;
		//--------------------------------------------------------------------------------------
		//	void SetDrawActive(
		//		bool b	//�A�N�e�B�u���ǂ���
		//	);
		/*!
		@breaf Draw���A�N�e�B�u���ǂ�����ݒ肷��
		@param bool b	�A�N�e�B�u���ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b);

		//����
		//--------------------------------------------------------------------------------------
		//	virtual void Update2();
		/*!
		@breaf Update2����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update2(){}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	/*!
	�ϊ��N���X
	*/
	//--------------------------------------------------------------------------------------
	class Transform : public Component {
		//--------------------------------------------------------------------------------------
		//	void SetLocalQuaternion(
		//		const Quaternion& quaternion, //��]�N�I�[�^�j�I��
		//		bool CalcFlg = true		//�����ɍs��v�Z���邩�ǂ���
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalQuaternion(const Quaternion& quaternion, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalRotation(
		//		const Vector3& Rot,	//��]�x�N�g��
		//		bool CalcFlg = true	//�����ɍs��v�Z���邩�ǂ���
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalRotation(const Vector3& Rot, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalRotation(
		//		float x, //X��]
		//		float y, //Y��]
		//		float z, //Z��]
		//		bool CalcFlg = true	//�����ɍs��v�Z���邩�ǂ���
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalRotation(float x, float y, float z, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		const Vector3& Position, //�ʒu�x�N�g��
		//		bool CalcFlg = true	//�����ɍs��v�Z���邩�ǂ���
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalPosition(const Vector3& Position, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalPosition(
		//		float x, //X�ʒu
		//		float y, //Y�ʒu
		//		float z, //Z�ʒu
		//		bool CalcFlg = true //�����ɍs��v�Z���邩�ǂ���
		//	);
		/*!
		@breaf ���[�J���ʒu��ݒ肷��
		@param float x, X�ʒu
		@param float y, Y�ʒu
		@param float z, Z�ʒu
		@param bool CalcFlg = true		�����ɍs��v�Z���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLocalPosition(float x, float y, float z, bool CalcFlg = true);
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	explicit Transform(
		//		const shared_ptr<GameObject>& GameObjectPtr	//���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		//	);
		/*!
		@breaf �R���X�g�N�^
		@param const shared_ptr<GameObject>& GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit Transform(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	virtual ~Transform();
		/*!
		@breaf �f�X�g�N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Transform();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	bool IsPriorityMatrix()const;;
		/*!
		@breaf �s��D��ɂ��邩�ǂ���
		@param �Ȃ�
		@return	�s��D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityMatrix()const;
		//--------------------------------------------------------------------------------------
		//	bool GetPriorityMatrix()const;;
		/*!
		@breaf �s��D��ɂ��邩�ǂ���
		@param �Ȃ�
		@return	�s��D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetPriorityMatrix()const;
		//--------------------------------------------------------------------------------------
		//	void SetPriorityMatrix(
		//	bool b	//�s��D��Ȃ�true
		//	);
		/*!
		@breaf �s��D��ɂ��邩�ǂ������Z�b�g.
		���s��D��ɂ���Ǝ����s��v�Z�͍s��Ȃ�
		@param bool b	�s��D��Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityMatrix(bool b);
		//--------------------------------------------------------------------------------------
		//	bool IsPriorityPosition()const;
		/*!
		@breaf �s��v�Z���ʒu�D��ɂ��邩�ǂ���
		@param �Ȃ�
		@return	�ʒu�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityPosition()const;
		//--------------------------------------------------------------------------------------
		//	bool GetPriorityPosition()const;
		/*!
		@breaf �s��v�Z���ʒu�D��ɂ��邩�ǂ���
		@param �Ȃ�
		@return	�ʒu�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetPriorityPosition()const;
		//--------------------------------------------------------------------------------------
		//	void SetPriorityPosition(
		//	bool b //�D��ɂ��邩�ǂ���
		//	);
		/*!
		@breaf �s��v�Z���ʒu�D��ɂ��邩�ǂ������Z�b�g
		@param bool b �D��ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityPosition(bool b);
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetBeforeWorldMatrix() const;
		/*!
		@breaf 1�^�[���O�̍s��𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̍s��
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetBeforeWorldMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetBeforeLocalMatrix() const;
		/*!
		@breaf 1�^�[���O�̃��[���h�s��𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃��[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetBeforeLocalMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeScale() const;
		/*!
		@breaf 1�^�[���O�̃X�P�[�����O�𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃X�P�[�����O
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeScale() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetBeforeWorldQuaternion() const;
		/*!
		@breaf 1�^�[���O�̃��[���h��]�𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃��[���h��]
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetBeforeWorldQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetBeforeLocalQuaternion() const;
		/*!
		@breaf 1�^�[���O�̃��[�J����]�𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃��[�J����]
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetBeforeLocalQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeWorldPosition() const;
		/*!
		@breaf 1�^�[���O�̃��[���h�ʒu�𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃��[���h�ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeWorldPosition() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeLocalPosition() const;
		/*!
		@breaf 1�^�[���O�̃��[�J���ʒu�𓾂�
		@param �Ȃ�
		@return	1�^�[���O�̃��[�J���ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeLocalPosition() const;
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetWorldMatrix() const;
		/*!
		@breaf ���[���h�s��𓾂�
		@param �Ȃ�
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetWorldMatrix() const;
		//--------------------------------------------------------------------------------------
		//	void SetWorldMatrix(
		//		const Matrix4X4& WorldMatrix	//���[���h�s��
		//	);
		/*!
		@breaf ���[���h�s���ݒ肷��.
		��PriorityMatrix���L���̂Ƃ��̂ݐݒ�ł���B�����̏ꍇ�͗�O
		@param const Matrix4X4& WorldMatrix	���[���h�s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetWorldMatrix(const Matrix4X4& WorldMatrix);
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetScale() const;
		/*!
		@breaf �X�P�[�����O�𓾂�
		@param �Ȃ�
		@return	�X�P�[�����O
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetScale() const;
		//--------------------------------------------------------------------------------------
		//	void SetScale(
		//		const Vector3& Scale	//�X�P�[�����O 
		//	);
		/*!
		@breaf �X�P�[�����O��ݒ肷��.
		@param const Vector3& Scale	�X�P�[�����O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(const Vector3& Scale);
		//--------------------------------------------------------------------------------------
		//	void SetScale(
		//		float x, //X�X�P�[�����O
		//		float y, //Y�X�P�[�����O
		//		float z //Z�X�P�[�����O
		//	);
		/*!
		@breaf �X�P�[�����O��ݒ肷��.
		@param float x X�X�P�[�����O
		@param float y Y�X�P�[�����O
		@param float z Z�X�P�[�����O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetQuaternion() const;
		/*!
		@breaf ���[���h��]�𓾂�
		@param �Ȃ�
		@return	���[���h��]�i�N�I�[�^�j�I���j
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	void SetWorldQuaternion(
		//		const Quaternion& quaternion	//�N�I�[�^�j�I��
		//	);
		/*!
		@breaf ���[���h��]��ݒ肷��
		@param const Quaternion& quaternion	�N�I�[�^�j�I��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetQuaternion(const Quaternion& quaternion);
		//--------------------------------------------------------------------------------------
		//	Vector3 GetRotation() const;
		/*!
		@breaf ���[���h��]�𓾂�
		@param �Ȃ�
		@return	���[���h��]�i�x�N�g���j
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetRotation() const;
		//--------------------------------------------------------------------------------------
		//	void SetRotation(
		//		const Vector3& Rot	//��]�x�N�g��
		//	);
		/*!
		@breaf ���[���h��]��ݒ肷��.
		�������ɍs��ɔ��f����
		@param const Vector3& Rot	��]�x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(const Vector3& Rot);
		//--------------------------------------------------------------------------------------
		//	void SetRotation(
		//		float x, //X��]
		//		float y, //Y��]
		//		float z	//Z��]
		//	);
		/*!
		@breaf ���[���h��]��ݒ肷��.
		�������ɍs��ɔ��f����
		@param float x,	X��]
		@param float y,	Y��]
		@param float z,	Z��]
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetPosition() const;
		/*!
		@breaf ���[���h�ʒu�𓾂�
		@param �Ȃ�
		@return	���[���h�ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetPosition() const;
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		const Vector3& Position	//�ݒ肷��ʒu
		//	);
		/*!
		@breaf ���[���h�ʒu��ݒ肷��.
		�������ɍs��ɔ��f����
		@param const Vector3& Position	�ݒ肷��ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(const Vector3& Position);
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		float x,	//X�ʒu
		//		float y,	//Y�ʒu
		//		float z		//Z�ʒu
		//	);
		/*!
		@breaf ���[���h�ʒu��ݒ肷��.
		�������ɍs��ɔ��f����
		@param float x,	X�ʒu
		@param float y,	Y�ʒu
		@param float z,	Z�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetLocalMatrix() const;
		/*!
		@breaf ���[�J���s��𓾂�
		@param �Ȃ�
		@return	���[�J���s��
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetLocalMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetLocalQuaternion() const;
		/*!
		@breaf ���[�J����]�𓾂�
		@param �Ȃ�
		@return	���[�J����]�i�N�I�[�^�j�I���j
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetLocalQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	Vector3 GetLocalRotation() const;
		/*!
		@breaf ���[�J����]�𓾂�
		@param �Ȃ�
		@return	���[�J����]�i�x�N�g���j
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetLocalRotation() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetLocalPosition() const;
		/*!
		@breaf ���[�J���ʒu�𓾂�
		@param �Ȃ�
		@return	���[�J���ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetLocalPosition() const;

		//--------------------------------------------------------------------------------------
		//	shared_ptr<GameObject> GetParent() const;
		/*!
		@breaf �e�𓾂�
		@param �Ȃ�
		@return	�e�Q�[���I�u�W�F�N�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetParent() const;
		//--------------------------------------------------------------------------------------
		//	void SetParent(
		//		const shared_ptr<GameObject>& Ptr	//�e�|�C���^
		//	);
		/*!
		@breaf �e��ݒ肷��
		@param const shared_ptr<GameObject>& Ptr	�e�|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetParent(const shared_ptr<GameObject>& Ptr);
		//--------------------------------------------------------------------------------------
		//	void ClearParent();
		/*!
		@breaf �e���N���A����BSetParent()��nullptr��n���̂Ɠ���
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearParent();
		//����
		//--------------------------------------------------------------------------------------
		//	void CalcMatrix();
		/*!
		@breaf �s��v�Z������
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CalcMatrix();
		//--------------------------------------------------------------------------------------
		//	void SetToBefore();
		/*!
		@breaf ������O�Ɉڂ�<br />
		�e���̓^�[�����Ƃ�1�O�̃^�[�����̏�������<br />
		���̊֐��͂��̏������s��
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetToBefore();
		//--------------------------------------------------------------------------------------
		//	void LerpBeforeToNow(
		//		float LerpTime	//��Ԏ���
		//	);
		/*!
		@breaf 1�O�̃^�[������̎w�莞�Ԃɏ����Z�b�g����<br />
		�Փ˂����ꍇ�A�I�u�W�F�N�g�͑O��̃^�[���ʒu����Փˈʒu�܂Ŗ߂�K�v������<br />
		���̊֐��͂��̏������s��
		@param float LerpTime	��Ԏ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void LerpBeforeToNow(float LerpTime);
		//--------------------------------------------------------------------------------------
		//	virtual void Update()override;
		/*!
		@breaf �X�V����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update()override;
		//--------------------------------------------------------------------------------------
		//	virtual void Draw()override;
		/*!
		@breaf �`�揈���B�f�t�H���g�͉����s��Ȃ�
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	�p�r: �A�N�V�����n�R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class ActionComponent : public Component{
	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	explicit ActionComponent(
		//		const shared_ptr<GameObject>& GameObjectPtr	//���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		//	);
		/*!
		@breaf �R���X�g�N�^
		@param const shared_ptr<GameObject>& GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit ActionComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ActionComponent();
		void SetRun(bool b);
		void SetArrived(bool b);
		//NowTime�����Z���āATotalTime�Ɣ�r����
		bool AdditionalNowTime();
	public:
		//�A�N�Z�T
		bool IsRun()const;
		bool GetRun()const;

		bool IsArrived()const;
		bool GetArrived()const;

		virtual float GetTotalTime() const;
		virtual void SetTotalTime(float f);

		virtual float GetNowTime() const;
		virtual void SetNowTime(float f);


		//����i�������z�֐��j
		virtual void Run() = 0;
		virtual void Stop();
		virtual void ReStart();
		//Draw�͊�{�I�ɍs��Ȃ�
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	�p�r: �A�N�V�����̃C���^�[�o��
	//--------------------------------------------------------------------------------------
	class ActionInterval : public  ActionComponent{
	public:
		//�\�z�Ɣj��
		explicit ActionInterval(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ActionInterval();
		void SetParams(float TotalTime);

		virtual void Run()override;
		virtual void Update()override;
	};



	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public ActionComponent ;
	//	�p�r: �g��k���A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	class ScaleComponent : public ActionComponent{
	protected:
		//�\�z�Ɣj��
		explicit ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleComponent();
	public:

		//�A�N�Z�T
		Lerp::rate GetRate() const;
		void SetRate(const Lerp::rate r);

		const Vector3& GetStartScale() const;
		void SetStartScale(const Vector3& StartScale);
		void SetStartScale(float x, float y, float z);

		const Vector3& GetTargetScale() const;
		void SetTargetScale(const Vector3& TargetPosition);
		void SetTargetScale(float x, float y, float z);

		//���݃X�P�[�����v�Z���ĕԂ�
		//�^�C���̍X�V�͍s��Ȃ��̂�
		//Update��ɌĂԂׂ�
		Vector3 GetNowScale() const;

		//����
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	�p�r: �ړI�̊g�嗦�Ɋg��k��
	//--------------------------------------------------------------------------------------
	class ScaleTo : public  ScaleComponent{
	public:
		//�\�z�Ɣj��
		explicit ScaleTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleTo();
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	�p�r: �ړI�̑��΃X�P�[���ɂɊg��k��
	//--------------------------------------------------------------------------------------
	class ScaleBy : public  ScaleComponent{
	public:
		//�\�z�Ɣj��
		explicit ScaleBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleBy();
		//�A�N�Z�T
		const Vector3& GetLocalScale() const;
		void SetLocalScale(const Vector3& LocalScale);
		void SetLocalScale(float x, float y, float z);
		void SetParams(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		//����
		virtual void Run()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	�p�r: ��]�A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	class RotateComponent : public ActionComponent{
	protected:
		//�\�z�Ɣj��
		explicit RotateComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateComponent();
	public:

		//�A�N�Z�T
		const Quaternion& GetStartQuaternion() const;
		void SetStartQuaternion(const Quaternion& StartQuaternion);

		Vector3 GetStartRotate() const;
		void SetStartRotate(const Vector3& StartRotate);
		void SetStartRotate(float x, float y, float z);

		const Quaternion& GetTargetQuaternion() const;
		void SetTargetQuaternion(const Quaternion& TargetQuaternion);

		Vector3 GetTargetRotate() const;
		void SetTargetRotate(const Vector3& TargetRotate);
		void SetTargetRotate(float x, float y, float z);

		//���݉�]���v�Z���ĕԂ�
		//�^�C���̍X�V�͍s��Ȃ��̂�
		//Update��ɌĂԂׂ�
		Vector3 GetNowRotate() const;
		Quaternion GetNowQuaternion() const;

		//����
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class RotateTo : public RotateComponent;
	//	�p�r: �ړI�̊p�x�ɉ�]
	//--------------------------------------------------------------------------------------
	class RotateTo : public  RotateComponent{
	public:
		//�\�z�Ɣj��
		explicit RotateTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateTo();
		void SetParams(float TotalTime, const Vector3& Target);
		void SetParams(float TotalTime, const Quaternion& TargetQuaternion);
	};

	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	�p�r: �ړI�̑��Ίp�x�ɉ�]
	//--------------------------------------------------------------------------------------
	class RotateBy : public  RotateComponent{
	public:
		//�\�z�Ɣj��
		explicit RotateBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateBy();
		//�A�N�Z�T
		const Quaternion& GetLocalQuaternion() const;
		void SetLocalQuaternion(const Quaternion& LocalQuaternion);
		Vector3 GetLocalRotate() const;
		void SetLocalRotate(const Vector3& LocalRotate);
		void SetLocalRotate(float x, float y, float z);
		void SetParams(float TotalTime, const Vector3& LocalRotate);
		void SetParams(float TotalTime, const Quaternion& LocalQuaternion);
		//����
		virtual void Run()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	�p�r: �ړ��A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	class MoveComponent : public ActionComponent{
		void CalcVelocity();
		Vector3 CalcVelocitySub(float NowTime);
	protected:
		//�\�z�Ɣj��
		explicit MoveComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveComponent();
	public:

		//�A�N�Z�T
		Lerp::rate GetRateX() const;
		Lerp::rate GetRateY() const;
		Lerp::rate GetRateZ() const;
		void SetRateX(const Lerp::rate r);
		void SetRateY(const Lerp::rate r);
		void SetRateZ(const Lerp::rate r);
		void SetRateAll(const Lerp::rate r);

		const Vector3& GetStartPosition() const;
		void SetStartPosition(const Vector3& StartPosition);
		void SetStartPosition(float x, float y, float z);

		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& TargetPosition);
		void SetTargetPosition(float x, float y, float z);
		//���݂̑��x
		Vector3 GetVelocity();


		//���݈ʒu���v�Z���ĕԂ�
		//�^�C���̍X�V�͍s��Ȃ��̂�
		//Update��ɌĂԂׂ�
		Vector3 GetNowPosition() const;

		//����
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	�p�r: �ړI�̈ʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	class MoveTo : public  MoveComponent{
	public:
		//�\�z�Ɣj��
		explicit MoveTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveTo();
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	�p�r: �ړI�̑��Έʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	class MoveBy : public  MoveComponent{
	public:
		//�\�z�Ɣj��
		explicit MoveBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveBy();
		//�A�N�Z�T
		const Vector3& GetLocalVector() const;
		void SetLocalVector(const Vector3& LocalVector);
		void SetLocalVector(float x, float y, float z);

		void SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		void SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//����
		virtual void Run()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	�p�r: �����ʒu�R���g���[��
	//--------------------------------------------------------------------------------------
	class Action : public Component{
		void RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		bool UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
	public:
		//�\�z�Ɣj��
		explicit Action(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Action();

		//�A�N�Z�T
		bool IsLooped()const;
		bool GetLooped()const;
		void SetLooped(bool b);

		bool IsArrived()const;
		bool GetArrived()const;

		//���݂̑��x
		//Move�R���|�[�l���g���Ȃ��ꍇ��Vector3(0,0,0)��Ԃ�
		Vector3 GetVelocity();


		size_t GetScaleActiveIndex() const;
		size_t GetRotateActiveIndex() const;
		size_t GetMoveActiveIndex() const;

		shared_ptr<ScaleTo> AddScaleTo(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<ScaleBy> AddScaleBy(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<ActionInterval> AddScaleInterval(float TotalTime);

		shared_ptr<ActionComponent> GetScaleComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GeScale(size_t TargetIndex){
			auto Ptr = GetScaleComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					L"if(!Ptr)",
					L"Action::GetScale()"
					);
			}
			return Ptr2;
		}


		shared_ptr<RotateTo> AddRotateTo(float TotalTime, const Vector3& Target);
		shared_ptr<RotateTo> AddRotateTo(float TotalTime, const Quaternion& TargetQuaternion);
		shared_ptr<RotateBy> AddRotateBy(float TotalTime, const Vector3& LocalRotate);
		shared_ptr<RotateBy> AddRotateBy(float TotalTime, const Quaternion& LocalQuaternion);
		shared_ptr<ActionInterval> AddRotateInterval(float TotalTime);

		shared_ptr<ActionComponent> GetRotateComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GeRotate(size_t TargetIndex){
			auto Ptr = GetRotateComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					L"if(!Ptr)",
					L"Action::GetRotate()"
					);
			}
			return Ptr2;
		}


		shared_ptr<MoveTo> AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<MoveTo> AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		shared_ptr<MoveBy> AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<MoveBy> AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);

		shared_ptr<ActionInterval> AddMoveInterval(float TotalTime);

		shared_ptr<ActionComponent> GetMoveComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GetMove(size_t TargetIndex){
			auto Ptr = GetMoveComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"���̃R���|�[�l���g��T�^�ɃL���X�g�ł��܂���",
					L"if(!Ptr)",
					L"Action::GetMove()"
					);
			}
			return Ptr2;
		}

		//���ׂẴA�N�V�������N���A����
		void AllActionClear();

		//����
		virtual void Run();
		virtual void Stop();
		virtual void ReStart();

		virtual void Update()override;
		virtual void Draw()override{}

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	�p�r: �J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class Camera : public Object, public ShapeInterface{
	public:
		//�\�z�Ɣj��
		explicit Camera();
		virtual ~Camera();
		//�A�N�Z�T
		const Vector3& GetEye() const;
		void SetEye(const Vector3& Eye);
		void SetEye(float x, float y, float z);

		const Vector3& GetAt() const;
		void SetAt(const Vector3& At);
		void SetAt(float x, float y, float z);

		const Vector3& GetUp() const;
		void SetUp(const Vector3& Up);
		void SetUp(float x, float y, float z);

		bool IsPers()const;
		bool GetPers()const;
		void SetPers(bool b);

		float GetFovY() const;
		void SetFovY(float f);

		float GetAspect() const;
		void SetAspect(float f);

		float GetWidth() const;
		void SetWidth(float f);

		float GetHeight() const;
		void SetHeight(float f);

		float GetNear() const;
		void SetNear(float f);

		float GetFar() const;
		void SetFar(float f);

		shared_ptr<GameObject> GetCameraObject() const;
		void SetCameraObject(const shared_ptr<GameObject>& Obj);
		void ClearCameraObject(const shared_ptr<GameObject>& Obj);


		void SetViewPort(const D3D11_VIEWPORT& v);

		const Matrix4X4& GetViewMatrix() const;
		const Matrix4X4& GetProjMatrix() const;
		//����
		virtual void Update()override;
		virtual void Update2(){}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	�p�r: LookAt�J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class LookAtCamera : public Camera{
	public:
		//�\�z�Ɣj��
		explicit LookAtCamera();
		virtual ~LookAtCamera();
		//�A�N�Z�T
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);

		//����
		virtual void Update() override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public ShapeInterface ;
	//	�p�r: ���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class Light : public Object, public ShapeInterface{
	public:
		explicit Light();
		Light(
			const Vector3& Directional,
			const Color4& DiffuseColor,
			const Color4& SpecularColor
			);

		virtual ~Light();
		//�A�N�Z�T
		const Vector3& GetDirectional() const;
		void SetDirectional(const Vector3& Directional);
		void SetDirectional(float x, float y, float z);
		void SetPositionToDirectional(const Vector3& Position);
		void SetPositionToDirectional(float x, float y, float z);

		const Color4& GetDiffuseColor()const;
		void SetDiffuseColor(const Color4& col);
		void SetDiffuseColor(float r, float g, float b, float a);

		const Color4& GetSpecularColor() const;
		void SetSpecularColor(const Color4& col);
		void SetSpecularColor(float r, float g, float b, float a);

		virtual void Update()override{}
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public ShapeInterface ;
	//	�p�r: �}���`���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class MultiLight : public Object, public ShapeInterface{
	public:
		//�\�z�Ɣj��
		explicit MultiLight();
		virtual ~MultiLight();
		//�A�N�Z�T
		//Light�̐�
		size_t GetLightCount() const;
		//Light�̎擾
		shared_ptr<Light> GetLight(size_t Index)const;
		//Light�̃Z�b�g
		void SetLight(size_t Index, shared_ptr<Light>& Ptr);
		//�z��̎Q��
		vector< shared_ptr<Light> >& GetLightVec();
		//Light�̒ǉ�
		shared_ptr<Light> AddLight();
		//Light�̍폜
		void RemoveLight(size_t Index);
		//�f�t�H���g�̃��C�e�B���O�̐ݒ�
		void SetDefaultLighting();

		//����
		virtual void Update()override;
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public ShapeInterface;
	//	�p�r: �r���[�N���X�i�r���[�|�[�g�ƃ��C�g�A�J�������Ǘ��A�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class View : public Object, public ShapeInterface{
		//�e���v���[�g����Ă΂��T�u�֐�
		void ResetParamatersSub(const D3D11_VIEWPORT& vp,const Color4& Col, 
			const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr);
	public:
		//�\�z�Ɣj��
		View();
		virtual ~View();
		//�A�N�Z�T
		const D3D11_VIEWPORT& GetViewPort()const;
		const D3D11_VIEWPORT* GetViewPortPtr()const;
		void SetViewPort(const D3D11_VIEWPORT& v);
		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& c);
		shared_ptr<Camera> GetCamera() const;
		void SetCamera(shared_ptr<Camera>& Ptr);

		shared_ptr<MultiLight> GetMultiLight() const;
		void SetMultiLight(shared_ptr<MultiLight>& Lights);

		shared_ptr<MultiLight> SetDefaultLighting();


		//����
		shared_ptr<Light> AddLight();

		template<typename CameraType,typename LightType>
		void ResetParamaters(const Rect2D<float>& ViewPortSize, Color4& Col, size_t LightCount, float MinDepth = 0.0f, float MaxDepth = 1.0f){
			D3D11_VIEWPORT vp;
			ZeroMemory(&vp, sizeof(vp));
			vp.MinDepth = MinDepth;
			vp.MaxDepth = MaxDepth;
			vp.TopLeftX = ViewPortSize.left;
			vp.TopLeftY = ViewPortSize.top;
			vp.Width = ViewPortSize.Width();
			vp.Height = ViewPortSize.Height();
			//�J�����������ւ���
			auto CameraPtr = Object::CreateObject<CameraType>();
			//���C�g�������ւ���
			auto MultiLightPtr = Object::CreateObject<LightType>();
			for (size_t i = 0; i < LightCount; i++){
				MultiLightPtr->AddLight();
			}
			//�T�u�֐����g���Đݒ�
			ResetParamatersSub(vp, Col, 
				dynamic_pointer_cast<Camera>(CameraPtr), 
				dynamic_pointer_cast<MultiLight>(MultiLightPtr));
		}
		//����
		virtual void Update()override;
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class MultiView: public Component;
	//	�p�r: �}���`�r���[�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class MultiView : public Component{
	public:
		//�\�z�Ɣj��
		explicit MultiView(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MultiView();
		//�A�N�Z�T
		size_t GetViewCount() const;
		shared_ptr<View> GetView(size_t Index) const;
		//ViewVec�𒼐ڑ��삷�鎞�p
		vector< shared_ptr<View> >& GetViewVec();
		//����
		shared_ptr<View> AddView();
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PlayMusic: public Component;
	//	�p�r: �~���[�W�b�N�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PlayMusic : public Component{
	public:
		//�\�z�Ɣj��
		PlayMusic(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey);
		virtual ~PlayMusic();
		//�A�N�Z�T
		shared_ptr<AudioResource> GetAudioResource() const;
		//����
		void Start(size_t LoopCount = 0, float Volume = 1.0f);
		void Start(const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop();
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SoundEffect: public Component;
	//	�p�r: �T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class SoundEffect : public Component{
	public:
		//�\�z�Ɣj��
		SoundEffect(const shared_ptr<GameObject>& GameObjectPtr,const wstring& ResKey);
		virtual ~SoundEffect();
		//�A�N�Z�T
		shared_ptr<AudioResource> GetAudioResource() const;
		//����
		void Start(size_t LoopCount = 0,float Volume = 1.0f);
		void Start(const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop();
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct SoundItem{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	�p�r: �}���`�T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class MultiSoundEffect : public Component{
	public:
		//�\�z�Ɣj��
		MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MultiSoundEffect();
		//�A�N�Z�T
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey,bool ExceptionActive = true) const;
		//����
		shared_ptr<AudioResource> AddAudioResource(const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//endof  basedx11
