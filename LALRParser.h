#pragma once

#include <vector>
#include <map>

#include "AugmentedRule.h"
#include "Tree.h"

namespace aux {
	class LALRParser
	{
	public:
		struct Action 
		{
			char action;
			int argument;
			Action(char _action = 'n', int _argument = -1)
			{
				action = _action;
				argument = _argument;
			}
		};
		std::vector<Symbol> symbolsIds;
		map<Symbol, int, Symbol::Comp1> symbols;
		int initialState;
		int finalState;
		map<AugmentedRule, int, AugmentedRule::Comp1> rules;
		vector<AugmentedRule> ruleIds;
		std::vector<std::vector<Action> > table;
		Tree * parse(const Symbols & input);
		string serialize();
	};
	LALRParser * deserialize(string s);
}

