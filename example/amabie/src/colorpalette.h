// Copyright(c) 2020 Takao Akaki

#ifndef _COLORPALETTE_H_
#define _COLORPALETTE_H_

#define PALETTE_SIZE 16

uint32_t colorpalette888[PALETTE_SIZE] = {

// You can create palette list from bmp file with GIMP.
//---------- index of color is (lineno - (lineno_of_this line) - 1).
0x060f0dU, // 0 
0x237a56U, // 1
0x505248U, // 2
0x00ff00U, // 3
0x4bcf53U, // 4
0xebe737U, // 5
0x987b84U, // 6
0x85a59cU, // 7
0xf3a3a4U, // 8
0xa9d39cU, // 9
0xcde4a4U, // 10
0xa2a3cfU, // 11
0xd8a9ccU, // 12
0xb0dbd8U, // 13 
0xd5f8d6U, // 14
0xffffffU, // 15
};

#endif // _COLORPALETTE_H_
