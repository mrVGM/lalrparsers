#pragma once

#include <set>
#include "Symbol.h"

namespace aux
{
	struct AugmentedRule
	{
	private:
		struct LAComp 
		{
			bool operator() (const Symbol& lhs, const Symbol& rhs) const {
				return lhs.name < rhs.name;
			}
		};
	public:

		struct Comp1 
		{
			bool operator() (const AugmentedRule& r1, const AugmentedRule& r2) const 
			{
				{
					if (r1.lhs.name != r2.lhs.name)
					{
						return r1.lhs.name < r2.lhs.name;
					}
					Symbols::const_iterator i1 = r1.rhs.begin();
					Symbols::const_iterator i2 = r2.rhs.begin();

					while (i1 < r1.rhs.end() && i2 < r2.rhs.end())
					{
						if ((*i1).name != (*i2).name)
						{
							return (*i1).name < (*i2).name;
						}
						++i1;
						++i2;
					}
					if (i1 == r1.rhs.end() && i2 == r2.rhs.end())
					{
						return false;
					}
					if (i1 == r1.rhs.end())
					{
						return true;
					}
					return false;
				}
			}
		};

		struct Comp2
		{
			bool operator() (const AugmentedRule& r1, const AugmentedRule& r2) const
			{
				Comp1 comp1;
				if (comp1(r1, r2) || comp1(r2, r1))
				{
					return comp1(r1, r2);
				}
				return r1.dotPosition < r2.dotPosition;
			}
		};

		Symbol lhs;
		Symbols rhs;
		int dotPosition;
		std::set<Symbol, Symbol::Comp1> lookAheadSymbols;
		AugmentedRule() : dotPosition(0) {}
		
		

		const Symbol* afterDot() const;
		void print() const;

		
	};
}

