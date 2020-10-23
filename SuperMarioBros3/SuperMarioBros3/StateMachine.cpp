#include "StateMachine.h"

void CStateMachine::SwitchState(IState* state)
{
	if (currentState != nullptr)
		currentState->Exit();

	this->currentState = state;
	currentState->Access();
}

void CStateMachine::Update()
{
	if (currentState != nullptr)
		currentState->Update();
}
