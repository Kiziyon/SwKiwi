#include "lauxlib.h"
#include "lua.h"
#include "core/log.h"
#include "core/hooks.h"
#include "core/java.h"

#define LOG_TAG "KiwiCButton"

#define BUTTON_CLASS "net/itsjustsomedude/swrdg/ButtonController"

static jclass getButtonClass(JNIEnv *env) {
	return (*env)->FindClass(env, BUTTON_CLASS);
}

static int newButton(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	const char *label = luaL_checkstring(L, 2);

	float x = (float)luaL_checknumber(L, 3);
	float y = (float)luaL_checknumber(L, 4);

	float width = (float)luaL_checknumber(L, 5);
	float height = (float)luaL_checknumber(L, 6);

	jmethodID method = (*env)->GetStaticMethodID(
		env,
		cls,
		"addButton",
		"(Ljava/lang/String;Ljava/lang/String;FFFF)V"
	);

	jstring jid = (*env)->NewStringUTF(env, id);
	jstring jlabel = (*env)->NewStringUTF(env, label);

	(*env)->CallStaticVoidMethod(
		env,
		cls,
		method,
		jid,
		jlabel,
		x,
		y,
		width,
		height
	);

	return 0;
}

static int makeMovable(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	bool snapback = lua_toboolean(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(
		env, cls, "makeMovable", "(Ljava/lang/String;Z)V"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(
		env,cls,method,jid,snapback
	);

	return 0;
}

static int deleteButton(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(
		env,cls,"removeButton","(Ljava/lang/String;)V"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env, cls, method, jid);

	return 0;
}

static int setClickable(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	int clickable = lua_toboolean(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(
		env,cls,"setClickable","(Ljava/lang/String;I)V"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env, cls, method, jid, clickable);

	return 0;
}

static int buttonPressed(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(
		env,cls,"isPressed","(Ljava/lang/String;)Z"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	jboolean pressed =
		(*env)->CallStaticBooleanMethod(env,cls,method,jid);

	lua_pushboolean(L, pressed);

	return 1;
}

static int buttonDragged(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(
		env,
		cls,
		"isDragging",
		"(Ljava/lang/String;)Z"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	jboolean dragging =
		(*env)->CallStaticBooleanMethod(env,cls,method,jid);

	lua_pushboolean(L, dragging);

	return 1;
}

static int buttonExists(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(
		env, cls, "exists", "(Ljava/lang/String;)Z"
	);

	jstring jid = (*env)->NewStringUTF(env, id);

	jboolean exists =
		(*env)->CallStaticBooleanMethod(env, cls, method, jid);

	lua_pushboolean(L, exists);
	return 1;
}

static int removeAll(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"removeAll","()V");

	(*env)->CallStaticVoidMethod(env, cls, method);

	return 0;
}

static int setScaling(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	float scaleX = (float)luaL_checknumber(L, 2);
	float scaleY = (float)luaL_checknumber(L, 3);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setScaling","(Ljava/lang/String;FF)V");

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env,cls,method,jid,scaleX,scaleY);
	return 0;
}


static int setAlpha(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	int alpha = (int)luaL_checknumber(L, 2);

	jstring jid = (*env)->NewStringUTF(env, id);
	jmethodID method = (*env)->GetStaticMethodID(
		env,
		cls,
		"setAlpha",
		"(Ljava/lang/String;I)V"
	);

	(*env)->CallStaticVoidMethod(
		env,
		cls,
		method,
		jid,
		alpha
	);

	(*env)->DeleteLocalRef(env, jid);

	return 0;
}

// Text functions
static int setText(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	const char *text = luaL_checkstring(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setText","(Ljava/lang/String;Ljava/lang/String;)V");

	jstring jid = (*env)->NewStringUTF(env, id);
	jstring jtext = (*env)->NewStringUTF(env, text);

	(*env)->CallStaticVoidMethod(env,cls,method,jid,jtext);

	return 0;
}

static int setTextFont(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	const char *fontPath = luaL_checkstring(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setTextFont","(Ljava/lang/String;Ljava/lang/String;)V");

	jstring jid = (*env)->NewStringUTF(env, id);
	jstring jfontText = (*env)->NewStringUTF(env, fontPath);

	(*env)->CallStaticVoidMethod(env,cls,method,jid,jfontText);

	return 0;
}

static int setTextScale(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	float scale = (float)luaL_checknumber(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setTextScale","(Ljava/lang/String;F)V");

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env,cls,method,jid,scale);

	return 0;
}

static int setTextColor(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	int color = (int)luaL_checkinteger(L, 2);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setTextColor","(Ljava/lang/String;I)V");

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env, cls, method, jid, color);
	return 0;
}

static int getPositionX(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"getPosition","(Ljava/lang/String;)[F");

	jstring jid = (*env)->NewStringUTF(env, id);

	jfloatArray arr = (jfloatArray)(*env)->CallStaticObjectMethod(env,cls,method,jid);

	jfloat *values = (*env)->GetFloatArrayElements(env, arr, NULL);

	lua_pushnumber(L, values[0]);

	(*env)->ReleaseFloatArrayElements(env,arr,values,0);

	return 1;
}

static int getPositionY(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"getPosition","(Ljava/lang/String;)[F");

	jstring jid = (*env)->NewStringUTF(env, id);

	jfloatArray arr =
		(jfloatArray)(*env)->CallStaticObjectMethod(env,cls,method,jid);

	jfloat *values =
		(*env)->GetFloatArrayElements(env, arr, NULL);

	lua_pushnumber(L, values[1]);

	(*env)->ReleaseFloatArrayElements(env,arr,values,0);

	return 1;
}

static int getPosition(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);
	const char *id = luaL_checkstring(L, 1);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"getPosition","(Ljava/lang/String;)[F");
	jstring jid = (*env)->NewStringUTF(env, id);
	jfloatArray arr =
		(jfloatArray)(*env)->CallStaticObjectMethod(env,cls,method,jid);

	jfloat *values =
		(*env)->GetFloatArrayElements(env, arr, NULL);

	lua_pushnumber(L, values[0]);
	lua_pushnumber(L, values[1]);

	(*env)->ReleaseFloatArrayElements(env,arr,values,0);
	return 2;
}

static int setPosition(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);

	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);

	jmethodID method = (*env)->GetStaticMethodID(env,cls,"setPosition","(Ljava/lang/String;FF)V");

	jstring jid = (*env)->NewStringUTF(env, id);

	(*env)->CallStaticVoidMethod(env,cls,method,jid,x,y);

	return 0;
}

static int setBGR(lua_State *L) {
    JNIEnv *env = miniJ_get_env();
    jclass cls = getButtonClass(env);

    const char *id = luaL_checkstring(L, 1);
    const char *resName = luaL_checkstring(L, 2);

    jmethodID method = (*env)->GetStaticMethodID(
        env,
        cls,
        "setBackgroundResource",
		"(Ljava/lang/String;Ljava/lang/String;)V"
    );

    if (method == NULL) {
        LOGE("Failed to find setBackgroundResource");
        return 0;
	}

    jstring jid = (*env)->NewStringUTF(env, id);
    jstring jres = (*env)->NewStringUTF(env, resName);

    (*env)->CallStaticVoidMethod(
        env,
        cls,
        method,
        jid,
		jres
    );

    (*env)->DeleteLocalRef(env, jid);
    (*env)->DeleteLocalRef(env, jres);

    return 0;
}

static int setBGA(lua_State *L) {
	JNIEnv *env = miniJ_get_env();
	jclass cls = getButtonClass(env);

	const char *id = luaL_checkstring(L, 1);
	int alpha = (int)luaL_checknumber(L, 2);

	jstring jid = (*env)->NewStringUTF(env, id);
	jmethodID method = (*env)->GetStaticMethodID(
		env,
		cls,
		"setBackgroundAlpha",
		"(Ljava/lang/String;I)V"
	);

	(*env)->CallStaticVoidMethod(
		env,
		cls,
		method,
		jid,
		alpha
	);

	(*env)->DeleteLocalRef(env, jid);

	return 0;
}

static const luaL_Reg button_library[] = {
	{"New", newButton},
	{"MakeMovable", makeMovable},
	{"SetClickable", setClickable},
	{"Delete", deleteButton},

	{"IsPressed", buttonPressed},
	{"IsDragging", buttonDragged},
	{"Exists", buttonExists},

	{"DeleteAll", removeAll},
	{"SetScaling", setScaling},
	{"SetAlpha", setAlpha},

	{"SetText", setText},
	{"SetTextFont", setTextFont},
	{"SetTextScale", setTextScale},
	{"SetTextColor", setTextColor},

	{"GetPositionX", getPositionX},
	{"GetPositionY", getPositionY},
	{"GetPosition", getPosition},
	{"SetPosition", setPosition},

	{"SetBackgroundResource", setBGR},
	{"SetBackgroundAlpha", setBGA},

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

#define HIDE_HOOK(NAME, SYMBOL, SIG, ...) \
STATIC_DL_HOOK_SYMBOL( \
    NAME, \
    SYMBOL, \
    void, SIG \
) { \
    hideAllButtons(); \
    orig_##NAME(__VA_ARGS__); \
}

#define SHOW_HOOK(NAME, SYMBOL, SIG, ...) \
STATIC_DL_HOOK_SYMBOL( \
    NAME, \
    SYMBOL, \
    void, SIG \
) { \
    unhideAllButtons(); \
    orig_##NAME(__VA_ARGS__); \
}

HIDE_HOOK(GameMenu_LoadView, "_ZN5Caver22GameMenuViewController8LoadViewEv", (void *thiz), thiz)
SHOW_HOOK(GameMenu_ViewWillDisappear, "_ZN5Caver22GameMenuViewController17ViewWillDisappearEv", (void *thiz), thiz)

HIDE_HOOK(PauseView_C, "_ZN5Caver9PauseViewC2Ev", (void *thiz), thiz)
SHOW_HOOK(PauseView_D, "_ZN5Caver9PauseView10TouchEndedERKNS_7FWTouchE", (void *thiz, void *touch), thiz, touch)

HIDE_HOOK(PortalView_AnimateIn, "_ZN5Caver10PortalView9AnimateInEv", (void *thiz), thiz)
SHOW_HOOK(PortalView_AnimateOut, "_ZN5Caver10PortalView10AnimateOutEv", (void *thiz), thiz)

HIDE_HOOK(SkillPicker_Load, "_ZN5Caver25SkillPickerViewController8LoadViewEv", (void *thiz), thiz)
SHOW_HOOK(SkillPicker_Destroy, "_ZN5Caver25SkillPickerViewControllerD0Ev", (void *thiz), thiz)

HIDE_HOOK(Portal_Enter, "_ZN5Caver7MapViewC1Ev", (void *thiz), thiz)

void initLL_button() {
    LOGD("Initialized Kiwi ButtonController library.");
    hook_GameMenu_LoadView();
    hook_GameMenu_ViewWillDisappear();
    hook_GameSceneView_SetCinematicMode();
    hook_PauseView_C();
    hook_PauseView_D();
    hook_PortalView_AnimateIn();
    hook_PortalView_AnimateOut();
    hook_SkillPicker_Load();
    hook_SkillPicker_Destroy();
    hook_Portal_Enter();
}