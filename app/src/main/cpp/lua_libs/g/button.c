#include "lauxlib.h"
#include "lua.h"
#include "lua_libs/lni/lni.h"

#define LNI_FUNC(name, java) \
	static int name(lua_State *L) { return lni_exec_name(L, java); }

LNI_FUNC(newButton, "NewButton")
LNI_FUNC(makeMovable, "MakeMovable")
LNI_FUNC(deleteButton, "RemoveButton")

LNI_FUNC(buttonPressed, "ButtonPressed")
LNI_FUNC(buttonDragged, "ButtonDragged")

LNI_FUNC(removeAll, "RemoveAllButtons")
LNI_FUNC(setScaling, "SetScaling")

// Text functions
LNI_FUNC(setText, "SetText")
LNI_FUNC(setTextScale, "SetTextScale")
LNI_FUNC(setTextColor, "SetTextColor")

LNI_FUNC(getPositionX, "GetPositionX")
LNI_FUNC(getPositionY, "GetPositionY")
LNI_FUNC(setPosition, "SetPosition")

LNI_FUNC(setBGR, "SetBackgroundResource")

static const luaL_Reg button_library[] = {
	{"New", newButton},
	{"MakeMovable", makeMovable},
	{"Delete", deleteButton},

	{"IsPressed", buttonPressed},
	{"IsDragging", buttonDragged},

	{"DeleteAll", removeAll},
	{"SetScaling", setScaling},

	{"SetText", setText},
	{"SetTextScale", setTextScale},
	{"SetTextColor", setTextColor},

	{"GetPositionX", getPositionX},
	{"GetPositionY", getPositionY},

	{"SetPosition", setPosition},
	{"SetBackgroundResource", setBGR},

	{NULL, NULL}
};

int miniLL_open_button(lua_State *L) {
	lua_newtable(L);
	luaL_register(L, NULL, button_library);
	return 1;
}