#include "game/los.h"

#include "game/items.h"
#include "game/math.h"
#include "global/const.h"
#include "global/funcs.h"
#include "global/vars.h"
#include "util.h"

#include <assert.h>

int32_t __cdecl LOS_CheckX(
    const struct GAME_VECTOR *const start, struct GAME_VECTOR *const target)
{
    const int32_t dx = target->x - start->x;
    if (dx == 0) {
        return 1;
    }

    const int32_t dy = ((target->y - start->y) << WALL_SHIFT) / dx;
    const int32_t dz = ((target->z - start->z) << WALL_SHIFT) / dx;

    int16_t room_num = start->room_num;
    int16_t last_room_num = room_num;

    g_LOSRooms[0] = room_num;
    g_LOSNumRooms = 1;

    if (dx < 0) {
        int32_t x = start->x & (~(WALL_L - 1));
        int32_t y = start->y + ((dy * (x - start->x)) >> WALL_SHIFT);
        int32_t z = start->z + ((dz * (x - start->x)) >> WALL_SHIFT);

        while (x > target->x) {
            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = room_num;
                    return -1;
                }
            }

            if (room_num != last_room_num) {
                last_room_num = room_num;
                g_LOSRooms[g_LOSNumRooms++] = room_num;
            }

            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x - 1, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x - 1, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x - 1, y, z);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->room_num = last_room_num;
                    target->y = y;
                    target->z = z;
                    return 0;
                }
            }

            x -= WALL_L;
            y -= dy;
            z -= dz;
        }
    } else {
        int32_t x = start->x | (WALL_L - 1);
        int32_t y = start->y + (((x - start->x) * dy) >> WALL_SHIFT);
        int32_t z = start->z + (((x - start->x) * dz) >> WALL_SHIFT);

        while (x < target->x) {
            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z);
                if (y > height || y < ceiling) {
                    target->z = z;
                    target->y = y;
                    target->x = x;
                    target->room_num = room_num;
                    return -1;
                }
            }

            if (room_num != last_room_num) {
                last_room_num = room_num;
                g_LOSRooms[g_LOSNumRooms++] = room_num;
            }

            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x + 1, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x + 1, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x + 1, y, z);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = last_room_num;
                    return 0;
                }
            }

            x += WALL_L;
            y += dy;
            z += dz;
        }
    }

    target->room_num = room_num;
    return 1;
}

int32_t __cdecl LOS_CheckZ(
    const struct GAME_VECTOR *const start, struct GAME_VECTOR *const target)
{
    const int32_t dz = target->z - start->z;
    if (dz == 0) {
        return 1;
    }

    const int32_t dx = ((target->x - start->x) << WALL_SHIFT) / dz;
    const int32_t dy = ((target->y - start->y) << WALL_SHIFT) / dz;

    int16_t room_num = start->room_num;
    int16_t last_room_num = room_num;

    g_LOSRooms[0] = room_num;
    g_LOSNumRooms = 1;

    if (dz < 0) {
        int32_t z = start->z & (~(WALL_L - 1));
        int32_t x = start->x + ((dx * (z - start->z)) >> WALL_SHIFT);
        int32_t y = start->y + ((dy * (z - start->z)) >> WALL_SHIFT);

        while (z > target->z) {
            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = room_num;
                    return -1;
                }
            }

            if (room_num != last_room_num) {
                last_room_num = room_num;
                g_LOSRooms[g_LOSNumRooms++] = room_num;
            }

            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z - 1, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z - 1);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z - 1);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = last_room_num;
                    return 0;
                }
            }

            z -= WALL_L;
            x -= dx;
            y -= dy;
        }
    } else {
        int32_t z = start->z | (WALL_L - 1);
        int32_t x = start->x + ((dx * (z - start->z)) >> WALL_SHIFT);
        int32_t y = start->y + ((dy * (z - start->z)) >> WALL_SHIFT);

        while (z < target->z) {
            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = room_num;
                    return -1;
                }
            }

            if (room_num != last_room_num) {
                last_room_num = room_num;
                g_LOSRooms[g_LOSNumRooms++] = room_num;
            }

            {
                const struct FLOOR_INFO *const floor =
                    Room_GetFloor(x, y, z + 1, &room_num);
                const int32_t height = Room_GetHeight(floor, x, y, z + 1);
                const int32_t ceiling = Room_GetCeiling(floor, x, y, z + 1);
                if (y > height || y < ceiling) {
                    target->x = x;
                    target->y = y;
                    target->z = z;
                    target->room_num = last_room_num;
                    return 0;
                }
            }

            z += WALL_L;
            x += dx;
            y += dy;
        }
    }

    target->room_num = room_num;
    return 1;
}

int32_t __cdecl LOS_ClipTarget(
    const struct GAME_VECTOR *const start, struct GAME_VECTOR *const target,
    const FLOOR_INFO *const floor)
{
    const int32_t dx = target->x - start->x;
    const int32_t dy = target->y - start->y;
    const int32_t dz = target->z - start->z;

    const int32_t height =
        Room_GetHeight(floor, target->x, target->y, target->z);
    if (target->y > height && start->y < height) {
        target->y = height;
        target->x = start->x + dx * (height - start->y) / dy;
        target->z = start->z + dz * (height - start->y) / dy;
        return 0;
    }

    const int32_t ceiling =
        Room_GetCeiling(floor, target->x, target->y, target->z);
    if (target->y < ceiling && start->y > ceiling) {
        target->y = ceiling;
        target->x = start->x + dx * (ceiling - start->y) / dy;
        target->z = start->z + dz * (ceiling - start->y) / dy;
        return 0;
    }

    return 1;
}

int32_t __cdecl LOS_Check(
    const struct GAME_VECTOR *const start, struct GAME_VECTOR *const target)
{
    int32_t los1;
    int32_t los2;

    if (ABS(target->z - start->z) > ABS(target->x - start->x)) {
        los1 = LOS_CheckX(start, target);
        los2 = LOS_CheckZ(start, target);
    } else {
        los1 = LOS_CheckZ(start, target);
        los2 = LOS_CheckX(start, target);
    }

    if (!los2) {
        return 0;
    }

    const FLOOR_INFO *const floor =
        Room_GetFloor(target->x, target->y, target->z, &target->room_num);

    if (!LOS_ClipTarget(start, target, floor)) {
        return 0;
    }
    if (los1 == 1 && los2 == 1) {
        return 1;
    }
    return 0;
}

int32_t __cdecl LOS_CheckSmashable(
    const struct GAME_VECTOR *const start, struct GAME_VECTOR *const target)
{
    const int32_t dx = target->x - start->x;
    const int32_t dy = target->y - start->y;
    const int32_t dz = target->z - start->z;

    for (int i = 0; i < g_LOSNumRooms; i++) {
        for (int16_t item_num = g_Rooms[g_LOSRooms[i]].item_num;
             item_num != NO_ITEM; item_num = g_Items[item_num].next_item) {
            const struct ITEM_INFO *const item = &g_Items[item_num];
            if (item->status == IS_DEACTIVATED) {
                continue;
            }

            if (!Item_IsSmashable(item)) {
                continue;
            }

            const enum DIRECTION direction = Math_GetDirection(item->pos.y_rot);
            const int16_t *const bounds = Item_GetBoundsAccurate(item);
            const int16_t *x_extent;
            const int16_t *z_extent;
            switch (direction) {
            case DIR_EAST:
            case DIR_WEST:
                x_extent = &bounds[FBBOX_MIN_Z];
                z_extent = &bounds[FBBOX_MIN_X];
                break;
            case DIR_NORTH:
            case DIR_SOUTH:
                x_extent = &bounds[FBBOX_MIN_X];
                z_extent = &bounds[FBBOX_MIN_Z];
                break;
            default:
                assert(false);
                break;
            }

            int32_t failure = 0;
            if (ABS(dz) > ABS(dx)) {
                int32_t distance = item->pos.z + z_extent[0] - start->z;
                for (int j = 0; j < 2; j++) {
                    if ((distance >= 0) == (dz >= 0)) {
                        const int32_t y = dy * distance / dz;
                        if (y <= item->pos.y + bounds[FBBOX_MIN_Y] - start->y
                            || y >= item->pos.y + bounds[FBBOX_MAX_Y]
                                    - start->y) {
                            continue;
                        }

                        const int32_t x = dx * distance / dz;
                        if (x < item->pos.x + x_extent[0] - start->x) {
                            failure |= 1;
                        } else if (x > item->pos.x + x_extent[1] - start->x) {
                            failure |= 2;
                        } else {
                            return item_num;
                        }
                    }

                    distance = item->pos.z + z_extent[1] - start->z;
                }

                if (failure == 3) {
                    return item_num;
                }
            } else {
                int32_t distance = item->pos.x + x_extent[0] - start->x;
                for (int j = 0; j < 2; j++) {
                    if ((distance >= 0) == (dx >= 0)) {
                        const int32_t y = dy * distance / dx;
                        if (y <= item->pos.y + bounds[FBBOX_MIN_Y] - start->y
                            || y >= item->pos.y + bounds[FBBOX_MAX_Y]
                                    - start->y) {
                            continue;
                        }

                        const int32_t z = dz * distance / dx;
                        if (z < item->pos.z + z_extent[0] - start->z) {
                            failure |= 1;
                        } else if (z > item->pos.z + z_extent[1] - start->z) {
                            failure |= 2;
                        } else {
                            return item_num;
                        }
                    }

                    distance = item->pos.x + x_extent[1] - start->x;
                }

                if (failure == 3) {
                    return item_num;
                }
            }
        }
    }

    return NO_ITEM;
}
