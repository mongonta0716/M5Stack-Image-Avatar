// Copyright(c) 2020 Takao Akaki

#ifndef _COLORPALETTE_H_
#define _COLORPALETTE_H_

#define PALETTE_SIZE 16

uint16_t colorpalette565[PALETTE_SIZE];

uint32_t colorpalette888[PALETTE_SIZE] = {

// You can create palette list from bmp file with GIMP.
//---------- index of color is (lineno - (lineno_of_this line) - 1).
0x2a2a2d,
0x3e3d45,
0x103661,
0x52515e,
0xce656c,
0x01ff01,
0x888776,
0x567ab7,
0xb0adae,
0xeda596,
0xf1c9b5,
0xcecbcb,
0xfbdcd4,
0xffe9db,
0xe7e8ea,
0xffffff
};

static void convertPalette() {
  for (int i=0; i<PALETTE_SIZE; i++) {
    colorpalette565[i] = (uint16_t)lgfx::convert_rgb888_to_rgb565(colorpalette888[i]);
//    colorpalette565[i] = (uint16_t)((colorpalette888[i]&0xf800000)>>8)&((colorpalette888[i]&0xfc00)>>5)&((colorpalette888[i]&0xf8)>>3);

#ifdef DEBUG
    Serial.printf("index:%d,565:%04x,888:%06x\n",i , colorpalette565[i], colorpalette888[i]);
#endif
  }
}

#endif
