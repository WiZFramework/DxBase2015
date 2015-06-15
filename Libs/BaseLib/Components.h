/*!
@file Components.h
@brief コンポーネント（更新系）

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	//	class Component :public Object, public ShapeInterface;
	/*!
	コンポーネント親クラス
	*/
	//--------------------------------------------------------------------------------------
	class Component :public Object, public ShapeInterface{
	protected:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	explicit Component(
		//		const shared_ptr<GameObject>& GameObjectPtr	//このコンポーネントを所持するゲームオブジェクト
		//	);
		/*!
		@breaf プロテクトコンストクタ
		@param const shared_ptr<GameObject>& GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit Component(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	virtual ~Component();
		/*!
		@breaf プロテクトデストクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Component();
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	shared_ptr<GameObject> GetGameObject() const;
		/*!
		@breaf ゲームオブジェクトの取得
		@param なし
		@return	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		//	bool IsGameObjectActive() const;
		/*!
		@breaf このコンポーネントを所持するゲームオブジェクトが有効かどうかを取得
		@param なし
		@return	ゲームオブジェクトが有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsGameObjectActive() const;
		//--------------------------------------------------------------------------------------
		//	void AttachGameObject(
		//		const shared_ptr<GameObject>& GameObjectPtr	//新しいゲームオブジェクト
		//	);
		/*!
		@breaf ゲームオブジェクトを設定する
		@param const shared_ptr<GameObject>& GameObjectPtr	新しいゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	bool IsUpdateActive() const;
		/*!
		@breaf Updateがアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		//	bool GetUpdateActive() const;
		/*!
		@breaf Updateがアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		//	void SetUpdateActive(
		//		bool b	//アクティブかどうか
		//	);
		/*!
		@breaf Updateがアクティブかどうかを設定する
		@param bool b	アクティブかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);

		//--------------------------------------------------------------------------------------
		//	bool IsUpdate2Active() const;
		/*!
		@breaf Update2がアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdate2Active() const;
		//--------------------------------------------------------------------------------------
		//	bool GetUpdate2Active() const;
		/*!
		@breaf Update2がアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdate2Active() const;
		//--------------------------------------------------------------------------------------
		//	void SetUpdate2Active(
		//		bool b	//アクティブかどうか
		//	);
		/*!
		@breaf Update2がアクティブかどうかを設定する
		@param bool b	アクティブかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdate2Active(bool b);

		//--------------------------------------------------------------------------------------
		//	bool IsDrawActive() const;
		/*!
		@breaf Drawがアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const;
		//--------------------------------------------------------------------------------------
		//	bool GetDrawActive() const;
		/*!
		@breaf Drawがアクティブかどうかを得る
		@param なし
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetDrawActive() const;
		//--------------------------------------------------------------------------------------
		//	void SetDrawActive(
		//		bool b	//アクティブかどうか
		//	);
		/*!
		@breaf Drawがアクティブかどうかを設定する
		@param bool b	アクティブかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b);

		//操作
		//--------------------------------------------------------------------------------------
		//	virtual void Update2();
		/*!
		@breaf Update2処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update2(){}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	/*!
	変換クラス
	*/
	//--------------------------------------------------------------------------------------
	class Transform : public Component {
		//--------------------------------------------------------------------------------------
		//	void SetLocalQuaternion(
		//		const Quaternion& quaternion, //回転クオータニオン
		//		bool CalcFlg = true		//すぐに行列計算するかどうか
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalQuaternion(const Quaternion& quaternion, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalRotation(
		//		const Vector3& Rot,	//回転ベクトル
		//		bool CalcFlg = true	//すぐに行列計算するかどうか
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalRotation(const Vector3& Rot, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalRotation(
		//		float x, //X回転
		//		float y, //Y回転
		//		float z, //Z回転
		//		bool CalcFlg = true	//すぐに行列計算するかどうか
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalRotation(float x, float y, float z, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		const Vector3& Position, //位置ベクトル
		//		bool CalcFlg = true	//すぐに行列計算するかどうか
		//	);
		//--------------------------------------------------------------------------------------
		void SetLocalPosition(const Vector3& Position, bool CalcFlg = true);
		//--------------------------------------------------------------------------------------
		//	void SetLocalPosition(
		//		float x, //X位置
		//		float y, //Y位置
		//		float z, //Z位置
		//		bool CalcFlg = true //すぐに行列計算するかどうか
		//	);
		/*!
		@breaf ローカル位置を設定する
		@param float x, X位置
		@param float y, Y位置
		@param float z, Z位置
		@param bool CalcFlg = true		すぐに行列計算するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLocalPosition(float x, float y, float z, bool CalcFlg = true);
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	explicit Transform(
		//		const shared_ptr<GameObject>& GameObjectPtr	//このコンポーネントを所持するゲームオブジェクト
		//	);
		/*!
		@breaf コンストクタ
		@param const shared_ptr<GameObject>& GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit Transform(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		//	virtual ~Transform();
		/*!
		@breaf デストクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Transform();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	bool IsPriorityMatrix()const;;
		/*!
		@breaf 行列優先にするかどうか
		@param なし
		@return	行列優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityMatrix()const;
		//--------------------------------------------------------------------------------------
		//	bool GetPriorityMatrix()const;;
		/*!
		@breaf 行列優先にするかどうか
		@param なし
		@return	行列優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetPriorityMatrix()const;
		//--------------------------------------------------------------------------------------
		//	void SetPriorityMatrix(
		//	bool b	//行列優先ならtrue
		//	);
		/*!
		@breaf 行列優先にするかどうかをセット.
		＊行列優先にすると自動行列計算は行わない
		@param bool b	行列優先ならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityMatrix(bool b);
		//--------------------------------------------------------------------------------------
		//	bool IsPriorityPosition()const;
		/*!
		@breaf 行列計算を位置優先にするかどうか
		@param なし
		@return	位置優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityPosition()const;
		//--------------------------------------------------------------------------------------
		//	bool GetPriorityPosition()const;
		/*!
		@breaf 行列計算を位置優先にするかどうか
		@param なし
		@return	位置優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetPriorityPosition()const;
		//--------------------------------------------------------------------------------------
		//	void SetPriorityPosition(
		//	bool b //優先にするかどうか
		//	);
		/*!
		@breaf 行列計算を位置優先にするかどうかをセット
		@param bool b 優先にするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityPosition(bool b);
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetBeforeWorldMatrix() const;
		/*!
		@breaf 1ターン前の行列を得る
		@param なし
		@return	1ターン前の行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetBeforeWorldMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetBeforeLocalMatrix() const;
		/*!
		@breaf 1ターン前のワールド行列を得る
		@param なし
		@return	1ターン前のワールド行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetBeforeLocalMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeScale() const;
		/*!
		@breaf 1ターン前のスケーリングを得る
		@param なし
		@return	1ターン前のスケーリング
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeScale() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetBeforeWorldQuaternion() const;
		/*!
		@breaf 1ターン前のワールド回転を得る
		@param なし
		@return	1ターン前のワールド回転
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetBeforeWorldQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetBeforeLocalQuaternion() const;
		/*!
		@breaf 1ターン前のローカル回転を得る
		@param なし
		@return	1ターン前のローカル回転
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetBeforeLocalQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeWorldPosition() const;
		/*!
		@breaf 1ターン前のワールド位置を得る
		@param なし
		@return	1ターン前のワールド位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeWorldPosition() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetBeforeLocalPosition() const;
		/*!
		@breaf 1ターン前のローカル位置を得る
		@param なし
		@return	1ターン前のローカル位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetBeforeLocalPosition() const;
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetWorldMatrix() const;
		/*!
		@breaf ワールド行列を得る
		@param なし
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetWorldMatrix() const;
		//--------------------------------------------------------------------------------------
		//	void SetWorldMatrix(
		//		const Matrix4X4& WorldMatrix	//ワールド行列
		//	);
		/*!
		@breaf ワールド行列を設定する.
		＊PriorityMatrixが有効のときのみ設定できる。無効の場合は例外
		@param const Matrix4X4& WorldMatrix	ワールド行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWorldMatrix(const Matrix4X4& WorldMatrix);
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetScale() const;
		/*!
		@breaf スケーリングを得る
		@param なし
		@return	スケーリング
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetScale() const;
		//--------------------------------------------------------------------------------------
		//	void SetScale(
		//		const Vector3& Scale	//スケーリング 
		//	);
		/*!
		@breaf スケーリングを設定する.
		@param const Vector3& Scale	スケーリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(const Vector3& Scale);
		//--------------------------------------------------------------------------------------
		//	void SetScale(
		//		float x, //Xスケーリング
		//		float y, //Yスケーリング
		//		float z //Zスケーリング
		//	);
		/*!
		@breaf スケーリングを設定する.
		@param float x Xスケーリング
		@param float y Yスケーリング
		@param float z Zスケーリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetQuaternion() const;
		/*!
		@breaf ワールド回転を得る
		@param なし
		@return	ワールド回転（クオータニオン）
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	void SetWorldQuaternion(
		//		const Quaternion& quaternion	//クオータニオン
		//	);
		/*!
		@breaf ワールド回転を設定する
		@param const Quaternion& quaternion	クオータニオン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetQuaternion(const Quaternion& quaternion);
		//--------------------------------------------------------------------------------------
		//	Vector3 GetRotation() const;
		/*!
		@breaf ワールド回転を得る
		@param なし
		@return	ワールド回転（ベクトル）
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetRotation() const;
		//--------------------------------------------------------------------------------------
		//	void SetRotation(
		//		const Vector3& Rot	//回転ベクトル
		//	);
		/*!
		@breaf ワールド回転を設定する.
		＊すぐに行列に反映する
		@param const Vector3& Rot	回転ベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(const Vector3& Rot);
		//--------------------------------------------------------------------------------------
		//	void SetRotation(
		//		float x, //X回転
		//		float y, //Y回転
		//		float z	//Z回転
		//	);
		/*!
		@breaf ワールド回転を設定する.
		＊すぐに行列に反映する
		@param float x,	X回転
		@param float y,	Y回転
		@param float z,	Z回転
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetPosition() const;
		/*!
		@breaf ワールド位置を得る
		@param なし
		@return	ワールド位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetPosition() const;
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		const Vector3& Position	//設定する位置
		//	);
		/*!
		@breaf ワールド位置を設定する.
		＊すぐに行列に反映する
		@param const Vector3& Position	設定する位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(const Vector3& Position);
		//--------------------------------------------------------------------------------------
		//	void SetPosition(
		//		float x,	//X位置
		//		float y,	//Y位置
		//		float z		//Z位置
		//	);
		/*!
		@breaf ワールド位置を設定する.
		＊すぐに行列に反映する
		@param float x,	X位置
		@param float y,	Y位置
		@param float z,	Z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		//	const Matrix4X4& GetLocalMatrix() const;
		/*!
		@breaf ローカル行列を得る
		@param なし
		@return	ローカル行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetLocalMatrix() const;
		//--------------------------------------------------------------------------------------
		//	const Quaternion& GetLocalQuaternion() const;
		/*!
		@breaf ローカル回転を得る
		@param なし
		@return	ローカル回転（クオータニオン）
		*/
		//--------------------------------------------------------------------------------------
		const Quaternion& GetLocalQuaternion() const;
		//--------------------------------------------------------------------------------------
		//	Vector3 GetLocalRotation() const;
		/*!
		@breaf ローカル回転を得る
		@param なし
		@return	ローカル回転（ベクトル）
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetLocalRotation() const;
		//--------------------------------------------------------------------------------------
		//	const Vector3& GetLocalPosition() const;
		/*!
		@breaf ローカル位置を得る
		@param なし
		@return	ローカル位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetLocalPosition() const;

		//--------------------------------------------------------------------------------------
		//	shared_ptr<GameObject> GetParent() const;
		/*!
		@breaf 親を得る
		@param なし
		@return	親ゲームオブジェクトのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetParent() const;
		//--------------------------------------------------------------------------------------
		//	void SetParent(
		//		const shared_ptr<GameObject>& Ptr	//親ポインタ
		//	);
		/*!
		@breaf 親を設定する
		@param const shared_ptr<GameObject>& Ptr	親ポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetParent(const shared_ptr<GameObject>& Ptr);
		//--------------------------------------------------------------------------------------
		//	void ClearParent();
		/*!
		@breaf 親をクリアする。SetParent()にnullptrを渡すのと同じ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearParent();
		//操作
		//--------------------------------------------------------------------------------------
		//	void CalcMatrix();
		/*!
		@breaf 行列計算をする
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CalcMatrix();
		//--------------------------------------------------------------------------------------
		//	void SetToBefore();
		/*!
		@breaf 情報を一つ前に移す<br />
		各情報はターンごとに1つ前のターン時の情報を持つ<br />
		この関数はその処理を行う
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetToBefore();
		//--------------------------------------------------------------------------------------
		//	void LerpBeforeToNow(
		//		float LerpTime	//補間時間
		//	);
		/*!
		@breaf 1つ前のターンからの指定時間に情報をセットする<br />
		衝突した場合、オブジェクトは前回のターン位置から衝突位置まで戻る必要がある<br />
		この関数はその処理を行う
		@param float LerpTime	補間時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void LerpBeforeToNow(float LerpTime);
		//--------------------------------------------------------------------------------------
		//	virtual void Update()override;
		/*!
		@breaf 更新処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update()override;
		//--------------------------------------------------------------------------------------
		//	virtual void Draw()override;
		/*!
		@breaf 描画処理。デフォルトは何も行わない
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	用途: アクション系コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class ActionComponent : public Component{
	protected:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	explicit ActionComponent(
		//		const shared_ptr<GameObject>& GameObjectPtr	//このコンポーネントを所持するゲームオブジェクト
		//	);
		/*!
		@breaf コンストクタ
		@param const shared_ptr<GameObject>& GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit ActionComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ActionComponent();
		void SetRun(bool b);
		void SetArrived(bool b);
		//NowTimeを加算して、TotalTimeと比較する
		bool AdditionalNowTime();
	public:
		//アクセサ
		bool IsRun()const;
		bool GetRun()const;

		bool IsArrived()const;
		bool GetArrived()const;

		virtual float GetTotalTime() const;
		virtual void SetTotalTime(float f);

		virtual float GetNowTime() const;
		virtual void SetNowTime(float f);


		//操作（純粋仮想関数）
		virtual void Run() = 0;
		virtual void Stop();
		virtual void ReStart();
		//Drawは基本的に行わない
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	用途: アクションのインターバル
	//--------------------------------------------------------------------------------------
	class ActionInterval : public  ActionComponent{
	public:
		//構築と破棄
		explicit ActionInterval(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ActionInterval();
		void SetParams(float TotalTime);

		virtual void Run()override;
		virtual void Update()override;
	};



	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public ActionComponent ;
	//	用途: 拡大縮小アクションの親クラス
	//--------------------------------------------------------------------------------------
	class ScaleComponent : public ActionComponent{
	protected:
		//構築と破棄
		explicit ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleComponent();
	public:

		//アクセサ
		Lerp::rate GetRate() const;
		void SetRate(const Lerp::rate r);

		const Vector3& GetStartScale() const;
		void SetStartScale(const Vector3& StartScale);
		void SetStartScale(float x, float y, float z);

		const Vector3& GetTargetScale() const;
		void SetTargetScale(const Vector3& TargetPosition);
		void SetTargetScale(float x, float y, float z);

		//現在スケールを計算して返す
		//タイムの更新は行わないので
		//Update後に呼ぶべき
		Vector3 GetNowScale() const;

		//操作
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	用途: 目的の拡大率に拡大縮小
	//--------------------------------------------------------------------------------------
	class ScaleTo : public  ScaleComponent{
	public:
		//構築と破棄
		explicit ScaleTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleTo();
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	用途: 目的の相対スケールにに拡大縮小
	//--------------------------------------------------------------------------------------
	class ScaleBy : public  ScaleComponent{
	public:
		//構築と破棄
		explicit ScaleBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ScaleBy();
		//アクセサ
		const Vector3& GetLocalScale() const;
		void SetLocalScale(const Vector3& LocalScale);
		void SetLocalScale(float x, float y, float z);
		void SetParams(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		//操作
		virtual void Run()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	用途: 回転アクションの親クラス
	//--------------------------------------------------------------------------------------
	class RotateComponent : public ActionComponent{
	protected:
		//構築と破棄
		explicit RotateComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateComponent();
	public:

		//アクセサ
		const Quaternion& GetStartQuaternion() const;
		void SetStartQuaternion(const Quaternion& StartQuaternion);

		Vector3 GetStartRotate() const;
		void SetStartRotate(const Vector3& StartRotate);
		void SetStartRotate(float x, float y, float z);

		const Quaternion& GetTargetQuaternion() const;
		void SetTargetQuaternion(const Quaternion& TargetQuaternion);

		Vector3 GetTargetRotate() const;
		void SetTargetRotate(const Vector3& TargetRotate);
		void SetTargetRotate(float x, float y, float z);

		//現在回転を計算して返す
		//タイムの更新は行わないので
		//Update後に呼ぶべき
		Vector3 GetNowRotate() const;
		Quaternion GetNowQuaternion() const;

		//操作
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class RotateTo : public RotateComponent;
	//	用途: 目的の角度に回転
	//--------------------------------------------------------------------------------------
	class RotateTo : public  RotateComponent{
	public:
		//構築と破棄
		explicit RotateTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateTo();
		void SetParams(float TotalTime, const Vector3& Target);
		void SetParams(float TotalTime, const Quaternion& TargetQuaternion);
	};

	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	用途: 目的の相対角度に回転
	//--------------------------------------------------------------------------------------
	class RotateBy : public  RotateComponent{
	public:
		//構築と破棄
		explicit RotateBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~RotateBy();
		//アクセサ
		const Quaternion& GetLocalQuaternion() const;
		void SetLocalQuaternion(const Quaternion& LocalQuaternion);
		Vector3 GetLocalRotate() const;
		void SetLocalRotate(const Vector3& LocalRotate);
		void SetLocalRotate(float x, float y, float z);
		void SetParams(float TotalTime, const Vector3& LocalRotate);
		void SetParams(float TotalTime, const Quaternion& LocalQuaternion);
		//操作
		virtual void Run()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	用途: 移動アクションの親クラス
	//--------------------------------------------------------------------------------------
	class MoveComponent : public ActionComponent{
		void CalcVelocity();
		Vector3 CalcVelocitySub(float NowTime);
	protected:
		//構築と破棄
		explicit MoveComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveComponent();
	public:

		//アクセサ
		Lerp::rate GetRateX() const;
		Lerp::rate GetRateY() const;
		Lerp::rate GetRateZ() const;
		void SetRateX(const Lerp::rate r);
		void SetRateY(const Lerp::rate r);
		void SetRateZ(const Lerp::rate r);
		void SetRateAll(const Lerp::rate r);

		const Vector3& GetStartPosition() const;
		void SetStartPosition(const Vector3& StartPosition);
		void SetStartPosition(float x, float y, float z);

		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& TargetPosition);
		void SetTargetPosition(float x, float y, float z);
		//現在の速度
		Vector3 GetVelocity();


		//現在位置を計算して返す
		//タイムの更新は行わないので
		//Update後に呼ぶべき
		Vector3 GetNowPosition() const;

		//操作
		virtual void Run()override;
		virtual void Update()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	用途: 目的の位置に移動
	//--------------------------------------------------------------------------------------
	class MoveTo : public  MoveComponent{
	public:
		//構築と破棄
		explicit MoveTo(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveTo();
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		void SetParams(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	用途: 目的の相対位置に移動
	//--------------------------------------------------------------------------------------
	class MoveBy : public  MoveComponent{
	public:
		//構築と破棄
		explicit MoveBy(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MoveBy();
		//アクセサ
		const Vector3& GetLocalVector() const;
		void SetLocalVector(const Vector3& LocalVector);
		void SetLocalVector(float x, float y, float z);

		void SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		void SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//操作
		virtual void Run()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	用途: 自動位置コントロール
	//--------------------------------------------------------------------------------------
	class Action : public Component{
		void RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		bool UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
	public:
		//構築と破棄
		explicit Action(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Action();

		//アクセサ
		bool IsLooped()const;
		bool GetLooped()const;
		void SetLooped(bool b);

		bool IsArrived()const;
		bool GetArrived()const;

		//現在の速度
		//Moveコンポーネントがない場合はVector3(0,0,0)を返す
		Vector3 GetVelocity();


		size_t GetScaleActiveIndex() const;
		size_t GetRotateActiveIndex() const;
		size_t GetMoveActiveIndex() const;

		shared_ptr<ScaleTo> AddScaleTo(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<ScaleBy> AddScaleBy(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<ActionInterval> AddScaleInterval(float TotalTime);

		shared_ptr<ActionComponent> GetScaleComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GeScale(size_t TargetIndex){
			auto Ptr = GetScaleComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					L"if(!Ptr)",
					L"Action::GetScale()"
					);
			}
			return Ptr2;
		}


		shared_ptr<RotateTo> AddRotateTo(float TotalTime, const Vector3& Target);
		shared_ptr<RotateTo> AddRotateTo(float TotalTime, const Quaternion& TargetQuaternion);
		shared_ptr<RotateBy> AddRotateBy(float TotalTime, const Vector3& LocalRotate);
		shared_ptr<RotateBy> AddRotateBy(float TotalTime, const Quaternion& LocalQuaternion);
		shared_ptr<ActionInterval> AddRotateInterval(float TotalTime);

		shared_ptr<ActionComponent> GetRotateComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GeRotate(size_t TargetIndex){
			auto Ptr = GetRotateComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					L"if(!Ptr)",
					L"Action::GetRotate()"
					);
			}
			return Ptr2;
		}


		shared_ptr<MoveTo> AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<MoveTo> AddMoveTo(float TotalTime, const Vector3& Target, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		shared_ptr<MoveBy> AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		shared_ptr<MoveBy> AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);

		shared_ptr<ActionInterval> AddMoveInterval(float TotalTime);

		shared_ptr<ActionComponent> GetMoveComponent(size_t TargetIndex);
		template<typename T>
		shared_ptr<T> GetMove(size_t TargetIndex){
			auto Ptr = GetMoveComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2){
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					L"if(!Ptr)",
					L"Action::GetMove()"
					);
			}
			return Ptr2;
		}

		//すべてのアクションをクリアする
		void AllActionClear();

		//操作
		virtual void Run();
		virtual void Stop();
		virtual void ReStart();

		virtual void Update()override;
		virtual void Draw()override{}

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	用途: カメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Camera : public Object, public ShapeInterface{
	public:
		//構築と破棄
		explicit Camera();
		virtual ~Camera();
		//アクセサ
		const Vector3& GetEye() const;
		void SetEye(const Vector3& Eye);
		void SetEye(float x, float y, float z);

		const Vector3& GetAt() const;
		void SetAt(const Vector3& At);
		void SetAt(float x, float y, float z);

		const Vector3& GetUp() const;
		void SetUp(const Vector3& Up);
		void SetUp(float x, float y, float z);

		bool IsPers()const;
		bool GetPers()const;
		void SetPers(bool b);

		float GetFovY() const;
		void SetFovY(float f);

		float GetAspect() const;
		void SetAspect(float f);

		float GetWidth() const;
		void SetWidth(float f);

		float GetHeight() const;
		void SetHeight(float f);

		float GetNear() const;
		void SetNear(float f);

		float GetFar() const;
		void SetFar(float f);

		shared_ptr<GameObject> GetCameraObject() const;
		void SetCameraObject(const shared_ptr<GameObject>& Obj);
		void ClearCameraObject(const shared_ptr<GameObject>& Obj);


		void SetViewPort(const D3D11_VIEWPORT& v);

		const Matrix4X4& GetViewMatrix() const;
		const Matrix4X4& GetProjMatrix() const;
		//操作
		virtual void Update()override;
		virtual void Update2(){}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	用途: LookAtカメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class LookAtCamera : public Camera{
	public:
		//構築と破棄
		explicit LookAtCamera();
		virtual ~LookAtCamera();
		//アクセサ
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);

		//操作
		virtual void Update() override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public ShapeInterface ;
	//	用途: ライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Light : public Object, public ShapeInterface{
	public:
		explicit Light();
		Light(
			const Vector3& Directional,
			const Color4& DiffuseColor,
			const Color4& SpecularColor
			);

		virtual ~Light();
		//アクセサ
		const Vector3& GetDirectional() const;
		void SetDirectional(const Vector3& Directional);
		void SetDirectional(float x, float y, float z);
		void SetPositionToDirectional(const Vector3& Position);
		void SetPositionToDirectional(float x, float y, float z);

		const Color4& GetDiffuseColor()const;
		void SetDiffuseColor(const Color4& col);
		void SetDiffuseColor(float r, float g, float b, float a);

		const Color4& GetSpecularColor() const;
		void SetSpecularColor(const Color4& col);
		void SetSpecularColor(float r, float g, float b, float a);

		virtual void Update()override{}
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public ShapeInterface ;
	//	用途: マルチライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class MultiLight : public Object, public ShapeInterface{
	public:
		//構築と破棄
		explicit MultiLight();
		virtual ~MultiLight();
		//アクセサ
		//Lightの数
		size_t GetLightCount() const;
		//Lightの取得
		shared_ptr<Light> GetLight(size_t Index)const;
		//Lightのセット
		void SetLight(size_t Index, shared_ptr<Light>& Ptr);
		//配列の参照
		vector< shared_ptr<Light> >& GetLightVec();
		//Lightの追加
		shared_ptr<Light> AddLight();
		//Lightの削除
		void RemoveLight(size_t Index);
		//デフォルトのライティングの設定
		void SetDefaultLighting();

		//操作
		virtual void Update()override;
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public ShapeInterface;
	//	用途: ビュークラス（ビューポートとライト、カメラを管理、コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class View : public Object, public ShapeInterface{
		//テンプレートから呼ばれるサブ関数
		void ResetParamatersSub(const D3D11_VIEWPORT& vp,const Color4& Col, 
			const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr);
	public:
		//構築と破棄
		View();
		virtual ~View();
		//アクセサ
		const D3D11_VIEWPORT& GetViewPort()const;
		const D3D11_VIEWPORT* GetViewPortPtr()const;
		void SetViewPort(const D3D11_VIEWPORT& v);
		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& c);
		shared_ptr<Camera> GetCamera() const;
		void SetCamera(shared_ptr<Camera>& Ptr);

		shared_ptr<MultiLight> GetMultiLight() const;
		void SetMultiLight(shared_ptr<MultiLight>& Lights);

		shared_ptr<MultiLight> SetDefaultLighting();


		//操作
		shared_ptr<Light> AddLight();

		template<typename CameraType,typename LightType>
		void ResetParamaters(const Rect2D<float>& ViewPortSize, Color4& Col, size_t LightCount, float MinDepth = 0.0f, float MaxDepth = 1.0f){
			D3D11_VIEWPORT vp;
			ZeroMemory(&vp, sizeof(vp));
			vp.MinDepth = MinDepth;
			vp.MaxDepth = MaxDepth;
			vp.TopLeftX = ViewPortSize.left;
			vp.TopLeftY = ViewPortSize.top;
			vp.Width = ViewPortSize.Width();
			vp.Height = ViewPortSize.Height();
			//カメラを差し替える
			auto CameraPtr = Object::CreateObject<CameraType>();
			//ライトを差し替える
			auto MultiLightPtr = Object::CreateObject<LightType>();
			for (size_t i = 0; i < LightCount; i++){
				MultiLightPtr->AddLight();
			}
			//サブ関数を使って設定
			ResetParamatersSub(vp, Col, 
				dynamic_pointer_cast<Camera>(CameraPtr), 
				dynamic_pointer_cast<MultiLight>(MultiLightPtr));
		}
		//操作
		virtual void Update()override;
		virtual void Update2()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class MultiView: public Component;
	//	用途: マルチビューコンポーネント
	//--------------------------------------------------------------------------------------
	class MultiView : public Component{
	public:
		//構築と破棄
		explicit MultiView(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MultiView();
		//アクセサ
		size_t GetViewCount() const;
		shared_ptr<View> GetView(size_t Index) const;
		//ViewVecを直接操作する時用
		vector< shared_ptr<View> >& GetViewVec();
		//操作
		shared_ptr<View> AddView();
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PlayMusic: public Component;
	//	用途: ミュージックコンポーネント
	//--------------------------------------------------------------------------------------
	class PlayMusic : public Component{
	public:
		//構築と破棄
		PlayMusic(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey);
		virtual ~PlayMusic();
		//アクセサ
		shared_ptr<AudioResource> GetAudioResource() const;
		//操作
		void Start(size_t LoopCount = 0, float Volume = 1.0f);
		void Start(const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop();
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SoundEffect: public Component;
	//	用途: サウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	class SoundEffect : public Component{
	public:
		//構築と破棄
		SoundEffect(const shared_ptr<GameObject>& GameObjectPtr,const wstring& ResKey);
		virtual ~SoundEffect();
		//アクセサ
		shared_ptr<AudioResource> GetAudioResource() const;
		//操作
		void Start(size_t LoopCount = 0,float Volume = 1.0f);
		void Start(const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop();
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct SoundItem{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	用途: マルチサウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	class MultiSoundEffect : public Component{
	public:
		//構築と破棄
		MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MultiSoundEffect();
		//アクセサ
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey,bool ExceptionActive = true) const;
		//操作
		shared_ptr<AudioResource> AddAudioResource(const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//endof  basedx11
