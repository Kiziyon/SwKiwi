#include "caver/components.h"
#include "caver/scene_object.h"
#include "core/hooks.h"
#include "lauxlib.h"
#include "lua_libs/libs.h"

STATIC_DL_FUNCTION_SYMBOL(
	StopMoving,
	"_ZN5Caver27CharAnimControllerComponent10StopMovingEv",
	void, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	StopAction,
	"_ZN5Caver27CharAnimControllerComponent10StopActionEv",
	void, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	StartMoving,
	"_ZN5Caver27CharAnimControllerComponent11StartMovingEv",
	void, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	BeginCasting,
	"_ZN5Caver27CharAnimControllerComponent12BeginCastingEv",
	void, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	StartFalling,
	"_ZN5Caver27CharAnimControllerComponent12StartFallingEv",
	void, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	IsReadyToJump,
	"_ZN5Caver27CharAnimControllerComponent13IsReadyToJumpEv",
	bool, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	ActionNearlyFinished,
	"_ZN5Caver27CharAnimControllerComponent13IsReadyToJumpEv",
	bool, (void* this)
)

STATIC_DL_FUNCTION_SYMBOL(
	IsMoving,
	"_ZN5Caver27CharAnimControllerComponent8IsMovingEv",
	bool, (void* this)
)

static int c_stopmoving(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;

	StopMoving(c);
	return 0;
}

static int c_stopaction(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;

	StopAction(c);
	return 0;
}

static int c_startmoving(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;

	StartMoving(c);
	return 0;
}

static int c_begincasting(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;

	BeginCasting(c);
	return 0;
}

static int c_startfalling(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;

	StartFalling(c);
	return 0;
}

static int c_isready(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;
	bool isReady = IsReadyToJump(c);
	lua_pushboolean(L, isReady);
	return 1;
}

static int c_ismoving(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;
	bool isMoving = IsMoving(c);
	lua_pushboolean(L, isMoving);
	return 1;
}

static int c_nearlyfinished(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	if (!o || !*o) return 0;

	void *c = SceneObject_ComponentWithInterface(
		*o, CharAnimControllerComponent_Interface);

	if (!c) return 0;
	bool nearlyFinished = ActionNearlyFinished(c);
	lua_pushboolean(L, nearlyFinished);
	return 1;
}

static const luaL_Reg button_library[] = {
	{"StopMoving", c_stopmoving},
	{"StopAction", c_stopaction},
	{"StartMoving", c_startmoving},
	{"BeginCasting", c_begincasting},
	{"StartFalling", c_startfalling},
	{"IsReadyToJump", c_isready},
	{"IsMoving", c_ismoving},
	{"ActionNearlyFinished", c_nearlyfinished},
	{NULL, NULL}
};

int miniLL_open_charanim(lua_State *L) {
	lua_newtable(L);
	luaL_register(L, NULL, button_library);
	return 1;
}

// Init...

void initLL_charanim() {
	dlsym_StopMoving();
	dlsym_StopAction();
	dlsym_StartMoving();
	dlsym_BeginCasting();
	dlsym_StartFalling();
	dlsym_IsReadyToJump();
	dlsym_ActionNearlyFinished();
	dlsym_IsMoving();
}