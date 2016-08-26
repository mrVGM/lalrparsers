#include "stdafx.h"
#include "AugmentedRule.h"

const aux::Symbol * aux::AugmentedRule::afterDot() const
{
	if (dotPosition < rhs.size())
	{
		return &rhs[dotPosition];
	}
	return 0;
}

#include <iostream>

void aux::AugmentedRule::print() const
{
	std::cout << lhs.name << " -> ";
	for (auto i = rhs.begin(); i < rhs.end(); ++i)
	{
		cout << (*i).name;
	}
	cout << " " << dotPosition << " ";
	for (auto i = lookAheadSymbols.begin(); i != lookAheadSymbols.end(); ++i)
	{
		cout << (*i).name;
	}
}
