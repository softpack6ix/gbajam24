
@{{BLOCK(dog_bn_gfx)

@=======================================================================
@
@	dog_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2024-06-21, 16:46:59
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global dog_bn_gfxTiles		@ 128 unsigned chars
	.hidden dog_bn_gfxTiles
dog_bn_gfxTiles:
	.word 0x00000000,0x00000000,0x00000000,0x66000000,0x44600000,0x34600000,0x24600000,0x22606600
	.word 0x00000000,0x00000000,0x00000000,0x00006666,0x00064444,0x00064464,0x00644464,0x00644443
	.word 0x33662600,0x33322600,0x33332600,0x36336000,0x22220000,0x00000000,0x00000000,0x00000000
	.word 0x00633333,0x00066622,0x00000633,0x00000636,0x00000022,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global dog_bn_gfxPal		@ 32 unsigned chars
	.hidden dog_bn_gfxPal
dog_bn_gfxPal:
	.hword 0x7C1F,0x7FFF,0x14B3,0x19BD,0x327E,0x2A5F,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(dog_bn_gfx)
