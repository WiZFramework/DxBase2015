
/*!
@file stdafx.h
@brief �v���R���p�C���ς݃w�b�_�쐬�p

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.<br />

DxBase2015�́A�ȉ��̃c�[���A�T���v���A���C�u�������̈ꕔ���g�p�܂��́A�ė��p���Ă��܂��B<br />
DirectXTK�ADirectXTX�ADirectX�T���v��: �}�C�N���\�t�g�Зl�EMicrosoft Public License(MS-PL)<br />
SpriteStudio5-SDK: ������ЃE�F�u�e�N�m���W�l(Web Technology Corp.)<br />
FBXSDK: �I�[�g�f�X�N������Зl(Autodesk, Inc.)
*/

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>

// C �����^�C�� �w�b�_�[ �t�@�C��
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wrl.h>

#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <Winhttp.h>


#include <xaudio2.h>	//�T�E���h
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
#include <functional>
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
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXTex.h>
#define FBXSDK_NEW_API
#include <fbxsdk.h>

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
//FBX
#pragma comment(lib, "libfbxsdk-mt.lib")
// XInput
#pragma comment( lib, "XInput.lib" )
//�I�[�f�B�I
#pragma comment( lib, "xaudio2.lib" )
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
//WinHTTP
#pragma comment( lib, "Winhttp.lib" )


#pragma warning(disable:4192)
#import "msxml6.dll" named_guids raw_interfaces_only
#pragma warning(default:4192)



namespace DirectX
{
#if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
#define XM_CALLCONV __fastcall
	typedef const XMVECTOR& HXMVECTOR;
	typedef const XMMATRIX& FXMMATRIX;
#endif
}

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;


//���[�e�B���e�B��{�N���X�i�폜�e���v���[�g�A��O�����Ȃǁj
#include "BaseHelper.h"
//�QD�p�̊�{�N���X
#include "Prim2D.h"
//�x�N�g���v�Z�̌v�Z�N���X
#include "MathVector.h"
//�s��A�N�I�[�^�j�I���A�J���[�Ȃǂ̌v�Z�N���X
#include "MathExt.h"
//�x�N�g���̃X�^�e�B�b�N�v�Z
#include "MathVectorEX.h"
//�s��A�N�I�[�^�j�I���A�J���[�Ȃǂ̃X�^�e�B�b�N�v�Z
#include "MathExtEX.h"
//�Փ˔���A��ԏ����p���[�e�B���e�B
#include "TransHelper.h"
//���_��`�A�v���~�e�B�u�쐬�֐���
#include "VertexHelper.h"
//���Ǌ֘A
#include "Steering.h"
//XML
#include "XmlDoc.h"

#include "DeviceResources.h"
#include "Renderer.h"
#include "App.h"
#include "Object.h"
#include "StateMachine.h"
#include "Shader.h"
#include "ShaderBasic.h"
#include "Components.h"
#include "SpriteComponents.h"
#include "Rigidbody.h"
#include "DrawComponents.h"
#include "BaseResource.h"
#include "SceneBase.h"
#include "HttpForm.h"
#include "SpriteStudio5.h"

