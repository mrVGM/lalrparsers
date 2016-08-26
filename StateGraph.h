#pragma once

#include <map>
#include <vector>

#include "State.h"
#include "LALRParser.h"

namespace aux
{
	struct StateGraph
	{
		struct Comp
		{
			bool operator() (const State* s1, const State *s2) const
			{
				State::Comp comp;
				return comp(*s1, *s2);
			}
		};

		int count;
		std::map<StateWrapper, int, StateWrapper::Comp> states;
		vector<StateWrapper> stateIds;
		map<Symbol, int, Symbol::Comp1> symbols;
		vector<Symbol> symbolIds;

		vector<vector<int>> table;

		StateGraph() : count(0) {}
		StateGraph(Grammar &gr);

		bool addState(State* state);

		LALRParser * createParser(const Grammar & gr) const;

		~StateGraph()
		{
			for (auto i = stateIds.begin(); i < stateIds.end(); ++i)
			{
				delete (*i).state;
			}
		}
	};
}
