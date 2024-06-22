#ifndef BN_SPRITE_ITEMS_JOCHEMWALKING_H
#define BN_SPRITE_ITEMS_JOCHEMWALKING_H

#include "bn_sprite_item.h"

//{{BLOCK(jochemwalking_bn_gfx)

//======================================================================
//
//	jochemwalking_bn_gfx, 64x2688@8, 
//	+ palette 128 entries, not compressed
//	+ 2688 tiles not compressed
//	Total size: 256 + 172032 = 172288
//
//	Time-stamp: 2024-06-22, 18:17:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_JOCHEMWALKING_BN_GFX_H
#define GRIT_JOCHEMWALKING_BN_GFX_H

#define jochemwalking_bn_gfxTilesLen 172032
extern const bn::tile jochemwalking_bn_gfxTiles[5376];

#define jochemwalking_bn_gfxPalLen 256
extern const bn::color jochemwalking_bn_gfxPal[128];

#endif // GRIT_JOCHEMWALKING_BN_GFX_H

//}}BLOCK(jochemwalking_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item jochemwalking(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(jochemwalking_bn_gfxTiles, 5376), bpp_mode::BPP_8, compression_type::NONE, 42), 
            sprite_palette_item(span<const color>(jochemwalking_bn_gfxPal, 128), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

