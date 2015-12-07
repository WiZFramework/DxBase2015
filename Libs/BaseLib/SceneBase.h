
#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class GameObject :public Object, public ShapeInterface;
	//	�p�r: �Q�[���z�u�I�u�W�F�N�g�e�N���X
	//--------------------------------------------------------------------------------------
	class GameObject :public Object, public ShapeInterface{
		shared_ptr<Component> SearchComponent(type_index TypeIndex)const;
		shared_ptr<Rigidbody> GetRigidbody()const;
		shared_ptr<Gravity> GetGravity()const;
		shared_ptr<Transform> GetTransform()const;
		shared_ptr<Collision> GetCollision()const;
		shared_ptr<CollisionSphere> GetCollisionSphere()const;
		shared_ptr<CollisionCapsule> GetCollisionCapsule()const;
		shared_ptr<CollisionObb> GetCollisionObb()const;

		void SetRigidbody(const shared_ptr<Rigidbody>& Ptr);
		void SetGravity(const shared_ptr<Gravity>& Ptr);
		void SetCollision(const shared_ptr<Collision>& Ptr);
		void SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr);
		void SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr);
		void SetCollisionObb(const shared_ptr<CollisionObb>& Ptr);
		void SetTransform(const shared_ptr<Transform>& Ptr);
		void AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr);
		template<typename T>
		shared_ptr<T> SearchDynamicComponent()const{
			auto mapPtr = GetCompoMap();
			auto it = mapPtr.begin();
			while (it != mapPtr.end()){
				auto Ptr = dynamic_pointer_cast<T>(it->second);
				if (Ptr){
					return Ptr;
				}
				it++;
			}
			return nullptr;
		}
		map<type_index, shared_ptr<Component> >& GetCompoMap() const;
		void RemoveTgtComponent(type_index TypeIndex);
	public:
		//�\�z�Ɣj��
		explicit GameObject(const shared_ptr<Stage>& StagePtr);
		virtual ~GameObject();
		//�A�N�Z�T
		bool IsUpdateActive() const;
		bool GetUpdateActive() const;
		void SetUpdateActive(bool b);

		bool IsDrawActive() const;
		bool GetDrawActive() const;
		void SetDrawActive(bool b);

		bool IsAlphaActive() const;
		bool GetAlphaActive() const;
		void SetAlphaActive(bool b);

		bool IsAlphaExActive() const;
		bool GetAlphaExActive() const;
		void SetAlphaExActive(bool b);


		//�X�v���C�g�Ƃ���Draw���邩�ǂ���
		bool IsSpriteDraw() const;
		bool GetSpriteDraw() const;
		void SetSpriteDraw(bool b);

		//�`�惌�C���[�̎擾�Ɛݒ�
		int GetDrawLayer() const;
		void  SetDrawLayer(int l);
		shared_ptr<Stage> GetStage() const;
		void SetStage(const shared_ptr<Stage>& stage);
		//����
		//�R���|�[�l���g�̎擾
		//ExceptionActive��true�̏ꍇ�͗�O�A�������nullptr��Ԃ�
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const{
			auto Ptr = SearchComponent(type_index(typeid(T)));
			if (Ptr) {
				//�w��̌^�̃R���|�[�l���g����������
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseMBException(
						"�R���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetComponent<T>()"
						);
				}
			}
			else{
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						typeid(T).name(),
						"GameObject::GetComponent<T>()"
						);
				}
			}
			return nullptr;
		}
		//Rigidbody���ꉻ
		template <>
		shared_ptr<Rigidbody> GetComponent<Rigidbody>(bool ExceptionActive)const{
			auto Ptr = GetRigidbody();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"Rigidbody",
						"GameObject::GetComponent<Rigidbody>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicRigidbody
		template <typename T>
		shared_ptr<T> GetDynamicRigidbody(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetRigidbody());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�Rigidbody����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicRigidbody<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//Gravity���ꉻ
		template <>
		shared_ptr<Gravity> GetComponent<Gravity>(bool ExceptionActive)const{
			auto Ptr = GetGravity();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"Gravity",
						"GameObject::GetComponent<Gravity>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicGravity
		template <typename T>
		shared_ptr<T> GetDynamicGravity(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetGravity());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�Gravity����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicGravity<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//Collision���ꉻ
		//�e�N���X�Ȃ̂�Get�̂ݗp��
		template <>
		shared_ptr<Collision> GetComponent<Collision>(bool ExceptionActive)const{
			auto Ptr = GetCollision();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"Collision",
						"GameObject::GetComponent<Collision>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollision
		template <typename T>
		shared_ptr<T> GetDynamicCollision(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollision());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�Collision����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicCollision<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//CollisionSphere���ꉻ
		template <>
		shared_ptr<CollisionSphere> GetComponent<CollisionSphere>(bool ExceptionActive)const{
			auto Ptr = GetCollisionSphere();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"CollisionSphere",
						"GameObject::GetComponent<CollisionSphere>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionSphere
		template <typename T>
		shared_ptr<T> GetDynamicCollisionSphere(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionSphere());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�CollisionSphere����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionSphere<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}


		//CollisionCapsule���ꉻ
		template <>
		shared_ptr<CollisionCapsule> GetComponent<CollisionCapsule>(bool ExceptionActive)const{
			auto Ptr = GetCollisionCapsule();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"CollisionCapsule",
						"GameObject::GetComponent<CollisionCapsule>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionCapsule
		template <typename T>
		shared_ptr<T> GetDynamicCollisionCapsule(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionCapsule());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�CollisionCapsule����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionCapsule<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//CollisionObb���ꉻ
		template <>
		shared_ptr<CollisionObb> GetComponent<CollisionObb>(bool ExceptionActive)const{
			auto Ptr = GetCollisionObb();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"CollisionObb",
						"GameObject::GetComponent<CollisionObb>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionObb
		template <typename T>
		shared_ptr<T> GetDynamicCollisionObb(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionObb());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�CollisionObb����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionObb<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//Transform���ꉻ
		template <>
		shared_ptr<Transform> GetComponent<Transform>(bool ExceptionActive)const{
			auto Ptr = GetTransform();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�R���|�[�l���g��������܂���",
						"Transform",
						"GameObject::GetComponent<Transform>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicTransform
		template <typename T>
		shared_ptr<T> GetDynamicTransform(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetTransform());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"�w��̌^�ւ�Transform����L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::GetDynamicTransform<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}


		//DynamicCast�𗘗p�����R���|�[�l���g�̎擾
		//�R���|�[�l���g�̔h���N���X�g�p���Ɏg��
		//ExceptionActive��true�̏ꍇ�͗�O�A�������nullptr��Ԃ�
		template <typename T>
		shared_ptr<T> GetDynamicComponent(bool ExceptionActive = true)const{
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr){
				return Ptr;
			}
			else{
				if (ExceptionActive){
					throw BaseMBException(
						"�^�L���X�g�ł���R���|�[�l���g��������܂���",
						typeid(T).name(),
						"GameObject::GetDynamicComponent<T>()"
						);
				}
			}
			return nullptr;
		}

		//�R���|�[�l���g�̒ǉ�
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params){
			auto Ptr = SearchComponent(type_index(typeid(T)));
			//�w��̌^�̃R���|�[�l���g����������
			if (Ptr){
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseMBException(
						"���łɃR���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						typeid(T).name(),
						"GameObject::AddComponent<T>()"
						);
				}
			}
			else{
				//������Ȃ��B�V���ɍ쐬����
				shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
				AddMakedComponent(type_index(typeid(T)), newPtr);
				return newPtr;
			}
		}
		//Rigidbody���ꉻ
		template <>
		shared_ptr<Rigidbody> AddComponent<Rigidbody>(){
			auto Ptr = GetRigidbody();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto RigidbodyPtr = Object::CreateObject<Rigidbody>(GetThis<GameObject>());
				SetRigidbody(RigidbodyPtr);
				return RigidbodyPtr;
			}
		}
		//DynamicRigidbody
		//Rigidbody�̔h���N���X��Rigidbody�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicRigidbody(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//Rigidbody�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Rigidbody>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Rigidbody�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicRigidbodyt<T>()"
					);
			}
			SetRigidbody(newPtr);
			return newPtr;
		}
		//Gravity���ꉻ
		template <>
		shared_ptr<Gravity> AddComponent<Gravity>(){
			auto Ptr = GetGravity();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto GravityPtr = Object::CreateObject<Gravity>(GetThis<GameObject>());
				SetGravity(GravityPtr);
				return GravityPtr;
			}
		}
		//DynamicGravity
		//Gravity�̔h���N���X��Gravity�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicGravity(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//Gravity�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Gravity>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Gravity�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicGravityt<T>()"
					);
			}
			SetGravity(newPtr);
			return newPtr;
		}

		//Collision���ꉻ
		template <>
		shared_ptr<Collision> AddComponent<Collision>(){
			auto Ptr = GetCollision();
			if (Ptr){
				return Ptr;
			}
			else{
				//�����Ă��쐬�ł��Ȃ�
				throw BaseException(
					L"���̃R���|�[�l���g�͍쐬�ł��܂���B",
					L"Collision",
					L"GameObject::AddComponent<Collision>()"
					);
			}
		}
		//DynamicCollision
		//Collision�̔h���N���X��Collision�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollision(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//Collision�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Collision>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Collision�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicCollision<T>()"
					);
			}
			SetCollision(newPtr);
			return newPtr;
		}

		//CollisionSphere���ꉻ
		template <>
		shared_ptr<CollisionSphere> AddComponent<CollisionSphere>(){
			auto Ptr = GetCollisionSphere();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto CollisionSpherePtr = Object::CreateObject<CollisionSphere>(GetThis<GameObject>());
				SetCollisionSphere(CollisionSpherePtr);
				return CollisionSpherePtr;
			}
		}
		//DynamicCollisionSphere
		//CollisionSphere�̔h���N���X��Collision�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionSphere(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//CollisionSphere�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<CollisionSphere>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��CollisionSphere�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionSphere<T>()"
					);
			}
			SetCollisionSphere(newPtr);
			return newPtr;
		}

		//CollisionCapsule���ꉻ
		template <>
		shared_ptr<CollisionCapsule> AddComponent<CollisionCapsule>(){
			auto Ptr = GetCollisionCapsule();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto CollisionCapsulePtr = Object::CreateObject<CollisionCapsule>(GetThis<GameObject>());
				SetCollisionCapsule(CollisionCapsulePtr);
				return CollisionCapsulePtr;
			}
		}
		//DynamicCollisionCapsule
		//CollisionCapsule�̔h���N���X��Collision�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionCapsule(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//CollisionCapsule�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<CollisionCapsule>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��CollisionCapsule�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionCapsule<T>()"
					);
			}
			SetCollisionCapsule(newPtr);
			return newPtr;
		}



		//CollisionObb���ꉻ
		template <>
		shared_ptr<CollisionObb> AddComponent<CollisionObb>(){
			auto Ptr = GetCollisionObb();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto CollisionObbPtr = Object::CreateObject<CollisionObb>(GetThis<GameObject>());
				SetCollisionObb(CollisionObbPtr);
				return CollisionObbPtr;
			}
		}
		//DynamicCollisionObb
		//CollisionObb�̔h���N���X��Collision�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionObb(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//CollisionObb�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<CollisionObb>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��CollisionObb�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionObb<T>()"
					);
			}
			SetCollisionObb(newPtr);
			return newPtr;
		}
		//Transform���ꉻ
		template <>
		shared_ptr<Transform> AddComponent<Transform>(){
			auto Ptr = GetTransform();
			if (Ptr){
				return Ptr;
			}
			else{
				//������ΐV���ɐ��삷��
				auto TransformPtr = Object::CreateObject<Transform>(GetThis<GameObject>());
				SetTransform(TransformPtr);
				return TransformPtr;
			}
		}
		//DynamicTransform
		//Transform�̔h���N���X��Transform�ɃZ�b�g����
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicTransform(Ts&&... params){
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = Object::CreateObject<T>(GetThis<GameObject>(), params...);
			//Transform�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Transform>(newPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Transform�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::AddDynamicTransform<T>()"
					);
			}
			SetTransform(newPtr);
			return newPtr;
		}


		//���łɂ���R���|�[�l���g�̃Z�b�g
		template <typename T>
		void SetComponent(const shared_ptr<T>& SetPtr){
			AddMakedComponent(type_index(typeid(T)), SetPtr);
		}
		//Rigidbody���ꉻ
		template <>
		void SetComponent<Rigidbody>(const shared_ptr<Rigidbody>& SetPtr){
			SetRigidbody(SetPtr);
		}
		//DynamicRigidbody
		//Rigidbody�̔h���N���X��Rigidbody�ɃZ�b�g����
		template <typename T>
		void SetDynamicRigidbody(const shared_ptr<T>& SetPtr){
			//Rigidbody�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Rigidbody>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Rigidbody�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicRigidbody<T>()"
					);
			}
			SetRigidbody(SetPtr);
		}
		//Gravity���ꉻ
		template <>
		void SetComponent<Gravity>(const shared_ptr<Gravity>& SetPtr){
			SetGravity(SetPtr);
		}
		//DynamicGravity
		//Gravity�̔h���N���X��Gravity�ɃZ�b�g����
		template <typename T>
		void SetDynamicGravity(const shared_ptr<T>& SetPtr){
			//Gravity�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Gravity>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Gravity�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicGravity<T>()"
					);
			}
			SetGravity(SetPtr);
		}


		//DynamicCollision
		//Collision�̔h���N���X��Collision�ɃZ�b�g����
		template <typename T>
		void SetDynamicCollision(const shared_ptr<T>& SetPtr){
			//Collision�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Collision>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Collision�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicCollision<T>()"
					);
			}
			SetCollision(SetPtr);
		}

		//CollisionSphere���ꉻ
		template <>
		void SetComponent<CollisionSphere>(const shared_ptr<CollisionSphere>& SetPtr){
			SetCollisionSphere(SetPtr);
		}
		//DynamicCollisionSphere
		//CollisionSphere�̔h���N���X��Collision�ɃZ�b�g����
		template <typename T>
		void SetDynamicCollisionSphere(const shared_ptr<T>& SetPtr){
			//CollisionSphere�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<CollisionSphere>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��CollisionSphere�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicCollisionSphere<T>()"
					);
			}
			SetCollisionSphere(SetPtr);
		}
		//CollisionObb���ꉻ
		template <>
		void SetComponent<CollisionObb>(const shared_ptr<CollisionObb>& SetPtr){
			SetCollisionObb(SetPtr);
		}
		//DynamicCollisionObb
		//CollisionObb�̔h���N���X��Collision�ɃZ�b�g����
		template <typename T>
		void SetDynamicCollisionObb(const shared_ptr<T>& SetPtr){
			//CollisionObb�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<CollisionObb>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��CollisionObb�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicCollisionObb<T>()"
					);
			}
			SetCollisionObb(SetPtr);
		}
		//Transform���ꉻ
		template <>
		void SetComponent<Transform>(const shared_ptr<Transform>& SetPtr){
			SetTransform(SetPtr);
		}
		//DynamicTransform
		//Transform�̔h���N���X��Transform�ɃZ�b�g����
		template <typename T>
		void SetDynamicTransform(const shared_ptr<T>& SetPtr){
			//Transform�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Transform>(SetPtr);
			if (!RetPtr){
				//�L���X�g�ł��Ȃ�
				throw BaseMBException(
					"���̃R���|�[�l���g��Transform�ɃL���X�g�ł��܂���B",
					typeid(T).name(),
					"GameObject::SetDynamicTransform<T>()"
					);
			}
			SetTransform(SetPtr);
		}
		//�R���|�[�l���g���폜
		template <typename T>
		void RemoveComponent(){
			RemoveTgtComponent(type_index(typeid(T)));
		}
		//Rigidbody���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <>
		void RemoveComponent<Rigidbody>(){
			SetRigidbody(nullptr);
		}
		//DynamicRigidbody���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <typename T>
		void RemoveDynamicRigidbody(bool ExceptionActive = true){
			//�w��̌^�ɃL���X�g�ł��Ȃ���Η�O���o�邱�Ƃ�����
			auto Ptr = GetDynamicRigidbody<T>(ExceptionActive);
			//��O���o�Ȃ����null������
			SetRigidbody(nullptr);
		}
		//Gravity���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <>
		void RemoveComponent<Gravity>(){
			SetGravity(nullptr);
		}
		//DynamicGravity���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <typename T>
		void RemoveDynamicGravity(bool ExceptionActive = true){
			//�w��̌^�ɃL���X�g�ł��Ȃ���Η�O���o�邱�Ƃ�����
			auto Ptr = GetDynamicGravity<T>(ExceptionActive);
			//��O���o�Ȃ����null������
			SetGravity(nullptr);
		}
		//DynamicCollision���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <typename T>
		void RemoveDynamicCollision(bool ExceptionActive = true){
			//�w��̌^�ɃL���X�g�ł��Ȃ���Η�O���o�邱�Ƃ�����
			auto Ptr = GetDynamicCollision<T>(ExceptionActive);
			//��O���o�Ȃ����null������
			SetCollision(nullptr);
		}
		//CollisionSphere���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <>
		void RemoveComponent<CollisionSphere>(){
			SetCollisionSphere(nullptr);
		}
		//DynamicCollisionSphere���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <typename T>
		void RemoveDynamicCollisionSphere(bool ExceptionActive = true){
			//�w��̌^�ɃL���X�g�ł��Ȃ���Η�O���o�邱�Ƃ�����
			auto Ptr = GetDynamicCollisionSphere<T>(ExceptionActive);
			//��O���o�Ȃ����null������
			SetCollisionSphere(nullptr);
		}
		//CollisionObb���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <>
		void RemoveComponent<CollisionObb>(){
			SetCollisionObb(nullptr);
		}
		//DynamicCollisionObb���ꉻ
		//�폜�ł͂Ȃ��Anull������
		template <typename T>
		void RemoveDynamicCollisionObb(bool ExceptionActive = true){
			//�w��̌^�ɃL���X�g�ł��Ȃ���Η�O���o�邱�Ƃ�����
			auto Ptr = GetDynamicCollisionObb<T>(ExceptionActive);
			//��O���o�Ȃ����null������
			SetCollisionObb(nullptr);
		}
		//RemoveComponent��Transform���ꉻ�͗p�ӂ��Ȃ�
		//����Transform���폜����悤�Ƃ��Ă�
		//RemoveComponent���Q�Ƃ��郊�X�g�ɂ�
		//Transform�͑��݂��Ȃ��̂ň��S�ł���

		//�R���|�[�l���g����
		void ComponentUpdate();
		void ComponentUpdate2();
		void DrawShadowmap();
		void ComponentDraw();

		//���z�֐�
		virtual void PreCreate()override;
		virtual void Update(){}
		virtual void Update2(){}
		virtual void Update3(){}
		virtual void PreDraw(){}
		virtual void Draw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup: public Object;
	//	�p�r: �Q�[���I�u�W�F�N�g��weak_ptr���O���[�v����������
	//--------------------------------------------------------------------------------------
	class GameObjectGroup : public Object{
	public:
		GameObjectGroup();
		virtual ~GameObjectGroup();
		//�A�N�Z�T
		const vector< weak_ptr<GameObject> >& GetGroupVector() const;
		shared_ptr<GameObject> at(size_t index);
		size_t size() const;
		//����
		void IntoGroup(const shared_ptr<GameObject>& Obj);
		void AllClear();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	struct ParticleSprite;
	//	�p�r: �p�[�e�B�N�����́A1�̃X�v���C�g������킷�\����
	//--------------------------------------------------------------------------------------
	struct ParticleSprite{
		bool m_Active;				//�A�N�e�B�u���ǂ���
		Color4 m_Color;				//�F�i����������ꍇ��a��0�ɂ���j
		Vector2 m_LocalScale;		//���[�J���X�P�[�����O�iZ��1.0�Œ�j		
		Quaternion m_LocalQt;		//���[�J����]�B�蓮�̂Ƃ��̂ݎg�p
		Vector3 m_LocalPos;			//���[�J���ʒu
		Vector2 m_UVLeftTop;		//UV�̍���
		Vector2 m_UVRightBottom;	//UV�̉E��
		Vector3 m_Velocity;			//���x(�K�v�ɉ����Ďg�p)
		ParticleSprite() :
			m_Active(true),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_LocalScale(1.0f, 1.0f),
			m_LocalQt(),
			m_LocalPos(0, 0, 0),
			m_UVLeftTop(0, 0),
			m_UVRightBottom(1.0f, 1.0f),
			m_Velocity(0, 0, 0)
		{}
		void Reflesh(){
			m_Active = true;
			m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = Vector2(1.0f, 1.0f);
			m_LocalQt.Identity();
			m_LocalPos = Vector3(0, 0, 0);
			m_UVLeftTop = Vector2(0, 0);
			m_UVRightBottom = Vector2(1.0f, 1.0f);
			m_Velocity = Vector3(0, 0, 0);
		}
	};
	class ParticleManager;
	//--------------------------------------------------------------------------------------
	//	class Particle: public Object;
	//	�p�r: �p�[�e�B�N��
	//	*1�̃G�~�b�^�[�������A������ParticleSprite��ێ�����
	//--------------------------------------------------------------------------------------
	class Particle : public Object{
	public:
		enum DrawOption{
			Billboard,	//�r���{�[�h
			Faceing,	//�t�F�C�V���O
			FaceingY,	//�t�F�C�V���O(Y���̂�)
			Normal		//�ʏ�(�蓮)
		};
		explicit Particle(size_t Count,DrawOption Option = DrawOption::Billboard);
		virtual ~Particle();
		//�A�N�Z�T
		DrawOption GetDrawOption()const;
		void SetDrawOption(DrawOption Option);

		bool IsActive() const;
		bool IsAllActive() const;
		void SetAllActive();
		void SetAllNoActive();

		void Reflesh(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);

		vector<ParticleSprite>& GetParticleSpriteVec() const;

		const Vector3& GetEmitterPos() const;
		void SetEmitterPos(const Vector3& Pos);

		float GetTotalTime() const;
		void SetTotalTime(float f);
		void AddTotalTime(float f);

		float GetMaxTime() const;
		void SetMaxTime(float f);


		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);

		//����
		virtual void Draw(const shared_ptr<ParticleManager>& Manager);

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	�p�r: �}���`�G�t�F�N�g 
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject{
	public:
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		virtual ~MultiParticle();
		//�A�N�Z�T
		vector< shared_ptr<Particle> >& GetParticleVec() const;
		//����
		shared_ptr<Particle> InsertParticle(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	�p�r: �p�[�e�B�N���}�l�[�W��
	//--------------------------------------------------------------------------------------
	class ParticleManager : public GameObject{
		void DrawSub(const shared_ptr<TextureResource>& TextureRes, UINT StartIndex, UINT DrawCount);
	public:
		//�\�z�Ə���
		explicit ParticleManager(const shared_ptr<Stage>& StagePtr);
		virtual ~ParticleManager();
		//������
		virtual void Create() override;

		//�A�N�Z�T
		bool GetZBufferUse() const;
		bool IsZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);


		bool GetSamplerWrap() const;
		bool IsSamplerWrap() const;
		void SetSamplerWrap(bool b);


		//����
		void AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
			const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);

		virtual void Update() override{}
		virtual void Draw()override;
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class InputTextManager : public GameObject;
	//	�p�r: ���̓e�L�X�g�}�l�[�W��
	//--------------------------------------------------------------------------------------
	class InputTextManager : public GameObject{
	public:
		//�\�z�Ə���
		explicit InputTextManager(const shared_ptr<Stage>& StagePtr);
		virtual ~InputTextManager();
		//������
		virtual void Create() override;
		//�A�N�Z�T
		shared_ptr<StringSprite> GetFocusInputString() const;
		void SetFocusInputString(const shared_ptr<StringSprite>& Ptr);

		//����
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void Update() override{}
		virtual void Draw()override{}
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Stage: public GameObject;
	//	�p�r: �X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class Stage : public GameObject{
		//�v���C�x�[�g�T�u�֐�
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		shared_ptr<GameObject> GetSharedGameObjectEx(const wstring& Key,bool ExceptionActive) const;
		void DrawBackColor();
		void SetParentStage(const shared_ptr<Stage>& ParentStage);
		void AddChileStageBase(const shared_ptr<Stage>& ChildStage);
	protected:
		//�\�z�Ɣj��
		Stage();
		virtual ~Stage();
	public:
		//�A�N�Z�T
		shared_ptr<ShadowMapRenderTarget> GetShadowMapRenderTarget() const;
		shared_ptr<DefaultRenderTarget> GetDefaultRenderTarget() const;
		shared_ptr<RenderState> GetRenderState() const;
		shared_ptr<ParticleManager> GetParticleManager() const;
		shared_ptr<InputTextManager> GetInputTextManager() const;

		vector< shared_ptr<GameObject> >& GetGameObjectVec();
		void SetGameObjectVec(vector< shared_ptr<GameObject> >& vec);
		size_t GetDrawViewIndex() const;
		void SetDrawViewIndex(size_t i);

		vector< shared_ptr<Stage> >& GetChileStageVec();
		template<typename T>
		shared_ptr<Stage> AddChileStage(){
			auto Ptr = Object::CreateObject<T>();
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr){
				throw BaseMBException(
					"�ȉ���Stage�Ɍ^�L���X�g�ł��܂���B",
					typeid(T).name(),
					"Stage::AddChileStage<T>()"
					);
			}
			AddChileStageBase(Ptr);
			return Ptr;
		}

		shared_ptr<Stage> GetParentStage() const;

		//�T�[�r�X�A�N�Z�T
		const D3D11_VIEWPORT& GetViewPort(size_t Index) const;
		const D3D11_VIEWPORT& GetTargetViewPort() const;
		const D3D11_VIEWPORT* GetViewPortRealPtr(size_t Index) const;
		const D3D11_VIEWPORT* GetTargetViewPortRealPtr() const;
		const Color4& GetBackColor(size_t Index) const;
		const Color4& GetTargetBackColor() const;
		shared_ptr<Camera> GetCamera(size_t Index) const;
		shared_ptr<Camera> GetTargetCamera() const;
		shared_ptr<MultiLight> GetMultiLight(size_t Index)const;
		shared_ptr<MultiLight> GetTargetMultiLight() const;
		shared_ptr<Light> GetLight(size_t ViewIndex, size_t LightIndex) const;
		shared_ptr<Light> GetTargetLight(size_t LightIndex) const;

		//����

		//GameObject�̐V�K�ǉ�
		template<typename T,typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params){
			try{
				auto Ptr = Object::CreateObject<T>(GetThis<Stage>(),params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...){
				throw;
			}
		}
		//���łɑ��݂���GemeObject�̒ǉ�
		shared_ptr<GameObject> AddGameObject(const shared_ptr<GameObject>& Obj);

		//�ǉ��҂��ɂȂ��Ă�I�u�W�F�N�g��ǉ�����
		void SetWaitToObjectVec();

		template<typename T>
		shared_ptr<T> GetSharedGameObject(const wstring& Key, bool ExceptionActive = true)const{
			shared_ptr<T> Ptr = dynamic_pointer_cast<T>(GetSharedGameObjectEx(Key, ExceptionActive));
			if (Ptr){
				return Ptr;
			}
			else{
				//�L���X�g�ł��Ȃ�
				if (ExceptionActive){
					//��O����
					wstring keyerr = Key;
					throw BaseException(
						L"�I�u�W�F�N�g��T�^�ɃL���X�g�ł��܂���",
						keyerr,
						L"Stage::GetSharedGameObject()"
						);
				}
			}
			return nullptr;
		}
		shared_ptr<Object> GetSharedObject(const wstring& Key, bool ExceptionActive = true)const;
		void SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr);

		shared_ptr<GameObjectGroup> CreateSharedObjectGroup(const wstring& Key);
		shared_ptr<GameObjectGroup> GetSharedObjectGroup(const wstring& Key,bool ExceptionActive = true)const;
		template<typename T>
		shared_ptr<T> GetSharedObjectGroup(const wstring& Key, bool ExceptionActive = true)const{
			auto RetPtr = dynamic_pointer_cast<T>(GetSharedObjectGroup(Key, ExceptionActive));
			if (RetPtr){
				return RetPtr;
			}
			else{
				if (ExceptionActive){
					//��O����
					wstring keyerr = Key;
					throw BaseException(
						L"�w��̂��[�̃O���[�v��T�^�ɕϊ��ł��܂���",
						keyerr,
						L"Stage::GetSharedObjectGroup<T>()"
						);
				}
			}
			return nullptr;
		}


		void SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr);

		//�f�t�H���g�̃����_�����O�^�[�Q�b�g�ނ���������
		void CreateDefaultRenderTargets();

		//���z�֐�
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void PreCreate()override;

		//�X�e�[�W���̍X�V�i�V�[�������΂��j
		virtual void UpdateStage();
		//�X�e�[�W���̕`��i�V�[�������΂��j
		virtual void DrawStage();


		//���z�֐�
		//�X�V,�`��͂����ł͎������Ȃ�
		//�h���N���XGameStage���ł͎����ł���
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	�p�r: �}���`�I�[�f�B�I
	// ���R���|�[�l���g�ł͂Ȃ��̂ŃV�[���Ɏ������邱�Ƃ��ł���
	//--------------------------------------------------------------------------------------
	class MultiAudioObject : public Object{
	public:
		//�\�z�Ɣj��
		MultiAudioObject();
		virtual ~MultiAudioObject();
		//�A�N�Z�T
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey, bool ExceptionActive = true) const;
		//����
		shared_ptr<AudioResource> AddAudioResource(const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SceneBase: public Object;
	//	�p�r: �V�[���x�[�X�N���X
	//--------------------------------------------------------------------------------------
	class SceneBase : public Object
	{
	public:
		//�\�z�Ɣj��
		SceneBase();
		virtual ~SceneBase();
		//�A�N�Z�b�T
		shared_ptr<Stage> GetActiveStage() const;
		void SetActiveStage(const shared_ptr<Stage>& stage);
		shared_ptr<EventDispatcher> GetEventDispatcher() const;


		//����
		//�A�N�e�B�u�ȃX�e�[�W��ݒ肵�ď���������
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params){
			auto Ptr = Object::CreateObject<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr){
				throw BaseMBException(
					"�ȉ���Stage�Ɍ^�L���X�g�ł��܂���B",
					typeid(T).name(),
					"SceneBase::ResetActiveStage<T>()"
					);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void PreCreate()override;
		//�V�[����ω�������
		virtual void Update();
		//�V�[����`��
		virtual void Draw();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}

//endof  basedx11
