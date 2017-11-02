#include "NumXML.h"

int main()
{
	xmlDocument* xmldoc = new xmlDocument();
	xmldoc->loadXMLDocument("template.xml");
	delete xmldoc;
	return 0;
}