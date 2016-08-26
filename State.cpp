#include "stdafx.h"
#include "State.h"

#include <queue>

void aux::State::extend(const Grammar & gr)
{
	queue<AugmentedRule> q;
	for (auto i = augmentedRules.begin(); i != augmentedRules.end(); ++i)
	{
		q.push(*i);
	}

	while (!q.empty())
	{
		AugmentedRule cur = q.front();
		q.pop();
		
		if (cur.afterDot() && gr.nonTerminals.find(*cur.afterDot()) != gr.nonTerminals.end())
		{
			set<Symbol, Symbol::Comp1> las;
			if (cur.rhs.size() == cur.dotPosition + 1)
			{
				las = cur.lookAheadSymbols;
			}
			else
			{
				las = gr.terminalsDerivedFrom(cur.rhs[cur.dotPosition + 1]);
			}

			for (auto i = gr.rules.begin(); i != gr.rules.end(); ++i)
			{
				if ((*i).first.lhs.name == cur.afterDot()->name)
				{		
					AugmentedRule tmp = (*i).first;
					tmp.dotPosition = 0;
					if (augmentedRules.find(tmp) == augmentedRules.end()) {
						tmp.lookAheadSymbols = las;
						augmentedRules.insert(tmp);
						q.push(tmp);
					}
					else
					{
						const AugmentedRule &inSet = *augmentedRules.find(tmp);
						bool complete = true;
						for (auto i = las.begin(); i != las.end(); ++i)
						{
							if (inSet.lookAheadSymbols.find(*i) == inSet.lookAheadSymbols.end())
							{
								complete = false;
							}
						}
						if (!complete) 
						{
							AugmentedRule tmp = inSet;
							tmp.lookAheadSymbols.insert(las.begin(), las.end());
							augmentedRules.erase(inSet);
							augmentedRules.insert(tmp);
							q.push(tmp);
						}
					}
				}
			}
		}

	}
}

#include <iostream>
void aux::State::print() const
{
	for (auto i = augmentedRules.begin(); i != augmentedRules.end(); ++i)
	{
		(*i).print();
		std::cout << endl;
	}
}

set<aux::Symbol, aux::Symbol::Comp1> aux::State::afterDots() const
{
	set<Symbol, Symbol::Comp1> res;
	for (auto i = augmentedRules.begin(); i != augmentedRules.end(); ++i)
	{
		if ((*i).afterDot())
		{
			res.insert(*(*i).afterDot());
		}
	}
	return res;
}

aux::State * aux::State::transitionWith(Symbol s, Grammar & gr)
{
	State * res = new State();
	Symbol::Comp1 comp;
	for (auto i = augmentedRules.begin(); i != augmentedRules.end(); ++i)
	{
		if ((*i).afterDot()) 
		{
			if (!comp(s, *(*i).afterDot()) && !comp(*(*i).afterDot(), s))
			{
				AugmentedRule r = *i;
				r.dotPosition++;
				res->augmentedRules.insert(r);
			}
		}
	}
	res->extend(gr);
	return res;
}

bool aux::State::addRule(const AugmentedRule & r)
{
	if (augmentedRules.find(r) == augmentedRules.end())
	{
		augmentedRules.insert(r);
		return true;
	}
	else
	{
		const AugmentedRule & inState = *augmentedRules.find(r);
		bool complete = true;
		for (auto i = r.lookAheadSymbols.begin(); i != r.lookAheadSymbols.end(); ++i)
		{
			if (inState.lookAheadSymbols.find(*i) == inState.lookAheadSymbols.end())
			{
				complete == false;
				break;
			}
		}
		if (complete)
		{
			return false;
		}
		else
		{
			AugmentedRule tmp = inState;
			tmp.lookAheadSymbols.insert(r.lookAheadSymbols.begin(), r.lookAheadSymbols.end());
			augmentedRules.erase(inState);
			augmentedRules.insert(tmp);
			return true;
		}
	}
}

aux::AugmentedRule * aux::State::reduce(bool & conflict, const Symbol & nextSymbol) const
{
	AugmentedRule *res = 0;
	for (auto i = augmentedRules.begin(); i != augmentedRules.end(); ++i)
	{
		if ((*i).dotPosition == (*i).rhs.size() && (*i).lookAheadSymbols.find(nextSymbol) != (*i).lookAheadSymbols.end())
		{
			if (res)
			{
				conflict = true;
				delete res;
				return 0;
			}
			else
			{
				res = new AugmentedRule();
				*res = (*i);
			}
		}
	}
	conflict = false;
	return res;
}


