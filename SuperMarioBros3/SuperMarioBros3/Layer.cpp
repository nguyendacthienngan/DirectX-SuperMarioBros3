#include "Layer.h"

#include <string>
#include <vector>
#include "Ultis.h"
using namespace std;
CLayer::CLayer()
{
	this->id = 0;
	this->width = 1;
	this->height = 1;
}

CLayer::CLayer(TiXmlElement* data)
{
	data->QueryIntAttribute("id", &this->id);
	data->QueryIntAttribute("width", &this->width);
	data->QueryIntAttribute("height", &this->height);

	tiles = new int* [this->width]; // ?

	const char* content = data->FirstChildElement()->GetText();
	vector<string> splitted = split(content, ",");

	for (int i = 0; i < this->width; i++) {
		tiles[i] = new int[height];
		for (int j = 0; j < this->height; j++) {
			tiles[i][j] = stoi(splitted[i + j * width]); // ?
		}
	}

	splitted.clear();
}

CLayer::~CLayer()
{
	for (int i = 0; i < width; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}

int CLayer::GetTileID(int x, int y)
{
	return tiles[x][y];
}
