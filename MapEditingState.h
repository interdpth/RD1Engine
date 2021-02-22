#pragma once
enum editingActions {
	PLACETILE,
	RESIZE,
	MOVE
};
enum  editingStates {
	MAP=0,
	TILESET=1,
	DOOR=2, 
	SPRITE=3,
	SCROLL=4
};
class MapEditingState
{
private:
	editingStates lastState;
	editingStates curState;

	editingActions curAction;
	editingActions lastAction;
	int stateObjId;
	int lastStateObjId;
public:
	bool SetAction(editingActions newAction);
	int GetStateObjectID();
	int GetLastStateObjectID();
	editingActions  GetLastAction();
	editingActions  GetAction();
	editingStates GetLastState();
	editingStates GetState(); 
	
	bool SetState(editingStates newState);
	int GetObjId();
	bool SetAction(int obj, editingActions newAction);
	bool SetObjId(int obj);
	MapEditingState();
	~MapEditingState();
};

