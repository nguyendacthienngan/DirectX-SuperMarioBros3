#pragma once
#include "IState.h"
class CStateMachine
{
protected:
	IState* currentState;
public:
	// Nếu có state hiện tại thì thực hiện thoát state hiện tại (Exit)
	// Chuyển state hiện tại sang state được truyền vô
	// Gọi sự kiện vào state mới (Access)
	virtual void SwitchState(IState* state);
	virtual void Process();
};

