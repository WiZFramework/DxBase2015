#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Labyrinth;
	//	用途: 迷路クラス
	//--------------------------------------------------------------------------------------
	class Labyrinth{
		//各定数
		static int m_Dirtable[24][4];	//方向表
		static int m_Dx[4];				//変位ベクトル
		static int m_Dy[4];				//変位ベクトル
		//迷路のサイズ関連
		int m_MaxX;			//迷路の横サイズ（偶数）
		int m_MaxY;			//迷路の縦サイズ（偶数）
		const int m_MaxSite;		//最大サイト数
		//動的に生成される配列関連
		vector< vector<int> > m_Map;
		//二次元配列。イニシャライザ注意
		vector<int> m_XX;
		vector<int> m_YY;
		int m_Nsite;	//登録サイト数
		//--------------------------------------------------------------------------------------
		//	void Add(
		//	int i,		//X軸
		//	int j		//Y軸
		//	);
		//	用途: サブ関数、サイトに加える
		//	戻り値: なし。
		//--------------------------------------------------------------------------------------
		void Add(int i, int j);
		//--------------------------------------------------------------------------------------
		//	int Select(
		//	int* i,	//X軸
		//	int* j		//Y軸
		//	);
		//	用途: サブ関数、サイトを乱数で選ぶ
		//	戻り値: サイトの番号。サイトが尽きたら0
		//--------------------------------------------------------------------------------------
		int Select(int* i, int* j);
	public:
		//--------------------------------------------------------------------------------------
		//	Labyrinth(
		//	DWORD  MaxX,		//横軸の大きさ（偶数）
		//	DWORD  MaxY		//縦軸の大きさ（偶数）
		//	);
		//	用途: コンストラクタ
		//	戻り値: なし（失敗時は例外をthrow）
		//--------------------------------------------------------------------------------------
		Labyrinth(DWORD MaxX, DWORD MaxY);
		//--------------------------------------------------------------------------------------
		//	~Labyrinth();
		//	用途: デストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		~Labyrinth(){
			//何もしない。配列はvectorが開放する
		}
		//--------------------------------------------------------------------------------------
		//	vector< vector<int> >& getMap();
		//	用途: 迷路のマップを得る
		//	戻り値: 迷路のマップ（サイズは呼び出す側で注意して使用する）
		//--------------------------------------------------------------------------------------
		vector< vector<int> >& getMap(){
			return m_Map;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//ピースの大きさ
		const Vector3 m_Piece;
		//迷路のX方向とZ方向サイズ（6以上の偶数にする）
		int m_MapSizeX;
		int m_MapSizeZ;
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//プレートの作成
		void CreatePlate();
		//固定のボックスの作成
		void CreateFixedBox();
		//データマップをXZ入れ替えする
		void ChangeVecXZ(const vector< vector<int> >& SrcMap, vector< vector<int> >& DestMap);
		//固定ボックスをAddする
		void SetFixedBox(vector< vector<int> >& DataMap, bool XLine);
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :
			Stage(), m_Piece(2.0f, 1.0f, 2.0f),
			m_MapSizeX(30),
			m_MapSizeZ(30)
		{}
		virtual ~GameStage(){}
		//アクセサ
		const Vector3& GetPiece() const{
			return m_Piece;
		}
		//初期化
		virtual void Create()override;
	};
}
//endof  basedx11
