/*!
@file BaseHelper.h
@brief ��O�����A�w���p�[�֐���

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//! �X�}�[�g�|�C���^�̃w���p�[
	struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
	typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
	inline HANDLE safe_handle(HANDLE h) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }

	//--------------------------------------------------------------------------------------
	//	class BaseException : public exception
	/*!
		��O�N���X
	*/
	//--------------------------------------------------------------------------------------
	class BaseException : public exception
	{
		//���b�Z�[�W�ϐ�
		wstring m_Message;
	public:
		//--------------------------------------------------------------------------------------
		//	BaseException(
		//	const wstring& m1,  //��1���b�Z�[�W�i�x�����j
		//	const wstring& m2,   //��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		//	const wstring& m3   //��3���b�Z�[�W�i���������ʒu�j
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const wstring& m1  ��1���b�Z�[�W�i�x�����j
		@param const wstring& m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		@param const wstring& m3  ��3���b�Z�[�W�i���������ʒu�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		BaseException(const wstring&  m1, const wstring&  m2, const wstring& m3){
			m_Message = m1;
			m_Message += L"\r\n";
			m_Message += m2;
			m_Message += L"\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		//	const wstring& what_w() const throw()
		/*!
		@breaf ���b�Z�[�W�̎擾
		@return	���b�Z�[�W������̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const wstring& what_w() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class BaseMBException : public exception
	/*!
	��O�N���X�i�}���`�o�C�g�Łj
	*/
	//--------------------------------------------------------------------------------------
	class BaseMBException : public exception{
		//���b�Z�[�W�ϐ�
		string m_Message;
	public:
		//--------------------------------------------------------------------------------------
		//	BaseMBException(
		//	const string& m1,  //��1���b�Z�[�W�i�x�����j
		//	const string& m2,   //��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		//	const string& m3   //��3���b�Z�[�W�i���������ʒu�j
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const string& m1  ��1���b�Z�[�W�i�x�����j
		@param const string& m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		@param const string& m3  ��3���b�Z�[�W�i���������ʒu�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		BaseMBException(const string& m1, const string& m2, const string& m3)
		{
			m_Message = m1;
			m_Message += "\r\n";
			m_Message += m2;
			m_Message += "\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		//	const wstring& what_m() const throw()
		/*!
		@breaf ���b�Z�[�W�̎擾�i�}���`�o�C�g�j
		@return	���b�Z�[�W������̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const string& what_m() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	//	inline HRESULT ThrowIfFailed(
	//		HRESULT hr,		//�C���^�[�t�F�C�X�����̖߂�l
	//		const wstring& Str1,	//�x��
	//		const wstring& Str2,	//���ƂȂ����f�[�^
	//		const wstring& Str3	//������
	//	);
	/*!
	@breaf �C���^�[�t�F�C�X�̗�O
	@param const wstring& Str1  ��1���b�Z�[�W�i�x�����j
	@param const wstring& Str2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
	@param const wstring& Str3  ��3���b�Z�[�W�i���������ʒu�j
	@return	HRESULT
	*/
	//--------------------------------------------------------------------------------------
	inline HRESULT ThrowIfFailed(HRESULT hr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (FAILED(hr))
		{
			throw BaseException(
				Str1,
				Str2,
				Str3
				);
		}
		return hr;
	}

	//--------------------------------------------------------------------------------------
	//	class BinaryReader
	/*!
	�o�C�i���f�[�^�ǂݍ��݁BDirectXTK�J�X�^�}�C�Y
	*/
	//--------------------------------------------------------------------------------------
	class BinaryReader
	{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit BinaryReader(
		//	const wstring& fileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const wstring& fileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit BinaryReader(const wstring& fileName);
		BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize);


		// Reads a single value.
		template<typename T> T const& Read()
		{
			return *ReadArray<T>(1);
		}


		// Reads an array of values.
		template<typename T> T const* ReadArray(size_t elementCount)
		{
			static_assert(std::is_pod<T>::value, "Can only read plain-old-data types");

			uint8_t const* newPos = mPos + sizeof(T)* elementCount;

			if (newPos > mEnd)
				throw std::exception("End of file");

			auto result = reinterpret_cast<T const*>(mPos);

			mPos = newPos;

			return result;
		}


		// Lower level helper reads directly from the filesystem into memory.
		static HRESULT ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);


	private:
		// The data currently being read.
		uint8_t const* mPos;
		uint8_t const* mEnd;

		std::unique_ptr<uint8_t[]> mOwnedData;

		//�R�s�[�֎~
		BinaryReader(const BinaryReader&) = delete;
		BinaryReader& operator=(const BinaryReader&) = delete;
		//���[�u�֎~
		BinaryReader(const BinaryReader&&) = delete;
		BinaryReader& operator=(const BinaryReader&&) = delete;
	};




	//--------------------------------------------------------------------------------------
	//	class CsvFile;
	/*!
	Csv�t�@�C���A�N�Z�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class CsvFile{
	public:
		//--------------------------------------------------------------------------------------
		//	CsvFile();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		CsvFile();
		//--------------------------------------------------------------------------------------
		//	explicit CsvFile(
		//	const wstring& FileName	//CSV�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const wstring& FileName	CSV�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit CsvFile(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~CsvFile();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CsvFile();
		//--------------------------------------------------------------------------------------
		//	const wstring& GetFileName() const;
		/*!
		@breaf �t�@�C�����𓾂�
		@param �Ȃ�
		@return	csv�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFileName() const;
		//--------------------------------------------------------------------------------------
		//	void SetFileName(
		//	const wstring& FileName	//CSV�t�@�C����
		//	);
		/*!
		@breaf �t�@�C�������Z�b�g����
		@param const wstring& FileName	CSV�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFileName(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	vector< wstring >& GetCsvVec()const;
		/*!
		@breaf CSV�z��𓾂�
		@param �Ȃ�
		@return	CSV�z��
		*/
		//--------------------------------------------------------------------------------------
		vector< wstring >& GetCsvVec()const;
		//--------------------------------------------------------------------------------------
		//	bool ReadCsv();
		/*!
		@breaf
		Csv��ǂݍ���.
		�����łɎ����Ă�f�[�^�͔j�������<br/>
		��Csv�t�@�C�������݂��Ȃ��ꍇ�͏���������������false��Ԃ��B<br />
		�����݂��Ă��ǂݍ��݂Ɏ��s�������O
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		bool ReadCsv();
		//--------------------------------------------------------------------------------------
		//	void SaveCsv(
		//	bool Exp = true	//��O�������邩�ǂ���
		//	);
		/*!
		@breaf
		Csv�������o��. 
		��Csv�t�@�C�������݂��Ȃ���΍쐬����A���݂��Ă�����A�f�[�^�͏����������<br/>
		���A�N�Z�X�Ɏ��s���A��O�����t���O�������Ă����O<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�<br />
		@param bool Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SaveCsv(bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	size_t AddRow(
		//	vector<wstring>& Row	//Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		//	);
		/*!
		@breaf
		1�s�Ō���ɒǉ�����.
		@param vector<wstring>& Row	Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		@return	�ǉ������s�ԍ�
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		//	size_t AddRow(
		//	wstring& Row	//Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		//	);
		/*!
		@breaf
		1�s�Ō���ɒǉ�����.
		@param wstring& Row	Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		@return	�ǉ������s�ԍ�
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(wstring& Row);
		//--------------------------------------------------------------------------------------
		//	void UpdateRow(
		//	size_t RowNum,			//�ύX�������s
		//	vector<wstring>& Row.	//Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		//	bool Exp = true			//��O�������邩�ǂ���
		//	);
		/*!
		@breaf
		�w�肵��1�s��ύX����.
		���K�������X�V�O�Ɨ�̐��͓����łȂ��Ă��悢�B<br />
		���w�肵���s�����݂��Ȃ��ꍇ�A��O�𔭐�����<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�
		@param size_t RowNum,	�ύX�������s
		@param vector<wstring>& Row.	Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		@param bool Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	void UpdateRow(
		//	size_t RowNum,	//�ύX�������s
		//	wstring& Row,	//Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		//	bool Exp = true	//��O�������邩�ǂ���
		//	);
		/*!
		@breaf
		�w�肵��1�s��ύX����.
		��Row�́u,�v�ɂ���ċ�؂��z�񉻂���ۑ������<br />
		���K�������X�V�O�Ɨ�̐��͓����łȂ��Ă��悢�B<br />
		���w�肵���s�����݂��Ȃ��ꍇ�A��O�𔭐�����<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�
		@param size_t RowNum,	�ύX�������s
		@param wstring& Row,	Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		@param bool Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, wstring& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	void UpdateCell(
		//	size_t RowNum,	//�ύX�s
		//	size_t ColNum,	//�ύX��
		//	wstring& Cell,	//�����ւ��镶����
		//	bool Exp = true			//��O�������邩�ǂ���
		//	);
		/*!
		@breaf
		1�̃Z����ݒ肷��.
		@param size_t RowNum,	�ύX�s
		@param size_t ColNum,	�ύX��
		@param wstring& Cell,	�����ւ��镶����
		@param bool Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	size_t GetRowCount() const;
		/*!
		@breaf
		�s���𓾂�.
		@param �Ȃ�
		@return	�s��
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRowCount() const;
		//--------------------------------------------------------------------------------------
		//	void GetRowVec(
		//	size_t RowNum,			//�󂯎�肽���s
		//	vector<wstring>& Row	//1�s���󂯎�镶����̔z��
		//	);
		/*!
		@breaf
		1�s���̔z��𓾂�.
		��RowNum���z��̏���𒴂����ꍇ�͗�O<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����
		@param size_t RowNum,	�󂯎�肽���s
		@param vector<wstring>& Row	1�s���󂯎�镶����̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetRowVec(size_t RowNum, vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		//	wstring GetCell(
		//	size_t RowNum,	//�󂯎�肽���s
		//	size_t ColNum	//�󂯎�肽����
		//	);
		/*!
		@breaf
		1�̃Z���𓾂�.
		��RowNum�܂���ColNum���z��̏���𒴂����ꍇ�͗�O<br/>
		���󂯎�镶���񂪋󔒂̏�Ԃ̂��Ƃ�����
		@param size_t RowNum,	�󂯎�肽���s
		@param size_t ColNum	�󂯎�肽����
		@return	�Z���̒��g
		*/
		//--------------------------------------------------------------------------------------
		wstring GetCell(size_t RowNum, size_t ColNum);
		//--------------------------------------------------------------------------------------
		//	void GetSelect(
		//	vector< wstring >& RetVec	//���ʂ��󂯎�镶����̔z��
		//	size_t ColNum,			//���������������
		//	const wstring& Key,		//�����L�[
		//	);
		/*!
		@breaf
		��������񂪏����L�[�Ɠ����s�𔲏o��������̔z��ɕԂ�.
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@param vector< wstring >& RetVec	���ʂ��󂯎�镶����̔z��
		@param size_t ColNum,	���������������
		@param const wstring& Key,	�����L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec, size_t ColNum, const wstring& Key);
		//--------------------------------------------------------------------------------------
		//	void GetSelect(
		//	vector< wstring >& RetVec,		//���ʂ��󂯎�镶����̔z��
		//	bool (Func)(const wstring&),	//��������������R�[���o�b�N�֐�
		//	);
		/*!
		@breaf
		�R�[���o�b�N�֐����Ăяo���Atrue���Ԃ����s�𔲏o��������̔z��ɕԂ�.
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@param vector< wstring >& RetVec	���ʂ��󂯎�镶����̔z��
		@param bool (Func)(const wstring&),	��������������R�[���o�b�N�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec,bool (Func)(const wstring&));
		//--------------------------------------------------------------------------------------
		//	template<typename Fct>
		//	void GetSelect2(
		//	vector< wstring >& RetVec,	//���ʂ��󂯎�镶����̔z��
		//	Fct f						//�������������郉���_���i�L���v�`���g�p�\�j
		//	);
		/*!
		@breaf
		�����_�����Ăяo���Atrue���Ԃ����s�𔲏o��������̔z��ɕԂ�.
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@param vector< wstring >& RetVec	���ʂ��󂯎�镶����̔z��
		@param Fct f,	�������������郉���_��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename Fct>
		void GetSelect2(vector< wstring >& RetVec, Fct f){
			//�n���ꂽ�z��ɒl�������Ă�ꍇ������̂ŃN���A
			RetVec.clear();
			auto& CsvVec = GetCsvVec();
			size_t sz = CsvVec.size();
			for (size_t i = 0; i < sz; i++){
				//�R�[���o�b�N�֐����Ăяo��
				if (f(CsvVec[i])){
					//����������Βǉ�
					RetVec.push_back(CsvVec[i]);
				}
			}
		}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		CsvFile(const CsvFile&) = delete;
		CsvFile& operator=(const CsvFile&) = delete;
		//���[�u�֎~
		CsvFile(const CsvFile&&) = delete;
		CsvFile& operator=(const CsvFile&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	wstring MakeRangeErr(
	//		const wchar_t* key1,	//�L�[1��
	//		T val1,					//�l�P
	//		const wchar_t* key2,	//�L�[�Q��
	//		T val2					//�l2
	//	)
	/*!
	@breaf
	��r�̃G���[�̕�������쐬����.
	@param const wchar_t* key1,	�L�[1��
	@param T val1,	�l�P
	@param const wchar_t* key2,	�L�[�Q��
	@param T val2	�l2
	@return	�G���[������
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	wstring MakeRangeErr(const wchar_t* key1, T val1, const wchar_t* key2, T val2){
		//�Ԃ�������
		wstring str;
		//�����𐮂���X�g���[��
		wostringstream stream;
		stream << key1 << L"==" << val1 << L"," << key2 << L"==" << val2;
		str = stream.str();
		return str;
	}

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeDelete(
	//	T*& p   // T�^�̃|�C���^�̎Q��
	//	)
	/*!
	@breaf
	���S�Ƀ|�C���^��delete����.
	@param T*& p	T�^�̃|�C���^�̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDelete(T*& p){
		if (p){
			delete p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeDeleteArr(
	//	T*& p   // T�^�̔z��̎Q��
	//	)
	/*!
	@breaf
	���S�ɔz���delete����.
	@param T*& p	T�^�̔z��̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDeleteArr(T*& p){
		if (p){
			delete[] p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeRelease(
	//	T*& p   // T�^�̃C���^�[�t�F�C�X�̃|�C���^�̎Q��
	//	)
	/*!
	@breaf
	���S�ɃC���^�[�t�F�C�X��release����.
	@param T*& p  T�^�̃C���^�[�t�F�C�X�̃|�C���^�̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeRelease(T*& p){
		if (p){
			p->Release();
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SefeDeletePointerList(
	//	list<C*>& List	//C�^�̃|�C���^�̃��X�g�̎Q��
	//	);
	/*!
	@breaf
	�|�C���^�̃��X�g�����S�ɃN���A����.
	@param list<C*>& List	C�^�̃|�C���^�̃��X�g�̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SefeDeletePointerList(list<C*>& List){
		list<C*>::iterator it;
		for (it = List.begin(); it != List.end(); ++it){
			delete (*it);
		}
		List.clear();
	}
	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SafeDeletePointerVector(
	//	vector<C*>& vec	//C�^�̃|�C���^�̔z��̎Q��
	//	);
	/*!
	@breaf
	�|�C���^�̔z��ivector�j�����S�ɃN���A����.
	@param vector<C*>& vec	//C�^�̃|�C���^�̔z��̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SafeDeletePointerVector(vector<C*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			delete Vec[i];
		}
		Vec.clear();
	}

	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SafeReleasePointerVector(
	//	vector<C*>& vec	//C�^�C���^�[�t�F�C�X�̃|�C���^�z��̎Q��
	//	);
	/*!
	@breaf
	�C���^�[�t�F�C�X�̔z��ivector�j�����S�Ƀ����[�X����.
	@param vector<C*>& vec	C�^�C���^�[�t�F�C�X�̃|�C���^�z��̎Q��
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SafeReleasePointerVector(vector<C*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			if (Vec[i]){
				Vec[i]->Release();
			}
			Vec[i] = nullptr;
		}
		Vec.clear();
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	shared_ptr<void> SharedToVoid(
	//		const shared_ptr<T>& SrcPtr //�ϊ���
	//	);
	/*!
	@breaf
	shared_ptr��void�^�ɕϊ�����.
	@param const shared_ptr<T>& SrcPtr�@�ϊ���
	@return	void�^��shared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<void> SharedToVoid(const shared_ptr<T>& SrcPtr){
		shared_ptr<void> RetPtr = static_pointer_cast<void>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	shared_ptr<T> VoidToShared(
	//		const shared_ptr<void>& SrcPtr //�ϊ���
	//	);
	/*!
	@breaf
	void�^��shared_ptr��T�^��shared_ptr�ɕϊ�����.
	@param const shared_ptr<void>& SrcPt�@�ϊ���
	@return	T�^��shared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> VoidToShared(const shared_ptr<void>& SrcPtr){
		shared_ptr<T> RetPtr = static_pointer_cast<T>(SrcPtr);
		return RetPtr;
	}

	//--------------------------------------------------------------------------------------
	//	struct Util;
	/*!
	���̑��̃��[�e�B���e�B�Bstatic�Ăяo��������
	*/
	//--------------------------------------------------------------------------------------
	struct Util{
		//--------------------------------------------------------------------------------------
		//	template<class T>
		//	T Maximum(
		//	const T& v1,	//T�^�̒l1�̎Q��
		//	const T& v2		//T�^�̒l2�̎Q��
		//	);
		/*!
		@breaf
		�傫���������߂�.
		@param const T& v1	T�^�̒l1
		@param const T& v2	T�^�̒l2
		@return	T�^�̑傫����
		*/
		//--------------------------------------------------------------------------------------
		template<class T>
		static T Maximum(const T& v1, const T& v2){
			return v1 > v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		//	template<class T>
		//	T Minimum(
		//	const T& v1,	//T�^�̒l1�̎Q��
		//	const T& v2		//T�^�̒l2�̎Q��
		//	);
		/*!
		@breaf
		�������������߂�.
		@param const T& v1	T�^�̒l1
		@param const T& v2	T�^�̒l2
		@return	T�^�̏�������
		*/
		//--------------------------------------------------------------------------------------
		template<class T>
		static T Minimum(const T& v1, const T& v2){
			return v1 < v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		//	template<typename T, typename TCreateFunc>
		//	static T* DemandCreate(
		//	ComPtr<T>& comPtr,	//COM�|�C���^
		//	std::mutex& mutex,	//�~���[�e�b�N�X
		//	TCreateFunc createFunc	//�쐬�֐�
		//	);
		/*!
		@breaf
		���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.
		@param ComPtr<T>& comPtr	COM�|�C���^
		@param std::mutex& mutex	�~���[�e�b�N�X
		@param TCreateFunc createFunc	�쐬�֐�
		@return�@COM�������̏ꍇ�͐V�����쐬���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();

			// Double-checked lock pattern.
			MemoryBarrier();

			if (!result)
			{
				std::lock_guard<std::mutex> lock(mutex);

				result = comPtr.Get();

				if (!result)
				{
					createFunc(&result);

					MemoryBarrier();

					comPtr.Attach(result);
				}
			}
			return result;
		}

		//--------------------------------------------------------------------------------------
		//	template<typename T>
		//	static T* GetComPtr(
		//	ComPtr<T>& comPtr,//COM�|�C���^
		//	);
		/*!
		@breaf
		���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.
		�����̏ꍇ�͗�O���o��
		@param ComPtr<T>& comPtr	COM�|�C���^
		@return�@COM�������̏ꍇ�͗�O���o���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T* GetComPtr(ComPtr<T>& comPtr){
			T* result = comPtr.Get();
			// Double-checked lock pattern.
			MemoryBarrier();
			if (!result)
			{
				//���s
				throw BaseException(
					L"���̃R���|�[�l���g���擾�ł��܂���",
					L"if (!result)",
					L"Util::GetComPtr()"
					);
			}
			return result;
		}
		//--------------------------------------------------------------------------------------
		//	static void WStrTrim(
		//	wstring& wstr,			//���ƂɂȂ镶����i���^�[���ɂ��g�p�����j
		//	const wchar_t* trimCharacterList = L" \t\v\r\n"	//��菜������
		//	)
		/*!
		@breaf
		���[����󔒂���菜��<br/>
		���ɂȂ邢�����񂩂��菜��
		@param wstring& wstr	���ƂɂȂ镶����i���^�[���ɂ��g�p�����j
		@param const wchar_t* trimCharacterList = L" \t\v\r\n"	��菜������
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStrTrim(wstring& wstr, const wchar_t* TrimCharList = L" \t\v\r\n"){
			wstring result(L"");
			if (wstr.size() <= 0){
				wstr = result;
				return;
			}
			wstring::size_type left = wstr.find_first_not_of(TrimCharList);
			if (left != wstring::npos){
				wstring::size_type right = wstr.find_last_not_of(TrimCharList);
				result = wstr.substr(left, right - left + 1);
			}
			wstr = result;
		}

		//--------------------------------------------------------------------------------------
		//	static void WStoMB(
		//	const wstring& src,	//�ϊ����镶����i���C�h�L�����j
		//	string& dest			//�ϊ���̕�����i�}���`�o�C�g�j
		//	);
		/*!
		@breaf
		���C�h�����񂩂�}���`�o�C�g������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param const wstring& src,	�ϊ����镶����i���C�h�L�����j
		@param string& dest			�ϊ���̕�����i�}���`�o�C�g�j
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStoMB(const wstring& src, string& dest){
			size_t i;
			char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs_s(
				&i,
				pMBstr,
				src.length() * MB_CUR_MAX + 1,
				src.c_str(),
				_TRUNCATE	//���ׂĕϊ��ł��Ȃ�������؂�̂�
				);
			dest = pMBstr;
			delete[] pMBstr;
		}

		//--------------------------------------------------------------------------------------
		//	static void ConvertWstringtoUtf8(
		//	const wstring& src,	//�ϊ����镶����i���C�h�L�����j
		//	string& dest			//�ϊ���̕�����i�}���`�o�C�gUTF8�j
		//	);
		/*!
		@breaf
		���C�h�����񂩂�}���`�o�C�gUTF8������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param const wstring& src,	�ϊ����镶����i���C�h�L�����j
		@param string& dest			�ϊ���̕�����i�}���`�o�C�g�j
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertWstringtoUtf8(const wstring& src, string& dest){
			INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
			char* Temp = new char[bufsize + 1];
			::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize, NULL, NULL);
			dest = Temp;
			delete[] Temp;
		}

		//--------------------------------------------------------------------------------------
		//	static void ConvertUtf8toWstring(
		//	const string& src				//�ϊ����镶����i�}���`�o�C�gUTF8�j
		//	wstring& dest,	//�ϊ��㕶����i���C�h�L�����j
		//	);
		/*!
		@breaf
		�}���`�o�C�gUTF8�������烏�C�h������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param const string& src�@�ϊ����镶����i�}���`�o�C�gUTF8�j
		@param wstring& dest,	//�ϊ��㕶����i���C�h�L�����j
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertUtf8toWstring(const string& src, wstring& dest){
			INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (wchar_t *)NULL, 0);
			wchar_t* Temp = (wchar_t*)new wchar_t[bufsize];
			::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize);
			dest = Temp;
			delete[] Temp;
		}
		//--------------------------------------------------------------------------------------
		//	static  void MBtoWS(
		//	const string& src,	//�ϊ����镶����i�}���`�o�C�g�j
		//	wstring& dest		//�ϊ���̕�����i���C�h�L�����j
		//	);
		/*!
		@breaf
		�}���`�o�C�g�����񂩂烏�C�h������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param const string& src,	�ϊ����镶����i�}���`�o�C�g�j
		@param wstring& dest		�ϊ���̕�����i���C�h�L�����j
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void MBtoWS(const string& src, wstring& dest){
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //���ׂĕϊ��ł��Ȃ�������؂�̂�
				);
			dest = WCstr;
			delete[] WCstr;
		}

		//--------------------------------------------------------------------------------------
		//	static void WStrToTokenVector(
		//	vector<wstring>& wstrvec,	//�ۑ������z��
		//	const wstring& line,		//���ƂȂ镶����
		//	wchar_t delimiter			//�f���~�^
		//	)
		/*!
		@breaf
		��������f���~�^�����ɂ���؂��Ĕz��ɕۑ�
		@param vector<wstring>& wstrvec,	�ۑ������z��
		@param const wstring& line,		���ƂȂ镶����
		@param wchar_t delimiter		�f���~�^
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStrToTokenVector(vector<wstring>& wstrvec, const wstring& line, wchar_t delimiter){
			wstring::size_type i = 0;
			wstring wks(L"");
			for (i = 0; i < line.size(); i++){
				if (line[i] == delimiter){
					if (wks != L""){
						wstrvec.push_back(wks);
					}
					wks = L"";
				}
				else{
					wks += line[i];
				}
			}
			wstrvec.push_back(wks);
		}

		enum FloatModify{
			Default = 0,
			Fixed,
			Scientific,
		};

		//--------------------------------------------------------------------------------------
		//	static wstring FloatToWStr(
		//	float Val,				//���������_�l
		//	streamsize Precision = 0,	//���x0�Ńf�t�H���g
		//	FloatModify Modify = FloatModify::Default	//�\���`��
		//	)
		/*!
		@breaf
		���������_�𕶎���ɕϊ�����iwstring�Łj
		@param float Val,				���������_�l
		@param streamsize Precision = 0,	���x0�Ńf�t�H���g
		@param FloatModify Modify = FloatModify::Default	�\���`��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring FloatToWStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//���������_�̐��x�����߂�
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static string FloatToStr(
		//	float Val,				//���������_�l
		//	streamsize Precision = 0,	//���x0�Ńf�t�H���g
		//	FloatModify Modify = FloatModify::Default	//�\���`��
		//	)
		/*!
		@breaf
		���������_�𕶎���ɕϊ�����istring�Łj
		@param float Val,				���������_�l
		@param streamsize Precision = 0,	���x0�Ńf�t�H���g
		@param FloatModify Modify = FloatModify::Default	�\���`��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string FloatToStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//���������_�̐��x�����߂�
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}


		enum NumModify{
			Dec = 0,
			Hex,
			Oct,
		};
		//--------------------------------------------------------------------------------------
		//	static wstring UintToWStr(
		//	UINT num,	//���l
		//	NumModify Modify = NumModify::Dec	//�i��
		//	)
		/*!
		@breaf
		unsigned���l�𕶎���ɕϊ�����iwstring�Łj
		@param UINT num,				unsigned���l
		@param NumModify Modify = NumModify::Dec	�i��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring UintToWStr(UINT num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		//	static string UintToStr(
		//	UINT num,
		//	NumModify Modify = NumModify::Dec
		//	)
		/*!
		@breaf
		unsigned���l�𕶎���ɕϊ�����istring�Łj
		@param UINT num,				unsigned���l
		@param NumModify Modify = NumModify::Dec	�i��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string UintToStr(UINT num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static wstring IntToWStr(
		//	int num,	//signed���l
		//	NumModify Modify = NumModify::Dec	//�i��
		//	)
		/*!
		@breaf
		signed���l�𕶎���ɕϊ�����iwstring�Łj
		@param int num,		signed���l
		@param NumModify Modify = NumModify::Dec	�i��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring IntToWStr(int num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static string IntToStr(
		//	int num,	//signed���l
		//	NumModify Modify = NumModify::Dec	//�i��
		//	)
		/*!
		@breaf
		signed���l�𕶎���ɕϊ�����istring�Łj
		@param int num,		signed���l
		@param NumModify Modify = NumModify::Dec	�i��
		@return�@�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string IntToStr(int num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static bool DivProbability(
		//	UINT Probability	//�m���B������1�����w�肷��
		//	);
		/*!
		@breaf
		Probability����1�̊m���ŁAtrue��Ԃ�<br />
		Probability��0���傫���Ȃ���΂Ȃ�Ȃ��i0���Z�͂ł��Ȃ��j
		@param UINT Probability	�m���B������1�����w�肷��
		@return�@Probability����1�̊m���ɂȂ��true
		*/
		//--------------------------------------------------------------------------------------
		static bool DivProbability(UINT Probability){
			if (Probability <= 0){
				return false;
			}
			double Prob = (double)rand() / (double)RAND_MAX;
			double Seed = 1.0 / (double)Probability;
			if (Prob <= Seed){
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		//	static float RandZeroToOne(
		//	bool ZeroOK = false
		//	);
		/*!
		@breaf
		0����1.0f�̊Ԃ̗�����Ԃ�<br />
		ZeroOK��false�̏ꍇ�A0���Ԃ�\���͂Ȃ��i0.00001f�ɂȂ�j
		@param bool ZeroOK = false	0���Ԃ��Ă������ǂ����̎w��
		@return�@0����1.0f�̊Ԃ̗���
		*/
		//--------------------------------------------------------------------------------------
		static float RandZeroToOne(bool ZeroOK = false){
			double Prob = (double)rand() / (double)RAND_MAX;
			if (Prob <= 0){
				if (!ZeroOK){
					Prob = 0.00001;
				}
				else{
					Prob = 0.0;
				}
			}
			return (float)Prob;
		}

	};

	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;


	//--------------------------------------------------------------------------------------
	//	class StepTimer;
	/*!
	�X�e�b�v�^�C�}�[
	*/
	//--------------------------------------------------------------------------------------
	class StepTimer{
	public:
		//--------------------------------------------------------------------------------------
		//	StepTimer();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^�̎��g�����擾�ł��܂���B",
					L"if (!QueryPerformanceFrequency(&m_qpcFrequency))",
					L"StepTimer::StepTimer()"
					);
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::StepTimer()"
					);
			}

			// �ő�f���^�� 1 �b�� 1/10 �ɏ��������܂��B
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		//--------------------------------------------------------------------------------------
		//	uint64 GetElapsedTicks() const;
		/*!
		@breaf �O�� Update �Ăяo������o�߂����J�E���^���擾.
		@param �Ȃ�
		@return	�o�߃J�E���^
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetElapsedTicks() const						{ return m_elapsedTicks; }
		//--------------------------------------------------------------------------------------
		//	double GetElapsedSeconds() const;
		/*!
		@breaf �O�� Update �Ăяo������o�߂������Ԃ��擾.
		@param �Ȃ�
		@return	�o�ߎ���
		*/
		//--------------------------------------------------------------------------------------
		double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }
		//--------------------------------------------------------------------------------------
		//	uint64 GetTotalTicks() const;
		/*!
		@breaf �v���O�����J�n����o�߂������v�J�E���^���擾.
		@param �Ȃ�
		@return	���v�J�E���^
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetTotalTicks() const						{ return m_totalTicks; }
		//--------------------------------------------------------------------------------------
		//	double GetTotalSeconds() const;
		/*!
		@breaf �v���O�����J�n����o�߂������v���Ԃ��擾.
		@param �Ȃ�
		@return	���v����
		*/
		//--------------------------------------------------------------------------------------
		double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }
		//--------------------------------------------------------------------------------------
		//	uint32 GetFrameCount() const;
		/*!
		@breaf �v���O�����J�n����̍��v�X�V�񐔂��擾.
		@param �Ȃ�
		@return	���v�X�V��
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFrameCount() const						{ return m_frameCount; }

		//--------------------------------------------------------------------------------------
		//	uint32 GetFramesPerSecond() const;
		/*!
		@breaf ���݂̃t���[�� ���[�g���擾.
		@param �Ȃ�
		@return	���݂̃t���[�� ���[�g
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFramesPerSecond() const					{ return m_framesPerSecond; }

		//--------------------------------------------------------------------------------------
		//	void SetFixedTimeStep(
		//		bool isFixedTimestep	//�Œ胂�[�h�ɂ��邩�ǂ���
		//	);
		/*!
		@breaf �Œ�܂��͉ς̂ǂ���̃^�C���X�e�b�v ���[�h���g�p���邩��ݒ�i�f�t�H���g�͉ρj.
		@param bool isFixedTimestep	�Œ胂�[�h�ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }

		//--------------------------------------------------------------------------------------
		//	void SetTargetElapsedTicks(
		//		uint64 targetElapsed	//�Ăяo���p�x�i�J�E���^�j
		//	);
		/*!
		@breaf �Œ�^�C���X�e�b�v ���[�h�ŁAUpdate �̌Ăяo���p�x��ݒ�.
		@param uint64 targetElapsed		�Ăяo���p�x�i�J�E���^�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedTicks(uint64 targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
		//--------------------------------------------------------------------------------------
		//	void SetTargetElapsedSeconds(
		//		double targetElapsed	//�Ăяo���p�x�i���ԁj
		//	);
		/*!
		@breaf �Œ�^�C���X�e�b�v ���[�h�ŁAUpdate �̌Ăяo���p�x��ݒ�.
		@param double targetElapsed		�Ăяo���p�x�i���ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// �����`���� 1 �b������ 10,000,000 �e�B�b�N���g�p���Ď��Ԃ�\���܂��B
		static const uint64 TicksPerSecond = 10000000;

		//--------------------------------------------------------------------------------------
		//	static double TicksToSeconds(
		//		uint64 ticks	//�`�b�N�J�E���^
		//	);
		/*!
		@breaf �`�b�N�J�E���^�����ԂɊ��Z.
		@param uint64 ticks		�`�b�N�J�E���^
		@return	���Z���ꂽ����
		*/
		//--------------------------------------------------------------------------------------
		static double TicksToSeconds(uint64 ticks)			{ return static_cast<double>(ticks) / TicksPerSecond; }
		//--------------------------------------------------------------------------------------
		//	static uint64 SecondsToTicks(
		//		double seconds	//����
		//	);
		/*!
		@breaf ���Ԃ��`�b�N�J�E���^�Ɋ��Z.
		@param double seconds	����
		@return	���Z���ꂽ�`�b�N�J�E���^
		*/
		//--------------------------------------------------------------------------------------
		static uint64 SecondsToTicks(double seconds)		{ return static_cast<uint64>(seconds * TicksPerSecond); }
		//--------------------------------------------------------------------------------------
		//	void ResetElapsedTime();
		/*!
		@breaf �Ăяo���̍X�V<br />
		�Ӑ}�I�ȃ^�C�~���O�̕s�A�����̌� (�u���b�N IO ����Ȃ�)<br />
		������Ăяo���ƁA�Œ�^�C���X�e�b�v ���W�b�N�ɂ���Ĉ�A�̃L���b�`�A�b�v�����s�����̂�����ł���
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		//--------------------------------------------------------------------------------------
		//	template<typename TUpdate>
		//	void Tick(
		//		const TUpdate& update	//�Ăяo���ׂ�Update�֐�
		//	)
		/*!
		@breaf �^�C�}�[��Ԃ��X�V���A�w��� Update �֐���K�؂ȉ񐔂����Ăяo��
		@param �Ăяo���ׂ�Update�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// ���݂̎������N�G�����܂��B
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// �ɒ[�ɑ傫�Ȏ��ԍ� (�f�o�b�K�[�ňꎞ��~������Ȃ�)  ���N�����v���܂��B
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// QPC �P�ʂ�W���̖ڐ���`���ɕϊ����܂��B�O�̃N�����v�������������߁A���̕ϊ��ł̓I�[�o�[�t���[���s�\�ł��B
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// �Œ�^�C���X�e�b�v�X�V���W�b�N

				// �N�����v�����^�[�Q�b�g�o�ߎ��� (1/4 �~���b�ȓ�) �ɐڋ߂��āA�A�v���P�[�V���������s����Ă���ꍇ
				// �^�[�Q�b�g�l�ɐ��m�Ɉ�v������N���b�N�B����ɂ��A���������֌W�ȃG���[�̔�����h���܂��B
				// �����ԓ��ɒ~�ς��ꂽ�B���̃N�����v�Ȃ��ŁA60 fps ��v�������Q�[����
				// �Œ肳�ꂽ�X�V (59.94 NTSC �f�B�X�v���C��ŗL���� vsync �Ŏ��s) �͍ŏI�I�ɂ́A
				// �t���[�����h���b�v����̂ɏ\���ȏ������G���[��~�ς��܂��B�ۂ߂邱�Ƃ������߂��܂�
				// �킸���ȕ΍����[���Ɏ����Ă����A�X���[�Y�ɐi�s����悤�ɂ��܂��B

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// �σ^�C���X�e�b�v�X�V���W�b�N�B
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// ���݂̃t���[�� ���[�g��ǐՂ��܂��B
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// �\�[�X �^�C�~���O �f�[�^�ł� QPC �P�ʂ��g�p���܂��B
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// �h���^�C�~���O �f�[�^�ł́A�W���̖ڐ���`�����g�p���܂��B
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// �t���[�� ���[�g�̒ǐ՗p�����o�[�B
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// �Œ�^�C���X�e�b�v ���[�h�̍\���p�����o�[�B
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;

		//�R�s�[�֎~
		StepTimer(const StepTimer&) = delete;
		StepTimer& operator=(const StepTimer&) = delete;
		//���[�u�֎~
		StepTimer(const StepTimer&&) = delete;
		StepTimer& operator=(const StepTimer&&) = delete;

	};





}

//end basedx11
