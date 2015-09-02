#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct SimpleDirectConstantBuffer;
	//	�p�r: ���̓o�b�t�@��CPU���\����
	//--------------------------------------------------------------------------------------
	struct SimpleDirectConstantBuffer
	{
		Color4 m_DiffuseColor;
		SimpleDirectConstantBuffer() {
			memset(this, 0, sizeof(SimpleDirectConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect, SimpleDirectConstantBuffer>{
	public:
	};



	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	�p�r: VSSimpleDirect���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>{
	public:
		//�\�z
		VSSimpleDirect();
	};

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	�p�r: PSSimple�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PSSimpleDirect : public PixelShader<PSSimpleDirect>{
	public:
		//�\�z
		PSSimpleDirect();
	};



	//--------------------------------------------------------------------------------------
	//	class GameObject : public Object, public SimpleInterface;
	//	�p�r: �Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	class GameObject : public Object, public SimpleInterface{
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//���b�V���̍쐬
		void CreateCustomMesh();
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		GameObject() :
			m_TotalTime(0)
		{}
		virtual ~GameObject(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


	//--------------------------------------------------------------------------------------
	//	class GameStage : public StageBase;
	//	�p�r: �Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	class GameStage : public StageBase{
		vector<shared_ptr<GameObject>> m_GameObjectVec;
		shared_ptr<View> m_View;
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//�f�t�H���g�̃����_�����O�^�[�Q�b�g
		//������
		shared_ptr<StringSprite> m_StringSprite;
	public:
		GameStage(){}
		virtual ~GameStage(){}
		virtual void Create()override;
		virtual void Update()override;
		virtual void Draw()override;
	};


}
//end basedx11
