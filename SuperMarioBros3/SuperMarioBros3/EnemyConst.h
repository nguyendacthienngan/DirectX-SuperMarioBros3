#pragma once

#define GOOMBA_STATE_IDLE		"IDLE"
#define GOOMBA_STATE_WALK		"WALK"
#define GOOMBA_STATE_SPIN		"SPIN"
#define GOOMBA_STATE_CROUCH		"CROUCH"
#define GOOMBA_STATE_DIE		"DIE"

#define KOOPA_STATE_IDLE		"IDLE"
#define KOOPA_STATE_MOVE		"MOVE"
#define KOOPA_STATE_SPIN		"SPIN"
#define KOOPA_STATE_CROUCH		"CROUCH"

#define KOOPA_SPEED				0.25f 
#define GOOMBA_SPEED			0.25f 

const D3DXVECTOR2 GOOMBA_BBOX	(16 * 3, 16 * 3);
const D3DXVECTOR2 KOOPA_BBOX	(16 * 3, 27 * 3);

#define GOOMBA_GRAVITY			0.00093f
#define KOOPA_GRAVITY			0.00093f

enum class EnemyType
{
	basic, red
};