#pragma once

#define GOOMBA_STATE_IDLE		"IDLE"
#define GOOMBA_STATE_WALK		"WALK"
#define GOOMBA_STATE_SPIN		"SPIN"
#define GOOMBA_STATE_CROUCH		"CROUCH"
#define GOOMBA_STATE_DIE		"DIE"

#define GOOMBA_SPEED			0.039f 

const D3DXVECTOR2 GOOMBA_BBOX(16 * 3, 16 * 3);

#define GOOMBA_GRAVITY			0.00093f

enum class GoombaState
{
	Walk, Die
};