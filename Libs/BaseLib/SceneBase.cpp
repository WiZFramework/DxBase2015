
#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct GameObject::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct GameObject::Impl{
		bool m_UpdateActive;	//update���邩�ǂ���
		bool m_DrawActive;		//Draw���邩�ǂ���
		bool m_AlphaActive;		//�������ǂ���
		//���������������邩�ǂ����im_AlphaActive��true�łȂ���ΈӖ����Ȃ��j
		bool m_AlphaExActive;

		bool m_SpriteDraw;
		int m_DrawLayer;	//�`�惌�C���[

		weak_ptr<Stage> m_Stage;	//�����X�e�[�W
		map<type_index, shared_ptr<Component> > m_CompMap;
		shared_ptr<Rigidbody> m_Rigidbody;	//Rigidbody�͕ʂɂ���
		shared_ptr<Gravity> m_Gravity;	//Gravity�͕ʂɂ���
		shared_ptr<Transform> m_Transform;	//Transform���ʂɂ���
		shared_ptr<Collision> m_Collision;	//Collision���ʂɂ���

		list<type_index> m_CompOrder;	//�R���|�[�l���g���s����



		Impl(const shared_ptr<Stage>& StagePtr) :
			m_UpdateActive(true),
			m_DrawActive(true),
			m_AlphaActive(false),
			m_AlphaExActive(false),
			m_SpriteDraw(false),
			m_DrawLayer(0),
			m_Stage(StagePtr)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObject :public Object, public ShapeInterface;
	//	�p�r: �Q�[���z�u�I�u�W�F�N�g�e�N���X
	//--------------------------------------------------------------------------------------
	//private�����o
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const{
		auto it = pImpl->m_CompMap.find(TypeIndex);
		if (it != pImpl->m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}
	shared_ptr<Rigidbody> GameObject::GetRigidbody()const{
		return pImpl->m_Rigidbody;
	}
	shared_ptr<Gravity> GameObject::GetGravity()const{
		return pImpl->m_Gravity;
	}

	shared_ptr<Transform> GameObject::GetTransform()const{
		return pImpl->m_Transform;
	}
	shared_ptr<Collision> GameObject::GetCollision()const{
		return pImpl->m_Collision;
	}
	shared_ptr<CollisionSphere> GameObject::GetCollisionSphere()const{
		return dynamic_pointer_cast<CollisionSphere>(pImpl->m_Collision);
	}

	shared_ptr<CollisionCapsule> GameObject::GetCollisionCapsule()const{
		return dynamic_pointer_cast<CollisionCapsule>(pImpl->m_Collision);
	}

	shared_ptr<CollisionObb> GameObject::GetCollisionObb()const{
		return dynamic_pointer_cast<CollisionObb>(pImpl->m_Collision);
	}


	void GameObject::SetRigidbody(const shared_ptr<Rigidbody>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Rigidbody = Ptr;
	}
	void GameObject::SetGravity(const shared_ptr<Gravity>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Gravity = Ptr;
	}
	void GameObject::SetCollision(const shared_ptr<Collision>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}
	void GameObject::SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionObb(const shared_ptr<CollisionObb>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetTransform(const shared_ptr<Transform>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Transform = Ptr;
	}
	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr){
		if (!SearchComponent(TypeIndex)){
			//���̃R���|�[�l���g���܂��Ȃ���ΐV�K�o�^
			pImpl->m_CompOrder.push_back(TypeIndex);
		}
		//map�ɒǉ��������͍X�V
		pImpl->m_CompMap[TypeIndex] = Ptr;
		Ptr->AttachGameObject(GetThis<GameObject>());
	}
	map<type_index, shared_ptr<Component> >& GameObject::GetCompoMap()const {
		return pImpl->m_CompMap;
	}

	void GameObject::RemoveTgtComponent(type_index TypeIndex){
		//���ԃ��X�g�����؂��č폜
		auto it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			if (*it == TypeIndex){
				auto it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//�w��̌^�̃R���|�[�l���g����������
					//map�f�[�^���폜
					pImpl->m_CompMap.erase(it2);
				}
				pImpl->m_CompOrder.erase(it);
				return;
			}
			it++;
		}
	}




	//�\�z�Ɣj��

	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		Object(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}
	GameObject::~GameObject(){}
	//�A�N�Z�T
	bool GameObject::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool GameObject::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void GameObject::SetUpdateActive(bool b){ pImpl->m_UpdateActive = b; }

	bool GameObject::IsDrawActive() const { return pImpl->m_DrawActive; }
	bool GameObject::GetDrawActive() const { return pImpl->m_DrawActive; }
	void GameObject::SetDrawActive(bool b){ pImpl->m_DrawActive = b; }

	bool GameObject::IsAlphaActive() const { return pImpl->m_AlphaActive; }
	bool GameObject::GetAlphaActive() const { return pImpl->m_AlphaActive; }
	void GameObject::SetAlphaActive(bool b){ pImpl->m_AlphaActive = b; }

	bool GameObject::IsAlphaExActive() const{
		return pImpl->m_AlphaExActive;
	}
	bool GameObject::GetAlphaExActive() const{
		return pImpl->m_AlphaExActive;
	}
	void GameObject::SetAlphaExActive(bool b){
		pImpl->m_AlphaExActive = b;
	}


	//�X�v���C�g�Ƃ���Draw���邩�ǂ���
	bool  GameObject::IsSpriteDraw() const{
		return pImpl->m_SpriteDraw;

	}
	bool  GameObject::GetSpriteDraw() const{
		return pImpl->m_SpriteDraw;
	}
	void  GameObject::SetSpriteDraw(bool b){
		pImpl->m_SpriteDraw = b;
	}
	//�`�惌�C���[�̎擾�Ɛݒ�
	int GameObject::GetDrawLayer() const{
		return pImpl->m_DrawLayer;
	}
	void  GameObject::SetDrawLayer(int l){
		pImpl->m_DrawLayer = l;
	}

	shared_ptr<Stage> GameObject::GetStage() const {
		if (!pImpl->m_Stage.expired()){
			return pImpl->m_Stage.lock();
		}
		return nullptr;
	}
	void GameObject::SetStage(const shared_ptr<Stage>& stage){ pImpl->m_Stage = stage; }

	void GameObject::PreCreate(){
		//Transform�K�{
		AddComponent<Transform>();
	}

	void GameObject::ComponentUpdate(){
		//Transform���Ȃ���Η�O
		auto Tptr = GetComponent<Transform>();
		auto RigidPtr = GetComponent<Rigidbody>(false);
		auto GravityPtr = GetComponent<Gravity>(false);
		if (RigidPtr){
			//Rigidbody������΃t�H�[�X��������
			RigidPtr->SetForce(0, 0, 0);
		}
		//�}�b�v�����؂���Update
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (it2->second->IsUpdateActive()){
					it2->second->Update();
				}
			}
			it++;
		}
		if (RigidPtr && RigidPtr->IsUpdateActive()){
			//Rigidbody�������Update()
			RigidPtr->Update();
		}
		if (GravityPtr && GravityPtr->IsUpdateActive()){
			//GravityPtr�������Update()
			GravityPtr->Update();
		}
		//Transform��Update
		if (Tptr->IsUpdateActive()){
			Tptr->Update();
		}
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsUpdateActive()){
			//Collision�������Update()
			CollisionPtr->Update();
		}
	}
	void GameObject::ComponentUpdate2(){
		//Transform���Ȃ���Η�O
		auto Tptr = GetComponent<Transform>();
		//�}�b�v�����؂���Update
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (it2->second->IsUpdate2Active()){
					it2->second->Update2();
				}
			}
			it++;
		}
		auto RigidPtr = GetComponent<Rigidbody>(false);
		if (RigidPtr && RigidPtr->IsUpdate2Active()){
			//Rigidbody�������Update2()
			RigidPtr->Update2();
		}
		auto GravityPtr = GetComponent<Gravity>(false);
		if (GravityPtr && GravityPtr->IsUpdate2Active()){
			//GravityPtr�������Update2()
			GravityPtr->Update2();
		}

		//Transform��Update
		if (Tptr->IsUpdate2Active()){
			Tptr->Update2();
		}
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsUpdate2Active()){
			//Collision�������Update2()
			CollisionPtr->Update2();
		}
	}

	void GameObject::DrawShadowmap(){
		auto shadowptr = GetDynamicComponent<Shadowmap>(false);
		if (shadowptr){
			shadowptr->Draw();
		}
	}


	void GameObject::ComponentDraw(){
		//Transform���Ȃ���Η�O
		auto Tptr = GetComponent<Transform>();
		//�}�b�v�����؂���Draw
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (!dynamic_pointer_cast<Shadowmap>(it2->second)){
					//�V���h�E�}�b�v�ȊO�Ȃ���s
					//���̃R���|�[�l���g�̎q�R���|�[�l���g�̕`��
					if (it2->second->IsDrawActive()){
						it2->second->Draw();
					}
				}
			}
			it++;
		}
		auto RigidPtr = GetComponent<Rigidbody>(false);
		//Rigidbody��Draw
		//Rigidbody�̔h���N���X�΍�
		if (RigidPtr && RigidPtr->IsDrawActive()){
			//Rigidbody�������Draw()
			RigidPtr->Draw();
		}
		//Transform��Draw
		//Transform�̔h���N���X�΍�
		if (Tptr->IsDrawActive()){
			Tptr->Draw();
		}
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsDrawActive()){
			//Collision�������Draw()
			CollisionPtr->Draw();
		}
	}

	void GameObject::Draw(){
		//�R���|�[�l���g�`��
		//�h���N���X�ő��d��`����ꍇ��
		//�R���|�[�l���g�`�悷��ꍇ��
		//GameObject::Draw()���Ăяo��
		ComponentDraw();
	}



	//--------------------------------------------------------------------------------------
	//	struct GameObjectGroup::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct GameObjectGroup::Impl{
		vector< weak_ptr<GameObject> > m_Group;
		Impl(){}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup;
	//	�p�r: �Q�[���I�u�W�F�N�g��weak_ptr���O���[�v����������
	//--------------------------------------------------------------------------------------
	GameObjectGroup::GameObjectGroup():
		Object(),
		pImpl(new Impl())
	{}
	GameObjectGroup::~GameObjectGroup(){}
	//�A�N�Z�T
	const vector< weak_ptr<GameObject> >& GameObjectGroup::GetGroupVector() const{
		return pImpl->m_Group;
	}
	shared_ptr<GameObject> GameObjectGroup::at(size_t index){
		if (index >= pImpl->m_Group.size()){
			wstring msg = Util::UintToWStr(index);
			msg += L" >= ";
			msg += Util::UintToWStr(pImpl->m_Group.size());
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				msg,
				L"GameObjectGroup::at()"
				);
		}
		if (pImpl->m_Group.at(index).expired()){
			wstring msg = Util::UintToWStr(index);
			throw BaseException(
				L"���̃C���f�b�N�X�̃I�u�W�F�N�g�͖����ł��B",
				msg,
				L"GameObjectGroup::at()"
				);
		}
		return pImpl->m_Group.at(index).lock();
	}
	size_t GameObjectGroup::size() const{
		return pImpl->m_Group.size();
	}
	//����
	void GameObjectGroup::IntoGroup(const shared_ptr<GameObject>& Obj){
		pImpl->m_Group.push_back(Obj);
	}

	void GameObjectGroup::AllClear(){
		pImpl->m_Group.clear();
	}


	//--------------------------------------------------------------------------------------
	//	struct Particle::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Particle::Impl{
		DrawOption m_DrawOption;		//�\���I�v�V����
		vector<ParticleSprite> m_ParticleSpriteVec;	//�ۑ����Ă����X�v���C�g�̔z��
		Vector3 m_EmitterPos;			//�G�~�b�^�[�ʒu
		float m_TotalTime;				//�^�C�}�[���䂷��ꍇ�Ɏg�p����ϐ�
		float m_MaxTime;				//���̃p�[�e�B�N���W���̂̕\������
		weak_ptr<TextureResource> m_TextureResource;	//�e�N�X�`��
		Impl(size_t Count, DrawOption Option):
			m_DrawOption(Option),
			m_ParticleSpriteVec(Count),
			m_EmitterPos(0,0,0),
			m_TotalTime(0),
			m_MaxTime(0)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Particle;
	//	�p�r: �p�[�e�B�N��
	//	*1�̃G�~�b�^�[�������A������ParticleSprite��ێ�����
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, DrawOption Option) :
		Object(),
		pImpl(new Impl(Count,Option))
	{}
	Particle::~Particle(){}

	Particle::DrawOption Particle::GetDrawOption()const{
		return pImpl->m_DrawOption;
	}
	void Particle::SetDrawOption(DrawOption Option){
		pImpl->m_DrawOption = Option;
	}

	const Vector3& Particle::GetEmitterPos() const{
		return pImpl->m_EmitterPos;
	}
	void Particle::SetEmitterPos(const Vector3& Pos){
		pImpl->m_EmitterPos = Pos;
	}

	float Particle::GetTotalTime() const{
		return pImpl->m_TotalTime;
	}
	void Particle::SetTotalTime(float f){
		pImpl->m_TotalTime = f;
	}
	void Particle::AddTotalTime(float f){
		pImpl->m_TotalTime += f;
	}

	float Particle::GetMaxTime() const{
		return pImpl->m_MaxTime;
	}
	void Particle::SetMaxTime(float f){
		pImpl->m_MaxTime = f;
	}



	bool Particle::IsActive() const{
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (Psp.m_Active){
				//1�ł��A�N�e�B�u�������true
				return true;
			}
		}
		return false;
	}
	bool Particle::IsAllActive() const{
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (!Psp.m_Active){
				//1�ł���A�N�e�B�u�������false
				return false;
			}
		}
		return true;
	}
	void Particle::SetAllActive(){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.m_Active = true;
		}
	}
	void Particle::SetAllNoActive(){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.m_Active = false;
		}
	}

	void Particle::Reflesh(size_t Count, Particle::DrawOption Option){
		pImpl->m_DrawOption = Option;
		pImpl->m_EmitterPos = Vector3(0, 0, 0);
		pImpl->m_TotalTime = 0;
		pImpl->m_MaxTime = 0;
		pImpl->m_ParticleSpriteVec.clear();
		pImpl->m_ParticleSpriteVec.resize(Count);
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.Reflesh();
		}
	}

	vector<ParticleSprite>& Particle::GetParticleSpriteVec() const{
		return pImpl->m_ParticleSpriteVec;
	}

	shared_ptr<TextureResource> Particle::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"�e�N�X�`�����\�[�X��������܂���",
					L"if (pImpl->m_Texture.expired())",
					L"Particle::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void Particle::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�e�N�X�`���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"Particle::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Particle::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void Particle::Draw(const shared_ptr<ParticleManager>& Manager){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (Psp.m_Active && !pImpl->m_TextureResource.expired()){
				Manager->AddParticle(Psp, GetDrawOption(),
					GetEmitterPos(), pImpl->m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiParticle::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct MultiParticle::Impl{
		vector< shared_ptr<Particle> > m_ParticleVec;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	�p�r: �}���`�G�t�F�N�g 
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	MultiParticle::~MultiParticle(){}

	vector< shared_ptr<Particle> >& MultiParticle::GetParticleVec() const{
		return pImpl->m_ParticleVec;
	}


	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, Particle::DrawOption Option){
		for (size_t i = 0; i < pImpl->m_ParticleVec.size(); i++){
			//������A�N�e�B�u�̃p�[�e�B�N��������Ώ��������ă��^�[��
			if (!pImpl->m_ParticleVec[i]->IsActive()){
				pImpl->m_ParticleVec[i]->Reflesh(Count, Option);
				return pImpl->m_ParticleVec[i];
			}
		}
		//�V�����p�[�e�B�N����ǉ�
		shared_ptr<Particle> ParticlePtr = Object::CreateObject<Particle>(Count, Option);
		pImpl->m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}

	void MultiParticle::Update(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()){
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()){
				if (rParticleSprite.m_Active){
					//�ړ����x�ɏ]���Ĉړ�������
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->GetTotalTime() >= ParticlePtr->GetMaxTime()){
						//�������ԂɂȂ�����
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::Draw(){
		if (pImpl->m_ParticleVec.size() > 0){
			for (auto Ptr : pImpl->m_ParticleVec){
				if (Ptr->IsActive()){
					Ptr->Draw(GetStage()->GetParticleManager());
				}
			}
		}
	}





	//Draw�����̂��߂̍\����
	struct ParticleSpriteForDraw{
		float m_ToCaneraLength;						//�p�[�e�B�N���̃J�����܂ł̋���
		VertexPositionColorTexture m_Vertex[4];	//���[�J���ϊ��ς݂̒��_�i�`��ɒ��ڎg�p�j
		shared_ptr<TextureResource> m_TextureRes;					//�e�N�X�`��
		ParticleSpriteForDraw() :
			m_ToCaneraLength(0)
		{}
	};

	//���ԓI�ȍ\����
	struct ParticleDrawSetter{
		shared_ptr<TextureResource> m_TextureRes;			//�ݒ肷��e�N�X�`��
		UINT StartIndex;			//�`��J�n�C���f�b�N�X
		UINT DrawCount;				//�`�搔
	};


	//--------------------------------------------------------------------------------------
	//	struct ParticleManager::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ParticleManager::Impl{
		size_t m_ParticleCount;			//���݊Ǘ����Ă���p�[�e�B�N���̐�
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
		bool m_ZBufferUse;				//Z�o�b�t�@���g�p���邩�ǂ���
		bool m_AlphaBlendSrcOne;	//����������SRC_ONE�ݒ�
		bool m_SamplerWrap;				//�T���v���[�̃��b�s���O���邩�ǂ���

		//Draw�����̂��߂̍\���̂̔z��
		vector<ParticleSpriteForDraw> m_ParticleSpriteForDrawVec;
		Impl():
			m_ParticleCount{ 256 },
			m_NumVertices{ 0 },
			m_NumIndicis{ 0 },
			m_ZBufferUse{ true },
			m_AlphaBlendSrcOne(false),
			m_SamplerWrap{ false }
		{}
		~Impl(){}
		//���_�o�b�t�@�̍쐬
		void CreateParticleBuffers();
	};
	void ParticleManager::Impl::CreateParticleBuffers(){
		try{
			//���_���쐬���邽�߂̔z��
			vector<VertexPositionColorTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			XMFLOAT4 col(1.0f, 1.0f, 1.0f, 0.0f);
			for (size_t i = 0; i < m_ParticleCount; i++){

				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(-0.5f, 0.5f, 0), col, XMFLOAT2(0.0f, 0.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(0.5f, 0.5f, 0), col, XMFLOAT2(1.0f, 0.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(-0.5f, -0.5f, 0), col, XMFLOAT2(0.0f, 1.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(0.5f, -0.5f, 0), col, XMFLOAT2(1.0f, 1.0f)));

				indices.push_back((uint16_t)i * 4 + 0);
				indices.push_back((uint16_t)i * 4 + 1);
				indices.push_back((uint16_t)i * 4 + 2);
				indices.push_back((uint16_t)i * 4 + 1);
				indices.push_back((uint16_t)i * 4 + 3);
				indices.push_back((uint16_t)i * 4 + 2);

			}

			//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//���_���ύX�ł���o�b�t�@�̍쐬
			{
				mutex Mutex;
				std::lock_guard<std::mutex> lock(Mutex);
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, &m_VertexBuffer);
			}
			//���_���̐ݒ�
			m_NumVertices = static_cast<UINT>(vertices.size());
			//�C���f�b�N�X�o�b�t�@�̍쐬
			{
				mutex Mutex;
				std::lock_guard<std::mutex> lock(Mutex);
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, &m_IndexBuffer);
			}
			//�C���f�b�N�X���̐ݒ�
			m_NumIndicis = static_cast<UINT>(indices.size());

		}
		catch (...){
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	�p�r: �p�[�e�B�N���}�l�[�W��
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	ParticleManager::ParticleManager(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	ParticleManager::~ParticleManager(){}

	//������
	void ParticleManager::Create(){
		try{
			//�o�b�t�@�̍쐬
			pImpl->CreateParticleBuffers();
			//���������̂ݎw�肵�Ă���
			SetAlphaActive(true);
		}
		catch (...){
			throw;
		}

	}

	bool ParticleManager::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	bool ParticleManager::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void ParticleManager::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool ParticleManager::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool ParticleManager::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void ParticleManager::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}


	bool ParticleManager::GetSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	bool ParticleManager::IsSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	void ParticleManager::SetSamplerWrap(bool b){
		pImpl->m_SamplerWrap = b;
	}

	void ParticleManager::AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
		const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes){
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		auto PtrCamera = GetStage()->GetTargetCamera();
		//�J�����̈ʒu
		Vector3 CameraEye = PtrCamera->GetEye();
		Vector3 CameraAt = PtrCamera->GetAt();


		ParticleSpriteForDraw Item;
		Vector3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		Item.m_ToCaneraLength = Vector3EX::Length(CameraEye - WorldPos);
		Item.m_TextureRes = TextureRes;

		Vector3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vector3 Temp;
		Quaternion Qt;
		Matrix4X4 RotMatrix;
		Vector4 dammi(0, 0, 0, 0);
		Vector3 DefUp(0, 1.0f, 0);
		switch (Option){
			case Particle::DrawOption::Billboard:
			{
				Temp = CameraAt - CameraEye;
				Vector2 TempVec2(Temp.x, Temp.z);
				if (Vector2EX::Length(TempVec2) < 0.1f){
					DefUp = Vector3(0, 0, 1.0f);
				}
				Temp.Normalize();
				RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
				RotMatrix.Inverse(&dammi);
				Qt = RotMatrix.QtInMatrix();
				Qt.Normalize();
			}
			break;
			case Particle::DrawOption::Faceing:
			{
				Temp = WorldPos - CameraEye;
				Vector2 TempVec2(Temp.x, Temp.z);
				if (Vector2EX::Length(TempVec2) < 0.1f){
					DefUp = Vector3(0, 0, 1.0f);
				}
				RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
				RotMatrix.Inverse(&dammi);
				Qt = RotMatrix.QtInMatrix();
				Qt.Normalize();
			}
			break;
			case Particle::DrawOption::FaceingY:
				Temp = WorldPos - CameraEye;
				Temp.Normalize();
				Qt.RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
				Qt.Normalize();
			break;
			case Particle::DrawOption::Normal:
				Qt = rParticleSprite.m_LocalQt;
				Qt.Normalize();
			break;
		}

		Matrix4X4 matrix;
		matrix.DefTransformation(
			LocalScale,
			Qt,
			rParticleSprite.m_LocalPos
			);


		Vector3 Pos0(-0.5f, 0.5f, 0);
		Pos0.Transform(matrix);

		Vector3 Pos1(0.5f, 0.5f, 0);
		Pos1.Transform(matrix);

		Vector3 Pos2(-0.5f, -0.5f, 0);
		Pos2.Transform(matrix);

		Vector3 Pos3(0.5f, -0.5f, 0);
		Pos3.Transform(matrix);

		Item.m_Vertex[0].position = Pos0 + EmitterPos;
		Item.m_Vertex[0].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[0].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVLeftTop.x, rParticleSprite.m_UVLeftTop.y);

		Item.m_Vertex[1].position = Pos1 + EmitterPos;
		Item.m_Vertex[1].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[1].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVRightBottom.x, rParticleSprite.m_UVLeftTop.y);

		Item.m_Vertex[2].position = Pos2 + EmitterPos;
		Item.m_Vertex[2].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[2].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVLeftTop.x, rParticleSprite.m_UVRightBottom.y);

		Item.m_Vertex[3].position = Pos3 + EmitterPos;
		Item.m_Vertex[3].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[3].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVRightBottom.x, rParticleSprite.m_UVRightBottom.y);

		pImpl->m_ParticleSpriteForDrawVec.push_back(Item);
	}

	void ParticleManager::DrawSub(const shared_ptr<TextureResource>& TextureRes, UINT StartIndex, UINT DrawCount){
		auto PtrStage = GetStage();
		if (!PtrStage){
			return;
		}
		Matrix4X4 ShaderMatrix;

		//�J�����̈ʒu
		auto PtrCamera = GetStage()->GetTargetCamera();
		Matrix4X4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();
		Matrix4X4 m;
		ShaderMatrix.Identity();
		ShaderMatrix *= View;
		ShaderMatrix *= Proj;


		//�e�N�X�`���͕K�{
		if (TextureRes){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//�X�e�[�^�X�̃|�C���^
			auto RenderStatePtr = PtrStage->GetRenderState();

			//�R���X�^���g�o�b�t�@�̐ݒ�
			SpriteConstantBuffer sb;
			sb.mTransformMatrix = ShaderMatrix;

			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionColorTexture);
			UINT offset = 0;
			//���_�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(pImpl->m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			ID3D11SamplerState* samplerState;
			if (IsSamplerWrap()){
				samplerState = RenderStatePtr->GetLinearWrap();
			}
			else{
				samplerState = RenderStatePtr->GetLinearClamp();
			}
			//�T���v���[��ݒ�
			pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
			//�e�N�X�`����ݒ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
			//�e�N�X�`������̃s�N�Z���V�F�[�_�̐ݒ�
			pID3D11DeviceContext->PSSetShader(PSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//���_�V�F�[�_�̐ݒ�
			pID3D11DeviceContext->VSSetShader(VSSpritePCT::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSSpritePCT::GetPtr()->GetInputLayout());
			if (IsZBufferUse()){
				//�f�v�X�X�e���V���g�p
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//�f�v�X�X�e���V���͎g�p���Ȃ�
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			}
			//�X�e�[�^�X�̐ݒ�
			//�A���t�@�u�����h
			if (IsAlphaActive()){
				if (IsAlphaBlendSrcOne()){
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
				else{
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
			}
			else{
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xFFFFFFFF);
			}
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;

			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			if (IsAlphaActive()){
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//�`��
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
			}
			else{
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
			}
			//��n��
			Dev->InitializeStates(RenderStatePtr);

		}

	}


	void ParticleManager::Draw(){

		if (pImpl->m_ParticleSpriteForDrawVec.size() <= 0){
			return;
		}
		size_t ParticleCount = pImpl->m_ParticleCount;
		while (ParticleCount < pImpl->m_ParticleSpriteForDrawVec.size()){
			ParticleCount *= 2;
			if (ParticleCount > 9000){
				throw BaseException(
					L"����ȏ�p�[�e�B�N���͒ǉ��ł��܂���B",
					L"if(ParticleCount > 9000)",
					L"ParticleManager::Draw()"
					);
			}
		}
		if (ParticleCount > pImpl->m_ParticleCount){
			pImpl->m_ParticleCount = ParticleCount;
			pImpl->CreateParticleBuffers();
		}
		//�J�����ʒu�Ń\�[�g
		auto func = [](ParticleSpriteForDraw& Left, ParticleSpriteForDraw& Right)->bool{
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(pImpl->m_ParticleSpriteForDrawVec.begin(), pImpl->m_ParticleSpriteForDrawVec.end(),func);


		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//�擪�̃e�N�X�`���𓾂�
		auto  NowTexPtr = pImpl->m_ParticleSpriteForDrawVec[0].m_TextureRes;
		size_t DrawCount = 0;
		size_t StartIndex = 0;

		//�`��p�̔z��
		vector<ParticleDrawSetter> ParticleDrawSetterVec;
		ParticleDrawSetter Item;


		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = pImpl->m_VertexBuffer.Get();

		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//���_�̕ύX
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < pImpl->m_ParticleSpriteForDrawVec.size(); i++){
			if (pImpl->m_ParticleSpriteForDrawVec[i].m_TextureRes != NowTexPtr){
				Item.m_TextureRes = NowTexPtr;
				Item.StartIndex = StartIndex;
				Item.DrawCount = DrawCount;
				ParticleDrawSetterVec.push_back(Item);
				NowTexPtr = pImpl->m_ParticleSpriteForDrawVec[i].m_TextureRes;
				DrawCount = 0;
				StartIndex = i;
			}
			vertices[i * 4 + 0] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[0];
			vertices[i * 4 + 1] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[1];
			vertices[i * 4 + 2] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[2];
			vertices[i * 4 + 3] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[3];
			DrawCount++;
		}
		Item.m_TextureRes = NowTexPtr;
		Item.StartIndex = StartIndex;
		Item.DrawCount = DrawCount;
		ParticleDrawSetterVec.push_back(Item);
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

		//�X�V���I�������A�`�揀���p�z����N���A
		pImpl->m_ParticleSpriteForDrawVec.clear();

		//�`��
		for (size_t i = 0; i < ParticleDrawSetterVec.size(); i++){
			DrawSub(
				ParticleDrawSetterVec[i].m_TextureRes,
				ParticleDrawSetterVec[i].StartIndex * 6,
				ParticleDrawSetterVec[i].DrawCount * 6
				);
		}
		ParticleDrawSetterVec.clear();

	}


	//--------------------------------------------------------------------------------------
	//	struct InputTextManager::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct InputTextManager::Impl{
		//�t�H�[�J�X�������Ă���̓X�g�����O
		weak_ptr<StringSprite> m_FocusInputString;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class InputTextManager : public GameObject;
	//	�p�r: ���̓e�L�X�g�}�l�[�W��
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	InputTextManager::InputTextManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	InputTextManager::~InputTextManager(){}

	//������
	void InputTextManager::Create(){
		try{
		}
		catch (...){
			throw;
		}
	}

	//�A�N�Z�T
	shared_ptr<StringSprite> InputTextManager::GetFocusInputString() const{
		if (!pImpl->m_FocusInputString.expired()){
			auto ShPtr = pImpl->m_FocusInputString.lock();
			return ShPtr;
		}
		else{
			return nullptr;
		}
	}
	void InputTextManager::SetFocusInputString(const shared_ptr<StringSprite>& Ptr){
		pImpl->m_FocusInputString = Ptr;
	}


	void InputTextManager::OnKeyDown(WPARAM wParam, LPARAM lParam){
		if (GetFocusInputString()){
			GetFocusInputString()->OnKeyDown(wParam, lParam);
		}
	}

	void InputTextManager::OnChar(WPARAM wParam, LPARAM lParam){
		if (GetFocusInputString()){
			GetFocusInputString()->OnChar(wParam, lParam);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Stage::Impl{
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;	//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//�f�t�H���g�̃����_�����O�^�[�Q�b�g
		shared_ptr<RenderState> m_RenderState;					//�����_�����O�X�e�[�g
		vector< shared_ptr<GameObject> > m_GameObjectVec;	//�I�u�W�F�N�g�̔z��
		shared_ptr<ParticleManager> m_ParticleManager;		//�p�[�e�B�N���}�l�[�W��
		shared_ptr<InputTextManager> m_InputTextManager;	//���̓}�l�[�W��
		//�V�F�A�I�u�W�F�N�g�|�C���^�̃}�b�v
		map<const wstring,weak_ptr<GameObject> > m_SharedMap;
		//�V�F�A�O���[�v�̃|�C���^�̃}�b�v
		map<const wstring, shared_ptr<GameObjectGroup> >  m_SharedGroupMap;
		size_t m_DrawViewIndex;		//����Draw����Ă���r���[�̃C���f�b�N�X

		//�w�ʂ��N���A���邽�߂̃o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		UINT m_NumVertices;				//���_�̐�

		vector< shared_ptr<Stage> > m_ChildStageVec;	//�q���X�e�[�W�̔z��
		weak_ptr<Stage> m_ParentStage;		//�e�X�e�[�W

		//Sprite�������łȂ����𕪗�����z��
		vector< shared_ptr<GameObject> > m_SpriteVec;
		vector< shared_ptr<GameObject> > m_Object3DVec;
		//3D�̓����Ɣ񓧖��𕪗�����z��
		vector< shared_ptr<GameObject> > m_Object3DNormalVec;
		vector< shared_ptr<GameObject> > m_Object3DAlphaVec;

		//�r���ɃI�u�W�F�N�g���ǉ����ꂽ�ꍇ�A�^�[���̊J�n�܂ő҂z��
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;

		Impl() :m_DrawViewIndex(0){}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage;
	//	�p�r: �X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	Stage();
	//	�p�r: �R���X�g���N�^�B�V�[���𐶐�
	//	�߂�l: �Ȃ��B���s�������O��throw
	//--------------------------------------------------------------------------------------
	//�v���C�x�[�g�T�u�֐�
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr){
		//���̃X�e�[�W�̓N���G�C�g��ł���
		if (IsCreated()){
			pImpl->m_WaitAddObjectVec.push_back(Ptr);
		}
		else{
		//�N���G�C�g�O
			pImpl->m_GameObjectVec.push_back(Ptr);
		}
	}

	shared_ptr<GameObject> Stage::GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive)const{
		map<const wstring, weak_ptr<GameObject> >::const_iterator it;
		//�d���L�[�̌���
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()){
			if (it->second.expired()){
				//���łɖ���
				if (ExceptionActive){
					//��O����
					wstring keyerr = Key;
					throw BaseException(
						L"�I�u�W�F�N�g�������ł�",
						keyerr,
						L"Stage::GetSharedGameObject()"
						);
				}
			}
			return it->second.lock();
		}
		else{
			//�w��̖��O��������Ȃ�����
			if (ExceptionActive){
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�I�u�W�F�N�g��������܂���",
					keyerr,
					L"Stage::GetSharedGameObject()"
					);
			}
		}
		return nullptr;
	}


	//�\�z�Ɣj��
	Stage::Stage() :
		GameObject(nullptr),
		pImpl(new Impl())
	{}
	Stage::~Stage(){}

	//�A�N�Z�T
	shared_ptr<ShadowMapRenderTarget> Stage::GetShadowMapRenderTarget() const { return pImpl->m_ShadowMapRenderTarget; }
	shared_ptr<DefaultRenderTarget> Stage::GetDefaultRenderTarget() const { return pImpl->m_DefaultRenderTarget; }
	shared_ptr<RenderState> Stage::GetRenderState() const { return pImpl->m_RenderState; }
	shared_ptr<ParticleManager> Stage::GetParticleManager() const{ return pImpl->m_ParticleManager; }
	shared_ptr<InputTextManager> Stage::GetInputTextManager() const{ return pImpl->m_InputTextManager; }


	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec(){ return pImpl->m_GameObjectVec; }
	void Stage::SetGameObjectVec(vector< shared_ptr<GameObject> >& vec){
		pImpl->m_GameObjectVec.clear();
		pImpl->m_GameObjectVec = vec;
	}
	size_t Stage::GetDrawViewIndex() const { return  pImpl->m_DrawViewIndex; }
	void Stage::SetDrawViewIndex(size_t i){ pImpl->m_DrawViewIndex = i; }


	vector< shared_ptr<Stage> >& Stage::GetChileStageVec(){
		return pImpl->m_ChildStageVec;
	}
	void Stage::AddChileStageBase(const shared_ptr<Stage>& ChildStage){
		pImpl->m_ChildStageVec.push_back(ChildStage);
		ChildStage->SetParentStage(GetThis<Stage>());
	}

	shared_ptr<Stage> Stage::GetParentStage() const{
		if (!pImpl->m_ParentStage.expired()){
			return  pImpl->m_ParentStage.lock();
		}
		return nullptr;
	}
	void Stage::SetParentStage(const shared_ptr<Stage>& ParentStage){
		pImpl->m_ParentStage = ParentStage;
	}



	//�T�[�r�X�A�N�Z�T
	const D3D11_VIEWPORT& Stage::GetViewPort(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetViewPort();
	}
	const D3D11_VIEWPORT& Stage::GetTargetViewPort() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetViewPort();
	}
	const D3D11_VIEWPORT* Stage::GetViewPortRealPtr(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetViewPortPtr();
	}
	const D3D11_VIEWPORT* Stage::GetTargetViewPortRealPtr() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetViewPortPtr();
	}
	const Color4& Stage::GetBackColor(size_t Index) const {
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetBackColor();
	}
	const Color4& Stage::GetTargetBackColor() const {
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetBackColor();
	}
	shared_ptr<Camera> Stage::GetCamera(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetCamera();
	}

	shared_ptr<Camera> Stage::GetTargetCamera() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetCamera();
	}

	shared_ptr<MultiLight> Stage::GetMultiLight(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetMultiLight();
	}

	shared_ptr<MultiLight> Stage::GetTargetMultiLight() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetMultiLight();
	}

	shared_ptr<Light> Stage::GetLight(size_t ViewIndex, size_t LightIndex) const{
		return GetDynamicComponent<MultiView>()->GetView(ViewIndex)->GetMultiLight()->GetLight(LightIndex);
	}

	shared_ptr<Light> Stage::GetTargetLight(size_t LightIndex) const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetMultiLight()->GetLight(LightIndex);
	}

	//����
	shared_ptr<GameObject> Stage::AddGameObject(const shared_ptr<GameObject>& Obj){
		try{
			//�z�u�I�u�W�F�N�g�̌���
			for (auto ptr : pImpl->m_GameObjectVec){
				if (Obj == ptr){
					throw BaseException(
						L"���łɂ��̃I�u�W�F�N�g�͓o�^����Ă��܂�",
						L"�d�����Ă܂�",
						L"Stage::AddGameObject()"
						);
				}
			}
			//�e�X�e�[�W�̐ݒ荷���ւ��i�������g�j
			Obj->SetStage(GetThis<Stage>());
			//Tranceform�K�{
			//�Ȃ������Ƃ��̏ꍇ�ɑΉ�
			Obj->AddComponent<Transform>();
			//�I�u�W�F�N�g�̒ǉ�
			PushBackGameObject(Obj);
			return Obj;
		}
		catch (...){
			throw;
		}
	}

	//�ǉ��҂��ɂȂ��Ă�I�u�W�F�N�g��ǉ�����
	void Stage::SetWaitToObjectVec(){
		if (pImpl->m_WaitAddObjectVec.size() > 0){
			for (auto Ptr : pImpl->m_WaitAddObjectVec){
				pImpl->m_GameObjectVec.push_back(Ptr);
			}
		}
		pImpl->m_WaitAddObjectVec.clear();
	}


	shared_ptr<Object> Stage::GetSharedObject(const wstring& Key, bool ExceptionActive)const{
		shared_ptr<Object> Ptr = GetSharedGameObjectEx(Key, ExceptionActive);
		return Ptr;
	}


	void Stage::SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr){
		map<const wstring, weak_ptr<GameObject> >::iterator it;
		//�d���L�[�̌���
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()){
			//���ɑ��݂���
			//��O����
			wstring keyerr = Key;
			throw BaseException(
				L"�����̃V�F�A�I�u�W�F�N�g������܂�",
				keyerr,
				L"Stage::SetSharedGameObjectEx()"
				);
		}
		else{
			pImpl->m_SharedMap[Key] = Ptr;
		}
	}

	shared_ptr<GameObjectGroup> Stage::CreateSharedObjectGroup(const wstring& Key){
		try{
			map<const wstring, shared_ptr<GameObjectGroup> >::iterator it;
			//�d���L�[�̌���
			it = pImpl->m_SharedGroupMap.find(Key);
			if (it != pImpl->m_SharedGroupMap.end()){
				//���ɑ��݂���
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�����̃V�F�A�I�u�W�F�N�g�z�񂪂���܂�",
					keyerr,
					L"Stage::CreateSharedObjectGroup()"
					);
			}
			else{
				auto Ptr = Object::CreateObject<GameObjectGroup>();
				pImpl->m_SharedGroupMap[Key] = Ptr;
				return Ptr;
			}
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<GameObjectGroup> Stage::GetSharedObjectGroup(const wstring& Key, bool ExceptionActive)const{
		//�d���L�[�̌���
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//�y�A��second��Ԃ�
			return it->second;
		}
		else{
			//�w��̖��O��������Ȃ�����
			if (ExceptionActive){
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�w��̃L�[��������܂���",
					keyerr,
					L"Stage::GetSharedObjectGroup() const"
					);
			}
		}
		return nullptr;
	}

	void Stage::SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr){
		//�d���L�[�̌���
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//��O����
			wstring keyerr = Key;
			throw BaseException(
				L"�����̃V�F�A�I�u�W�F�N�g�z�񂪂���܂�",
				keyerr,
				L"Stage::SetSharedObjectGroup()"
				);
		}
		else{
			//�w��̖��O��������Ȃ�����
			//�o�^�ł���
			pImpl->m_SharedGroupMap[Key] = NewPtr;
		}
	}




	//�f�t�H���g�̃����_�����O�^�[�Q�b�g�ނ���������
	void Stage::CreateDefaultRenderTargets(){
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Ptr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		Ptr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

		//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g���쐬
		pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>();
		//�f�t�H���g�̃����_�����O�^�[�Q�b�g���쐬
		pImpl->m_DefaultRenderTarget = make_shared<DefaultRenderTarget>(GetThis<Stage>());
		//�����_�����O�X�e�[�g�̍쐬
		pImpl->m_RenderState = make_shared<RenderState>();
	}

	void Stage::DrawBackColor(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto RenderStatePtr = GetRenderState();

		//�R���X�^���g�o�b�t�@�̏���
		SimpleDirectConstantBuffer sb;
		sb.m_DiffuseColor = GetTargetBackColor();
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//�R���X�^���g�o�b�t�@�̐ݒ�(�s�N�Z���V�F�[�_�݂̂ł悢)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());

		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p���Ȃ�
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//
		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);

		//�`��
		pD3D11DeviceContext->Draw(pImpl->m_NumVertices, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);
	}


	//���z�֐�
	void Stage::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		bool IsFormMessage = false;
		//�z�u�I�u�W�F�N�g�̌���
		for (auto ptr : pImpl->m_GameObjectVec){
			auto FormPtr = dynamic_pointer_cast<HttpForm>(ptr);
			//�t�H�[����T��
			if (FormPtr){
				FormPtr->OnMessage(message,wParam,lParam);
				IsFormMessage = true;
			}
		}
		if (!IsFormMessage){
			switch (message){
			case WM_KEYDOWN:
				GetInputTextManager()->OnKeyDown(wParam, lParam);
				break;
			case WM_CHAR:
				GetInputTextManager()->OnChar(wParam, lParam);
				break;
			default:
				break;
			}
		}
	}

	void Stage::PreCreate(){
		GameObject::PreCreate();
		//�p�[�e�B�N���}�l�[�W���̍쐬
		pImpl->m_ParticleManager = Object::CreateObject<ParticleManager>(GetThis<Stage>());
		//���̓}�l�[�W���̍쐬
		pImpl->m_InputTextManager = Object::CreateObject<InputTextManager>(GetThis<Stage>());


		//���_���쐬���邽�߂̔z��
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(-1.0f, 1.0f, 1.0f)) },
			{ VertexPosition(Vector3(1.0f, 1.0f, 1.0f)) },
			{ VertexPosition(Vector3(-1.0f, -1.0f, 1.0f)) },
			{ VertexPosition(Vector3(1.0f, -1.0f, 1.0f)) },
		};
		VertexUtil::CreateVertexBuffer(pImpl->m_VertexBuffer, vertices);
		//���_���̐ݒ�
		pImpl->m_NumVertices = static_cast<UINT>(vertices.size());
		//�}���`�r���[�R���|�[�l���g�̓o�^
		AddComponent<MultiView>();
	}


	//�X�e�[�W���̍X�V�i�V�[�������΂��j
	void Stage::UpdateStage(){
		//Transform�R���|�[�l���g�̒l���o�b�N�A�b�v�ɃR�s�[
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				auto ptr2 = ptr->GetComponent<Transform>();
				ptr2->SetToBefore();
			}
		}
		if (IsUpdateActive()){
			//���g�̃o�b�N�A�b�v
			auto ptr2 = GetComponent<Transform>();
			ptr2->SetToBefore();
		}
		//�z�u�I�u�W�F�N�g�̍X�V1
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->Update();
			}
		}
		//���g�̍X�V1
		if (IsUpdateActive()){
			Update();
		}
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V1
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->ComponentUpdate();
			}
		}
		//���g�̃R���|�[�l���g���A�b�v�f�[�g
		if (IsUpdateActive()){
			ComponentUpdate();
		}
		//�z�u�I�u�W�F�N�g�̍X�V2
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->Update2();
			}
		}
		//���g�̍X�V2
		if (IsUpdateActive()){
			Update2();
		}
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V2
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->ComponentUpdate2();
			}
		}
		//���g�̃R���|�[�l���g�̍X�V2
		if (IsUpdateActive()){
			ComponentUpdate2();
		}
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V3
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->Update3();
			}
		}
		//���g�̍X�V3
		if (IsUpdateActive()){
			Update3();
		}
		//�q���X�e�[�W�̍X�V
		for (auto PtrChileStage : GetChileStageVec()){
			PtrChileStage->UpdateStage();
		}
	}



	//�X�e�[�W���̕`��i�V�[�������΂��j
	void Stage::DrawStage(){
		//Sprite�������łȂ����𕪗�
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsDrawActive()){
				//Sprite�����̔h���N���X�Ȃ番��
				if (ptr->GetDynamicComponent<Sprite>(false) || ptr->IsSpriteDraw()){
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else{
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//���C���[�̎擾�Ɛݒ�
		set<int> DrawLayers;
		//3D�̓����Ɣ񓧖��𕪗�
		for (auto ptr : pImpl->m_Object3DVec){
			if (ptr->IsDrawActive()){
				DrawLayers.insert(ptr->GetDrawLayer());
				if (ptr->IsAlphaActive()){
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else{
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		

		auto MultiViewPtr = GetComponent<MultiView>();
		if (!GetParentStage()){
			//�ŏ�̃X�e�[�W�̏ꍇ�̂ݑS��ʃN���A
			GetDefaultRenderTarget()->ClearViews();
		}
		size_t DrawCount = MultiViewPtr->GetViewCount();

		for (size_t i = 0; i < DrawCount; i++){

			SetDrawViewIndex(i);
			//�V���h�E�}�b�v�̊J�n
			GetShadowMapRenderTarget()->ClearViews();
			GetShadowMapRenderTarget()->StartRenderTarget();
			//�z�u�I�u�W�F�N�g�̃V���h�E�}�b�v�`��(�m�[�}��)
			for (auto ptr : pImpl->m_Object3DNormalVec){
				ptr->DrawShadowmap();
			}
			//�z�u�I�u�W�F�N�g�̃V���h�E�}�b�v�`��(����)
			for (auto ptr : pImpl->m_Object3DAlphaVec){
				ptr->DrawShadowmap();
			}
			//�X�e�[�W��DrawShadowmap()�͌Ă΂Ȃ�
			GetShadowMapRenderTarget()->EndRenderTarget();
			//�����_�����O�^�[�Q�b�g�̊J�n
			GetDefaultRenderTarget()->StartRenderTarget();
			//�w�i�F�̕`��
			DrawBackColor();
			auto PtrCamera = GetTargetCamera();
			//�J�����̈ʒu
			Vector3 CameraEye = PtrCamera->GetEye();
			//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
			//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
			//--------------------------------------------------------
			auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool{
				auto PtrLeftTrans = Left->GetComponent<Transform>();
				auto PtrRightTrans = Right->GetComponent<Transform>();

				auto LeftLen = Vector3EX::Length(PtrLeftTrans->GetPosition() - CameraEye);
				auto RightLen = Vector3EX::Length(PtrRightTrans->GetPosition() - CameraEye);

				return (LeftLen > RightLen);
			};

			std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);


			//3D�m�[�}���I�u�W�F�N�g�̕`�揀��
			for (auto ptr : pImpl->m_Object3DNormalVec){
				ptr->PreDraw();
			}
			//3D�����I�u�W�F�N�g�̕`�揀��
			for (auto ptr : pImpl->m_Object3DAlphaVec){
				ptr->PreDraw();
			}
			//�p�[�e�B�N���̕`�揀��
			if (GetParticleManager()){
				GetParticleManager()->PreDraw();
			}
			//�X�v���C�g�I�u�W�F�N�g�̕`�揀��
			for (auto ptr : pImpl->m_SpriteVec){
				ptr->PreDraw();
			}

			//�X�v���C�g��Z���W�����Ń\�[�g
			//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
			//--------------------------------------------------------
			auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool{
				auto PtrLeftTrans = Left->GetComponent<Transform>();
				auto PtrRightTrans = Right->GetComponent<Transform>();
				float LeftZ = PtrLeftTrans->GetPosition().z;
				float RightZ = PtrRightTrans->GetPosition().z;
				return (LeftZ > RightZ);
			};
			std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

			for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++){
				int Tgt = *it;
				//3D�m�[�}���I�u�W�F�N�g�̕`��
				for (auto ptr : pImpl->m_Object3DNormalVec){
					if (ptr->GetDrawLayer() == Tgt){
						ptr->Draw();
					}
				}
				//3D�����I�u�W�F�N�g�̕`��
				for (auto ptr : pImpl->m_Object3DAlphaVec){
					if (ptr->GetDrawLayer() == Tgt){
						ptr->Draw();
					}
				}
				//�p�[�e�B�N���̕`��
				if (GetParticleManager()){
					GetParticleManager()->Draw();
				}
			}
			//�X�v���C�g�I�u�W�F�N�g�̕`��
			for (auto ptr : pImpl->m_SpriteVec){
				ptr->Draw();
			}
			//�X�e�[�W��Draw();
			Draw();
			//�����_�����O�^�[�Q�b�g�̏I��
			GetDefaultRenderTarget()->EndRenderTarget();
		}
		//���[�N�p�z��̃N���A
		//���[�N�z��͖��^�[�����Ƃɏ���������邪�A
		//�ő�l�͌���Ȃ��̂�2��ڂ̃^�[���ȍ~�͍����ɓ��삷��
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
		//�q���X�e�[�W�̕`��
		for (auto PtrChileStage : pImpl->m_ChildStageVec){
			PtrChileStage->DrawStage();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiAudioObject::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct MultiAudioObject::Impl{
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

	void MultiAudioObject::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Start()"
			);
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiAudioObject::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiAudioObject::Impl::Start()"
			);
	}

	void MultiAudioObject::Impl::Stop(const SoundItem* pItem){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Stop()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Stop()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Stop()"
			);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	�p�r: �}���`�I�[�f�B�I
	// ���R���|�[�l���g�ł͂Ȃ��̂ŃV�[���Ɏ������邱�Ƃ��ł���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiAudioObject::MultiAudioObject() :
		pImpl(new Impl())
	{}
	MultiAudioObject::~MultiAudioObject(){}

	//�A�N�Z�T
	shared_ptr<AudioResource> MultiAudioObject::AddAudioResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::AddAudioResource()"
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
						L"MultiAudioObject::AddAudioResource()"
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
				L"MultiAudioObject::AddAudioResource()"
				);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...){
			throw;
		}
	}
	void MultiAudioObject::RemoveAudioResource(const wstring& ResKey){
		if (ResKey == L""){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X�����󔒂ł�",
				L"if (ResKey == L\"\")",
				L"MultiAudioObject::RemoveAudioResource()"
				);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiAudioObject::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const{
		if (ResKey == L""){
			if (ExceptionActive){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::GetAudioResource()"
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
						L"MultiAudioObject::AddAudioResource()"
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
					L"MultiAudioObject::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
	}

	void MultiAudioObject::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiAudioObject::Start(const wstring& ResKey, size_t LoopCount, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				ResKey,
				L"MultiAudioObject::Start()"
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
	void MultiAudioObject::Stop(const wstring& ResKey){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Stop()"
				);
		}
		pImpl->Stop(pItem);
	}




	//--------------------------------------------------------------------------------------
	//	struct SceneBase::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SceneBase::Impl{
		shared_ptr<Stage> m_ActiveStage;	//�A�N�e�B�u�ȃX�e�[�W
		shared_ptr<EventDispatcher> m_EventDispatcher;	//�C�x���g�z���N���X
		Impl() :
			m_ActiveStage()
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SceneBase: public Object;
	//	�p�r: �V�[���x�[�X�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SceneBase::SceneBase():
		pImpl(new Impl())
	{
		try{
			//�f�t�H���g�̃��\�[�X�̍쐬
			App::GetApp()->RegisterResource(L"DEFAULT_SQUARE", CommonMeshResource::CreateSquare(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_CUBE", CommonMeshResource::CreateCube(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE", CommonMeshResource::CreateSphere(1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CAPSULE", CommonMeshResource::CreateCapsule(1.0f,1.0f,18));
			App::GetApp()->RegisterResource(L"DEFAULT_CYLINDER", CommonMeshResource::CreateCylinder(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CONE", CommonMeshResource::CreateCone(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TORUS", CommonMeshResource::CreateTorus(1.0f, 0.3f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TETRAHEDRON", CommonMeshResource::CreateTetrahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_OCTAHEDRON", CommonMeshResource::CreateOctahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_DODECAHEDRON", CommonMeshResource::CreateDodecahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_ICOSAHEDRON", CommonMeshResource::CreateIcosahedron(1.0f));
		}
		catch (...){
			throw;
		}
	}
	SceneBase::~SceneBase(){}


	//�A�N�Z�b�T
	shared_ptr<Stage> SceneBase::GetActiveStage() const{
		if (!pImpl->m_ActiveStage){
			//�A�N�e�B�u�ȃX�e�[�W�������Ȃ�
			throw BaseException(
				L"�A�N�e�B�u�ȃX�e�[�W������܂���",
				L"if(!m_ActiveStage.get())",
				L"SceneBase::GetActiveStage()"
				);
		}
		return pImpl->m_ActiveStage;
	}
	void SceneBase::SetActiveStage(const shared_ptr<Stage>& stage){
		pImpl->m_ActiveStage = stage;
	}

	shared_ptr<EventDispatcher> SceneBase::GetEventDispatcher() const{
		return pImpl->m_EventDispatcher;
	}
	void SceneBase::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		if (pImpl->m_ActiveStage){
			pImpl->m_ActiveStage->OnMessage(message,wParam, lParam);
		}
	}

	void SceneBase::PreCreate(){
		//�C�x���g���M�I�u�W�F�N�g�쐬
		pImpl->m_EventDispatcher = make_shared<EventDispatcher>(GetThis<SceneBase>());
	}
	void SceneBase::Update(){
		if (pImpl->m_ActiveStage){
			//�ǉ��҂��ɂȂ��Ă���I�u�W�F�N�g�̒ǉ�
			pImpl->m_ActiveStage->SetWaitToObjectVec();
			//�C�x���g�L���[�̑��o
			pImpl->m_EventDispatcher->DispatchDelayedEvwnt();
			//�X�e�[�W�̃A�b�v�f�[�g
			pImpl->m_ActiveStage->UpdateStage();
		}
	}
	void SceneBase::Draw(){
		if (pImpl->m_ActiveStage){
			pImpl->m_ActiveStage->DrawStage();
		}
	}


}

//endof  basedx11
