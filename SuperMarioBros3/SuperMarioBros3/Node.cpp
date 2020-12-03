#include "Node.h"
using namespace std;
CNode::CNode()
{
}
CNode::CNode(D3DXVECTOR2 size)
{
	
	graph = NULL;
}

std::vector<int> CNode::GetAdjacencyNodes()
{
	return adjacencyNodes;
}

int CNode::GetNodeID()
{
	return nodeID;
}

void CNode::SetNodeID(int id)
{
	this->nodeID = id;
}

void CNode::AddAdjacencyNode(int adjIndex)
{
	adjacencyNodes.push_back(adjIndex);
}
