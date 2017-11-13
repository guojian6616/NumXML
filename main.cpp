#include "NumXML.h"

int main()
{
	xmlDocument* xmldoc = new xmlDocument();
	xmldoc->loadXMLDocument("template.xml");

	int number = xmldoc->getNumberElement("material");
	printf("number material = %d\n", number);



	xmlElement** material = new xmlElement* [number];

	xmldoc->getElementsNamedAfter("material", material);


	printf("=========================================\n");
	for(int i=0; i<number; i++)
	{
		material[i]->print();
	}

	printf("=========================================\n");
	printf("material id=%d\n", material[0]->getAttributeValueInteger("id"));
	printf("material name=%s\n", material[0]->getAttributeValue("name"));
	printf("material id=%s\n", material[0]->getAttributeValue("id"));
	printf("material name=%s sigma_t=%s\n", material[0]->getAttributeValue("name"), material[0]->getElementValue("sigma_t"));

	// int number = 0;
	double* sigma_t = material[0]->getElementValueDoubles("sigma_t", &number);
	printf("number = %d\n", number);

	for (int i=0; i<number; i++)
		printf("sigma_t[%d] = %f\n", i, sigma_t[i]);

	delete [] sigma_t;

	delete [] material;

	delete xmldoc;
	return 0;
}