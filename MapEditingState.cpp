#include "MapEditingState.h"



MapEditingState::MapEditingState()
{
	lastState = editingStates::MAP;
	curState = editingStates::MAP;

	editingActions curAction = editingActions::PLACETILE;
	editingActions lastAction = editingActions::PLACETILE;
	stateObjId = 0;
	lastStateObjId = 0;
}


MapEditingState::~MapEditingState()
{
}

editingStates MapEditingState::GetState()
{
	return curState;
}
bool MapEditingState::SetState(editingStates newState)
{
	lastState = curState;
	curState = newState;
	return lastState == newState;
}


bool MapEditingState::SetAction(editingActions newAction)
{
	lastAction = curAction;
	curAction = newAction;
	return lastAction == newAction;
}
int MapEditingState::GetObjId() {
	return stateObjId;
}
bool MapEditingState::SetObjId(int obj) {
	if (obj == -1) {
		lastStateObjId = stateObjId;
	}
	lastStateObjId = stateObjId;
	stateObjId = obj;
	return stateObjId == obj;
}


int MapEditingState::GetStateObjectID()
{
	return stateObjId;
}
int MapEditingState::GetLastStateObjectID()
{
	return lastStateObjId;
}
editingActions MapEditingState::GetLastAction() {
	return lastAction;
}
editingActions MapEditingState::GetAction() {
	return curAction;
}
editingStates MapEditingState::GetLastState() {
	return lastState;
}