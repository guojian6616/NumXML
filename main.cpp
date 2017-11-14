#include "NumXML.h"

int main(int argc, char* argv[])
{
	xmlDocument* xmldoc = new xmlDocument();
	xmldoc->loadXMLDocument(argv[1]);
	// delete xmldoc;


	// xmlDocument* xmldoc = new xmlDocument();
	// xmldoc->loadXMLDocument("template.xml");

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
	// printf("material id=%d\n", material[0]->getAttributeValueInteger("id"));
	// printf("material name=%s\n", material[0]->getAttributeValue("name"));
	// printf("material id=%s\n", material[0]->getAttributeValue("id"));
	// printf("material name=%s sigma_t=%s\n", material[0]->getAttributeValue("name"), material[0]->getElementValue("sigma_t"));

	// int number = 0;
	double* sigma_t = material[0]->getElementValueDoubles("sigma_t", &number);
	printf("number = %d\n", number);

	for (int i=0; i<number; i++)
		printf("sigma_t[%d] = %f\n", i, sigma_t[i]);

	xmldoc->free(sigma_t);

	int num = material[0]->getNumberChildren();
	printf("number children = %d\n", num);

	char* value = material[0]->getElementValue("sigma_t");
	printf("sigma_t=[%s]\n", value);

	value = material[0]->getElementValue("sigma_s");
	printf("sigma_s=[%s]\n", value);

	sigma_t = material[0]->getElementValueDoubles("nu_sigma_f", &number);
	printf("number = %d\n", number);

	// for (int i=0; i<number; i++)
	// 	printf("nu_sigma_f[%d] = %f\n", i, sigma_t[i]);

	// xmldoc->free(sigma_t);
	// // delete [] sigma_t;

	// int* g = material[0]->getAttributeValueIntegers("energy_group", &number);
	// // printf("g = %f\n", g);
	// printf("number = %d\n", number);
	// for (int i=0; i<number; i++)
	// 	printf("g[%d] = %d\n", i, g[i]);
	delete [] material;

	delete xmldoc;
	return 0;
}