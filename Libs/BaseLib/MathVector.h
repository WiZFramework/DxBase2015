
//--------------------------------------------------------------------------------------
//	MathVector.h
//
//	�x�N�g���v�Z�̌v�Z�N���X
//	XNAMATH �̃��b�s���O�N���X�Q
//
//	Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
//	DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
//--------------------------------------------------------------------------------------

#pragma once

#include "StdAfx.h"

namespace basedx11{

	struct Vector2;
	struct Vector3;
	struct Vector4;

	//--------------------------------------------------------------------------------------
	//	struct Vector2 : public XMFLOAT2;
	//	�p�r: 2����XMVECTOR�̃��b�s���O�\����
	//--------------------------------------------------------------------------------------
	struct Vector2 : public XMFLOAT2{
		//--------------------------------------------------------------------------------------
		//	Vector2();
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//	�������̏��������@�̒�`
		//--------------------------------------------------------------------------------------
		Vector2() :XMFLOAT2(){
			x = 0;
			y = 0;
		}
		Vector2(const XMFLOAT2& v2) :XMFLOAT2(v2){
		}
		Vector2(const XMVECTOR& other) :XMFLOAT2(){
			XMVECTOR temp = other;
			XMStoreFloat2(this, temp);
		}
		Vector2(float x, float y) :
			XMFLOAT2(x, y){
		}
		Vector2(const Vector2& other) :XMFLOAT2(){
			x = other.x;
			y = other.y;
		}
		Vector2(wstring& x_str, wstring& y_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT2 temp = *this;
			XMVECTOR Vec = XMLoadFloat2(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
		}
		void Set(float xx, float yy){
			x = xx;
			y = yy;
		}
		void Zero(){
			x = 0;
			y = 0;
		}
		//--------------------------------------------------------------------------------------
		//	���d��`
		//--------------------------------------------------------------------------------------
		//���
		Vector2& operator=(const Vector2& other){
			//���ȑ���̗}��
			if (this != &other){
				x = other.x;
				y = other.y;
			}
			return *this;
		}
		Vector2& operator=(const XMVECTOR& other){
			XMVECTOR temp = other;
			XMStoreFloat2(this, temp);
			return *this;
		}
		//�ǉ����Z
		Vector2& operator+=(const Vector2& other){
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2& operator-=(const Vector2& other){
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2& operator*=(float val){
			x *= val;
			y *= val;
			return *this;
		}
		Vector2& operator/=(float val){
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector2::operator/=()"
					);
			}
			x /= val;
			y /= val;
			return *this;
		}
		//�P�����Z�q
		Vector2 operator+ () const{
			return *this;
		}
		Vector2 operator- () const{
			Vector2 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//���Z
		Vector2 operator+(const Vector2& other)const{
			Vector2 ret = *this;
			ret += other;
			return ret;
		}
		Vector2 operator-(const Vector2& other)const{
			Vector2 ret = *this;
			ret -= other;
			return ret;
		}
		Vector2 operator*(float val)const{
			Vector2 ret = *this;
			ret *= val;
			return ret;
		}
		Vector2 operator/(float val)const{
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector4::operator/()"
					);
			}
			Vector2 ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](int i){
			if (i > 1 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 1 || i < 0)",
					L"Vector2::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			}
		}
		const float& operator[](int i) const{
			if (i > 1 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 1 || i < 0)",
					L"Vector2::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			}
		}

		//��r
		//--------------------------------------------------------------------------------------
		//	bool Equal(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���
		//	�߂�l: ���������true
		//--------------------------------------------------------------------------------------
		bool Equal(const Vector2& other)const{
			return XMVector2Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector2& other)const{
			return Equal(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool EqualInt(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���B
		//	���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		//	�߂�l: ���������true
		//--------------------------------------------------------------------------------------
		bool EqualInt(const Vector2& other)const{
			return XMVector2EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		//	bool Greater(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g�����傫�����ǂ��������؂���B
		//	�߂�l: �傫�����true
		//--------------------------------------------------------------------------------------
		bool Greater(const Vector2& other)const{
			return XMVector2Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//���傫������r
		bool operator>(const Vector2& other)const{
			return Greater(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool GreaterOrEqual(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
		//	�߂�l: �ȏ�̑傫���Ȃ��true
		//--------------------------------------------------------------------------------------
		bool GreaterOrEqual(const Vector2& other)const{
			return XMVector2GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȏ�̑傫������r
		bool operator >= (const Vector2& other)const{
			return GreaterOrEqual(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool IsInfinite() const;
		//	�p�r: this�̒l�̂����ꂩ���������̖����傩�ǂ��������؂���B
		//	�߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
		//--------------------------------------------------------------------------------------
		bool IsInfinite() const{
			return XMVector2IsInfinite(XMVECTOR(*this));
		}
		//--------------------------------------------------------------------------------------
		//	bool IsIsNaN() const;
		//	�p�r: this�̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
		//	�߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
		//--------------------------------------------------------------------------------------
		bool IsIsNaN() const{
			return XMVector2IsNaN(XMVECTOR(*this));
		}
		//--------------------------------------------------------------------------------------
		//	bool Less(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g����菬�������ǂ��������؂���B
		//	�߂�l: ���������true
		//--------------------------------------------------------------------------------------
		bool Less(const Vector2& other)const{
			return XMVector2Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//��菬��������r
		bool operator<(const Vector2& other)const{
			return Less(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool LessOrEqual(
		//	const Vector2& other	//��r����x�N�g��
		//	) const;
		//	�p�r: this��������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
		//	�߂�l: �ȉ��̑傫���Ȃ��true
		//--------------------------------------------------------------------------------------
		bool LessOrEqual(const Vector2& other)const{
			return XMVector2LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȉ��̑傫������r
		bool operator <= (const Vector2& other)const{
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector2& other,	//��r����x�N�g��
		float Epsilon			//��r����ő勖�e��
		) const;
		�p�r: this��������̃x�N�g�����߂��傫�����ǂ��������؂���B
		�߂�l: �߂��傫���Ȃ��true
		***************************************************************************/
		bool NearEqual(const Vector2& other, float Epsilon)const{
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector2NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector2& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqual(const Vector2& other)const{
			return XMVector2NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//��r
		bool operator!=(const Vector2& other)const{
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector2& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqualInt(const Vector2& other)const{
			return XMVector2NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//�W�I���g��
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector2& other	//������̃x�N�g��
		)const;
		�p�r: ���K������this�Ƃ�����̂̐��K�������x�N�g���Ƃ�
		�@�@�@���W�A���p���v�Z���Ԃ�
		   �߂�l: ���W�A���p
		   �����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		   ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		   ��this�͕ύX���Ȃ�
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector2& other)const{
			return ((Vector2)XMVector2AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector2& other	//��r����x�N�g��
		)const;
		�p�r: ���K�����ꂽthis�Ɛ��K�����ꂽ������̂̃x�N�g���Ԃ̃��W�A���p��\�����ĕԂ��B
		�߂�l: ���W�A���p
		�����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector2& other)const{
			return ((Vector2)XMVector2AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector2& other	//��r����x�N�g��
		)const;
		�p�r: this�Ƃ�����̂̃x�N�g���Ԃ̃��W�A���p���v�Z����B
		�߂�l: ���W�A���p
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float AngleBetweenVectors(const Vector2& other)const{
			return ((Vector2)XMVector2AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//�����̍ŏ��l
		float LengthMax		//�����̍ő�l
		);
		�p�r: this���x�N�g���̒������w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax){
			*this = XMVector2ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector2& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
		const Vector2& LengthMaxV		//�����̍ő�l(�x�N�g��)
		);
		�p�r: this���w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLengthV(const Vector2& LengthMinV, const Vector2& LengthMaxV){
			*this = XMVector2ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		float Cross(
		const Vector2& other		//����̃x�N�g��
		)const;
		�p�r: this�Ƃ̊O�ς�Ԃ�
		�߂�l: �O��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Cross(const Vector2& other)const{
			return ((Vector2)XMVector2Cross(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float Dot(
		const Vector2& other	//����̃x�N�g��
		)const;
		�p�r: this�Ƃ̓��ς�Ԃ�
		�߂�l: ����
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Dot(const Vector2& other)const{
			return ((Vector2)XMVector2Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector2& other	//��r����x�N�g��
		)const;
		�p�r: this�̂��ׂĂ̗v�f���A�ݒ肳�ꂽ���E���ɂ���ꍇ��true��Ԃ�
		�߂�l: ���E���ɂ���ꍇ��true
		��this�͕ύX���Ȃ�
		***************************************************************************/
		bool InBounds(const Vector2& other)const{
			return XMVector2InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		�p�r: �x�N�g���̒�����Ԃ�
		�߂�l: �x�N�g���̒���
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Length() const{
			return  ((Vector2)XMVector2Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		�p�r: �x�N�g���̒�����\�z���ĕԂ�
		�߂�l: �x�N�g���̒����̗\�z
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthEst() const{
			return ((Vector2)XMVector2LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		�p�r: �x�N�g���̒�����2���Ԃ�
		�߂�l: �x�N�g���̒�����2��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthSq() const{
			return ((Vector2)XMVector2LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		�p�r: ���K������
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Normalize(){
			*this = XMVector2Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		�p�r: ���K���̗\�z�l��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void NormalizeEst(){
			*this = XMVector2NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		�p�r: this�ɐ����ȃx�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Orthogonal(){
			*this = XMVector2Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		�p�r: this�̒����̋t����Ԃ�
		�߂�l: this�̒����̋t��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLength()const{
			return ((Vector2)XMVector2ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		�p�r: this�̒����̋t���̗\���l��Ԃ�
		�߂�l: this�̒����̋t���̗\���l
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLengthEst()const{
			return ((Vector2)XMVector2ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector2& Normal
		);
		�p�r: this�ɖ@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Reflect(const Vector2& Normal){
			*this = XMVector2Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector2& Normal,	//���܂�����@���x�N�g��
		float RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Refract(const Vector2& Normal, float RefractionIndex){
			*this = XMVector2Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector2& Normal,	//���܂�����@���x�N�g��
		const Vector2& RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
		�@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		   �߂�l: �Ȃ�
		   ��this�Ɍ��ʂ�ݒ肷��
		   ***************************************************************************/
		void RefractV(const Vector2& Normal, const Vector2& RefractionIndex){
			*this = XMVector2RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//�g�����X�t�H�[��
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//�g�����X�t�H�[������s��
		);
		�p�r: this��m�ɂ���ăg�����X�t�H�[�����ꂽ�x�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Transform(const XMMATRIX& m){
			*this = XMVector2Transform(XMVECTOR(*this), m);
		}
	};

	/**************************************************************************
	struct Vector3 : public XMFLOAT3;
	�p�r: 3����XMVECTOR�̃��b�s���O�\����
	****************************************************************************/
	struct Vector3 : public XMFLOAT3{
		/**************************************************************************
		Vector3();
		�p�r: �R���X�g���N�^
		�߂�l: �Ȃ�
		�������̏��������@�̒�`
		***************************************************************************/
		Vector3() :XMFLOAT3(){
			x = 0;
			y = 0;
			z = 0;
		}
		Vector3(const XMFLOAT3& v3) :XMFLOAT3(v3){
		}
		Vector3(const XMVECTOR& other) :XMFLOAT3(){
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
		}
		Vector3(float x, float y, float z) :
			XMFLOAT3(x, y, z){
		}
		Vector3(const Vector3& other) :XMFLOAT3(){
			x = other.x;
			y = other.y;
			z = other.z;
		}
		Vector3(wstring& x_str, wstring& y_str, wstring& z_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT3 temp = *this;
			XMVECTOR Vec = XMLoadFloat3(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
		}
		void Set(float xx, float yy, float zz){
			x = xx;
			y = yy;
			z = zz;
		}
		void Zero(){
			x = 0;
			y = 0;
			z = 0;
		}
		/**************************************************************************
		���d��`
		***************************************************************************/
		//���
		Vector3& operator=(const Vector3& other){
			//���ȑ���̗}��
			if (this != &other){
				x = other.x;
				y = other.y;
				z = other.z;
			}
			return *this;
		}
		Vector3& operator=(const XMVECTOR& other){
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
			return *this;
		}
		//�ǉ����Z
		Vector3& operator+=(const Vector3& other){
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& other){
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3& operator*=(float val){
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}
		Vector3& operator/=(float val){
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector3::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}
		//�P�����Z�q
		Vector3 operator+ () const{
			return *this;
		}
		Vector3 operator- () const{
			Vector3 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//���Z
		Vector3 operator+(const Vector3& other)const{
			Vector3 ret = *this;
			ret += other;
			return ret;
		}
		Vector3 operator-(const Vector3& other)const{
			Vector3 ret = *this;
			ret -= other;
			return ret;
		}
		Vector3 operator*(float val)const{
			Vector3 ret = *this;
			ret *= val;
			return ret;
		}
		Vector3 operator/(float val)const{
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector3::operator/()"
					);
			}
			Vector3 ret = *this;
			ret /= val;
			return ret;
		}
		float& operator[](int i){
			if (i > 2 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 2 || i < 0)",
					L"Vector3::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			}
			return x;
		}
		const float& operator[](int i) const{
			if (i > 2 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 2 || i < 0)",
					L"Vector3::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			}
			return x;
		}
		//��r
		/**************************************************************************
		bool Equal(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���
		�߂�l: ���������true
		***************************************************************************/
		bool Equal(const Vector3& other)const{
			return XMVector3Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector3& other)const{
			return Equal(other);
		}
		/**************************************************************************
		bool EqualInt(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���B
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		�߂�l: ���������true
		***************************************************************************/
		bool EqualInt(const Vector3& other)const{
			return XMVector3EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		bool Greater(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g�����傫�����ǂ��������؂���B
		�߂�l: �傫�����true
		***************************************************************************/
		bool Greater(const Vector3& other)const{
			return XMVector3Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//���傫������r
		bool operator>(const Vector3& other)const{
			return Greater(other);
		}
		/**************************************************************************
		bool GreaterOrEqual(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
		�߂�l: �ȏ�̑傫���Ȃ��true
		***************************************************************************/
		bool GreaterOrEqual(const Vector3& other)const{
			return XMVector3GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȏ�̑傫������r
		bool operator >= (const Vector3& other)const{
			return GreaterOrEqual(other);
		}
		/**************************************************************************
		bool IsInfinite() const;
		�p�r: this�̒l�̂����ꂩ���������̖����傩�ǂ��������؂���B
		�߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
		***************************************************************************/
		bool IsInfinite() const{
			return XMVector3IsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsIsNaN() const;
		�p�r: this�̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
		�߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
		***************************************************************************/
		bool IsIsNaN() const{
			return XMVector3IsNaN(XMVECTOR(*this));
		}
		/**************************************************************************
		bool Less(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g����菬�������ǂ��������؂���B
		�߂�l: ���������true
		***************************************************************************/
		bool Less(const Vector3& other)const{
			return XMVector3Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//��菬��������r
		bool operator<(const Vector3& other)const{
			return Less(other);
		}
		/**************************************************************************
		bool LessOrEqual(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
		�߂�l: �ȉ��̑傫���Ȃ��true
		***************************************************************************/
		bool LessOrEqual(const Vector3& other)const{
			return XMVector3LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȉ��̑傫������r
		bool operator <= (const Vector3& other)const{
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector3& other,	//��r����x�N�g��
		float Epsilon			//��r����ő勖�e��
		) const;
		�p�r: this��������̃x�N�g�����߂��傫�����ǂ��������؂���B
		�߂�l: �߂��傫���Ȃ��true
		***************************************************************************/
		bool NearEqual(const Vector3& other, float Epsilon)const{
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector3NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqual(const Vector3& other)const{
			return XMVector3NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//��r
		bool operator!=(const Vector3& other)const{
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqualInt(const Vector3& other)const{
			return XMVector3NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//�W�I���g��
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector3& other	//������̃x�N�g��
		) const;
		�p�r: ���K������this�Ƃ�����̂̐��K�������x�N�g���Ƃ�
		�@�@�@���W�A���p���v�Z�ݒ肷��B
		   �߂�l: ���W�A���p
		   �����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		   ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		   ��this�͕ύX���Ȃ�
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector3& other) const{
			return ((Vector3)XMVector3AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector3& other	//��r����x�N�g��
		)const;
		�p�r: ���K�����ꂽthis�Ɛ��K�����ꂽ������̂̃x�N�g���Ԃ̃��W�A���p��\���ݒ肷��B
		�߂�l: ���W�A���p
		�����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector3& other)const{
			return ((Vector3)XMVector3AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this�Ƃ�����̂̃x�N�g���Ԃ̃��W�A���p���v�Z����B
		�߂�l: �Ȃ�
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float AngleBetweenVectors(const Vector3& other) const{
			return ((Vector3)XMVector3AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//�����̍ŏ��l
		float LengthMax		//�����̍ő�l
		);
		�p�r: this���x�N�g���̒������w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax){
			*this = XMVector3ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector3& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
		const Vector3& LengthMaxV		//�����̍ő�l(�x�N�g��)
		);
		�p�r: this���w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLengthV(const Vector3& LengthMinV, const Vector3& LengthMaxV){
			*this = XMVector3ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		void Cross(
		const Vector3& other,		//����̃x�N�g���P
		);
		�p�r: this�Ƃ̊O�ς��v�Z��this�ɐݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Cross(const Vector3& other){
			*this = (Vector3)XMVector3Cross(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Dot(
		const Vector3& other	//����̃x�N�g��
		)const;
		�p�r: this�Ƃ̓��ς��v�Z����
		�߂�l: ����
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Dot(const Vector3& other)const{
			return ((Vector3)XMVector3Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector3& other	//��r����x�N�g��
		) const;
		�p�r: this�̂��ׂĂ̗v�f���A�ݒ肳�ꂽ���E���ɂ���ꍇ��true��Ԃ�
		�߂�l: ���E���ɂ���ꍇ��true
		��this�͕ύX���Ȃ�
		***************************************************************************/
		bool InBounds(const Vector3& other) const{
			return XMVector3InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		�p�r: �x�N�g���̒�����Ԃ�
		�߂�l: �x�N�g���̒���
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Length() const{
			return ((Vector3)XMVector3Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		�p�r: �x�N�g���̒�����\�z���ĕԂ�
		�߂�l: �x�N�g���̒����̗\�z
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthEst() const{
			return ((Vector3)XMVector3LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		�p�r: �x�N�g���̒�����2���Ԃ�
		�߂�l: �x�N�g���̒�����2��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthSq() const{
			return ((Vector3)XMVector3LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		�p�r: ���K������
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Normalize(){
			*this = XMVector3Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		�p�r: ���K���̗\�z�l��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void NormalizeEst(){
			*this = XMVector3NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		�p�r: this�ɐ����ȃx�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Orthogonal(){
			*this = XMVector3Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		�p�r: this�̒����̋t����Ԃ�
		�߂�l: this�̒����̋t��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLength()const{
			return ((Vector3)XMVector3ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		�p�r: this�̒����̋t���̗\���l��Ԃ�
		�߂�l: this�̒����̋t���̗\���l
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLengthEst()const{
			return ((Vector3)XMVector3ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector3& Normal
		);
		�p�r: this�ɖ@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Reflect(const Vector3& Normal){
			*this = XMVector3Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector3& Normal,	//���܂�����@���x�N�g��
		float RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Refract(const Vector3& Normal, float RefractionIndex){
			*this = XMVector3Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector3& Normal,	//���܂�����@���x�N�g��
		const Vector3& RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
		�@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		   �߂�l: �Ȃ�
		   ��this�Ɍ��ʂ�ݒ肷��
		   ***************************************************************************/
		void RefractV(const Vector3& Normal, const Vector3& RefractionIndex){
			*this = XMVector3RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//�g�����X�t�H�[��
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//�g�����X�t�H�[������s��
		);
		�p�r: this��m�ɂ���ăg�����X�t�H�[�����ꂽ�x�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Transform(const XMMATRIX& m){
			*this = XMVector3Transform(XMVECTOR(*this), m);
		}
	};


	/**************************************************************************
	struct Vector4 : public XMFLOAT4;
	�p�r: 4����XMVECTOR�̃��b�s���O�\����
	****************************************************************************/
	struct Vector4 : public XMFLOAT4{
		/**************************************************************************
		Vector4();
		�p�r: �R���X�g���N�^
		�߂�l: �Ȃ�
		�������̏��������@�̒�`
		***************************************************************************/
		Vector4() :XMFLOAT4(){
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		Vector4(const XMFLOAT4& v4) :XMFLOAT4(v4){
		}
		Vector4(const XMVECTOR& other) :XMFLOAT4(){
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		Vector4(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w){
		}
		Vector4(const Vector4& other) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		Vector4(const Vector3& other) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
		}
		Vector4(const Vector3& other, float ww) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = ww;
		}
		Vector4(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		void Set(float xx, float yy, float zz, float ww){
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}
		void Zero(){
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		/**************************************************************************
		���d��`
		***************************************************************************/
		//���
		Vector4& operator=(const Vector4& other){
			//���ȑ���̗}��
			if (this != &other){
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		Vector4& operator=(const Vector3& other){
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
			return *this;
		}
		Vector4& operator=(const XMVECTOR& other){
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//�ǉ����Z
		Vector4& operator+=(const Vector4& other){
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		Vector4& operator-=(const Vector4& other){
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		Vector4& operator*=(float val){
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		Vector4& operator/=(float val){
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector4::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}
		//�P�����Z�q
		Vector4 operator+ () const{
			return *this;
		}
		Vector4 operator- () const{
			Vector4 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//���Z
		Vector4 operator+(const Vector4& other)const{
			Vector4 ret = *this;
			ret += other;
			return ret;
		}
		Vector4 operator-(const Vector4& other)const{
			Vector4 ret = *this;
			ret -= other;
			return ret;
		}
		Vector4 operator*(float val)const{
			Vector4 ret = *this;
			ret *= val;
			return ret;
		}
		Vector4 operator/(float val)const{
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"Vector4::operator/()"
					);
			}
			Vector4 ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](int i){
			if (i > 3 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 3 || i < 0)",
					L"Vector4::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		const float& operator[](int i) const{
			if (i > 2 || i < 0){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 3 || i < 0)",
					L"Vector4::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}

		//��r
		/**************************************************************************
		bool Equal(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���
		�߂�l: ���������true
		***************************************************************************/
		bool Equal(const Vector4& other)const{
			return XMVector4Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector4& other)const{
			return Equal(other);
		}
		/**************************************************************************
		bool EqualInt(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ��������ǂ��������؂���B
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		�߂�l: ���������true
		***************************************************************************/
		bool EqualInt(const Vector4& other)const{
			return XMVector4EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		bool Greater(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g�����傫�����ǂ��������؂���B
		�߂�l: �傫�����true
		***************************************************************************/
		bool Greater(const Vector4& other)const{
			return XMVector4Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//���傫������r
		bool operator>(const Vector4& other)const{
			return Greater(other);
		}
		/**************************************************************************
		bool GreaterOrEqual(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
		�߂�l: �ȏ�̑傫���Ȃ��true
		***************************************************************************/
		bool GreaterOrEqual(const Vector4& other)const{
			return XMVector4GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȏ�̑傫������r
		bool operator >= (const Vector4& other)const{
			return GreaterOrEqual(other);
		}
		/**************************************************************************
		bool IsInfinite() const;
		�p�r: this�̒l�̂����ꂩ���������̖����傩�ǂ��������؂���B
		�߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
		***************************************************************************/
		bool IsInfinite() const{
			return XMVector4IsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsIsNaN() const;
		�p�r: this�̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
		�߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
		***************************************************************************/
		bool IsIsNaN() const{
			return XMVector4IsNaN(XMVECTOR(*this));
		}
		/**************************************************************************
		bool Less(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g����菬�������ǂ��������؂���B
		�߂�l: ���������true
		***************************************************************************/
		bool Less(const Vector4& other)const{
			return XMVector4Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//��菬��������r
		bool operator<(const Vector4& other)const{
			return Less(other);
		}
		/**************************************************************************
		bool LessOrEqual(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
		�߂�l: �ȉ��̑傫���Ȃ��true
		***************************************************************************/
		bool LessOrEqual(const Vector4& other)const{
			return XMVector4LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//�ȉ��̑傫������r
		bool operator <= (const Vector4& other)const{
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector4& other,	//��r����x�N�g��
		float Epsilon			//��r����ő勖�e��
		) const;
		�p�r: this��������̃x�N�g�����߂��傫�����ǂ��������؂���B
		�߂�l: �߂��傫���Ȃ��true
		***************************************************************************/
		bool NearEqual(const Vector4& other, float Epsilon)const{
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector4NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqual(const Vector4& other)const{
			return XMVector4NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//��r
		bool operator!=(const Vector4& other)const{
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this��������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
		�߂�l: �������Ȃ����true
		***************************************************************************/
		bool NotEqualInt(const Vector4& other)const{
			return XMVector4NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//�W�I���g��
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector4& other	//������̃x�N�g��
		)const;
		�p�r: ���K������this�Ƃ�����̂̐��K�������x�N�g���Ƃ�
		�@�@�@���W�A���p��Ԃ��B
		   �߂�l: ���W�A���p
		   ��this�͕ύX���Ȃ�
		   �����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		   ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector4& other)const{
			return ((Vector4)XMVector4AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector4& other	//��r����x�N�g��
		)const;
		�p�r: ���K�����ꂽthis�Ɛ��K�����ꂽ������̂̃x�N�g���Ԃ̃��W�A���p��Ԃ�
		�߂�l: ���W�A���p
		��this�͕ύX���Ȃ�
		�����K���͂��Ȃ��̂ŁAthis�����other�͂��炩���ߐ��K�����Ă���
		���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector4& other)const{
			return ((Vector4)XMVector4AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector4& other	//��r����x�N�g��
		)const;
		�p�r: this�Ƃ�����̂̃x�N�g���Ԃ̃��W�A���p�B
		�߂�l: ���W�A���p
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float AngleBetweenVectors(const Vector4& other)const{
			return ((Vector4)XMVector4AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//�����̍ŏ��l
		float LengthMax		//�����̍ő�l
		);
		�p�r: this���x�N�g���̒������w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax){
			*this = XMVector4ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector4& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
		const Vector4& LengthMaxV		//�����̍ő�l(�x�N�g��)
		);
		�p�r: this���w�肵���傫���ɃN�����v����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void ClampLengthV(const Vector4& LengthMinV, const Vector4& LengthMaxV){
			*this = XMVector4ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		void Cross(
		const Vector4& other1,		//����̃x�N�g���P
		const Vector4& other2		//����̃x�N�g���Q
		);
		�p�r: this�Ƃ̊O�ς�ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Cross(const Vector4& other1, const Vector4& other2){
			*this = XMVector4Cross(XMVECTOR(*this), XMVECTOR(other1), XMVECTOR(other2));
		}
		/**************************************************************************
		float Dot(
		const Vector4& other	//����̃x�N�g��
		)const;
		�p�r: this�Ƃ̓��ς��v�Z����
		�߂�l: ����
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Dot(const Vector4& other)const{
			return ((Vector4)XMVector4Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector4& other	//��r����x�N�g��
		) const;
		�p�r: this�̂��ׂĂ̗v�f���A�ݒ肳�ꂽ���E���ɂ���ꍇ��true��Ԃ�
		�߂�l: ���E���ɂ���ꍇ��true
		��this�͕ύX���Ȃ�
		***************************************************************************/
		bool InBounds(const Vector4& other) const{
			return XMVector4InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		�p�r: �x�N�g���̒�����Ԃ�
		�߂�l: �x�N�g���̒���
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float Length() const{
			return ((Vector4)XMVector4Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		�p�r: �x�N�g���̒�����\�z���ĕԂ�
		�߂�l: �x�N�g���̒����̗\�z
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthEst() const{
			return ((Vector4)XMVector4LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		�p�r: �x�N�g���̒�����2���Ԃ�
		�߂�l: �x�N�g���̒�����2��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float LengthSq() const{
			return ((Vector4)XMVector4LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		�p�r: ���K������
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Normalize(){
			*this = XMVector4Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		�p�r: ���K���̗\�z�l��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void NormalizeEst(){
			*this = XMVector4NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		�p�r: this�ɐ����ȃx�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Orthogonal(){
			*this = XMVector4Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		�p�r: this�̒����̋t����Ԃ�
		�߂�l: this�̒����̋t��
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLength()const{
			return ((Vector4)XMVector4ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		�p�r: this�̒����̋t���̗\���l��Ԃ�
		�߂�l: this�̒����̋t���̗\���l
		��this�͕ύX���Ȃ�
		***************************************************************************/
		float ReciprocalLengthEst()const{
			return ((Vector4)XMVector4ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector4& Normal
		);
		�p�r: this�ɖ@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Reflect(const Vector4& Normal){
			*this = XMVector4Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector4& Normal,	//���܂�����@���x�N�g��
		float RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Refract(const Vector4& Normal, float RefractionIndex){
			*this = XMVector4Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector4& Normal,	//���܂�����@���x�N�g��
		const Vector4& RefractionIndex	//���ܗ�
		);
		�p�r: this�ɖ@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
		�@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��ݒ肷��
		   �߂�l: �Ȃ�
		   ��this�Ɍ��ʂ�ݒ肷��
		   ***************************************************************************/
		void RefractV(const Vector4& Normal, const Vector4& RefractionIndex){
			*this = XMVector4RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//�g�����X�t�H�[��
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//�g�����X�t�H�[������s��
		);
		�p�r: this��m�ɂ���ăg�����X�t�H�[�����ꂽ�x�N�g����ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Transform(const XMMATRIX& m){
			*this = XMVector4Transform(XMVECTOR(*this), m);
		}
	};

	//--------------------------------------------------------------------------------------
	//	struct XMVector : public XMFLOAT4;
	//	�p�r: 1����XMVECTOR�̃��b�s���O�\����
	//	���e�v�f�̑���ȂǁA��{�I��XM�֐��Q
	//--------------------------------------------------------------------------------------
	struct XMVector : public XMFLOAT4{
		/**************************************************************************
		XMVector();
		�p�r: �R���X�g���N�^
		�߂�l: �Ȃ�
		�������̏��������@�̒�`
		***************************************************************************/
		XMVector() :XMFLOAT4(){
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		XMVector(const XMFLOAT4& v4) :XMFLOAT4(v4){
		}
		XMVector(const XMVECTOR& other) :XMFLOAT4(){
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		XMVector(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w){
		}
		XMVector(const XMVector& other) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		XMVector(const Vector4& other) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		XMVector(const Vector3& other) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
		}
		XMVector(const Vector3& other, float ww) :XMFLOAT4(){
			x = other.x;
			y = other.y;
			z = other.z;
			w = ww;
		}
		XMVector(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str){
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		void Set(float xx, float yy, float zz, float ww){
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}
		void Zero(){
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		/**************************************************************************
		���d��`
		***************************************************************************/
		//���
		XMVector& operator=(const XMVector& other){
			//���ȑ���̗}��
			if (this != &other){
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		XMVector& operator=(const Vector3& other){
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
			return *this;
		}
		XMVector& operator=(const XMVECTOR& other){
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//�ǉ����Z
		XMVector& operator+=(const XMVector& other){
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		XMVector& operator-=(const XMVector& other){
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		XMVector& operator*=(float val){
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		XMVector& operator/=(float val){
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"VectorBase::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}
		//�P�����Z�q
		XMVector operator+ () const{
			return *this;
		}
		XMVector operator- () const{
			XMVector ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//���Z
		XMVector operator+(const XMVector& other)const{
			XMVector ret = *this;
			ret += other;
			return ret;
		}
		XMVector operator-(const XMVector& other)const{
			XMVector ret = *this;
			ret -= other;
			return ret;
		}
		XMVector operator*(float val)const{
			XMVector ret = *this;
			ret *= val;
			return ret;
		}
		XMVector operator/(float val)const{
			if (val == 0.0f){
				//0���Z
				throw BaseException(
					L"0���Z�ł�",
					L"if(val == 0.0f)",
					L"VectorBase::operator/()"
					);
			}
			XMVector ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](size_t i){
			if (i > 3){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 3)",
					L"XMVector::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		float operator[](size_t i) const{
			if (i > 3){
				throw BaseException(
					L"�C���f�b�N�X���͈͊O�ł�",
					L"if(i > 3)",
					L"XMVector::operator[]()"
					);
			}
			switch (i){
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		//�Z�p
		/**************************************************************************
		void Abs();
		�p�r: �e�v�f���Βl�ɐݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Abs(){
			*this = XMVectorAbs(XMVECTOR(*this));
		}
		/**************************************************************************
		void Add(
		const XMVector& other	//�v�Z����
		);
		�p�r: �e�v�f��other�̊e�v�f�����Z����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Add(const XMVector& other){
			*this = XMVectorAdd(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void AddAngles(
		const XMVector& other	//�v�Z����
		);
		�p�r: �e�v�f��other�̊e�v�f�p�x���Z����
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		��this��other�́A-XM_PI�ȏ�XM_PI�����ł���K�v������
		***************************************************************************/
		void AddAngles(const XMVector& other){
			*this = XMVectorAddAngles(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void Ceiling(
			const XMVector& other	//�v�Z����
		);
		�p�r: �e�v�f�̏�����v�Z���A�ݒ肷��
		�߂�l: �Ȃ�
		��this�Ɍ��ʂ�ݒ肷��
		***************************************************************************/
		void Ceiling(const XMVector& other){
			*this = XMVectorCeiling(XMVECTOR(other));
		}


	};
}
//end of namespace basedx11.
