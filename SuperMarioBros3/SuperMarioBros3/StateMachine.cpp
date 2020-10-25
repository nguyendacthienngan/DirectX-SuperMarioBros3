#include "StateMachine.h"
#include "Ultis.h"

void CStateMachine::SwitchState(IState* state)
{
	if (currentState != nullptr)
	{
		currentState->Exit();
		DebugOut(L"Exit \n");
	}

	this->currentState = state;
	currentState->Access();
}

void CStateMachine::Update()
{

	if (currentState != nullptr)
		currentState->Update();
}
