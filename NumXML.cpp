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

	// 删除换行符
	printf("length: %ld buffer size %lu ", length, strlen(_buffer));

	printf("xml file:\n%s\n", _buffer);

	// char* buffer = new char [length+1];

	// int counter = 0;
	// for (int i=0; i<(int)length; i++)
	// {
	// 	if(_buffer[i] != '\n' && _buffer[i] != '\t')
	// 		buffer[counter++] = _buffer[i];
	// }

	// printf("new buffer:\n%s\n", buffer);

	// delete [] buffer;

	fclose(fin);
}


// xmlElement::xmlElement()
// {

// }


// xmlNode::xmlNode()
// {

// }