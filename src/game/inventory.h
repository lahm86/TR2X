#pragma once

#include "global/types.h"

void __cdecl Inv_Construct(void);
int32_t __cdecl Inv_Display(int32_t inventory_mode);
void __cdecl Inv_SelectMeshes(INVENTORY_ITEM *inv_item);
int32_t __cdecl Inv_AnimateInventoryItem(INVENTORY_ITEM *inv_item);
void __cdecl Inv_DrawInventoryItem(INVENTORY_ITEM *inv_item);
