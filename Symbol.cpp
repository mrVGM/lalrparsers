#include "stdafx.h"
#include "Symbol.h"

aux::Symbol::operator string() const
{
	string res;
	res.push_back('"');
	for (auto i = name.begin(); i != name.end(); ++i)
	{
		if ((char)(*i) == '"' || (char)(*i) == '\\')
		{
			res.push_back('\\');
		}
		res.push_back(*i);
	}
	res.push_back('"');
	return res;
}
