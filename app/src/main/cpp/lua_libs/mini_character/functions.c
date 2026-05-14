#include "caver/scene_object.h"
#include "core/hooks.h"
#include "core/log.h"
#include "caver/components.h"
#include "lua/lua.h"
#include "mini_character.h"

#define LOG_TAG "MiniCharacter"

// afk...

static SceneObject *get_hero(lua_State *L) {
	lua_getglobal(L, "gameController");
	if (!lua_islightuserdata(L, -1)) {
		LOGE("Could not find gameController!");
		return NULL;
	}
	void *gameController = lua_touserdata(L, -1);
	void *game_scene_controller = *$(void*, gameController, 0x68, 0xc8);
	if (!game_scene_controller) {
		LOGE("GameSceneController is null");
		return NULL;
	}
	void *hero = *$(void*, game_scene_controller, 0xa4, 0xd8);
	if (!hero) {
		LOGE("Hero is null");
		return NULL;
	}
	return (SceneObject *) hero;
}

//STATIC_DL_FUNCTION_SYMBOL(
//	DropQuickly,
//	"_ZN5Caver23CharControllerComponent11DropQuicklyEv",
//	void, (void *CharControllerComponent)
//)
//
//static int voidFunc(lua_State *L) {
//	SceneObject *hero = get_hero(L);
//	if (!hero) return 0;
//	void *component = SceneObject_ComponentWithInterface(hero, CharControllerComponent_Interface);
//	DropQuickly(component);
//}

#define VOID_FUNC(NAME, SYM) \
STATIC_DL_FUNCTION_SYMBOL( \
	NAME, \
	SYM, \
	void, (void *CharControllerComponent) \
)                            \
static int L_##NAME(lua_State *L) { \
	SceneObject *hero = get_hero(L); \
	if (!hero) return 0; \
	void *component = SceneObject_ComponentWithInterface(hero, CharControllerComponent_Interface); \
	NAME(component);            \
    return 0;                         \
}                            \

#define BOOL_FUNC(NAME, SYM) \
STATIC_DL_FUNCTION_SYMBOL( \
	NAME, \
	SYM, \
	bool, (void *CharControllerComponent) \
)                            \
static int L_##NAME(lua_State *L) { \
	SceneObject *hero = get_hero(L); \
	if (!hero) return 0; \
	void *component = SceneObject_ComponentWithInterface(hero, CharControllerComponent_Interface); \
	bool b = NAME(component);   \
    lua_pushboolean(L, b);                         \
    return 1;                         \
}

VOID_FUNC(DropQuickly, "_ZN5Caver23CharControllerComponent11DropQuicklyEv")
VOID_FUNC(StartJumping, "_ZN5Caver23CharControllerComponent12StartJumpingEv")
VOID_FUNC(StopJumping, "_ZN5Caver23CharControllerComponent11StopJumpingEv")
VOID_FUNC(CancelCasting, "_ZN5Caver23CharControllerComponent13CancelCastingEv")
VOID_FUNC(FinishCasting, "_ZN5Caver23CharControllerComponent13FinishCastingEv")
VOID_FUNC(Die, "_ZN5Caver23CharControllerComponent3DieEv")
VOID_FUNC(Use, "_ZN5Caver23CharControllerComponent3UseEv")
VOID_FUNC(Hurt, "_ZN5Caver23CharControllerComponent4HurtEv")
VOID_FUNC(Swing, "_ZN5Caver23CharControllerComponent5SwingEv")

BOOL_FUNC(CanDoSomething, "_ZN5Caver23CharControllerComponent14CanDoSomethingEv")
BOOL_FUNC(CanBeginCasting, "_ZN5Caver23CharControllerComponent15CanBeginCastingEv")
BOOL_FUNC(CanUse, "_ZN5Caver23CharControllerComponent6CanUseEv")
BOOL_FUNC(CanJump, "_ZN5Caver23CharControllerComponent7CanJumpEv")

const luaL_Reg character_caver_functions[] = {
	{"DropQuickly", L_DropQuickly},
	{"StartJumping", L_StartJumping},
	{"StopJumping", L_StopJumping},
	{"CancelCasting", L_CancelCasting},
	{"FinishCasting", L_FinishCasting},
	{"Die", L_Die},
	{"Use", L_Use},
	{"Hurt", L_Hurt},
	{"Swing", L_Swing},

	{"CanDoSomething", L_CanDoSomething},
	{"CanBeginCasting", L_CanBeginCasting},
	{"CanUse", L_CanUse},
	{"CanJump", L_CanJump},
	{NULL, NULL}
};

void initLL_character_caver() {
	LOGD("Character Caver functions initiated!");
	dlsym_DropQuickly();
	dlsym_StartJumping(); dlsym_StopJumping(); dlsym_CanJump();
	dlsym_CancelCasting(); dlsym_FinishCasting(); dlsym_CanBeginCasting();
	dlsym_CanDoSomething(); dlsym_CanUse(); dlsym_Use();
	dlsym_Die(); dlsym_Hurt();
	dlsym_Swing();
}