#pragma once

#include <map>
#include "AugmentedRule.h"

namespace aux 
{
	struct Grammar
	{
		int count;
		std::map<AugmentedRule, int, AugmentedRule::Comp1> rules;
		AugmentedRule initialRule;
		Grammar() : count(0) {}
		
		set<Symbol, Symbol::Comp1> nonTerminals;
		set<Symbol, Symbol::Comp1> terminals;

		void initSymbols();

		set<Symbol, Symbol::Comp1> terminalsDerivedFrom(Symbol s) const;
	};

	Grammar * readGrammarFromString(std::string definition);
}
