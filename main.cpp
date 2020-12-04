#include "common.h"
#include "LexAnalyst.h"

int main()
{
	LexAnalyst LA("input_test1.txt");
	LA.Analyse();
	LA.Output();
	return 0;
}