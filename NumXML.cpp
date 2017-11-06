#include "NumXML.h"

// xmlDocument::xmlDocument()
// {

// }

void xmlDocument::loadXMLDocument(const char* xmldoc)
{
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

	// 删除换行符
	printf("length: %ld buffer size %lu ", length, strlen(_buffer));

	printf("xml file:\n%s\n", _buffer);

	char* buffer = new char [length+1];

	char* start = buffer;

	while(_buffer != NULL && *_buffer != '\0')
	{
		if(*_buffer != '\n' && *_buffer != '\t')
		{
			*start = *_buffer;
			start++;
		}
		// printf("%c",*_buffer);
		_buffer++;
	}
	*start = '\0';

	// printf("new buffer size %lu:\n%s\n\n", strlen(buffer), buffer);

	start = skipDeclaration(buffer);
	// printf("skip declaration:\n%s\n", start);

	start = skipComment(start);
	printf("skip comment:\n%s\n", start);

	// parse(start);
	while(start != NULL && *start != '\0')
	{
		
	}
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