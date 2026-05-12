#include "lauxlib.h"
#include "lua.h"
#include "lua_libs/lni/lni.h"
#include "core/log.h"
#include "core/hooks.h"
#include "core/java.h"

#define LOG_TAG "KiwiCButton"

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

static void hideAllButtons() {
	JNIEnv *env = miniJ_get_env();
	jclass cls = (*env)->FindClass(env, "net/itsjustsomedude/swrdg/ButtonController");
	jmethodID hideAll = (*env)->GetStaticMethodID(env, cls, "hideAll", "()V");
	(*env)->CallStaticVoidMethod(env, cls, hideAll);
}

static void unhideAllButtons() {
	JNIEnv *env = miniJ_get_env();
	jclass cls = (*env)->FindClass(env, "net/itsjustsomedude/swrdg/ButtonController");
	jmethodID unhideAll = (*env)->GetStaticMethodID(env, cls, "unhideAll", "()V");
	(*env)->CallStaticVoidMethod(env, cls, unhideAll);
}

STATIC_DL_HOOK_SYMBOL(
    GameMenu_LoadView,
    "_ZN5Caver22GameMenuViewController8LoadViewEv",
    void, (void *thiz)
) {
	hideAllButtons();
    orig_GameMenu_LoadView(thiz);
}

STATIC_DL_HOOK_SYMBOL(
    GameMenu_ViewWillDisappear,
    "_ZN5Caver22GameMenuViewController17ViewWillDisappearEv",
    void, (void *thiz)
) {
	unhideAllButtons();
    orig_GameMenu_ViewWillDisappear(thiz);
}

STATIC_DL_HOOK_SYMBOL(
	GameSceneView_SetCinematicMode,
	"_ZN5Caver13GameSceneView23SetCinematicModeEnabledEbbb",
	void, (void *thiz, bool enabled, bool animate, bool unknown)
) {
	if (enabled) {
		hideAllButtons();
	} else {
		unhideAllButtons();
	}
	orig_GameSceneView_SetCinematicMode(thiz, enabled, animate, unknown);
}

void initLL_button() {
    LOGD("Initialized Kiwi ButtonController library.");
    hook_GameMenu_LoadView();
    hook_GameMenu_ViewWillDisappear();
	hook_GameSceneView_SetCinematicMode();
}