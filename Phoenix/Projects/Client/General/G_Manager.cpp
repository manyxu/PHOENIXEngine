// G_Manage.cpp

#include "G_Manager.hpp"
#include "PX2ProjectEvent.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
G_Man::G_Man()
{
}
//----------------------------------------------------------------------------
G_Man::~G_Man()
{
}
//----------------------------------------------------------------------------
void G_Man::Initlize()
{
	PX2_EW.ComeIn(this);
}
//----------------------------------------------------------------------------
void G_Man::Terminate()
{
	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void G_Man::Update()
{
}
//----------------------------------------------------------------------------
void G_Man::_Play()
{
	mIsPlaying = true;
}
//----------------------------------------------------------------------------
void G_Man::_Stop()
{
	mIsPlaying = false;
}
//----------------------------------------------------------------------------
void G_Man::OnEvent(Event *event)
{
	if (ProjectES::IsEqual(event, ProjectES::Play))
	{
		_Play();
	}
	else if (ProjectES::IsEqual(event, ProjectES::Stop))
	{
		_Stop();
	}

	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------