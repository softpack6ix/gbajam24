#ifndef BN_SPRITE_ITEMS_JOCHEM_H
#define BN_SPRITE_ITEMS_JOCHEM_H

#include "bn_sprite_item.h"

//{{BLOCK(jochem_bn_gfx)

//======================================================================
//
//	jochem_bn_gfx, 64x7616@8, 
//	+ palette 128 entries, not compressed
//	+ 7616 tiles not compressed
//	Total size: 256 + 487424 = 487680
//
//	Time-stamp: 2024-06-22, 18:17:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_JOCHEM_BN_GFX_H
#define GRIT_JOCHEM_BN_GFX_H

#define jochem_bn_gfxTilesLen 487424
extern const bn::tile jochem_bn_gfxTiles[15232];

#define jochem_bn_gfxPalLen 256
extern const bn::color jochem_bn_gfxPal[128];

#endif // GRIT_JOCHEM_BN_GFX_H

//}}BLOCK(jochem_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item jochem(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(jochem_bn_gfxTiles, 15232), bpp_mode::BPP_8, compression_type::NONE, 119), 
            sprite_palette_item(span<const color>(jochem_bn_gfxPal, 128), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

