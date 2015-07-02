/*!
@file HttpForm.h
@brief HttpのGET/POST、Formオブジェクト

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class HttpRequest : public Object;
	//	用途: Http要求のカプセルクラス
	//--------------------------------------------------------------------------------------
	class HttpRequest : public Object{
	public:
		//構築と破棄
		explicit HttpRequest();
		virtual ~HttpRequest();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

}
//end basedx11
