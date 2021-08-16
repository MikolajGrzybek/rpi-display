#include "sbt_fontdraw.h"
#include "DEV_Config.h"
#include "fontdraw.h"
#include "freetype/freetype.h"
#include <stdio.h>
#include <math.h>


int paint_percent(FT_Face face, double num, UWORD xpos, UWORD ypos, double brighten){
    int perc = num * 100; //a neat cast to int should happen automatically here
    if(perc > 100){
        fprintf(stderr, "The number to be represented in the percent format exceeded value 1.0 (100%%). It may get stripped on the display of the digits in front.\n");
        perc %= 1000;
    }
    FT_ULong perc_string[4];
    int len = integer_to_unicode(perc, perc_string);
    perc_string[len] = _perc;

    return paint_string(face, perc_string, len + 1, xpos, ypos, brighten);
}

int paint_percent_centered(FT_Face face, double num, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten){
    int perc = num * 100; //a neat cast to int should happen automatically here
    if(perc > 100){
        fprintf(stderr, "The number to be represented in the percent format exceeded value 1.0 (100%%). It may get stripped on the display of the digits in front.\n");
        perc %= 1000;
    }
    FT_ULong perc_string[4];
    int len = integer_to_unicode(perc, perc_string);
    perc_string[len] = _perc;

    return paint_string_centered(face, perc_string, len + 1, xpos_start, xpos_end, ypos, brighten);
}

int paint_temperature(FT_Face face, double temp, UWORD xpos, UWORD ypos, double brighten){
    int temperature = temp;
    if(temperature >= pow(10, MAX_INTEGER_LENGTH)){
        fprintf(stderr, "The value passed as temperature exceeded the allowed number of digits. It will be stripped on the display.");
        temperature %= (int) pow(10, MAX_INTEGER_LENGTH);
    }
    FT_ULong temp_string[MAX_INTEGER_LENGTH + 2];
    int len = integer_to_unicode(temperature, temp_string);
    temp_string[len] = _degree;
    temp_string[len + 1] = _C;
    return paint_string(face, temp_string, len + 2, xpos, ypos, brighten);
}

int paint_temperature_centered(FT_Face face, double temp, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten){
    int temperature = temp;
    if(temperature >= pow(10, MAX_INTEGER_LENGTH)){
        fprintf(stderr, "The value passed as temperature exceeded the allowed number of digits. It will be stripped on the display.");
        temperature %= (int) pow(10, MAX_INTEGER_LENGTH);
    }
    FT_ULong temp_string[MAX_INTEGER_LENGTH + 2];
    int len = integer_to_unicode(temperature, temp_string);
    temp_string[len] = _degree;
    temp_string[len + 1] = _C;
    return paint_string_centered(face, temp_string, len + 2, xpos_start , xpos_end, ypos, brighten);
}

int paint_float_centered(FT_Face face, double num, int precision, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten){
    FT_ULong float_string[MAX_INTEGER_LENGTH * 2 + 1]; 
    int len = float_to_unicode(num, precision, float_string);
    return paint_string_centered(face, float_string, len, xpos_start, xpos_end, ypos, brighten);
}