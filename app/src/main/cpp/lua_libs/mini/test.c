#include "caver/components.h"
#include "caver/scene_object.h"
//#include "caver/types.h"
#include "core/log.h"
#include "init/lua_libs.h"
#include "mini.h"

#define LOG_TAG "KiwiTest"

static void iterateOffsets(int start, int end, void* comp) {
    for (int i = start; i < end; i += 4) {
        int intVal = *(int *)((char *)comp + i);
        float floatVal = *(float *)((char *)comp + i);
        uint8_t byteVal = *(uint8_t *)((char *)comp + i);
        if (intVal == 117) continue;
        LOGD("offset 0x%x = {\n  %d,\n  %f,\n  %u\n};", i, intVal, floatVal, byteVal);
    }
}

int miniLL_test(lua_State *L) {
	SceneObject **o = lua_touserdata(L, 1);
	void *comp = SceneObject_ComponentWithInterface(*o, EntityComponent_Interface);
	if (!comp) return 0;
	iterateOffsets(0, 300, comp);
	*$(float, comp, 0xa4, 0x0) = 400.0f;
 	return 0;
}

void initLL_test() {
}
