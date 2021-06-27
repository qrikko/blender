/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2008 Blender Foundation.
 * All rights reserved.
 *
 *
 * Contributor(s): qrikko@gmail.com
 *
 * ***** END GPL LICENSE BLOCK *****
 */
 
/** \file blender/editors/space_asset_shelf/space_asset_shelf.c
 *  \ingroup sppainter
 */

#include <string.h>

#include "DNA_text_types.h"
#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"

#include "BKE_context.h"
#include "BKE_screen.h"

#include "ED_space_api.h"
#include "ED_screen.h"

#include "BIF_glutil.h"

#include "WM_api.h"
#include "WM_types.h"

#include "UI_interface.h"
#include "UI_resources.h"
#include "UI_view2d.h"

static SpaceLink *asset_shelf_create(const ScrArea *UNUSED(area), const Scene *UNUSED(scene)) {
    ARegion *region;
    SpaceAssetShelf *asset_shelf;

    asset_shelf = MEM_callocN(sizeof(SpaceAssetShelf), "init-asset-shelf");
    asset_shelf->spacetype = SPACE_ASSET_SHELF;

    /* header */
    region = MEM_callocN(sizeof(ARegion), "header for asset shelf");
    
    BLI_addtail(&asset_shelf->regionbase, region);
    region->regiontype = RGN_TYPE_HEADER;
    region->alignment = (U.uiflag & USER_HEADER_BOTTOM) ? RGN_ALIGN_BOTTOM : RGN_ALIGN_TOP;

    /* main region */
    region = MEM_callocN(sizeof(ARegion), "main region for assets");
    BLI_addtail(&asset_shelf->regionbase, region);
    region->regiontype = RGN_TYPE_WINDOW;
    
    /* keep in sync with info */
    region->v2d.scroll |= V2D_SCROLL_RIGHT;
    region->v2d.align |= V2D_ALIGN_NO_NEG_X | V2D_ALIGN_NO_NEG_Y; /* align bottom left */
    region->v2d.keepofs |= V2D_LOCKOFS_X;
    region->v2d.keepzoom = (V2D_LOCKZOOM_X | V2D_LOCKZOOM_Y | V2D_LIMITZOOM | V2D_KEEPASPECT);
    //region->v2d.keeptot = V2D_KEEPTOT_BOUNDS;
    region->v2d.minzoom = region->v2d.maxzoom = 1.0f;
    
    return (SpaceLink *)asset_shelf;
}

static void asset_shelf_free(SpaceLink *sl) {
  //SpaceAssetShelf *sc = (SpaceAssetShelf *)sl;
}

/* spacetype; init callback */
static void asset_shelf_init(struct wmWindowManager *UNUSED(wm), ScrArea *UNUSED(area)) {
}

static void asset_shelf_main_region_init (wmWindowManager *wm, ARegion *region) {
    UI_view2d_region_reinit(&region->v2d, V2D_COMMONVIEW_CUSTOM, region->winx, region->winy);
}

static void asset_shelf_main_region_draw(const bContext *C, ARegion *region) {
    SpaceAssetShelf *sc = CTX_wm_space_asset_shelf(C);
    View2D *v2d = &region->v2d;

    /* clear and setup matrix */
    UI_ThemeClearColor(TH_BACK);

    /* Works best with no view2d matrix set. */
    UI_view2d_view_ortho(v2d);

    /* reset view matrix */
    UI_view2d_view_restore(C);

    /* scrollers */
    UI_view2d_scrollers_draw(v2d, NULL);
}

/****************** header region ******************/
/* add handlers, stuff you only do once or on area/region changes */
static void asset_shelf_header_region_init(wmWindowManager *UNUSED(wm), ARegion *region) {
    ED_region_header_init(region);
}

static void asset_shelf_header_region_draw(const bContext *C, ARegion *region) {
    ED_region_header(C, region);
}

void ED_spacetype_asset_shelf(void) {
    SpaceType *st = MEM_callocN(sizeof(SpaceType), "spacetype asset shelf");
    ARegionType *art;

    st->spaceid = SPACE_ASSET_SHELF;
    strncpy(st->name, "Asset Shelf", BKE_ST_MAXNAME);
    
    /* SpaceType callbacks */
    st->create = asset_shelf_create;
    st->free = asset_shelf_free;
    st->init = asset_shelf_init;
    //st->duplicate = asset_shelf_duplicate;
    //st->operatortypes = asset_shelf_operatortypes;
    //st->keymap = asset_shelf_keymap;
    //st->dropboxes = asset_shelf_dropboxes;

    /* regions: main window */
    art = MEM_callocN(sizeof(ARegionType), "spacetype asset shelf region");
    art->regionid = RGN_TYPE_WINDOW;
    art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D;

    art->init = asset_shelf_main_region_init;
    art->draw = asset_shelf_main_region_draw;
    /*
    art->cursor = asset_shelf_cursor;
    art->event_cursor = true;
    art->listener = asset_shelf_main_region_listener;
    */

    BLI_addhead(&st->regiontypes, art);

    /* regions: header */
    art = MEM_callocN(sizeof(ARegionType), "spacetype asset shelf region");
    art->regionid = RGN_TYPE_HEADER;
    art->prefsizey = HEADERY;
    art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_HEADER;

    art->init = asset_shelf_header_region_init;
    art->draw = asset_shelf_header_region_draw;

    BLI_addhead(&st->regiontypes, art);

    BKE_spacetype_register(st);
}