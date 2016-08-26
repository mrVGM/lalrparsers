#include "stdafx.h"

#include "LALRParser.h"
#include "Tree.h"

#include <stack>

aux::Tree * aux::LALRParser::parse(const Symbols & input)
{
	std::stack<Tree*> treeStack;
	std::stack<int> stateStack;
	stateStack.push(initialState);

	auto i = input.begin();

	while(i != input.end())
	{
		Symbol nextSymbol = *i;
		if (symbols.find(nextSymbol) == symbols.end())
		{
			while (!treeStack.empty())
			{
				delete treeStack.top();
				treeStack.pop();
			}
			return 0;
		}
		Action action = table[stateStack.top()][symbols[*i]];
		if (action.action == 'n')
		{
			while (!treeStack.empty())
			{
				delete treeStack.top();
				treeStack.pop();
			}
			return 0;
		}
		if (action.action == 's')
		{
			stateStack.push(action.argument);
			Tree * t = new Tree();
			t->data = *i;
			treeStack.push(t);
			++i;
		}
		else
		{
			AugmentedRule rule = ruleIds[action.argument];
			Tree * t = new Tree(rule.rhs.size());
			t->data = rule.lhs;
			for (int i = rule.rhs.size() - 1; i >= 0; i--)
			{
				t->children[i] = treeStack.top();
				treeStack.pop();
				stateStack.pop();
			}
			treeStack.push(t);
			stateStack.push(table[stateStack.top()][symbols[rule.lhs]].argument);
		}
	}
	if (stateStack.top() != finalState) 
	{
		while (!treeStack.empty())
		{
			delete treeStack.top();
			treeStack.pop();
		}
		return 0;
	}
	Tree * t = new Tree(2);
	t->children[1] = treeStack.top();
	treeStack.pop();
	t->children[0] = treeStack.top();
	t->data = Symbol("Root");
	return t;
}

#include <sstream>

string aux::LALRParser::serialize()
{
	stringstream ss;
	ss << symbolsIds.size() << ' ';
	
	for (auto i = symbolsIds.begin(); i != symbolsIds.end(); ++i)
	{
		ss << (string) (*i);
	}
	ss << ' ' << ruleIds.size() << ' ';

	for (auto i = ruleIds.begin(); i != ruleIds.end(); ++i)
	{
		ss << symbols[(*i).lhs] << ' ' << (*i).rhs.size() << ' ';
		for (auto j = (*i).rhs.begin(); j != (*i).rhs.end(); ++j)
		{
			ss << symbols[(*j)] << ' ';
		}
	}

	ss << table.size() << ' ' << table[0].size() << ' ';

	for (auto i = table.begin(); i != table.end(); ++i)
	{
		for (auto j = (*i).begin(); j != (*i).end(); ++j)
		{
			ss << (*j).argument << (*j).action << ' ';
		}
	}

	ss << initialState << ' ' << finalState;

	return string(ss.str());
}

aux::LALRParser * aux::deserialize(string s)
{
	LALRParser * res = new LALRParser();
	stringstream ss;
	ss << s;

	int nSymblols;
	ss >> nSymblols;

	bool in = false;

	string strName;

	for (int i = 0; i < nSymblols; i++)
	{
		strName = "";
		char cur;
		do
		{
			ss >> cur;
			if (!in)
			{
				if (cur == '"')
				{
					in = true;
					cur = ' ';
				}
			}
			else
			{
				if (cur == '\\')
				{
					char tmp;
					ss >> tmp;
					strName.push_back(tmp);
				}
				else if (cur != '"')
				{
					strName.push_back(cur);
				}
			}
		} while (!(cur == '"' && in));
		in = false;
		res->symbolsIds.push_back(Symbol(strName));
	}

	for (int i = 0; i < res->symbolsIds.size(); i++)
	{
		res->symbols[res->symbolsIds[i]] = i;
	}

	int nRules;
	ss >> nRules;

	for (int i = 0; i < nRules; i++)
	{
		AugmentedRule rule;
		int lhs;
		ss >> lhs;
		rule.lhs = Symbol(res->symbolsIds[lhs]);
		int nRhs;
		ss >> nRhs;
		for (int j = 0; j < nRhs; j++)
		{
			int curSymbol;
			ss >> curSymbol;
			rule.rhs.push_back(Symbol(res->symbolsIds[curSymbol]));
		}
		res->ruleIds.push_back(rule);
	}

	for (int i = 0; i < res->ruleIds.size(); i++)
	{
		res->rules[res->ruleIds[i]] = i;
	}

	int rows;
	ss >> rows;
	int columns;
	ss >> columns;

	for (int i = 0; i < rows; i++)
	{
		res->table.push_back(vector<LALRParser::Action>());
		for (int j = 0; j < columns; j++)
		{
			LALRParser::Action act;
			ss >> act.argument;
			ss >> act.action;
			res->table[res->table.size() - 1].push_back(act);
		}
	}

	ss >> res->initialState;
	ss >> res->finalState;
	
	return res;
}
