/* clang-format off */
#include "mini.h" /* clang-format on */

#include "core/hooks.h"
#include "core/log.h"
#include "lua.h"

#define LOG_TAG "MiniLuaDebug"

int miniLL_toggle_debug(lua_State *L) {
	bool debugEnabled = lua_toboolean(L, 1);
	lua_getglobal(L, "scene");
	const void *Scene = lua_topointer(L, -1);
	LOGD("Found Scene: %p", Scene);

	*$(bool, Scene, 0x208, 0x358) = debugEnabled;

	return 0;
}

void initLL_debug() {}
