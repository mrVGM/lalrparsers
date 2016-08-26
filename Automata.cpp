#include "stdafx.h"
#include "Automata.h"

void aux::Automata::regNode(automata::Node * node)
{
	if (!node->registered)
	{
		nodes.push_back(node);
		node->registered = true;
	}
}

void aux::Automata::addTransition(automata::Node * from, automata::Node * to, Symbol s, automata::Action * action)
{
	regNode(from);
	regNode(to);
	pair<automata::Node*, automata::Action*> &tmp = from->transitions[s];
	tmp.first = to;
	tmp.second = action;
}

void aux::Automata::setDefaultTransition(automata::Node * node, automata::Node * to, automata::Action * action)
{
	regNode(node);
	regNode(to);
	node->defaultTransition.first = to;
	node->defaultTransition.second = action;
}

#include <iostream>
bool aux::Automata::parse(Symbols & output, Symbols & input)
{
	aux.clear();
	automata::Node* cur = start;
	
	for (vector<Symbol>::iterator it = input.begin(); it < input.end(); ++it)
	{
		if (cur->transitions.find(*it) == cur->transitions.end())
		{
			cur->defaultTransition.second->apply(*it, output, aux);
			cur = cur->defaultTransition.first;
		}
		else
		{
			pair<automata::Node*, automata::Action*>  tmp = cur->transitions[*it];
			tmp.second->apply(*it, output, aux);
			cur = tmp.first;
		}
		
	}
	return cur->isFinal;
}

aux::Automata::~Automata()
{
	for (vector<automata::Node*>::iterator it = nodes.begin(); it < nodes.end(); ++it)
	{
		for (map<Symbol, pair<automata::Node*, automata::Action*> >::iterator itmap = (*it)->transitions.begin(); 
			itmap != (*it)->transitions.end(); ++itmap)
		{
			delete (*itmap).second.second;
		}
		delete *it;
	}
}

template <typename T>
void blank(aux::automata::Node *from, aux::automata::Node *to, aux::Automata &automata) {
	automata.addTransition(from, to, aux::Symbol(" "), new T());
	automata.addTransition(from, to, aux::Symbol("\t"), new T());
	automata.addTransition(from, to, aux::Symbol("\n"), new T());
}

aux::Automata *automataReader = 0;

aux::Automata & aux::automata::getReader()
{
	if (automataReader) 
	{
		return *automataReader;
	}
	else
	{
		typedef aux::automata::Node N;
		typedef aux::Symbol S;
		typedef aux::automata::Skip Skip;
		typedef aux::automata::ToMain ToMain;
		typedef aux::automata::ToAux ToAux;
		typedef aux::automata::Group Group;


		N* n1 = new N();
		N* n2 = new N();
		N* n3 = new N();
		N* n4 = new N();
		N* n5 = new N();
		N* n6 = new N();
		N* n7 = new N();
		N* n8 = new N();
		N* n9 = new N();
		N* n10 = new N();
		N* n11 = new N();
		N* n12 = new N();
		N* n13 = new N();

		n1->id = "n1";
		n2->id = "n2";
		n3->id = "n3";
		n4->id = "n4";
		n5->id = "n5";
		n6->id = "n6";
		n7->id = "n7";
		n8->id = "n8";
		n9->id = "n9";
		n10->id = "n10";
		n11->id = "n11";
		n12->id = "n12";
		n13->id = "n13";

		automataReader = new aux::Automata(n1);
		aux::Automata &a = *automataReader;

		class From : public aux::automata::Group {
		public:
			From() : Group("from") {}
		};

		class To : public aux::automata::Group {
		public:
			To() : Group("to") {}
		};

		class Act : public aux::automata::Group {
		public:
			Act() : Group("action") {}
		};


		blank<Skip>(n1, n1, a);
		a.addTransition(n1, n2, S("("), new Skip());
		a.setDefaultTransition(n1, n11, new Skip());

		blank<Skip>(n2, n2, a);
		a.addTransition(n2, n11, S(")"), new Skip());
		a.setDefaultTransition(n2, n3, new ToAux());

		blank<From>(n3, n4, a);
		a.addTransition(n3, n11, S(")"), new Skip());
		a.setDefaultTransition(n3, n3, new ToAux());

		blank<Skip>(n4, n4, a);
		a.addTransition(n4, n5, S("\""), new Skip());
		a.setDefaultTransition(n4, n11, new Skip());

		a.addTransition(n5, n6, S("\\"), new Skip());
		a.addTransition(n5, n7, S("\""), new Group("symbol"));
		a.setDefaultTransition(n5, n5, new ToAux());

		a.setDefaultTransition(n6, n5, new ToAux());

		blank<Skip>(n7, n8, a);
		a.setDefaultTransition(n7, n11, new Skip());

		blank<Skip>(n8, n8, a);
		a.addTransition(n8, n11, S(")"), new Skip());
		a.setDefaultTransition(n8, n9, new ToAux());

		blank<To>(n9, n10, a);
		a.addTransition(n9, n11, S(")"), new Skip());
		a.setDefaultTransition(n9, n9, new ToAux());

		blank<Skip>(n10, n10, a);
		a.addTransition(n10, n11, S(")"), new Skip());
		a.setDefaultTransition(n10, n12, new ToAux());

		blank<Act>(n12, n13, a);
		a.addTransition(n12, n1, S(")"), new Act());
		a.setDefaultTransition(n12, n12, new ToAux());

		blank<Skip>(n13, n13, a);
		a.addTransition(n13, n1, S(")"), new Skip());
		a.setDefaultTransition(n13, n11, new Skip());

		a.setDefaultTransition(n11, n11, new Skip());

		n1->isFinal = true;

		return *automataReader;
	}
}

void aux::automata::destroyAutomataReader()
{
	delete automataReader;
	automataReader = 0;
}

aux::Automata * aux::automata::createAutomataFromString(string definition, string start, vector<string> finals, map<string, Action*> actions)
{
	aux::Symbols def;
	for (string::iterator it = definition.begin(); it < definition.end(); ++it) 
	{
		string tmp;
		tmp.push_back(*it);
		def.push_back(aux::Symbol(tmp, tmp));
	}
	aux::Symbols t;
	aux::Automata &reader = aux::automata::getReader();

	if (!reader.parse(t, def)) 
	{
		return 0;
	}

	map<string, aux::automata::Node*> nodeNames;

	aux::Symbols::iterator it = t.begin();

	while (it < t.end()) 
	{
		aux::Symbol from = *it;
		++it;
		aux::Symbol symbol = *it;
		++it;
		aux::Symbol to = *it;
		++it;
		aux::Symbol action = *it;
		++it;

		if (nodeNames.find(from.data) == nodeNames.end())
		{
			nodeNames[from.data] = new aux::automata::Node();
		}
		if (nodeNames.find(to.data) == nodeNames.end())
		{
			nodeNames[to.data] = new aux::automata::Node();
		}
		if (actions.find(action.data) == actions.end())
		{
			for (map<string, aux::automata::Node*>::iterator i = nodeNames.begin(); i != nodeNames.end(); ++i)
			{
				delete (*i).second;
			}
			return 0;
		}
	}

	if (nodeNames.find(start) == nodeNames.end())
	{
		for (map<string, aux::automata::Node*>::iterator i = nodeNames.begin(); i != nodeNames.end(); ++i)
		{
			delete (*i).second;
		}
		return 0;
	}

	for (vector<string>::iterator j = finals.begin(); j < finals.end(); ++j)
	{
		if (nodeNames.find(*j) == nodeNames.end())
		{
			for (map<string, aux::automata::Node*>::iterator i = nodeNames.begin(); i != nodeNames.end(); ++i)
			{
				delete (*i).second;
			}
			return 0;
		}
		nodeNames[*j]->isFinal = true;
	}

	aux::Automata *res = new aux::Automata(nodeNames[start]);


	it = t.begin();

	while (it < t.end())
	{
		aux::Symbol from = *it;
		++it;
		aux::Symbol symbol = *it;
		++it;
		aux::Symbol to = *it;
		++it;
		aux::Symbol action = *it;
		++it;
		
		if (symbol.data.size() > 0) 
		{
			res->addTransition(nodeNames[from.data], nodeNames[to.data], aux::Symbol(symbol.data), actions[action.data]->copy());
		}
		else
		{
			res->setDefaultTransition(nodeNames[from.data], nodeNames[to.data], actions[action.data]->copy());
		}
	}

	return res;
}
