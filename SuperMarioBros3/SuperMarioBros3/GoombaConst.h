#pragma once

#define GOOMBA_STATE_IDLE			"IDLE"
#define GOOMBA_STATE_WALK			"WALK"
#define GOOMBA_STATE_DIE			"DIE"
#define GOOMBA_STATE_HEADSHOT		"HEAD-SHOT"

#define GOOMBA_DIE_TIME				300
#define GOOMBA_SPEED				0.039f 

#define GOOMBA_HIT_FORCE			0.2f
#define GOOMBA_HIT_MAX_HEIGHT		170.0f

const D3DXVECTOR2 GOOMBA_BBOX		(16 * 3, 16 * 3);
const D3DXVECTOR2 GOOMBA_DIE_BBOX	(16 * 3, 9 * 3);

#define GOOMBA_GRAVITY				0.00093f

enum class GoombaState
{
	Walk, Die, HeadShot
};