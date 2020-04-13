// Copyright(c) 2020 Takao Akaki

#ifndef _AVATARCONFIG_H_
#define _AVATARCONFIG_H_

#include <stdint.h>

#define MAX_EXPRESSION 1
#define MAX_FILENAME 40

#define NORMAL 0
#define SAD    1
#define ANGRY  2
#define HAPPY  3
#define DOUBT  4
#define SLEEPY 5

#define FIXED_PARTS 2

#define HEAD    0
#define BODY    1
// #define EYEBALL 2

#define RIGHT true
#define LEFT  false

#define EYEBALL_MAX 10

// Margin when using rotation
static int16_t marginX = 10; // left,right   (340-320)/2
static int16_t marginY = 10; // upper,lower  (260-240)/2

// About:color Value 
// when use palette  =>  palette index
// when dont use palette =>  color No. rgb565(0xXXXX) or rgb888(0xXXXXXXU)

// Avatar Color Settings
static uint16_t skinColor  = 15; // Color of skin
static uint16_t whiteColor = 15; // color fill eyeball
static uint16_t tpColor    = 3;  // Color of transparent

// Sprite Common Settings
typedef struct SpriteCommon {
  bool     psram;
  uint8_t  colorDepth;
  bool     swapBytes;
  uint16_t tpColor;
} spcommon_s;

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
  { spcommon, 0,   0, 340, 204, "/bmp/amb_head.bmp" },
  { spcommon, 0, 155, 340,  97, "/bmp/amb_body.bmp"  },
};

// eyeball
static params_fixed_s eyeball[MAX_EXPRESSION] = {
  { spcommon, 0,   0,  72,  72, "/bmp/amb_eyeball.bmp"  },
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
    { spcommon, 158, 135, 80, 82, "/bmp/amb_mouth_open_normal.bmp", 
      "/bmp/amb_mouth_close_normal.bmp", 1.0, 1.0, 0.3, 1.0 },
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
    { spcommon, 100, 76, 220, 76, 72, 72, 0, 0, 0, 0, "/bmp/amb_eye_open_normal.bmp",
      "/bmp/amb_eye_close_normal.bmp", 1.0, 1.0, 0.3, 1.0 },
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
    { spcommon, 105,  55, 225,  55, 60, 24, "/bmp/amb_eyebrow.bmp" },
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
};

#endif  // _AVATARCONFIG_H_
