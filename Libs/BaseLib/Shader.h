#pragma once

#include "stdafx.h"

namespace basedx11{



	//--------------------------------------------------------------------------------------
	//	class ShaderResource;
	//	�p�r: �V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class ShaderResource{
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	protected:
		ShaderResource();
		virtual ~ShaderResource();
	public:
		static const int m_LightMax{ 3 };
		//--------------------------------------------------------------------------------------
		//	static void ReadBinaryFile(
		//		const wstring& fileName,		//�t�@�C����
		//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^�̖߂�
		//		size_t& CsoSz					//�T�C�Y�̖߂�
		//	);
		//	�p�r: �o�C�i���t�@�C����ǂݍ���
		//--------------------------------------------------------------------------------------
		static void ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz);
		//--------------------------------------------------------------------------------------
		//	static void CreateVertexShader(
		//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
		//		size_t CsoSz,					//�T�C�Y
		//		ID3D11VertexShader** pResult			//�󂯎��V�F�[�_
		//	);
		//	�p�r: �o�C�i���f�[�^���璸�_�V�F�[�_���쐬����
		//--------------------------------------------------------------------------------------
		static void CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateInputLayout(
		//		unique_ptr<uint8_t[]>& Data,	//���_�V�F�[�_�̃o�C�i���f�[�^
		//		size_t CsoSz,					//�T�C�Y
		//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//���_��`
		//		UINT NumElement,							//���_��`�̐�
		//		ID3D11InputLayout** pResult			//�󂯎�郌�C�A�E�g
		//	);
		//	�p�r: �o�C�i���f�[�^����C���v�b�g���C�A�E�g���쐬����
		//--------------------------------------------------------------------------------------
		static void CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreatePixelShader(
		//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
		//		size_t CsoSz,					//�T�C�Y
		//		ID3D11PixelShader** pResult			//�󂯎��V�F�[�_
		//	);
		//	�p�r: �o�C�i���f�[�^����s�N�Z���V�F�[�_���쐬����
		//--------------------------------------------------------------------------------------
		static void CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateGeometryShader(
		//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
		//		size_t CsoSz,					//�T�C�Y
		//		ID3D11GeometryShader** pResult			//�󂯎��V�F�[�_
		//	);
		//	�p�r: �o�C�i���f�[�^����W�I���g���V�F�[�_���쐬����
		//--------------------------------------------------------------------------------------
		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult);

		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, 
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride,ID3D11GeometryShader** pResult);

		//--------------------------------------------------------------------------------------
		//	static void CreateComputeShader(
		//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
		//		size_t CsoSz,					//�T�C�Y
		//		ID3D11ComputeShader** pResult			//�󂯎��V�F�[�_
		//	);
		//	�p�r: �o�C�i���f�[�^����R���s���[�g�V�F�[�_���쐬����
		//--------------------------------------------------------------------------------------
		static void CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult);



		//--------------------------------------------------------------------------------------
		//	static void CreateConstantBuffer(
		//		UINT BuffSize,					//�T�C�Y
		//		ID3D11Buffer** pResult			//�󂯎��V�F�[�_
		//	);
		//	�p�r: �R���X�^���g�o�b�t�@���쐬����
		//--------------------------------------------------------------------------------------
		static void CreateConstantBuffer(UINT BuffSize,ID3D11Buffer** pResult);
	protected:



		//�~���[�e�b�N�X
		std::mutex MutexBase;
		//�s�N�Z���V�F�[�_�A�N�Z�b�T
		ID3D11PixelShader* GetPixelShaderBase(const wstring& Filename, ComPtr<ID3D11PixelShader>& PSPtr){
			return Util::DemandCreate(PSPtr, MutexBase, [&](ID3D11PixelShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreatePixelShader(data, cso_sz, pResult);
			});
		}

		//�W�I���g���V�F�[�_�A�N�Z�b�T
		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename, ComPtr<ID3D11GeometryShader>& GSPtr){
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, pResult);
			});
		}


		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename,
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride,UINT NumStride,
			ComPtr<ID3D11GeometryShader>& GSPtr){
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, SOEntries, Stride, NumStride,pResult);
			});
		}

		//�R���s���[�g�V�F�[�_�A�N�Z�b�T
		ID3D11ComputeShader* GetComputeShaderBase(const wstring& Filename, ComPtr<ID3D11ComputeShader>& CSPtr){
			return Util::DemandCreate(CSPtr, MutexBase, [&](ID3D11ComputeShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateComputeShader(data, cso_sz, pResult);
			});
		}

		//���_�V�F�[�_�A�N�Z�b�T
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr);
		//�C���v�b�g���C�A�E�g�A�N�Z�b�T
		ID3D11InputLayout* GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements);
		//�R���X�^���g�o�b�t�@�A�N�Z�b�T
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer);
	private:
		//�R�s�[�֎~
		ShaderResource(const ShaderResource&) = delete;
		ShaderResource& operator=(const ShaderResource&) = delete;
		//���[�u�֎~
		ShaderResource(const ShaderResource&&) = delete;
		ShaderResource& operator=(const ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType, typename ConstantType>
	//	class ConstantBuffer : public ShaderResource;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename ConstantType>
	class ConstantBuffer : public ShaderResource{
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11Buffer> m_Buffer;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�\�z�Ɣj��
		ConstantBuffer<ShaderType, ConstantType>() : ShaderResource(){}
		virtual ~ConstantBuffer(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//�R�s�[�֎~
		ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&);
		ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&);
	public:
		//���J�A�N�Z�b�T
		ID3D11Buffer* GetBuffer(){
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ConstantType>
	//	class CBuffer : public ShaderResource;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	template<typename ConstantType>
	class CBuffer : public ShaderResource{
		ComPtr<ID3D11Buffer> m_Buffer;
		//�R�s�[�֎~
		CBuffer(CBuffer const&);
		CBuffer& operator=(CBuffer<ConstantType> const&);
	public:
		//�\�z�Ɣj��
		CBuffer() : ShaderResource(){
			GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		virtual ~CBuffer(){}
		//���J�A�N�Z�b�T
		ID3D11Buffer* GetBuffer(){
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
	};



	//--------------------------------------------------------------------------------------
	//	class VertexShader : public ShaderResource;
	//	�p�r: ���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename VirtexType>
	class VertexShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�\�z�Ɣj��
		VertexShader<ShaderType, VirtexType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~VertexShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//���_�V�F�[�_���J�A�N�Z�b�T
		ID3D11VertexShader* GetShader(){
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//�C���v�b�g���C�A�E�g���J�A�N�Z�b�T
		ID3D11InputLayout* GetInputLayout(){
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//template<typename VirtexType>
	//	class VShader : public ShaderResource;
	//	�p�r: �V���O���g�����������Ȃ����_�V�F�[�_�i�N���C�A���g�Œ��ڎg�p�ł���j
	//--------------------------------------------------------------------------------------
	template<typename VirtexType>
	class VShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
	public:
		VShader() : ShaderResource(), m_Filename(){}
		VShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		virtual ~VShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_VSPtr = nullptr;
			m_ILPtr = nullptr;
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//���_�V�F�[�_���J�A�N�Z�b�T
		ID3D11VertexShader* GetShader(){
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//�C���v�b�g���C�A�E�g���J�A�N�Z�b�T
		ID3D11InputLayout* GetInputLayout(){
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class PixelShader : public ShaderResource;
	//	�p�r: �s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class PixelShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11PixelShader> m_PSPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�\�z�Ɣj��
		PixelShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~PixelShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//�s�N�Z���V�F�[�_���J�A�N�Z�b�T
		ID3D11PixelShader* GetShader(){
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PShader : public ShaderResource;
	//	�p�r: �s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	class PShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11PixelShader> m_PSPtr;
	public:
		//�\�z�Ɣj��
		PShader() : ShaderResource(), m_Filename(){}
		PShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		virtual ~PShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_PSPtr = nullptr;
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//�s�N�Z���V�F�[�_���J�A�N�Z�b�T
		ID3D11PixelShader* GetShader(){
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
	};



	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class GeometryShader : public ShaderResource;
	//	�p�r: �W�I���g���V�F�[�_
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class GeometryShader : public ShaderResource{
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�\�z�Ɣj��
		GeometryShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~GeometryShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//�X���b�g�G���g���̓o�^
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride,UINT NumStride){
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries){
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
	public:
		//�W�I���g���V�F�[�_���J�A�N�Z�b�T
		ID3D11GeometryShader* GetShader(){
			if (m_SOEntries.size() > 0){
				return GetGeometryShaderBase(m_Filename, m_SOEntries,m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	class GShader : public ShaderResource;
	//	�p�r: �W�I���g���V�F�[�_
	//--------------------------------------------------------------------------------------
	class GShader : public ShaderResource{
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	public:
		//�\�z�Ɣj��
		GShader() : ShaderResource(), m_Filename(){}
		GShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~GShader(){}
		//�X���b�g�G���g���̓o�^
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride, UINT NumStride){
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries){
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_GSPtr = nullptr;
			if (m_SOEntries.size() > 0){
				GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//�W�I���g���V�F�[�_���J�A�N�Z�b�T
		ID3D11GeometryShader* GetShader(){
			if (m_SOEntries.size() > 0){
				return GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class ComputeShader : public ShaderResource;
	//	�p�r: �R���s���[�g�V�F�[�_
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class ComputeShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11ComputeShader> m_CSPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�\�z�Ɣj��
		ComputeShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~ComputeShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//�R���s���[�g�V�F�[�_���J�A�N�Z�b�T
		ID3D11ComputeShader* GetShader(){
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class CShader : public ShaderResource;
	//	�p�r: �R���s���[�g�V�F�[�_
	//--------------------------------------------------------------------------------------
	class CShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11ComputeShader> m_CSPtr;
	public:
		//�\�z�Ɣj��
		CShader() : ShaderResource(), m_Filename(){}
		CShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		virtual ~CShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_CSPtr = nullptr;
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//�R���s���[�g�V�F�[�_���J�A�N�Z�b�T
		ID3D11ComputeShader* GetShader(){
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
	};




}
//endof  basedx11
