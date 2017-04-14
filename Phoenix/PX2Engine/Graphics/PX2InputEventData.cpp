// PX2InputEventData.cpp

#include "PX2InputEventData.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
TouchState::TouchState()
{
}
//----------------------------------------------------------------------------
TouchState::~TouchState()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
InputEventData::InputEventData() :
ViewID(0),
TheEventType(0),
MWheel(0.0f)
{
	KCode = KC_UNASSIGNED;
	MButtonID = MBID_MAX;
}
//----------------------------------------------------------------------------
InputEventData::~InputEventData()
{
}
//----------------------------------------------------------------------------
bool InputEventData::IsKeyEvent() const
{
	return TheEventType == InputEventSpace::KeyPressed ||
		TheEventType == InputEventSpace::KeyReleased;
}
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CanvasInputData::CanvasInputData() :
TheMouseTag(MT_NONE),
Wheel(0.0f),
PickType(UIPT_NONE)
{
}
//-----------------------------------------------------------------------------
CanvasInputData::~CanvasInputData()
{
}
//----------------------------------------------------------------------------