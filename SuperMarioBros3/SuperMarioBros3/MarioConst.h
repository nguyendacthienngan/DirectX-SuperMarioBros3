﻿#pragma once

#define MARIO_START_X					94.0f // left,top của BB Mario k đồng bộ với vị trí mario
#define MARIO_START_Y					1150.0f // Tọa độ theo hệ quy chiếu world
//#define MARIO_START_Y					1165 // Tọa độ theo hệ quy chiếu world
//#define MARIO_START_Y					1100 // Tọa độ theo hệ quy chiếu world

#define MARIO_GRAVITY					0.00093f

#define MARIO_WALKING_SPEED				0.1f 
#define MARIO_JUMP_SPEED_Y				0.3f
#define MARIO_RUNNING_SPEED				0.3f	
#define MARIO_HIGHSPEED_SPEED			0.45f	

#define MARIO_WALKING_DRAG_FORCE		0.0138f
#define MARIO_RUNNING_DRAG_FORCE		0.0069f

#define MARIO_WALKING_ACCELERATION		0.00276f
#define MARIO_RUNNING_ACCELERATION		0.0000001f

#define MARIO_STATE_IDLE				"IDLE"
#define MARIO_STATE_WALKING				"WALK"
#define MARIO_STATE_RUNNING				"RUN"
#define MARIO_STATE_HIGH_SPEED			"HIGH-SPEED"
#define MARIO_STATE_JUMP				"JUMP"
#define MARIO_STATE_HIGH_JUMP			"HIGH-JUMP"
#define MARIO_STATE_CROUCH				"CROUCH"

#define SPACE_MARIO_MOVING_IN_CAMERA	30

#define BIG_MARIO_BBOX_WIDTH			42
#define BIG_MARIO_BBOX_HEIGHT			81
#define BIG_MARIO_CROUCH_BBOX_WIDTH		42
#define BIG_MARIO_CROUCH_BBOX_HEIGHT	42

const D3DXVECTOR2 BIG_MARIO_BBOX			(42, 81);
const D3DXVECTOR2 BIG_MARIO_CROUCH_BBOX		(42, 42);
const D3DXVECTOR2 SMALL_MARIO_CROUCH_BBOX	(36, 45);