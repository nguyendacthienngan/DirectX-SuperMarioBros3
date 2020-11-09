﻿#pragma once

#define MARIO_GRAVITY					0.00093f

#define MARIO_WALKING_SPEED				0.24 
#define MARIO_RUNNING_SPEED				0.38f	
#define MARIO_HIGHSPEED_SPEED			0.53f	
#define MARIO_JUMP_SPEED_Y				0.35f

#define MARIO_WALKING_DRAG_FORCE		0.0002506f
#define MARIO_RUNNING_DRAG_FORCE		0.0006266f
#define MARIO_JUMP_FORCE				0.3f
#define MARIO_HIGH_JUMP_FORCE			0.6f
#define MARIO_SUPER_JUMP_FORCE			0.53f
#define MARIO_PUSH_FORCE				0.3f
#define MARIO_FLY_FORCE					0.0002f
#define RACCOON_FLOAT_VELOCITY			0.07f
#define RACCOON_FLY_VELOCITY			-0.2f
#define MARIO_HIGH_JUMP_HEIGHT			180.5f
#define MARIO_SUPER_JUMP_HEIGHT			200.5f

#define MARIO_RUNNING_ACCELERATION		0.000376f
#define MARIO_WALKING_ACCELERATION		0.000350f
#define MARIO_SKID_FORCE				0.001104f

#define MARIO_DEFLECT_TIME				800
#define MARIO_DEFLECT_Y					0.4f
#define MARIO_DEFLECT_X					0.3f

#define MARIO_FEVER_TIME				2000

#define PMETER_MAX						5
#define PMETER_STEP						0.005f

#define FLYING_TIME						4000
#define FLOATING_TIME					5000
#define ATTACKING_TIME					300

#define TIME_TO_PRESS_S_TO_FLY			200
#define TIME_TO_PRESS_S_TO_FLOAT		1000

#define MARIO_BOUNCE_FORCE				0.3f

#define MARIO_STATE_IDLE				"IDLE"
#define MARIO_STATE_WALKING				"WALK"
#define MARIO_STATE_RUNNING				"RUN"
#define MARIO_STATE_HIGH_SPEED			"HIGH-SPEED"
#define MARIO_STATE_JUMP				"JUMP"
#define MARIO_STATE_HIGH_JUMP			"HIGH-JUMP"
#define MARIO_STATE_CROUCH				"CROUCH"
#define MARIO_STATE_SKID				"SKID"
#define MARIO_STATE_FALL				"FALL"
#define MARIO_STATE_ATTACK				"ATTACK"
#define MARIO_STATE_JUMP_ATTACK			"JUMP-ATTACK"
#define MARIO_STATE_FLY					"FLY"
#define MARIO_STATE_FULLFLY				"FULL-FLY"
#define MARIO_STATE_FLOAT				"FLOAT"
#define MARIO_STATE_HOLD_MOVE			"HOLD-MOVE"
#define MARIO_STATE_HOLD_IDLE			"HOLD-IDLE"
#define MARIO_STATE_KICK				"KICK"


#define SPACE_MARIO_MOVING_IN_CAMERA	30

#define BIG_MARIO_BBOX_WIDTH			42
#define BIG_MARIO_BBOX_HEIGHT			81
#define BIG_MARIO_CROUCH_BBOX_WIDTH		42
#define BIG_MARIO_CROUCH_BBOX_HEIGHT	42

const D3DXVECTOR2 SUPER_MARIO_BBOX			(42, 81);
const D3DXVECTOR2 SUPER_MARIO_CROUCH_BBOX	(42, 42);
const D3DXVECTOR2 SMALL_MARIO_BBOX			(36, 45);


