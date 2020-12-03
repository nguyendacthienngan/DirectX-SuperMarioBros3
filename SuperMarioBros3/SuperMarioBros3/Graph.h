#pragma once
#include "Node.h"
#include <vector>
class CNode;
class CGraph
{
	std::vector<CNode*> listNodes; // undirected graph
public:
	CGraph();
	std::vector<CNode*> GetListNodes();
	CNode* GetNodeByID(int id);
	void AddNode(CNode* node);
};