#include "game/console/cmd/flipmap.h"

#include "game/game_string.h"
#include "game/gameflow/gameflow_new.h"
#include "game/room.h"
#include "global/vars.h"

#include <libtrx/strings.h>

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *ctx);

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *const ctx)
{
    if (g_GameInfo.current_level.type == GFL_TITLE
        || g_GameInfo.current_level.type == GFL_DEMO
        || g_GameInfo.current_level.type == GFL_CUTSCENE) {
        return CR_UNAVAILABLE;
    }

    bool new_state;
    if (String_Equivalent(ctx->args, "")) {
        new_state = !g_FlipStatus;
    } else if (!String_ParseBool(ctx->args, &new_state)) {
        return CR_BAD_INVOCATION;
    }

    if (g_FlipStatus == new_state) {
        Console_Log(
            new_state ? GS(OSD_FLIPMAP_FAIL_ALREADY_ON)
                      : GS(OSD_FLIPMAP_FAIL_ALREADY_OFF));
        return CR_SUCCESS;
    }

    Room_FlipMap();
    Console_Log(new_state ? GS(OSD_FLIPMAP_ON) : GS(OSD_FLIPMAP_OFF));
    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_FlipMap = {
    .prefix = "flip|flipmap",
    .proc = M_Entrypoint,
};
