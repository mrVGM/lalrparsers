#include "stdafx.h"

#include <fstream>
#include <istream>
#include "Grammar.h"
#include "Automata.h"
#include "AugmentedRule.h"

aux::Grammar * aux::readGrammarFromString(std::string definition)
{
	string s = "(1 \" \" 1 skip)\n(1 \"\t\" 1 skip)\n(1 \"\n\" 1 skip)\n(1 \"<\" 2 skip)\n(1 \"\" 7 skip)\n\n(2 \">\" 3 group1)\n(2 \"\" 2 toAux)\n\n(3 \" \" 3 skip)\n(3 \"\t\" 3 skip)\n(3 \"\\\"\" 4 skip)\n(3 \"\" 7 skip)\n\n(4 \"\\\\\" 5 skip)\n(4 \"\\\"\" 6 group2)\n(4 \"\" 4 toAux)\n\n(5 \"\" 4 toAux)\n\n(6 \" \" 6 skip)\n(6 \"\t\" 6 skip)\n(6 \"\n\" 1 skip)\n(6 \"\" 7 skip)\n\n(7 \"\" 7 skip)\n";


	aux::automata::Skip skip;
	aux::automata::Group group1("lhs");
	aux::automata::ToAux toAux;
	aux::automata::Group group2("rhs");
	aux::automata::Group group("symbol");
	aux::automata::ToMain toMain;

	map<string, aux::automata::Action*> f;
	f["skip"] = &skip;
	f["group1"] = &group1;
	f["toAux"] = &toAux;
	f["group2"] = &group2;

	vector<string> tmp;
	tmp.push_back("1");
	tmp.push_back("6");

	aux::Automata *r = aux::automata::createAutomataFromString(s, "1", tmp, f);

	if (r == 0)
	{
		return 0;
	}

	f["group"] = &group;
	f["toMain"] = &toMain;

	string rhsAutomata = "(1 \"\" 1 toMain) (1 \".\" 2 skip) (1 \"<\" 3 skip) (2 \"\" 1 toMain)	(3 \"\" 3 toAux) (3 \">\" 1 group)";

	tmp.clear();
	tmp.push_back("1");

	aux::Automata *rhsReader = aux::automata::createAutomataFromString(rhsAutomata, "1", tmp, f);

	if (rhsReader == 0) 
	{
		delete r;
		return 0;
	}

	Symbols def;
	for (string::iterator i = definition.begin(); i < definition.end(); ++i)
	{
		string tmp;
		tmp.push_back(*i);
		def.push_back(Symbol(tmp, tmp));
	}

	Symbols lhs_rhs;
	if (!r->parse(lhs_rhs, def))
	{
		delete r;
		delete rhsReader;
		return 0;
	}


	Grammar * res = new Grammar();

	Symbols::iterator it = lhs_rhs.begin();

	bool firstIteration = true;

	while (it < lhs_rhs.end())
	{
		Symbol lhs = *it;
		++it;
		Symbol rhs = *it;
		++it;

		AugmentedRule rule;
		rule.lhs = Symbol(lhs.data, lhs.data);

		Symbols t;
		for (string::iterator i = rhs.data.begin(); i < rhs.data.end(); ++i)
		{
			string str;
			str.push_back(*i);
			t.push_back(Symbol(str, str));
		}

		if (!rhsReader->parse(rule.rhs, t))
		{
			delete r;
			delete rhsReader;
			delete res;
			return 0;
		}

		for (Symbols::iterator i = rule.rhs.begin(); i < rule.rhs.end(); ++i)
		{
			(*i).name = (*i).data;
		}
		res->rules[rule] = res->count++;
		
		if (firstIteration) {
			res->initialRule = rule;
		}
		firstIteration = false;
	}

	delete r;
	delete rhsReader;

	res->initSymbols();

	return res;
}

void aux::Grammar::initSymbols()
{
	for (auto i = rules.begin(); i != rules.end(); ++i)
	{
		nonTerminals.insert((*i).first.lhs);
	}
	for (auto i = rules.begin(); i != rules.end(); ++i)
	{
		const AugmentedRule &cur = (*i).first;
		for (auto j = cur.rhs.begin(); j != cur.rhs.end(); j++)
		{
			if (nonTerminals.find(*j) == nonTerminals.end())
			{
				terminals.insert(*j);
			}
		}
	}
}

#include <queue>

set<aux::Symbol, aux::Symbol::Comp1> aux::Grammar::terminalsDerivedFrom(Symbol s) const
{
	set<Symbol, Symbol::Comp1> res;
	if (terminals.find(s) != terminals.end()) 
	{
		res.insert(s);
		return res;
	}
	std::queue<Symbol> q;
	q.push(s);
	set<Symbol, Symbol::Comp1> processed;
	while (!q.empty())
	{
		Symbol cur = q.front();
		q.pop();

		if (terminals.find(cur) != terminals.end()) 
		{
			res.insert(cur);
		}
		else if (processed.find(cur) == processed.end())
		{
			processed.insert(cur);
			for (auto i = rules.begin(); i != rules.end(); ++i)
			{
				if ((*i).first.lhs.name == cur.name) 
				{
					q.push((*i).first.rhs[0]);
				}
			}
		}
	}
	return res;
}
