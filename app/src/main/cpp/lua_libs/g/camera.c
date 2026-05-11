#include "caver/types.h"
#include "core/hooks.h"
#include "core/log.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua_libs/libs.h"

#define LOG_TAG "MiniCamera"

static void *get_cc(lua_State *L) {
	lua_getglobal(L, "cameraController");

	if (!lua_islightuserdata(L, -1)) {
		LOGE("Could not find cameraController!");
		luaL_error(L, "Could not find cameraController!");
		return NULL;
	}

	void *cc = (void *)lua_topointer(L, -1);
	lua_pop(L, 1);

	return cc;
}

static void push_vector3_table(lua_State *L, Vector3 v) {
	lua_newtable(L);

	lua_pushnumber(L, v.x);
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, v.y);
	lua_rawseti(L, -2, 2);

	lua_pushnumber(L, v.z);
	lua_rawseti(L, -2, 3);
}

static Vector3 check_vector3_table(lua_State *L, int index) {
	Vector3 v = {0};

	luaL_checktype(L, index, LUA_TTABLE);

	lua_rawgeti(L, index, 1);
	v.x = (float)luaL_optnumber(L, -1, 0.0f);
	lua_pop(L, 1);

	lua_rawgeti(L, index, 2);
	v.y = (float)luaL_optnumber(L, -1, 0.0f);
	lua_pop(L, 1);

	lua_rawgeti(L, index, 3);
	v.z = (float)luaL_optnumber(L, -1, 0.0f);
	lua_pop(L, 1);

	return v;
}

static int SetCameraOffset(lua_State *L) {
	void *cc = get_cc(L);

	Vector3 offset = check_vector3_table(L, 1);
	// CameraController + 0x04
	*$(Vector3, cc, 0x04, 0x04) = offset;

	return 0;
}

static int GetCameraOffset(lua_State *L) {
	void *cc = get_cc(L);

	// CameraController + 0x04
	Vector3 offset = *$(Vector3, cc, 0x04, 0x04);
	push_vector3_table(L, offset);

	return 1;
}

static int SetUpVector(lua_State *L) {
	void *cc = get_cc(L);

	Vector3 up = check_vector3_table(L, 1);
	// CameraController + 0x48
	*$(Vector3, cc, 0x48, 0x48) = up;

	return 0;
}

static int GetUpVector(lua_State *L) {
	void *cc = get_cc(L);

	// CameraController + 0x48
	Vector3 up = *$(Vector3, cc, 0x48, 0x48);
	push_vector3_table(L, up);

	return 1;
}

static const luaL_Reg camera_library[] = {
	{"SetPositionOffset", SetCameraOffset},
	{"GetPositionOffset", GetCameraOffset},

	{"SetUpVector", SetUpVector},
	{"GetUpVector", GetUpVector},

	{NULL, NULL}
};

int miniLL_open_camera(lua_State *L) {
	lua_newtable(L);
	luaL_register(L, NULL, camera_library);
	return 1;
}