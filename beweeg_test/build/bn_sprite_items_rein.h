#ifndef BN_SPRITE_ITEMS_REIN_H
#define BN_SPRITE_ITEMS_REIN_H

#include "bn_sprite_item.h"

//{{BLOCK(rein_bn_gfx)

//======================================================================
//
//	rein_bn_gfx, 64x6144@8, 
//	+ palette 128 entries, not compressed
//	+ 6144 tiles not compressed
//	Total size: 256 + 393216 = 393472
//
//	Time-stamp: 2024-06-22, 18:17:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_REIN_BN_GFX_H
#define GRIT_REIN_BN_GFX_H

#define rein_bn_gfxTilesLen 393216
extern const bn::tile rein_bn_gfxTiles[12288];

#define rein_bn_gfxPalLen 256
extern const bn::color rein_bn_gfxPal[128];

#endif // GRIT_REIN_BN_GFX_H

//}}BLOCK(rein_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item rein(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(rein_bn_gfxTiles, 12288), bpp_mode::BPP_8, compression_type::NONE, 96), 
            sprite_palette_item(span<const color>(rein_bn_gfxPal, 128), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

