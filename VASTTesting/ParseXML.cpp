#include "ParseXML.h"


Configuration::Configuration() {};

/*Configuration Configuration::LoadConfig(string filelocation)
{

};*/

XMLParser::XMLParser(string file)
{
	_file = file;
}

string indent(int level) {
	string s;
	for (int i = 0; i < level; i++) s += "  ";
	return s;
}

void printTree(ptree &pt, int level) {
	if (pt.empty()) {
		cerr << "\"" << pt.data() << "\"";
	}

	else {
		if (level) cerr << endl;

		cerr << indent(level) << "{" << endl;
		//std::cout << "Test";
		for (ptree::iterator pos = pt.begin(); pos != pt.end();) {
			cerr << indent(level + 1) << "\"" << pos->first << "\": ";

			printTree(pos->second, level + 1);
			++pos;
			if (pos != pt.end()) {
				cerr << ",";
			}
			cerr << endl;
		}

		cerr << indent(level) << " }";
	}

	return;
}

void XMLParser::Parse()
{
	read_xml(_file, pt1);
	//printTree(pt1, 0);

	//std::cout << "\n\n" << pt1.get<std::string>("VAST.module.map.pair.key");
	
	//std::cout << pt1.size;
	//std::cout << pt1.get_child("VAST.module");

	BOOST_FOREACH(ptree::value_type const& node, pt1.get_child("VAST"))
	{
		//cerr << node.first.data. << "\n";
		boost::property_tree::ptree subtree = node.second;

		if (node.first == "module")
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type const& map, subtree.get_child("map"))
			{
				boost::property_tree::ptree subMap = map.second;

				BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair, subMap.get_child(""))
				{
					boost::property_tree::ptree key = pair.second;

					BOOST_FOREACH(boost::property_tree::ptree::value_type const& value, key.get_child(""))
					{
						std::string label = value.first;
						boost::property_tree::ptree keyValue = value.second;

						std::string test = keyValue.get<std::string>("<xmlattr>.key");
						std::cout << test;
						/*
						if (label != "<xmlattr>")
						{
							//std::cout << pair.first << "\n";
							std::cout << value.first;
							//std::string key = keyValue.get<std::string>("type.key");
						}*/
					}
				}
				//std::string value = test.get<std::string>("pair.value");

				//std::cout << "key: " << key << "value: " << value;

				//std::string label = v.second;
				//std::cout << label;
				
				/*if (label != "<xmlattr>")
				{
					std::string value = subtree.get<std::string>(label);
					std::cout << label << ":  " << value << std::endl;
				}*/
			}
			//std::cout << std::endl;
		}
	}//
	
}



//// Definitions for the Public methods
//bool CSV_Parser::parse_line(const STR& input_line, CSV_FIELDS& output_fields)
//{
//	/*
//		A public method which accepts the following arguments
//		a. A string
//		b. A vector of strings
//		Parse the CSV line and populate the vector with the output
//	*/
//	bool status;
//	status = parse(input_line, output_fields);
//	return status;
//}
//
//bool CSV_Parser::parse_line(const STR& input_line, CSV_FIELDS& header_fields, KEY_VAL_FIELDS& key_val)
//{
//	/*
//		A public method which accepts the following arguments
//		a. A string
//		b. A vector of strings
//		c. A map with key and value both being strings
//		Parse the CSV line, use the header provided in the vector to populate the map
//	*/
//	bool status;
//	CSV_FIELDS output_fields;
//	status = parse(input_line, output_fields);
//
//	if (status == true && output_fields.size() == header_fields.size())
//	{
//		VECTOR_ITR it1 = output_fields.begin();
//		VECTOR_ITR it2 = header_fields.begin();
//		for (; it1 != output_fields.end(); ++it1, ++it2)
//		{
//			key_val.insert(MAP_ENTRY(*it2, *it1));
//		}
//	}
//	return status;
//}
//
//bool CSV_Parser::parse(const STR& input_line, CSV_FIELDS& output_fields)
//{
//	/*
//		A private method which handles the parsing logic used by both the overloaded public methods
//	*/
//	STR field;
//	int i, j;
//
//	if (input_line.length() == 0)
//	{
//		return false;
//	}
//
//	i = 0;
//	do
//	{
//		if (i < input_line.length() && input_line[i] == CSV_QUOTE)
//		{
//			j = parse_quoted_fields(input_line, field, ++i);
//		}
//		else
//		{
//			j = parse_normal_fields(input_line, field, i);
//		}
//		output_fields.push_back(field);
//		i = j + 1;
//	} while (j < input_line.length());
//
//	return true;
//}
//
//int CSV_Parser::parse_normal_fields(const STR& input_line, STR& field, int& i)
//{
//	/*
//		Normal fields are the ones which contain no escaped or quoted characters
//		For instance - Consider that input_line is - 1997,Ford,E350,"Super, luxurious truck"
//		An example for a normal field would be - Ford
//	*/
//	int j;
//	j = input_line.find_first_of(CSV_DELIMITER, i);
//	if (j > input_line.length())
//	{
//		j = input_line.length();
//	}
//	field = std::string(input_line, i, j - i);
//	return j;
//}
//
//int CSV_Parser::parse_quoted_fields(const STR& input_line, STR& field, int& i)
//{
//	/*
//		Quoted fields are the ones which are enclosed within quotes
//		For instance - Consider that input_line is - 1997,Ford,E350,"Super, luxurious truck"
//		An example for a quoted field would be - Super luxurious truck
//		Another instance being - 1997,Ford,E350,"Super, ""luxurious"" truck"
//	*/
//	int j;
//	field = "";
//
//	for (j = i; j < input_line.length(); j++)
//	{
//		if (input_line[j] == '"' && input_line[++j] != '"')
//		{
//			int k = input_line.find_first_of(CSV_DELIMITER, j);
//			if (k > input_line.length())
//			{
//				k = input_line.length();
//			}
//			for (k -= j; k-- > 0; )
//			{
//				field += input_line[j++];
//			}
//			break;
//		}
//		else
//		{
//			field += input_line[j];
//		}
//	}
//	return j;
//}

/*
HRESULT XMLParser::WriteAttributes(IXmlReader* pReader)	
{
    const WCHAR* pwszPrefix;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
    HRESULT hr = pReader->MoveToFirstAttribute();

    if (S_FALSE == hr)
        return hr;
    if (S_OK != hr)
    {
        wprintf(L"Error moving to first attribute, error is %08.8lx", hr);
        return hr;
    }
    else
    {
        while (TRUE)
        {
            if (!pReader->IsDefault())
            {
                UINT cwchPrefix;
                if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
                {
                    wprintf(L"Error getting prefix, error is %08.8lx", hr);
                    return hr;
                }
                if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
                {
                    wprintf(L"Error getting local name, error is %08.8lx", hr);
                    return hr;
                }
                if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
                {
                    wprintf(L"Error getting value, error is %08.8lx", hr);
                    return hr;
                }
				if(wcscmp(pwszLocalName,L"module")==0)
				{ 
					if (wcscmp(pwszValue, L"VAST"))
					{
						ParseModule();
					}
					else if (wcscmp(pwszValue, L"Environment"))
					{

					}
					else if (wcscmp(pwszValue, L"AV"))
					{

					}
				}
                if (cwchPrefix > 0)
                    wprintf(L"Attr: %s:%s=\"%s\" \n", pwszPrefix, pwszLocalName, pwszValue);
                else
                    wprintf(L"Attr: %s=\"%s\" \n", pwszLocalName, pwszValue);
            }

            if (S_OK != pReader->MoveToNextAttribute())
                break;
        }
    }
    return hr;
}

int __cdecl XMLParser::Parse()
{
    HRESULT hr = S_OK;
    IStream *pFileStream = NULL;
    IXmlReader *pReader = NULL;
    XmlNodeType nodeType;
    const WCHAR* pwszPrefix;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
    UINT cwchPrefix;
	
    if (_file == NULL || _file[0] == 0)
    {
        wprintf(L"Usage: XmlLiteReader.exe name-of-input-file\n");
        return 0;
    }

    //Open read-only input stream
    if (FAILED(hr = SHCreateStreamOnFile(_file, STGM_READ, &pFileStream)))
    {
        wprintf(L"Error creating file reader, error is %08.8lx", hr);
        HR(hr);
    }

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
    {
        wprintf(L"Error creating xml reader, error is %08.8lx", hr);
        HR(hr);
    }

    if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        HR(hr);
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        wprintf(L"Error setting input for reader, error is %08.8lx", hr);
        HR(hr);
    }

    //read until there are no more nodes
    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
		// NodeType = Attribute declaration
        case XmlNodeType_XmlDeclaration:
            wprintf(L"XmlDeclaration\n");
			// Call function to enumerate the attribute
            if (FAILED(hr = WriteAttributes(pReader)))
            {
                wprintf(L"Error writing attributes, error is %08.8lx", hr);
                HR(hr);
            }
            break;
		// Nodetype = Element beginning
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
            {
                wprintf(L"Error getting prefix, error is %08.8lx", hr);
                HR(hr);
            }
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                wprintf(L"Error getting local name, error is %08.8lx", hr);
                HR(hr);
            }
			// Check if the element is the beginning of a module
			if (wcscmp(pwszLocalName, L"module") == 0)
			{
				printf("test\n");
				//ParseModule(hr, pFileStream, pReader, nodeType, pwszPrefix, pwszLocalName, pwszValue, cwchPrefix);
			}
            if (cwchPrefix > 0)
                wprintf(L"Element: %s:%s\n", pwszPrefix, pwszLocalName);
            else
                wprintf(L"Element: %s\n", pwszLocalName);

            if (FAILED(hr = WriteAttributes(pReader)))
            {
                wprintf(L"Error writing attributes, error is %08.8lx", hr);
                HR(hr);
            }

            if (pReader->IsEmptyElement() )
                wprintf(L" (empty)");
            break;
        case XmlNodeType_EndElement:
            if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
            {
                wprintf(L"Error getting prefix, error is %08.8lx", hr);
                HR(hr);
            }
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                wprintf(L"Error getting local name, error is %08.8lx", hr);
                HR(hr);
            }
            if (cwchPrefix > 0)
                wprintf(L"End Element: %s:%s\n", pwszPrefix, pwszLocalName);
            else
                wprintf(L"End Element: %s\n", pwszLocalName);
            break;
        case XmlNodeType_Text:
        case XmlNodeType_Whitespace:
            if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
            {
                wprintf(L"Error getting value, error is %08.8lx", hr);
                HR(hr);
            }
            wprintf(L"Text: >%s<\n", pwszValue);
            break;
        case XmlNodeType_CDATA:
            if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
            {
                wprintf(L"Error getting value, error is %08.8lx", hr);
                HR(hr);
            }
            wprintf(L"CDATA: %s\n", pwszValue);
            break;
        case XmlNodeType_ProcessingInstruction:
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                wprintf(L"Error getting name, error is %08.8lx", hr);
                HR(hr);
            }
            if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
            {
                wprintf(L"Error getting value, error is %08.8lx", hr);
                HR(hr);
            }
            wprintf(L"Processing Instruction name:%s value:%s\n", pwszLocalName, pwszValue);
            break;
        case XmlNodeType_Comment:
            if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
            {
                wprintf(L"Error getting value, error is %08.8lx", hr);
                HR(hr);
            }
            wprintf(L"Comment: %s\n", pwszValue);
            break;
        case XmlNodeType_DocumentType:
            wprintf(L"DOCTYPE is not printed\n");
            break;
        }
    }

CleanUp:
    SAFE_RELEASE(pFileStream);
    SAFE_RELEASE(pReader);
    return hr;
}

void XMLParser::ParseModule(HRESULT hr,
							IStream *pFileStream,
							IXmlReader *pReader,
							XmlNodeType nodeType,
							const WCHAR* pwszPrefix,
							const WCHAR* pwszLocalName,
							const WCHAR* pwszValue,
							UINT cwchPrefix)
{
	while (S_OK == (hr = pReader->Read(&nodeType)) || (nodeType != XmlNodeType_EndElement && pwszLocalName != L"module"))
	{
		switch (nodeType)
		{
		case XmlNodeType_XmlDeclaration:
			wprintf(L"XmlDeclaration\n");
			if (FAILED(hr = WriteAttributes(pReader)))
			{
				wprintf(L"Error writing attributes, error is %08.8lx", hr);
				HR(hr);
			}
			break;
		case XmlNodeType_Element:
			if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			{
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting local name, error is %08.8lx", hr);
				HR(hr);
			}
			if (pwszLocalName == L"module")
			{
				//ParseModule(hr, pFileStream, pReader, nodeType, pwszLocalName);
			}
			if (cwchPrefix > 0)
				wprintf(L"Element: %s:%s\n", pwszPrefix, pwszLocalName);
			else
				wprintf(L"Element: %s\n", pwszLocalName);

			if (FAILED(hr = WriteAttributes(pReader)))
			{
				wprintf(L"Error writing attributes, error is %08.8lx", hr);
				HR(hr);
			}

			if (pReader->IsEmptyElement())
				wprintf(L" (empty)");
			break;
		case XmlNodeType_EndElement:
			if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			{
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting local name, error is %08.8lx", hr);
				HR(hr);
			}
			if (cwchPrefix > 0)
				wprintf(L"End Element: %s:%s\n", pwszPrefix, pwszLocalName);
			else
				wprintf(L"End Element: %s\n", pwszLocalName);
			break;
		case XmlNodeType_Text:
		case XmlNodeType_Whitespace:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"Text: >%s<\n", pwszValue);
			break;
		case XmlNodeType_CDATA:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"CDATA: %s\n", pwszValue);
			break;
		case XmlNodeType_ProcessingInstruction:
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting name, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"Processing Instruction name:%s value:%s\n", pwszLocalName, pwszValue);
			break;
		case XmlNodeType_Comment:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"Comment: %s\n", pwszValue);
			break;
		case XmlNodeType_DocumentType:
			wprintf(L"DOCTYPE is not printed\n");
			break;
		}
	}

CleanUp:
	SAFE_RELEASE(pFileStream);
	SAFE_RELEASE(pReader);
	//return hr;
}//*/