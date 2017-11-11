#include "NumXML.h"

int main()
{
	xmlDocument* xmldoc = new xmlDocument();
	xmldoc->loadXMLDocument("template_input.xml");
	delete xmldoc;
	return 0;
}