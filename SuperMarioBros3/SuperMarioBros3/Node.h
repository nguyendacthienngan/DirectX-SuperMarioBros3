#pragma once
#include <d3dx9math.h>
#include <vector>
#ifndef NODE_H
#define NODE_H
#include "Graph.h"
class CGraph;
class CNode
{
protected:
	CGraph* graph;
	int nodeID;
	std::vector<int> adjacencyNodes; // Lưu danh sách các node kề với node hiện tại
public:
	CNode();
	CNode(D3DXVECTOR2 size);
	std::vector<int> GetAdjacencyNodes();
	int GetNodeID();
	void SetNodeID(int id);
	void AddAdjacencyNode(int adjIndex);
};

#endif
