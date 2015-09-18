/*!
@file BaseResource.h
@brief ��������ɕێ����郊�\�[�X�i�e�N�X�`���A���b�V���A�I�[�f�B�I�Ȃǁj

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/


#pragma once
#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class BaseResource : public Object;
	/*!
	��������ɕێ����郊�\�[�X�̐e�N���X
	*/
	//--------------------------------------------------------------------------------------
	class BaseResource : public Object{
	protected:
		//--------------------------------------------------------------------------------------
		//	BaseResource();
		/*!
		@breaf �v���e�N�g�R���X���g�N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		BaseResource(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~BaseResource();
		/*!
		@breaf �v���e�N�g�f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource(){}
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	/*!
	�e�N�X�`�����\�[�X
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit TextureResource(
		//	const wstring& FileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const wstring& FileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit TextureResource(const wstring& FileName,const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		//	virtual ~TextureResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		/*!
		@breaf �V�F�[�_�[���\�[�X�r���[�̎擾
		@param �Ȃ�
		@return	���̃e�N�X�`���̃V�F�[�_�[���\�[�X�r���[
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		//	const wstring& GetTextureFileName() const;
		/*!
		@breaf �e�N�X�`���̃t�@�C�����̎擾
		@param �Ȃ�
		@return	���̃e�N�X�`���̃t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureFileName() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	/*!
	�I�[�f�B�I���\�[�X�iwav�j�N���X
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^<br />
		�E�F�u�t�@�C������n��
		@param const wstring& FileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf �T�E���h�f�[�^�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X�̃T�E���h�f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf �E�F�u�t�H�[�}�b�g�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X��WAVEFORMATEX�E�F�u�t�H�[�}�b�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MeshResource : public BaseResource;
	/*!
	���b�V���e�N���X
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource{
	protected:
		//--------------------------------------------------------------------------------------
		//	MeshResource();
		/*!
		@breaf �v���e�N�g�R���X�g���N�^<br />
		�h���N���X����\�z����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		MeshResource();
		//--------------------------------------------------------------------------------------
		//	virtual ~MeshResource();
		/*!
		@breaf �v���e�N�g�f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource();
		//�h���N���X����̂݃A�N�Z�X�ł���A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	void SetVertexBuffer(
		//	ComPtr<ID3D11Buffer>& VertexBuffer	//���_�o�b�t�@
		//	);
		/*!
		@breaf ���_�o�b�t�@�̐ݒ�
		@param ComPtr<ID3D11Buffer>& VertexBuffer	���_�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer);
		//--------------------------------------------------------------------------------------
		//	void SetNumVertices(
		//		UINT NumVertices	//���_��
		//	);
		/*!
		@breaf ���_���̐ݒ�<br />
		���_�o�b�t�@�̐��ƍ��킹�Ȃ���΂Ȃ�Ȃ��̂ŁA�T�d�Ɏg�p����
		@param UINT NumVertices	���_��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices);
		//--------------------------------------------------------------------------------------
		//	void SetIndexBuffer(
		//		ComPtr<ID3D11Buffer>& IndexBuffer	//�C���f�b�N�X�o�b�t�@
		//	);
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�̐ݒ�
		@param ComPtr<ID3D11Buffer>& IndexBuffer	�C���f�b�N�X�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer);
		//--------------------------------------------------------------------------------------
		//	void SetNumIndicis(
		//		UINT NumIndicis	//�C���f�b�N�X��
		//	);
		/*!
		@breaf �C���f�b�N�X���̐ݒ�<br />
		�C���f�b�N�X�o�b�t�@�̐��ƍ��킹�Ȃ���΂Ȃ�Ȃ��̂ŁA�T�d�Ɏg�p����
		@param ComPtr<ID3D11Buffer>& IndexBuffer	�C���f�b�N�X�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis);
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		/*!
		@breaf ���_�o�b�t�@�̎擾
		@param �Ȃ�
		@return	���_�o�b�t�@
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�̎擾
		@param �Ȃ�
		@return	�C���f�b�N�X�o�b�t�@
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumVertices() const;
		/*!
		@breaf ���_���̎擾
		@param �Ȃ�
		@return	���_��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumIndicis() const;
		/*!
		@breaf �C���f�b�N�X���̎擾
		@param �Ȃ�
		@return	�C���f�b�N�X��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const;
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf �X�L�j���O���邩�ǂ���.<br />
		���z�֐��Ȃ̂ŁA�h���N���X�œƎ��ɑ��d��`����
		@param �Ȃ�
		@return	�X�L�j���O����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const{ return false; }
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class CommonMeshResource : public MeshResource;
	/*!
	@breaf �v���~�e�B�u���b�V���N���X.<br />
	�v���~�e�B�u���b�V���́A�X�^�e�B�b�N�֐����g���Đ�������
	*/
	//--------------------------------------------------------------------------------------
	class CommonMeshResource : public MeshResource{
		//--------------------------------------------------------------------------------------
		//	CommonMeshResource();
		/*!
		@breaf �v���C�x�[�g�R���X�g���N�^<br />
		�\�z�̓X�^�e�B�b�N�֐��𗘗p����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		CommonMeshResource();
	public:
		//--------------------------------------------------------------------------------------
		//	virtual ~CommonMeshResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CommonMeshResource();
		//���\�[�X�\�z
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCommonMeshResource(
		//		const vector<VertexPositionNormalTexture>& Vertices,	//���_�̔z��
		//		const vector<uint16_t>& Indices				//�C���f�b�N�X�̔z��
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ���_�ƃC���f�b�N�X��ݒ肵���R�������b�V���̍쐬
		@param	const vector<VertexPositionNormalTexture>& Vertices		���_�̔z��
		@param	const vector<uint16_t>& Indices	�C���f�b�N�X�̔z��
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCommonMeshResource(const vector<VertexPositionNormalTexture>& Vertices, const vector<uint16_t>& Indices, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateSquare(
		//		float Size,		//1�ӂ̃T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf 4�p�`���ʂ̍쐬
		@param	float size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateSquare(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCube(
		//		float Size,		//1�ӂ̃T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �L���[�u�i�����́j�̍쐬
		@param	float size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCube(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateSphere(
		//		float Diameter,		//���a
		//		size_t Tessellation,	//������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ���̂̍쐬
		@param	float Diameter		���a
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCapsule(
		//		float Diameter,		//���a
		//		float Height,		//����
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �J�v�Z���̍쐬
		@param	float Diameter		���a
		@param	float Height,		����
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCylinder(
		//		float Height,	//����
		//		float Diameter, //���a
		//		size_t Tessellation,	//������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �V�����_�[�̍쐬
		@param	float Height,		����
		@param	float Diameter		���a
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCone(
		//		float Diameter, //���a
		//		float Height, //����
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �R�[���̍쐬
		@param	float Diameter		���a
		@param	float Height,		����
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateTorus(
		//		float Diameter, //���a
		//		float Thickness, //�h�[�i�b�c�̑���
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �g�[���X�̍쐬
		@param	float Diameter		���a
		@param	float Thickness		�h�[�i�b�c�̑���
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateTetrahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��4�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateTetrahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateOctahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��8�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateOctahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateDodecahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��12�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateDodecahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateIcosahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��20�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateIcosahedron(float Size, bool AccessWrite = false);

		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	vector<VertexPositionNormalTexture>& GetBackupVertices() const;
		/*!
		@breaf �o�b�N�A�b�v���_�z��̎擾<br />
		AccessWrite��true�ō쐬���ꂽ���\�[�X�́A�o�b�N�A�b�v���_�̔z�������<br />
		�N���C�A���g�͂��̔z��𗘗p���āA�K�v�Ȍ��̂ݏ��������邱�Ƃ��ł���
		@param	�Ȃ�
		@return	�o�b�N�A�b�v���_�z��
		*/
		//--------------------------------------------------------------------------------------
		vector<VertexPositionNormalTexture>& GetBackupVertices() const;
		//--------------------------------------------------------------------------------------
		//	void UpdateVirtexBuffer(
		//		const vector<VertexPositionNormalTexture>& NewBuffer	//���_�̔z��
		//	);
		/*!
		@breaf ���_�̕ύX.<br />
		AccessWrite��true�ō쐬���ꂽ���\�[�X�́A���_�̔z��ɂ���Ē��_��ύX����B
		@param	const vector<VertexPositionNormalTexture>& NewBuffer	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVirtexBuffer(const vector<VertexPositionNormalTexture>& NewBuffer);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	class FbxSceneResource;

	//--------------------------------------------------------------------------------------
	//	struct	AnimationData;
	/*!
	�A�j���[�V�����f�[�^�\����.
	*/
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		//!	�X�^�[�g�t���[��
		UINT	m_StartFrame;
		//!	�t���[���̒���
		UINT	m_FrameLength;
		//!	���[�v���邩�ǂ���
		bool	m_IsLoop;
		//!	���[�v���I���������ǂ���
		bool	m_IsLoopEnd;
		//!	1�b������̃t���[��
		float	m_FramesParSecond;
		//--------------------------------------------------------------------------------------
		//	AnimationData();
		/*!
		@breaf �R���X�g���N�^.
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		//	AnimationData(
		//		UINT StartFrame,	//�X�^�[�g�t���[��
		//		UINT FrameLength,	//�t���[���̒���
		//		bool bLoop,			//���[�v���邩�ǂ���
		//		float FramesParSecond = 30.0f	//1�b������̃t���[����
		//	);
		/*!
		@breaf �R���X�g���N�^.
		@param UINT StartFrame	�X�^�[�g�t���[��
		@param UINT FrameLength	�t���[���̒���
		@param bool bLoop	���[�v���邩�ǂ���
		@param float FramesParSecond = 30.0f	1�b������̃t���[����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartFrame, UINT FrameLength, bool bLoop,
			float FramesParSecond = 30.0f): 
			m_StartFrame{ StartFrame },
			m_FrameLength{ FrameLength },
			m_IsLoop{ bLoop },
			m_IsLoopEnd{ false },
			m_FramesParSecond{ FramesParSecond }
		{}
	};

	//--------------------------------------------------------------------------------------
	//	struct	Bone;
	/*!
	Bone�\����.
	*/
	//--------------------------------------------------------------------------------------
	struct	Bone
	{
		//!��{�|�[�Y�ւ̍s��
		Matrix4X4	m_BindPose;
		//!���݂̍s��
		Matrix4X4	m_CurrentPose;
		//!�v�Z���ꂽ���݂̍s��
		Matrix4X4	m_ConbinedPose;
		//--------------------------------------------------------------------------------------
		//	Bone();
		/*!
		@breaf �R���X�g���N�^.
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		Bone()
		{
			::ZeroMemory(this, sizeof(Bone));
		}
	};

	//--------------------------------------------------------------------------------------
	//	struct Material;
	/*!
	�}�e���A���\����.
	*/
	//--------------------------------------------------------------------------------------
	struct Material{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Color4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Color4 m_Specular;
		//! �A���r�G���g�i���F�j
		Color4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Color4 m_Emissive;
		//! �Q�Ɨp�e�N�X�`���i�폜���Ȃ��j
		shared_ptr<TextureResource> m_Texture;
	};

	//--------------------------------------------------------------------------------------
	//	class FbxMeshResource : public MeshResource;
	/*!
	FBX���b�V�����\�[�X�N���X.
	*/
	//--------------------------------------------------------------------------------------
	class FbxMeshResource : public MeshResource{
		//--------------------------------------------------------------------------------------
		//	void CreateInstanceFromStaticFbx();
		/*!
		@breaf �X�^�e�B�b�NFBX����̃C���X�^���X�̍\�z.
		@param �Ȃ�
		@return	�Ȃ��i��O��throw�����j
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromStaticFbx();
		//--------------------------------------------------------------------------------------
		//	void CreateInstanceFromSkinFbx();
		/*!
		@breaf �X�L�����b�V��FBX����̃C���X�^���X�̍\�z.
		@param �Ȃ�
		@return	�Ȃ��i��O��throw�����j
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromSkinFbx();
		//--------------------------------------------------------------------------------------
		//	void CreateMaterial();
		/*!
		@breaf FBX����}�e���A����ǂݍ���.
		@param �Ȃ�
		@return	�Ȃ��i��O��throw�����j
		*/
		//--------------------------------------------------------------------------------------
		void CreateMaterial();
	public:
		//--------------------------------------------------------------------------------------
		//	struct FbxMeshDeleter;
		/*!
		FBX���b�V���̃f���[�^�[�֐��I�u�W�F�N�g.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxMeshDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxMesh *p	//FBX���b�V���̃|�C���^
			//	);
			/*!
			@breaf FBX���b�V����delete.<br />
			�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			@param FbxMesh *p	FBX���b�V���̃|�C���^
			@return	�Ȃ�
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxMesh *p){
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//--------------------------------------------------------------------------------------
		//	struct FbxSkinDeleter;
		/*!
		FBX�X�L���̃f���[�^�[�֐��I�u�W�F�N�g.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxSkinDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxSkin *p	//FBX�X�L���̃|�C���^
			//	);
			/*!
			@breaf FBX�X�L����delete.<br />
			�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			@param FbxSkin *p	FBX�X�L���̃|�C���^
			@return	�Ȃ�
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxSkin *p){
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	FbxMeshResource(
		//		shared_ptr<FbxSceneResource> FbxSceneResourcePtr,	//FBX�V�[�����\�[�X
		//		FbxMesh* pFbxMesh,	//FBX���b�V�����\�[�X
		//		bool NeedStatic = false	//�X�L�����b�V���ł��X�^�e�B�b�N���b�V���ɂ��邩�ǂ���
		//	);
		/*!
		@breaf �R���X�g���N�^.<br/>
		FBX���b�V������f�[�^��ǂݏo��
		@param shared_ptr<FbxSceneResource> FbxSceneResourcePtr	FBX�V�[�����\�[�X
		@param FbxMesh* pFbxMesh	FBX���b�V�����\�[�X
		@param bool NeedStatic = false	�X�L�����b�V���ł��X�^�e�B�b�N���b�V���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		FbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic = false);
		//--------------------------------------------------------------------------------------
		//	virtual ~FbxMeshResource();
		/*!
		@breaf �f�X�g���N�^.
		@param�@�Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxMeshResource();
		//������
		//--------------------------------------------------------------------------------------
		//	virtual void Create();
		/*!
		@breaf ������
		@param�@�Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Create();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf �X�L�j���O���邩�ǂ���
		@param�@�Ȃ�
		@return	�X�L�j���O����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const;
		//--------------------------------------------------------------------------------------
		//	const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		/*!
		@breaf FBX���b�V���̎擾
		@param�@�Ȃ�
		@return	FBX���b�V���̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		//--------------------------------------------------------------------------------------
		//	const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		/*!
		@breaf FBX�X�L���̎擾
		@param�@�Ȃ�
		@return	FBX�X�L���̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumBones() const;
		/*!
		@breaf �{�[�����̎擾
		@param�@�Ȃ�
		@return	�{�[����
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumBones() const;
		//--------------------------------------------------------------------------------------
		//	const vector<Material>& GetMaterialVec()const;
		/*!
		@breaf �}�e���A���z��̎擾
		@param�@�Ȃ�
		@return	�}�e���A���z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<Material>& GetMaterialVec()const;
		//--------------------------------------------------------------------------------------
		//	const AnimationData& GetAnimationData(const string& AnimeName) const;
		/*!
		@breaf �A�j���[�V�����f�[�^�𓾂�B������Η�O
		@param�@�Ȃ�
		@return	�A�j���[�V�����f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;
		//--------------------------------------------------------------------------------------
		//	const vector< Bone >& GetBonesVec() const;
		/*!
		@breaf �{�[���̔z��𓾂�
		@param�@�Ȃ�
		@return	�{�[���̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector< Bone >& GetBonesVec() const;
		//����
		//--------------------------------------------------------------------------------------
		//	bool GenerateCurrentPose(
		//		vector< Bone >& tgtBones,			//�ύX���ׂ��{�[���z��
		//		AnimationData& rAnimationData,		//�w��̃A�j���[�V�����f�[�^
		//		float CurrentTime					//���Ԏw��
		//	);
		/*!
		@breaf �w��̃A�j���[�V�����̐ݒ肳��Ă��鎞�Ԃɂ�����A�J�����g�|�[�Y�ƍ����ϊ��s����Čv�Z����
		@param�@vector< Bone >& tgtBones	�ύX���ׂ��{�[���z��
		@param�@AnimationData& rAnimationData	�w��̃A�j���[�V�����f�[�^
		@param�@float CurrentTime	���Ԏw��
		@return	���[�v�Ȃ��ōŌ�ɓ��B������true
		*/
		//--------------------------------------------------------------------------------------
		bool GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime);
		//--------------------------------------------------------------------------------------
		//	void AddAnimation(
		//		const char* Name,	//�A�j���[�V������
		//		UINT StartFrame,	//�X�^�[�g�ʒu
		//		UINT FrameLength,	//����
		//		bool Loop,			//���[�v���邩�ǂ���
		//		float FramesParSecond = 30.0f //�P�b������̃t���[����
		//	);
		/*!
		@breaf �A�j���[�V������ݒ肷��i���łɂ��̒�`������΁A�����ւ���j
		@param�@const char* Name	�A�j���[�V������
		@param�@UINT StartFrame,	�X�^�[�g�ʒu
		@param�@UINT FrameLength	����
		@param�@bool Loop,			���[�v���邩�ǂ���
		@param�@float FramesParSecond = 30.0f �P�b������̃t���[����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class FbxSceneResource : public BaseResource;
	/*!
	FBX�V�[�����\�[�X�N���X.<br />
	���FBX�t�@�C���͂P��FbxSceneResource�Ƃ��ĊǗ�����
	*/
	//--------------------------------------------------------------------------------------
	class FbxSceneResource : public BaseResource{
		//--------------------------------------------------------------------------------------
		//	void CreateFbxMeshVector(
		//		FbxNode* pFbxNode				//�^�[�Q�b�g�ƂȂ�m�[�h
		//	);
		/*!
		@breaf FBX���b�V���z����ċA�I�ɓǂݍ���
		@param�@FbxNode* pFbxNode	�^�[�Q�b�g�ƂȂ�m�[�h
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateFbxMeshVector(FbxNode* pFbxNode);
	protected:
		//--------------------------------------------------------------------------------------
		//	FbxSceneResource(
		//		const wstring& DataDir,	//�f�[�^�f�B���N�g��
		//		const wstring& FileName,	//FBX�t�@�C����
		//		const string& SceneName,	//�V�[����
		//		bool NeedStatic				//static�\�z�����邩�ǂ���
		//	);
		/*!
		@breaf �v���e�N�g�R���X�g���N�^.<br/>
		FBX�t�@�C������FBX�V�[�����\�z����
		@param const wstring& DataDir	�f�[�^�f�B���N�g��
		@param const wstring& FileName	FBX�t�@�C����
		@param const string& SceneName	�V�[����
		@param bool NeedStatic	static�\�z�����邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		FbxSceneResource(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool NeedStatic);
		//������
		//--------------------------------------------------------------------------------------
		//	virtual void Create();
		/*!
		@breaf ������
		@param�@�Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Create();
	public:
		//--------------------------------------------------------------------------------------
		//	struct FbxSceneDeleter;
		/*!
		FBX�V�[���̃f���[�^�[�֐��I�u�W�F�N�g.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxSceneDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxScene *p	//FBX�V�[���̃|�C���^
			//	);
			/*!
			@breaf FBX�V�[����delete.<br />
			�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			@param FbxScene *p	FBX�V�[���̃|�C���^
			@return	�Ȃ�
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxScene *p){
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//--------------------------------------------------------------------------------------
		//	virtual ~FbxSceneResource();
		/*!
		@breaf �f�X�g���N�^.
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxSceneResource();
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<FbxSceneResource> CreateFbxScene(
		//		const wstring& DataDir,		//�f�[�^�f�B���N�g��
		//		const wstring& FileName,	//�t�@�C����
		//		const string& SceneName = "",	//�V�[����
		//		bool NeedStatic = false		//static���b�V���Ƃ��č\�z���邩�ǂ���
		//	);
		/*!
		@breaf �\�z�pstatic�֐�.
		@param const wstring& DataDir	�f�[�^�f�B���N�g��
		@param const wstring& FileName		�t�@�C����
		@param const string& SceneName = ""	�V�[����
		@param bool NeedStatic = false	static���b�V���Ƃ��č\�z���邩�ǂ���
		@return�@FBX�V�[�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxSceneResource> CreateFbxScene(const wstring& DataDir,
			const wstring& FileName, const string& SceneName = "", bool NeedStatic = false);
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	wstring GetDataDir() const;
		/*!
		@breaf �f�[�^�f�B���N�g���𓾂�
		@param�@�Ȃ�
		@return	�f�[�^�f�B���N�g��
		*/
		//--------------------------------------------------------------------------------------
		wstring GetDataDir() const;
		//--------------------------------------------------------------------------------------
		//	wstring GetFileName() const;
		/*!
		@breaf �t�@�C�����𓾂�
		@param�@�Ȃ�
		@return	�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		wstring GetFileName() const;
		//--------------------------------------------------------------------------------------
		//	wstring GetFileName() const;
		/*!
		@breaf FBX�V�[�����𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[����
		*/
		//--------------------------------------------------------------------------------------
		string GetFbxSceneName() const;
		//--------------------------------------------------------------------------------------
		//	shared_ptr<FbxMeshResource> GetFbxMeshResource(
		//		size_t Index	//FBX���̃��b�V��ID
		//	) const;
		/*!
		@breaf ���b�V�����\�[�X�𓾂�
		@param�@size_t Index	FBX���̃��b�V��ID
		@return	FBX���b�V�����\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<FbxMeshResource> GetFbxMeshResource(size_t Index) const;
		//--------------------------------------------------------------------------------------
		//	size_t GetFbxMeshResourceSize() const;
		/*!
		@breaf ���b�V�����\�[�X���𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[���Ɋ܂܂�郁�b�V�����\�[�X��
		*/
		//--------------------------------------------------------------------------------------
		size_t GetFbxMeshResourceSize() const;
		//--------------------------------------------------------------------------------------
		//	vector< shared_ptr<FbxMeshResource> >& GetFbxMeshResourceVec() const;
		/*!
		@breaf ���b�V�����\�[�X�z��𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[���Ɋ܂܂�郁�b�V�����\�[�X�z��
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<FbxMeshResource> >& GetFbxMeshResourceVec() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//endof  basedx11
