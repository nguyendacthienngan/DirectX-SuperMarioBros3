#pragma once

enum class MoveOnGroundStates
{
	Idle, Walk, Run, HighSpeed, Skid, Crouch, Attack
};

enum class JumpOnAirStates
{
	Stand, Jump, HighJump, Fall, LowJump
};

struct MarioStateSet
{
	MoveOnGroundStates move;
	JumpOnAirStates jump;
};