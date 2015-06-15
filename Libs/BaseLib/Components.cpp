#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Component::Impl{
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//update���邩�ǂ���
		bool m_Update2Active{ true };	//update2���邩�ǂ���
		bool m_DrawActive{ true };		//Draw���邩�ǂ���

		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
			{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	�p�r: �R���|�[�l���g���N���X
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		Object(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component(){}
	//�A�N�Z�T
	shared_ptr<GameObject> Component::GetGameObject() const{
		if (pImpl->m_GameObject.expired()){
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (pImpl->m_GameObject.expired())",
				L"Component::GetGameObject()"
				);
		}
		else{
			return pImpl->m_GameObject.lock();
		}
	}
	bool Component::IsGameObjectActive() const{
		return !pImpl->m_GameObject.expired();
	}
	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr){
		pImpl->m_GameObject = GameObjectPtr;
	}

	bool Component::IsUpdateActive() const{
		return pImpl->m_UpdateActive;
	}
	bool Component::GetUpdateActive() const{
		return pImpl->m_UpdateActive;
	}
	void Component::SetUpdateActive(bool b){
		pImpl->m_UpdateActive = b;
	}

	bool Component::IsUpdate2Active() const{
		return pImpl->m_Update2Active;
	}
	bool Component::GetUpdate2Active() const{
		return pImpl->m_Update2Active;
	}
	void Component::SetUpdate2Active(bool b){
		pImpl->m_Update2Active = b;
	}


	bool Component::IsDrawActive() const{
		return pImpl->m_DrawActive;
	}
	bool Component::GetDrawActive() const{
		return pImpl->m_DrawActive;
	}
	void Component::SetDrawActive(bool b){
		pImpl->m_DrawActive = b;
	}


	//--------------------------------------------------------------------------------------
	//	struct Transform::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Transform::Impl{
		bool m_Init{ false };	//�������ς݂��ǂ����i1��ڂ�Update�ŁABefore�ɒl������j
		bool m_PriorityMatrix{ false };
		bool m_PriorityPosition{ false };	//�s��v�Z���ʒu�D��ɂ��邩�ǂ���
		weak_ptr<GameObject> m_Parent;	//�e�Q�[���I�u�W�F�N�g

		Matrix4X4 m_BeforeLocalMatrix{ Matrix4X4() };			//�A�b�v�f�[�g���O�̃��[�J���s��
		Matrix4X4 m_BeforeWorldMatrix{ Matrix4X4() };			//�A�b�v�f�[�g���O�̃��[���h�s��
		Vector3 m_BeforeScale{ 1.0f, 1.0f, 1.0f };			//�X�P�[�����O�̓��[�J�����[���h����	
		Quaternion m_BeforeLocalQuaternion{ Quaternion() };		//��O�̃��[�J���N�I�[�^�j�I��
		Quaternion m_BeforeWorldQuaternion{ Quaternion() };		//��O�̃��[���h�N�I�[�^�j�I��
		Vector3 m_BeforeLocalPosition{ 0, 0, 0 };	//��O�̃��[�J���ʒu
		Vector3 m_BeforeWorldPosition{ 0, 0, 0 };	//��O�̃��[���h�ʒu

		Matrix4X4 m_LocalMatrix{ Matrix4X4() };
		Matrix4X4 m_WorldMatrix{ Matrix4X4() };
		Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };		//�X�P�[�����O�̓��[�J�����[���h����
		Quaternion m_LocalQuaternion{ Quaternion() };
		Quaternion m_Quaternion{ Quaternion() };
		Vector3 m_LocalPosition{ 0, 0, 0 };
		Vector3 m_Position{ 0, 0, 0 };
		Impl() 
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	�p�r: �ω�
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform(){}

	//�A�N�Z�T
	bool Transform::IsPriorityMatrix()const{ return pImpl->m_PriorityMatrix; }
	bool Transform::GetPriorityMatrix()const{ return pImpl->m_PriorityMatrix; }
	void Transform::SetPriorityMatrix(bool b){ pImpl->m_PriorityMatrix = b; }

	bool Transform::IsPriorityPosition()const{
		return pImpl->m_PriorityPosition;
	}
	bool Transform::GetPriorityPosition()const{
		return pImpl->m_PriorityPosition;
	}
	void Transform::SetPriorityPosition(bool b){
		pImpl->m_PriorityPosition = b;
	}


	//BeforeGetter
	const Matrix4X4& Transform::GetBeforeWorldMatrix() const{
		return pImpl->m_BeforeWorldMatrix;
	}
	const Matrix4X4& Transform::GetBeforeLocalMatrix() const{
		return pImpl->m_BeforeLocalMatrix;
	}
	const Vector3& Transform::GetBeforeScale() const{
		return pImpl->m_BeforeScale;
	}
	const Quaternion& Transform::GetBeforeWorldQuaternion() const{
		return pImpl->m_BeforeWorldQuaternion;
	}
	const Quaternion& Transform::GetBeforeLocalQuaternion() const{
		return pImpl->m_BeforeLocalQuaternion;
	}
	const Vector3& Transform::GetBeforeWorldPosition() const{
		return pImpl->m_BeforeWorldPosition;
	}
	const Vector3& Transform::GetBeforeLocalPosition() const{
		return pImpl->m_BeforeLocalPosition;
	}


	//Getter&Setter
	const Matrix4X4& Transform::GetWorldMatrix()const {
		return pImpl->m_WorldMatrix;
	}
	void Transform::SetWorldMatrix(const Matrix4X4& WorldMatrix){
		if (!pImpl->m_PriorityMatrix){
			throw BaseException(
				L"PriorityMatrix�ł͂Ȃ��̂ŁA�s��͒��ڕύX�ł��܂���",
				L"if (!pImpl->m_PriorityMatrix)",
				L"Transform::Impl::SetMatrix()"
				);
		}
		//m_PriorityMatrix�̏ꍇ��m_WorldMatrix�����g��Ȃ�
		pImpl->m_WorldMatrix = WorldMatrix;
	}

	const Vector3& Transform::GetScale() const{
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const Vector3& Scale){
		//Scale�̓��[�J�����[���h����
		pImpl->m_Scale = Scale;
		if (!pImpl->m_PriorityMatrix){
			//WorldSetter�͂����Ɍv�Z����
			CalcMatrix();
		}
	}
	void Transform::SetScale(float x, float y, float z){
		SetScale(Vector3(x, y, z));
	}

	const Quaternion& Transform::GetQuaternion() const{
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const Quaternion& quaternion){
		if (!pImpl->m_PriorityMatrix){
			//���͂̓��[���h���W�ł���
			if (GetParent()){
				auto Ptr = GetParent()->GetComponent<Transform>();
				Matrix4X4 Parmatrix = Ptr->GetWorldMatrix();
				//�X�P�[�����O��1.0�ɂ���
				Parmatrix.ScaleIdentity();
				Vector4 v;
				Parmatrix.Inverse(&v);
				Quaternion ParQt = Parmatrix.QtInMatrix();
				pImpl->m_LocalQuaternion = quaternion * ParQt;
			}
			else{
				pImpl->m_LocalQuaternion = quaternion;
			}
			//WorldSetter�͂����Ɍv�Z����
			CalcMatrix();
		}
		else{
			pImpl->m_Quaternion = quaternion;
		}
	}
	Vector3 Transform::GetRotation() const{
		return pImpl->m_Quaternion.GetRotation();
	}

	void Transform::SetRotation(const Vector3& Rot){
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetQuaternion(Qt);
	}
	void Transform::SetRotation(float x, float y, float z){
		SetRotation(Vector3(x, y, z));
	}

	const Vector3& Transform::GetPosition() const{
		return pImpl->m_Position;
	}

	void Transform::SetPosition(const Vector3& Position){
		if (!pImpl->m_PriorityMatrix){
			//���͂̓��[���h���W�ł���
			if (auto ParPtr = GetParent()){
				Vector3 Temp = Position - pImpl->m_Position;
				pImpl->m_LocalPosition += Temp;
			}
			else{
				pImpl->m_LocalPosition = Position;
			}
			//Setter�͂����Ɍv�Z����
			CalcMatrix();
		}
		else{
			pImpl->m_Position = Position;
		}
	}
	void Transform::SetPosition(float x, float y, float z){
		SetPosition(Vector3(x, y, z));
	}

	const Matrix4X4& Transform::GetLocalMatrix() const{
		return pImpl->m_LocalMatrix;
	}

	const Quaternion& Transform::GetLocalQuaternion() const{
		return pImpl->m_LocalQuaternion;
	}
	void Transform::SetLocalQuaternion(const Quaternion& quaternion, bool CalcFlg){
		pImpl->m_LocalQuaternion = quaternion;
		if (!pImpl->m_PriorityMatrix){
			//Setter�͂����Ɍv�Z����
			if (CalcFlg){
				CalcMatrix();
			}
		}
		//PriorityMatrix���͉������Ȃ�
	}
	Vector3 Transform::GetLocalRotation() const{
		return pImpl->m_LocalQuaternion.GetRotation();
	}

	void Transform::SetLocalRotation(const Vector3& Rot, bool CalcFlg){
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetLocalQuaternion(Qt, CalcFlg);
	}
	void Transform::SetLocalRotation(float x, float y, float z, bool CalcFlg){
		SetLocalRotation(Vector3(x, y, z), CalcFlg);
	}


	const Vector3& Transform::GetLocalPosition() const{
		return pImpl->m_LocalPosition;
	}

	void Transform::SetLocalPosition(const Vector3& Position, bool CalcFlg){
		pImpl->m_LocalPosition = Position;
		if (!pImpl->m_PriorityMatrix){
			//Setter�͂����Ɍv�Z����
			if (CalcFlg){
				CalcMatrix();
			}
		}
		//PriorityMatrix���͉������Ȃ�
	}

	void Transform::SetLocalPosition(float x, float y, float z, bool CalcFlg){
		SetLocalPosition(Vector3(x, y, z), CalcFlg);
	}


	shared_ptr<GameObject> Transform::GetParent() const{
		if (!pImpl->m_Parent.expired()){
			return pImpl->m_Parent.lock();
		}
		return nullptr;
	}
	void Transform::SetParent(const shared_ptr<GameObject>& Ptr){
		if (Ptr){
			if (!pImpl->m_Parent.expired()){
				ClearParent();
			}
			pImpl->m_Parent = Ptr;
			auto ParTransPtr = Ptr->GetComponent<Transform>();
			Matrix4X4 ParMatrix = ParTransPtr->GetWorldMatrix();
			ParMatrix.ScaleIdentity();
			Vector4 Vec;
			ParMatrix.Inverse(&Vec);
			auto Matrix = GetLocalMatrix() * ParMatrix;
			//�ݒ莞�͌v�Z���Ȃ�
			SetLocalQuaternion(Matrix.QtInMatrix(), false);
			SetLocalPosition(Matrix.PosInMatrix(), false);
			CalcMatrix();
		}
		else{
			ClearParent();
		}
	}
	void Transform::ClearParent(){
		pImpl->m_Parent.reset();
		//�ݒ莞�͌v�Z���Ȃ�
		SetLocalQuaternion(GetQuaternion(), false);
		SetLocalPosition(GetPosition(), false);
		CalcMatrix();
	}


	//�v�Z
	void Transform::CalcMatrix(){
		if (IsGameObjectActive()){
			if (!pImpl->m_PriorityMatrix){
				if (pImpl->m_PriorityPosition){
					pImpl->m_LocalMatrix.STRTransformation(
						pImpl->m_Scale,
						pImpl->m_LocalPosition,
						pImpl->m_LocalQuaternion
						);
				}
				else{
					pImpl->m_LocalMatrix.DefTransformation(
						pImpl->m_Scale,
						pImpl->m_LocalQuaternion,
						pImpl->m_LocalPosition
						);
				}
				//�e������ꍇ�́A�e�l�͐e�̔z���ɂȂ�B
				if (GetParent()){
					auto PtrTrans = GetParent()->GetComponent<Transform>();
					Matrix4X4 Parmatrix = PtrTrans->GetWorldMatrix();
					//�X�P�[�����O��1.0�ɂ���
					Parmatrix.ScaleIdentity();
					pImpl->m_WorldMatrix = pImpl->m_LocalMatrix * Parmatrix;
					pImpl->m_Quaternion = pImpl->m_WorldMatrix.QtInMatrix();
					pImpl->m_Position = pImpl->m_WorldMatrix.PosInMatrix();
				}
				else{
					//�e���Ȃ�
					pImpl->m_WorldMatrix = pImpl->m_LocalMatrix;
					pImpl->m_Quaternion = pImpl->m_LocalQuaternion;
					pImpl->m_Position = pImpl->m_LocalPosition;
				}
			}
		}
	}

	void Transform::SetToBefore(){
		if (GetGameObject()){
			auto PtrRidid = GetGameObject()->GetComponent<Rigidbody>(false);
			if (PtrRidid && !pImpl->m_Init){
				float Len = Vector3EX::Length(PtrRidid->GetVelocity());
				float PosLen = Vector3EX::Length(pImpl->m_Position - pImpl->m_BeforeWorldPosition);
				if (PtrRidid->IsVelocityZeroCommand() && Len > 0 && PosLen <= 0){
					PtrRidid->SetVelocity(0, 0, 0);
				}
			}
		}
		pImpl->m_BeforeLocalMatrix = pImpl->m_LocalMatrix;
		pImpl->m_BeforeWorldMatrix = pImpl->m_WorldMatrix;
		pImpl->m_BeforeScale = pImpl->m_Scale;
		pImpl->m_BeforeLocalQuaternion = pImpl->m_LocalQuaternion;
		pImpl->m_BeforeWorldQuaternion = pImpl->m_Quaternion;
		pImpl->m_BeforeLocalPosition = pImpl->m_LocalPosition;
		pImpl->m_BeforeWorldPosition = pImpl->m_Position;
	}

	void Transform::LerpBeforeToNow(float LerpTime){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		pImpl->m_Scale = Lerp::CalculateLerp<Vector3>(
			pImpl->m_BeforeScale, pImpl->m_Scale, 0, ElapsedTime, LerpTime, Lerp::Linear
			);

		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_BeforeLocalQuaternion,
			pImpl->m_LocalQuaternion,
			LerpTime / ElapsedTime
			);
		Qt.Normalize();
		pImpl->m_LocalQuaternion = Qt;

		pImpl->m_LocalPosition = Lerp::CalculateLerp<Vector3>(
			pImpl->m_BeforeLocalPosition, pImpl->m_LocalPosition, 0, ElapsedTime, LerpTime, Lerp::Linear
			);
		if (!pImpl->m_PriorityMatrix){
			CalcMatrix();
		}
	}
	//����
	void Transform::Update(){
		CalcMatrix();
		if (!pImpl->m_Init){
			SetToBefore();
			pImpl->m_Init = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ActionComponent::Impl{
		bool m_Run;			//�ړ������ǂ���
		bool m_Arrived;	//�����������ǂ���
		float m_TotalTime;	//�ړ��ɂ����鎞��
		float m_NowTime;	//���݂̎���
		Impl() :
			m_Run(false),
			m_Arrived(false),
			m_TotalTime(0),
			m_NowTime(0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	�p�r: �A�N�V�����n�R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActionComponent::ActionComponent(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	ActionComponent::~ActionComponent(){}

	//�A�N�Z�T
	void ActionComponent::SetRun(bool b){
		pImpl->m_Run = b;
	}
	void  ActionComponent::SetArrived(bool b){
		pImpl->m_Arrived = b;
	}


	bool ActionComponent::IsRun()const{
		return pImpl->m_Run;
	}
	bool ActionComponent::GetRun()const{
		return pImpl->m_Run;
	}

	bool ActionComponent::IsArrived()const{
		return pImpl->m_Arrived;
	}
	bool ActionComponent::GetArrived()const{
		return pImpl->m_Arrived;
	}

	float ActionComponent::GetTotalTime() const{
		return pImpl->m_TotalTime;
	}
	void ActionComponent::SetTotalTime(float f){
		pImpl->m_TotalTime = f;
	}

	float ActionComponent::GetNowTime() const{
		return pImpl->m_NowTime;
	}
	void ActionComponent::SetNowTime(float f){
		pImpl->m_NowTime = f;
	}

	//NowTime�����Z���āATotalTime�Ɣ�r����
	bool ActionComponent::AdditionalNowTime(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive() && pImpl->m_Run){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && pImpl->m_TotalTime > 0){
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime){
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//��������
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}


	void ActionComponent::Stop(){
		pImpl->m_Run = false;
	}
	void ActionComponent::ReStart(){
		if (!pImpl->m_Arrived){
			//�������ĂȂ���Γ��삳����
			pImpl->m_Run = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	�p�r: �A�N�V�����̃C���^�[�o��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActionInterval::ActionInterval(const shared_ptr<GameObject>& GameObjectPtr):
		ActionComponent(GameObjectPtr)
	{}
	ActionInterval::~ActionInterval(){}

	void ActionInterval::SetParams(float TotalTime){
		SetTotalTime(TotalTime);
	}

	void ActionInterval::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}
	void ActionInterval::Update(){
		if (IsGameObjectActive() && GetRun()){
			if (!AdditionalNowTime()){
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct ScaleComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ScaleComponent::Impl{
		Vector3 m_StartScale;	//�J�n�{��
		Vector3 m_TargetScale;	//�ړI�{��
		Lerp::rate m_Rate;	//��ԕ��@
		Impl() :
			m_StartScale(1.0f, 1.0f, 1.0f),
			m_TargetScale(1.0f, 1.0f, 1.0f),
			m_Rate(Lerp::Linear)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public Component ;
	//	�p�r: �g��k���A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ScaleComponent::ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ScaleComponent::~ScaleComponent(){}


	//�A�N�Z�T
	Lerp::rate ScaleComponent::GetRate() const{ return pImpl->m_Rate; }
	void ScaleComponent::SetRate(const Lerp::rate r){ pImpl->m_Rate = r; }

	const Vector3& ScaleComponent::GetStartScale() const{ return pImpl->m_StartScale; }
	void ScaleComponent::SetStartScale(const Vector3& StartPosition){ pImpl->m_StartScale = StartPosition; }
	void ScaleComponent::SetStartScale(float x, float y, float z){ pImpl->m_StartScale = Vector3(x, y, z); }

	const Vector3& ScaleComponent::GetTargetScale() const{ return pImpl->m_TargetScale; }
	void ScaleComponent::SetTargetScale(const Vector3& TargetScale){ pImpl->m_TargetScale = TargetScale; }
	void ScaleComponent::SetTargetScale(float x, float y, float z){ pImpl->m_TargetScale = Vector3(x, y, z); }

	//����
	void ScaleComponent::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_StartScale = TransPtr->GetScale();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	void ScaleComponent::Update(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Vector3 Scale = Lerp::CalculateLerp<Vector3>(
					pImpl->m_StartScale,
					pImpl->m_TargetScale,
					0,
					GetTotalTime(),
					GetNowTime(),
					pImpl->m_Rate
					);
				TransPtr->SetScale(Scale);
			}
			else{
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//���݃X�P�[�����v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	Vector3 ScaleComponent::GetNowScale() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetScale;
		}
		Vector3 Scale = Lerp::CalculateLerp(
			pImpl->m_StartScale,
			pImpl->m_TargetScale,
			0,
			GetTotalTime(),
			GetNowTime(),
			pImpl->m_Rate
			);
		return Scale;
	}



	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	�p�r: �ړI�̊g�嗦�Ɋg��k��
	//--------------------------------------------------------------------------------------
	ScaleTo::ScaleTo(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr){}
	ScaleTo::~ScaleTo(){}
	void ScaleTo::SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate){
		SetTargetScale(Target);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct ScaleBy::Impl{
		Vector3 m_LocalScale;	//���΃X�P�[��
	public:
		Impl() :
			m_LocalScale(1.0f, 1.0f, 1.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	�p�r: �ړI�̑��΃X�P�[���ɂɊg��k��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ScaleBy::ScaleBy(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr), pImpl(new Impl()){}
	ScaleBy::~ScaleBy(){}


	//�A�N�Z�T
	const Vector3& ScaleBy::GetLocalScale() const{ return pImpl->m_LocalScale; }
	void ScaleBy::SetLocalScale(const Vector3& LocalScale){ pImpl->m_LocalScale = LocalScale; }
	void ScaleBy::SetLocalScale(float x, float y, float z){ pImpl->m_LocalScale = Vector3(x, y, z); }

	void ScaleBy::SetParams(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate){
		SetLocalScale(LocalScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}


	//����
	void ScaleBy::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Vector3 Target = TransPtr->GetScale();
				Target.x *= pImpl->m_LocalScale.x;
				Target.y *= pImpl->m_LocalScale.y;
				Target.z *= pImpl->m_LocalScale.z;
				SetTargetScale(Target);
				ScaleComponent::Run();
			}
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct RotateComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct RotateComponent::Impl{
		Quaternion m_StartQuaternion;	//�J�n��]
		Quaternion m_TargetQuaternion;	//�I����]
	public:
		Impl() :
			m_StartQuaternion(),
			m_TargetQuaternion()
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	�p�r: ��]�A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateComponent::RotateComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{
	}
	RotateComponent::~RotateComponent(){}


	//�A�N�Z�T
	const Quaternion& RotateComponent::GetStartQuaternion() const{ return pImpl->m_StartQuaternion; }
	void RotateComponent::SetStartQuaternion(const Quaternion& StartQuaternion){
		pImpl->m_StartQuaternion = StartQuaternion;
		pImpl->m_StartQuaternion.Normalize();
	}

	Vector3 RotateComponent::GetStartRotate() const{
		return pImpl->m_StartQuaternion.GetRotation();
	}
	void RotateComponent::SetStartRotate(const Vector3& StartRotate){
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(StartRotate);
		pImpl->m_StartQuaternion.Normalize();
	}
	void RotateComponent::SetStartRotate(float x, float y, float z){
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_StartQuaternion.Normalize();
	}

	const Quaternion& RotateComponent::GetTargetQuaternion() const{
		return pImpl->m_TargetQuaternion;
	}
	void RotateComponent::SetTargetQuaternion(const Quaternion& TargetQuaternion){
		pImpl->m_TargetQuaternion = TargetQuaternion;
		pImpl->m_TargetQuaternion.Normalize();
	}
	Vector3 RotateComponent::GetTargetRotate() const{
		return pImpl->m_TargetQuaternion.GetRotation();
	}
	void RotateComponent::SetTargetRotate(const Vector3& TargetRotate){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(TargetRotate);
		pImpl->m_TargetQuaternion.Normalize();
	}
	void RotateComponent::SetTargetRotate(float x, float y, float z){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_TargetQuaternion.Normalize();
	}

	//����
	void RotateComponent::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				//�N�I�[�^�j�I�����猻�݂̉�]�x�N�g���𓾂�
				pImpl->m_StartQuaternion = TransPtr->GetQuaternion();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}
	void RotateComponent::Update(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Quaternion Qt = QuaternionEX::Slerp(
					pImpl->m_StartQuaternion,
					pImpl->m_TargetQuaternion,
					GetNowTime() / GetTotalTime()
					);
				Qt.Normalize();
				TransPtr->SetQuaternion(Qt);
			}
			else{
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//���݉�]���v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	Vector3 RotateComponent::GetNowRotate() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetQuaternion.GetRotation();
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
			);
		Qt.Normalize();
		return Qt.GetRotation();
	}
	Quaternion RotateComponent::GetNowQuaternion() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetQuaternion;
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
			);
		Qt.Normalize();
		return Qt;
	}



	//--------------------------------------------------------------------------------------
	//	class RotateTo : public  MoveComponent ;
	//	�p�r: �ړI�̊p�x�ɉ�]
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateTo::RotateTo(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr)
	{}
	RotateTo::~RotateTo(){}

	void RotateTo::SetParams(float TotalTime, const Vector3& Target){
		SetTargetRotate(Target);
		SetTotalTime(TotalTime);
	}

	void RotateTo::SetParams(float TotalTime, const Quaternion& TargetQuaternion){
		SetTargetQuaternion(TargetQuaternion);
		SetTotalTime(TotalTime);
	}


	//--------------------------------------------------------------------------------------
	//	struct RotateBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct RotateBy::Impl{
		Quaternion m_LocalQuaternion;	//���Ή�]
		Impl() :
			m_LocalQuaternion()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	�p�r: �ړI�̑��Ίp�x�ɉ�]
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RotateBy::RotateBy(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr), pImpl(new Impl()){}
	RotateBy::~RotateBy(){}

	//�A�N�Z�T
	const Quaternion& RotateBy::GetLocalQuaternion() const{ return pImpl->m_LocalQuaternion; }
	void RotateBy::SetLocalQuaternion(const Quaternion& LocalQuaternion){
		pImpl->m_LocalQuaternion = LocalQuaternion;
		pImpl->m_LocalQuaternion.Normalize();
	}
	Vector3 RotateBy::GetLocalRotate() const{
		return pImpl->m_LocalQuaternion.GetRotation();
	}
	void RotateBy::SetLocalRotate(const Vector3& LocalRotate){
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(LocalRotate);
		pImpl->m_LocalQuaternion.Normalize();
	}
	void RotateBy::SetLocalRotate(float x, float y, float z){
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_LocalQuaternion.Normalize();
	}

	void RotateBy::SetParams(float TotalTime, const Vector3& LocalRotate){
		SetTotalTime(TotalTime);
		SetLocalRotate(LocalRotate);
	}

	void RotateBy::SetParams(float TotalTime, const Quaternion& LocalQuaternion){
		SetTotalTime(TotalTime);
		SetLocalQuaternion(LocalQuaternion);
	}


	//����
	void RotateBy::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Quaternion NowQt = TransPtr->GetQuaternion();
				Quaternion Target = NowQt * pImpl->m_LocalQuaternion;
				SetTargetQuaternion(Target);
				RotateComponent::Run();
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct MoveComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MoveComponent::Impl{
		Vector3 m_StartPosition;	//�J�n�n�_
		Vector3 m_TargetPosition;	//�ړI�n�_
		Lerp::rate m_RateX;	//��ԕ��@X
		Lerp::rate m_RateY;	//��ԕ��@Y
		Lerp::rate m_RateZ;	//��ԕ��@Z
		Vector3 m_Velocity;	//���݂̑��x
		Impl() :
			m_StartPosition(0, 0, 0),
			m_TargetPosition(0, 0, 0),
			m_RateX(Lerp::Linear),
			m_RateY(Lerp::Linear),
			m_RateZ(Lerp::Linear),
			m_Velocity(0,0,0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	�p�r: �ړ��A�N�V�����̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveComponent::MoveComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	MoveComponent::~MoveComponent(){}
	//�A�N�Z�T

	Lerp::rate MoveComponent::GetRateX() const{
		return pImpl->m_RateX;
	}
	Lerp::rate MoveComponent::GetRateY() const{
		return pImpl->m_RateY;
	}
	Lerp::rate MoveComponent::GetRateZ() const{
		return pImpl->m_RateZ;
	}
	void MoveComponent::SetRateX(const Lerp::rate r){
		pImpl->m_RateX = r;
	}
	void MoveComponent::SetRateY(const Lerp::rate r){
		pImpl->m_RateY = r;
	}
	void MoveComponent::SetRateZ(const Lerp::rate r){
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate r){
		pImpl->m_RateX = r;
		pImpl->m_RateY = r;
		pImpl->m_RateZ = r;
	}

	Vector3 MoveComponent::GetVelocity(){
		if (GetTotalTime() <= 0){
			pImpl->m_Velocity = Vector3(0, 0, 0);
		}
		else{
			if (!GetRun() || GetArrived()){
				pImpl->m_Velocity = Vector3(0, 0, 0);
			}
		}
		return pImpl->m_Velocity;
	}


	const Vector3& MoveComponent::GetStartPosition() const{ return pImpl->m_StartPosition; }
	void MoveComponent::SetStartPosition(const Vector3& StartPosition){ pImpl->m_StartPosition = StartPosition; }
	void MoveComponent::SetStartPosition(float x, float y, float z){ pImpl->m_StartPosition = Vector3(x, y, z); }

	const Vector3& MoveComponent::GetTargetPosition() const{ return pImpl->m_TargetPosition; }
	void MoveComponent::SetTargetPosition(const Vector3& TargetPosition){ pImpl->m_TargetPosition = TargetPosition; }
	void MoveComponent::SetTargetPosition(float x, float y, float z){ pImpl->m_TargetPosition = Vector3(x, y, z); }

	Vector3 MoveComponent::CalcVelocitySub(float NowTime){
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
		}
		else{
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateZ
				);
		}
		return Pos;
	}


	void MoveComponent::CalcVelocity(){
		if (GetTotalTime() <= 0){
			pImpl->m_Velocity = Vector3(0, 0, 0);
			return;
		}
		else{
			if (!GetRun() || GetArrived()){
				pImpl->m_Velocity = Vector3(0, 0, 0);
				return;
			}
			else{
				float NowTime = GetNowTime();
				if (NowTime <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				if (ElapsedTime <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float BeforeTime = GetNowTime() - App::GetApp()->GetElapsedTime();
				if (BeforeTime <= 0){
					BeforeTime = 0;
				}
				if ((NowTime - BeforeTime) <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				Vector3 BeforePos = CalcVelocitySub(BeforeTime);
				Vector3 NowPos = CalcVelocitySub(NowTime);
				pImpl->m_Velocity = (NowPos - BeforePos) / ElapsedTime;
				return;
			}
		}
	}


	//����
	void MoveComponent::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_StartPosition = TransPtr->GetPosition();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ɐݒ肳��Ă���
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	void MoveComponent::Update(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Vector3 Pos;
				if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
					Pos = Lerp::CalculateLerp<Vector3>(
						pImpl->m_StartPosition,
						pImpl->m_TargetPosition,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
				}
				else{
					Pos.x = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.x,
						pImpl->m_TargetPosition.x,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
					Pos.y = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.y,
						pImpl->m_TargetPosition.y,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateY
						);
					Pos.z = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.z,
						pImpl->m_TargetPosition.z,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateZ
						);
				}
				TransPtr->SetPosition(Pos);
			}
			else{
				if (GetTotalTime() <= 0){
					//�g�[�^���^�C����0�ȉ��Ȃ�A���łɓ������Ă��邱�ƂɂȂ�
					TransPtr->SetPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	//���݈ʒu���v�Z���ĕԂ�
	//�^�C���̍X�V�͍s��Ȃ��̂�
	//Update��ɌĂԂׂ�
	Vector3 MoveComponent::GetNowPosition() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetPosition;
		}
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
		}
		else{
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateZ
				);
		}
		return Pos;
	}



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	�p�r: �ړI�̈ʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	MoveTo::MoveTo(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr){}
	MoveTo::~MoveTo(){}

	void MoveTo::SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate){
		SetTargetPosition(Target);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveTo::SetParams(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		SetTargetPosition(Target);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}



	//--------------------------------------------------------------------------------------
	//	struct MoveBy::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MoveBy::Impl{
		Vector3 m_LocalVector;	//���Ε���
		Impl() :
			m_LocalVector(0, 0, 0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	�p�r: �ړI�̑��Έʒu�Ɉړ�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBy::MoveBy(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr), pImpl(new Impl()){}
	MoveBy::~MoveBy(){}

	//�A�N�Z�T
	const Vector3& MoveBy::GetLocalVector() const{ return pImpl->m_LocalVector; }
	void MoveBy::SetLocalVector(const Vector3& LocalVector){ pImpl->m_LocalVector = LocalVector; }
	void MoveBy::SetLocalVector(float x, float y, float z){ pImpl->m_LocalVector = Vector3(x, y, z); }

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate){
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}

	//����
	void MoveBy::Run(){
		//m_GameObject��null�Ȃ�Update���Ă��Ӗ����Ȃ�
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Vector3 Target = TransPtr->GetPosition() + pImpl->m_LocalVector;
				SetTargetPosition(Target);
				MoveComponent::Run();
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct Action::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Action::Impl{
		bool m_Looped;

		bool m_ScaleArrived;	//m_Loop��false�̂Ƃ��̂ݗL��
		bool m_RotateArrived;	//m_Loop��false�̂Ƃ��̂ݗL��
		bool m_MoveArrived;	//m_Loop��false�̂Ƃ��̂ݗL��


		vector<shared_ptr<ActionComponent> > m_ScaleVec;
		vector<shared_ptr<ActionComponent> > m_RotateVec;
		vector<shared_ptr<ActionComponent> > m_MoveVec;

		size_t m_ScaleActiveIndex;
		size_t m_RotateActiveIndex;
		size_t m_MoveActiveIndex;

		Impl() :
			m_Looped(false),
			m_ScaleArrived(false),
			m_RotateArrived(false),
			m_MoveArrived(false),
			m_ScaleActiveIndex(0),
			m_RotateActiveIndex(0),
			m_MoveActiveIndex(0)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	�p�r: �����ʒu�R���g���[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Action::Action(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Action::~Action(){}

	//�A�N�Z�T
	bool Action::IsLooped()const{ return pImpl->m_Looped; }
	bool Action::GetLooped()const{ return pImpl->m_Looped; }
	void Action::SetLooped(bool b){ pImpl->m_Looped = b; }

	bool Action::IsArrived()const{
		bool retScale = true;
		if (pImpl->m_ScaleVec.size() > 0){
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_ScaleArrived){
				retScale = false;
			}
		}
		bool retRot = true;
		if (pImpl->m_RotateVec.size() > 0){
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_RotateArrived){
				retRot = false;
			}
		}
		bool retMove = true;
		if (pImpl->m_MoveVec.size() > 0){
			//�A�N�V���������邪�I�����ĂȂ�
			if (!pImpl->m_MoveArrived){
				retMove = false;
			}
		}
		return
			retScale &&
			retRot &&
			retMove;
	}
	bool Action::GetArrived()const{ return IsArrived(); }


	size_t Action::GetScaleActiveIndex() const {
		return pImpl->m_ScaleActiveIndex;
	}
	size_t Action::GetRotateActiveIndex() const {
		return pImpl->m_RotateActiveIndex;
	}
	size_t Action::GetMoveActiveIndex() const {
		return pImpl->m_MoveActiveIndex;
	}

	//���݂̑��x
	//Move�R���|�[�l���g���Ȃ��ꍇ��Vector3(0,0,0)��Ԃ�
	Vector3 Action::GetVelocity(){
		if (pImpl->m_MoveVec.size() <= 0){
			return Vector3(0, 0, 0);
		}
		else{
			auto Ptr = dynamic_pointer_cast<MoveComponent>(pImpl->m_MoveVec[pImpl->m_MoveActiveIndex]);
			return Ptr->GetVelocity();
		}
	}


	//����
	shared_ptr<ScaleTo> Action::AddScaleTo(float TotalTime, const Vector3& Target, Lerp::rate Rate){
		auto Ptr = Object::CreateObject<ScaleTo>(GetGameObject());
		Ptr->SetParams(TotalTime, Target, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}
	shared_ptr<ScaleBy> Action::AddScaleBy(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate){
		auto Ptr = Object::CreateObject<ScaleBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionInterval> Action::AddScaleInterval(float TotalTime){
		auto Ptr = Object::CreateObject<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetScaleComponent(size_t TargetIndex){
		if (TargetIndex >= pImpl->m_ScaleVec.size()){
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_ScaleVec.size())",
				L"Action::GetScaleComponent()"
				);
		}
		return pImpl->m_ScaleVec[TargetIndex];
	}


	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Vector3& Target){
		auto Ptr = Object::CreateObject<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, Target);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Quaternion& TargetQuaternion){
		auto Ptr = Object::CreateObject<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Vector3& LocalRotate){
		auto Ptr = Object::CreateObject<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Quaternion& LocalQuaternion){
		auto Ptr = Object::CreateObject<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddRotateInterval(float TotalTime){
		auto Ptr = Object::CreateObject<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetRotateComponent(size_t TargetIndex){
		if (TargetIndex >= pImpl->m_RotateVec.size()){
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_RotateVec.size())",
				L"Action::GetRotateComponent()"
				);
		}
		return pImpl->m_RotateVec[TargetIndex];
	}




	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate Rate){
		auto Ptr = Object::CreateObject<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, Target, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		auto Ptr = Object::CreateObject<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, Target, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate){
		auto Ptr = Object::CreateObject<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		auto Ptr = Object::CreateObject<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddMoveInterval(float TotalTime){
		auto Ptr = Object::CreateObject<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetMoveComponent(size_t TargetIndex){
		if (TargetIndex >= pImpl->m_MoveVec.size()){
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				L"if (TargetIndex >= pImpl->m_MoveVec.size())",
				L"Action::GetMoveComponent()"
				);
		}
		return pImpl->m_MoveVec[TargetIndex];
	}

	//���ׂẴA�N�V�������N���A����
	void Action::AllActionClear(){
		pImpl->m_ScaleVec.clear();
		pImpl->m_RotateVec.clear();
		pImpl->m_MoveVec.clear();

		pImpl->m_Looped = false;
		pImpl->m_ScaleArrived = false;
		pImpl->m_RotateArrived = false;
		pImpl->m_MoveArrived = false;
		pImpl->m_ScaleActiveIndex = 0;
		pImpl->m_RotateActiveIndex = 0;
		pImpl->m_MoveActiveIndex = 0;
	}



	void Action::RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0){
			TgtIndex = 0;
			TgtVector[TgtIndex]->Run();
		}
	}
	void Action::Run(){
		RunSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		RunSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		RunSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			TgtVector[TgtIndex]->Stop();
		}
	}
	void Action::Stop(){
		StopSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		StopSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		StopSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			TgtVector[TgtIndex]->ReStart();
		}
	}

	void Action::ReStart(){
		ReStartSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		ReStartSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		ReStartSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	bool Action::UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		bool ret = false;
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			if (TgtVector[TgtIndex]->IsUpdateActive()){
				TgtVector[TgtIndex]->Update();
				if (TgtVector[TgtIndex]->IsArrived()){
					//��������
					//�C���f�b�N�X�����Z
					TgtIndex++;
					if (TgtIndex >= TgtVector.size()){
						//�͈͊O�Ȃ�
						if (pImpl->m_Looped){
							TgtIndex = 0;
							TgtVector[TgtIndex]->Run();
						}
						else{
							TgtIndex = TgtVector.size() - 1;
							//Run�͍s��Ȃ�
							//��������
							ret = true;
						}
					}
					else{
						TgtVector[TgtIndex]->Run();
					}
				}
			}
		}
		return ret;
	}
	//�X�V
	void Action::Update(){
		pImpl->m_ScaleArrived = UpdateSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		pImpl->m_RotateArrived = UpdateSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		pImpl->m_MoveArrived = UpdateSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}




	//--------------------------------------------------------------------------------------
	//	struct Camera::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct Camera::Impl{
		Vector3 m_Eye;	//�J�����ʒu
		Vector3 m_At;	//���ړ_
		Vector3 m_Up;   //�J�����̌X���i�A�b�v�j
		bool m_Pers;	//���ߖ@���g�����ǂ���
		weak_ptr<GameObject> m_CameraObject;

		D3D11_VIEWPORT m_ViewPort;

		float m_FovY;   //�ˉe�p�x
		float m_Aspect;	//�A�X�y�N�g��

		float m_Width;	//���ߖ@���g��Ȃ��ꍇ�i���ˉe�j�̕�
		float m_Height;	//���ߖ@���g��Ȃ��ꍇ�i���ˉe�j�̍���
		float m_Near;	//��O�̍ŏ�����
		float m_Far;	//���̍ő勗��

		Matrix4X4 m_ViewMatrix;
		Matrix4X4 m_ProjMatrix;
		Impl() :
			m_Eye(0, 10.0f, -20.0f),	//�f�t�H���g�͌���΂ߏ�
			m_At(0, 0, 0),
			m_Up(0, 1.0f, 0),
			m_Pers(true),
			m_FovY(XM_PIDIV4),
			m_Aspect(0),
			m_Width(0),
			m_Height(0),
			m_Near(0.5f),
			m_Far(1000.0f),
			m_ViewMatrix(),
			m_ProjMatrix()
		{
			try{
				m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
				m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
				m_ViewPort.MinDepth = 0.0f;
				m_ViewPort.MaxDepth = 1.0f;
				m_ViewPort.TopLeftX = 0;
				m_ViewPort.TopLeftY = 0;

				m_Width = m_ViewPort.Width;
				m_Height = m_ViewPort.Height;
				m_Aspect = m_Width / m_Height;
			}
			catch (...){
				throw;
			}
		}
		~Impl(){}





	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	�p�r: �J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	Camera::Camera()
		:Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{

	}
	Camera::~Camera(){}

	//�A�N�Z�T
	const Vector3& Camera::GetEye() const{ return pImpl->m_Eye; }
	void Camera::SetEye(const Vector3& Eye){ pImpl->m_Eye = Eye; }
	void Camera::SetEye(float x, float y, float z){ pImpl->m_Eye = Vector3(x, y, z); }

	const Vector3& Camera::GetAt() const{ return pImpl->m_At; }
	void Camera::SetAt(const Vector3& At){ pImpl->m_At = At; }
	void Camera::SetAt(float x, float y, float z){ pImpl->m_At = Vector3(x, y, z); }

	const Vector3& Camera::GetUp() const{ return pImpl->m_Up; }
	void Camera::SetUp(const Vector3& Up){ pImpl->m_Up = Up; }
	void Camera::SetUp(float x, float y, float z){ pImpl->m_Up = Vector3(x, y, z); }

	bool Camera::IsPers()const{ return pImpl->m_Pers; }
	bool Camera::GetPers()const{ return pImpl->m_Pers; }
	void Camera::SetPers(bool b){ pImpl->m_Pers = b; }

	float Camera::GetFovY() const{ return pImpl->m_FovY; }
	void Camera::SetFovY(float f){ pImpl->m_FovY = f; }

	float Camera::GetAspect() const{ return pImpl->m_Aspect; }
	void Camera::SetAspect(float f){ pImpl->m_Aspect = f; }

	float Camera::GetWidth() const{ return pImpl->m_Width; }
	void Camera::SetWidth(float f){ pImpl->m_Width = f; }

	float Camera::GetHeight() const{ return pImpl->m_Height; }
	void Camera::SetHeight(float f){ pImpl->m_Height = f; }

	float Camera::GetNear() const{ return pImpl->m_Near; }
	void Camera::SetNear(float f){ pImpl->m_Near = f; }

	float Camera::GetFar() const{ return pImpl->m_Far; }
	void Camera::SetFar(float f){ pImpl->m_Far = f; }

	shared_ptr<GameObject> Camera::GetCameraObject() const{
		if (pImpl->m_CameraObject.expired()){
			return nullptr;
		}
		else{
			return pImpl->m_CameraObject.lock();
		}
	}
	void Camera::SetCameraObject(const shared_ptr<GameObject>& Obj){
		if (!Obj){
			pImpl->m_CameraObject.reset();
		}
		else{
			pImpl->m_CameraObject = Obj;
		}
	}
	void Camera::ClearCameraObject(const shared_ptr<GameObject>& Obj){
		SetCameraObject(nullptr);
	}

	void Camera::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
		pImpl->m_Width = pImpl->m_ViewPort.Width;
		pImpl->m_Height = pImpl->m_ViewPort.Height;
		pImpl->m_Aspect = pImpl->m_Width / pImpl->m_Height;
	}


	const Matrix4X4& Camera::GetViewMatrix() const{ return pImpl->m_ViewMatrix; }
	const Matrix4X4& Camera::GetProjMatrix() const{ return pImpl->m_ProjMatrix; }

	//����
	void Camera::Update(){
		if (!pImpl->m_CameraObject.expired()){
			auto ShPtr = pImpl->m_CameraObject.lock();
			auto TransPtr = ShPtr->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_ViewMatrix.LookAtLH(TransPtr->GetPosition(), pImpl->m_At, pImpl->m_Up);
				if (pImpl->m_Pers){
					pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
				}
				else{
					pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
				}
			}
		}
		else{
			pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
			if (pImpl->m_Pers){
				pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
			}
			else{
				pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct LookAtCamera::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct LookAtCamera::Impl{
		weak_ptr<GameObject> m_TargetObject;	//�ڕW�ƂȂ�I�u�W�F�N�g
		float m_ToTargetLerp;	//�ڕW��ǂ�������ۂ̕�Ԓl
		Impl():
			m_ToTargetLerp(1.0f)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	�p�r: LookAt�J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	LookAtCamera::LookAtCamera() :
		Camera(),
		pImpl(new Impl())
	{
	}
	LookAtCamera::~LookAtCamera(){}
	//�A�N�Z�T
	shared_ptr<GameObject> LookAtCamera::GetTargetObject() const{
		if (!pImpl->m_TargetObject.expired()){
			return pImpl->m_TargetObject.lock();
		}
		return nullptr;
	}

	void LookAtCamera::SetTargetObject(const shared_ptr<GameObject>& Obj){
		pImpl->m_TargetObject = Obj;
	}

	float LookAtCamera::GetToTargetLerp() const{
		return pImpl->m_ToTargetLerp;
	}
	void LookAtCamera::SetToTargetLerp(float f){
		pImpl->m_ToTargetLerp = f;
	}


	void LookAtCamera::Update(){
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (CntlVec[0].bConnected){
			Vector3 NewAt(0, 0, 0);
			auto TargetPtr = GetTargetObject();
			if (TargetPtr){
				//�ڎw�������ꏊ
				Vector3 ToAt = TargetPtr->GetComponent<Transform>()->GetPosition();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, pImpl->m_ToTargetLerp, Lerp::Linear);
			}
			//�X�e�b�v1�A�����_�ƈʒu�̕ύX
			Vector3 Span = GetAt() - GetEye();
			Vector3 NewEye = NewAt - Span;
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v�Q�A�Y�[���̕ύX
			//�J�����ʒu�ƒ����_�̊Ԃ̃x�N�g�����Z�o
			Span = GetAt() - GetEye();
			//���K��
			Span.Normalize();
			//�ω��l�̌���
			Span = Span * ElapsedTime * 10.0f;

			Vector3 NewArm = GetAt() - GetEye();
			//D�p�b�h��
			//�J����������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN){
				//�J�����ʒu������
				NewEye = NewEye - Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() > (GetFar() * 0.1f)){
					NewEye = NewEye + Span;
					NewArm = NewAt - NewEye;
				}
			}
			//D�p�b�h��
			//�J���������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP){
				//�J�����ʒu�����
				NewEye = NewEye + Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() < GetNear() * 2.0f){
					NewEye = NewEye - Span;
					NewArm = NewAt - NewEye;
				}
			}
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v3�p�x�̕ύX
			//���݂�At��Eye�̊p�x�𓾂�
			Vector3 ArmInv = GetEye() - GetAt();
			//�E�X�e�B�b�NX����
			FLOAT AngleY = 0;
			//�E�X�e�B�b�NY����
			FLOAT AngleX = 0;
			FLOAT AngleZ = 0;
			if (CntlVec[0].fThumbRX != 0){
				AngleY = -CntlVec[0].fThumbRX * ElapsedTime;
			}
			if (CntlVec[0].fThumbRY != 0){
				AngleX = CntlVec[0].fThumbRY * ElapsedTime;
				AngleZ = CntlVec[0].fThumbRY * ElapsedTime;
			}
			if (ArmInv.z > 0){
				AngleX *= -1.0f;
			}
			if (ArmInv.x < 0){
				AngleZ *= -1.0f;
			}
			Quaternion QtSpan(AngleX, AngleY, AngleZ);
			QtSpan.Normalize();
			//��]��v�Z�̍s����쐬
			Matrix4X4 Mat, Mat2;
			Mat.STRTransformation(
				Vector3(1.0f, 1.0f, 1.0f),
				ArmInv,
				QtSpan);
			Mat2.TranslationFromVector(GetAt());
			Mat *= Mat2;
			NewEye = Mat.PosInMatrix();
			if (NewEye.y < 0.5f){
				NewEye.y = 0.5f;
			}
			//�J���������ȏ�A�ォ�王���ɂȂ�Ȃ悤�ɒ���
			ArmInv = NewEye - GetAt();
			ArmInv.Normalize();
			float y2 = ArmInv.y * ArmInv.y;
			float x2 = ArmInv.x * ArmInv.x;
			float z2 = ArmInv.z * ArmInv.z;
			if (y2 <= (x2 + z2)){
				SetEye(NewEye);
			}

		}
		Camera::Update();
	}


	//--------------------------------------------------------------------------------------
	//	struct Light::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct Light::Impl{
		Vector3 m_Directional;	//���C�g�̌���
		Color4 m_DiffuseColor;	//�f�B�t�B�[�Y�F
		Color4 m_SpecularColor;	//�X�y�L�����[�F
		Impl() :
			m_Directional(0, 0, 0),
			m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
			{
		}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public ShapeInterface ;
	//	�p�r: ���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Light::Light() :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
		SetPositionToDirectional(Vector3(-1.0f, 1.0f, -1.0f));
	}
	Light::Light(
		const Vector3& Directional,
		const Color4& DiffuseColor,
		const Color4& SpecularColor
		):
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
		pImpl->m_Directional = Directional;
		pImpl->m_DiffuseColor = DiffuseColor;
		pImpl->m_SpecularColor = SpecularColor;
	}

	Light::~Light(){}

	//�A�N�Z�T
	const Vector3& Light::GetDirectional() const{
		return pImpl->m_Directional; 
	}
	void Light::SetDirectional(const Vector3& Directional){
		pImpl->m_Directional = Directional; 
		pImpl->m_Directional.Normalize(); 
	}
	void Light::SetDirectional(float x, float y, float z){
		pImpl->m_Directional = Vector3(x, y, z);
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(const Vector3& Position){
		pImpl->m_Directional = Position;
		pImpl->m_Directional *= -1.0f;
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(float x, float y, float z){ 
		pImpl->m_Directional = Vector3(x, y, z); 
		pImpl->m_Directional *= -1.0f; 
		pImpl->m_Directional.Normalize();
	}

	const Color4& Light::GetDiffuseColor() const{
		return pImpl->m_DiffuseColor;
	}
	void Light::SetDiffuseColor(const Color4& col){ 
		pImpl->m_DiffuseColor = col;
	}
	void Light::SetDiffuseColor(float r, float g, float b, float a){ 
		pImpl->m_DiffuseColor = Color4(r, g, b, a);
	}

	const Color4& Light::GetSpecularColor() const{
		return pImpl->m_SpecularColor;
	}
	void Light::SetSpecularColor(const Color4& col){ 
		pImpl->m_SpecularColor = col;
	}
	void Light::SetSpecularColor(float r, float g, float b, float a){ 
		pImpl->m_SpecularColor = Color4(r, g, b, a);
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiLight::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct MultiLight::Impl{
		vector< shared_ptr<Light> > m_LightVec;
		Impl(){}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public ShapeInterface ;
	//	�p�r: �}���`���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiLight::MultiLight():
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
	}
	MultiLight::~MultiLight(){
	}
	//Light�̐�
	size_t MultiLight::GetLightCount() const{ 
		return pImpl->m_LightVec.size(); 
	}
	//Light�̎擾
	shared_ptr<Light> MultiLight::GetLight(size_t Index)const{
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::GetLight()"
				);
		}
		return pImpl->m_LightVec[Index];
	}
	//Light�̃Z�b�g
	void MultiLight::SetLight(size_t Index, shared_ptr<Light>& Ptr){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::SetLight()"
				);
		}
		pImpl->m_LightVec[Index] = Ptr;
	}
	//�z��̎Q��
	vector< shared_ptr<Light> >& MultiLight::GetLightVec(){ 
		return pImpl->m_LightVec;
	}
	//Light�̒ǉ�
	shared_ptr<Light> MultiLight::AddLight(){
		auto Ptr = Object::CreateObject<Light>();
		pImpl->m_LightVec.push_back(Ptr);
		return Ptr;
	}
	//Light�̍폜
	void MultiLight::RemoveLight(size_t Index){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::RemoveLight()"
				);
		}
		vector< shared_ptr<Light> >::iterator it;
		size_t count = 0;
		for (it = pImpl->m_LightVec.begin(); it != pImpl->m_LightVec.end(); it++){
			if (count == Index){
				pImpl->m_LightVec.erase(it);
				return;
			}
			count++;
		}
	}

	//�f�t�H���g�̃��C�e�B���O�̐ݒ�
	void MultiLight::SetDefaultLighting(){
		size_t NouCount = pImpl->m_LightVec.size();
		pImpl->m_LightVec.clear();
		//�Œ��̓��C�g��u��
		auto Ptr = Object::CreateObject<Light>(
			Vector3(0.5265408f, -0.5735765f, 0.6275069f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
			);
		pImpl->m_LightVec.push_back(Ptr);
		if (NouCount >= 2){

			Ptr = Object::CreateObject<Light>(
				Vector3(-0.7198464f, 0.3420201f, -0.6040227f),
				Color4(0.9647059f, 0.7607844f, 0.4078432f, 1.0f),
				Color4(0.0000000f, 0.0000000f, 0.0000000f, 0.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
		if (NouCount >= 3){
			Ptr = Object::CreateObject<Light>(
				Vector3(0.4545195f, -0.7660444f, 0.4545195f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
	}


	//����
	void MultiLight::Update(){
		for (auto Ptr : pImpl->m_LightVec){
			Ptr->Update();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct View::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct View::Impl{
		D3D11_VIEWPORT m_ViewPort;	//�r���[�|�[�g
		Color4 m_BackColor;			//�w�i�F
		shared_ptr<Camera> m_Camera;	//�J����
		shared_ptr<MultiLight> m_MultiLight;	//���C�g
		explicit Impl() :
			m_BackColor(0.0f, 0.125f, 0.3f, 1.0f)
		{
			//�r���[�|�[�g�̃Z�b�g�A�b�v(�N���C�A���g�̈�S��)
			ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
			m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			m_ViewPort.MinDepth = 0.0f;
			m_ViewPort.MaxDepth = 1.0f;
			m_ViewPort.TopLeftX = 0;
			m_ViewPort.TopLeftY = 0;
			//�J�������쐬���Ă���
			m_Camera = make_shared<Camera>();
			//���C�g���쐬���Ă���
			m_MultiLight = make_shared<MultiLight>();
		}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public ShapeInterface;
	//	�p�r: �r���[�N���X�i�r���[�|�[�g�ƃ��C�g�A�J�������Ǘ��A�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�e���v���[�g����Ă΂��T�u�֐�
	void View::ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr){
		pImpl->m_ViewPort = vp;
		pImpl->m_BackColor = Col;
		pImpl->m_Camera = CameraPtr;
		pImpl->m_Camera->SetViewPort(vp);
		pImpl->m_MultiLight = MultiLightPtr;
	}

	//�\�z�Ɣj��
	View::View() :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
	}
	View::~View(){}

	//�A�N�Z�T
	const D3D11_VIEWPORT& View::GetViewPort()const { 
		return pImpl->m_ViewPort; 
	}
	const D3D11_VIEWPORT* View::GetViewPortPtr()const {
		return &pImpl->m_ViewPort;
	}
	void View::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
	}
	const Color4& View::GetBackColor() const {
		return pImpl->m_BackColor;
	}
	void View::SetBackColor(const Color4& c){ 
		pImpl->m_BackColor = c;
	}
	shared_ptr<Camera> View::GetCamera() const { 
		return pImpl->m_Camera;
	}
	void View::SetCamera(shared_ptr<Camera>& Ptr){ 
		pImpl->m_Camera = Ptr;
	}

	shared_ptr<MultiLight> View::GetMultiLight() const { 
		return pImpl->m_MultiLight;
	}
	void View::SetMultiLight(shared_ptr<MultiLight>& Lights){ 
		pImpl->m_MultiLight = Lights;
	}

	shared_ptr<MultiLight> View::SetDefaultLighting(){
		pImpl->m_MultiLight->SetDefaultLighting();
		return pImpl->m_MultiLight;
	}

	shared_ptr<Light> View::AddLight(){
		return pImpl->m_MultiLight->AddLight();
	}



	//����
	void View::Update(){
		if (pImpl->m_Camera){
			pImpl->m_Camera->Update();
		}
		if (pImpl->m_MultiLight){
			pImpl->m_MultiLight->Update();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct MultiView::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MultiView::Impl{
		vector< shared_ptr<View> > m_ViewVec;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiView: public Component;
	//	�p�r: �}���`�r���[�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiView::MultiView(const shared_ptr<GameObject>& GameObjectPtr)
		: Component(GameObjectPtr),
		pImpl(new Impl())
	{
		try{
			auto Ptr = dynamic_pointer_cast<Stage>(GameObjectPtr);
			if (!Ptr){
				throw BaseException(
					L"�Q�[���I�u�W�F�N�g���X�e�[�W�ł͂���܂���",
					L"MultiView�̓X�e�[�W�ɂ̂ݒǉ��ł��܂�",
					L"MultiView::MultiView()"
					);
			}
		}
		catch (...){
			throw;
		}
	}
	MultiView::~MultiView(){}
	//�A�N�Z�T
	size_t MultiView::GetViewCount()const{
		return pImpl->m_ViewVec.size(); 
	}
	//ViewVec�𒼐ڑ��삷�鎞�p
	vector< shared_ptr<View> >& MultiView::GetViewVec(){
		return pImpl->m_ViewVec;
	}
	shared_ptr<View> MultiView::GetView(size_t Index) const {
		if (Index >= pImpl->m_ViewVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_ViewVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiView::GetView()"
				);
		}
		return pImpl->m_ViewVec[Index];
	}
	//����
	shared_ptr<View> MultiView::AddView(){
		auto Ptr = make_shared<View>();
		pImpl->m_ViewVec.push_back(Ptr);
		return Ptr;
	}

	void MultiView::Update(){
		for (auto Ptr : pImpl->m_ViewVec){
			Ptr->Update();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PlayMusic::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct PlayMusic::Impl{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//���\�[�X�L�[
		wstring m_ResKey;
		Impl(const wstring& ResKey) :
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl(){
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PlayMusic: public Component;
	//	�p�r: �~���[�W�b�N�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PlayMusic::PlayMusic(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey) :
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try{
			auto MusicRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = MusicRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetMusicEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx()),
				L"�~���[�W�b�N�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx())",
				L"PlayMusic::PlayMusic()"
				);
		}
		catch (...){
			throw;
		}
	}
	PlayMusic::~PlayMusic(){
	}

	shared_ptr<AudioResource> PlayMusic::GetAudioResource() const{
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::GetAudioResource()"
				);
		}
		return pImpl->m_AudioResource.lock();
	}


	void PlayMusic::Start(size_t LoopCount, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//���d��`�֐����Ă�
		Start(buffer, Volume);
	}

	void PlayMusic::Start(const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
			);
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
				);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"�~���[�W�b�N�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"PlayMusic::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�~���[�W�b�N�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"PlayMusic::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"�~���[�W�b�N�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Start()",
			L"PlayMusic::Start()"
			);

	}


	void PlayMusic::Stop(){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
			);
	}





	//--------------------------------------------------------------------------------------
	//	struct SoundEffect::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct SoundEffect::Impl{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//���\�[�X�L�[
		wstring m_ResKey;
		Impl(const wstring& ResKey):
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl(){
		}
	};



	//--------------------------------------------------------------------------------------
	//	class SoundEffect: public Component;
	//	�p�r: �T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SoundEffect::SoundEffect(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey):
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try{
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"SoundEffect::SoundEffect()"
				);
		}
		catch (...){
			throw;
		}
	}
	SoundEffect::~SoundEffect(){
	}

	shared_ptr<AudioResource> SoundEffect::GetAudioResource() const{
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::GetAudioResource()"
				);
		}
		return pImpl->m_AudioResource.lock();
	}


	void SoundEffect::Start(size_t LoopCount, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//���d��`�֐����Ă�
		Start(buffer, Volume);
	}

	void SoundEffect::Start(const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Start()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Start()"
			);
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
				);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"SoundEffect::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundEffect::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Start()",
			L"SoundEffect::Start()"
			);

	}


	void SoundEffect::Stop(){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Stop()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Stop()"
			);
	}




	//--------------------------------------------------------------------------------------
	//	struct MultiSoundEffect::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct MultiSoundEffect::Impl{
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl(){
		}
		SoundItem* GetItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//�������O����������
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//�������O����������
				//Map����폜
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);

	};

	void MultiSoundEffect::Impl::Start(const SoundItem* pItem,const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Start()"
			);
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiSoundEffect::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiSoundEffect::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiSoundEffect::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiSoundEffect::Impl::Start()"
			);
	}

	void MultiSoundEffect::Impl::Stop(const SoundItem* pItem){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Stop()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Stop()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Stop()"
			);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	�p�r: �}���`�T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSoundEffect::MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	MultiSoundEffect::~MultiSoundEffect(){}

	//�A�N�Z�T
	shared_ptr<AudioResource> MultiSoundEffect::AddAudioResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::AddAudioResource()"
					);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem){
				//�������O����������
				if (!pItem->m_AudioResource.expired()){
					return pItem->m_AudioResource.lock();
				}
				else{
					throw BaseException(
						L"�������O�̃I�[�f�B�I���\�[�X������܂������A���\�[�X�������ł�",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
						);
				}
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();
			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiSoundEffect::AddAudioResource()"
				);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...){
			throw;
		}
	}
	void MultiSoundEffect::RemoveAudioResource(const wstring& ResKey){
		if (ResKey == L""){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X�����󔒂ł�",
				L"if (ResKey == L\"\")",
				L"MultiSoundEffect::RemoveAudioResource()"
				);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiSoundEffect::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const{
		if (ResKey == L""){
			if (ExceptionActive){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem){
			//�������O����������
			if (!pItem->m_AudioResource.expired()){
				return pItem->m_AudioResource.lock();
			}
			else{
				if (ExceptionActive){
					throw BaseException(
						L"�������O�̃I�[�f�B�I���\�[�X������܂������A���\�[�X�������ł�",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
						);
				}
				else{
					return nullptr;
				}
			}
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
					ResKey,
					L"MultiSoundEffect::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
	}

	void MultiSoundEffect::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiSoundEffect::Start(const wstring& ResKey, size_t LoopCount, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImpl�̊֐����Ă�
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiSoundEffect::Stop(const wstring& ResKey){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiSoundEffect::Stop()"
				);
		}
		pImpl->Stop(pItem);
	}



}
//endof  basedx11
