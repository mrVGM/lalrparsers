#include "stdafx.h"

#include <queue>

#include "StateGraph.h"

aux::StateGraph::StateGraph(Grammar & gr)
{
	count = 0;
	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table[i].size(); j++) 
		{
			table[i][j] = -1;
		}
	}
	int n = 0;
	for (auto i = gr.terminals.begin(); i != gr.terminals.end(); ++i)
	{
		if (symbols.find(*i) == symbols.end())
		{
			symbols[*i] = n++;
			symbolIds.push_back(*i);
		}
	}
	for (auto i = gr.nonTerminals.begin(); i != gr.nonTerminals.end(); ++i)
	{
		if (symbols.find(*i) == symbols.end())
		{
			symbols[*i] = n++;
			symbolIds.push_back(*i);
		}
	}

	State * initial = new State();
	AugmentedRule tmp = gr.initialRule;
	tmp.dotPosition = 0;
	initial->augmentedRules.insert(tmp);

	initial->extend(gr);
	
	addState(initial);

	StateWrapper sw;
	sw.state = initial;
	queue<StateWrapper> q;
	q.push(sw);

	while (!q.empty())
	{
		StateWrapper cur = q.front();
		q.pop();

		auto afterDotSymbols = cur.state->afterDots();

		for (auto i = afterDotSymbols.begin(); i != afterDotSymbols.end(); ++i)
		{
			State * tmp = cur.state->transitionWith(*i, gr);
			auto check = states[cur];
			if (addState(tmp))
			{
				q.push(StateWrapper(tmp));
				table[states[cur]][symbols[*i]] = states[StateWrapper(tmp)];
				auto y = states[StateWrapper(tmp)];
				int p;
			}
			else 
			{
				table[states[cur]][symbols[*i]] = states[StateWrapper(tmp)];
				delete tmp;
			}
		}
	}

}

bool aux::StateGraph::addState(State * state)
{
	StateWrapper sw;
	sw.state = state;
	if (states.find(sw) == states.end()) 
	{
		states[sw] = count++;
		stateIds.push_back(sw);
		table.push_back(vector<int>(symbolIds.size()));
		for (int i = 0; i < table[table.size() - 1].size(); i++)
		{
			table[table.size() - 1][i] = -1;
		}
		return true;
	}
	else 
	{
		const StateWrapper & inStateGraph = (*states.find(sw)).first;
		bool complete = true;
		for (auto i = state->augmentedRules.begin(); i != state->augmentedRules.end(); ++i)
		{
			if (inStateGraph.state->addRule(*i))
			{
				complete = false;
			}
		}
		if (!complete)
		{
			delete state;
			return true;
		}
		else
		{
			return false;
		}
	}
}

aux::LALRParser * aux::StateGraph::createParser(const Grammar & gr) const
{
	LALRParser * res = new LALRParser();

	res->symbolsIds = symbolIds;
	res->symbols = symbols;

	int n = 0;

	for (int i = 0; i < table.size(); ++i)
	{
		res->table.push_back(vector<LALRParser::Action>());
		for (int j = 0; j < table[i].size(); ++j)
		{
			bool conflict = false;
			AugmentedRule * red = stateIds[i].state->reduce(conflict, symbolIds[j]);
			if (conflict)
			{
				delete res;
				return 0;
			}
			if (red)
			{
				if (table[i][j] != -1)
				{
					delete red;
					delete res;
					return 0;
				}
				else
				{
					if (res->rules.find(*red) == res->rules.end())
					{
						res->rules[*red] = n++;
						res->ruleIds.push_back(*red);
					}
					res->table[i].push_back(LALRParser::Action('r', res->rules[*red]));
				}
			}
			else
			{
				if (table[i][j] != -1)
				{
					res->table[i].push_back(LALRParser::Action('s', table[i][j]));
				}
				else 
				{
					res->table[i].push_back(LALRParser::Action());
				}
			}
		}
	}
	res->initialState = 0;
	State fs;
	AugmentedRule r = gr.initialRule;
	r.dotPosition = r.rhs.size();
	
	StateWrapper fsw(&fs);
	fs.augmentedRules.insert(r);
	res->finalState = (*states.find(fsw)).second;
	
	return res;
}
