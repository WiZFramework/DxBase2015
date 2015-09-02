
/*!
@file stdafx.h
@brief プリコンパイル済みヘッダ作成用

*/

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <wrl.h>

#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <atlbase.h>
#include <commctrl.h>
#include <shellapi.h>
#include <limits.h>
#include <time.h>

#include <xaudio2.h>	//サウンド
#include <xaudio2fx.h>
#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <msxml6.h>



// STL
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <new>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <typeindex>

using namespace std;

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>
#include <wincodec.h>


#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXTex.h>
// XInput includes
#include <xinput.h>


#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
//DirectXTx
#pragma comment( lib, "DirectXTex.lib" )

// XInput
#pragma comment( lib, "XInput.lib" )
//オーディオ
#pragma comment( lib, "xaudio2.lib" )
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")

#pragma warning(disable:4192)
#import "msxml6.dll" named_guids raw_interfaces_only
#pragma warning(default:4192)


using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;

//ユーティリティ基本クラス（削除テンプレート、例外処理など）
#include "BaseHelper.h"
//２D用の基本クラス
#include "Prim2D.h"
//ベクトル計算の計算クラス
#include "MathVector.h"
//行列、クオータニオン、カラーなどの計算クラス
#include "MathExt.h"
//ベクトルのスタティック計算
#include "MathVectorEX.h"
//行列、クオータニオン、カラーなどのスタティック計算
#include "MathExtEX.h"
//衝突判定、補間処理用ユーティリティ
#include "TransHelper.h"
//頂点定義、プリミティブ作成関数等
#include "VertexHelper.h"
//XML
#include "XmlDoc.h"
//DirectXデバイス
#include "DeviceResources.h"
//レンダリングターゲット
#include "Renderer.h"
//シェーダー
#include "Shader.h"
//アプリケーションクラス
#include "App.h"
//オブジェクト基底クラス
#include "Object.h"

