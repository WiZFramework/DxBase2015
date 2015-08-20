
/******************************************************************************
 MathExtEX.h

 行列、クオータニオン、カラーなどのスタティック計算
 XNAMATH のラッピングスタティック関数群

 Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
 DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "StdAfx.h"

namespace basedx11{

namespace PlaneEX{
/**************************************************************************
 static inline  float Dot(
	const Plane& plane,		//計算もとの平面
	const Vector4& other	//計算するVector4
 );
 用途: planeとotherの内積を返す
 戻り値: 内積
***************************************************************************/
	static inline  float Dot(const Plane& plane,const Vector4& other){
		return ((Vector4)XMPlaneDot(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  float DotCoord(
	const Plane& plane,		//計算もとの平面
	const Vector3& other	//相手のVector3
 );
 用途: planeとVector3の内積を計算し返す
 戻り値: 内積
 ＊あるポイントから平面までの符号付き距離を求めるのに便利である
***************************************************************************/
	static inline  float DotCoord(const Plane& plane,const Vector3& other){
		return  ((Vector4)XMPlaneDotCoord(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  float DotNormal(
	const Plane& plane,		//計算もとの平面
	const Vector3& other	//相手のVector3
 );
 用途:　planeの法線ベクトルと 3D ベクトルの間の内積を計算し返す
 戻り値: 内積
 ＊平面の法線ベクトルと別の法線ベクトルとの角度を算出するときに役立つ
***************************************************************************/
	static inline  float DotNormal(const Plane& plane,const Vector3& other){
		return ((Vector4)XMPlaneDotNormal(XMVECTOR(plane),XMVECTOR(other))).x;
	}
/**************************************************************************
 static inline  bool Equal(
	const Plane& plane,		//計算もとの平面
	const Plane& other	//相手のPlane
 );
 用途:　planeと相手の平面が同じかどうかを調べる
 戻り値: 同じならtrue
***************************************************************************/
	static inline  bool Equal(const Plane& plane,const Plane& other){
		return XMPlaneEqual(XMVECTOR(plane),XMVECTOR(other));
	}
/**************************************************************************
 static inline  Plane FromPointNormal(
	const Vector3& Point,	//平面上の点
	const Vector3& Normal	//法線
 );
 用途:　点と法線から平面を作成して、返す
 戻り値: 作成したPlane
***************************************************************************/
	static inline  Plane FromPointNormal(const Vector3& Point,const Vector3& Normal){
		return (Plane)XMPlaneFromPointNormal(XMVECTOR(Point),XMVECTOR(Normal));
	}
/**************************************************************************
 static inline  Plane FromPoints(
	const Vector3& Point1,	//頂点1
	const Vector3& Point2,	//頂点2
	const Vector3& Point3	//頂点3
 );
 用途:　３点から平面を作成して、返す
 戻り値: 作成したPlane
***************************************************************************/
	static inline  Plane FromPoints(const Vector3& Point1,const Vector3& Point2,const Vector3& Point3){
		return (Plane)XMPlaneFromPoints(XMVECTOR(Point1),XMVECTOR(Point2),XMVECTOR(Point3));
	}
/**************************************************************************
 static inline  Vector3 IntersectLine(
	const Plane& plane,		//計算もとの平面
	const Vector3& LinePoint1,	//頂点1
	const Vector3& LinePoint2	//頂点2
 );
 用途:　planeと２つの頂点が作成する直線の交差する頂点を返す
 戻り値: planeと交差する３Ｄベクトル
 ＊平面と直線が平行の場合は、戻る各値はQNaN（非数） になるので、
 　Vector3::IsNaN()などでチェックする
***************************************************************************/
	static inline  Vector3 IntersectLine(const Plane& plane,
		const Vector3& LinePoint1,const Vector3& LinePoint2){
		return (Vector3)XMPlaneIntersectLine(XMVECTOR(plane),
			XMVECTOR(LinePoint1),XMVECTOR(LinePoint2));
	}
/**************************************************************************
 static inline  void IntersectPlane(
	Vector3& RetLinePoint1,	//返される頂点1
	Vector3& RetLinePoint2,	//返される頂点2
	const Plane& plane,		//計算もとの平面
	const Plane& other		//planeと比べられるもう一つの面
 );
 用途:　planeともう一つの面が交差する2つの頂点を計算する
 戻り値: なし
 ＊RetLinePoint1とRetLinePoint2に交差する頂点を返す
 ＊2つの平面が平行の場合は、戻る各値はQNaN（非数） になるので、
 　Vector3::IsNaN()などでチェックする
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
	const Plane& plane		//計算もとの平面
 );
 用途:　planeの要素のいずれかが正または負の無限大かどうかを調べる
 戻り値: 正または負の無限大ならtrue
***************************************************************************/
	static inline  bool IsInfinite(const Plane& plane){
		return XMPlaneIsInfinite(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Plane& plane		//計算もとの平面
 );
 用途:　Planeの要素のいずれかが NaNかどうかを調べる
 戻り値: NaNならtrue
 ＊thisは変更しない
***************************************************************************/
	static inline  bool IsNaN(const Plane& plane){
		return XMPlaneIsNaN(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool NearEqual(
	const Plane& plane,		//計算もとの平面
	const Plane& other		//planeと比べられるもう一つの面
	const Vector4& Epsilon	//各要素の許容範囲
 );
 用途:　planeともう一つの平面がほぼ等しいかどうか調べる
 戻り値: ほぼ等しければtrue
***************************************************************************/
	static inline  bool NearEqual(const Plane& plane,const Plane& other,const Vector4& Epsilon){
		return XMPlaneNearEqual(XMVECTOR(plane),XMVECTOR(other),Epsilon);
	}
/**************************************************************************
 static inline  Plane Normalize(
	const Plane& plane		//計算もとの平面
 );
 用途:planeを正規化する
 戻り値:正規化した平面
***************************************************************************/
	static inline  Plane Normalize(const Plane& plane){
		return (Plane)XMPlaneNormalize(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  Plane NormalizeEst(
	const Plane& plane		//計算もとの平面
 );
 用途:planeをx,y,zの係数が単位の法線ベクトルを形成するように、平面の係数を予測
 戻り値: 予測した平面
***************************************************************************/
	static inline  Plane NormalizeEst(const Plane& plane){
		return (Plane)XMPlaneNormalizeEst(XMVECTOR(plane));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Plane& plane,		//計算もとの平面
	const Plane& other	//相手のPlane
 );
 用途:　planeと相手の平面が等しくないかどうかを調べる
 戻り値: 等しくないならtrue
***************************************************************************/
	static inline  bool NotEqual(const Plane& plane,const Plane& other){
		return XMPlaneNotEqual(XMVECTOR(plane),XMVECTOR(other));
	}
/**************************************************************************
 static inline  Plane Transform(
	const Plane& plane,		//計算もとの平面
	const XMMATRIX& m	//トランスフォームする行列
 );
 用途:planeをmでトランスフォームする
 戻り値: トランスフォームした平面
***************************************************************************/
	static inline  Plane Transform(const Plane& plane,const XMMATRIX& m){
		return (Plane)XMPlaneTransform(XMVECTOR(plane),m);
	}
//　XMPlaneTransformStream関数は、ラッピング関数は用意しない
//　理由：パラメータに配列を渡す必要があり、ラッピングによって、最適化が損なわれる
};

namespace Color4EX{
	//比較
/**************************************************************************
 static inline  bool Equal(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラーと等しいかどうかを検証する
 戻り値: 等しければtrue
***************************************************************************/
	static inline  bool Equal(const Color4& Col,const Color4& other){
		return XMColorEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラーと等しくないかどうかを検証する
 戻り値: 等しくなければtrue
***************************************************************************/
	static inline  bool NotEqual(const Color4& Col,const Color4& other){
		return XMColorNotEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Greater(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラーより大きいかどうかを検証する
 戻り値: 大きければtrue
***************************************************************************/
	static inline  bool Greater(const Color4& Col,const Color4& other){
		return XMColorGreater(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool GreaterOrEqual(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラー以上かどうかを検証する
 戻り値: 以上であればtrue
***************************************************************************/
	static inline  bool GreaterOrEqual(const Color4& Col,const Color4& other){
		return XMColorGreaterOrEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool LessOrEqual(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラー以下かどうかを検証する
 戻り値: 以下であればtrue
***************************************************************************/
	static inline  bool LessOrEqual(const Color4& Col,const Color4& other){
		return XMColorLessOrEqual(XMVECTOR(Col),XMVECTOR(other));
	}
/**************************************************************************
 static inline  bool Less(
	const Color4& Col,		//カラー
	const Color4& other		//比較するカラー
 );
 用途: カラーがもう一つのカラーより小さいかどうかを検証する
 戻り値: より小さければtrue
***************************************************************************/
	static inline  bool Less(const Color4& Col,const Color4& other){
		return XMColorLess(XMVECTOR(Col),XMVECTOR(other));
	}
	//状態
/**************************************************************************
 static inline  bool IsInfinite(
	const Color4& Col		//カラー
 );
 用途: カラーのいずれかの成分が正または負の無限大かどうかをテストする
 戻り値: カラーのいずれかの成分が正または負の無限大ならtrue
***************************************************************************/
	static inline  bool IsInfinite(const Color4& Col){
		return XMColorIsInfinite(XMVECTOR(Col));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Color4& Col		//カラー
 );
 用途: カラーのいずれかの成分がNaNかどうかをテストする
 戻り値: カラーのいずれかの成分がNaNならtrue
***************************************************************************/
	static inline  bool IsNaN(const Color4& Col){
		return XMColorIsNaN(XMVECTOR(Col));
	}
	//操作
/**************************************************************************
 static inline  Color4 AdjustRange (
		const Color4& Col	//カラー
 );
 用途: カラーを0.0から1.0の範囲に調整する
 戻り値: 調整したカラー
 ＊範囲外の要素は0.0か1.0に設定される
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
		const Color4& Col,	//カラー
		float Contrast		//コントラスト範囲
 );
 用途: カラーのコントラストを調節する
 戻り値: 調整したカラー
 ＊カラーは0.0から1.0の範囲に調整され、計算される
***************************************************************************/
	static inline  Color4 AdjustContrast(const Color4& Col,float Contrast){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorAdjustContrast(XMVECTOR(temp),Contrast);
	}
/**************************************************************************
 static inline  Color4 AdjustSaturation(
		const Color4& Col,	//カラー
		float Saturation		//彩度値
 );
 用途: カラーの彩度を調節する
 戻り値: 調整したカラー
 ＊カラーは0.0から1.0の範囲に調整され、計算される
 ＊彩度値が0の場合はグレースケール、1.0の場合はもとの色になる
***************************************************************************/
	static inline  Color4 AdjustSaturation(const Color4& Col,float Saturation){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorAdjustSaturation(XMVECTOR(temp),Saturation);
	}
/**************************************************************************
 static inline  Color4 Modulate(
		const Color4& Col1,	//カラー1
		const Color4& Col2	//カラー2
 );
 用途: 対応する成分を乗算して2つのカラーをブレンドする
 戻り値: ブレンドしたカラー
***************************************************************************/
	static inline  Color4 Modulate(const Color4& Col1,const Color4& Col2){
		return (Color4)XMColorModulate(XMVECTOR(Col1),XMVECTOR(Col2));
	}
/**************************************************************************
 static inline  Color4 Negative(
		const Color4& Col	//カラー
 );
 用途: カラーの負の RGB カラー値を求める
 戻り値: 負の RGB カラー値
 ＊カラーは0.0から1.0の範囲に調整され、計算される
***************************************************************************/
	static inline  Color4 Negative(const Color4& Col){
		Color4 temp = Col;
		temp.AdjustRange();
		return (Color4)XMColorNegative(XMVECTOR(temp));
	}
/**************************************************************************
 static inline  Color4 GrayColor();
 用途: グレーのカラーを得る
 戻り値: グレーのカラー値（0.5,0.5,0.5,1.0）
***************************************************************************/
	static inline  Color4 GrayColor(){
		return Color4(0.5f,0.5f,0.5f,1.0f);
	}
/**************************************************************************
 static inline  Color4 WhiteColor();
 用途: 白のカラーを得る
 戻り値: 白のカラー値（1.0,1.0,1.0,1.0）
***************************************************************************/
	static inline  Color4 WhiteColor(){
		return Color4(1.0f,1.0f,1.0f,1.0f);
	}
};
//end Color4EX

namespace QuaternionEX{
	//比較
/**************************************************************************
 static inline  bool Equal(
	const Quaternion& Qt1,	//クオータニオン1
	const Quaternion& Qt2	//クオータニオン2
 );
 用途: 2つのクオータニオンが等しいかどうかを検証する
 戻り値: 等しければtrue
***************************************************************************/
	static inline  bool Equal(const Quaternion& Qt1,const Quaternion& Qt2){
		return XMQuaternionEqual(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
/**************************************************************************
 static inline  bool NotEqual(
	const Quaternion& Qt1,	//クオータニオン1
	const Quaternion& Qt2	//クオータニオン2
 );
 用途: 2つのクオータニオンが等しくないかどうかを検証する
 戻り値: 等しくなければtrue
***************************************************************************/
	static inline  bool NotEqual(const Quaternion& Qt1,const Quaternion& Qt2){
		return XMQuaternionNotEqual(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
	//検証
/**************************************************************************
 static inline  bool IsIdentity(
	const Quaternion& Qt	//クオータニオン
 );
 用途: クオータニオンが 単位クオータニオン(0, 0, 0, 1)かどうかを検証する。
 戻り値:  単位クオータニオンならばtrue
***************************************************************************/
	static inline  bool IsIdentity(const Quaternion& Qt){
		return XMQuaternionIsIdentity(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Quaternion& Qt	//クオータニオン
 );
 用途: クオータニオンの値のいずれかが正か負の無限大かどうかを検証する。
 戻り値: 値のいずれかが正か負の無限大ならばtrue
***************************************************************************/
	static inline  bool IsInfinite(const Quaternion& Qt){
		return XMQuaternionIsInfinite(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Quaternion& Qt	//クオータニオン
 );
 用途: クオータニオンの値のいずれかがNaNかどうかを検証する。
 戻り値: 値のいずれかがNaNならばtrue
***************************************************************************/
	static inline  bool IsNaN(const Quaternion& Qt){
		return XMQuaternionIsNaN(XMVECTOR(Qt));
	}
	//操作
/**************************************************************************
 static inline  Quaternion AddRotation(
	const Quaternion& Qt,	//もとになるクオータニオン
	const Vector3& other	//追加回転する回転行列
 );
 用途: 回転行列を追加回転する
 戻り値: なし
***************************************************************************/
	static inline  Quaternion AddRotation(const Quaternion& Qt,const Vector3& other){
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(other);
		return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt),XMVECTOR(Span));
	}

/**************************************************************************
 static inline  Vector3 GetRotation();
 用途: クオータニオンから回転行列に変換して返す
 戻り値: 回転行列
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
	const Quaternion& Q0,	//三角形の 1 つ目のクオータニオン
	const Quaternion& Q1,	//三角形の 2 つ目のクオータニオン
	const Quaternion& Q2,	//三角形の 3 つ目のクオータニオン
	float f,	//加重係数
	float g		//加重係数
 );
 用途: 指定されたクオータニオンを使用して、重心座標のポイントを返す
 戻り値: 重心座標のクオータニオン
***************************************************************************/
	static inline  Quaternion BaryCentric(const Quaternion& Q0,const Quaternion& Q1,const Quaternion& Q2,float f,float g){
		return (Quaternion)XMQuaternionBaryCentric(XMVECTOR(Q0),XMVECTOR(Q1),XMVECTOR(Q2),f,g);
	}
/**************************************************************************
 static inline  Quaternion BaryCentric(
	const Quaternion& Q0,	//三角形の 1 つ目のクオータニオン
	const Quaternion& Q1,	//三角形の 2 つ目のクオータニオン
	const Quaternion& Q2,	//三角形の 3 つ目のクオータニオン
	const Vector4& f,	//加重係数
	const Vector4& g		//加重係数
 );
 用途: 指定されたクオータニオンとベクトルを使用して、重心座標のポイントを返す
 戻り値: 重心座標のクオータニオン
 ＊ベクトルfとgはそれぞれ要素が同じ値にする
***************************************************************************/
	static inline  Quaternion BaryCentricV(const Quaternion& Q0,const Quaternion& Q1,const Quaternion& Q2,
		const Vector4& f,const Vector4& g){
		return (Quaternion)XMQuaternionBaryCentricV(XMVECTOR(Q0),XMVECTOR(Q1),XMVECTOR(Q2),XMVECTOR(f),XMVECTOR(g));
	}
/**************************************************************************
 static inline  Quaternion Conjugate(
	const Quaternion& Qt	//クオータニオン
 );
 用途: クオータニオンの共役を返す
 戻り値: クオータニオンの共役
***************************************************************************/
	static inline  Quaternion Conjugate(const Quaternion& Qt){
		return (Quaternion)XMQuaternionConjugate(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float Dot(
	const Quaternion& Qt1,	//クオータニオン1
	const Quaternion& Qt2		//クオータニオン2
 );
 用途: 2つのクオータニオンの内積を返す
 戻り値: クオータニオンの内積
***************************************************************************/
	static inline  float Dot(const Quaternion& Qt1,const Quaternion& Qt2){
		return ((Vector4)XMQuaternionDot(XMVECTOR(Qt1),XMVECTOR(Qt2))).x;
	}
/**************************************************************************
 static inline  Quaternion Exp(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの指数を返す
 戻り値: クオータニオンの指数
 ＊入力のw要素は無視される
***************************************************************************/
	static inline  Quaternion Exp(const Quaternion& Qt){
		return (Quaternion)XMQuaternionExp(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion Identity();
 用途: 単位クオータニオンを返す
 戻り値: 単位クオータニオン
***************************************************************************/
	static inline  Quaternion Identity(){
		return (Quaternion)XMQuaternionIdentity();
	}
/**************************************************************************
 static inline  Quaternion Inverse(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの反転を返す
 戻り値: クオータニオンの反転
***************************************************************************/
	static inline  Quaternion Inverse(const Quaternion& Qt){
		return (Quaternion)XMQuaternionInverse(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float Length(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの長さを返す
 戻り値: クオータニオンの長さ
***************************************************************************/
	static inline  float Length(const Quaternion& Qt){
		return ((Vector4)XMQuaternionLength(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  float LengthSq(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの長さの2乗を返す
 戻り値: クオータニオンの長さの2乗
***************************************************************************/
	static inline  float LengthSq(const Quaternion& Qt){
		return ((Vector4)XMQuaternionLengthSq(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  Quaternion Ln(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの自然対数を返す
 戻り値: クオータニオンの自然対数
 ＊入力は正規化しておく
***************************************************************************/
	static inline  Quaternion Ln(const Quaternion& Qt){
		return (Quaternion)XMQuaternionLn(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion Multiply(
	const Quaternion& Qt1,		//クオータニオン1
	const Quaternion& Qt2			//クオータニオン2
 );
 用途: 2つのクオータニオンを乗算して返す
 戻り値: クオータニオンの乗算
***************************************************************************/
	static inline  Quaternion Multiply(const Quaternion& Qt1,const Quaternion& Qt2){
		return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt1),XMVECTOR(Qt2));
	}
/**************************************************************************
 static inline  Quaternion Normalize(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンを正規化して返す
 戻り値: 正規化したクオータニオン
***************************************************************************/
	static inline  Quaternion Normalize(const Quaternion& Qt){
		return (Quaternion)XMQuaternionNormalize(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Quaternion NormalizeEst(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンを正規化バージョンの予測値を返す
 戻り値: 正規化バージョンの予測値
***************************************************************************/
	static inline  Quaternion NormalizeEst(const Quaternion& Qt){
		return (Quaternion)XMQuaternionNormalizeEst(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  float ReciprocalLength(
	const Quaternion& Qt		//クオータニオン
 );
 用途: クオータニオンの長さの逆数を返す
 戻り値: クオータニオンの長さの逆数
***************************************************************************/
	 static inline  float ReciprocalLength(const Quaternion& Qt){
		return ((Vector4)XMQuaternionReciprocalLength(XMVECTOR(Qt))).x;
	}
/**************************************************************************
 static inline  Quaternion RotationAxis(
	const Vector3& Vec,		//回転軸を表す３Ｄベクトル
	float Angle				//回転角度（時計回りラジアン）
 );
 用途: 回転軸と角度からクオータニオンを作成する
 戻り値: 作成したクオータニオン
***************************************************************************/
	static inline  Quaternion RotationAxis(const Vector3& Vec,float Angle){
		return (Quaternion)XMQuaternionRotationAxis(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 static inline  Quaternion RotationMatrix(
	const XMMATRIX& m				//回転行列
 );
 用途: 回転行列からクオータニオンを作成する
 戻り値: 作成したクオータニオン
***************************************************************************/
	static inline  Quaternion RotationMatrix(const XMMATRIX& m){
		return (Quaternion)XMQuaternionRotationMatrix(m);
	}
/**************************************************************************
 static inline  Quaternion RotationNormal(
	const Vector3& Vec,		//回転軸を表す正規化された３Ｄベクトル
	float Angle				//回転角度（時計回りラジアン）
 );
 用途: 正規化された回転軸と角度からクオータニオンを作成する
 戻り値: 作成したクオータニオン
***************************************************************************/
	static inline  Quaternion RotationNormal(const Vector3& Vec,float Angle){
		return (Quaternion)XMQuaternionRotationNormal(XMVECTOR(Vec),Angle);
	}
/**************************************************************************
 static inline  Quaternion RotationRollPitchYaw(
	float Pitch,		//X軸回転（ラジアン）
	float Yaw,			//Y軸回転（ラジアン）
	float Roll			//Z軸回転（ラジアン）
 );
 用途: 各軸の回転からクオータニオンを作成する
 戻り値: 作成したクオータニオン
 ＊回転はRollPitchYaw（Z、Y、X）の順で行われる
***************************************************************************/
	static inline  Quaternion RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		return (Quaternion)XMQuaternionRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 static inline  Quaternion RotationRollPitchYawFromVector(
	const Vector3& Vec		//回転軸を表す３Ｄベクトル
 );
 用途: 3D回転ベクトルからクオータニオンを作成する
 戻り値: 作成したクオータニオン
 ＊回転はRollPitchYaw（Z、Y、X）の順で行われる
***************************************************************************/
	static inline  Quaternion RotationRollPitchYawFromVector(const Vector3& Vec){
		return (Quaternion)XMQuaternionRotationRollPitchYawFromVector(Vec);
	}
/**************************************************************************
 static inline  Quaternion Slerp(
	const Quaternion& Qt0,		//補間元（正規化されたクオータニオン）
	const Quaternion& Qt1,		//補間先（正規化されたクオータニオン）
	float t						//補間係数（0.0fから1.0fの間を指定）
 );
 用途: 2つのクオータニオンから係数で補間されたクオータニオンを作成する
 戻り値: 作成したクオータニオン
 ＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
***************************************************************************/
	static inline  Quaternion Slerp(const Quaternion& Qt0,const Quaternion& Qt1,float t){
		return (Quaternion)XMQuaternionSlerp(XMVECTOR(Qt0),XMVECTOR(Qt1),t);
	}
/**************************************************************************
 static inline  Quaternion SlerpV(
	const Quaternion& Qt0,		//補間元（正規化されたクオータニオン）
	const Quaternion& Qt1,		//補間先（正規化されたクオータニオン）
	const Vector4& Vec			//補間係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
 );
 用途: 2つのクオータニオンから係数で補間されたクオータニオンを作成する
 戻り値: 作成したクオータニオン
 ＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
***************************************************************************/
	static inline  Quaternion SlerpV(const Quaternion& Qt0,const Quaternion& Qt1,const Vector4& Vec){
		return (Quaternion)XMQuaternionSlerpV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  Quaternion Squad(
	const Quaternion& Qt0,		//正規化されたクオータニオン１
	const Quaternion& Qt1,		//正規化されたクオータニオン２
	const Quaternion& Qt2,		//正規化されたクオータニオン３
	const Quaternion& Qt3,		//正規化されたクオータニオン４
	float t						//補間制御係数（0.0fから1.0fの間を指定）
 );
 用途: 球状平方補間を使用して、4 つの単位クオータニオン間を補間する
 戻り値: 作成したクオータニオン
 ＊Q1、Q2、Q3はQuaternionEX::SquadSetup関数で作成する
***************************************************************************/
	static inline  Quaternion Squad(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,float t){
		return (Quaternion)XMQuaternionSquad(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),t);
	}
/**************************************************************************
 static inline  void SquadSetup(
	Quaternion* pA,				//結果を受けるクオータニオンA
	Quaternion* pB,				//結果を受けるクオータニオンB
	Quaternion* pC,				//結果を受けるクオータニオンC
	const Quaternion& Qt0,		//正規化されたクオータニオン１
	const Quaternion& Qt1,		//正規化されたクオータニオン２
	const Quaternion& Qt2,		//正規化されたクオータニオン３
	const Quaternion& Qt3		//正規化されたクオータニオン４
 );
 用途: 球状平方補間のためのセットアップを行う
 戻り値: なし
 ＊pA、pB、pCはそれぞれSquad()、SquadV()関数のQt1、Qt2、Qt3に入力する
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
	const Quaternion& Qt0,		//正規化されたクオータニオン１
	const Quaternion& Qt1,		//正規化されたクオータニオン２
	const Quaternion& Qt2,		//正規化されたクオータニオン３
	const Quaternion& Qt3,		//正規化されたクオータニオン４
	const Vector4& Vec			//補間制御係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
 );
 用途: 球状平方補間を使用して、4 つの単位クオータニオン間を補間する
 戻り値: 作成したクオータニオン
 ＊Q1、Q2、Q3はQuaternionEX::SquadSetup関数で作成する
***************************************************************************/
	static inline  Quaternion SquadV(const Quaternion& Qt0,const Quaternion& Qt1,const Quaternion& Qt2,const Quaternion& Qt3,const Vector4& Vec){
		return (Quaternion)XMQuaternionSquadV(XMVECTOR(Qt0),XMVECTOR(Qt1),XMVECTOR(Qt2),XMVECTOR(Qt3),XMVECTOR(Vec));
	}
/**************************************************************************
 static inline  void ToAxisAngle(
	Vector3* pVec,			//回転軸を得る３Ｄベクトル
	float* pAngle,			//回転角度を得る変数
	const Quaternion& Qt	//計算もととなるクオータニオン
 );
 用途: クオータニオンから、回転軸と回転角度を得る
 戻り値: なし
 ＊pVecには回転軸ベクトル、pAngleには角度が設定される
***************************************************************************/
	static inline  void ToAxisAngle(Vector3* pVec,float* pAngle,const Quaternion& Qt){
		XMVECTOR Vec(*pVec);
		XMQuaternionToAxisAngle(&Vec,pAngle,XMVECTOR(Qt));
		*pVec = Vec;
	}


/**************************************************************************
	static inline  Quaternion Billboard(
	const Vector3& Line			//計算元となる線
	)t;
	用途: Lineに対してビルボード角度を設定したクオータニオンを得る
	戻り値: クオータニオン
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
	const Vector3& Line			//計算元となる線(オブジェクトからカメラ位置を引いたもの)
	)t;
	用途: Lineに対してフェイシング角度を設定したクオータニオンを得る
	戻り値: クオータニオン
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
	const Vector3& Line			//計算元となる線(オブジェクトからカメラ位置を引いたもの)
	)t;
	用途: Lineに対してY軸フェイシング角度を設定したクオータニオンを得る
	戻り値: クオータニオン
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
		const Matrix4X4& m						//元になる行列
 );
 用途: 行列内の位置情報を取得する(直接行列から取り出す)
 戻り値: 位置ベクトル
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
		const Matrix4X4& m						//元になる行列
 );
 用途: 行列内の位置情報を取得する
 戻り値: 位置ベクトル
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
		const Matrix4X4& m						//元になる行列
 );
 用途: 行列内のスケーリングを取得する
 戻り値: スケーリングベクトル
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
		const Matrix4X4& m						//元になる行列
 );
 用途: 行列内の回転を取得する
 戻り値: 回転クオータニオン
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
 用途: 行列内のX軸回転を取得する
 戻り値: X軸の回転ベクトル
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
 用途: 行列内のY軸回転を取得する
 戻り値: Y軸の回転ベクトル
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
 用途: 行列内のZ軸回転を取得する
 戻り値: Z軸の回転ベクトル
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
		const Vector3& Scaling,					//スケーリング
		const Vector3& RotOrigin,				//回転の中心
		const Quaternion& RotationQuaternion,	//クオータニオン
		const Vector3& Translation				//平行移動
 );
 用途: スケーリング、回転の中心座標、クオータニオン、平行移動から行列を作成する
 戻り値: 作成した行列
***************************************************************************/
	static inline  Matrix4X4 AffineTransformation(const Vector3& Scaling,const Vector3& RotOrigin,
			const Quaternion& RotationQuaternion,const Vector3& Translation){
		return (Matrix4X4)XMMatrixAffineTransformation(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			XMVECTOR(RotationQuaternion),XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  Matrix4X4 AffineTransformation2D(
		const Vector2& Scaling,					//スケーリング
		const Vector2& RotOrigin,				//回転の中心
		float Rotation,							//回転
		const Vector2& Translation				//平行移動
 );
 用途: スケーリング、回転の中心座標、回転値（ラジアン）、平行移動から2D行列を作成する
 戻り値: 作成した行列
***************************************************************************/
	static inline  Matrix4X4 AffineTransformation2D(const Vector2& Scaling,
		const Vector2& RotOrigin,float Rotation,const Vector2& Translation){
		return (Matrix4X4)XMMatrixAffineTransformation2D(XMVECTOR(Scaling),XMVECTOR(RotOrigin),
			Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  bool Decompose(
		Vector3* pScaling,					//スケーリング
		Quaternion* pQt,					//クオータニオン
		Vector3* pTranslation,				//平行移動
		const Matrix4X4& m						//元になる行列
 );
 用途: 行列からスケーリング、回転、平行移動を取り出す
 戻り値: 成功すればtrue
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
	const Matrix4X4& m						//元になる行列
 );
 用途: 行列式を返す
 戻り値: 作成した行列式（Vector4の各要素に入る）
***************************************************************************/
	static inline  Vector4 Determinant(const Matrix4X4& m){
		return (Vector4)XMMatrixDeterminant(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 Identity();
 用途: 単位行列を返す
 戻り値: 単位行列
***************************************************************************/
	static inline  Matrix4X4 Identity(){
		return (Matrix4X4)XMMatrixIdentity();
	}
/**************************************************************************
 static inline  Matrix4X4 Inverse(
	Vector4* pVec,					//行列式を受け取るベクトル
	const Matrix4X4& m					//元になる行列
 );
 用途:逆行列を返す
 戻り値: 単位行列
 ＊pVecには行列式が入る
***************************************************************************/
	static inline  Matrix4X4 Inverse(Vector4* pVec,const Matrix4X4& m){
		XMVECTOR Vec(*pVec);
		Matrix4X4 ret = (Matrix4X4)XMMatrixInverse(&Vec,XMMATRIX(m));
		*pVec = Vec;
		return ret;
	}
/**************************************************************************
 static inline  bool IsIdentity(
	const Matrix4X4& m					//元になる行列
 );
 用途: 単位行列かどうかを検証する
 戻り値: 単位行列ならtrue
***************************************************************************/
	static inline  bool IsIdentity(const Matrix4X4& m){
		return XMMatrixIsIdentity(XMMATRIX(m));
	}
/**************************************************************************
 static inline  bool IsInfinite(
	const Matrix4X4& m					//元になる行列
 );
 用途: 行列のいずれかの要素が正または負の無限大かどうかを検証する
 戻り値: 正または負の無限大ならtrue
***************************************************************************/
	static inline  bool IsInfinite(const Matrix4X4& m){
		return XMMatrixIsInfinite(XMMATRIX(m));
	}
/**************************************************************************
 static inline  bool IsNaN(
	const Matrix4X4& m					//元になる行列
 );
 用途: 行列のいずれかの要素がNaNかどうかを検証する
 戻り値: NaNならtrue
***************************************************************************/
	static inline  bool IsNaN(const Matrix4X4& m){
		return XMMatrixIsNaN(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 LookAtLH(
	const Vector3& Eye,					//カメラ位置
	const Vector3& At,					//カメラ視点
	const Vector3& Up					//カメラ傾き（通常<0,1,0>）
 );
 用途:左手座標ビュー行列を作成する
 戻り値: 左手座標ビュー行列
***************************************************************************/
	static inline  Matrix4X4 LookAtLH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookAtLH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookAtRH(
	const Vector3& Eye,					//カメラ位置
	const Vector3& At,					//カメラ視点
	const Vector3& Up					//カメラ傾き（通常<0,1,0>）
 );
 用途:右手座標ビュー行列を作成する
 戻り値: 右手座標ビュー行列
***************************************************************************/
	static inline  Matrix4X4 LookAtRH(const Vector3& Eye,const Vector3& At,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookAtRH(
								XMVECTOR(Eye),
								XMVECTOR(At),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookToLH(
	const Vector3& Eye,					//カメラ位置
	const Vector3& EyeDirection,		//カメラ向き
	const Vector3& Up					//カメラ傾き（通常<0,1,0>）
 );
 用途:カメラ位置、向き、傾きから左手座標ビュー行列を作成する
 戻り値: 左手座標ビュー行列
***************************************************************************/
	static inline  Matrix4X4 LookToLH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookToLH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 LookToRH(
	const Vector3& Eye,					//カメラ位置
	const Vector3& EyeDirection,		//カメラ向き
	const Vector3& Up					//カメラ傾き（通常<0,1,0>）
 );
 用途:カメラ位置、向き、傾きから右手座標ビュー行列を作成する
 戻り値: 右手座標ビュー行列
***************************************************************************/
	static inline  Matrix4X4 LookToRH(const Vector3& Eye,const Vector3& EyeDirection,const Vector3& Up){
		return (Matrix4X4)XMMatrixLookToRH(
								XMVECTOR(Eye),
								XMVECTOR(EyeDirection),
								XMVECTOR(Up));
	}
/**************************************************************************
 static inline  Matrix4X4 Multiply(
	const Matrix4X4& m1,					//演算元になる行列
	const Matrix4X4& m2						//演算先になる行列
 );
 用途:行列の積を返す
 戻り値: 行列の積
***************************************************************************/
	static inline  Matrix4X4 Multiply(const Matrix4X4& m1,const Matrix4X4& m2){
		return  (Matrix4X4)XMMatrixMultiply(XMMATRIX(m1),XMMATRIX(m2));
	}
/**************************************************************************
 static inline  Matrix4X4 MultiplyTranspose(
	const Matrix4X4& m1,					//演算元になる行列
	const Matrix4X4& m2						//演算先になる行列
 );
 用途:積の転置行列を返す
 戻り値: 積の転置行列
***************************************************************************/
	static inline  Matrix4X4 MultiplyTranspose(const Matrix4X4& m1,const Matrix4X4& m2){
		return  (Matrix4X4)XMMatrixMultiplyTranspose(XMMATRIX(m1),XMMATRIX(m2));
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicLH(
	float ViewWidth,		//幅
	float ViewHeight,		//高さ
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途:左手座標系の正射影行列を返す
 戻り値: 左手座標系の正射影行列
***************************************************************************/
	static inline  Matrix4X4 OrthographicLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicRH(
	float ViewWidth,		//幅
	float ViewHeight,		//高さ
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途:右手座標系の正射影行列を返す
 戻り値: 右手座標系の正射影行列
***************************************************************************/
	static inline  Matrix4X4 OrthographicRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicOffCenterLH(
	float ViewLeft,			//左
	float ViewRight,		//右
	float ViewBottom,		//下
	float ViewTop,			//上
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途:左手座標系のカスタム正射影行列を返す
 戻り値: 左手座標系のカスタム正射影行列
***************************************************************************/
	static inline  Matrix4X4 OrthographicOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicOffCenterLH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 OrthographicOffCenterRH(
	float ViewLeft,			//左
	float ViewRight,		//右
	float ViewBottom,		//下
	float ViewTop,			//上
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途:右手座標系のカスタム正射影行列を返す
 戻り値: 右手座標系のカスタム正射影行列
***************************************************************************/
	static inline  Matrix4X4 OrthographicOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixOrthographicOffCenterRH(
						ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveFovLH(
	float FovAngleY,		//トップダウン視野角度
	float AspectHByW,		//ビュー空間の高さと幅のアスペクト比
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: 視野に基づいて、左手座標系のパースペクティブ射影行列を返す
 戻り値: 左手座標系のパースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveFovLH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveFovLH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveFovRH(
	float FovAngleY,		//トップダウン視野角度
	float AspectHByW,		//ビュー空間の高さと幅のアスペクト比
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: 視野に基づいて、右手座標系のパースペクティブ射影行列を返す
 戻り値: 右手座標系のパースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveFovRH(float FovAngleY,float AspectHByW,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveFovRH(FovAngleY,AspectHByW,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveLH(
	float ViewWidth,		//近くのクリップ面の視錐台の幅
	float ViewHeight,		//近くのクリップ面の視錐台の高さ
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: 左手座標系のパースペクティブ射影行列を返す
 戻り値: 左手座標系のパースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveLH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveLH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveRH(
	float ViewWidth,		//近くのクリップ面の視錐台の幅
	float ViewHeight,		//近くのクリップ面の視錐台の高さ
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: 右手座標系のパースペクティブ射影行列を返す
 戻り値: 右手座標系のパースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveRH(float ViewWidth,float ViewHeight,float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveRH(ViewWidth,ViewHeight,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveOffCenterLH(
	float ViewLeft,			//クリップ視錐台の左側の x 座標
	float ViewRight,		//クリップ視錐台の右側の x 座標
	float ViewBottom,		//クリップ視錐台の下側の y 座標
	float ViewTop,			//クリップ視錐台の上側の y 座標
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: カスタム バージョンの左手座標系パースペクティブ射影行列を返す
 戻り値: カスタム バージョンの左手座標系パースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveOffCenterLH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveOffCenterLH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 PerspectiveOffCenterRH(
	float ViewLeft,			//クリップ視錐台の左側の x 座標
	float ViewRight,		//クリップ視錐台の右側の x 座標
	float ViewBottom,		//クリップ視錐台の下側の y 座標
	float ViewTop,			//クリップ視錐台の上側の y 座標
	float NearZ,			//奥行手前
	float FarZ				//奥行奥
 );
 用途: カスタム バージョンの右手座標系パースペクティブ射影行列を返す
 戻り値: カスタム バージョンの右手座標系パースペクティブ射影行列
***************************************************************************/
	static inline  Matrix4X4 PerspectiveOffCenterRH(float ViewLeft,float ViewRight,float ViewBottom,float ViewTop,
			float NearZ,float FarZ){
		return  (Matrix4X4)XMMatrixPerspectiveOffCenterRH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
	}
/**************************************************************************
 static inline  Matrix4X4 Reflect(
	const Plane& Pln		//平面
 );
 用途: 指定された平面を介してベクトルを反映するトランスフォーム行列を作成し返す
 戻り値: ベクトルを反映するトランスフォーム行列
***************************************************************************/
	static inline  Matrix4X4 Reflect(const Plane& Pln){
		return  (Matrix4X4)XMMatrixReflect(XMVECTOR(Pln));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationAxis(
	const Vector3& Axis,		//回転軸を表すベクトル
	float Angle				//回転角度（ラジアン）
 );
 用途: 任意の軸を回転軸にして、回転した行列を作成し返す
 戻り値: 任意の軸を回転軸にして、回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationAxis(const Vector3& Axis,float Angle){
		return  (Matrix4X4)XMMatrixRotationAxis(XMVECTOR(Axis),Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationNormal(
	const Vector3& NormalAxis,		//回転軸を記述する法線ベクトル
	float Angle						//回転角度（ラジアン）
 );
 用途: 任意の法線ベクトルを回転軸にして、回転した行列を作成し返す
 戻り値: 任意の法線ベクトルを回転軸にして、回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationNormal(const Vector3& NormalAxis,float Angle){
		return  (Matrix4X4)XMMatrixRotationNormal(XMVECTOR(NormalAxis),Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationQuaternion(
	const Quaternion& Qt		//回転を記述するクオータニオン
 );
 用途: クオータニオンから回転行列を作成し返す
 戻り値: クオータニオンから回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationQuaternion(const Quaternion& Qt){
		return  (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(Qt));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationRollPitchYaw(
	float Pitch,		//X軸回転
	float Yaw,			//Y軸回転
	float Roll			//Z軸回転
 );
 用途: オイラー角に基づいて回転行列を作成する
 戻り値: オイラー角に基づいて回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationRollPitchYaw(float Pitch,float Yaw,float Roll){
		return  (Matrix4X4)XMMatrixRotationRollPitchYaw(Pitch,Yaw,Roll);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationRollPitchYawFromVector(
	const Vector3& Angles		//回転を記述するベクトル
 );
 用途: オイラー角(ベクトル)に基づいて回転行列を作成する
 戻り値: オイラー角(ベクトル)に基づいて回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationRollPitchYawFromVector(const Vector3& Angles){
		return  (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Angles));
	}
/**************************************************************************
 static inline  Matrix4X4 RotationX(
	float Angle		//X軸回転
 );
 用途: X軸回転に基づいて回転行列を作成する
 戻り値: X軸回転に基づいて回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationX(float Angle){
		return  (Matrix4X4)XMMatrixRotationX(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationY(
	float Angle		//Y軸回転
 );
 用途: Y軸回転に基づいて回転行列を作成する
 戻り値: Y軸回転に基づいて回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationY(float Angle){
		return  (Matrix4X4)XMMatrixRotationY(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 RotationZ(
	float Angle		//Z軸回転
 );
 用途: Z軸回転に基づいて回転行列を作成する
 戻り値: Z軸回転に基づいて回転した行列
***************************************************************************/
	static inline  Matrix4X4 RotationZ(float Angle){
		return  (Matrix4X4)XMMatrixRotationZ(Angle);
	}
/**************************************************************************
 static inline  Matrix4X4 Scaling(
	float ScaleX,	//Xに沿ったスケーリング
	float ScaleY,	//Yに沿ったスケーリング
	float ScaleZ	//Zに沿ったスケーリング
 );
 用途: x 軸、y 軸、および z 軸に沿ってスケーリングした行列を作成する
 戻り値: x 軸、y 軸、および z 軸に沿ってスケーリングした行列
***************************************************************************/
	static inline  Matrix4X4 Scaling(float ScaleX,float ScaleY,float ScaleZ){
		return  (Matrix4X4)XMMatrixScaling(ScaleX,ScaleY,ScaleZ);
	}
/**************************************************************************
 static inline  Matrix4X4 ScalingFromVector(
	const Vector3& Scale		//スケーリングを記述するベクトル
 );
 用途: ベクトルに沿ってスケーリングした行列を作成する
 戻り値: ベクトルに沿ってスケーリングした行列
***************************************************************************/
	static inline  Matrix4X4 ScalingFromVector(const Vector3& Scale){
		return  (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scale));
	}
/**************************************************************************
 static inline  Matrix4X4 Set(
	float m00,	//(0,0)の値
	float m01,	//(0,1)の値
	float m02,	//(0,2)の値
	float m03,	//(0,3)の値
	float m10,	//(1,0)の値
	float m11,	//(1,1)の値
	float m12,	//(1,2)の値
	float m13,	//(1,3)の値
	float m20,	//(2,0)の値
	float m21,	//(2,1)の値
	float m22,	//(2,2)の値
	float m23,	//(2,3)の値
	float m30,	//(3,0)の値
	float m31,	//(3,1)の値
	float m32,	//(3,2)の値
	float m33	//(3,3)の値
 );
 用途: 各値を設定した行列を作成する
 戻り値: 各値を設定した行列
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
	const Plane& ShadowPlane,			//基準面
	const Vector4& LightPosition		//ライトの位置
 );
 用途: ジオメトリを平面に射影するトランスフォーム行列を作成する
 戻り値: ジオメトリを平面に射影するトランスフォーム行列
 ＊ライトのw要素は、0.0でディレクショナルライト、1.0でポイントライトとなる
***************************************************************************/
	static inline  Matrix4X4 Shadow(const Plane& ShadowPlane,const Vector4& LightPosition){
		return  (Matrix4X4)XMMatrixShadow(XMVECTOR(ShadowPlane),XMVECTOR(LightPosition));
	}
/**************************************************************************
 static inline  Matrix4X4 Transformation(
 	const Vector3& ScalingOrigin,		//スケーリングの中心を記述する 3D ベクトル
 	const Quaternion& ScalingOrientationQuaternion,		//スケーリングの向きを記述するクオータニオン
	const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
 	const Vector3& RotationOrigin,		//回転の中心を記述する 3D ベクトル
 	const Quaternion& RotationQuaternion,		//RotationOrigin によって示される原点を中心とする回転を記述するクオータニオン
	const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 );
 用途: トランスフォーム行列を作成する
 戻り値: トランスフォーム行列
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
 	const Vector2& ScalingOrigin,		//スケーリングの中心を記述する 2D ベクトル
 	float ScalingOrientation,			//スケーリング回転係数
	const Vector2& Scaling,				//x 軸および y 軸のスケーリング係数を含む 2D ベクトル
 	const Vector2& RotationOrigin,		//回転の中心を記述する 2D ベクトル
 	float Rotation,						//回転の角度 (ラジアン単位)
	const Vector2& Translation			//平行移動を記述する 2D ベクトル
 );
 用途: 2Dトランスフォーム行列をxy平面に作成する
 戻り値: 2Dトランスフォーム行列
***************************************************************************/
	static inline  Matrix4X4 Transformation2D(const Vector2& ScalingOrigin,float ScalingOrientation,const Vector2& Scaling,
 	const Vector2& RotationOrigin,float Rotation,
	const Vector2& Translation){
		return  (Matrix4X4)XMMatrixTransformation2D(XMVECTOR(ScalingOrigin),ScalingOrientation,XMVECTOR(Scaling),
			XMVECTOR(RotationOrigin),Rotation,XMVECTOR(Translation));
	}
/**************************************************************************
 static inline  Matrix4X4 Translation(
	float OffsetX,	//X軸に沿った平行移動
	float OffsetY,	//Y軸に沿った平行移動
	float OffsetZ	//Z軸に沿った平行移動
 );
 用途: 指定されたオフセットから平行移動行列を作成する
 戻り値: 指定されたオフセットで平行移動した行列
***************************************************************************/
	static inline  Matrix4X4 Translation(float OffsetX,float OffsetY,float OffsetZ){
		return  (Matrix4X4)XMMatrixTranslation(OffsetX,OffsetY,OffsetZ);
	}
/**************************************************************************
 static inline  Matrix4X4 TranslationFromVector(
	const Vector3& Offset				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 );
 用途: ベクトルから平行移動行列を作成する
 戻り値: ベクトルから平行移動した行列
***************************************************************************/
	static inline  Matrix4X4 TranslationFromVector(const Vector3& Offset){
		return  (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Offset));
	}
/**************************************************************************
 static inline  Matrix4X4 Transpose(
	const Matrix4X4& m				//転置する行列
 );
 用途: 行列を転置する
 戻り値: 転置した行列
***************************************************************************/
	static inline  Matrix4X4 Transpose(const Matrix4X4& m){
		return  (Matrix4X4)XMMatrixTranspose(XMMATRIX(m));
	}
/**************************************************************************
 static inline  Matrix4X4 DefTransformation(
	const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
 	const Quaternion& RotationQt,		//回転を記述するクオータニオン
	const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 );
 用途: スケーリング、回転、移動から行列を作成する
 戻り値: 作成した行列
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
	const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
 	const Vector3& Rotation,			//回転を記述するベクトル
	const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 );
 用途: スケーリング、回転、移動から行列を作成する
 戻り値: 作成した行列
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
	const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
	const Vector3& Translation,				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 	const Quaternion& RotationQuaternion		//回転を記述するクオータニオン
 );
 用途: スケーリング、移動、回転から行列を作成する
 戻り値: 作成した行列
 ＊回転より移動を先に行う
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
	const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
	const Vector3& Translation,				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
 	const Vector3& Rotation			//回転を記述するベクトル
 );
 用途: スケーリング、移動、回転から行列を作成する
 戻り値: 作成した行列
 ＊回転より移動を先に行う
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

