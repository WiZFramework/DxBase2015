#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Object;
	//	�p�r: ���ׂẴI�u�W�F�N�g�̊��N���X�i���ۃN���X�j
	//--------------------------------------------------------------------------------------
	void Object::PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void Object::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}



	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	�p�r: Impl�\����
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl{
		weak_ptr<SceneBase> m_SceneBase;
		//�C�x���g�̃L���[
		list< shared_ptr<Event> > m_PriorityQ;
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//�C�x���g
		//	);
		//�p�r: �C�x���g�̑��M
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl(const shared_ptr<SceneBase>& SceneBasePtr) :m_SceneBase(SceneBasePtr){}
		~Impl(){}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event){
		if (m_SceneBase.expired()){
			//�V�[��������
			m_PriorityQ.clear();
			return;
		}
		auto SceneBasePtr = m_SceneBase.lock();
		if (event->m_ReceiverKey != L""){
			//�L�[������������
			if (auto Vec = SceneBasePtr->GetActiveStage()->GetSharedObjectGroup(event->m_ReceiverKey, false)){
				//�z�񂪂�����
				for (size_t i = 0; i < Vec->size(); i++){
					Vec->at(i)->OnEvent(event);
				}
			}
			//�����ăV�F�A�I�u�W�F�N�g�������L�[������Α���
			if (auto Ptr = SceneBasePtr->GetActiveStage()->GetSharedObject(event->m_ReceiverKey, false)){
				Ptr->OnEvent(event);
			}
		}
		else if (!event->m_Receiver.expired()){
			//�󂯎肪�L��
			auto RePtr = event->m_Receiver.lock();
			RePtr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	�p�r: �C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EventDispatcher::EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr):
		pImpl(new Impl(SceneBasePtr))
	{}
	EventDispatcher::~EventDispatcher(){}


	//�C�x���g��POST�i�L���[�ɓ����j
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(Delay,Sender, Receiver, MsgStr, Info);
		//�L���[�ɂ��߂�
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
	void EventDispatcher::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(0.0f,Sender, Receiver, MsgStr, Info);
		//���M
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::DispatchDelayedEvwnt(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()){
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f){
				(*it)->m_DispatchTime = 0.0f;
				//���b�Z�[�W�̑��M
				pImpl->Discharge(*it);
				//�L���[����폜
				it = pImpl->m_PriorityQ.erase(it);
				//�폜��̃C�e���[�^���u�Ō�v��
				//�Ƃ��̓��[�v�𔲂���
				if (it == pImpl->m_PriorityQ.end()){
					break;
				}
			}
			else{
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ(){
		pImpl->m_PriorityQ.clear();
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
				pImpl->m_Eye = TransPtr->GetPosition();
				pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
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
		Impl() :
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
		) :
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
	MultiLight::MultiLight() :
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




}
//endof  basedx11

