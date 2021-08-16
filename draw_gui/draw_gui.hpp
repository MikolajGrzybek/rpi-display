#ifndef DRAW_GUI_HPP
#define DRAW_GUI_HPP

#include <time.h>
#include <ft2build.h>
#include FT_FREETYPE_H

extern "C"{
    #include "epaper/e-Paper/EPD_IT8951.h"
    #include "epaper/GUI/GUI_Paint.h"
    #include "epaper/GUI/GUI_BMPfile.h"
    #include "epaper/Config/Debug.h"
    #include "epaper/Config/DEV_Config.h"
    #include "fontdraw/fontdraw.h"
    #include "fontdraw/sbt_fontdraw.h"
    #include "UI_config/utf_text.h"
    #include "UI_config/font.h"
}

#define BitsPerPixel_4 4
#define BitsPerPixel_8 8


#define ROW_1 72
#define ROW_2 502
#define ROW_3 944

#define COL_1 382
#define COL_2 600
#define COL_3 850

#define SIZE_X 442
#define SIZE_Y 120

extern UBYTE *Refresh_Frame_Buf;
extern bool Four_Byte_Align;


typedef struct{
    UWORD Dynamic_Area_Width;
    UWORD Dynamic_Area_Height;

    UDOUBLE Imagesize;

    UWORD Start_X;
    UWORD Start_Y;
    
    UBYTE *Dynamic_Refresh_Frame_Buf = NULL;

    UWORD Dynamic_Area_Count;
    UWORD Repeat_Area_Times;
} Dynamic_Refresh_Area;


UBYTE Display_InitGui(UWORD Panel_Width, UWORD Panel_Height, UDOUBLE Init_Target_Memory_Addr);

void InitGui();

UBYTE Dynamic_Refresh(UWORD width, UWORD height, UWORD start_x, UWORD start_y, int xd, char unit[]);


#endif // DRAW_GUI_HPP
