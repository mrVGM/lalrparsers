#include "stdafx.h"
#include "Parser.h"
#include "Grammar.h"
#include "StateGraph.h"

lalrParser::Parser::Parser(aux::LALRParser * p)
{
	refs = new int;
	*refs = 1;
	parser = p;

}

void lalrParser::Parser::destroy()
{
	(*refs)--;
	if (*refs == 0)
	{
		delete refs;
		delete parser;
	}
}

lalrParser::Parser::Parser(const Parser & other)
{
	refs = other.refs;
	(*refs)++;
	parser = other.parser;
}

lalrParser::Parser & lalrParser::Parser::operator=(const Parser & other)
{
	destroy();
	refs = other.refs;
	(*refs)++;
	parser = other.parser;
	return *this;
}

std::string lalrParser::Parser::serialize()
{
	return parser->serialize();
}

lalrParser::DerivationTree lalrParser::Parser::parse(const Symbols & symbols, bool & successful)
{
	aux::Tree * t = 0;
	t = parser->parse(symbols);

	if (t == 0)
	{
		t = new aux::Tree();
		successful = false;
	}
	else
	{
		successful = true;
	}
	return DerivationTree(t); 
}

lalrParser::Parser::~Parser()
{
	destroy();
}


lalrParser::Parser lalrParser::restore(string & ser)
{
	auto parser = aux::deserialize(ser);
	return Parser(parser);
}


lalrParser::Parser lalrParser::create(string & def)
{
	aux::Grammar *grammar = aux::readGrammarFromString(def);
	auto stateGraph = aux::StateGraph(*grammar);
	auto parser = stateGraph.createParser(*grammar);
	return Parser(parser);
}
