#include "draw_gui.hpp"
#include "Debug.h"
#include "EPD_IT8951.h"
#include "GUI_Paint.h"
#include <sys/types.h>

IT8951_Dev_Info Dev_Info;

UWORD Panel_Width = 0;
UWORD Panel_Height = 0;
UDOUBLE Init_Target_Memory_Addr = 0;

UBYTE *Refresh_Frame_Buf = NULL;
bool Four_Byte_Align = true;
UBYTE BitsPerPixel = 8;


const int epd_mode = 0;
const UWORD VCOM = 2100;
extern UBYTE isColor; 

FT_Face font_small;
FT_Face font_digits;




static void Epd_Mode(int mode)
{
    if(mode == 3) {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_NONE);
        isColor = 1;
    }else if(mode == 2) {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_HORIZONTAL);
    }else if(mode == 1) {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_HORIZONTAL);
    }else {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_NONE);
    }
}





UBYTE Display_InitGui(UWORD Panel_Width, UWORD Panel_Height, UDOUBLE Init_Target_Memory_Addr){
    
    UWORD Display_Area_Height = Panel_Height;

    UWORD Display_Area_Width;

    if(Four_Byte_Align == true){
        Display_Area_Width = Panel_Width - (Panel_Width % 32);
    }else{
        Display_Area_Width = Panel_Width;
    }


    UDOUBLE Imagesize;

    Imagesize = ((Display_Area_Width * BitsPerPixel % 8 == 0)? (Display_Area_Width * BitsPerPixel / 8 ): (Display_Area_Width * BitsPerPixel / 8 + 1)) * Display_Area_Height;
    if((Refresh_Frame_Buf = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for image memory...\r\n");
        return -1;
    }

    Paint_NewImage(Refresh_Frame_Buf, Display_Area_Width, Display_Area_Height, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
    Epd_Mode(epd_mode);
    Paint_SetBitsPerPixel(BitsPerPixel);
    Paint_Clear(WHITE);

    /* ---- Actually print GUI ---- */

  

    /* ---- TOP BAR ---- */
    // BOAT MODE
    FT_ULong t_boat_mode[] = T_BOAT_MODE;
    paint_string(font_small, t_boat_mode, T_BOAT_MODE_L, 1020 + SMALL_FONT_W, 90 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    // BOAT STATUS 
    FT_ULong t_boat_status[] = T_BOAT_STATUS;
    paint_string(font_small, t_boat_status, T_BOAT_STATUS_L, 1020 + SMALL_FONT_W, 200 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    // CONSUMPTION BAR
    Paint_DrawRectangle(82, 88, 922, 272, 0x00, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
    Paint_DrawLine(292, 88, 292, 272, 0x00, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    Paint_DrawLine(502, 88, 292, 272, 0x00, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    Paint_DrawLine(712, 88, 292, 272, 0x00, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    /* ----------------- */


    /* ---- FIRST ROW ---- */
    // CHARGE
    FT_ULong t_charge[] = T_CHARGE;
    paint_string_centered(font_small, t_charge, T_CHARGE_L, 72, 502, 324 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    //DISCHARGE
    FT_ULong t_discharge[] = T_DISCHARGE;
    paint_string_centered(font_small, t_discharge, T_DISCHARGE_L, 503, 944, 324 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    //THROTTLE    
    FT_ULong t_throttle[] = T_THROTTLE;
    paint_string_centered(font_small, t_throttle, T_THROTTLE_L, 945, 1388, 324 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);
    /* ----------------- */

    
    /* ---- SECOND ROW ---- */
    // MAX VOLT.
    FT_ULong t_max_volt[] = T_MAX_VOLT;
    paint_string_centered(font_small, t_max_volt, T_MAX_VOLT_L, 72, 502, 562 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    // MIN VOLT.
    FT_ULong t_min_volt[] = T_MIN_VOLT;
    paint_string_centered(font_small, t_min_volt, T_MIN_VOLT_L, 503, 944, 562 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    //SPEED    
    FT_ULong t_speed[] = T_SPEED;
    paint_string_centered(font_small, t_speed, T_SPEED_L, 945, 1388, 562 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);
    /* ----------------- */


    /* ---- THIRD ROW ---- */
    // T. MAIN MOTOR
    FT_ULong t_main_motor[] = T_T_MAIN_MOTOR;
    paint_string_centered(font_small, t_main_motor, T_T_MAIN_MOTOR_L, 72, 502, 799 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    // T. AUX MOTOR
    FT_ULong t_aux_motor[] = T_T_AUX_MOTOR;
    paint_string_centered(font_small, t_aux_motor, T_T_AUX_MOTOR_L, 503, 944, 799 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    // T. EXTRA
    FT_ULong t_extra[] = T_T_EXTRA;
    paint_string_centered(font_small, t_extra, T_T_EXTRA_L, 945, 1388, 799 + SMALL_FONT_H, SMALL_FONT_BRIGHTEN);

    /* ----------------- */

    /* ===== FONTDRAW TEST ===== */

    //now as a pretty UI preview

    //preview SPEED
   // paint_float_centered(font_digits, 45.8, 1, 944, 1388, 732, 0.0);
    //preview THROTTLE
    //paint_percent_centered(font_digits, 0.62, 944, 1388, 494, 0.0);
    //preview T. EXTRA
   // paint_temperature_centered(font_digits, 65.5, 944, 1388, 970, 0.0);
   
    /* ========================= */



    switch(BitsPerPixel){
        case BitsPerPixel_8:{
            EPD_IT8951_8bp_Refresh(Refresh_Frame_Buf, 0, 0, Display_Area_Width,  Display_Area_Height, false, Init_Target_Memory_Addr);
            break;
        }
        case BitsPerPixel_4:{
            EPD_IT8951_4bp_Refresh(Refresh_Frame_Buf, 0, 0, Display_Area_Width,  Display_Area_Height, false, Init_Target_Memory_Addr,false);
            break;
        }
    }

    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Refresh_Frame_Buf = NULL;
    }

    return 0;
}

void InitGui(){  
    // Init the BCM2835 Device
    if(DEV_Module_Init()!=0){
        printf("ERROR");
        return;
    }

    Dev_Info = EPD_IT8951_Init(VCOM);

    Panel_Width = Dev_Info.Panel_W;
    Panel_Height = Dev_Info.Panel_H;
    Init_Target_Memory_Addr = Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);

    init_freetype_font(&font_small);
    configure_font(font_small, SMALL_FONT_W, SMALL_FONT_H);

    init_freetype_font(&font_digits);
    configure_font(font_digits, DIGIT_FONT_W, DIGIT_FONT_H);

    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    Display_InitGui(Panel_Width, Panel_Height, Init_Target_Memory_Addr);
    return;
}

UBYTE Dynamic_Refresh(UWORD width, UWORD height, UWORD start_x, UWORD start_y, int msg){
    Dynamic_Refresh_Area Area;

    Area.Dynamic_Area_Width = width;
    Area.Dynamic_Area_Height = height;
    Area.Start_X = start_x;
    Area.Start_Y = start_y;
    
    Area.Imagesize = ((Area.Dynamic_Area_Width * 1 % 8 == 0)? (Area.Dynamic_Area_Width* 1 / 8 ): (Area.Dynamic_Area_Width * 1 / 8 + 1)) * Area.Dynamic_Area_Height;
    if((Refresh_Frame_Buf = (UBYTE *)malloc(Area.Imagesize)) == NULL) {
        Debug("Failed to apply for image memory...\r\n");
        return -1;
    } 

    Paint_NewImage(Refresh_Frame_Buf, Area.Dynamic_Area_Width, Area.Dynamic_Area_Height, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
    Epd_Mode(epd_mode);
    Paint_SetBitsPerPixel(1);

    Paint_Clear(WHITE);
  
    /* DRAW HERE */
    Paint_DrawRectangle(start_x + 45, start_y + 45, start_x + Area.Dynamic_Area_Width - 45, start_y + Area.Dynamic_Area_Height - 45, 0x00, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    Paint_DrawNum(Area.Dynamic_Area_Width/2, Area.Dynamic_Area_Height/2, msg, &Font24, 0x00, 0xF0);     


    EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, Area.Start_X, Area.Start_Y, width, height, A2_Mode, Init_Target_Memory_Addr, true);
    
    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Refresh_Frame_Buf = NULL;
    }

    return 0;

}
