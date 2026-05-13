#include "lauxlib.h"
#include "lua.h"
#include "lua_libs/lni/lni.h"
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

	int width = (int)luaL_checkinteger(L, 5);
	int height = (int)luaL_checkinteger(L, 6);

	jmethodID method = (*env)->GetStaticMethodID(
		env,
		cls,
		"addButton",
		"(Ljava/lang/String;Ljava/lang/String;FFII)V"
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

static const luaL_Reg button_library[] = {
	{"New", newButton},
	{"MakeMovable", makeMovable},
	{"Delete", deleteButton},

	{"IsPressed", buttonPressed},
	{"IsDragging", buttonDragged},
	{"Exists", buttonExists},

	{"DeleteAll", removeAll},
	{"SetScaling", setScaling},

	{"SetText", setText},
	{"SetTextScale", setTextScale},
	{"SetTextColor", setTextColor},

	{"GetPositionX", getPositionX},
	{"GetPositionY", getPositionY},
	{"GetPosition", getPosition},
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