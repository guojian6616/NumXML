#include "NumXML.h"

/* --------------------------------- xmlNode -------------------------------- */
xmlNode::xmlNode():
_name(NULL),
_value(NULL),
_parent(NULL),
_first_child(NULL),
_last_child(NULL),
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


void xmlNode::setType(xml_node_type type)
{
	_type = type;
}


xml_node_type xmlNode::getType()
{
	return _type;
}


char* xmlNode::parse(char* buffer, bool* status)
{
	bool stat = true;
	status = &stat;
	while (buffer != NULL && *buffer != '\0')
	{
		xmlNode* node = NULL;
		buffer = identifyNodeType(buffer, &node);

		if (node == NULL)
			break;

		if (node->getType() == XML_ELEMENT_NODE_END)
		{
			buffer = checkEndTag(buffer, status);
			if (*status)
			{
				printf("error, Node %s tags mismatch\n", _name);
			}
			delete node;
			break;
		}


		buffer = node->parse(buffer, status);

		if (node->getType() == XML_DECLARATION_NODE && !isDocument())
		{
			printf("error, declaration in the subnodes\n");
		}

		appendChild(node);

	}

	return buffer;
}


char* xmlNode::identifyNodeType(char* buffer, xmlNode** node)
{
	static const char xml_header[] = "<?";
	static const char xml_comm_header[] = "<!--";
	static const char xml_element_header[] = "<";
	static const char xml_element_end[] = "</";

	buffer = skipSpace(buffer);

	xmlNode* child = NULL;
	if (strncmp(buffer, xml_comm_header, 4) == 0)
	{
		child = new xmlComment();
		buffer = buffer + 4;
	}
	else if (strncmp(buffer, xml_header, 2) == 0)
	{
		child = new xmlDeclaration();
		buffer = buffer + 2;
	}
	else if (strncmp(buffer, xml_element_end, 2) ==0)
	{
		child = new xmlNode();
		child->setType(XML_ELEMENT_NODE_END);
		buffer = buffer + 2;
	}
	else if (strncmp(buffer, xml_element_header, 1) == 0)
	{
		child = new xmlElement();
		buffer = buffer + 1;
	}
	else
	{
		child = new xmlText();
	}

	*node = child;

	return buffer;
}


bool xmlNode::isDocument()
{
	if (_parent == NULL)
		return true;
	return false;
}


char* xmlNode::checkEndTag(char* buffer, bool* status)
{
	return buffer;
}


void xmlNode::appendChild(xmlNode* child)
{
	if (child != NULL)
	{
		if (_first_child == NULL && _last_child == NULL)
		{
			_first_child = child;
			_last_child = child;
		}

		if (_last_child != NULL)
		{
			_last_child->setNext(child);
			child->setPrev(_last_child);
			child->setParent(this);
		}
	}
}


void xmlNode::setPrev(xmlNode* prev)
{
	_prev_sibling = prev;
}

void xmlNode::setNext(xmlNode* next)
{
	_next_sibling = next;
}

void xmlNode::setParent(xmlNode* parent)
{
	_parent = parent;
}

char* xmlNode::skipSpace(char* buffer)
{
	while(*buffer == ' ')
	{
		buffer++;
	}
	return buffer;
}

char* xmlNode::skipDeclaration(char* buffer)
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


char* xmlNode::skipComment(char* buffer)
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
/* ------------------------------- xmlElement ------------------------------- */
xmlElement::xmlElement(): xmlNode(),
// _first_child(NULL),
// _last_child(NULL),
_first_attribute(NULL),
_last_attribute(NULL)
{
	_type = XML_ELEMENT_NODE;
}


xmlElement::~xmlElement()
{

}


char* xmlElement::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);

	// 获取元素名称
	char* start = buffer;
	while(*buffer != ' ')
	{
		buffer++;
	}
	int length = buffer - start;

	if (_name != NULL)
		delete [] _name;

	_name = new char [length+1];
	strncpy(_name, start, length);

	// 获取属性
	while (*buffer != '/' && *buffer != '>')
	{
		xmlAttribute* attr = new xmlAttribute();

		buffer = attr->parse(buffer);

		setAttributeNode(attr);
	}

	if (strncmp(buffer, "/>", 2) != 0 && strncmp(buffer, ">", 1) == 0)
	{
		bool status = true;
		buffer = this->xmlNode::parse(buffer, &status);
	}

	// 获取
	return buffer;
}


void xmlElement::setAttributeNode(xmlAttribute* attr)
{
	if (attr != NULL)
	{
		if (_first_attribute == NULL && _last_attribute == NULL)
		{
			_first_attribute = attr;
			_last_child = attr;
		}

		if (_last_attribute != NULL)
		{
			_last_attribute->setNext(attr);
			attr->setPrev(_last_attribute);
			attr->setParent(this);
		}
	}
}

/* --------------------------------- xmlText -------------------------------- */
xmlText::xmlText()
{
	_type = XML_TEXT_NODE;
}


xmlText::~xmlText()
{

}


char* xmlText::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);
	char* start = buffer;
	while (*buffer != '<')
	{
		buffer++;
	}

	int length = buffer - start;
	if (_value != NULL)
		delete [] _value;

	_value = new char [length+1];

	strncpy(_value, start, length);
	return buffer;
}

/* ------------------------------- xmlAttribute ----------------------------- */
xmlAttribute::xmlAttribute()
{
	_type = XML_ATTRIBUTE_NODE;
}


xmlAttribute::~xmlAttribute()
{

}


char* xmlAttribute::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);

	*status = false;

	char* start = buffer;
	while(strncmp(buffer, "/>", 2) != 0 || strncmp(buffer, ">", 1) != 0)
	{
		buffer++;
		if (*buffer == '=' && buffer > start)
		{
			int length = buffer - start;
			_name = new char [length+1];
			strncpy(_name, buffer, length);
		}

		if (*buffer == '\"' && *status == false)
		{
			start = buffer;
			*status = true;
		}

		if (*buffer == '\"' && *status == true)
		{
			if (buffer > start)
			{
				if (buffer == (start + 1))
				{
					if (_value != NULL) delete [] _value;
					else
					{
						int length = buffer - start;
						_value = new char [length];

						strncpy(_value, (start+1), length-1);
					}
				}
			}
		}
	}
	return buffer;
}
/* -------------------------------- xmlComment ------------------------------ */
xmlComment::xmlComment()
{
	_type = XML_COMMENT_NODE;
}


xmlComment::~xmlComment()
{

}


char* xmlComment::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);
	char* start = buffer;
	while (strncmp(buffer, "-->", 3) != 0)
	{
		buffer++;
	}

	int length = buffer - start;
	if (_value != NULL)
		delete [] _value;

	_value = new char [length+1];

	strncpy(_value, start, length);

	buffer = buffer + 3;
	return buffer;
}


/* ------------------------------ xmlDeclaration ---------------------------- */
xmlDeclaration::xmlDeclaration()
{
	_type = XML_DECLARATION_NODE;
}

xmlDeclaration::~xmlDeclaration()
{

}

char* xmlDeclaration::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);
	char* start = buffer;
	while (strncmp(buffer, "?>", 2) != 0)
	{
		buffer++;
	}

	int length = buffer - start;
	if (_value != NULL)
		delete [] _value;

	_value = new char [length+1];

	strncpy(_value, start, length);
	buffer = buffer + 2;
	return buffer;
}


/* ------------------------------- xmlDocument ------------------------------ */
xmlDocument::xmlDocument(): xmlElement(),
_buffer(NULL)
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
	bool status = true;
	parse(buffer, &status);
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