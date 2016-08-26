#pragma once

#include "LALRParser.h"

namespace lalrParser {
	
	class DerivationTree
	{
		friend class Parser;
	private:
		aux::Tree * tree;
		int *refs;
		DerivationTree(aux::Tree * t);

		void destroy();
		
	public:
		DerivationTree(const DerivationTree & other);
		DerivationTree & operator=(const DerivationTree & other);
		int numberOfChildren();
		DerivationTree operator[](int index) const;
		aux::Symbol getData();
		~DerivationTree();
	};

}