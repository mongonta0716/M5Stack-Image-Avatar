// Copyright(c) 2020 Takao Akaki

#ifndef _AVATARCONFIG_H_
#define _AVATARCONFIG_H_

#include <stdint.h>

#define MAX_FILENAME 50

#define MAX_EXPRESSION 3

#define NORMAL 0
#define SAD    1
#define ANGRY  2
#define HAPPY  3
#define DOUBT  4
#define SLEEPY 5

#define FIXED_PARTS 2

#define HEAD    0
#define BODY    1

#define RIGHT true
#define LEFT  false

#define EYEBALL_MAX 10

// Margin when using rotation
static int16_t marginX = 0; // left,right   (320-320)/2
static int16_t marginY = 0; // upper,lower  (240-240)/2

// About:Color Value 
// when use palette  =>  palette index
// when dont use palette =>  rgb888(0xXXXXXXU)

// Avatar Color Settings
static uint32_t skinColor  = 13; // Color of skin
static uint32_t whiteColor = 15; // Color fill eyeball
static uint32_t tpColor    = 5;  // Color of transparent

// Sprite Common Settings
typedef struct SpriteCommon {
  bool     psram;
  uint8_t  colorDepth;
  bool     swapBytes;
  uint32_t tpColor;
} spcommon_s;

// 149 is ColorIndex of transparent 0x07e0
static spcommon_s spcommon = { true, 4, true, tpColor };


// ---------- Body,Head,Eyeball Config ----------
// Sprite Params for body,head,eyeball
typedef struct SpriteParams1phase {
    spcommon_s cmn;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    char filename[MAX_FILENAME];
} params_fixed_s;

// 1 phase parts(head,body)
static params_fixed_s fixedParts[FIXED_PARTS] = {
  //          x,   y,   w,   h, , bmpfilename
  { spcommon, 0, -18, 320, 260, "/bmp/head_4bit.bmp" },
  { spcommon, 0, 195, 320,  45, "/bmp/body_4bit.bmp"  },
};

// eyeball
static params_fixed_s eyeball[MAX_EXPRESSION] = {
  { spcommon, 0,   0,  30,  38, "/bmp/eyeball_4bit.bmp"  },
  { spcommon, 0,   0,  30,  38, "/bmp/eyeball_4bit.bmp"  },
  { spcommon, 0,   0,  30,  38, "/bmp/eyeball_4bit.bmp"  },
};

// ---------- Mouth Config ----------
typedef struct SpriteParamsMouth {
    spcommon_s cmn;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    char  filename_op[MAX_FILENAME];
    char  filename_cl[MAX_FILENAME];
    float minScaleX;
    float maxScaleX;
    float minScaleY;
    float maxScaleY;
} params_mouth_s;

static params_mouth_s mouth[MAX_EXPRESSION] = {
    { spcommon, 158, 175, 50, 40, "/bmp/mouth_op_normal_4bit.bmp", 
      "/bmp/mouth_cl_normal_4bit.bmp", 1.0, 1.0, 0.3, 1.0 },
    { spcommon, 158, 175, 60, 45, "/bmp/mouth_op_angry_4bit.bmp",
      "/bmp/mouth_cl_angry_4bit.bmp",  1.0, 1.0, 0.3, 1.0 },
    { spcommon, 158, 175, 60, 45, "/bmp/mouth_op_angry_4bit.bmp",
      "/bmp/mouth_cl_angry_4bit.bmp",  1.0, 1.0, 0.3, 1.0 },
};

// ---------- Eyes Config ----------
typedef struct SpriteParamsEyes {
    spcommon_s cmn;
    int16_t  rx;        // Right X
    int16_t  ry;        // Right Y 
    int16_t  lx;        // Left  X
    int16_t  ly;        // Left  Y
    uint16_t w;         // width of eyelid
    uint16_t h;         // height of eyelid
    uint16_t ballcrx;    // eyeball Center right X
    uint16_t ballcry;    // eyeball Center right Y
    uint16_t ballclx;    // eyeball Center left  X
    uint16_t ballcly;    // eyeball Center left  Y
    char  filename_op[MAX_FILENAME];
    char  filename_cl[MAX_FILENAME];
    float minScaleX;
    float maxScaleX;
    float minScaleY;
    float maxScaleY;
} params_eye_s;

static params_eye_s eyes[MAX_EXPRESSION]= {
    { spcommon, 113, 110, 208, 110, 63, 73, 35, 41, 30, 41, "/bmp/eye_r_op_normal_4bit.bmp",
      "/bmp/eye_r_cl_normal_4bit.bmp", 1.0, 1.0, 0.3, 1.0 },
    { spcommon, 113, 110, 208, 110, 63, 73, 35, 41, 30, 41, "/bmp/eye_r_op_angry_4bit.bmp",
      "/bmp/eye_r_cl_angry_4bit.bmp", 1.0, 1.0, 0.3, 1.0 },
    { spcommon, 113, 110, 208, 110, 63, 73, 35, 41, 30, 41, "/bmp/eye_r_op_angry_4bit.bmp",
      "/bmp/eye_r_cl_angry_4bit.bmp", 1.0, 1.0, 0.3, 1.0 },
};

// ---------- Eyebrows Config ----------
typedef struct SpriteParamsEyeBrows {
    spcommon_s cmn;
    int16_t  rx;        // Right X
    int16_t  ry;        // Right Y 
    int16_t  lx;        // Left  X
    int16_t  ly;        // Left  Y
    uint16_t w;         // width of eyelid
    uint16_t h;         // height of eyelid
    char  filename[MAX_FILENAME];
} params_eyebrow_s;

static params_eyebrow_s eyebrows[MAX_EXPRESSION]= {
    { spcommon, 113,  55, 208,  55, 75, 24, "/bmp/eyebrow_normal_4bit.bmp" },
    { spcommon, 113,  55, 208,  55, 75, 24, "/bmp/eyebrow_angry_4bit.bmp" },
    { spcommon, 113,  55, 208,  55, 75, 24, "/bmp/eyebrow_angry_4bit.bmp"  }
};

// move Parameters
typedef struct moveParam {
    int eyeballX;
    int eyeballY;
    float eye_l_ratio;
    float eye_r_ratio;
    float mouth_ratio;
    float angle;
    float eyebrow_l_angle;
    float eyebrow_r_angle;
    int breath;
} move_param_s;
static move_param_s move_init_param[MAX_EXPRESSION]= {
  { 0,  0,  0.0,  0.0, 0.0, 0.0,  0.0,    0.0, 0 },
  { 0,  0,  0.0,  0.0, 0.0, 0.0,  20.0, -20.0, 0 },
  { 0, -10, 0.0,  0.0, 0.0, 0.0, -20.0,  20.0, 0 }
};

#endif
