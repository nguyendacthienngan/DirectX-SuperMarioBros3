#include "KeyEventHandler.h"
using namespace std;
void CKeyEventHandler::AddCurrentTarget(CGameObject* target)
{
	this->currentTargets.push_back(target);
}

