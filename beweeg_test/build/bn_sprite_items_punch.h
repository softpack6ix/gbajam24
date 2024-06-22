#ifndef BN_SPRITE_ITEMS_PUNCH_H
#define BN_SPRITE_ITEMS_PUNCH_H

#include "bn_sprite_item.h"

//{{BLOCK(punch_bn_gfx)

//======================================================================
//
//	punch_bn_gfx, 64x1024@4, 
//	+ palette 16 entries, not compressed
//	+ 1024 tiles not compressed
//	Total size: 32 + 32768 = 32800
//
//	Time-stamp: 2024-06-22, 18:17:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PUNCH_BN_GFX_H
#define GRIT_PUNCH_BN_GFX_H

#define punch_bn_gfxTilesLen 32768
extern const bn::tile punch_bn_gfxTiles[1024];

#define punch_bn_gfxPalLen 32
extern const bn::color punch_bn_gfxPal[16];

#endif // GRIT_PUNCH_BN_GFX_H

//}}BLOCK(punch_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item punch(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(punch_bn_gfxTiles, 1024), bpp_mode::BPP_4, compression_type::NONE, 16), 
            sprite_palette_item(span<const color>(punch_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

