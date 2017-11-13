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

	xmlNode* node = _first_child;
	while (node != NULL)
	{
		xmlNode* temp = node;
		xml_node_type type = temp->getType();
		node = node->getNext();
		// delete node;
		if (type == XML_ELEMENT_NODE)
		{
			xmlElement* element = static_cast<xmlElement*>(temp);

			delete element;
		}

		else if (type == XML_TEXT_NODE)
		{
			xmlText* text = static_cast<xmlText*>(temp);
			delete text;
		}

		else if (type == XML_COMMENT_NODE)
		{
			xmlComment* comment = static_cast<xmlComment*>(temp);
			delete comment;
		}

		else if (type == XML_DECLARATION_NODE)
		{
			xmlDeclaration* declaration = static_cast<xmlDeclaration*>(temp);
			delete declaration;
		}
	}
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

	while (buffer != NULL && *buffer != '\0')
	{
		xmlNode* node = NULL;
		buffer = identifyNodeType(buffer, &node);

		if (node == NULL)
			break;

		if (node->getType() == XML_ELEMENT_NODE_END)
		{
			buffer = checkEndTag(buffer, &stat);
			if (!stat)
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
	buffer = skipSpace(buffer);
	char* start = buffer;
	while (*buffer != '>')
	{
		buffer++;
	}
	int length = buffer - start;
	if (strncmp(_name, start, length) == 0)
		*status = true;
	else
		*status = false;

	if (*buffer == '>')
		buffer++;
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
			child->setParent(this);
		}

		else if (_last_child != NULL)
		{
			_last_child->setNext(child);
			child->setPrev(_last_child);
			_last_child = child;
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

xmlNode* xmlNode::getPrev()
{
	return _prev_sibling;
}

xmlNode* xmlNode::getNext()
{
	return _next_sibling;
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

char* xmlNode::getName()
{
	return _name;
}

char* xmlNode::getValue()
{
	return _value;
}

void xmlNode::print()
{

}

bool xmlNode::isNameAfter(const char* name)
{
	if (_name != NULL && strcmp(_name, name) == 0)
		return true;
	return false;
}

int xmlNode::getNumberFigures(char* str)
{
	int number = 0;
	char* endptr = str;
	while (*endptr != '\0')
	{
		strtod(str, &endptr);
		str = endptr;
		number++;
		while (*endptr == ' ' || *endptr == '\t')
			endptr++;
	}

	return number;
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
	// 释放所有的属性内存
	xmlAttribute* attr = _first_attribute;
	while (attr != NULL)
	{
		xmlAttribute* temp = static_cast<xmlAttribute*>(attr);
		attr = static_cast<xmlAttribute*>(attr->getNext());
		delete temp;
	}
}


char* xmlElement::parse(char* buffer, bool* status)
{
	buffer = skipSpace(buffer);

	// 获取元素名称
	char* start = buffer;
	while(*buffer != ' ' && *buffer != '/' && *buffer != '>' && *buffer != '\t')
	{
		buffer++;
	}
	int length = buffer - start;

	if (_name != NULL)
		delete [] _name;

	_name = new char [length+1];
	strncpy(_name, start, length);

	_name[length] = '\0';

	printf("element tag: %s\n", _name);

	buffer = skipSpace(buffer);

	// 获取所有属性
	while (*buffer != '/' && *buffer != '>')
	{
		xmlAttribute* attr = new xmlAttribute();
		bool stat = true;
		buffer = attr->parse(buffer, &stat);

		setAttributeNode(attr);
	}

	// if (strncmp(buffer, "/>", 2) != 0 && strncmp(buffer, ">", 1) == 0)
	if (strncmp(buffer, "/>", 2) == 0)
	{
		buffer = buffer +2;
		return buffer;
	}
	else if (strncmp(buffer, ">", 1) == 0)
	{
		buffer++;
		bool status = true;
		buffer = this->xmlNode::parse(buffer, &status);
	}

	return buffer;
}


void xmlElement::setAttributeNode(xmlAttribute* attr)
{
	if (attr != NULL)
	{
		if (attr->getName() == NULL)
		{
			delete attr;
			return;
		}
		if (_first_attribute == NULL && _last_attribute == NULL)
		{
			_first_attribute = attr;
			_last_attribute = attr;
			attr->setParent(this);
		}
		else if (_last_attribute != NULL)
		{
			_last_attribute->setNext(attr);
			attr->setPrev(_last_attribute);
			_last_attribute = attr;
			attr->setParent(this);
		}
	}
}

xmlAttribute* xmlElement::getAttributeNode(const char* name)
{
	xmlAttribute* attr = _first_attribute;

	while (attr != NULL)
	{
		if (attr->isNameAfter(name))
			return attr;
		attr = static_cast<xmlAttribute*>(attr->getNext());
	}

	return NULL;
}

xmlElement** xmlElement::getElementsNamedAfter(const char* name, xmlElement** eles)
{
	if (!eles)
	{
		printf("error, elements pointer is null.\n");
		return NULL;
	}
	if (isNameAfter(name))
	{
		*eles = this;
		eles = eles + 1;
	}

	xmlNode* node = _first_child;
	
	while (node != NULL)
	{
		if (node->getType() == XML_ELEMENT_NODE)
		{
			eles = static_cast<xmlElement*>(node)->getElementsNamedAfter(name, eles);
		}
		node = node->getNext();
	}

	return eles;
}

int xmlElement::getNumberElement(const char* name)
{
	int number = 0;

	if (isNameAfter(name))
		number++;

	xmlNode* node = _first_child;
	while (node != NULL)
	{
		if (node->getType() == XML_ELEMENT_NODE)
			number = number + static_cast<xmlElement*>(node)->getNumberElement(name);
		node = node->getNext();
	}

	return number;
}

char* xmlElement::getElementValue()
{
	if (_first_child->getType() == XML_TEXT_NODE)
		return _first_child->getValue();
	else
	{
		printf("Element: %s has no value.\n", _name);
		return NULL;
	}
}

char* xmlElement::getElementValue(const char* name)
{
	if (name == NULL)
	{
		printf("ELement: %s, child element name is null\n", _name);
		return NULL;
	}

	xmlNode* node = _first_child;
	while (node != NULL)
	{
		if (node->getType() == XML_ELEMENT_NODE && static_cast<xmlElement*>(node)->isNameAfter(name))
		{
			return static_cast<xmlElement*>(node)->getElementValue();
			node = node->getNext();
		}
	}

	printf("Element: %s has no child element named after %s\n", _name, name);
	return NULL;
}

char* xmlElement::getAttributeValue(const char* name)
{
	if (name == NULL)
	{
		printf("ELement: %s, attribute name is null\n", _name);
		return NULL;
	}

	xmlAttribute* attr = _first_attribute;
	while (attr != NULL)
	{
		if (attr->isNameAfter(name))
			return attr->getValue();
		attr = static_cast<xmlAttribute*>(attr->getNext());
	}

	printf("Element: %s has no attribute named after %s\n", _name, name);
	return NULL;
}

int xmlElement::getElementValueInteger()
{
	char* value = getElementValue();
	if (value == NULL)
	{
		printf("Element: %s has no value\n", _name);
		return 0;
	}

	int number =0;
	number = atoi(value);
	return number;	
}

int* xmlElement::getElementValueIntegers(int* number)
{
	char* value = getElementValue();
	if (value == NULL)
	{
		printf("Element: %s has no value\n", _name);
		*number = 0;
		return NULL;
	}

	*number = getNumberFigures(value);
	int * figures = new int [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = (int)strtod(value, &endptr);
		value = endptr;
	}
	return figures;
}

double xmlElement::getElementValueDouble()
{
	char* value = getElementValue();
	if (value == NULL)
	{
		printf("Element: %s has no value\n", _name);
		return 0.;
	}
	double figure = 0.;

	figure = strtod(value, NULL);
	return figure;
}

double* xmlElement::getElementValueDoubles(int* number)
{
	char* value = getElementValue();
	if (value == NULL)
	{
		printf("Element: %s has no value\n", _name);
		*number = 0;
		return NULL;
	}
	*number = getNumberFigures(value);
	double * figures = new double [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = strtod(value, &endptr);
		value = endptr;
	}
	return figures;
}

int xmlElement::getElementValueInteger(const char* name)
{
	char* value = getElementValue(name);
	if (value == NULL)
	{
		// printf("Element: %s has no value\n", _name);
		return 0;
	}

	int number =0;
	number = atoi(value);
	return number;	
}

int* xmlElement::getElementValueIntegers(const char* name, int* number)
{
	char* value = getElementValue(name);
	if (value == NULL)
	{
		// printf("Element: %s has no value\n", _name);
		*number = 0;
		return NULL;
	}

	*number = getNumberFigures(value);
	int * figures = new int [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = (int)strtod(value, &endptr);
		value = endptr;
	}
	return figures;
}

double xmlElement::getElementValueDouble(const char* name)
{
	char* value = getElementValue(name);
	if (value == NULL)
	{
		// printf("Element: %s has no value\n", _name);
		return 0.;
	}
	double figure = 0.;

	figure = strtod(value, NULL);
	return figure;
}

double* xmlElement::getElementValueDoubles(const char* name, int* number)
{
	char* value = getElementValue(name);
	if (value == NULL)
	{
		// printf("Element: %s has no value\n", _name);
		*number = 0;
		return NULL;
	}
	*number = getNumberFigures(value);
	double * figures = new double [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = strtod(value, &endptr);
		value = endptr;
	}
	return figures;
}

int xmlElement::getAttributeValueInteger(const char* name)
{
	char* value = getAttributeValue(name);
	if (value == NULL)
	{
		printf("Element: %s, Attribute: %s has no value\n", _name, name);
		return 0;
	}

	int number =0;
	number = atoi(value);
	return number;	
}

int* xmlElement::getAttributeValueIntegers(const char* name, int* number)
{
	char* value = getAttributeValue(name);
	if (value == NULL)
	{
		printf("Element: %s, Attribute: %s has no value\n", _name, name);
		return NULL;
	}

	*number = getNumberFigures(value);
	int * figures = new int [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = (int)strtod(value, &endptr);
		value = endptr;
	}
	return figures;
}

double xmlElement::getAttributeValueDouble(const char* name)
{
	char* value = getAttributeValue(name);
	if (value == NULL)
	{
		printf("Element: %s, Attribute: %s has no value\n", _name, name);
		return 0.;
	}

	double figure = 0.;

	figure = strtod(value, NULL);
	return figure;
}

double* xmlElement::getAttributeValueDoubles(const char* name, int* number)
{
	char* value = getAttributeValue(name);
	if (value == NULL)
	{
		printf("Element: %s, Attribute: %s has no value\n", _name, name);
		return NULL;
	}

	*number = getNumberFigures(value);
	double * figures = new double [*number];
	char* endptr = value;
	for (int i=0; i<*number; i++)
	{
		figures[i] = strtod(value, &endptr);
		value = endptr;
	}
	return figures;	
}

void xmlElement::print()
{
	printf("element name: %s\n", _name);
	xmlAttribute* attr = _first_attribute;
	while(attr != NULL)
	{
		attr->print();
		attr = static_cast<xmlAttribute*>(attr->getNext());
	}

	xmlNode* node = _first_child;
	while(node != NULL)
	{
		node->print();
		node = node->getNext();
	}
}

/* --------------------------------- xmlText -------------------------------- */
xmlText::xmlText() : xmlNode()
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

	_value[length] = '\0';

	printf("text: %s\n", _value);

	return buffer;
}

void xmlText::print()
{
	printf("%s\n", _value);
}
/* ------------------------------- xmlAttribute ----------------------------- */
xmlAttribute::xmlAttribute() : xmlNode()
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
	while(strncmp(buffer, "/>", 2) != 0 && strncmp(buffer, ">", 1) != 0)
	{
		if (*buffer == '=' && buffer > start)
		{
			int length = buffer - start;
			_name = new char [length+1];
			strncpy(_name, start, length);
			_name[length] = '\0';
			for (int i=0; i<length; i++)
			{
				if (_name[i] == ' ' || _name[i] == '\t')
					_name[i] = '\0';
			}
			printf("attribute name: %s\n", _name);
		}

		// printf("buffer = %c\n", *buffer);
		if (*buffer == '\"' && *status == false)
		{
			start = buffer;
			*status = true;
		}
		else if (*buffer == '\"' && *status == true)
		{
			int length = buffer - start;
			start++;
			_value = new char [length];
			strncpy(_value, start, length-1);
			_value[length-1] = '\0';
			printf("attribute value: %s\n", _value);
			buffer++;
			break;
		}
		buffer++;
	}
	return buffer;
}

void xmlAttribute::print()
{
	printf("%s=\"%s\"\n", _name, _value);
}
/* -------------------------------- xmlComment ------------------------------ */
xmlComment::xmlComment() : xmlNode()
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
	_value[length] = '\0';

	printf("comment: %s\n", _value);

	buffer = buffer + 3;
	return buffer;
}

void xmlComment::print()
{
	printf("comment: %s\n", _value);
}
/* ------------------------------ xmlDeclaration ---------------------------- */
xmlDeclaration::xmlDeclaration() : xmlNode()
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
	_value[length] = '\0';
	printf("declaration: %s\n", _value);
	buffer = buffer + 2;
	return buffer;
}

void xmlDeclaration::print()
{
	printf("declaration: %s\n", _value);
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
	xmlNode::parse(buffer, &status);

	printf("=================================");
	print();
}