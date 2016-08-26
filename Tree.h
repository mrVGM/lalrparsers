#pragma once

#include "Symbol.h"

namespace aux {
	class Tree
	{

	public:
		int n;
		Tree ** children;
		Symbol data;
		Tree(int _n = 0);
		~Tree();
	};
}