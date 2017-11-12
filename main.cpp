#include "NumXML.h"

int main()
{
	xmlDocument* xmldoc = new xmlDocument();
	xmldoc->loadXMLDocument("template.xml");

	int number = xmldoc->getNumberElement("material");
	printf("number material = %d\n", number);

	xmlElement** material = new xmlElement* [number];

	

	delete [] material;

	delete xmldoc;
	return 0;
}