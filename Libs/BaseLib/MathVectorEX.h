
/******************************************************************************
 MathVectorEX.h

 �x�N�g���̃X�^�e�B�b�N�v�Z
 XNAMATH �̃��b�s���O�X�^�e�B�b�N�N���X�Q

 Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
 DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "StdAfx.h"

namespace basedx11{

/**************************************************************************
 namespace Vector2EX;
 �p�r: 2����XMVECTOR��static�֐��\����
****************************************************************************/
namespace Vector2EX{
	//�쐬
/**************************************************************************
 static inline Vector2 WStr2ToVec2(
	wstring& x_str,
	wstring& y_str
 );
 �p�r: ���[�e�B���e�B�֐�
 �߂�l: wstring����x�N�g�����쐬����
***************************************************************************/
	static inline Vector2 WStr2ToVec2(wstring& x_str, wstring& y_str){
		return
			Vector2(
				(float)_wtof(x_str.c_str()),
				(float)_wtof(y_str.c_str())
			);
	}
	//��r
/**************************************************************************
 static inline bool Equal(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	static inline bool Equal(const Vector2& Vec, const Vector2& other){
		return XMVector2Equal(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static bool EqualInt(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���B
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: ���������true
***************************************************************************/
	static inline bool EqualInt(const Vector2& Vec, const Vector2& other){
		return XMVector2EqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool Greater(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g�����傫�����ǂ��������؂���B
 �߂�l: �傫�����true
***************************************************************************/
	static inline bool Greater(const Vector2& Vec, const Vector2& other){
		return XMVector2Greater(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool GreaterOrEqual(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
 �߂�l: �ȏ�̑傫���Ȃ��true
***************************************************************************/
	static inline bool GreaterOrEqual(const Vector2& Vec, const Vector2& other){
		return XMVector2GreaterOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool IsInfinite(
	const Vector2& Vec		//�x�N�g��
 );
 �p�r: �x�N�g�����l�̂����ꂩ���������̖����傩�ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
***************************************************************************/
	static inline bool IsInfinite(const Vector2& Vec){
		return XMVector2IsInfinite(XMVECTOR(Vec));
	}
/**************************************************************************
 static bool IsNaN(
	const Vector2& Vec		//�x�N�g��
 );
 �p�r: �x�N�g���̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
***************************************************************************/
	static inline bool IsNaN(const Vector2& Vec){
		return XMVector2IsNaN(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline bool Less(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g����菬�������ǂ��������؂���B
 �߂�l: ���������true
***************************************************************************/
	static inline bool Less(const Vector2& Vec, const Vector2& other){
		return XMVector2Less(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool LessOrEqual(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
 �߂�l: �ȉ��̑傫���Ȃ��true
***************************************************************************/
	static inline bool LessOrEqual(const Vector2& Vec, const Vector2& other){
		return XMVector2LessOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool NearEqual(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other,	//��r����x�N�g��
	float Epsilon			//��r����ő勖�e��
 );
 �p�r: �x�N�g����������̃x�N�g�����߂��傫�����ǂ��������؂���B
 �߂�l: �߂��傫���Ȃ��true
***************************************************************************/
	static inline bool NearEqual(const Vector2& Vec, const Vector2& other, float Epsilon){
		float temp = Epsilon;
		XMVECTOR VecEp = XMLoadFloat(&temp);
		return XMVector2NearEqual(XMVECTOR(Vec),XMVECTOR(other),VecEp);
	}
/**************************************************************************
 static inline bool NotEqual(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline bool NotEqual(const Vector2& Vec, const Vector2& other){
		return XMVector2NotEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline bool NotEqualInt(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline bool NotEqualInt(const Vector2& Vec, const Vector2& other){
		return XMVector2NotEqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
//�W�I���g��
/**************************************************************************
 static inline float AngleBetweenNormals(
		const Vector2& Vec1,	//�x�N�g��1
		const Vector2& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z����
 �߂�l: ���W�A���p
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline float AngleBetweenNormals(const Vector2& Vec1, const Vector2& Vec2){
		return ((Vector2)XMVector2AngleBetweenNormals(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline float AngleBetweenNormalsEst(
		const Vector2& Vec1,	//�x�N�g��1
		const Vector2& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z�\������
 �߂�l: ���W�A���p�x��\���x�N�g��
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline float AngleBetweenNormalsEst(const Vector2& Vec1, const Vector2& Vec2){
		return ((Vector2)XMVector2AngleBetweenNormalsEst(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline float AngleBetweenVectors(
		const Vector2& Vec1,	//�x�N�g��1
		const Vector2& Vec2		//�x�N�g��2
 );
 �p�r: ��� �x�N�g���Ԃ̃��W�A���p���v�Z����B
 �߂�l: ���W�A���p�x
***************************************************************************/
	static inline float AngleBetweenVectors(const Vector2& Vec1, const Vector2& Vec2){
		return ((Vector2)XMVector2AngleBetweenVectors(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline Vector2 ClampLength(
	const Vector2& Vec,	//�x�N�g��
	float LengthMin,	//�����̍ŏ��l
	float LengthMax		//�����̍ő�l
 );
 �p�r: �x�N�g���̒������w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�������ʃx�N�g��
***************************************************************************/
	static inline Vector2 ClampLength(const Vector2& Vec, float LengthMin, float LengthMax){
		return (Vector2)XMVector2ClampLength(XMVECTOR(Vec),LengthMin,LengthMax);
	}
/**************************************************************************
 static inline Vector2 ClampLengthV(
	const Vector2& Vec,				//�x�N�g��
	const Vector2& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
	const Vector2& LengthMaxV		//�����̍ő�l(�x�N�g��)
 );
 �p�r: �x�N�g�����w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�����x�N�g��
***************************************************************************/
	static inline Vector2 ClampLengthV(const Vector2& Vec, const Vector2& LengthMinV, const Vector2& LengthMaxV){
		return (Vector2)XMVector2ClampLengthV(XMVECTOR(Vec),XMVECTOR(LengthMinV),XMVECTOR(LengthMaxV));
	}
/**************************************************************************
 static inline float Cross(
	const Vector2& Vec1,	//�x�N�g���P
	const Vector2& Vec2		//�x�N�g���Q
 );
 �p�r: �O�ς��v�Z���ĕԂ�
 �߂�l: �Ȃ�
***************************************************************************/
	static inline float Cross(const Vector2& Vec1, const Vector2& Vec2){
		return ((Vector2)XMVector2Cross(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline float Dot(
	const Vector2& Vec1,	//�x�N�g���P
	const Vector2& Vec2		//�x�N�g���Q
 );
 �p�r: �x�N�g�����m�̓��ς��v�Z���ĕԂ�
 �߂�l: ����
***************************************************************************/
	static inline float Dot(const Vector2& Vec1, const Vector2& Vec2){
		return ((Vector2)XMVector2Dot(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline bool InBounds(
	const Vector2& Vec1,	//��r���x�N�g��
	const Vector2& Vec2		//��r����x�N�g��
 );
 �p�r: ���ׂĂ̗v�f���A�ݒ肳�ꂽ���E�x�N�g�����ɂ���ꍇ��true��Ԃ�
 �߂�l: ���E���ɂ���ꍇ��true
***************************************************************************/
	static inline bool InBounds(const Vector2& Vec1, const Vector2& Vec2){
		return XMVector2InBounds(XMVECTOR(Vec1),XMVECTOR(Vec2));
	}
/**************************************************************************
 static inline float Length(
	const Vector2& Vec,	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����Ԃ�
 �߂�l: �x�N�g���̒���
***************************************************************************/
	static inline float Length(const Vector2& Vec){
		return ((Vector2)XMVector2Length(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline float LengthEst(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����\�z���ĕԂ�
 �߂�l: �x�N�g���̒����̗\�z
***************************************************************************/
	static inline float LengthEst(const Vector2& Vec){
		return ((Vector2)XMVector2LengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline float LengthSq(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����2���Ԃ�
 �߂�l: �x�N�g���̒�����2��
***************************************************************************/
	static inline float LengthSq(const Vector2& Vec){
		return ((Vector2)XMVector2LengthSq(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline Vector2 Normalize(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���𐳋K������
 �߂�l: ���K�������x�N�g��
***************************************************************************/
	static inline Vector2 Normalize(const Vector2& Vec){
		return (Vector2)XMVector2Normalize(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline Vector2 NormalizeEst(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: ���K���̗\�z�l��Ԃ�
 �߂�l: ���K���̗\�z�l
***************************************************************************/
	static inline Vector2 NormalizeEst(const Vector2& Vec){
		return (Vector2)XMVector2NormalizeEst(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline Vector2 Orthogonal(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �����ȃx�N�g����Ԃ�
 �߂�l: �����ȃx�N�g��
***************************************************************************/
	static inline Vector2 Orthogonal(const Vector2& Vec){
		return (Vector2)XMVector2Orthogonal(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline float ReciprocalLength(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t����Ԃ�
 �߂�l: �x�N�g���̒����̋t��
***************************************************************************/
	static inline float ReciprocalLength(const Vector2& Vec){
		return ((Vector2)XMVector2ReciprocalLength(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float ReciprocalLengthEst(
	const Vector2& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t���̗\���l��Ԃ�
 �߂�l: �x�N�g���̒����̋t���̗\���l
***************************************************************************/
	 static inline  float ReciprocalLengthEst(const Vector2& Vec){
		return ((Vector2)XMVector2ReciprocalLengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  Vector2 Reflect(
	const Vector2& Vec,	//�x�N�g��
	const Vector2& Normal	//�@���x�N�g��
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector2 Reflect(const Vector2& Vec,const Vector2& Normal){
		return (Vector2)XMVector2Reflect(XMVECTOR(Vec),XMVECTOR(Normal));
	}
/**************************************************************************
 static inline  Vector2 Refract(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& Normal,	//���܂�����@���x�N�g��
	float RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector2 Refract(const Vector2& Vec,const Vector2& Normal,float RefractionIndex){
		return (Vector2)XMVector2Refract(XMVECTOR(Vec),XMVECTOR(Normal),RefractionIndex);
	}
/**************************************************************************
 static inline  Vector2 RefractV(
	const Vector2& Vec,		//�x�N�g��
	const Vector2& Normal,	//���܂�����@���x�N�g��
	const Vector2& RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
 �@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector2 RefractV(const Vector2& Vec,const Vector2& Normal,const Vector2& RefractionIndex){
		return (Vector2)XMVector2RefractV(XMVECTOR(Vec),XMVECTOR(Normal),
			XMVECTOR(RefractionIndex));
	}
	//�g�����X�t�H�[��
/**************************************************************************
 static inline  Vector2 Transform(
	const Vector2& Vec,		//�x�N�g��
	const XMMATRIX& m		//�g�����X�t�H�[������s��
 );
 �p�r: �x�N�g����m�ɂ���ăg�����X�t�H�[���������x�N�g����Ԃ�
 �߂�l: �g�����X�t�H�[���������x�N�g��
***************************************************************************/
	static inline  Vector2 Transform(const Vector2& Vec,const XMMATRIX& m){
		return (Vector2)XMVector2Transform(XMVECTOR(Vec),m);
	}

};
// end Vector2EX

/**************************************************************************
 struct Vector3EX;
 �p�r: 3����XMVECTOR��static inline �֐��\����
****************************************************************************/
namespace Vector3EX{
	//�쐬
/**************************************************************************
 static inline  Vector3 WStr3ToVec3(
	wstring& x_str,
	wstring& y_str,
	wstring& z_str
 );
 �p�r: ���[�e�B���e�B�֐�
 �߂�l: wstring����x�N�g�����쐬����
***************************************************************************/
	static inline  Vector3 WStr3ToVec3(wstring& x_str,wstring& y_str,wstring& z_str){
		return
			Vector3(
				(float)_wtof(x_str.c_str()),
				(float)_wtof(y_str.c_str()),
				(float)_wtof(z_str.c_str())
			);
	}
	//��r
/**************************************************************************
 static inline  bool Equal(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Equal(const Vector3& Vec,const Vector3& other){
		return XMVector3Equal(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool EqualInt(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���B
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: ���������true
***************************************************************************/
	static inline  bool EqualInt(const Vector3& Vec,const Vector3& other){
		return XMVector3EqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Greater(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g�����傫�����ǂ��������؂���B
 �߂�l: �傫�����true
***************************************************************************/
	static inline  bool Greater(const Vector3& Vec,const Vector3& other){
		return XMVector3Greater(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool GreaterOrEqual(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
 �߂�l: �ȏ�̑傫���Ȃ��true
***************************************************************************/
	static inline  bool GreaterOrEqual(const Vector3& Vec,const Vector3& other){
		return XMVector3GreaterOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Vector3& Vec		//�x�N�g��
 );
 �p�r: �x�N�g�����l�̂����ꂩ���������̖����傩�ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
***************************************************************************/
	static inline  bool IsInfinite(const Vector3& Vec){
		return XMVector3IsInfinite(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Vector3& Vec		//�x�N�g��
 );
 �p�r: �x�N�g���̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
***************************************************************************/
	static inline  bool IsNaN(const Vector3& Vec){
		return XMVector3IsNaN(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  bool Less(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g����菬�������ǂ��������؂���B
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Less(const Vector3& Vec,const Vector3& other){
		return XMVector3Less(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool LessOrEqual(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
 �߂�l: �ȉ��̑傫���Ȃ��true
***************************************************************************/
	static inline  bool LessOrEqual(const Vector3& Vec,const Vector3& other){
		return XMVector3LessOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NearEqual(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other,	//��r����x�N�g��
	float Epsilon			//��r����ő勖�e��
 );
 �p�r: �x�N�g����������̃x�N�g�����߂��傫�����ǂ��������؂���B
 �߂�l: �߂��傫���Ȃ��true
***************************************************************************/
	static inline  bool NearEqual(const Vector3& Vec,const Vector3& other,float Epsilon){
		float temp = Epsilon;
		XMVECTOR VecEp = XMLoadFloat(&temp);
		return XMVector3NearEqual(XMVECTOR(Vec),XMVECTOR(other),VecEp);
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqual(const Vector3& Vec,const Vector3& other){
		return XMVector3NotEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NotEqualInt(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqualInt(const Vector3& Vec,const Vector3& other){
		return XMVector3NotEqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
//�W�I���g��
/**************************************************************************
 static inline  float AngleBetweenNormals(
		const Vector3& Vec1,	//�x�N�g��1
		const Vector3& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z����
 �߂�l: ���W�A���p�x
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline  float AngleBetweenNormals(const Vector3& Vec1,const Vector3& Vec2){
		return ((Vector3)XMVector3AngleBetweenNormals(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  float AngleBetweenNormalsEst(
		const Vector3& Vec1,	//�x�N�g��1
		const Vector3& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z�\������
 �߂�l: ���W�A���p�x
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline  float AngleBetweenNormalsEst(const Vector3& Vec1,const Vector3& Vec2){
		return ((Vector3)XMVector3AngleBetweenNormalsEst(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  float AngleBetweenVectors(
		const Vector3& Vec1,	//�x�N�g��1
		const Vector3& Vec2		//�x�N�g��2
 );
 �p�r: ��� �x�N�g���Ԃ̃��W�A���p���v�Z����B
 �߂�l: ���W�A���p�x��\���x�N�g��
***************************************************************************/
	static inline  float AngleBetweenVectors(const Vector3& Vec1,const Vector3& Vec2){
		return ((Vector3)XMVector3AngleBetweenVectors(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  Vector3 ClampLength(
	const Vector3& Vec,	//�x�N�g��
	float LengthMin,	//�����̍ŏ��l
	float LengthMax		//�����̍ő�l
 );
 �p�r: �x�N�g���̒������w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�������ʃx�N�g��
***************************************************************************/
	static inline  Vector3 ClampLength(const Vector3& Vec,float LengthMin,float LengthMax){
		return (Vector3)XMVector3ClampLength(XMVECTOR(Vec),LengthMin,LengthMax);
	}
/**************************************************************************
 static inline  Vector3 ClampLengthV(
	const Vector3& Vec,				//�x�N�g��
	const Vector3& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
	const Vector3& LengthMaxV		//�����̍ő�l(�x�N�g��)
 );
 �p�r: �x�N�g�����w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�����x�N�g��
***************************************************************************/
	static inline  Vector3 ClampLengthV(const Vector3& Vec,const Vector3& LengthMinV,const Vector3& LengthMaxV){
		return (Vector3)XMVector3ClampLengthV(XMVECTOR(Vec),XMVECTOR(LengthMinV),XMVECTOR(LengthMaxV));
	}
/**************************************************************************
 static inline  Vector3 Cross(
	const Vector3& Vec1,	//�x�N�g���P
	const Vector3& Vec2		//�x�N�g���Q
 );
 �p�r: �O�ς��v�Z���ĕԂ�
 �߂�l: �O��
***************************************************************************/
	static inline  Vector3 Cross(const Vector3& Vec1,const Vector3& Vec2){
		return (Vector3)XMVector3Cross(XMVECTOR(Vec1),XMVECTOR(Vec2));
	}
/**************************************************************************
 static inline  float Dot(
	const Vector3& Vec1,	//�x�N�g���P
	const Vector3& Vec2		//�x�N�g���Q
 );
 �p�r: �x�N�g�����m�̓��ς��v�Z���ĕԂ�
 �߂�l: ����
***************************************************************************/
	static inline  float Dot(const Vector3& Vec1,const Vector3& Vec2){
		return ((Vector3)XMVector3Dot(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  bool InBounds(
	const Vector3& Vec1,	//��r���x�N�g��
	const Vector3& Vec2		//��r����x�N�g��
 );
 �p�r: ���ׂĂ̗v�f���A�ݒ肳�ꂽ���E�x�N�g�����ɂ���ꍇ��true��Ԃ�
 �߂�l: ���E���ɂ���ꍇ��true
***************************************************************************/
	static inline  bool InBounds(const Vector3& Vec1,const Vector3& Vec2){
		return XMVector3InBounds(XMVECTOR(Vec1),XMVECTOR(Vec2));
	}
/**************************************************************************
 static inline  float Length(
	const Vector3& Vec,	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����Ԃ�
 �߂�l: �x�N�g���̒���
***************************************************************************/
	static inline float Length(const Vector3& Vec){
		return ((Vector3)XMVector3Length(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float LengthEst(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����\�z���ĕԂ�
 �߂�l: �x�N�g���̒����̗\�z
***************************************************************************/
	static inline  float LengthEst(const Vector3& Vec){
		return ((Vector3)XMVector3LengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float LengthSq(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����2���Ԃ�
 �߂�l: �x�N�g���̒�����2��
***************************************************************************/
	static inline  float LengthSq(const Vector3& Vec){
		return ((Vector3)XMVector3LengthSq(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  Vector3 Normalize(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���𐳋K������
 �߂�l: ���K�������x�N�g��
***************************************************************************/
	static inline  Vector3 Normalize(const Vector3& Vec){
		return (Vector3)XMVector3Normalize(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Vector3 NormalizeEst(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: ���K���̗\�z�l��Ԃ�
 �߂�l: ���K���̗\�z�l
***************************************************************************/
	 static inline  Vector3 NormalizeEst(const Vector3& Vec){
		return (Vector3)XMVector3NormalizeEst(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Vector3 Orthogonal(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �����ȃx�N�g����Ԃ�
 �߂�l: �����ȃx�N�g��
***************************************************************************/
	 static inline  Vector3 Orthogonal(const Vector3& Vec){
		return (Vector3)XMVector3Orthogonal(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  float ReciprocalLength(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t����Ԃ�
 �߂�l: �x�N�g���̒����̋t��
***************************************************************************/
	 static inline  float ReciprocalLength(const Vector3& Vec){
		return ((Vector3)XMVector3ReciprocalLength(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float ReciprocalLengthEst(
	const Vector3& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t���̗\���l��Ԃ�
 �߂�l: �x�N�g���̒����̋t���̗\���l
***************************************************************************/
	 static inline  float ReciprocalLengthEst(const Vector3& Vec){
		return ((Vector3)XMVector3ReciprocalLengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  Vector3 Reflect(
	const Vector3& Vec,	//�x�N�g��
	const Vector3& Normal	//�@���x�N�g��
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector3 Reflect(const Vector3& Vec,const Vector3& Normal){
		return (Vector3)XMVector3Reflect(XMVECTOR(Vec),XMVECTOR(Normal));
	}
/**************************************************************************
 static inline  Vector3 Refract(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& Normal,	//���܂�����@���x�N�g��
	float RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��this�ɐݒ肷��
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector3 Refract(const Vector3& Vec,const Vector3& Normal,float RefractionIndex){
		return (Vector3)XMVector3Refract(XMVECTOR(Vec),XMVECTOR(Normal),RefractionIndex);
	}
/**************************************************************************
 static inline  Vector3 RefractV(
	const Vector3& Vec,		//�x�N�g��
	const Vector3& Normal,	//���܂�����@���x�N�g��
	const Vector3& RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
 �@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector3 RefractV(const Vector3& Vec,const Vector3& Normal,const Vector3& RefractionIndex){
		return (Vector3)XMVector3RefractV(XMVECTOR(Vec),XMVECTOR(Normal),
			XMVECTOR(RefractionIndex));
	}
	//�g�����X�t�H�[��
/**************************************************************************
 static inline  Vector3 Transform(
	const Vector3& Vec,		//�x�N�g��
	const XMMATRIX& m		//�g�����X�t�H�[������s��
 );
 �p�r: �x�N�g����m�ɂ���ăg�����X�t�H�[���������x�N�g����Ԃ�
 �߂�l: �g�����X�t�H�[���������x�N�g��
***************************************************************************/
	static inline  Vector3 Transform(const Vector3& Vec,const XMMATRIX& m){
		return (Vector3)XMVector3Transform(XMVECTOR(Vec),m);
	}

};
//end Vector3EX

/**************************************************************************
 namespace Vector4EX;
 �p�r: 4����XMVECTOR��static inline �֐��\����
****************************************************************************/
namespace Vector4EX{
	//�쐬
/**************************************************************************
 static inline  Vector4 WStr4ToVec4(
	wstring& x_str,
	wstring& y_str,
	wstring& z_str,
	wstring& w_str
 );
 �p�r: ���[�e�B���e�B�֐�
 �߂�l: wstring����x�N�g�����쐬����
***************************************************************************/
	static inline  Vector4 WStr4ToVec4(wstring& x_str,wstring& y_str,wstring& z_str,wstring& w_str){
		return
			Vector4(
				(float)_wtof(x_str.c_str()),
				(float)_wtof(y_str.c_str()),
				(float)_wtof(z_str.c_str()),
				(float)_wtof(w_str.c_str())
			);
	}
	//��r
/**************************************************************************
 static inline  bool Equal(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Equal(const Vector4& Vec,const Vector4& other){
		return XMVector4Equal(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool EqualInt(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ��������ǂ��������؂���B
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: ���������true
***************************************************************************/
	static inline  bool EqualInt(const Vector4& Vec,const Vector4& other){
		return XMVector4EqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Greater(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g�����傫�����ǂ��������؂���B
 �߂�l: �傫�����true
***************************************************************************/
	static inline  bool Greater(const Vector4& Vec,const Vector4& other){
		return XMVector4Greater(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool GreaterOrEqual(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȏ�̑傫�����ǂ��������؂���B
 �߂�l: �ȏ�̑傫���Ȃ��true
***************************************************************************/
	static inline  bool GreaterOrEqual(const Vector4& Vec,const Vector4& other){
		return XMVector4GreaterOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Vector4& Vec		//�x�N�g��
 );
 �p�r: �x�N�g�����l�̂����ꂩ���������̖����傩�ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
***************************************************************************/
	static inline  bool IsInfinite(const Vector4& Vec){
		return XMVector4IsInfinite(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Vector4& Vec		//�x�N�g��
 );
 �p�r: �x�N�g���̒l�̂����ꂩ���񐔁i�����l�j���ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���񐔁i�����l�j�Ȃ��true
***************************************************************************/
	static inline  bool IsNaN(const Vector4& Vec){
		return XMVector4IsNaN(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  bool Less(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g����菬�������ǂ��������؂���B
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Less(const Vector4& Vec,const Vector4& other){
		return XMVector4Less(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool LessOrEqual(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���ȉ��̑傫�����ǂ��������؂���B
 �߂�l: �ȉ��̑傫���Ȃ��true
***************************************************************************/
	static inline  bool LessOrEqual(const Vector4& Vec,const Vector4& other){
		return XMVector4LessOrEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NearEqual(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other,	//��r����x�N�g��
	float Epsilon			//��r����ő勖�e��
 );
 �p�r: �x�N�g����������̃x�N�g�����߂��傫�����ǂ��������؂���B
 �߂�l: �߂��傫���Ȃ��true
***************************************************************************/
	static inline  bool NearEqual(const Vector4& Vec,const Vector4& other,float Epsilon){
		float temp = Epsilon;
		XMVECTOR VecEp = XMLoadFloat(&temp);
		return XMVector4NearEqual(XMVECTOR(Vec),XMVECTOR(other),VecEp);
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqual(const Vector4& Vec,const Vector4& other){
		return XMVector4NotEqual(XMVECTOR(Vec),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NotEqualInt(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& other	//��r����x�N�g��
 );
 �p�r: �x�N�g����������̃x�N�g���Ɠ������Ȃ����ǂ��������؂���
		���ꂼ��̗v�f�͕����Ȃ������Ƃ��Ĉ����B
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqualInt(const Vector4& Vec,const Vector4& other){
		return XMVector4NotEqualInt(XMVECTOR(Vec),XMVECTOR(other));
	}
//�W�I���g��
/**************************************************************************
 static inline  float AngleBetweenNormals(
		const Vector4& Vec1,	//�x�N�g��1
		const Vector4& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z����
 �߂�l: ���W�A���p�x
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline  float AngleBetweenNormals(const Vector4& Vec1,const Vector4& Vec2){
		return ((Vector4)XMVector4AngleBetweenNormals(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  float AngleBetweenNormalsEst(
		const Vector4& Vec1,	//�x�N�g��1
		const Vector4& Vec2		//�x�N�g��2
 );
 �p�r: ���K�������x�N�g�����m�̃��W�A���p�x���v�Z�\������
 �߂�l: ���W�A���p�x��\���x�N�g��
 �����K���͂��Ȃ��̂ŁA�����͂͂��炩���ߐ��K�����Ă���
 ���x�N�g���̃��W�A���p�v�Z�ł́A���K�����Ȃ�������
***************************************************************************/
	static inline  float AngleBetweenNormalsEst(const Vector4& Vec1,const Vector4& Vec2){
		return ((Vector4)XMVector4AngleBetweenNormalsEst(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  float AngleBetweenVectors(
		const Vector4& Vec1,	//�x�N�g��1
		const Vector4& Vec2		//�x�N�g��2
 );
 �p�r: ��� �x�N�g���Ԃ̃��W�A���p���v�Z����B
 �߂�l: ���W�A���p�x��\���x�N�g��
***************************************************************************/
	static inline  float AngleBetweenVectors(const Vector4& Vec1,const Vector4& Vec2){
		return ((Vector4)XMVector4AngleBetweenVectors(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  Vector4 ClampLength(
	const Vector4& Vec,	//�x�N�g��
	float LengthMin,	//�����̍ŏ��l
	float LengthMax		//�����̍ő�l
 );
 �p�r: �x�N�g���̒������w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�������ʃx�N�g��
***************************************************************************/
	static inline  Vector4 ClampLength(const Vector4& Vec,float LengthMin,float LengthMax){
		return (Vector4)XMVector4ClampLength(XMVECTOR(Vec),LengthMin,LengthMax);
	}
/**************************************************************************
 static inline  Vector4 ClampLengthV(
	const Vector4& Vec,				//�x�N�g��
	const Vector4& LengthMinV,		//�����̍ŏ��l(�x�N�g��)
	const Vector4& LengthMaxV		//�����̍ő�l(�x�N�g��)
 );
 �p�r: �x�N�g�����w�肵���傫���ɃN�����v����
 �߂�l: �N�����v�����x�N�g��
***************************************************************************/
	static inline  Vector4 ClampLengthV(const Vector4& Vec,const Vector4& LengthMinV,const Vector4& LengthMaxV){
		return (Vector4)XMVector4ClampLengthV(XMVECTOR(Vec),XMVECTOR(LengthMinV),XMVECTOR(LengthMaxV));
	}
/**************************************************************************
 static inline  Vector4 Cross(
	const Vector4& Vec1,	//�x�N�g���P
	const Vector4& Vec2,	//�x�N�g���Q
	const Vector4& Vec3		//�x�N�g���R
 );
 �p�r: �O�ς��v�Z���ĕԂ�
 �߂�l: �Ȃ�
***************************************************************************/
	static inline  Vector4 Cross(const Vector4& Vec1,const Vector4& Vec2,const Vector4& Vec3){
		return (Vector4)XMVector4Cross(XMVECTOR(Vec1),XMVECTOR(Vec2),XMVECTOR(Vec3));
	}
/**************************************************************************
 static inline  float Dot(
	const Vector4& Vec1,	//�x�N�g���P
	const Vector4& Vec2		//�x�N�g���Q
 );
 �p�r: �x�N�g�����m�̓��ς��v�Z���ĕԂ�
 �߂�l: ����
***************************************************************************/
	static inline  float Dot(const Vector4& Vec1,const Vector4& Vec2){
		return ((Vector4)XMVector4Dot(XMVECTOR(Vec1),XMVECTOR(Vec2))).x;
	}
/**************************************************************************
 static inline  bool InBounds(
	const Vector4& Vec1,	//��r���x�N�g��
	const Vector4& Vec2		//��r����x�N�g��
 );
 �p�r: ���ׂĂ̗v�f���A�ݒ肳�ꂽ���E�x�N�g�����ɂ���ꍇ��true��Ԃ�
 �߂�l: ���E���ɂ���ꍇ��true
***************************************************************************/
	static inline  bool InBounds(const Vector4& Vec1,const Vector4& Vec2){
		return XMVector4InBounds(XMVECTOR(Vec1),XMVECTOR(Vec2));
	}
/**************************************************************************
 static inline  float Length(
	const Vector4& Vec,	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����Ԃ�
 �߂�l: �x�N�g���̒���
***************************************************************************/
	static inline  float Length(const Vector4& Vec){
		return ((Vector4)XMVector4Length(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float LengthEst(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����\�z���ĕԂ�
 �߂�l: �x�N�g���̒����̗\�z
***************************************************************************/
	static inline  float LengthEst(const Vector4& Vec){
		return ((Vector4)XMVector4LengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float LengthSq(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒�����2���Ԃ�
 �߂�l: �x�N�g���̒�����2��
***************************************************************************/
	static inline  float LengthSq(const Vector4& Vec){
		return ((Vector4)XMVector4LengthSq(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  Vector4 Normalize(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���𐳋K������
 �߂�l: ���K�������x�N�g��
***************************************************************************/
	static inline  Vector4 Normalize(const Vector4& Vec){
		return (Vector4)XMVector4Normalize(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Vector4 NormalizeEst(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: ���K���̗\�z�l��Ԃ�
 �߂�l: ���K���̗\�z�l
***************************************************************************/
	 static inline  Vector4 NormalizeEst(const Vector4& Vec){
		return (Vector4)XMVector4NormalizeEst(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Vector4 Orthogonal(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �����ȃx�N�g����Ԃ�
 �߂�l: �����ȃx�N�g��
***************************************************************************/
	 static inline  Vector4 Orthogonal(const Vector4& Vec){
		return (Vector4)XMVector4Orthogonal(XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  float ReciprocalLength(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t����Ԃ�
 �߂�l: �x�N�g���̒����̋t��
***************************************************************************/
	 static inline  float ReciprocalLength(const Vector4& Vec){
		return ((Vector4)XMVector4ReciprocalLength(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  float ReciprocalLengthEst(
	const Vector4& Vec	//�x�N�g��
 );
 �p�r: �x�N�g���̒����̋t���̗\���l��Ԃ�
 �߂�l: �x�N�g���̒����̋t���̗\���l
***************************************************************************/
	 static inline  float ReciprocalLengthEst(const Vector4& Vec){
		return ((Vector4)XMVector4ReciprocalLengthEst(XMVECTOR(Vec))).x;
	}
/**************************************************************************
 static inline  Vector4 Reflect(
	const Vector4& Vec,	//�x�N�g��
	const Vector4& Normal	//�@���x�N�g��
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���Ĕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector4 Reflect(const Vector4& Vec,const Vector4& Normal){
		return (Vector4)XMVector4Reflect(XMVECTOR(Vec),XMVECTOR(Normal));
	}
/**************************************************************************
 static inline  Vector4 Refract(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& Normal,	//���܂�����@���x�N�g��
	float RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��Ŕ��˂������x�N�g��(���˃x�N�g��)��this�ɐݒ肷��
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector4 Refract(const Vector4& Vec,const Vector4& Normal,float RefractionIndex){
		return (Vector4)XMVector4Refract(XMVECTOR(Vec),XMVECTOR(Normal),RefractionIndex);
	}
/**************************************************************************
 static inline  Vector4 RefractV(
	const Vector4& Vec,		//�x�N�g��
	const Vector4& Normal,	//���܂�����@���x�N�g��
	const Vector4& RefractionIndex	//���ܗ�
 );
 �p�r: �x�N�g����@���x�N�g���ɂ���ċ��ܗ��x�N�g���i���ׂĂ̗v�f�������l�j
 �@�@�@�Ŕ��˂������x�N�g��(���˃x�N�g��)��Ԃ�
 �߂�l: ���˂������x�N�g��
***************************************************************************/
	static inline  Vector4 RefractV(const Vector4& Vec,const Vector4& Normal,const Vector4& RefractionIndex){
		return (Vector4)XMVector4RefractV(XMVECTOR(Vec),XMVECTOR(Normal),
			XMVECTOR(RefractionIndex));
	}
	//�g�����X�t�H�[��
/**************************************************************************
 static inline  Vector4 Transform(
	const Vector4& Vec,		//�x�N�g��
	const XMMATRIX& m		//�g�����X�t�H�[������s��
 );
 �p�r: �x�N�g����m�ɂ���ăg�����X�t�H�[���������x�N�g����Ԃ�
 �߂�l: �g�����X�t�H�[���������x�N�g��
***************************************************************************/
	static inline  Vector4 Transform(const Vector4& Vec,const XMMATRIX& m){
		return (Vector4)XMVector4Transform(XMVECTOR(Vec),m);
	}

};
//end Vector4EX


}
//end of namespace basedx11.



