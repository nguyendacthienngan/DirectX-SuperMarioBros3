#pragma once

const int PIPE_DEFAULT_TILE_SIZE = 48;

const int PIPE_START_X = 0;
const int PIPE_START_Y = 0;
const int PIPE_HEIGHT = 192;
const int PIPE_WIDTH = 672;


const D3DXVECTOR2 DEFAULT_OFFSET = D3DXVECTOR2(289, 44);

enum class PipeDirection
{
	Left, Right, Up, Down
};

