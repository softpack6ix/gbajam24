#ifndef BN_SPRITE_ITEMS_MUD_H
#define BN_SPRITE_ITEMS_MUD_H

#include "bn_sprite_item.h"

//{{BLOCK(mud_bn_gfx)

//======================================================================
//
//	mud_bn_gfx, 64x5760@4, 
//	+ palette 16 entries, not compressed
//	+ 5760 tiles not compressed
//	Total size: 32 + 184320 = 184352
//
//	Time-stamp: 2024-06-22, 18:17:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MUD_BN_GFX_H
#define GRIT_MUD_BN_GFX_H

#define mud_bn_gfxTilesLen 184320
extern const bn::tile mud_bn_gfxTiles[5760];

#define mud_bn_gfxPalLen 32
extern const bn::color mud_bn_gfxPal[16];

#endif // GRIT_MUD_BN_GFX_H

//}}BLOCK(mud_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item mud(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(mud_bn_gfxTiles, 5760), bpp_mode::BPP_4, compression_type::NONE, 90), 
            sprite_palette_item(span<const color>(mud_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

