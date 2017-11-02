#ifndef NUMXML_H_
#define NUMXML_H_
#include <stdio.h>
#include <iostream>
#include <string.h>

enum xml_node_type
{
	XML_ELEMENT_NODE = 1,
	XML_ATTRIBUTE_NODE = 2,
	XML_TEXT_NODE = 3,
	XML_COMMENT_NODE = 8,
	XML_DOCUMENT_NODE = 9,
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

	xmlNode* _next_sibling;
	xmlNode* _prev_sibling;

	xml_node_type _type;

public:
	// xmlNode();
	// virtual ~xmlNode();

	void clone(bool flag);
};


class xmlElement : public xmlNode
{
private:
	xmlNode* _first_child;
	xmlNode* _last_child;
	xmlAttribute* _first_attribute;
	xmlAttribute* _last_attribute;
public:
	// xmlElement();
	// virtual ~xmlElement();

	void appendChild(xmlNode* child);
	void removeChild(xmlNode* child);

	void setAttribute(const char* attr, const char* value);
	void setAttributeNode(xmlAttribute* attr);
	void removeAttribute(const char* attr);
	xmlAttribute* getAttributeNode(const char* attr);
	void removeAttributeNode(xmlAttribute* attr);

	xmlNode** getElementsByTagName(const char* name);

	void insertBefore(xmlNode* new_node, xmlNode* node);
};


class xmlText : public xmlNode
{
private:

public:
	// xmlText();
	// virtual ~xmlText();
};


class xmlDocument : public xmlElement
{
private:
	char* _buffer;

public:
	// xmlDocument();
	// virtual ~xmlDocument();

	void loadXMLDocument(const char* xmldocument);
	void loadXMLString(const char* xmlstring);

	xmlElement* createElement(const char* name);
	xmlText* createTextNode(const char* text);
	xmlComment* createComment(const char* comment);

};


class xmlAttribute : public xmlNode
{
private:

public:
	// xmlAttribute();
	// virtual ~xmlAttribute();
};

class xmlComment : public xmlNode
{
private:

public:
	// xmlComment();
	// virtual ~xmlComment();
};

#endif