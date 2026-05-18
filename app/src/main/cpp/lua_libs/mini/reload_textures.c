#include "core/log.h"
#include "mini.h"

#define LOG_TAG "MiniReloadTextures"

STATIC_DL_FUNCTION_SYMBOL(
	TL_SHARED,
	"_ZN5Caver14TextureLibrary13sharedLibraryEv",
	void*, (void)
)

STATIC_DL_FUNCTION_SYMBOL(
	TL_RELOAD,
	"_ZN5Caver14TextureLibrary14ReloadTexturesEb",
	void, (void *TL, bool force)
)

int miniLL_reload_textures(lua_State *L) {
	bool force = lua_toboolean(L, 1);
	void *TextureLibrary = TL_SHARED();
	if (!TextureLibrary) return 0;
	TL_RELOAD(TextureLibrary, force);
	return 0;
}

void initLL_reload_textures() {
	dlsym_TL_SHARED();
	dlsym_TL_RELOAD();
}