#pragma once
// Interface  state
// Là một trạng thái
class IState
{
public:
	virtual void Access() = 0; // Sự kiện vào state 
	virtual void Process() = 0;
	virtual void Exit() = 0; // Sự kiện khi thoát khỏi state
};

