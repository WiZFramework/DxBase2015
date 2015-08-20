#pragma once

#include "StdAfx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class XmlDocReader;
	//	�p�r: XML�ǂݍ��݃I�����[�N���X
	//--------------------------------------------------------------------------------------
	class XmlDocReader {
	protected:
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
	public:
		//�\�z�Ə���
		XmlDocReader(const wstring& StrReadContext, bool IsMemory = false);
		virtual ~XmlDocReader();
		//����
		//XPath�ɂ��擾
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;

		//Node������擾����static�֐�
		static long GetLength(const IXMLDOMNodeListPtr& NodeList);
		static IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index);
		static IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node);
		static wstring GetBaseName(const IXMLDOMNodePtr& Node);
		static wstring GetText(const IXMLDOMNodePtr& Node);
		static wstring GetXML(const IXMLDOMNodePtr& Node);
		static wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag);
		static IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
		static IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
	};



	//--------------------------------------------------------------------------------------
	//	class XmlDoc;
	//	�p�r: XML�N���X
	//--------------------------------------------------------------------------------------
	class XmlDoc{
		void CheckXmlDoc() const;
		IXMLDOMNodePtr CreateNodeBase(const CComBSTR& NodeName);
		IXMLDOMNodePtr AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode);
	public:
		//�\�z�Ə���
		XmlDoc(const wstring& Filename);
		virtual ~XmlDoc();
		//����
		void Save(const wstring& Filename, bool Exp = true);
		//XPath�ɂ��擾
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;

		//Node������擾����static�֐�
		long GetLength(const IXMLDOMNodeListPtr& NodeList)const;
		IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index) const;
		IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node)const;
		wstring GetBaseName(const IXMLDOMNodePtr& Node) const;
		wstring GetText(const IXMLDOMNodePtr& Node) const;
		void SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text);
		//�m�[�h��XML�ւ̃A�N�Z�X�̓��[�h�I�����[
		wstring GetXML(const IXMLDOMNodePtr& Node) const;

		wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag)const;
		void SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue);

		IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;

		IXMLDOMNodePtr AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		XmlDoc(const XmlDoc&) = delete;
		XmlDoc& operator=(const XmlDoc&) = delete;
		//���[�u�֎~
		XmlDoc(const XmlDoc&&) = delete;
		XmlDoc& operator=(const XmlDoc&&) = delete;
	};



}
//endof  basedx11

