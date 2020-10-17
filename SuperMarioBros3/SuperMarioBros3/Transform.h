#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <d3d9.h>
#include <d3dx9.h>
struct Transform
{
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	float rotationAngle; // nhận vào là độ. Khi vào draw phải chuyển sang rad
	
	Transform CreateTransformation(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotationAngle)
	{
		Transform result;
		result.position = position;
		result.scale = scale;
		result.rotationAngle = rotationAngle;
		return result;
	}
};


#endif