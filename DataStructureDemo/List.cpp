#include "stdafx.h"
#include "DataStructureDemo.h"

void ShowLinklist();
void ShowSqlist();
void ShowPolynmail();

void ShowList(int cat)
{
	switch (cat)
	{
	case 1:
		ShowSqlist();
		break;
	case 2:
		ShowLinklist();
		break;
	case 3:
		ShowPolynmail();
		break;
	default:
		break;
	}
}