#pragma once

#include <string>
#include <vector>

using namespace std;

namespace aux {

	struct Symbol
	{
	public:
		string name;
		string data;
		struct Comp1 
		{
			bool operator() (const Symbol& s1, const Symbol& s2) const
			{
				return s1.name < s2.name;
			}
		};
		Symbol(string _name = "", string _data = "")
		{
			name = _name;
			data = _data;
		}
		operator string() const;
		
	};

	typedef vector<Symbol> Symbols;

}