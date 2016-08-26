#pragma once

#include <string>

#include "Symbol.h"
#include "LALRParser.h"
#include "DerivationTree.h"

namespace lalrParser 
{
	typedef aux::Symbol Symbol;
	typedef aux::Symbols Symbols;
	
	class Parser
	{
		friend Parser restore(string & ser);
		friend Parser create(string & def);
	private:
		int * refs;
		aux::LALRParser * parser;

		Parser(aux::LALRParser * p);
		void destroy();

	public:

		Parser(const Parser & other);
		Parser & operator=(const Parser & other);
		std::string serialize();
		DerivationTree parse(const Symbols & symbols, bool & successful);
		~Parser();
	};

	
}

