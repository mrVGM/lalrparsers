#include "stdafx.h"
#include "Tree.h"

aux::Tree::Tree(int _n)
{
	n = _n;
	children = new Tree *[n];
	for (int i = 0; i < n; i++)
	{
		children[i] = 0;
	}
}

#include <stack>

aux::Tree::~Tree()
{
	Tree * obj = this;
	if (n == 0)
	{
		return;
	}
	std::stack<Tree*> st;
	for (int i = 0; i < n; i++)
	{
		if (children[i] != 0)
			st.push(children[i]);
	}
	delete children;

	while (!st.empty())
	{
		Tree * cur = st.top();
		st.pop();
		for (int i = 0; i < cur->n; i++)
		{
			if (cur->children[i] != 0)
				st.push(cur->children[i]);
		}
		delete cur->children;
		cur->n = 0;
		delete cur;
	}
}
