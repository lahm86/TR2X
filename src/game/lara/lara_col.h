#pragma once

#include "global/types.h"

void __cdecl Lara_Col_Walk(struct ITEM_INFO *item, struct COLL_INFO *coll);
void __cdecl Lara_Col_Run(struct ITEM_INFO *item, struct COLL_INFO *coll);
void __cdecl Lara_Col_Stop(struct ITEM_INFO *item, struct COLL_INFO *coll);
void __cdecl Lara_Col_ForwardJump(
    struct ITEM_INFO *item, struct COLL_INFO *coll);
void __cdecl Lara_Col_FastBack(struct ITEM_INFO *item, struct COLL_INFO *coll);
void __cdecl Lara_Col_TurnRight(struct ITEM_INFO *item, struct COLL_INFO *coll);
