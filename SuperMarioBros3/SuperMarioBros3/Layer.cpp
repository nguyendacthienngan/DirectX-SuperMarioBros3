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

	tiles = new int* [this->width]; // số tiles theo chiều ngang

	const char* content = data->FirstChildElement()->GetText();
	vector<string> splitted = split(content, ","); // Trả về chuỗi các tileID trong layer bằng cách tách dấu phẩy ra [ Lúc này chỉ là 1 mảng 1 chiều ]

	for (int i = 0; i < this->width; i++) 
	{
		tiles[i] = new int[height];  // số tiles theo chiều dọc
		for (int j = 0; j < this->height; j++) 
		{
			tiles[i][j] = stoi(splitted[i + j * width]); // stoi giúp chuyển từ string sang int
			// Đem về mảng 2 chiều
		}
	}

	// Splitted lưu trữ dưới dạng: 0 1 2 3 4 5 6 7 8 9
	// Còn ma trận ta muốn lưu nó sẽ như vầy
	//     0 1 2 3 4 
	//     5 6 7 8 9
	// Ta có i = 0, j = 1
	// Thì muốn lấy 5 ra thì ta phải lấy 0 + 1 * 5 ( i + j * width )
	splitted.clear();
}

CLayer::~CLayer()
{
	for (int i = 0; i < width; i++) 
	{
		delete[] tiles[i];
	}
	delete[] tiles;
}

int CLayer::GetTileID(int x, int y)
{
	return tiles[x][y];
}
