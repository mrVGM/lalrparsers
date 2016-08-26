#pragma once

#include <map>
#include <vector>
#include "Symbol.h"

namespace aux {
	namespace automata {
		class Action {
		public:
			virtual void apply(Symbol s, Symbols &main, Symbols &aux) = 0;
			virtual Action* copy() = 0;
		};
		struct Node {
		private:
			struct Comp {
				bool operator() (const Symbol& lhs, const Symbol& rhs) const {
					return lhs.name < rhs.name;
				}
			};
		public:
			string id;
			bool isFinal;
			std::map<Symbol, std::pair<Node*, Action*>, Comp> transitions;
			pair<Node*, Action*> defaultTransition;
			bool registered;
			Node() {
				isFinal = false;
				registered = false;
			}
		};
		
		class Skip : public Action
		{
		public:
			void apply(Symbol s, Symbols &main, Symbols &aux) {}
			Action *copy() 
			{
				Skip *res = new Skip(*this);
				return res;
			}
		};
		class ToMain : public Action
		{
		public:
			void apply(Symbol s, Symbols &main, Symbols &aux)
			{
				main.push_back(s);
			}
			Action *copy()
			{
				ToMain *res = new ToMain(*this);
				return res;
			}
		};
		class ToAux : public Action
		{
		public:
			void apply(Symbol s, Symbols &main, Symbols &aux)
			{
				aux.push_back(s);
			}
			Action *copy()
			{
				ToAux *res = new ToAux(*this);
				return res;
			}
		};
		class Group : public Action
		{
		public:
			string groupName;
			Group(string _groupName) : groupName(_groupName) {}
			void apply(Symbol s, Symbols &main, Symbols &aux)
			{
				Symbol ss;
				ss.name = groupName;
				for (vector<Symbol>::iterator it = aux.begin(); it != aux.end(); ++it)
				{
					ss.data += (*it).data;
				}
				main.push_back(ss);
				aux.clear();
			}
			Action *copy()
			{
				Group *res = new Group(*this);
				return res;
			}
		};

	}
	
	class Automata
	{
	private:
		std::vector<automata::Node*> nodes;
		Symbols aux;
		automata::Node *start;
		void regNode(automata::Node *node);
	public:
		Automata(automata::Node *_start) { start = _start; }
		void addTransition(automata::Node *from, automata::Node *to, Symbol s, automata::Action *action);
		void setDefaultTransition(automata::Node *node, automata::Node *to, automata::Action *action);
		bool parse(Symbols &output, Symbols &input);
		~Automata();
	};

	namespace automata
	{
		Automata& getReader();
		void destroyAutomataReader();
		Automata *createAutomataFromString(string definition, string start, vector<string> finals, map<string, Action*> actions);
	}
}