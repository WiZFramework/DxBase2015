#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct HttpRequest::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct HttpRequest::Impl{
		Impl() {}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class HttpRequest : public Object;
	//	�p�r: Http�v���̃J�v�Z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HttpRequest::HttpRequest() :
		Object(),
		pImpl(new Impl())
	{}
	HttpRequest::~HttpRequest(){}



}
//end basedx11
