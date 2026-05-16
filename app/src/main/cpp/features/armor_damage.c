#include "armor_damage.h"

#include "core/hooks.h"
#include "libs/khash.h"
#include "core/log.h"

#include <stdlib.h>
#include <string.h>

#define LOG_TAG "MiniArmorDamage"

KHASH_MAP_INIT_STR(str_float, float)

static kh_str_float_t *multipliers = NULL;

STATIC_DL_HOOK_SYMBOL(
    admwi,
    "_ZN5Caver14CharacterState29ArmorDamageMultiplierWithItemERKN5boost10shared_ptrINS_4ItemEEES6_",
    float, (void *param_1, void **param_2)
) {
    if (param_2 != NULL && *param_2 != NULL) {
        char *item_id = *$(char*, *param_2, 0x4, 0x8);
        LOGD("ArmorDamage Item ID: %s", item_id);

        if (multipliers != NULL) {
            khint_t k = kh_get_str_float(multipliers, item_id);
            if (k != kh_end(multipliers)) {
                return kh_val(multipliers, k);
			}
        }
	}

    return orig_admwi(param_1, param_2);
}

void miniAD_add_multiplier(const char *_Nonnull item_name, float multiplier) {
    if (multipliers == NULL)
        multipliers = kh_init_str_float();

    int absent;
    khint_t k = kh_put_str_float(multipliers, strdup(item_name), &absent);
    if (!absent) {
        free((void *)kh_key(multipliers, k));
	}
    LOGD("Setting dmg multiplier... '%s' %f", item_name, multiplier);
    kh_value(multipliers, k) = multiplier;
}

void initF_armor_damage() {
    hook_admwi();
}