#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//������
	void FixedBox::Create(){
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e�{�b�N�X��Draw���Ȃ�
		//�}�l�[�W���ɂ���Ĉ�̃��b�V���ɂ܂Ƃ߂��`�悳���
		SetDrawActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"FIXED_BOX");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<FixedBox>());

	}


	//--------------------------------------------------------------------------------------
	//	class FixedBoxManager : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X�̃}�l�[�W���i�`�悷��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBoxManager::FixedBoxManager(shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	FixedBoxManager::~FixedBoxManager(){}

	//���_�ƃC���f�b�N�X���s��ƁA�i���o�[�ɍ��킹�ĕύX����
	void FixedBoxManager::ChangeVertex(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, const Matrix4X4& Mat, size_t Num){
		auto GStage = dynamic_pointer_cast<GameStage>(GetStage());

		float XScale = Mat.ScaleInMatrix().x / GStage->GetPiece().x;
		float ZScale = Mat.ScaleInMatrix().z / GStage->GetPiece().z;
		for (auto& V : vertices){
			//���_���s��ɂ��ύX����
			V.position = Vector3EX::Transform(V.position, Mat);
			//UV�l�̕ύX
			//�^�C�����O�̎���
			if (V.normal.x == 0.0f){
				if (V.textureCoordinate.x != 0){
					V.textureCoordinate.x = XScale;
				}
				if (V.textureCoordinate.y != 0){
					if (V.normal.z != 0.0f){
						V.textureCoordinate.y = 1.0f;
					}
					else{
						V.textureCoordinate.y = ZScale;
					}
				}
			}
			else{
				if (V.textureCoordinate.x != 0){
					V.textureCoordinate.x = ZScale;
				}
				if (V.textureCoordinate.y != 0){
					V.textureCoordinate.y = 1.0f;
				}
			}
		}
		//���_�̐��𓾂�
		size_t sz = vertices.size();
		for (auto& I : indices){
			//�C���f�b�N�X��Num�i�{�b�N�X�̃i���o�[�j�ɍ��킹�ĕύX
			I = (Num * sz) + I;
		}
	}


	//������
	void FixedBoxManager::Create(){
		//FixBox�̃O���[�v����̃��b�V���ɂ܂Ƃ߂�
		//�������邱�ƂŃ}�b�v��Draw�́A���̕`�施�߂ŕ`�悳���
		//�g�����X�t�H�[���͌��_�ɒu��
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		PtrTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		PtrTransform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		//�V�����쐬���钸�_�̔z��
		vector<VertexPositionNormalTexture> NewCubeVerteces;
		//�V�����쐬����C���f�b�N�X�̔z��
		vector<uint16_t> NewCubeIndices;
		//FIXEDBOX�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"FIXED_BOX");
		auto Vec = Group->GetGroupVector();
		for (size_t Index = 0; Index < Vec.size(); Index++){

			if (!Vec[Index].expired()){
				//�O���[�v�z���weakptr�Ȃ̂�shared_ptr�ɃL���X�g
				auto ShPtr = Vec[Index].lock();
				//FIXED�{�b�N�X�̂��ꂼ��̍s��𓾂�
				auto Mat = ShPtr->GetComponent<Transform>()->GetWorldMatrix();
				vector<VertexPositionNormalTexture> CubeVerteces;
				vector<uint16_t> CubeIndices;
				//��ƂȂ�CUBE�̍쐬(�f�t�H���g�Ɠ����傫��)
				VertexUtil::CreateCube(1.0f, CubeVerteces, CubeIndices);
				//�{�b�N�X�̃C���f�b�N�X�ɂ�蒸�_����ύX
				ChangeVertex(CubeVerteces, CubeIndices, Mat, Index);
				//���_�ƃC���f�b�N�X��V�����z��ɒǉ�
				for (auto V : CubeVerteces){
					NewCubeVerteces.push_back(V);
				}
				for (auto I : CubeIndices){
					NewCubeIndices.push_back(I);
				}
			}
		}
		//���b�V�����\�[�X�̍쐬
		m_BoxesRes = CommonMeshResource::CreateCommonMeshResource(NewCubeVerteces, NewCubeIndices);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_BoxesRes);

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		PtrDraw->SetMeshResource(m_BoxesRes);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = GetStage()->GetRenderState();
		//�^�C�����O�̐ݒ�
		PtrDraw->SetSamplerState(RenderStatePtr->GetLinearWrap());
		PtrDraw->SetOwnShadowActive(true);
	}



}
//endof  basedx11
