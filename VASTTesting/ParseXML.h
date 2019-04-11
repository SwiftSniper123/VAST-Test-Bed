#pragma once

#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>
#include "AV.h"
#include "Environment.h"

class XMLParser
{
public:
	XMLParser(LPCWSTR configFile);
	HRESULT WriteAttributes(IXmlReader* pReader);
	int __cdecl Parse();

private:
	LPCWSTR _file;
	void ParseModule(HRESULT hr,
					IStream *pFileStream,
					IXmlReader *pReader,
					XmlNodeType nodeType,
					const WCHAR* pwszLocalName);
};

class Configuration
{
public:
	Configuration();

private:
	AV **_AV;
	Environment *_Env;
	
};