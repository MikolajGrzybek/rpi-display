#include "draw_gui.hpp"
#include <sys/types.h>

IT8951_Dev_Info Dev_Info;

UWORD Panel_Width = NULL;
UWORD Panel_Height = NULL;
UDOUBLE Init_Target_Memory_Addr = NULL;

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
    paint_float_centered(font_digits, 45.8, 1, 944, 1388, 732, 0.0);
    //preview THROTTLE
    paint_percent_centered(font_digits, 0.62, 944, 1388, 494, 0.0);
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


UBYTE Dynamic_Refresh_Example(IT8951_Dev_Info Dev_Info, UDOUBLE Init_Target_Memory_Addr, int n){
    UWORD Panel_Width = Dev_Info.Panel_W;
    UWORD Panel_Height = Dev_Info.Panel_H;

    UWORD Dynamic_Area_Width = 96;
    UWORD Dynamic_Area_Height = 48;

    UDOUBLE Imagesize;

    UWORD Start_X = 0,Start_Y = 0;

    UWORD Dynamic_Area_Count = 0;

    UWORD Repeat_Area_Times = 0;

    //malloc enough memory for 1bp picture first
    Imagesize = ((Panel_Width * 1 % 8 == 0)? (Panel_Width * 1 / 8 ): (Panel_Width * 1 / 8 + 1)) * Panel_Height;
    if((Refresh_Frame_Buf = (UBYTE *)malloc(Imagesize)) == NULL){
        Debug("Failed to apply for picture memory...\r\n");
        return -1;
    }

    clock_t Dynamic_Area_Start, Dynamic_Area_Finish;
    double Dynamic_Area_Duration;  

    while(1)
    {
        Dynamic_Area_Width = 430;
        Dynamic_Area_Height = 120;

        Start_X = 72;
        Start_Y = 382;

        Dynamic_Area_Count = 0;

        Dynamic_Area_Start = clock();
        Debug("Start to dynamic display...\r\n");       

        Imagesize = ((Dynamic_Area_Width % 8 == 0)? (Dynamic_Area_Width / 8 ): (Dynamic_Area_Width / 8 + 1)) * Dynamic_Area_Height;
        Paint_NewImage(Refresh_Frame_Buf, Dynamic_Area_Width, Dynamic_Area_Height, 0, BLACK);
        Paint_SelectImage(Refresh_Frame_Buf);
        Epd_Mode(epd_mode);
        Paint_SetBitsPerPixel(1);

        Paint_Clear(WHITE);

        //For color definition of all BitsPerPixel, you can refer to GUI_Paint.h
        Paint_DrawRectangle(50, 50, 100, 100, 0x00, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);

        Paint_DrawNum(Dynamic_Area_Width/2, Dynamic_Area_Height/2, n, &Font20, 0x00, 0xF0);
        //  paint_percent(font_digits, ++Dynamic_Area_Count/100, Dynamic_Area_Width/4, Dynamic_Area_Height/4, 0.0);

        EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, Start_X, Start_Y, Dynamic_Area_Width,  Dynamic_Area_Height, A2_Mode, Init_Target_Memory_Addr, true);
    
           

        Dynamic_Area_Finish = clock();
        Dynamic_Area_Duration = (double)(Dynamic_Area_Finish - Dynamic_Area_Start) / CLOCKS_PER_SEC;
        Debug( "Write and Show occupy %f second\n", Dynamic_Area_Duration );

        Repeat_Area_Times ++;
        if(Repeat_Area_Times > 0){
            break;
        }
    }
    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Refresh_Frame_Buf = NULL;
    }

    return 0;
}


