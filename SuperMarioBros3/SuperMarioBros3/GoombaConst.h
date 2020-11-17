#pragma once

#define GOOMBA_STATE_IDLE			"IDLE"
#define GOOMBA_STATE_WALK			"WALK"
#define GOOMBA_STATE_DIE			"DIE"
#define GOOMBA_STATE_HEADSHOT		"HEAD-SHOT"

const int GOOMBA_DIE_TIME = 300;

const float GOOMBA_GRAVITY = 0.00093f*2;

const float GOOMBA_SPEED = 0.039f*2;
const float GOOMBA_HIT_FORCE = 0.5f *1.5f;
const float GOOMBA_HIT_MAX_HEIGHT = 170.0f *1.5f;
const float GOOMBA_HIT_FORCE_X = 0.08f*2;

const D3DXVECTOR2 GOOMBA_BBOX		(16 * 3, 16 * 3);
const D3DXVECTOR2 GOOMBA_DIE_BBOX	(16 * 3, 9 * 3);

enum class GoombaState
{
	Walk, Die, HeadShot
};