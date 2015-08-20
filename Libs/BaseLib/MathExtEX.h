
/******************************************************************************
 MathExtEX.h

 �s��A�N�I�[�^�j�I���A�J���[�Ȃǂ̃X�^�e�B�b�N�v�Z
 XNAMATH �̃��b�s���O�X�^�e�B�b�N�֐��Q

 Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
 DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "StdAfx.h"

namespace basedx11{

namespace PlaneEX{
/**************************************************************************
 static inline  float Dot(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Vector4& other	//�v�Z����Vector4
 );
 �p�r: plane��other�̓��ς�Ԃ�
 �߂�l: ����
***************************************************************************/
	static inline  float Dot(const Plane& plane,const Vector4& other){
		return ((Vector4)XMPlaneDot(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  float DotCoord(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Vector3& other	//�����Vector3
 );
 �p�r: plane��Vector3�̓��ς��v�Z���Ԃ�
 �߂�l: ����
 ������|�C���g���畽�ʂ܂ł̕����t�����������߂�̂ɕ֗��ł���
***************************************************************************/
	static inline  float DotCoord(const Plane& plane,const Vector3& other){
		return  ((Vector4)XMPlaneDotCoord(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  float DotNormal(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Vector3& other	//�����Vector3
 );
 �p�r:�@plane�̖@���x�N�g���� 3D �x�N�g���̊Ԃ̓��ς��v�Z���Ԃ�
 �߂�l: ����
 �����ʂ̖@���x�N�g���ƕʂ̖@���x�N�g���Ƃ̊p�x���Z�o����Ƃ��ɖ𗧂�
***************************************************************************/
	static inline  float DotNormal(const Plane& plane,const Vector3& other){
		return ((Vector4)XMPlaneDotNormal(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  bool Equal(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Plane& other	//�����Plane
 );
 �p�r:�@plane�Ƒ���̕��ʂ��������ǂ����𒲂ׂ�
 �߂�l: �����Ȃ�true
***************************************************************************/
	static inline  bool Equal(const Plane& plane,const Plane& other){
		return XMPlaneEqual(XMVECTOR(plane),XMVECTOR(other));
	}
/**************************************************************************
 static inline  Plane FromPointNormal(
	const Vector3& Point,	//���ʏ�̓_
	const Vector3& Normal	//�@��
 );
 �p�r:�@�_�Ɩ@�����畽�ʂ��쐬���āA�Ԃ�
 �߂�l: �쐬����Plane
***************************************************************************/
	static inline  Plane FromPointNormal(const Vector3& Point,const Vector3& Normal){
		return (Plane)XMPlaneFromPointNormal(XMVECTOR(Point),XMVECTOR(Normal));
	}
/**************************************************************************
 static inline  Plane FromPoints(
	const Vector3& Point1,	//���_1
	const Vector3& Point2,	//���_2
	const Vector3& Point3	//���_3
 );
 �p�r:�@�R�_���畽�ʂ��쐬���āA�Ԃ�
 �߂�l: �쐬����Plane
***************************************************************************/
	static inline  Plane FromPoints(const Vector3& Point1,const Vector3& Point2,const Vector3& Point3){
		return (Plane)XMPlaneFromPoints(XMVECTOR(Point1),XMVECTOR(Point2),XMVECTOR(Point3));
	}
/**************************************************************************
 static inline  Vector3 IntersectLine(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Vector3& LinePoint1,	//���_1
	const Vector3& LinePoint2	//���_2
 );
 �p�r:�@plane�ƂQ�̒��_���쐬���钼���̌������钸�_��Ԃ�
 �߂�l: plane�ƌ�������R�c�x�N�g��
 �����ʂƒ��������s�̏ꍇ�́A�߂�e�l��QNaN�i�񐔁j �ɂȂ�̂ŁA
 �@Vector3::IsNaN()�ȂǂŃ`�F�b�N����
***************************************************************************/
	static inline  Vector3 IntersectLine(const Plane& plane,
		const Vector3& LinePoint1,const Vector3& LinePoint2){
		return (Vector3)XMPlaneIntersectLine(XMVECTOR(plane),
			XMVECTOR(LinePoint1),XMVECTOR(LinePoint2));
	}
/**************************************************************************
 static inline  void IntersectPlane(
	Vector3& RetLinePoint1,	//�Ԃ���钸�_1
	Vector3& RetLinePoint2,	//�Ԃ���钸�_2
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Plane& other		//plane�Ɣ�ׂ��������̖�
 );
 �p�r:�@plane�Ƃ�����̖ʂ���������2�̒��_���v�Z����
 �߂�l: �Ȃ�
 ��RetLinePoint1��RetLinePoint2�Ɍ������钸�_��Ԃ�
 ��2�̕��ʂ����s�̏ꍇ�́A�߂�e�l��QNaN�i�񐔁j �ɂȂ�̂ŁA
 �@Vector3::IsNaN()�ȂǂŃ`�F�b�N����
***************************************************************************/
	static inline  void IntersectPlane(Vector3& RetLinePoint1,Vector3& RetLinePoint2,
		const Plane& plane,const Plane& other){
		XMVECTOR Temp1,Temp2;
		XMPlaneIntersectPlane(&Temp1,&Temp2,XMVECTOR(plane),XMVECTOR(other));
		RetLinePoint1 = (Vector3)Temp1;
		RetLinePoint2 = (Vector3)Temp2;
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Plane& plane		//�v�Z���Ƃ̕���
 );
 �p�r:�@plane�̗v�f�̂����ꂩ�����܂��͕��̖����傩�ǂ����𒲂ׂ�
 �߂�l: ���܂��͕��̖�����Ȃ�true
***************************************************************************/
	static inline  bool IsInfinite(const Plane& plane){
		return XMPlaneIsInfinite(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Plane& plane		//�v�Z���Ƃ̕���
 );
 �p�r:�@Plane�̗v�f�̂����ꂩ�� NaN���ǂ����𒲂ׂ�
 �߂�l: NaN�Ȃ�true
 ��this�͕ύX���Ȃ�
***************************************************************************/
	static inline  bool IsNaN(const Plane& plane){
		return XMPlaneIsNaN(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool NearEqual(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Plane& other		//plane�Ɣ�ׂ��������̖�
	const Vector4& Epsilon	//�e�v�f�̋��e�͈�
 );
 �p�r:�@plane�Ƃ�����̕��ʂ��قړ��������ǂ������ׂ�
 �߂�l: �قړ��������true
***************************************************************************/
	static inline  bool NearEqual(const Plane& plane,const Plane& other,const Vector4& Epsilon){
		return XMPlaneNearEqual(XMVECTOR(plane),XMVECTOR(other),Epsilon);
	}
/**************************************************************************
 static inline  Plane Normalize(
	const Plane& plane		//�v�Z���Ƃ̕���
 );
 �p�r:plane�𐳋K������
 �߂�l:���K����������
***************************************************************************/
	static inline  Plane Normalize(const Plane& plane){
		return (Plane)XMPlaneNormalize(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  Plane NormalizeEst(
	const Plane& plane		//�v�Z���Ƃ̕���
 );
 �p�r:plane��x,y,z�̌W�����P�ʂ̖@���x�N�g�����`������悤�ɁA���ʂ̌W����\��
 �߂�l: �\����������
***************************************************************************/
	static inline  Plane NormalizeEst(const Plane& plane){
		return (Plane)XMPlaneNormalizeEst(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const Plane& other	//�����Plane
 );
 �p�r:�@plane�Ƒ���̕��ʂ��������Ȃ����ǂ����𒲂ׂ�
 �߂�l: �������Ȃ��Ȃ�true
***************************************************************************/
	static inline  bool NotEqual(const Plane& plane,const Plane& other){
		return XMPlaneNotEqual(XMVECTOR(plane),XMVECTOR(other));
	}
/**************************************************************************
 static inline  Plane Transform(
	const Plane& plane,		//�v�Z���Ƃ̕���
	const XMMATRIX& m	//�g�����X�t�H�[������s��
 );
 �p�r:plane��m�Ńg�����X�t�H�[������
 �߂�l: �g�����X�t�H�[����������
***************************************************************************/
	static inline  Plane Transform(const Plane& plane,const XMMATRIX& m){
		return (Plane)XMPlaneTransform(XMVECTOR(plane),m);
	}
//�@XMPlaneTransformStream�֐��́A���b�s���O�֐��͗p�ӂ��Ȃ�
//�@���R�F�p�����[�^�ɔz���n���K�v������A���b�s���O�ɂ���āA�œK�������Ȃ���
};

namespace Color4EX{
	//��r
/**************************************************************************
 static inline  bool Equal(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[�Ɠ��������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Equal(const Color4& Col,const Color4& other){
		return XMColorEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[�Ɠ������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqual(const Color4& Col,const Color4& other){
		return XMColorNotEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Greater(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[���傫�����ǂ��������؂���
 �߂�l: �傫�����true
***************************************************************************/
	static inline  bool Greater(const Color4& Col,const Color4& other){
		return XMColorGreater(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool GreaterOrEqual(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[�ȏォ�ǂ��������؂���
 �߂�l: �ȏ�ł����true
***************************************************************************/
	static inline  bool GreaterOrEqual(const Color4& Col,const Color4& other){
		return XMColorGreaterOrEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool LessOrEqual(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[�ȉ����ǂ��������؂���
 �߂�l: �ȉ��ł����true
***************************************************************************/
	static inline  bool LessOrEqual(const Color4& Col,const Color4& other){
		return XMColorLessOrEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Less(
	const Color4& Col,		//�J���[
	const Color4& other		//��r����J���[
 );
 �p�r: �J���[��������̃J���[��菬�������ǂ��������؂���
 �߂�l: ��菬�������true
***************************************************************************/
	static inline  bool Less(const Color4& Col,const Color4& other){
		return XMColorLess(XMVECTOR(Col),XMVECTOR(other));
	}
	//���
/**************************************************************************
 static inline  bool IsInfinite(
	const Color4& Col		//�J���[
 );
 �p�r: �J���[�̂����ꂩ�̐��������܂��͕��̖����傩�ǂ������e�X�g����
 �߂�l: �J���[�̂����ꂩ�̐��������܂��͕��̖�����Ȃ�true
***************************************************************************/
	static inline  bool IsInfinite(const Color4& Col){
		return XMColorIsInfinite(XMVECTOR(Col));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Color4& Col		//�J���[
 );
 �p�r: �J���[�̂����ꂩ�̐�����NaN���ǂ������e�X�g����
 �߂�l: �J���[�̂����ꂩ�̐�����NaN�Ȃ�true
***************************************************************************/
	static inline  bool IsNaN(const Color4& Col){
		return XMColorIsNaN(XMVECTOR(Col));
	}
	//����
/**************************************************************************
 static inline  Color4 AdjustRange (
		const Color4& Col	//�J���[
 );
 �p�r: �J���[��0.0����1.0�͈̔͂ɒ�������
 �߂�l: ���������J���[
 ���͈͊O�̗v�f��0.0��1.0�ɐݒ肳���
***************************************************************************/
	static inline  Color4 AdjustRange(const Color4& Col){
		Color4 temp = Col;
		if(temp.x <= 0){
			temp.x = 0;
		}
		if(temp.y <= 0){
			temp.y = 0;
		}
		if(temp.z <= 0){
			temp.z = 0;
		}
		if(temp.w <= 0){
			temp.w = 0;
		}
		if(temp.x >= 1.0f){
			temp.x = 1.0f;
		}
		if(temp.y >= 1.0f){
			temp.y = 1.0f;
		}
		if(temp.z >= 1.0f){
			temp.z = 1.0f;
		}
		if(temp.w >= 1.0f){
			temp.w = 1.0f;
		}
		return temp;
	}
/**************************************************************************
 static inline  Color4 AdjustContrast(
		const Color4& Col,	//�J���[
		float Contrast		//�R���g���X�g�͈�
 );
 �p�r: �J���[�̃R���g���X�g�𒲐߂���
 �߂�l: ���������J���[
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
***************************************************************************/
	static inline  Color4 AdjustContrast(const Color4& Col,float Contrast){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorAdjustContrast(XMVECTOR(temp),Contrast);
	}
/**************************************************************************
 static inline  Color4 AdjustSaturation(
		const Color4& Col,	//�J���[
		float Saturation		//�ʓx�l
 );
 �p�r: �J���[�̍ʓx�𒲐߂���
 �߂�l: ���������J���[
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
 ���ʓx�l��0�̏ꍇ�̓O���[�X�P�[���A1.0�̏ꍇ�͂��Ƃ̐F�ɂȂ�
***************************************************************************/
	static inline  Color4 AdjustSaturation(const Color4& Col,float Saturation){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorAdjustSaturation(XMVECTOR(temp),Saturation);
	}
/**************************************************************************
 static inline  Color4 Modulate(
		const Color4& Col1,	//�J���[1
		const Color4& Col2	//�J���[2
 );
 �p�r: �Ή����鐬������Z����2�̃J���[���u�����h����
 �߂�l: �u�����h�����J���[
***************************************************************************/
	static inline  Color4 Modulate(const Color4& Col1,const Color4& Col2){
		return (Color4)XMColorModulate(XMVECTOR(Col1),XMVECTOR(Col2));
	}
/**************************************************************************
 static inline  Color4 Negative(
		const Color4& Col	//�J���[
 );
 �p�r: �J���[�̕��� RGB �J���[�l�����߂�
 �߂�l: ���� RGB �J���[�l
 ���J���[��0.0����1.0�͈̔͂ɒ�������A�v�Z�����
***************************************************************************/
	static inline  Color4 Negative(const Color4& Col){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorNegative(XMVECTOR(temp));
	}
/**************************************************************************
 static inline  Color4 GrayColor();
 �p�r: �O���[�̃J���[�𓾂�
 �߂�l: �O���[�̃J���[�l�i0.5,0.5,0.5,1.0�j
***************************************************************************/
	static inline  Color4 GrayColor(){
		return Color4(0.5f,0.5f,0.5f,1.0f);
	}
/**************************************************************************
 static inline  Color4 WhiteColor();
 �p�r: ���̃J���[�𓾂�
 �߂�l: ���̃J���[�l�i1.0,1.0,1.0,1.0�j
***************************************************************************/
	static inline  Color4 WhiteColor(){
		return Color4(1.0f,1.0f,1.0f,1.0f);
	}
};
//end Color4EX

namespace QuaternionEX{
	//��r
/**************************************************************************
 static inline  bool Equal(
	const Quaternion& Qt1,	//�N�I�[�^�j�I��1
	const Quaternion& Qt2	//�N�I�[�^�j�I��2
 );
 �p�r: 2�̃N�I�[�^�j�I�������������ǂ��������؂���
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Equal(const Quaternion& Qt1,const Quaternion& Qt2){
		return XMQuaternionEqual(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Quaternion& Qt1,	//�N�I�[�^�j�I��1
	const Quaternion& Qt2	//�N�I�[�^�j�I��2
 );
 �p�r: 2�̃N�I�[�^�j�I�����������Ȃ����ǂ��������؂���
 �߂�l: �������Ȃ����true
***************************************************************************/
	static inline  bool NotEqual(const Quaternion& Qt1,const Quaternion& Qt2){
		return XMQuaternionNotEqual(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
	//����
/**************************************************************************
 static inline  bool IsIdentity(
	const Quaternion& Qt	//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���� �P�ʃN�I�[�^�j�I��(0, 0, 0, 1)���ǂ��������؂���B
 �߂�l:  �P�ʃN�I�[�^�j�I���Ȃ��true
***************************************************************************/
	static inline  bool IsIdentity(const Quaternion& Qt){
		return XMQuaternionIsIdentity(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Quaternion& Qt	//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̒l�̂����ꂩ���������̖����傩�ǂ��������؂���B
 �߂�l: �l�̂����ꂩ���������̖�����Ȃ��true
***************************************************************************/
	static inline  bool IsInfinite(const Quaternion& Qt){
		return XMQuaternionIsInfinite(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Quaternion& Qt	//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̒l�̂����ꂩ��NaN���ǂ��������؂���B
 �߂�l: �l�̂����ꂩ��NaN�Ȃ��true
***************************************************************************/
	static inline  bool IsNaN(const Quaternion& Qt){
		return XMQuaternionIsNaN(XMVECTOR(Qt));
	}
	//����
/**************************************************************************
 static inline  Quaternion AddRotation(
	const Quaternion& Qt,	//���ƂɂȂ�N�I�[�^�j�I��
	const Vector3& other	//�ǉ���]�����]�s��
 );
 �p�r: ��]�s���ǉ���]����
 �߂�l: �Ȃ�
***************************************************************************/
	static inline  Quaternion AddRotation(const Quaternion& Qt,const Vector3& other){
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(other);
		return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt),XMVECTOR(Span));
	}

/**************************************************************************
 static inline  Vector3 GetRotation();
 �p�r: �N�I�[�^�j�I�������]�s��ɕϊ����ĕԂ�
 �߂�l: ��]�s��
***************************************************************************/
	static inline  Vector3 GetRotation(const Quaternion& Qt){
		Quaternion Temp = Qt;
		Temp.Normalize();
		Matrix4X4 mt;
		mt.RotationQuaternion(Temp);
		Vector3 Rot;
		if(mt._32 == 1.0f){
			Rot.x = XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21,mt._11);
		}
		else if(mt._32 == -1.0f){
			Rot.x = -XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21,mt._11);
		}
		else{
			Rot.x = -asin(mt._32);
			Rot.y = -atan2(-mt._31,mt._33);
			Rot.z = atan2(mt._12,mt._11);
		}
		return Rot;
	}
/**************************************************************************
 static inline  Quaternion BaryCentric(
	const Quaternion& Q0,	//�O�p�`�� 1 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q1,	//�O�p�`�� 2 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q2,	//�O�p�`�� 3 �ڂ̃N�I�[�^�j�I��
	float f,	//���d�W��
	float g		//���d�W��
 );
 �p�r: �w�肳�ꂽ�N�I�[�^�j�I�����g�p���āA�d�S���W�̃|�C���g��Ԃ�
 �߂�l: �d�S���W�̃N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion BaryCentric(const Quaternion& Q0,const Quaternion& Q1,const Quaternion& Q2,float f,float g){
		return (Quaternion)XMQuaternionBaryCentric(XMVECTOR(Q0),XMVECTOR(Q1),XMVECTOR(Q2),f,g);
	}
/**************************************************************************
 static inline  Quaternion BaryCentric(
	const Quaternion& Q0,	//�O�p�`�� 1 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q1,	//�O�p�`�� 2 �ڂ̃N�I�[�^�j�I��
	const Quaternion& Q2,	//�O�p�`�� 3 �ڂ̃N�I�[�^�j�I��
	const Vector4& f,	//���d�W��
	const Vector4& g		//���d�W��
 );
 �p�r: �w�肳�ꂽ�N�I�[�^�j�I���ƃx�N�g�����g�p���āA�d�S���W�̃|�C���g��Ԃ�
 �߂�l: �d�S���W�̃N�I�[�^�j�I��
 ���x�N�g��f��g�͂��ꂼ��v�f�������l�ɂ���
***************************************************************************/
	static inline  Quaternion BaryCentricV(const Quaternion& Q0,const Quaternion& Q1,const Quaternion& Q2,
		const Vector4& f,const Vector4& g){
		return (Quaternion)XMQuaternionBaryCentricV(XMVECTOR(Q0),XMVECTOR(Q1),XMVECTOR(Q2),XMVECTOR(f),XMVECTOR(g));
	}
/**************************************************************************
 static inline  Quaternion Conjugate(
	const Quaternion& Qt	//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̋�����Ԃ�
 �߂�l: �N�I�[�^�j�I���̋���
***************************************************************************/
	static inline  Quaternion Conjugate(const Quaternion& Qt){
		return (Quaternion)XMQuaternionConjugate(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float Dot(
	const Quaternion& Qt1,	//�N�I�[�^�j�I��1
	const Quaternion& Qt2		//�N�I�[�^�j�I��2
 );
 �p�r: 2�̃N�I�[�^�j�I���̓��ς�Ԃ�
 �߂�l: �N�I�[�^�j�I���̓���
***************************************************************************/
	static inline  float Dot(const Quaternion& Qt1,const Quaternion& Qt2){
		return ((Vector4)XMQuaternionDot(XMVECTOR(Qt1),XMVECTOR(Qt2))).x;
	}
/**************************************************************************
 static inline  Quaternion Exp(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̎w����Ԃ�
 �߂�l: �N�I�[�^�j�I���̎w��
 �����͂�w�v�f�͖��������
***************************************************************************/
	static inline  Quaternion Exp(const Quaternion& Qt){
		return (Quaternion)XMQuaternionExp(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion Identity();
 �p�r: �P�ʃN�I�[�^�j�I����Ԃ�
 �߂�l: �P�ʃN�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion Identity(){
		return (Quaternion)XMQuaternionIdentity();
	}
/**************************************************************************
 static inline  Quaternion Inverse(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̔��]��Ԃ�
 �߂�l: �N�I�[�^�j�I���̔��]
***************************************************************************/
	static inline  Quaternion Inverse(const Quaternion& Qt){
		return (Quaternion)XMQuaternionInverse(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float Length(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̒�����Ԃ�
 �߂�l: �N�I�[�^�j�I���̒���
***************************************************************************/
	static inline  float Length(const Quaternion& Qt){
		return ((Vector4)XMQuaternionLength(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  float LengthSq(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̒�����2���Ԃ�
 �߂�l: �N�I�[�^�j�I���̒�����2��
***************************************************************************/
	static inline  float LengthSq(const Quaternion& Qt){
		return ((Vector4)XMQuaternionLengthSq(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  Quaternion Ln(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̎��R�ΐ���Ԃ�
 �߂�l: �N�I�[�^�j�I���̎��R�ΐ�
 �����͂͐��K�����Ă���
***************************************************************************/
	static inline  Quaternion Ln(const Quaternion& Qt){
		return (Quaternion)XMQuaternionLn(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion Multiply(
	const Quaternion& Qt1,		//�N�I�[�^�j�I��1
	const Quaternion& Qt2			//�N�I�[�^�j�I��2
 );
 �p�r: 2�̃N�I�[�^�j�I������Z���ĕԂ�
 �߂�l: �N�I�[�^�j�I���̏�Z
***************************************************************************/
	static inline  Quaternion Multiply(const Quaternion& Qt1,const Quaternion& Qt2){
		return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
/**************************************************************************
 static inline  Quaternion Normalize(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���𐳋K�����ĕԂ�
 �߂�l: ���K�������N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion Normalize(const Quaternion& Qt){
		return (Quaternion)XMQuaternionNormalize(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion NormalizeEst(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���𐳋K���o�[�W�����̗\���l��Ԃ�
 �߂�l: ���K���o�[�W�����̗\���l
***************************************************************************/
	static inline  Quaternion NormalizeEst(const Quaternion& Qt){
		return (Quaternion)XMQuaternionNormalizeEst(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float ReciprocalLength(
	const Quaternion& Qt		//�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I���̒����̋t����Ԃ�
 �߂�l: �N�I�[�^�j�I���̒����̋t��
***************************************************************************/
	 static inline  float ReciprocalLength(const Quaternion& Qt){
		return ((Vector4)XMQuaternionReciprocalLength(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  Quaternion RotationAxis(
	const Vector3& Vec,		//��]����\���R�c�x�N�g��
	float Angle				//��]�p�x�i���v��胉�W�A���j
 );
 �p�r: ��]���Ɗp�x����N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion RotationAxis(const Vector3& Vec,float Angle){
		return (Quaternion)XMQuaternionRotationAxis(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 static inline  Quaternion RotationMatrix(
	const XMMATRIX& m				//��]�s��
 );
 �p�r: ��]�s�񂩂�N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion RotationMatrix(const XMMATRIX& m){
		return (Quaternion)XMQuaternionRotationMatrix(m);
	}
/**************************************************************************
 static inline  Quaternion RotationNormal(
	const Vector3& Vec,		//��]����\�����K�����ꂽ�R�c�x�N�g��
	float Angle				//��]�p�x�i���v��胉�W�A���j
 );
 �p�r: ���K�����ꂽ��]���Ɗp�x����N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion RotationNormal(const Vector3& Vec,float Angle){
		return (Quaternion)XMQuaternionRotationNormal(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 static inline  Quaternion RotationRollPitchYaw(
	float Pitch,		//X����]�i���W�A���j
	float Yaw,			//Y����]�i���W�A���j
	float Roll			//Z����]�i���W�A���j
 );
 �p�r: �e���̉�]����N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
 ����]��RollPitchYaw�iZ�AY�AX�j�̏��ōs����
***************************************************************************/
	static inline  Quaternion RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		return (Quaternion)XMQuaternionRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 static inline  Quaternion RotationRollPitchYawFromVector(
	const Vector3& Vec		//��]����\���R�c�x�N�g��
 );
 �p�r: 3D��]�x�N�g������N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
 ����]��RollPitchYaw�iZ�AY�AX�j�̏��ōs����
***************************************************************************/
	static inline  Quaternion RotationRollPitchYawFromVector(const Vector3& Vec){
		return (Quaternion)XMQuaternionRotationRollPitchYawFromVector(Vec);
	}
/**************************************************************************
 static inline  Quaternion Slerp(
	const Quaternion& Qt0,		//��Ԍ��i���K�����ꂽ�N�I�[�^�j�I���j
	const Quaternion& Qt1,		//��Ԑ�i���K�����ꂽ�N�I�[�^�j�I���j
	float t						//��ԌW���i0.0f����1.0f�̊Ԃ��w��j
 );
 �p�r: 2�̃N�I�[�^�j�I������W���ŕ�Ԃ��ꂽ�N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
 ���W����0�̏ꍇ��Q0�A1.0�Ȃ�Q1�Ɠ������̂�Ԃ�
***************************************************************************/
	static inline  Quaternion Slerp(const Quaternion& Qt0,const Quaternion& Qt1,float t){
		return (Quaternion)XMQuaternionSlerp(XMVECTOR(Qt0),XMVECTOR(Qt1),t);
	}
/**************************************************************************
 static inline  Quaternion SlerpV(
	const Quaternion& Qt0,		//��Ԍ��i���K�����ꂽ�N�I�[�^�j�I���j
	const Quaternion& Qt1,		//��Ԑ�i���K�����ꂽ�N�I�[�^�j�I���j
	const Vector4& Vec			//��ԌW���i���ׂĂ̗v�f��0.0f����1.0f�̊Ԃ̓����l���w��j
 );
 �p�r: 2�̃N�I�[�^�j�I������W���ŕ�Ԃ��ꂽ�N�I�[�^�j�I�����쐬����
 �߂�l: �쐬�����N�I�[�^�j�I��
 ���W����0�̏ꍇ��Q0�A1.0�Ȃ�Q1�Ɠ������̂�Ԃ�
***************************************************************************/
	static inline  Quaternion SlerpV(const Quaternion& Qt0,const Quaternion& Qt1,const Vector4& Vec){
		return (Quaternion)XMQuaternionSlerpV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Quaternion Squad(
	const Quaternion& Qt0,		//���K�����ꂽ�N�I�[�^�j�I���P
	const Quaternion& Qt1,		//���K�����ꂽ�N�I�[�^�j�I���Q
	const Quaternion& Qt2,		//���K�����ꂽ�N�I�[�^�j�I���R
	const Quaternion& Qt3,		//���K�����ꂽ�N�I�[�^�j�I���S
	float t						//��Ԑ���W���i0.0f����1.0f�̊Ԃ��w��j
 );
 �p�r: ���󕽕���Ԃ��g�p���āA4 �̒P�ʃN�I�[�^�j�I���Ԃ��Ԃ���
 �߂�l: �쐬�����N�I�[�^�j�I��
 ��Q1�AQ2�AQ3��QuaternionEX::SquadSetup�֐��ō쐬����
***************************************************************************/
	static inline  Quaternion Squad(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,float t){
		return (Quaternion)XMQuaternionSquad(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),t);
	}
/**************************************************************************
 static inline  void SquadSetup(
	Quaternion* pA,				//���ʂ��󂯂�N�I�[�^�j�I��A
	Quaternion* pB,				//���ʂ��󂯂�N�I�[�^�j�I��B
	Quaternion* pC,				//���ʂ��󂯂�N�I�[�^�j�I��C
	const Quaternion& Qt0,		//���K�����ꂽ�N�I�[�^�j�I���P
	const Quaternion& Qt1,		//���K�����ꂽ�N�I�[�^�j�I���Q
	const Quaternion& Qt2,		//���K�����ꂽ�N�I�[�^�j�I���R
	const Quaternion& Qt3		//���K�����ꂽ�N�I�[�^�j�I���S
 );
 �p�r: ���󕽕���Ԃ̂��߂̃Z�b�g�A�b�v���s��
 �߂�l: �Ȃ�
 ��pA�ApB�ApC�͂��ꂼ��Squad()�ASquadV()�֐���Qt1�AQt2�AQt3�ɓ��͂���
***************************************************************************/
	static inline  void SquadSetup(Quaternion* pA,Quaternion* pB,Quaternion* pC,
		const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3){
		XMVECTOR A(*pA);
		XMVECTOR B(*pB);
		XMVECTOR C(*pC);
		XMQuaternionSquadSetup(&A,&B,&C,
			XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3));
		*pA = A;
		*pB = B;
		*pC = C;
	}
/**************************************************************************
 static inline  Quaternion SquadV(
	const Quaternion& Qt0,		//���K�����ꂽ�N�I�[�^�j�I���P
	const Quaternion& Qt1,		//���K�����ꂽ�N�I�[�^�j�I���Q
	const Quaternion& Qt2,		//���K�����ꂽ�N�I�[�^�j�I���R
	const Quaternion& Qt3,		//���K�����ꂽ�N�I�[�^�j�I���S
	const Vector4& Vec			//��Ԑ���W���i���ׂĂ̗v�f��0.0f����1.0f�̊Ԃ̓����l���w��j
 );
 �p�r: ���󕽕���Ԃ��g�p���āA4 �̒P�ʃN�I�[�^�j�I���Ԃ��Ԃ���
 �߂�l: �쐬�����N�I�[�^�j�I��
 ��Q1�AQ2�AQ3��QuaternionEX::SquadSetup�֐��ō쐬����
***************************************************************************/
	static inline  Quaternion SquadV(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,const Vector4& Vec){
		return (Quaternion)XMQuaternionSquadV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  void ToAxisAngle(
	Vector3* pVec,			//��]���𓾂�R�c�x�N�g��
	float* pAngle,			//��]�p�x�𓾂�ϐ�
	const Quaternion& Qt	//�v�Z���ƂƂȂ�N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I������A��]���Ɖ�]�p�x�𓾂�
 �߂�l: �Ȃ�
 ��pVec�ɂ͉�]���x�N�g���ApAngle�ɂ͊p�x���ݒ肳���
***************************************************************************/
	static inline  void ToAxisAngle(Vector3* pVec,float* pAngle,const Quaternion& Qt){
		XMVECTOR Vec(*pVec);
		XMQuaternionToAxisAngle(&Vec,pAngle,XMVECTOR(Qt));
		*pVec = Vec;
	}


/**************************************************************************
	static inline  Quaternion Billboard(
	const Vector3& Line			//�v�Z���ƂȂ��
	)t;
	�p�r: Line�ɑ΂��ăr���{�[�h�p�x��ݒ肵���N�I�[�^�j�I���𓾂�
	�߂�l: �N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion Billboard(const Vector3& Line){
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
		Quaternion Qt;
		Qt = RotMatrix.QtInMatrix();
		Qt.Normalize();
		return Qt;
	}
/**************************************************************************
	void Faceing(
	const Vector3& Line			//�v�Z���ƂȂ��(�I�u�W�F�N�g����J�����ʒu������������)
	)t;
	�p�r: Line�ɑ΂��ăt�F�C�V���O�p�x��ݒ肵���N�I�[�^�j�I���𓾂�
	�߂�l: �N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion Faceing(const Vector3& Line){
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
		Quaternion Qt;
		Qt = RotMatrix.QtInMatrix();
		Qt.Normalize();
		return Qt;
	}

/**************************************************************************
	void FaceingY(
	const Vector3& Line			//�v�Z���ƂȂ��(�I�u�W�F�N�g����J�����ʒu������������)
	)t;
	�p�r: Line�ɑ΂���Y���t�F�C�V���O�p�x��ݒ肵���N�I�[�^�j�I���𓾂�
	�߂�l: �N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion FaceingY(const Vector3& Line){
		Vector3 Temp = Line;
		Temp.Normalize();
		Quaternion Qt;
		Qt.Identity();
		Qt.RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
		Qt.Normalize();
		return Qt;
	}



};
//end QuaternionEX


namespace Matrix4X4EX{
/**************************************************************************
 static inline  Vector3 PosInMatrixSt(
		const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s����̈ʒu�����擾����(���ڍs�񂩂���o��)
 �߂�l: �ʒu�x�N�g��
***************************************************************************/
	static inline  Vector3 PosInMatrixSt(const Matrix4X4& m){
		Vector3 Vec;
		Vec.x = m._41;
		Vec.y = m._42;
		Vec.z = m._43;
		return Vec;
	}
/**************************************************************************
 static inline  Vector3 PosInMatrix(
		const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s����̈ʒu�����擾����
 �߂�l: �ʒu�x�N�g��
***************************************************************************/
	static inline  Vector3 PosInMatrix(const Matrix4X4& m){
		Vector3 Vec(0,0,0);
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(m))){
			Vec = Translation;
		}
		return Vec;
	}
/**************************************************************************
 static inline  Vector3 ScaleInMatrix(
		const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s����̃X�P�[�����O���擾����
 �߂�l: �X�P�[�����O�x�N�g��
***************************************************************************/
	static inline  Vector3 ScaleInMatrix(const Matrix4X4& m){
		Vector3 Vec(1.0f,1.0f,1.0f);
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(m))){
			Vec = Scale;
		}
		return Vec;
	}
/**************************************************************************
 static inline  Quaternion QtInMatrix(
		const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s����̉�]���擾����
 �߂�l: ��]�N�I�[�^�j�I��
***************************************************************************/
	static inline  Quaternion QtInMatrix(const Matrix4X4& m){
		Quaternion RetQt;
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(m))){
			RetQt = Qt;
		}
		return RetQt;
	}
/**************************************************************************
 static inline  Vector3 RotXInMatrix();
 �p�r: �s�����X����]���擾����
 �߂�l: X���̉�]�x�N�g��
***************************************************************************/
	static inline  Vector3 RotXInMatrix(const Matrix4X4& m){
		Vector3 ret(m._11,m._12,m._13);
		Vector3 Scale = ScaleInMatrix(m);
		Matrix4X4::GetSafeScale(Scale);
		ret.x /= Scale.x;
		ret.y /= Scale.x;
		ret.z /= Scale.x;
		return ret;
	}
/**************************************************************************
 static inline  Vector3 RotYInMatrix();
 �p�r: �s�����Y����]���擾����
 �߂�l: Y���̉�]�x�N�g��
***************************************************************************/
	static inline  Vector3 RotYInMatrix(const Matrix4X4& m){
		Vector3 ret(m._21,m._22,m._23);
		Vector3 Scale = ScaleInMatrix(m);
		Matrix4X4::GetSafeScale(Scale);
		ret.x /= Scale.y;
		ret.y /= Scale.y;
		ret.z /= Scale.y;
		return ret;
	}
/**************************************************************************
 static inline  Vector3 RotZInMatrix();
 �p�r: �s�����Z����]���擾����
 �߂�l: Z���̉�]�x�N�g��
***************************************************************************/
	static inline  Vector3 RotZInMatrix(const Matrix4X4& m){
		Vector3 ret(m._31,m._32,m._33);
		Vector3 Scale = ScaleInMatrix(m);
		Matrix4X4::GetSafeScale(Scale);
		ret.x /= Scale.z;
		ret.y /= Scale.z;
		ret.z /= Scale.z;
		return ret;
	}
/**************************************************************************
 static inline  Matrix4X4 AffineTransformation(
		const Vector3& Scaling,					//�X�P�[�����O
		const Vector3& RotOrigin,				//��]�̒��S
		const Quaternion& RotationQuaternion,	//�N�I�[�^�j�I��
		const Vector3& Translation				//���s�ړ�
 );
 �p�r: �X�P�[�����O�A��]�̒��S���W�A�N�I�[�^�j�I���A���s�ړ�����s����쐬����
 �߂�l: �쐬�����s��
***************************************************************************/
	static inline  Matrix4X4 AffineTransformation(const Vector3& Scaling,const Vector3& RotOrigin,
			const Quaternion& RotationQuaternion,const Vector3& Translation){
		return (Matrix4X4)XMMatrixAffineTransformation(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			XMVECTOR(RotationQuaternion),XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  Matrix4X4 AffineTransformation2D(
		const Vector2& Scaling,					//�X�P�[�����O
		const Vector2& RotOrigin,				//��]�̒��S
		float Rotation,							//��]
		const Vector2& Translation				//���s�ړ�
 );
 �p�r: �X�P�[�����O�A��]�̒��S���W�A��]�l�i���W�A���j�A���s�ړ�����2D�s����쐬����
 �߂�l: �쐬�����s��
***************************************************************************/
	static inline  Matrix4X4 AffineTransformation2D(const Vector2& Scaling,
		const Vector2& RotOrigin,float Rotation,const Vector2& Translation){
		return (Matrix4X4)XMMatrixAffineTransformation2D(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  bool Decompose(
		Vector3* pScaling,					//�X�P�[�����O
		Quaternion* pQt,					//�N�I�[�^�j�I��
		Vector3* pTranslation,				//���s�ړ�
		const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s�񂩂�X�P�[�����O�A��]�A���s�ړ������o��
 �߂�l: ���������true
***************************************************************************/
	static inline  bool Decompose(Vector3* pScaling,Quaternion* pQt,
		Vector3* pTranslation,const Matrix4X4& m){
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if(XMMatrixDecompose(&Scale,&Qt,&Translation,XMMATRIX(m))){
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
 static inline  Vector4 Determinant(
	const Matrix4X4& m						//���ɂȂ�s��
 );
 �p�r: �s�񎮂�Ԃ�
 �߂�l: �쐬�����s�񎮁iVector4�̊e�v�f�ɓ���j
***************************************************************************/
	static inline  Vector4 Determinant(const Matrix4X4& m){
		return (Vector4)XMMatrixDeterminant(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 Identity();
 �p�r: �P�ʍs���Ԃ�
 �߂�l: �P�ʍs��
***************************************************************************/
	static inline  Matrix4X4 Identity(){
		return (Matrix4X4)XMMatrixIdentity();
	}
/**************************************************************************
 static inline  Matrix4X4 Inverse(
	Vector4* pVec,					//�s�񎮂��󂯎��x�N�g��
	const Matrix4X4& m					//���ɂȂ�s��
 );
 �p�r:�t�s���Ԃ�
 �߂�l: �P�ʍs��
 ��pVec�ɂ͍s�񎮂�����
***************************************************************************/
	static inline  Matrix4X4 Inverse(Vector4* pVec,const Matrix4X4& m){
		XMVECTOR Vec(*pVec);
		Matrix4X4 ret = (Matrix4X4)XMMatrixInverse(&Vec,XMMATRIX(m));
		*pVec = Vec;
		return ret;
	}
/**************************************************************************
 static inline  bool IsIdentity(
	const Matrix4X4& m					//���ɂȂ�s��
 );
 �p�r: �P�ʍs�񂩂ǂ��������؂���
 �߂�l: �P�ʍs��Ȃ�true
***************************************************************************/
	static inline  bool IsIdentity(const Matrix4X4& m){
		return XMMatrixIsIdentity(XMMATRIX(m));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Matrix4X4& m					//���ɂȂ�s��
 );
 �p�r: �s��̂����ꂩ�̗v�f�����܂��͕��̖����傩�ǂ��������؂���
 �߂�l: ���܂��͕��̖�����Ȃ�true
***************************************************************************/
	static inline  bool IsInfinite(const Matrix4X4& m){
		return XMMatrixIsInfinite(XMMATRIX(m));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Matrix4X4& m					//���ɂȂ�s��
 );
 �p�r: �s��̂����ꂩ�̗v�f��NaN���ǂ��������؂���
 �߂�l: NaN�Ȃ�true
***************************************************************************/
	static inline  bool IsNaN(const Matrix4X4& m){
		return XMMatrixIsNaN(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 LookAtLH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& At,					//�J�������_
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:������W�r���[�s����쐬����
 �߂�l: ������W�r���[�s��
***************************************************************************/
	static inline  Matrix4X4 LookAtLH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookAtLH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookAtRH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& At,					//�J�������_
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�E����W�r���[�s����쐬����
 �߂�l: �E����W�r���[�s��
***************************************************************************/
	static inline  Matrix4X4 LookAtRH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookAtRH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookToLH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& EyeDirection,		//�J��������
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�J�����ʒu�A�����A�X�����獶����W�r���[�s����쐬����
 �߂�l: ������W�r���[�s��
***************************************************************************/
	static inline  Matrix4X4 LookToLH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookToLH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookToRH(
	const Vector3& Eye,					//�J�����ʒu
	const Vector3& EyeDirection,		//�J��������
	const Vector3& Up					//�J�����X���i�ʏ�<0,1,0>�j
 );
 �p�r:�J�����ʒu�A�����A�X������E����W�r���[�s����쐬����
 �߂�l: �E����W�r���[�s��
***************************************************************************/
	static inline  Matrix4X4 LookToRH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookToRH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 Multiply(
	const Matrix4X4& m1,					//���Z���ɂȂ�s��
	const Matrix4X4& m2						//���Z��ɂȂ�s��
 );
 �p�r:�s��̐ς�Ԃ�
 �߂�l: �s��̐�
***************************************************************************/
	static inline  Matrix4X4 Multiply(const Matrix4X4& m1,const Matrix4X4& m2){
		return  (Matrix4X4)XMMatrixMultiply(XMMATRIX(m1),XMMATRIX(m2));
	}
/**************************************************************************
 static inline  Matrix4X4 MultiplyTranspose(
	const Matrix4X4& m1,					//���Z���ɂȂ�s��
	const Matrix4X4& m2						//���Z��ɂȂ�s��
 );
 �p�r:�ς̓]�u�s���Ԃ�
 �߂�l: �ς̓]�u�s��
***************************************************************************/
	static inline  Matrix4X4 MultiplyTranspose(const Matrix4X4& m1,const Matrix4X4& m2){
		return  (Matrix4X4)XMMatrixMultiplyTranspose(XMMATRIX(m1),XMMATRIX(m2));
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicLH(
	float ViewWidth,		//��
	float ViewHeight,		//����
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:������W�n�̐��ˉe�s���Ԃ�
 �߂�l: ������W�n�̐��ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 OrthographicLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicRH(
	float ViewWidth,		//��
	float ViewHeight,		//����
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:�E����W�n�̐��ˉe�s���Ԃ�
 �߂�l: �E����W�n�̐��ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 OrthographicRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicOffCenterLH(
	float ViewLeft,			//��
	float ViewRight,		//�E
	float ViewBottom,		//��
	float ViewTop,			//��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:������W�n�̃J�X�^�����ˉe�s���Ԃ�
 �߂�l: ������W�n�̃J�X�^�����ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 OrthographicOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicOffCenterLH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicOffCenterRH(
	float ViewLeft,			//��
	float ViewRight,		//�E
	float ViewBottom,		//��
	float ViewTop,			//��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r:�E����W�n�̃J�X�^�����ˉe�s���Ԃ�
 �߂�l: �E����W�n�̃J�X�^�����ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 OrthographicOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicOffCenterRH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveFovLH(
	float FovAngleY,		//�g�b�v�_�E������p�x
	float AspectHByW,		//�r���[��Ԃ̍����ƕ��̃A�X�y�N�g��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ����Ɋ�Â��āA������W�n�̃p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: ������W�n�̃p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveFovLH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveFovLH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveFovRH(
	float FovAngleY,		//�g�b�v�_�E������p�x
	float AspectHByW,		//�r���[��Ԃ̍����ƕ��̃A�X�y�N�g��
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ����Ɋ�Â��āA�E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: �E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveFovRH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveFovRH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveLH(
	float ViewWidth,		//�߂��̃N���b�v�ʂ̎�����̕�
	float ViewHeight,		//�߂��̃N���b�v�ʂ̎�����̍���
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: ������W�n�̃p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: ������W�n�̃p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveRH(
	float ViewWidth,		//�߂��̃N���b�v�ʂ̎�����̕�
	float ViewHeight,		//�߂��̃N���b�v�ʂ̎�����̍���
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: �E����W�n�̃p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveOffCenterLH(
	float ViewLeft,			//�N���b�v������̍����� x ���W
	float ViewRight,		//�N���b�v������̉E���� x ���W
	float ViewBottom,		//�N���b�v������̉����� y ���W
	float ViewTop,			//�N���b�v������̏㑤�� y ���W
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �J�X�^�� �o�[�W�����̍�����W�n�p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: �J�X�^�� �o�[�W�����̍�����W�n�p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveOffCenterLH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveOffCenterRH(
	float ViewLeft,			//�N���b�v������̍����� x ���W
	float ViewRight,		//�N���b�v������̉E���� x ���W
	float ViewBottom,		//�N���b�v������̉����� y ���W
	float ViewTop,			//�N���b�v������̏㑤�� y ���W
	float NearZ,			//���s��O
	float FarZ				//���s��
 );
 �p�r: �J�X�^�� �o�[�W�����̉E����W�n�p�[�X�y�N�e�B�u�ˉe�s���Ԃ�
 �߂�l: �J�X�^�� �o�[�W�����̉E����W�n�p�[�X�y�N�e�B�u�ˉe�s��
***************************************************************************/
	static inline  Matrix4X4 PerspectiveOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveOffCenterRH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 Reflect(
	const Plane& Pln		//����
 );
 �p�r: �w�肳�ꂽ���ʂ���ăx�N�g���𔽉f����g�����X�t�H�[���s����쐬���Ԃ�
 �߂�l: �x�N�g���𔽉f����g�����X�t�H�[���s��
***************************************************************************/
	static inline  Matrix4X4 Reflect(const Plane& Pln){
		return  (Matrix4X4)XMMatrixReflect(XMVECTOR(Pln));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationAxis(
	const Vector3& Axis,		//��]����\���x�N�g��
	float Angle				//��]�p�x�i���W�A���j
 );
 �p�r: �C�ӂ̎�����]���ɂ��āA��]�����s����쐬���Ԃ�
 �߂�l: �C�ӂ̎�����]���ɂ��āA��]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationAxis(const Vector3& Axis,float Angle){
		return  (Matrix4X4)XMMatrixRotationAxis(XMVECTOR(Axis),Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationNormal(
	const Vector3& NormalAxis,		//��]�����L�q����@���x�N�g��
	float Angle						//��]�p�x�i���W�A���j
 );
 �p�r: �C�ӂ̖@���x�N�g������]���ɂ��āA��]�����s����쐬���Ԃ�
 �߂�l: �C�ӂ̖@���x�N�g������]���ɂ��āA��]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationNormal(const Vector3& NormalAxis,float Angle){
		return  (Matrix4X4)XMMatrixRotationNormal(XMVECTOR(NormalAxis),Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationQuaternion(
	const Quaternion& Qt		//��]���L�q����N�I�[�^�j�I��
 );
 �p�r: �N�I�[�^�j�I�������]�s����쐬���Ԃ�
 �߂�l: �N�I�[�^�j�I�������]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationQuaternion(const Quaternion& Qt){
		return  (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationRollPitchYaw(
	float Pitch,		//X����]
	float Yaw,			//Y����]
	float Roll			//Z����]
 );
 �p�r: �I�C���[�p�Ɋ�Â��ĉ�]�s����쐬����
 �߂�l: �I�C���[�p�Ɋ�Â��ĉ�]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		return  (Matrix4X4)XMMatrixRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationRollPitchYawFromVector(
	const Vector3& Angles		//��]���L�q����x�N�g��
 );
 �p�r: �I�C���[�p(�x�N�g��)�Ɋ�Â��ĉ�]�s����쐬����
 �߂�l: �I�C���[�p(�x�N�g��)�Ɋ�Â��ĉ�]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationRollPitchYawFromVector(const Vector3& Angles){
		return  (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Angles));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationX(
	float Angle		//X����]
 );
 �p�r: X����]�Ɋ�Â��ĉ�]�s����쐬����
 �߂�l: X����]�Ɋ�Â��ĉ�]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationX(float Angle){
		return  (Matrix4X4)XMMatrixRotationX(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationY(
	float Angle		//Y����]
 );
 �p�r: Y����]�Ɋ�Â��ĉ�]�s����쐬����
 �߂�l: Y����]�Ɋ�Â��ĉ�]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationY(float Angle){
		return  (Matrix4X4)XMMatrixRotationY(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationZ(
	float Angle		//Z����]
 );
 �p�r: Z����]�Ɋ�Â��ĉ�]�s����쐬����
 �߂�l: Z����]�Ɋ�Â��ĉ�]�����s��
***************************************************************************/
	static inline  Matrix4X4 RotationZ(float Angle){
		return  (Matrix4X4)XMMatrixRotationZ(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 Scaling(
	float ScaleX,	//X�ɉ������X�P�[�����O
	float ScaleY,	//Y�ɉ������X�P�[�����O
	float ScaleZ	//Z�ɉ������X�P�[�����O
 );
 �p�r: x ���Ay ���A����� z ���ɉ����ăX�P�[�����O�����s����쐬����
 �߂�l: x ���Ay ���A����� z ���ɉ����ăX�P�[�����O�����s��
***************************************************************************/
	static inline  Matrix4X4 Scaling(float ScaleX,float ScaleY,float ScaleZ){
		return  (Matrix4X4)XMMatrixScaling(ScaleX,ScaleY,ScaleZ);
	}
/**************************************************************************
 static inline  Matrix4X4 ScalingFromVector(
	const Vector3& Scale		//�X�P�[�����O���L�q����x�N�g��
 );
 �p�r: �x�N�g���ɉ����ăX�P�[�����O�����s����쐬����
 �߂�l: �x�N�g���ɉ����ăX�P�[�����O�����s��
***************************************************************************/
	static inline  Matrix4X4 ScalingFromVector(const Vector3& Scale){
		return  (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scale));
	}
/**************************************************************************
 static inline  Matrix4X4 Set(
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
 �p�r: �e�l��ݒ肵���s����쐬����
 �߂�l: �e�l��ݒ肵���s��
***************************************************************************/
	static inline  Matrix4X4 Set(
		float m00,float m01,float m02,float m03,
		float m10,float m11,float m12,float m13,
		float m20,float m21,float m22,float m23,
		float m30,float m31,float m32,float m33
		){
		return  (Matrix4X4)XMMatrixSet(
							m00,m01,m02,m03,
							m10,m11,m12,m13,
							m20,m21,m22,m23,
							m30,m31,m32,m33
							);
	}
/**************************************************************************
 static inline  Matrix4X4 Shadow(
	const Plane& ShadowPlane,			//���
	const Vector4& LightPosition		//���C�g�̈ʒu
 );
 �p�r: �W�I���g���𕽖ʂɎˉe����g�����X�t�H�[���s����쐬����
 �߂�l: �W�I���g���𕽖ʂɎˉe����g�����X�t�H�[���s��
 �����C�g��w�v�f�́A0.0�Ńf�B���N�V���i�����C�g�A1.0�Ń|�C���g���C�g�ƂȂ�
***************************************************************************/
	static inline  Matrix4X4 Shadow(const Plane& ShadowPlane,const Vector4& LightPosition){
		return  (Matrix4X4)XMMatrixShadow(XMVECTOR(ShadowPlane),XMVECTOR(LightPosition));
	}
/**************************************************************************
 static inline  Matrix4X4 Transformation(
 	const Vector3& ScalingOrigin,		//�X�P�[�����O�̒��S���L�q���� 3D �x�N�g��
 	const Quaternion& ScalingOrientationQuaternion,		//�X�P�[�����O�̌������L�q����N�I�[�^�j�I��
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Vector3& RotationOrigin,		//��]�̒��S���L�q���� 3D �x�N�g��
 	const Quaternion& RotationQuaternion,		//RotationOrigin �ɂ���Ď�����錴�_�𒆐S�Ƃ����]���L�q����N�I�[�^�j�I��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �g�����X�t�H�[���s����쐬����
 �߂�l: �g�����X�t�H�[���s��
***************************************************************************/
	static inline  Matrix4X4 Transformation(const Vector3& ScalingOrigin,const Quaternion& ScalingOrientationQuaternion,const Vector3& Scaling,
 		const Vector3& RotationOrigin,const Quaternion& RotationQuaternion,
		const Vector3& Translation){
		return  (Matrix4X4)XMMatrixTransformation(
			XMVECTOR(ScalingOrigin),XMVECTOR(ScalingOrientationQuaternion),XMVECTOR(Scaling),
			XMVECTOR(RotationOrigin),XMVECTOR(RotationQuaternion),
			XMVECTOR(Translation)
			);
	}
/**************************************************************************
 static inline  Matrix4X4 Transformation2D(
 	const Vector2& ScalingOrigin,		//�X�P�[�����O�̒��S���L�q���� 2D �x�N�g��
 	float ScalingOrientation,			//�X�P�[�����O��]�W��
	const Vector2& Scaling,				//x ������� y ���̃X�P�[�����O�W�����܂� 2D �x�N�g��
 	const Vector2& RotationOrigin,		//��]�̒��S���L�q���� 2D �x�N�g��
 	float Rotation,						//��]�̊p�x (���W�A���P��)
	const Vector2& Translation			//���s�ړ����L�q���� 2D �x�N�g��
 );
 �p�r: 2D�g�����X�t�H�[���s���xy���ʂɍ쐬����
 �߂�l: 2D�g�����X�t�H�[���s��
***************************************************************************/
	static inline  Matrix4X4 Transformation2D(const Vector2& ScalingOrigin,float ScalingOrientation,const Vector2& Scaling,
 	const Vector2& RotationOrigin,float Rotation,
	const Vector2& Translation){
		return  (Matrix4X4)XMMatrixTransformation2D(XMVECTOR(ScalingOrigin),ScalingOrientation,XMVECTOR(Scaling),
			XMVECTOR(RotationOrigin),Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  Matrix4X4 Translation(
	float OffsetX,	//X���ɉ��������s�ړ�
	float OffsetY,	//Y���ɉ��������s�ړ�
	float OffsetZ	//Z���ɉ��������s�ړ�
 );
 �p�r: �w�肳�ꂽ�I�t�Z�b�g���畽�s�ړ��s����쐬����
 �߂�l: �w�肳�ꂽ�I�t�Z�b�g�ŕ��s�ړ������s��
***************************************************************************/
	static inline  Matrix4X4 Translation(float OffsetX,float OffsetY,float OffsetZ){
		return  (Matrix4X4)XMMatrixTranslation(OffsetX,OffsetY,OffsetZ);
	}
/**************************************************************************
 static inline  Matrix4X4 TranslationFromVector(
	const Vector3& Offset				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �x�N�g�����畽�s�ړ��s����쐬����
 �߂�l: �x�N�g�����畽�s�ړ������s��
***************************************************************************/
	static inline  Matrix4X4 TranslationFromVector(const Vector3& Offset){
		return  (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Offset));
	}
/**************************************************************************
 static inline  Matrix4X4 Transpose(
	const Matrix4X4& m				//�]�u����s��
 );
 �p�r: �s���]�u����
 �߂�l: �]�u�����s��
***************************************************************************/
	static inline  Matrix4X4 Transpose(const Matrix4X4& m){
		return  (Matrix4X4)XMMatrixTranspose(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 DefTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Quaternion& RotationQt,		//��]���L�q����N�I�[�^�j�I��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �X�P�[�����O�A��]�A�ړ�����s����쐬����
 �߂�l: �쐬�����s��
***************************************************************************/
	static inline  Matrix4X4 DefTransformation(const Vector3& Scaling,const Quaternion& RotationQt,const Vector3& Translation){
		Matrix4X4 Scale = ScalingFromVector(Scaling);
		Matrix4X4 Qt = RotationQuaternion(RotationQt);
		Matrix4X4 Trans = TranslationFromVector(Translation);
		Matrix4X4 ret = Scale * Qt * Trans; 
		return  ret;
	}
/**************************************************************************
 static inline  Matrix4X4 DefTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
 	const Vector3& Rotation,			//��]���L�q����x�N�g��
	const Vector3& Translation				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 );
 �p�r: �X�P�[�����O�A��]�A�ړ�����s����쐬����
 �߂�l: �쐬�����s��
***************************************************************************/
	static inline  Matrix4X4 DefTransformation(const Vector3& Scaling,const Vector3& Rotation,const Vector3& Translation){
		Matrix4X4 Scale = ScalingFromVector(Scaling);
		Matrix4X4 Rot = RotationRollPitchYawFromVector(Rotation);
		Matrix4X4 Trans = TranslationFromVector(Translation);
		Matrix4X4 ret = Scale * Rot * Trans; 
		return  ret;
	}
/**************************************************************************
 static inline  Matrix4X4 STRTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
	const Vector3& Translation,				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 	const Quaternion& RotationQuaternion		//��]���L�q����N�I�[�^�j�I��
 );
 �p�r: �X�P�[�����O�A�ړ��A��]����s����쐬����
 �߂�l: �쐬�����s��
 ����]���ړ����ɍs��
***************************************************************************/
	static inline  Matrix4X4 STRTransformation(const Vector3& Scaling,const Vector3& Translation,const Quaternion& RotationQt){
		Matrix4X4 Scale = ScalingFromVector(Scaling);
		Matrix4X4 Trans = TranslationFromVector(Translation);
		Matrix4X4 Qt = RotationQuaternion(RotationQt);
		Matrix4X4 ret = Scale * Trans * Qt ; 
		return  ret;
	}
/**************************************************************************
 static inline  Matrix4X4 STRTransformation(
	const Vector3& Scaling,				//x ���Ay ���Az ���̃X�P�[�����O�W�����܂� 3D �x�N�g��
	const Vector3& Translation,				//x ���Ay ���Az ���ɉ��������s�ړ����L�q���� 3D �x�N�g��
 	const Vector3& Rotation			//��]���L�q����x�N�g��
 );
 �p�r: �X�P�[�����O�A�ړ��A��]����s����쐬����
 �߂�l: �쐬�����s��
 ����]���ړ����ɍs��
***************************************************************************/
	static inline  Matrix4X4 STRTransformation(const Vector3& Scaling,const Vector3& Translation,const Vector3& Rotation){
		Matrix4X4 Scale = ScalingFromVector(Scaling);
		Matrix4X4 Trans = TranslationFromVector(Translation);
		Matrix4X4 Rot = RotationRollPitchYawFromVector(Rotation);
		Matrix4X4 ret = Scale * Trans * Rot;
		return  ret;
	}


};
//end Matrix4X4EX




}
//end of namespace basedx11.

