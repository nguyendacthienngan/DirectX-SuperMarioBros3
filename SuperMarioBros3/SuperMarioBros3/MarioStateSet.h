#pragma once

enum class MoveOnGroundStates
{
	Idle, Walk, Run, HighSpeed, Skid, Crouch, Attack, JumpAttack
};

enum class JumpOnAirStates
{
	Stand, Jump, HighJump, Fall, LowJump, Fly
};

struct MarioStateSet
{
	MoveOnGroundStates move;
	JumpOnAirStates jump;
};

enum class MarioStates
{
	SmallMario, SuperMario, FireMario, RacoonMario
};