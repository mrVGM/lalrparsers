#pragma once

#include <set>

#include "AugmentedRule.h"
#include "Grammar.h"

namespace aux
{
	struct State
	{
		struct Comp
		{
			bool operator() (const State & s1, const State & s2) const
			{
				auto it1 = s1.augmentedRules.begin();
				auto it2 = s2.augmentedRules.begin();
				AugmentedRule::Comp2 comp;

				while (it1 != s1.augmentedRules.end() && it2 != s2.augmentedRules.end())
				{
					if (comp(*it1, *it2) || comp(*it2, *it1))
					{
						return comp(*it1, *it2);
					}
					++it1;
					++it2;
				}
				if (it1 == s1.augmentedRules.end() && it2 == s2.augmentedRules.end())
				{
					return false;
				}
				if (it1 == s1.augmentedRules.end())
				{
					return true;
				}
				return false;
			}
		};

		std::set<AugmentedRule, AugmentedRule::Comp2> augmentedRules;
		void extend(const Grammar & gr);
		void print() const;
		set<Symbol, Symbol::Comp1> afterDots() const;
		State * transitionWith(Symbol s, Grammar &gr);
		bool addRule(const AugmentedRule &r);
		AugmentedRule * reduce(bool &conflict, const Symbol & nextSymbol) const;
	};

	struct StateWrapper
	{
		struct Comp
		{
			bool operator() (const StateWrapper & s1, const StateWrapper & s2) const
			{
				State::Comp comp;
				return comp(*s1.state, *s2.state);
			}
		};
		State * state;
		StateWrapper() {}
		StateWrapper(State * s) : state(s) {}
	};
}

