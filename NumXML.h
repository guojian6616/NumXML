#ifndef NUMXML_H_
#define NUMXML_H_
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

enum xml_node_type
{
	XML_ELEMENT_NODE = 1,
	XML_ATTRIBUTE_NODE = 2,
	XML_TEXT_NODE = 3,
	XML_COMMENT_NODE = 8,
	XML_DOCUMENT_NODE = 9,
	XML_DECLARATION_NODE = 254,
	XML_UNSET_NODE = 255,

	/** </xxx> */
	XML_ELEMENT_NODE_END = 0
};


class xmlAttribute;
class xmlComment;


class xmlNode
{
private:

protected:
	char* _name;
	char* _value;
	xmlNode* _parent;

	xmlNode* _first_child;
	xmlNode* _last_child;

	xmlNode* _next_sibling;
	xmlNode* _prev_sibling;

	xml_node_type _type;

public:
	xmlNode();
	virtual ~xmlNode();

	// virtual void parse(char* p);

	void clone(bool flag);

	void setType(xml_node_type type);
	xml_node_type getType();

	virtual char* parse(char* buffer, bool* status=NULL);

	char* identifyNodeType(char* buffer, xmlNode** node);

	/** Check if the node is document */
	bool isDocument();

	/** Check the end tag <tag> ... </tag> */
	char* checkEndTag(char* buffer, bool* status);

	void appendChild(xmlNode* child);
	void removeChild(xmlNode* child);

	void setPrev(xmlNode* prev);
	void setNext(xmlNode* next);
	xmlNode* getPrev();
	xmlNode* getNext();
	void setParent(xmlNode* parent);

	char* skipDeclaration(char* buffer);
	char* skipComment(char* buffer);
	char* skipSpace(char* buffer);

	char* getName();
	char* getValue();

	virtual void print();

	bool isNameAfter(const char* name);

	int getNumberFigures(char* str);
};


class xmlElement : public xmlNode
{
private:
	xmlAttribute* _first_attribute;
	xmlAttribute* _last_attribute;
public:
	xmlElement();
	virtual ~xmlElement();

	void setAttribute(const char* attr, const char* value);
	void setAttributeNode(xmlAttribute* attr);
	void removeAttribute(const char* attr);
	xmlAttribute* getAttributeNode(const char* name);
	void removeAttributeNode(xmlAttribute* attr);

	// xmlNode** getElementsByTagName(const char* name);

	xmlElement** getElementsNamedAfter(const char* name, xmlElement** eles);

	int getNumberElement(const char* name);

	char* getElementValue();
	char* getElementValue(const char* name);
	char* getAttributeValue(const char* name);

	int getElementValueInteger();
	int* getElementValueIntegers(int* number);
	double getElementValueDouble();
	double* getElementValueDoubles(int* number);

	int getElementValueInteger(const char* name);
	int* getElementValueIntegers(const char* name, int* number);
	double getElementValueDouble(const char* name);
	double* getElementValueDoubles(const char* name, int* number);

	int getAttributeValueInteger(const char* name);
	int* getAttributeValueIntegers(const char* name, int* number);
	double getAttributeValueDouble(const char* name);
	double* getAttributeValueDoubles(const char* name, int* number);

	void insertBefore(xmlNode* new_node, xmlNode* node);

	char* parse(char* buffer, bool* status=NULL);

	void print();
};


class xmlText : public xmlNode
{
private:

public:
	xmlText();
	virtual ~xmlText();
	char* parse(char* buffer, bool* status=NULL);
	void print();
};


class xmlAttribute : public xmlNode
{
private:

public:
	xmlAttribute();
	virtual ~xmlAttribute();
	char* parse(char* buffer, bool* status=NULL);
	void print();
};

class xmlComment : public xmlNode
{
private:

public:
	xmlComment();
	virtual ~xmlComment();
	char* parse(char* buffer, bool* status=NULL);
	void print();
};


class xmlDeclaration : public xmlNode
{
private:

public:
	xmlDeclaration();
	~xmlDeclaration();
	char* parse(char* buffer, bool* status=NULL);
	void print();
};


class xmlDocument : public xmlElement
{
private:
	char* _buffer;

public:
	xmlDocument();
	virtual ~xmlDocument();

	void loadXMLDocument(const char* xmldocument);
	void loadXMLString(const char* xmlstring);

	xmlElement* createElement(const char* name);
	xmlText* createTextNode(const char* text);
	xmlComment* createComment(const char* comment);
	// void print();
};

#endif