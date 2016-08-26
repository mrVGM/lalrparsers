#include "stdafx.h"
#include "DerivationTree.h"

lalrParser::DerivationTree::DerivationTree(aux::Tree * t)
{
	refs = new int;
	*refs = 1;
	tree = t;
}

void lalrParser::DerivationTree::destroy()
{
	(*refs)--;
	if (*refs == 0)
	{
		delete refs;
		delete tree;
	}
}

lalrParser::DerivationTree::DerivationTree(const DerivationTree & other)
{
	refs = other.refs;
	(*refs)++;
	tree = other.tree;
}

lalrParser::DerivationTree & lalrParser::DerivationTree::operator=(const DerivationTree & other)
{
	destroy();
	refs = other.refs;
	(*refs)++;
	tree = other.tree;
	return *this;
}

int lalrParser::DerivationTree::numberOfChildren()
{
	return tree->n;
}

lalrParser::DerivationTree lalrParser::DerivationTree::operator[](int index) const
{
	DerivationTree dt(tree->children[index]);
	*dt.refs = 2;
	return dt;
}

aux::Symbol lalrParser::DerivationTree::getData()
{
	return tree->data;
}

lalrParser::DerivationTree::~DerivationTree()
{
	destroy();
}
