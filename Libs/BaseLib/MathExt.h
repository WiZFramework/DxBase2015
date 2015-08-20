
/*!
@file MathExt.h
@brief �s��A�N�I�[�^�j�I���A�J���[�Ȃǂ̌v�Z�N���X<br />
XNAMATH �̃��b�s���O�N���X�Q

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

//--------------------------------------------------------------------------------------
// MathExt.h
//
// �s��A�N�I�[�^�j�I���A�J���[�Ȃǂ̌v�Z�N���X
// XNAMATH �̃��b�s���O�N���X�Q
//
// Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
// DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
//--------------------------------------------------------------------------------------

#pragma once

#include "StdAfx.h"

namespace basedx11{

struct Matrix4X4;

/**************************************************************************
 struct Plane : public XMFLOAT4;
 �p�r: Plane�i���ʁj�\����
****************************************************************************/
struct Plane: public XMFLOAT4{
/**************************************************************************
 Plane();
 �p�r: �R���X�g���N�^
 �߂�l: �Ȃ�
 �������̏��������@�̒�`
***************************************************************************/
	Plane():XMFLOAT4(){}
	Plane(const XMFLOAT4& v4):XMFLOAT4(){
		x = v4.x;
		y = v4.y;
		z = v4.z;
		w = v4.w;
	}
	Plane(const Plane& other):XMFLOAT4(){
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}
	Plane(const XMVECTOR& other):XMFLOAT4(){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
	}
	Plane(const Vector3& Point,const Vector3& Normal){
		*this = (Plane)XMPlaneFromPointNormal(XMVECTOR(Point),XMVECTOR(Normal));
	}
	Plane(const Vector3& Point1,const Vector3& Point2,const Vector3& Point3){
		*this = (Plane)XMPlaneFromPoints(XMVECTOR(Point1),XMVECTOR(Point2),XMVECTOR(Point3));
	}
	operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	void Set(float xx,float yy,float zz,float ww){
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
	Plane& operator=(const Plane& other){
		//���ȑ���̗}��
		if(this != &other){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}
	Plane& operator=(const XMVECTOR& other){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
		return *this;
	}
/**************************************************************************
 float Dot(
	const Vector4& other	//�v�Z����Vector4
 )const;
 �p�r: this��other�̓��ς�Ԃ�
 �߂�l: ����
 ��this�͕ύX���Ȃ�
***************************************************************************/
	float Dot(const Vector4& other){
		return ((Vector4)XMPlaneDot(XMVECTOR(*this),XMVECTOR(other))).x;
	}
/**************************************************************************
 float DotCoord(
	const Vector3& other	//�����Vector3
 ) const;
 �p�r: this��Vector3�̓��ς�Ԃ�
 �߂�l: ����
 ��this�͕ύX���Ȃ�
 ������|�C���g���畽�ʂ܂ł̕����t�����������߂�̂ɕ֗��ł���
***************************************************************************/
	float DotCoord(const Vector3& other){
		return  ((Vector4)XMPlaneDotCoord(XMVECTOR(*this),XMVECTOR(other))).x;
	}
/**************************************************************************
 float DotNormal(
	const Vector3& other	//�����Vector3
 ) const;
 �p�r:�@this�̖@���x�N�g���� 3D �x�N�g���̊Ԃ̓��ς��v�Z���Ԃ�
 �߂�l: ����
 ��this�͕ύX���Ȃ�
 �����ʂ̖@���x�N�g���ƕʂ̖@���x�N�g���Ƃ̊p�x���Z�o����Ƃ��ɖ𗧂�
***************************************************************************/
	float DotNormal(const Vector3& other)const {
		return ((Vector4)XMPlaneDotNormal(XMVECTOR(*this),XMVECTOR(other))).x;
	}
/**************************************************************************
 bool Equal(
	const Plane& other	//�����Plane
 )const;
 �p�r:�@this�Ƒ���̕��ʂ��������ǂ����𒲂ׂ�
 �߂�l: �����Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool Equal(const Plane& other)const{
		return XMPlaneEqual(XMVECTOR(*this),XMVECTOR(other));
	}
/**************************************************************************
 void FromPointNormal(
	const Vector3& Point,	//���ʏ�̓_
	const Vector3& Normal	//�@��
 );
 �p�r:�@�_�Ɩ@�����畽�ʂ��쐬���āAthis�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void FromPointNormal(const Vector3& Point,const Vector3& Normal){
		*this = (Plane)XMPlaneFromPointNormal(XMVECTOR(Point),XMVECTOR(Normal));
	}
/**************************************************************************
 void FromPoints(
	const Vector3& Point1,	//���_1
	const Vector3& Point2,	//���_2
	const Vector3& Point3	//���_3
 );
 �p�r:�@�R�_���畽�ʂ��쐬���āAthis�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void FromPoints(const Vector3& Point1,const Vector3& Point2,const Vector3& Point3){
		*this = (Plane)XMPlaneFromPoints(XMVECTOR(Point1),XMVECTOR(Point2),XMVECTOR(Point3));
	}
/**************************************************************************
 Vector3 IntersectLine(
	const Vector3& LinePoint1,	//���_1
	const Vector3& LinePoint2	//���_2
 )const;
 �p�r:�@this�ƂQ�̒��_���쐬���钼���̌������钸�_��Ԃ�
 �߂�l: this�ƌ�������R�c�x�N�g��
 ��this�͕ύX���Ȃ�
 �����ʂƒ��������s�̏ꍇ�́A�߂�e�l��QNaN�i�񐔁j �ɂȂ�̂ŁA
 �@Vector3::IsNaN()�ȂǂŃ`�F�b�N����
***************************************************************************/
	Vector3 IntersectLine(const Vector3& LinePoint1,const Vector3& LinePoint2)const{
		return (Vector3)XMPlaneIntersectLine(XMVECTOR(*this),
			XMVECTOR(LinePoint1),XMVECTOR(LinePoint2));
	}
/**************************************************************************
 void IntersectPlane(
	Vector3& RetLinePoint1,	//�Ԃ���钸�_1
	Vector3& RetLinePoint2,	//�Ԃ���钸�_2
	const Plane& other		//this�Ɣ�ׂ��������̖�
 )const;
 �p�r:�@this�Ƃ�����̖ʂ���������2�̒��_���v�Z����
 �߂�l: �Ȃ�
 ��this�͕ύX���Ȃ�
 ��RetLinePoint1��RetLinePoint2�Ɍ������钸�_��Ԃ�
 ��2�̕��ʂ����s�̏ꍇ�́A�߂�e�l��QNaN�i�񐔁j �ɂȂ�̂ŁA
 �@Vector3::IsNaN()�ȂǂŃ`�F�b�N����
***************************************************************************/
	void IntersectPlane(Vector3& RetLinePoint1,Vector3& RetLinePoint2,
		const Plane& other)const{
		XMVECTOR Temp1,Temp2;
		XMPlaneIntersectPlane(&Temp1,&Temp2,XMVECTOR(*this),XMVECTOR(other));
		RetLinePoint1 = (Vector3)Temp1;
		RetLinePoint2 = (Vector3)Temp2;
	}
/**************************************************************************
 bool IsInfinite()const;
 �p�r:�@this�̗v�f�̂����ꂩ�����܂��͕��̖����傩�ǂ����𒲂ׂ�
 �߂�l: ���܂��͕��̖�����Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsInfinite()const{
		return XMPlaneIsInfinite(XMVECTOR(*this));
	}
/**************************************************************************
 bool IsNaN()const;
 �p�r:�@this�̗v�f�̂����ꂩ�� NaN���ǂ����𒲂ׂ�
 �߂�l: NaN�Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsNaN()const{
		return XMPlaneIsNaN(XMVECTOR(*this));
	}
/**************************************************************************
 bool NearEqual(
	const Plane& other		//this�Ɣ�ׂ��������̖�
	const Vector4& Epsilon	//�e�v�f�̋��e�͈�
 )const;
 �p�r:�@this�Ƃ�����̕��ʂ��قړ��������ǂ������ׂ�
 �߂�l: �قړ��������true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool NearEqual(const Plane& other,const Vector4& Epsilon)const{
		return XMPlaneNearEqual(XMVECTOR(*this),XMVECTOR(other),Epsilon);
	}
/**************************************************************************
 void Normalize();
 �p�r:this�𐳋K������
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Normalize(){
		*this = (Plane)XMPlaneNormalize(XMVECTOR(*this));
	}
/**************************************************************************
 void NormalizeEst();
 �p�r:this��x,y,z�̌W�����P�ʂ̖@���x�N�g�����`������悤�ɁA���ʂ̌W����\��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void NormalizeEst(){
		*this = (Plane)XMPlaneNormalizeEst(XMVECTOR(*this));
	}
/**************************************************************************
 bool NotEqual(
	const Plane& other	//�����Plane
 )const;
 �p�r:�@this�Ƒ���̕��ʂ��������Ȃ����ǂ����𒲂ׂ�
 �߂�l: �������Ȃ��Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool NotEqual(const Plane& other)const{
		return XMPlaneNotEqual(XMVECTOR(*this),XMVECTOR(other));
	}
/**************************************************************************
 void Transform(
	const XMMATRIX& m	//�g�����X�t�H�[������s��
 );
 �p�r:this��m�Ńg�����X�t�H�[������
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Transform(const XMMATRIX& m){
		*this = (Plane)XMPlaneTransform(XMVECTOR(*this),m);
	}

//�@XMPlaneTransformStream�֐��́A���b�s���O�֐��͗p�ӂ��Ȃ�
//�@���R�F�p�����[�^�ɔz���n���K�v������A���b�s���O�ɂ���āA�œK�������Ȃ���
};


/**************************************************************************
 struct Color4 : public XMFLOAT4;
 �p�r: r�Ag�Ab�Aa�̐F���`����\����
****************************************************************************/
struct Color4: public XMFLOAT4{
/**************************************************************************
 Color4();
 �p�r: �R���X�g���N�^
 �߂�l: �Ȃ�
 �������̏��������@�̒�`
***************************************************************************/
	Color4():XMFLOAT4(){
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Color4(const XMFLOAT4& v4):XMFLOAT4(v4){
	}
	Color4(const XMVECTOR& other):XMFLOAT4(){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
	}
	Color4(float x,float y,float z,float w):
		XMFLOAT4(x,y,z,w){
	}
	Color4(const Color4& other):XMFLOAT4(){
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}
	operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	void Set(float xx,float yy,float zz,float ww){
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
	Color4& operator=(const Color4& other){
		//���ȑ���̗}��
		if(this != &other){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}
	Color4& operator=(const XMVECTOR& other){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
		return *this;
	}
	//�ǉ����Z
	Color4& operator+=(const Color4& other){
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	Color4& operator-=(const Color4& other){
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	Color4& operator*=(float val){
		x *= val;
		y *= val;
		z *= val;
		w *= val;
		return *this;
	}
	Color4& operator/=(float val){
		if(val == 0.0f){
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
	Color4 operator+ () const{
		return *this;
	}
	Color4 operator- () const{
		Vector4 ret = *this;
		ret *= -1.0f;
		return ret;
	}
	//���Z
	Color4 operator+(const Color4& other)const{
		Color4 ret = *this;
		ret += other;
		return ret;
	}
	Color4 operator-(const Color4& other)const{
		Color4 ret = *this;
		ret -= other;
		return ret;
	}
	Color4 operator*(float val)const{
		Color4 ret = *this;
		ret *= val;
		return ret;
	}
	Color4 operator/(float val)const{
		if(val == 0.0f){
			//0���Z
			throw BaseException(
				L"0���Z�ł�",
				L"if(val == 0.0f)",
				L"Color4::operator/()"
				);
		}
		Color4 ret = *this;
		ret /= val;
		return ret;
	}

	//�ʃA�N�Z�X
	float GetR() const{
		return x;
	}
	void SetR(float r){
		x = r;
	}
	float GetG() const{
		return y;
	}
	void SetG(float g){
		y = g;
	}
	float GetB() const{
		return z;
	}
	void SetB(float b){
		z = b;
	}
	float GetA() const{
		return w;
	}
	void SetA(float a){
		w = a;
	}
	void SetRGBA(float r,float g,float b,float a){
		x = r;
		y = g;
		z = b;
		w = a;
	}
/**************************************************************************
 bool Equal(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[�Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	bool Equal(const Color4& other)const{
		return XMColorEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator==(const Color4& other)const{
		return Equal(other);
	}
/**************************************************************************
 bool NotEqual(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[�Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	bool NotEqual(const Color4& other)const{
		return XMColorNotEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator!=(const Color4& other)const{
		return NotEqual(other);
	}

/**************************************************************************
 bool Greater(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[���傫�����ǂ��������؂���
 �߂�l: �傫�����true
***************************************************************************/
	bool Greater(const Color4& other)const{
		return XMColorGreater(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator>(const Color4& other)const{
		return Greater(other);
	}
/**************************************************************************
 bool GreaterOrEqual(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[�ȏォ�ǂ��������؂���
 �߂�l: �ȏ�ł����true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool GreaterOrEqual(const Color4& other)const{
		return XMColorGreaterOrEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator>=(const Color4& other)const{
		return GreaterOrEqual(other);
	}
/**************************************************************************
 bool LessOrEqual(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[�ȉ����ǂ��������؂���
 �߂�l: �ȉ��ł����true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool LessOrEqual(const Color4& other)const{
		return XMColorLessOrEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator<=(const Color4& other)const{
		return LessOrEqual(other);
	}
/**************************************************************************
 bool Less(
	const Color4& other	//��r����x�N�g��
 ) const;
 �p�r: this��������̃J���[��菬�������ǂ��������؂���
 �߂�l: ��菬�������true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool Less(const Color4& other)const{
		return XMColorLess(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator<(const Color4& other)const{
		return Less(other);
	}
/**************************************************************************
 bool IsInfinite()const;
 �p�r: this�̃J���[�̂����ꂩ�̐��������܂��͕��̖����傩�ǂ������e�X�g����
 �߂�l: this�̃J���[�̂����ꂩ�̐��������܂��͕��̖�����Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsInfinite()const{
		return XMColorIsInfinite(XMVECTOR(*this));
	}
/**************************************************************************
 bool IsNaN();
 �p�r: this�̃J���[�̂����ꂩ�̐�����NaN���ǂ������e�X�g����
 �߂�l: this�̃J���[�̂����ꂩ�̐�����NaN�Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsNaN()const{
		return XMColorIsNaN(XMVECTOR(*this));
	}
	//����
/**************************************************************************
 void AdjustRange();
 �p�r: this�̃J���[��0.0����1.0�͈̔͂ɒ�����this�ɐݒ肷��
 �߂�l: �Ȃ�
 ���͈͊O�̗v�f��0.0��1.0�ɐݒ肳���
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AdjustRange(){
		if(this->x <= 0){
			this->x = 0;
		}
		if(this->y <= 0){
			this->y = 0;
		}
		if(this->z <= 0){
			this->z = 0;
		}
		if(this->w <= 0){
			this->w = 0;
		}
		if(this->x >= 1.0f){
			this->x = 1.0f;
		}
		if(this->y >= 1.0f){
			this->y = 1.0f;
		}
		if(this->z >= 1.0f){
			this->z = 1.0f;
		}
		if(this->w >= 1.0f){
			this->w = 1.0f;
		}
	}
/**************************************************************************
 void AdjustContrast(
		float Contrast		//�R���g���X�g�͈�
 );
 �p�r: this�̃J���[�̃R���g���X�g�𒲐߂���
 �߂�l: �Ȃ�
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AdjustContrast(float Contrast){
		AdjustRange();
		*this = (Color4)XMColorAdjustContrast(XMVECTOR(*this),Contrast);
	}
/**************************************************************************
 void AdjustSaturation(
		const Color4& Col,	//�J���[
		float Saturation		//�ʓx�l
 );
 �p�r: �J���[�̍ʓx�𒲐߂�this�ɐݒ肷��
 �߂�l: �Ȃ�
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
 ���ʓx�l��0�̏ꍇ�̓O���[�X�P�[���A1.0�̏ꍇ�͂��Ƃ̐F�ɂȂ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AdjustSaturation(const Color4& Col,float Saturation){
		AdjustRange();
		*this = (Color4)XMColorAdjustSaturation(XMVECTOR(*this),Saturation);
	}
/**************************************************************************
 void Modulate(
		const Color4& other	//�J���[1
 );
 �p�r: this�ɂ�����̃J���[�̑Ή����鐬������Z���ău�����h�������̂�this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Modulate(const Color4& other){
		*this = (Color4)XMColorModulate(XMVECTOR(*this),XMVECTOR(other));
	}
/**************************************************************************
 void Negative();
 �p�r: this�̃J���[�̕��� RGB �J���[�l������this�ɐݒ肷��
 �߂�l: �Ȃ�
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Negative(const Color4& Col){
		AdjustRange();
		*this = (Color4)XMColorNegative(XMVECTOR(*this));
	}
/**************************************************************************
 void GrayColor();
 �p�r: this�̃J���[���O���[�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	 void GrayColor(){
		*this = Color4(0.5f,0.5f,0.5f,1.0f);
	}
/**************************************************************************
 void WhiteColor();
 �p�r: this�̃J���[�𔒂ɐݒ肷��
 �߂�l: �Ȃ�
 ���J���[�͔��̃J���[�l�i1.0,1.0,1.0,1.0�j�ɂȂ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	 void WhiteColor(){
		*this = Color4(1.0f,1.0f,1.0f,1.0f);
	}


};

/**************************************************************************
 struct Quaternion : public XMFLOAT4;
 �p�r: �N�I�[�^�j�I���\����
****************************************************************************/
struct Quaternion : public XMFLOAT4{
/**************************************************************************
 Quaternion();
 �p�r: �R���X�g���N�^
 �߂�l: �Ȃ�
 �������̏��������@�̒�`
***************************************************************************/
	Quaternion():XMFLOAT4(){
		Identity();
	}
	Quaternion(const XMFLOAT4& v4):XMFLOAT4(v4){
	}
	Quaternion(const XMVECTOR& other):XMFLOAT4(){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
	}
	Quaternion(float x,float y,float z,float w):
		XMFLOAT4(x,y,z,w){
	}
	Quaternion(const Quaternion& other):XMFLOAT4(){
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}
	//��]�X�J������쐬
	Quaternion(float Pitch,float Yaw,float Roll):XMFLOAT4(){
		RotationRollPitchYaw(Pitch,Yaw,Roll);
	}
	//��]���Ɗp�x����쐬
	Quaternion(const Vector3& Vec,float Angle):XMFLOAT4(){
		RotationAxis(Vec,Angle);
	}
	//��]�s�񂩂�쐬
	Quaternion(const XMMATRIX& m):XMFLOAT4(){
		RotationMatrix(m);
	}
	operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	void Set(float xx,float yy,float zz,float ww){
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}
/**************************************************************************
	���d��`
***************************************************************************/
	//���
	Quaternion& operator=(const Quaternion& other){
		//���ȑ���̗}��
		if(this != &other){
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}
	Quaternion& operator=(const XMVECTOR& other){
		XMVECTOR temp = other;
		XMStoreFloat4(this,temp);
		return *this;
	}
	//�ǉ����Z
	Quaternion& operator*=(const Quaternion& other){
		*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this),XMVECTOR(other));
		return *this;
	}
	//���Z
	Quaternion operator*(const Quaternion& other)const{
		Quaternion temp(*this);
		temp *= other;
		return temp;
	}
	//��r
/**************************************************************************
 bool Equal(
	const Quaternion& other	//��r����N�I�[�^�j�I��
 ) const;
 �p�r: this��������̃N�I�[�^�j�I���Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	bool Equal(const Quaternion& other)const{
		return XMQuaternionEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator==(const Quaternion& other)const{
		return Equal(other);
	}
/**************************************************************************
 bool NotEqual(
	const Quaternion& other	//��r����N�I�[�^�j�I��
 ) const;
 �p�r: this��������̃N�I�[�^�j�I���Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	bool NotEqual(const Quaternion& other)const{
		return XMQuaternionNotEqual(XMVECTOR(*this),XMVECTOR(other));
	}
	bool operator!=(const Quaternion& other)const{
		return NotEqual(other);
	}
	//����
/**************************************************************************
 bool IsIdentity() const;
 �p�r: this���P�ʃN�I�[�^�j�I�� (0, 0, 0, 1)���ǂ��������؂���B
 �߂�l: �N�I�[�^�j�I�� (0, 0, 0, 1)�Ȃ��true
***************************************************************************/
	bool IsIdentity() const{
		return XMQuaternionIsIdentity(XMVECTOR(*this));
	}
/**************************************************************************
 bool IsInfinite() const;
 �p�r: this�̒l�̂����ꂩ���������̖����傩�ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
***************************************************************************/
	bool IsInfinite() const{
		return XMQuaternionIsInfinite(XMVECTOR(*this));
	}
/**************************************************************************
 bool IsNaN() const;
 �p�r: �l�̂����ꂩ��NaN���ǂ��������؂���B
 �߂�l: �l�̂����ꂩ��NaN�Ȃ��true
***************************************************************************/
	bool IsNaN() const{
		return XMQuaternionIsNaN(XMVECTOR(*this));
	}
	//����
/**************************************************************************
 void AddRotation(
	const Vector3& other	//�ǉ���]�����]�s��
 );
 �p�r: ��]�s���ǉ���]����
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AddRotation(const Vector3& other){
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(other);
		*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this),XMVECTOR(Span));
	}
/**************************************************************************
 Vector3 GetRotation();
 �p�r: �N�I�[�^�j�I�������]�s��ɕϊ����ĕԂ�
 �߂�l: ��]�s��
***************************************************************************/
	Vector3 GetRotation()const;
/**************************************************************************
 void BaryCentric(
	const Quaternion& Q1,	//�O�p�`�� 2 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q2,	//�O�p�`�� 3 �ڂ̃N�I�[�^�j�I��
	float f,	//���d�W��
	float g		//���d�W��
 );
 �p�r: �O�p�`��1�ڂ̃N�I�[�^�j�I��this�ɁA
�@�@�@�w�肳�ꂽ�N�I�[�^�j�I�����g�p���āA�d�S���W�̃|�C���g��ݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void BaryCentric(const Quaternion& Q1,const Quaternion& Q2,float f,float g){
		*this = (Quaternion)XMQuaternionBaryCentric(XMVECTOR(*this),XMVECTOR(Q1),XMVECTOR(Q2),f,g);
	}
/**************************************************************************
 void BaryCentricV(
	const Quaternion& Q1,	//�O�p�`�� 2 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q2,	//�O�p�`�� 3 �ڂ̃N�I�[�^�j�I��
	const Vector4& f,	//���d�W��
	const Vector4& g		//���d�W��
 );
 �p�r: �O�p�`��1�ڂ̃N�I�[�^�j�I��this�ɁA
�@�@�@�w�肳�ꂽ�N�I�[�^�j�I���ƃx�N�g�����g�p���āA�d�S���W�̃|�C���g��ݒ肷��
 �߂�l: �Ȃ�
 ���x�N�g��f��g�͂��ꂼ��v�f�������l�ɂ���
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void BaryCentricV(const Quaternion& Q1,const Quaternion& Q2,
		const Vector4& f,const Vector4& g){
		*this = (Quaternion)XMQuaternionBaryCentricV(XMVECTOR(*this),XMVECTOR(Q1),XMVECTOR(Q2),XMVECTOR(f),XMVECTOR(g));
	}
/**************************************************************************
 void Quaternion Conjugate();
 �p�r: this�̋�����ݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Conjugate(){
		*this = (Quaternion)XMQuaternionConjugate(XMVECTOR(*this));
	}
/**************************************************************************
 float Quaternion Dot(
	const Quaternion& other	//������̃N�I�[�^�j�I��
 ) const;
 �p�r: this�Ƃ�����̃N�I�[�^�j�I���Ƃ̓��ς��v�Z���ĕԂ�
 �߂�l: ����
 ��this�͕ύX���Ȃ�
***************************************************************************/
	float Dot(const Quaternion& other)const{
		return ((Vector4)XMQuaternionDot(XMVECTOR(*this),XMVECTOR(other))).x;
	}
/**************************************************************************
 void Exp();
 �p�r: this�̎w����this�ɐݒ肷��
 �߂�l: �Ȃ�
 �����͂�w�v�f�͖��������
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Exp(){
		*this = (Quaternion)XMQuaternionExp(XMVECTOR(*this));
	}
/**************************************************************************
 void Identity();
 �p�r: this�ɒP�ʃN�I�[�^�j�I����ݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Identity(){
		*this = (Quaternion)XMQuaternionIdentity();
	}
/**************************************************************************
 void Inverse();
 �p�r: this�𔽓]����
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Inverse(){
		*this = (Quaternion)XMQuaternionInverse(XMVECTOR(*this));
	}
/**************************************************************************
 float Length() const;
 �p�r: this�̒�����Ԃ�
 �߂�l: this�̒���
 ��this�͕ύX���Ȃ�
***************************************************************************/
	float Length() const{
		return ((Vector4)XMQuaternionLength(XMVECTOR(*this))).x;
	}
/**************************************************************************
 float LengthSq() const;
 �p�r: this�̒�����2���Ԃ�
 �߂�l: this�̒�����2��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	float LengthSq() const{
		return  ((Vector4)XMQuaternionLengthSq(XMVECTOR(*this))).x;
	}
/**************************************************************************
 void Ln();
 �p�r: this�̎��R�ΐ���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�͐��K�����Ă���
***************************************************************************/
	void Ln(){
		*this = (Quaternion)XMQuaternionLn(XMVECTOR(*this));
	}
/**************************************************************************
 void Multiply(
	const Quaternion& other	//����̃N�I�[�^�j�I��
 );
 �p�r: this�𑊎�̃N�I�[�^�j�I���Ə�Z����
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Multiply(const Quaternion& other){
		*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this),XMVECTOR(other));
	}
/**************************************************************************
 void Normalize();
 �p�r: this�𐳋K������
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Normalize(){
		*this = (Quaternion)XMQuaternionNormalize(XMVECTOR(*this));
	}

/**************************************************************************
 void NormalizeEst();
 �p�r: this�ɐ��K���̗\���l��ݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void NormalizeEst(){
		*this = (Quaternion)XMQuaternionNormalizeEst(XMVECTOR(*this));
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
 void RotationAxis(
	const Vector3& Vec,		//��]����\���R�c�x�N�g��
	float Angle				//��]�p�x�i���v��胉�W�A���j
 );
 �p�r: ��]���Ɗp�x����N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationAxis(const Vector3& Vec,float Angle){
		*this = XMQuaternionRotationAxis(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 void RotationMatrix(
	const XMMATRIX& m				//��]�s��
 );
 �p�r: ��]�s�񂩂�N�I�[�^�j�I����this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationMatrix(const XMMATRIX& m){
		*this = (Quaternion)XMQuaternionRotationMatrix(m);
	}
/**************************************************************************
 void RotationNormal(
	const Vector3& Vec,		//��]����\�����K�����ꂽ�R�c�x�N�g��
	float Angle				//��]�p�x�i���v��胉�W�A���j
 );
 �p�r: ���K�����ꂽ��]���Ɗp�x����N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationNormal(const Vector3& Vec,float Angle){
		*this = (Quaternion)XMQuaternionRotationNormal(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 void RotationRollPitchYaw(
	float Pitch,		//X����]�i���W�A���j
	float Yaw,			//Y����]�i���W�A���j
	float Roll			//Z����]�i���W�A���j
 );
 �p�r: �e���̉�]����N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ����]��RollPitchYaw�iZ�AY�AX�j�̏��ōs����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		*this = (Quaternion)XMQuaternionRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 void RotationRollPitchYawFromVector(
	const Vector3& Vec		//��]����\���R�c�x�N�g��
 );
 �p�r: 3D��]�x�N�g������N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ����]��RollPitchYaw�iZ�AY�AX�j�̏��ōs����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationRollPitchYawFromVector(const Vector3& Vec){
		*this = (Quaternion)XMQuaternionRotationRollPitchYawFromVector(XMVECTOR(Vec));
	}
/**************************************************************************
 void Slerp(
	const Quaternion& Qt0,		//��Ԍ��i���K�����ꂽ�N�I�[�^�j�I���j
	const Quaternion& Qt1,		//��Ԑ�i���K�����ꂽ�N�I�[�^�j�I���j
	float t						//��ԌW���i0.0f����1.0f�̊Ԃ��w��j
 );
 �p�r: 2�̃N�I�[�^�j�I������W���ŕ�Ԃ��ꂽ�N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ���W����0�̏ꍇ��Q0�A1.0�Ȃ�Q1�Ɠ������̂�Ԃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Slerp(const Quaternion& Qt0,const Quaternion& Qt1,float t){
		*this = XMQuaternionSlerp(XMVECTOR(Qt0),XMVECTOR(Qt1),t);
	}
/**************************************************************************
 void SlerpV(
	const Quaternion& Qt0,		//��Ԍ��i���K�����ꂽ�N�I�[�^�j�I���j
	const Quaternion& Qt1,		//��Ԑ�i���K�����ꂽ�N�I�[�^�j�I���j
	const Vector4& Vec			//��ԌW���i���ׂĂ̗v�f��0.0f����1.0f�̊Ԃ̓����l���w��j
 );
 �p�r: 2�̃N�I�[�^�j�I������W���ŕ�Ԃ��ꂽ�N�I�[�^�j�I�����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ���W����0�̏ꍇ��Q0�A1.0�Ȃ�Q1�Ɠ������̂�Ԃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void SlerpV(const Quaternion& Qt0,const Quaternion& Qt1,const Vector4& Vec){
		*this = XMQuaternionSlerpV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Vec));
	}
/**************************************************************************
 void Squad(
	const Quaternion& Qt0,		//���K�����ꂽ�N�I�[�^�j�I���P
	const Quaternion& Qt1,		//���K�����ꂽ�N�I�[�^�j�I���Q
	const Quaternion& Qt2,		//���K�����ꂽ�N�I�[�^�j�I���R
	const Quaternion& Qt3,		//���K�����ꂽ�N�I�[�^�j�I���S
	float t						//��Ԑ���W���i0.0f����1.0f�̊Ԃ��w��j
 );
 �p�r: ���󕽕���Ԃ��g�p���āA4 �̒P�ʃN�I�[�^�j�I���Ԃ��Ԃ�this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��Q1�AQ2�AQ3��Quaternion::GetSquadSetup�֐��ō쐬����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Squad(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,float t){
		*this = XMQuaternionSquad(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),t);
	}
/**************************************************************************
 void SquadV(
	const Quaternion& Qt0,		//���K�����ꂽ�N�I�[�^�j�I���P
	const Quaternion& Qt1,		//���K�����ꂽ�N�I�[�^�j�I���Q
	const Quaternion& Qt2,		//���K�����ꂽ�N�I�[�^�j�I���R
	const Quaternion& Qt3,		//���K�����ꂽ�N�I�[�^�j�I���S
	const Vector4& Vec			//��Ԑ���W���i���ׂĂ̗v�f��0.0f����1.0f�̊Ԃ̓����l���w��j
 );
 �p�r: ���󕽕���Ԃ��g�p���āA4 �̒P�ʃN�I�[�^�j�I���Ԃ��Ԃ�this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��Q1�AQ2�AQ3��Quaternion::GetSquadSetup�֐��ō쐬����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void SquadV(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,const Vector4& Vec){
		*this = XMQuaternionSquadV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),XMVECTOR(Vec));
	}
/**************************************************************************
 void ToAxisAngle(
	Vector3* pVec,			//��]���𓾂�R�c�x�N�g��
	float* pAngle			//��]�p�x�𓾂�ϐ�
 )const;
 �p�r: this�N�I�[�^�j�I������A��]���Ɖ�]�p�x�𓾂�
 �߂�l: �Ȃ�
 ��pVec�ɂ͉�]���x�N�g���ApAngle�ɂ͊p�x���ݒ肳���
***************************************************************************/
	void ToAxisAngle(Vector3* pVec,float* pAngle)const{
		XMVECTOR Vec(*pVec);
		XMQuaternionToAxisAngle(&Vec,pAngle,XMVECTOR(*this));
		*pVec = Vec;
	}

/**************************************************************************
	void Billboard(
	const Vector3& Line			//�v�Z���ƂȂ��
	)t;
	�p�r: Line�ɑ΂��ăr���{�[�h�p�x��this�ɐݒ肷��
	�߂�l: �Ȃ�
	��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Billboard(const Vector3& Line);

/**************************************************************************
	void Facing(
	const Vector3& Line			//�v�Z���ƂȂ��(�I�u�W�F�N�g����J�����ʒu������������)
	)t;
	�p�r: Line�ɑ΂��ăt�F�C�V���O�p�x��this�ɐݒ肷��
	�߂�l: �Ȃ�
	��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Facing(const Vector3& Line);

/**************************************************************************
	void FacingY(
	const Vector3& Line			//�v�Z���ƂȂ��(�I�u�W�F�N�g����J�����ʒu������������)
	)t;
	�p�r: Line�ɑ΂���Y���t�F�C�V���O�p�x��this�ɐݒ肷��
	�߂�l: �Ȃ�
	��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void FacingY(const Vector3& Line);


};

/**************************************************************************
 struct Matrix4X4 : public XMFLOAT4X4;
 �p�r:�@�s���\���\����
****************************************************************************/
struct Matrix4X4 : public XMFLOAT4X4{
/**************************************************************************
 Matrix4X4();
 �p�r: �R���X�g���N�^
 �߂�l: �Ȃ�
 �������̏��������@�̒�`
***************************************************************************/
	Matrix4X4():XMFLOAT4X4(){
		Identity();
	}
	Matrix4X4(const XMMATRIX& other):
		XMFLOAT4X4(){
		XMMATRIX temp = other;
		XMStoreFloat4x4(this,temp);
	}
	Matrix4X4(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33):
		XMFLOAT4X4(m00,m01,m02,m03,
				 m10,m11,m12,m13,
				 m20,m21,m22,m23,
				 m30,m31,m32,m33){}
	Matrix4X4(const XMFLOAT4X4& m):
		XMFLOAT4X4(m._11,m._12,m._13,m._14,
				 m._21,m._22,m._23,m._24,
				 m._31,m._32,m._33,m._34,
				 m._41,m._42,m._43,m._44){}
	Matrix4X4(const Matrix4X4& other):
		XMFLOAT4X4(){
		for(UINT i = 0;i <4;i++){
			m[i][0] = other.m[i][0];
			m[i][1] = other.m[i][1];
			m[i][2] = other.m[i][2];
			m[i][3] = other.m[i][3];
		}
	}
	Matrix4X4(const Vector3& Scaling,
		const Quaternion& RotationQuaternion,
		const Vector3& Translation):
		XMFLOAT4X4(){
		DefTransformation(Scaling,RotationQuaternion,Translation);
	}
	Matrix4X4(const Vector3& Scaling,
		const Vector3& Rotation,
		const Vector3& Translation):
		XMFLOAT4X4(){
		DefTransformation(Scaling,Rotation,Translation);
	}
	operator XMMATRIX() const {
		Matrix4X4 temp = *this;
		XMMATRIX m = XMLoadFloat4x4(&temp);
		return m;
	}
/**************************************************************************
	���d��`
***************************************************************************/
	//���
	Matrix4X4& operator=(const Matrix4X4& other){
		//���ȑ���̗}��
		if(this != &other){
			for(UINT i = 0;i <4;i++){
				m[i][0] = other.m[i][0];
				m[i][1] = other.m[i][1];
				m[i][2] = other.m[i][2];
				m[i][3] = other.m[i][3];
			}
		}
		return *this;
	}
	Matrix4X4& operator=(const XMMATRIX& other){
		XMMATRIX temp = other;
		XMStoreFloat4x4(this,temp);
		return *this;
	}
	//�ǉ����Z
	Matrix4X4& operator+=(const Matrix4X4& other){
		XMMATRIX temp = XMMATRIX(*this);
		temp += XMMATRIX(other);
		*this = Matrix4X4(temp);
		return *this;
	}
	Matrix4X4& operator-=(const Matrix4X4& other){
		XMMATRIX temp = XMMATRIX(*this);
		temp -= XMMATRIX(other);
		*this = Matrix4X4(temp);
		return *this;
	}
	Matrix4X4& operator*=(const Matrix4X4& other){
		XMMATRIX temp = XMMATRIX(*this);
		temp *= XMMATRIX(other);
		*this = Matrix4X4(temp);
		return *this;
	}
	Matrix4X4& operator*=(float f){
		XMMATRIX temp = XMMATRIX(*this);
		temp *= f;
		*this = Matrix4X4(temp);
		return *this;
	}
	Matrix4X4& operator/=(float f){
		if(f == 0.0f){
			//0���Z
			throw BaseException(
				L"0���Z�ł�",
				L"if(f == 0.0f)",
				L"Matrix4X4::operator/=()"
				);
		}
		XMMATRIX temp = XMMATRIX(*this);
		temp /= f;
		*this = Matrix4X4(temp);
		return *this;
	}
    Matrix4X4 operator+ (const Matrix4X4& other) const{
		Matrix4X4 temp(*this);
		temp += other;
		return temp;
	}
    Matrix4X4 operator- (const Matrix4X4& other) const{
		Matrix4X4 temp(*this);
		temp -= other;
		return temp;
	}
    Matrix4X4 operator* (const Matrix4X4& other) const{
		Matrix4X4 temp(*this);
		temp *= other;
		return temp;
	}
    Matrix4X4 operator* (float f) const{
		Matrix4X4 temp(*this);
		temp *= f;
		return temp;
	}
    Matrix4X4 operator/ (float f) const{
		Matrix4X4 temp(*this);
		temp /= f;
		return temp;
	}
	//�ʃA�N�Z�X
	float Get(UINT Row,UINT Col){
		if(Row > 3 || Col > 3){
			throw BaseException(
				L"�s��͈̔͊O�ł�",
				L"if(Row > 3 || Col > 3)",
				L"Matrix4X4::Get()"
				);
		}
		return m[Row][Col];
	}
	void Set(UINT Row,UINT Col,float f){
		if(Row > 3 || Col > 3){
			throw BaseException(
				L"�s��͈̔͊O�ł�",
				L"if(Row > 3 || Col > 3)",
				L"Matrix4X4::Set()"
				);
		}
		m[Row][Col] = f;
	}
/**************************************************************************
 Vector3 PosInMatrixSt()const;
 �p�r: this�s����̈ʒu�����擾����(���ڍs�񂩂���o��
 �߂�l: �ʒu�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 PosInMatrixSt()const{
		Vector3 Vec;
		Vec.x = _41;
		Vec.y = _42;
		Vec.z = _43;
		return Vec;
	}
/**************************************************************************
 Vector3 PosInMatrix()const;
 �p�r: this�s����̈ʒu�����擾����
 �߂�l: �ʒu�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 PosInMatrix()const{
		Vector3 Vec(0,0,0);
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(*this))){
			Vec = Translation;
		}
		return Vec;
	}
/**************************************************************************
 Vector3 ScaleInMatrix()const;
 �p�r: this�s����̃X�P�[�����O���擾����
 �߂�l: �X�P�[�����O�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 ScaleInMatrix()const{
		Vector3 Vec(1.0f,1.0f,1.0f);
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(*this))){
			Vec = Scale;
		}
		return Vec;
	}
/**************************************************************************
 Quaternion QtInMatrix()const;
 �p�r: this�s����̉�]���擾����
 �߂�l: ��]�N�I�[�^�j�I��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Quaternion QtInMatrix() const{
		Quaternion RetQt;
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(*this))){
			RetQt = Qt;
		}
		return RetQt;
	}

/**************************************************************************
 static void GetSafeScale(
	Vector3&	Scale	//�ω�������X�P�[��
 );
 �p�r: 0�X�P�[�����Ȃ���
 �߂�l: �Ȃ��A0�X�P�[�����O��0.001�ɕύX����
***************************************************************************/
	static void GetSafeScale(Vector3& Scale){
		if(Scale.x <= 0){
			Scale.x = 0.001f;
		}
		if(Scale.y <= 0){
			Scale.y = 0.001f;
		}
		if(Scale.z <= 0){
			Scale.z = 0.001f;
		}
	}
/**************************************************************************
 Vector3 RotXInMatrix()const;
 �p�r: this�s�����X����]���擾����
 �߂�l: X���̉�]�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 RotXInMatrix()const{
		Vector3 ret(_11,_12,_13);
		Vector3 Scale = ScaleInMatrix();
		GetSafeScale(Scale);
		ret.x /= Scale.x;
		ret.y /= Scale.x;
		ret.z /= Scale.x;
		return ret;
	}
/**************************************************************************
 Vector3 RotYInMatrix()const;
 �p�r: this�s�����Y����]���擾����
 �߂�l: Y���̉�]�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 RotYInMatrix()const{
		Vector3 ret(_21,_22,_23);
		Vector3 Scale = ScaleInMatrix();
		GetSafeScale(Scale);
		ret.x /= Scale.y;
		ret.y /= Scale.y;
		ret.z /= Scale.y;
		return ret;
	}
/**************************************************************************
 Vector3 RotZInMatrix()const;
 �p�r: this�s�����Z����]���擾����
 �߂�l: Z���̉�]�x�N�g��
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector3 RotZInMatrix()const{
		Vector3 ret(_31,_32,_33);
		Vector3 Scale = ScaleInMatrix();
		GetSafeScale(Scale);
		ret.x /= Scale.z;
		ret.y /= Scale.z;
		ret.z /= Scale.z;
		return ret;
	}

/**************************************************************************
 void AffineTransformation(
		const Vector3& Scaling,					//�X�P�[�����O
		const Vector3& RotOrigin,				//��]�̒��S
		const Quaternion& RotationQuaternion,	//�N�I�[�^�j�I��
		const Vector3& Translation				//���s�ړ�
 );
 �p�r: �X�P�[�����O�A��]�̒��S���W�A�N�I�[�^�j�I���A���s�ړ�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AffineTransformation(const Vector3& Scaling,const Vector3& RotOrigin,
			const Quaternion& RotationQuaternion,const Vector3& Translation){
		*this = (Matrix4X4)XMMatrixAffineTransformation(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			XMVECTOR(RotationQuaternion),XMVECTOR(Translation));
	}
/**************************************************************************
 void AffineTransformation2D(
		const Vector2& Scaling,					//�X�P�[�����O
		const Vector2& RotOrigin,				//��]�̒��S
		float Rotation,							//��]
		const Vector2& Translation				//���s�ړ�
 );
 �p�r: �X�P�[�����O�A��]�̒��S���W�A��]�l�i���W�A���j�A���s�ړ�����2D�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void AffineTransformation2D(const Vector2& Scaling,
		const Vector2& RotOrigin,float Rotation,const Vector2& Translation){
		*this = (Matrix4X4)XMMatrixAffineTransformation2D(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 bool Decompose(
		Vector3* pScaling,					//�X�P�[�����O
		Quaternion* pQt,					//�N�I�[�^�j�I��
		Vector3* pTranslation,				//���s�ړ�
 )const;
 �p�r: this����X�P�[�����O�A��]�A���s�ړ������o��
 �߂�l: ���������true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool Decompose(Vector3* pScaling,Quaternion* pQt,
		Vector3* pTranslation)const{
		XMVECTOR Scale(*pScaling);
		XMVECTOR Qt(*pQt);
		XMVECTOR Translation(*pTranslation);
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(*this))){
			*pScaling = Scale;
			*pQt = Qt;
			*pTranslation = Translation;
			return true;
		}
		else{
			return false;
		}
	}
/**************************************************************************
	void ScaleIdentity();
	�p�r: �X�P�[�����O��1.0�ɂ����s��ɕϊ�����
	�߂�l: �Ȃ�
	��this�ɐݒ肷��
***************************************************************************/
	void ScaleIdentity(){
		Vector3 Scale, Translation;
		Quaternion Qt;
		Decompose(&Scale, &Qt, &Translation);
		Scale = Vector3(1.0f, 1.0f, 1.0f);
		DefTransformation(Scale, Qt, Translation);
	}

/**************************************************************************
 Vector4 Determinant()const;
 �p�r: this�̍s�񎮂�Ԃ�
 �߂�l: �쐬�����s�񎮁iVector4�̊e�v�f�ɓ���j
 ��this�͕ύX���Ȃ�
***************************************************************************/
	Vector4 Determinant()const{
		return (Vector4)XMMatrixDeterminant(XMMATRIX(*this));
	}
/**************************************************************************
 void Identity();
 �p�r: �P�ʍs���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Identity(){
		*this = (Matrix4X4)XMMatrixIdentity();
	}
/**************************************************************************
 void Inverse(
	Vector4* pVec					//�s�񎮂��󂯎��x�N�g��
 );
 �p�r:�t�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��pVec�ɂ͍s�񎮂�����
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Inverse(Vector4* pVec){
		XMVECTOR Vec(*pVec);
		*this = (Matrix4X4)XMMatrixInverse(&Vec,XMMATRIX(*this));
		*pVec = Vec;
	}
/**************************************************************************
 bool IsIdentity()const;
 �p�r: this���P�ʍs�񂩂ǂ��������؂���
 �߂�l: �P�ʍs��Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsIdentity()const{
		return XMMatrixIsIdentity(XMMATRIX(*this));
	}
/**************************************************************************
 bool IsInfinite()const;
 �p�r: this�̂����ꂩ�̗v�f�����܂��͕��̖����傩�ǂ��������؂���
 �߂�l: ���܂��͕��̖�����Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsInfinite()const{
		return XMMatrixIsInfinite(XMMATRIX(*this));
	}
/**************************************************************************
 bool IsNaN()const;
 �p�r: this�̂����ꂩ�̗v�f��NaN���ǂ��������؂���
 �߂�l: NaN�Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	bool IsNaN()const{
		return XMMatrixIsNaN(XMMATRIX(*this));
	}
/**************************************************************************
 void LookAtLH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& At,					//�J�������_
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:������W�r���[�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void LookAtLH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		*this = (Matrix4X4)XMMatrixLookAtLH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 void LookAtRH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& At,					//�J�������_
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�E����W�r���[�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void LookAtRH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		*this = (Matrix4X4)XMMatrixLookAtRH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 void LookToLH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& EyeDirection,		//�J��������
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�J�����ʒu�A�����A�X�����獶����W�r���[�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void LookToLH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		*this = (Matrix4X4)XMMatrixLookToLH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 void LookToRH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& EyeDirection,		//�J��������
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�J�����ʒu�A�����A�X������E����W�r���[�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void LookToRH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		*this = (Matrix4X4)XMMatrixLookToRH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 void Multiply(
	const Matrix4X4& m					//���Z����s��
 );
 �p�r:this�ƍs��̐ς�this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Multiply(const Matrix4X4& m){
		*this = (Matrix4X4)XMMatrixMultiply(XMMATRIX(*this),XMMATRIX(m));
	}
/**************************************************************************
 void MultiplyTranspose(
	const Matrix4X4& m					//���Z����s��
 );
 �p�r:this�ƍs��̐ς̓]�u�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void MultiplyTranspose(const Matrix4X4& m){
		*this = (Matrix4X4)XMMatrixMultiplyTranspose(XMMATRIX(*this),XMMATRIX(m));
	}
/**************************************************************************
 void OrthographicLH(
	float ViewWidth,		//��
	float ViewHeight,		//����
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:������W�n�̐��ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void OrthographicLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixOrthographicLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 void OrthographicRH(
	float ViewWidth,		//��
	float ViewHeight,		//����
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:�E����W�n�̐��ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void OrthographicRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixOrthographicRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 void OrthographicOffCenterLH(
	float ViewLeft,			//��
	float ViewRight,		//�E
	float ViewBottom,		//��
	float ViewTop,			//��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:������W�n�̃J�X�^�����ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void OrthographicOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixOrthographicOffCenterLH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 void OrthographicOffCenterRH(
	float ViewLeft,			//��
	float ViewRight,		//�E
	float ViewBottom,		//��
	float ViewTop,			//��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:�E����W�n�̃J�X�^�����ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void OrthographicOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixOrthographicOffCenterRH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveFovLH(
	float FovAngleY,		//�g�b�v�_�E������p�x
	float AspectHByW,		//�r���[��Ԃ̍����ƕ��̃A�X�y�N�g��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ����Ɋ�Â��āA������W�n�̃p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveFovLH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveFovLH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveFovRH(
	float FovAngleY,		//�g�b�v�_�E������p�x
	float AspectHByW,		//�r���[��Ԃ̍����ƕ��̃A�X�y�N�g��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ����Ɋ�Â��āA�E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveFovRH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveFovRH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveLH(
	float ViewWidth,		//�߂��̃N���b�v�ʂ̎�����̕�
	float ViewHeight,		//�߂��̃N���b�v�ʂ̎�����̍���
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ������W�n�̃p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveRH(
	float ViewWidth,		//�߂��̃N���b�v�ʂ̎�����̕�
	float ViewHeight,		//�߂��̃N���b�v�ʂ̎�����̍���
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveOffCenterLH(
	float ViewLeft,			//�N���b�v������̍����� x ���W
	float ViewRight,		//�N���b�v������̉E���� x ���W
	float ViewBottom,		//�N���b�v������̉����� y ���W
	float ViewTop,			//�N���b�v������̏㑤�� y ���W
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �J�X�^�� �o�[�W�����̍�����W�n�p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveOffCenterLH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 void PerspectiveOffCenterRH(
	float ViewLeft,			//�N���b�v������̍����� x ���W
	float ViewRight,		//�N���b�v������̉E���� x ���W
	float ViewBottom,		//�N���b�v������̉����� y ���W
	float ViewTop,			//�N���b�v������̏㑤�� y ���W
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �J�X�^�� �o�[�W�����̉E����W�n�p�[�X�y�N�e�B�u�ˉe�s���this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void PerspectiveOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		*this = (Matrix4X4)XMMatrixPerspectiveOffCenterRH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 void Reflect(
	const Plane& Pln		//����
 );
 �p�r: �w�肳�ꂽ���ʂ���ăx�N�g���𔽉f����g�����X�t�H�[���s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Reflect(const Plane& Pln){
		*this = (Matrix4X4)XMMatrixReflect(XMVECTOR(Pln));
	}
/**************************************************************************
 void RotationAxis(
	const Vector3& Axis,		//��]����\���x�N�g��
	float Angle				//��]�p�x�i���W�A���j
 );
 �p�r: �C�ӂ̎�����]���ɂ��āA��]�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationAxis(const Vector3& Axis,float Angle){
		*this = (Matrix4X4)XMMatrixRotationAxis(XMVECTOR(Axis),Angle);
	}
/**************************************************************************
 void RotationNormal(
	const Vector3& NormalAxis,		//��]�����L�q����@���x�N�g��
	float Angle						//��]�p�x�i���W�A���j
 );
 �p�r: �C�ӂ̖@���x�N�g������]���ɂ��āA��]�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationNormal(const Vector3& NormalAxis,float Angle){
		*this = (Matrix4X4)XMMatrixRotationNormal(XMVECTOR(NormalAxis),Angle);
	}
/**************************************************************************
 void RotationQuaternion(
	const Quaternion& Qt		//��]���L�q����N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I�������]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationQuaternion(const Quaternion& Qt){
		*this = (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(Qt));
	}
/**************************************************************************
 void RotationRollPitchYaw(
	float Pitch,		//X����]
	float Yaw,			//Y����]
	float Roll			//Z����]
 );
 �p�r: �I�C���[�p�Ɋ�Â��ĉ�]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		*this = (Matrix4X4)XMMatrixRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 void RotationRollPitchYawFromVector(
	const Vector3& Angles		//��]���L�q����x�N�g��
 );
 �p�r: �I�C���[�p(�x�N�g��)�Ɋ�Â��ĉ�]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationRollPitchYawFromVector(const Vector3& Angles){
		*this = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Angles));
	}
/**************************************************************************
 void RotationX(
	float Angle		//X����]
 );
 �p�r: X����]�Ɋ�Â��ĉ�]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationX(float Angle){
		*this = (Matrix4X4)XMMatrixRotationX(Angle);
	}
/**************************************************************************
 void RotationY(
	float Angle		//X����]
 );
 �p�r: Y����]�Ɋ�Â��ĉ�]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationY(float Angle){
		*this = (Matrix4X4)XMMatrixRotationY(Angle);
	}
/**************************************************************************
 void RotationZ(
	float Angle		//X����]
 );
 �p�r: Z����]�Ɋ�Â��ĉ�]�s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void RotationZ(float Angle){
		*this = (Matrix4X4)XMMatrixRotationZ(Angle);
	}
/**************************************************************************
 void Scaling(
	float ScaleX,	//X�ɉ������X�P�[�����O
	float ScaleY,	//Y�ɉ������X�P�[�����O
	float ScaleZ	//Z�ɉ������X�P�[�����O
 );
 �p�r: x ���Ay ���A����� z ���ɉ����ăX�P�[�����O�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Scaling(float ScaleX,float ScaleY,float ScaleZ){
		*this = (Matrix4X4)XMMatrixScaling(ScaleX,ScaleY,ScaleZ);
	}
/**************************************************************************
 void ScalingFromVector(
	const Vector3& Scale		//�X�P�[�����O���L�q����x�N�g��
 );
 �p�r: �x�N�g���ɉ����ăX�P�[�����O�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
***************************************************************************/
	void ScalingFromVector(const Vector3& Scale){
		*this = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scale));
	}
/**************************************************************************
 void Set(
	float m00,	//(0,0)�̒l
	float m01,	//(0,1)�̒l
	float m02,	//(0,2)�̒l
	float m03,	//(0,3)�̒l
	float m10,	//(1,0)�̒l
	float m11,	//(1,1)�̒l
	float m12,	//(1,2)�̒l
	float m13,	//(1,3)�̒l
	float m20,	//(2,0)�̒l
	float m21,	//(2,1)�̒l
	float m22,	//(2,2)�̒l
	float m23,	//(2,3)�̒l
	float m30,	//(3,0)�̒l
	float m31,	//(3,1)�̒l
	float m32,	//(3,2)�̒l
	float m33	//(3,3)�̒l
 );
 �p�r: �e�l��ݒ肵���s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Set(
		float m00,float m01,float m02,float m03,
		float m10,float m11,float m12,float m13,
		float m20,float m21,float m22,float m23,
		float m30,float m31,float m32,float m33
		){
		*this = (Matrix4X4)XMMatrixSet(
							m00,m01,m02,m03,
							m10,m11,m12,m13,
							m20,m21,m22,m23,
							m30,m31,m32,m33
							);
	}
/**************************************************************************
 void Shadow(
	const Plane& ShadowPlane,			//���
	const Vector4& LightPosition		//���C�g�̈ʒu
 );
 �p�r: �W�I���g���𕽖ʂɎˉe����g�����X�t�H�[���s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 �����C�g��w�v�f�́A0.0�Ńf�B���N�V���i�����C�g�A1.0�Ń|�C���g���C�g�ƂȂ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Shadow(const Plane& ShadowPlane,const Vector4& LightPosition){
		*this = (Matrix4X4)XMMatrixShadow(XMVECTOR(ShadowPlane),XMVECTOR(LightPosition));
	}
/**************************************************************************
 void Transformation(
 	const Vector3& ScalingOrigin,		//�X�P�[�����O�̒��S���L�q���� 3D �x�N�g��
 	const Quaternion& ScalingOrientationQuaternion,		//�X�P�[�����O�̌������L�q����N�I�[�^�j�I��
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Vector3& RotationOrigin,		//��]�̒��S���L�q���� 3D �x�N�g��
 	const Quaternion& RotationQuaternion,		//RotationOrigin �ɂ���Ď�����錴�_�𒆐S�Ƃ����]���L�q����N�I�[�^�j�I��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �g�����X�t�H�[���s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Transformation(const Vector3& ScalingOrigin,const Quaternion& ScalingOrientationQuaternion,const Vector3& Scaling,
 		const Vector3& RotationOrigin,const Quaternion& RotationQuaternion,
		const Vector3& Translation){
		*this = (Matrix4X4)XMMatrixTransformation(
			XMVECTOR(ScalingOrigin),XMVECTOR(ScalingOrientationQuaternion),XMVECTOR(Scaling),
			XMVECTOR(RotationOrigin),XMVECTOR(RotationQuaternion),
			XMVECTOR(Translation)
			);
	}
/**************************************************************************
 void Transformation2D(
 	const Vector2& ScalingOrigin,		//�X�P�[�����O�̒��S���L�q���� 2D �x�N�g��
 	float ScalingOrientation,			//�X�P�[�����O��]�W��
	const Vector2& Scaling,				//x ������� y ���̃X�P�[�����O�W�����܂� 2D �x�N�g��
 	const Vector2& RotationOrigin,		//��]�̒��S���L�q���� 2D �x�N�g��
 	float Rotation,						//��]�̊p�x (���W�A���P��)
	const Vector2& Translation			//���s�ړ����L�q���� 2D �x�N�g��
 );
 �p�r: 2D�g�����X�t�H�[���s���xy���ʂɍ쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Transformation2D(const Vector2& ScalingOrigin,float ScalingOrientation,const Vector2& Scaling,
 	const Vector2& RotationOrigin,float Rotation,
	const Vector2& Translation){
		*this = (Matrix4X4)XMMatrixTransformation2D(XMVECTOR(ScalingOrigin),ScalingOrientation,XMVECTOR(Scaling),
			XMVECTOR(RotationOrigin),Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 void Translation(
	float OffsetX,	//X���ɉ��������s�ړ�
	float OffsetY,	//Y���ɉ��������s�ړ�
	float OffsetZ	//Z���ɉ��������s�ړ�
 );
 �p�r: �w�肳�ꂽ�I�t�Z�b�g���畽�s�ړ��s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Translation(float OffsetX,float OffsetY,float OffsetZ){
		*this = (Matrix4X4)XMMatrixTranslation(OffsetX,OffsetY,OffsetZ);
	}
/**************************************************************************
 void TranslationFromVector(
	const Vector3& Offset				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �x�N�g�����畽�s�ړ��s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void TranslationFromVector(const Vector3& Offset){
		*this = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Offset));
	}
/**************************************************************************
 void Transpose(
	const Matrix4X4& m				//�]�u����s��
 );
 �p�r: �s���]�u����this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Transpose(const Matrix4X4& m){
		*this = (Matrix4X4)XMMatrixTranspose(XMMATRIX(m));
	}
/**************************************************************************
 void Transpose();
 �p�r: this��]�u����this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void Transpose(){
		*this = (Matrix4X4)XMMatrixTranspose(XMMATRIX(*this));
	}
/**************************************************************************
 void DefTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Quaternion& RotationQuaternion,		//��]���L�q����N�I�[�^�j�I��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �X�P�[�����O�A��]�A�ړ�����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void DefTransformation(const Vector3& Scaling,const Quaternion& RotationQuaternion,const Vector3& Translation){
		Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
		Matrix4X4 Qt = (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(RotationQuaternion));
		Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
		*this = Scale * Qt * Trans; 
	}
/**************************************************************************
 void DefTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Vector3& Rotation,			//��]���L�q����x�N�g��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �X�P�[�����O�A��]�A�ړ�����s����쐬����
 �߂�l: �쐬�����s��
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void DefTransformation(const Vector3& Scaling,const Vector3& Rotation,const Vector3& Translation){
		Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
		Matrix4X4 Rot = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Rotation));
		Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
		*this = Scale * Rot * Trans; 
	}
/**************************************************************************
 void STRTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
	const Vector3& Translation,				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 	const Quaternion& RotationQuaternion		//��]���L�q����N�I�[�^�j�I��
 );
 �p�r: �X�P�[�����O�A�ړ��A��]����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ����]���ړ����ɍs��
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void STRTransformation(const Vector3& Scaling,const Vector3& Translation,const Quaternion& RotationQuaternion){
		Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
		Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
		Matrix4X4 Qt = (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(RotationQuaternion));
		*this = Scale * Trans * Qt ; 
	}
/**************************************************************************
 void STRTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
	const Vector3& Translation,				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 	const Vector3& Rotation			//��]���L�q����x�N�g��
 );
 �p�r: �X�P�[�����O�A�ړ��A��]����s����쐬��this�ɐݒ肷��
 �߂�l: �Ȃ�
 ����]���ړ����ɍs��
 ��this�Ɍ��ʂ�ݒ肷��
***************************************************************************/
	void STRTransformation(const Vector3& Scaling,const Vector3& Translation,const Vector3& Rotation){
		Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
		Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
		Matrix4X4 Rot = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Rotation));
		*this = Scale * Trans * Rot;
	}
};


/**************************************************************************
 Vector3 GetRotation();
 �p�r: �N�I�[�^�j�I�������]�s��ɕϊ����ĕԂ�
 �߂�l: ��]�s��
***************************************************************************/
inline Vector3 Quaternion::GetRotation()const{
	Quaternion Temp = *this;
	Temp.Normalize();
	Matrix4X4 mt;
	mt.RotationQuaternion(Temp);
	Vector3 Rot;
	if(mt._32 == 1.0f){
		Rot.x = XM_PI / 2.0f;
		Rot.y = 0;
		Rot.z = -atan2(mt._21, mt._11);
	}
	else if(mt._32 == -1.0f){
		Rot.x = -XM_PI / 2.0f;
		Rot.y = 0;
		Rot.z = -atan2(mt._21, mt._11);
	}
	else{
		Rot.x = -asin(mt._32);
		Rot.y = -atan2(-mt._31,mt._33);
		Rot.z = atan2(mt._12, mt._11);
	}
	return Rot;
}



inline void Quaternion::Billboard(const Vector3& Line){
	Vector3 Temp = Line;
	Matrix4X4 RotMatrix;
	Vector4 dammi(0, 0, 0, 0);
	Vector3 DefUp(0, 1.0f, 0);
	Vector2 TempVec2(Temp.x, Temp.z);
	if (TempVec2.Length() < 0.1f){
		DefUp = Vector3(0, 0, 1.0f);
	}
	Temp.Normalize();
	RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
	RotMatrix.Inverse(&dammi);
	*this = RotMatrix.QtInMatrix();
	this->Normalize();
}


inline void Quaternion::Facing(const Vector3& Line){
	Vector3 Temp = Line;
	Matrix4X4 RotMatrix;
	Vector4 dammi(0, 0, 0, 0);
	Vector3 DefUp(0, 1.0f, 0);
	Vector2 TempVec2(Temp.x, Temp.z);
	if (TempVec2.Length() < 0.1f){
		DefUp = Vector3(0, 0, 1.0f);
	}
	RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
	RotMatrix.Inverse(&dammi);
	*this = RotMatrix.QtInMatrix();
	this->Normalize();
}

inline void Quaternion::FacingY(const Vector3& Line){
	Vector3 Temp = Line;
	Temp.Normalize();
	this->Identity();
	this->RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
	this->Normalize();
}



}
//end of namespace basedx11.

