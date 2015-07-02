/*!
@file HttpForm.h
@brief Http��GET/POST�AForm�I�u�W�F�N�g

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class HttpRequest : public Object;
	//	�p�r: Http�v���̃J�v�Z���N���X
	//--------------------------------------------------------------------------------------
	class HttpRequest : public Object{
	public:
		//�\�z�Ɣj��
		explicit HttpRequest();
		virtual ~HttpRequest();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

}
//end basedx11
