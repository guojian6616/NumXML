#include "NumXML.h"

/* --------------------------------- xmlNode -------------------------------- */
xmlNode::xmlNode():
_name(NULL),
_value(NULL),
_parent(NULL),
_next_sibling(NULL),
_prev_sibling(NULL),
_type(XML_UNSET_NODE)
{

}


xmlNode::~xmlNode()
{
	if(_name != NULL)
		delete [] _name;
	if(_value != NULL)
		delete [] _value;
}


char* xmlNode::parse(char* buffer, bool status)
{

}


/* ------------------------------- xmlElement ------------------------------- */
xmlElement::xmlElement()
{

}


xmlElement::~xmlElement()
{

}


char* xmlElement::parse(char* buffer, bool status)
{
	
}

/* --------------------------------- xmlNode -------------------------------- */
xmlText::xmlText()
{

}


xmlText::~xmlText()
{

}


char* xmlText::parse(char* buffer, bool status)
{
	
}

/* ------------------------------- xmlAttribute ----------------------------- */
xmlAttribute::xmlAttribute()
{

}


xmlAttribute::~xmlAttribute()
{

}


char* xmlAttribute::parse(char* buffer, bool status)
{
	
}
/* -------------------------------- xmlComment ------------------------------ */
xmlComment::xmlComment()
{

}


xmlComment::~xmlComment()
{

}


char* xmlComment::parse(char* buffer, bool status)
{
	
}

/* ------------------------------- xmlDocument ------------------------------ */
xmlDocument::xmlDocument(): xmlNode()
{
	_type = XML_DOCUMENT_NODE;
}

xmlDocument::~xmlDocument()
{
	if(_buffer != NULL)
		delete [] _buffer;
}

void xmlDocument::loadXMLDocument(const char* xmldoc)
{
	// 读取文件
	FILE* fin = fopen(xmldoc, "r");
	if(fin == NULL)
		printf("error: open file %s failed.\n", xmldoc);
	fseek(fin, 0, SEEK_END);
	long length = ftell(fin);

	_buffer = new char [length+1];
	fseek(fin, 0, SEEK_SET);
	// rewind(fin);
	fread(_buffer, sizeof(char), length, fin);

	fclose(fin);

	// 删除换行符和TAB
	printf("length: %ld buffer size %lu ", length, strlen(_buffer));

	printf("xml file:\n%s\n", _buffer);

	char* buffer = new char [length+1];

	char* start = buffer;
	char* old_buffer = _buffer;

	while(_buffer != NULL && *_buffer != '\0')
	{
		if(*_buffer != '\n' && *_buffer != '\t')
		{
			*start = *_buffer;
			start++;
		}
		_buffer++;
	}
	*start = '\0';

	/** 有两个指针
	 *  _buffer 预留，用来释放内存，不要修改
	 *   buffer 解析时使用 */
	delete [] old_buffer;
	_buffer = NULL;
	_buffer = buffer;
	printf("--------------------------------------\n%s\n", _buffer);

	// 解析xml
}


char* xmlDocument::skipDeclaration(char* buffer)
{
	// printf("local buffer:\n%s\n\n", buffer);
	if(strncmp(buffer, "<?", 2) == 0)
	{
		printf("start\n");
		buffer = buffer + 2;
		while(strncmp(buffer, "?>", 2) != 0)
		{
			buffer++;
		}
		buffer = buffer + 2;
	}

	return buffer;
}


char* xmlDocument::skipComment(char* buffer)
{
	// printf("local buffer:\n%s\n\n", buffer);
	if(strncmp(buffer, "<!--", 4) == 0)
	{
		printf("start\n");
		buffer = buffer + 3;
		while(strncmp(buffer, "-->", 3) != 0)
		{
			buffer++;
		}
		buffer = buffer + 3;
	}

	return buffer;
}


char* xmlDocument::skipSpace(char* buffer)
{
	while(*buffer == ' ')
	{
		buffer++;
	}
	return buffer;
}


// xmlElement::xmlElement()
// {
// 	_type = XML_ELEMENT_NODE;
// 	_first_child = NULL;
// 	_last_child = NULL;
// 	_first_attribute = NULL;
// 	_last_attribute = NULL;
// }


// xmlNode::xmlNode()
// {

// }