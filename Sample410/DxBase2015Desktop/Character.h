#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct SplitBoxData;
	//	�p�r:�������ꂽ�I�u�W�F�N�g�̃f�[�^
	//--------------------------------------------------------------------------------------
	struct SplitBoxData{
		bool m_Enabled;	//�L�����ǂ���
		float m_Time;	//�o�ߎ���
		Vector3 m_Scale;	//���݂̃X�P�[��
		Quaternion m_Qt;	//���݂̉�]
		Vector3 m_Pos;	//���݂̈ʒu
		Vector3 m_Velocity;	//���x
		Quaternion m_QtVelocity;	//��]���x
		Vector3 m_GravityVelocity;	//�d�͂ɂ�鑬�x
		SplitBoxData(const Vector3&scale, const Vector3&rot,const Vector3&pos, const Vector3&velo, const Vector3&rotvelo) :
			m_Enabled(true),
			m_Time(0),
			m_Scale(scale),
			m_Qt(),
			m_Pos(pos),
			m_Velocity(velo),
			m_QtVelocity(),
			m_GravityVelocity(0.0f,0.0f,0.0f)
		{
			m_Qt.RotationRollPitchYawFromVector(rot);
			m_QtVelocity.RotationRollPitchYawFromVector(rotvelo);
		}
		void UpdateData(float timespan){
			if (m_Enabled){
				m_Time += timespan;
				m_Pos += m_Velocity * timespan;
				m_GravityVelocity += Vector3(0, -9.8f, 0) * timespan;
				m_Pos += m_GravityVelocity * timespan;
				//��]�̒ǉ�
				m_Qt *= m_QtVelocity;
				//m_Pos��Y��0�ɂȂ�܂ōX�V����
				//�ꍇ�ɂ���Ă�m_Pos��Y��0�ł͂Ȃ��P�[�X���ݒ�ł���
				//�ӂ�����Ԃ��������������ꍇ�́Am_Time�����ŏ����`�ɂ��ł���
				//���̏ꍇ�A�C���X�^���X��MAX�Ɉ���������Ȃ��悤����
				if (m_Pos.y <= 0.0f || m_Time >= 1.0f){
					m_Enabled = false;
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		shared_ptr< StateMachine<FixedBox> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		//������Ԃ̎��̃f�[�^�̔z��
		vector<SplitBoxData> m_SplitBoxDataVec;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//�A�N�Z�T
		shared_ptr< StateMachine<FixedBox> > GetStateMachine() const{
			return m_StateMachine;
		}
		//������
		virtual void Create() override;
		//����
		virtual void Update() override;
		virtual void Update3() override;
		//���[�V�����n
		//�v���C���[���q�b�g������
		bool IsPlayerHit() const;
		//�����p�̍s��z��̍쐬
		void CreateSplitVec(size_t splitcount);
		//�����p�̍s��z��̍X�V
		void UpdateSplitVec();
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxDefaultState : public ObjState<FixedBox>;
	//	�p�r: �ʏ�
	//--------------------------------------------------------------------------------------
	class FixedBoxDefaultState : public ObjState<FixedBox>
	{
		FixedBoxDefaultState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<FixedBoxDefaultState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxSplitState : public ObjState<FixedBox>;
	//	�p�r: �������
	//--------------------------------------------------------------------------------------
	class  FixedBoxSplitState : public ObjState<FixedBox>
	{
		FixedBoxSplitState(){}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr< FixedBoxSplitState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};




	//--------------------------------------------------------------------------------------
	//const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureMatrixLayout[];
	//�p�r: �ʒu�Ɩ@���ƃe�N�X�`���ƍs��������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	//--------------------------------------------------------------------------------------
	// struct VertexPositionNormalTextureMatrix;
	//�p�r: �ʒu�Ɩ@���ƃe�N�X�`���ƍs��������_�̒�`
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTextureMatrix
	{
		VertexPositionNormalTextureMatrix()
		{ }

		VertexPositionNormalTextureMatrix(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate,
			XMFLOAT4X4 const& matrix)
			: position(position),
			normal(normal),
			textureCoordinate(textureCoordinate),
			matrix(matrix)
		{ }

		VertexPositionNormalTextureMatrix(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate,
			FXMMATRIX const& matrix)
		{
			XMStoreFloat3(&this->position, position);
			XMStoreFloat3(&this->normal, normal);
			XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
			XMStoreFloat4x4(&this->matrix, matrix);
		}

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoordinate;
		XMFLOAT4X4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureMatrixLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE(VertexPositionNormalTextureMatrixLayout);
		}
	};



	//�C���X�^���X�V���h�E�}�b�v�p�R���X�^���g�o�b�t�@�\����
	struct InstanceShadowmapConstantBuffer
	{
		XMMATRIX mView;
		XMMATRIX mProj;
		InstanceShadowmapConstantBuffer() {
			memset(this, 0, sizeof(InstanceShadowmapConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBInstanceShadowmap : public ConstantBuffer<CBInstanceShadowmap,InstanceShadowmapConstantBuffer>;
	//	�p�r: �C���X�^���X�V���h�E�}�b�v�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBInstanceShadowmap : public ConstantBuffer<CBInstanceShadowmap, InstanceShadowmapConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSInstanceShadowmap : public VertexShader<VSInstanceShadowmap, VertexPositionNormalTextureMatrix>;
	//	�p�r: VSInstanceShadowmap���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSInstanceShadowmap : public VertexShader<VSInstanceShadowmap, VertexPositionNormalTextureMatrix>{
	public:
		//�\�z
		VSInstanceShadowmap();
	};


	//--------------------------------------------------------------------------------------
	//	class InstanceShadowmap : public Shadowmap;
	//	�p�r: �C���X�^���X�V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class InstanceShadowmap : public Shadowmap{
	public:
		explicit InstanceShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~InstanceShadowmap();
		//����
		virtual void Draw();
	};





	//--------------------------------------------------------------------------------------
	//	struct InstanceConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct InstanceConstantBuffer
	{
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//�ėp�p�����[�^
		Vector4 LPos;
		Vector4 EyePos;
		Matrix4X4 LView;
		Matrix4X4 LProjection;
		InstanceConstantBuffer() {
			memset(this, 0, sizeof(InstanceConstantBuffer));
		};
	};

	//�V�F�[�_�錾
	//�R���X�^���g�o�b�t�@
	class CBInstance : public ConstantBuffer < CBInstance, InstanceConstantBuffer>{
	public: 
	};
	//���_�V�F�[�_
	class VSInstance : public VertexShader<VSInstance, VertexPositionNormalTextureMatrix> {
	public: 
		VSInstance();
	};
	//�s�N�Z���V�F�[�_
	class PSInstance : public PixelShader<PSInstance>
	{
	public:
		PSInstance();
	};




	//--------------------------------------------------------------------------------------
	//	class DrawBoxManager : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X�̕`��}�l�[�W��
	//--------------------------------------------------------------------------------------
	class DrawBoxManager : public GameObject{
		//�C���X�^���X�̍ő�l
		const size_t m_MaxInstance;
		wstring m_MeshKey;
		wstring m_TextureKey;
		vector<Matrix4X4> m_InstanceVec;
		//�s��p�̒��_�o�b�t�@
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//�s��p�̒��_�o�b�t�@�̍쐬
		void CreateMatrixBuffer();
	public:
		//�\�z�Ɣj��
		DrawBoxManager(const shared_ptr<Stage>& StagePtr, size_t MaxInstance, const wstring& MeshKey,const wstring& TextureKey);
		virtual ~DrawBoxManager();
		//������
		virtual void Create() override;
		//�A�N�Z�T
		const ComPtr<ID3D11Buffer>& GetMatrixBuffer() const{
			return m_MatrixBuffer;
		}
		const vector<Matrix4X4>& GetInstanceVec(){
			return m_InstanceVec;
		}
		//����
		//�X�P�[���A��]�A�ʒu�Œǉ�
		void AddInstanceVectors(const Vector3& Scale, const Vector3& Rot, const Vector3& Pos);
		//�s��Œǉ�
		void AddInstanceMatrix(const Matrix4X4& Mat);
		//���z�֐�
		virtual void Update3() override;
		virtual void Draw() override;
	};


}
//endof  basedx11
