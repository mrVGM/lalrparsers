#include "stdafx.h"

#include <iostream>

#include "Parser.h"
#include "DerivationTree.h"

using namespace std;

void printTree(lalrParser::DerivationTree & t)
{
	cout << "(" << t.getData().name;
	if (t.numberOfChildren() == 0)
	{
		 cout << ")";
	}
	else 
	{
		for (int i = 0; i < t.numberOfChildren(); i++)
		{
			cout << " ";
			printTree(t[i]);
		}
		cout << ")";
	}
}




int main()
{
	string gr = "<S> \"<Exp><END>\"\n\n<Exp> \"<Exp>+<Term>\"\n<Exp> \"<Exp>-<Term>\"\n<Exp> \"<Term>\"\n\n<Term> \"<Term>*<Factor>\"\n<Term> \"<Term>/<Factor>\"\n<Term> \"<Factor>\"\n\n<Factor> \"x\"\n<Factor> \"(<Exp>)\"\n<Factor> \"-<Factor>\"\n";
	
	auto parser = lalrParser::create(gr);
		
	lalrParser::Symbols inp;
	inp.push_back(lalrParser::Symbol("x", "x"));
	inp.push_back(lalrParser::Symbol("+", "+"));
	inp.push_back(lalrParser::Symbol("x", "x"));
	inp.push_back(lalrParser::Symbol("END", "END"));

	bool success;

	lalrParser::DerivationTree dt = parser.parse(inp, success);

	printTree(dt);
	
	std::cout << endl;

	return 0;
}
