-----------------------------------------------------------------------------------------------------------------------
-- T_Player
-----------------------------------------------------------------------------------------------------------------------
T_Player = class(T_ActorCtrl,
{
	_name = "T_Player",
	_val0 = 100,
	_val1 = 101,
});

function T_Player:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", "T_Player:OnInitUpdate")
end
