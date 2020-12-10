#include "common.h"
#include "LexAnalyst.h"
#include "ParAnalyst.h"

int main()
{
	// LexAnalyst LA("input_test1.txt");
	// LA.Analyse();
	// LA.Output();

	ParAnalyst PA("inputProductions.txt");
	PA.getProductions();
	PA.showProductions();
	PA.initFirst();
	PA.showFirst();
	PA.createDFA();

	return 0;
}