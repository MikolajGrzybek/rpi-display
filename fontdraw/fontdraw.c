#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "fontdraw.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftimage.h"
#include "freetype/fttypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>

// TODO: Refactor code so that the font type could be specified as a function argument
// and the macro or envvar stored only the base path (as it is not uniform among linux systems).
#define FONT_PATH_DEFAULT "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"

#ifdef ENVVAR_CONFIG
    #define FONT_PATH_ENVVAR "SBT_DISPLAY_FONT_PATH"
    #define FILEPATH_BUFFSIZE 200
#endif


int init_freetype_font(FT_Face* fdata){
    int err;

    FT_Library lib;
    FT_Face* face = fdata;

    err = FT_Init_FreeType(&lib);
    if(err){
        fprintf(stderr, "Failed to link the Freetype library.\n");
        return -1;
    }


    #ifdef ENVVAR_CONFIG
        //if an environment variable is set load a font from the specified path instead of the default one
        char font_path_override[FILEPATH_BUFFSIZE];

        if(getenv(FONT_PATH_ENVVAR)){
            if(snprintf(font_path_override, FILEPATH_BUFFSIZE, "%s", getenv(FONT_PATH_ENVVAR)) < FILEPATH_BUFFSIZE)
                err = FT_New_Face(
                    lib, 
                    font_path_override, 
                    0, 
                    face
                );
            else{
                fprintf(stderr, "The buffer meant to hold envvar value was too small, the file path was too long.\n");
                return -1;
            }
        }
        else
            err = FT_New_Face(
                lib, 
                FONT_PATH_DEFAULT, 
                0,
                face
            );
    #else
        err = FT_New_Face(
            lib,
            FONT_PATH_DEFAULT,
            0,
            face
        );
    #endif // ENVVAR_CONFIG


    if(err){
        fprintf(stderr, "Failed to open the font file.\n");
        return -1;
    }

    return 0;
}

int configure_font(FT_Face face, uint width, uint height){
    int err;

    err = FT_Set_Pixel_Sizes(
        face,
        width,
        height
    );

    if(err){
        fprintf(stderr, "Failed to configure the font to have size of %dx%d\n", width, height);
        return -1;
    }

    return 0;
}

// Use UTF-32 Unicode representation for charcode
int font_glyph_to_slot(FT_Face face, FT_ULong charcode){
    int err;

    FT_UInt glyph_index = FT_Get_Char_Index(face, charcode);

    if(glyph_index == 0){
        // This error can be left unhandled. What will happen then is that instead
        // of the desired glyph a special "missing glyph" will be loaded 
        // (usually an empty box).
        fprintf(stderr, "Failed to find a glyph for charcode %ld.\n", charcode);
        return -2;
    }

    err = FT_Load_Glyph(
        face, 
        glyph_index, 
        FT_LOAD_DEFAULT | FT_LOAD_TARGET_MONO
    );

    if(err){
        fprintf(stderr, "Failed to load glyph.\n");
        return -1;
    }

    err = FT_Render_Glyph(
        face->glyph, 
        FT_RENDER_MODE_NORMAL
    );

    if(err){
        fprintf(stderr, "Failed to render glyph.\n");
        return -1;
    }

    return 0;
}

int paint_glyph(FT_GlyphSlot gslot, UWORD xpos, UWORD ypos, double brighten){
    if(brighten > 1.0){
        fprintf(stderr, "Cannot paint a glyph which was brightened with a value bigger than 1,0.\n");
        return -1;
    }
    FT_Bitmap bmp = gslot->bitmap;

    UWORD height = bmp.rows;
    UWORD width = bmp.width;


    // manipulate the height of the bitmap so that symbols like 'g'
    // have their tails below a baseline. 
    // assuming: subtraction = go back to top of the screen
    // the scalling of horiBearingY looses some precision in current implementation.
    ypos -= (gslot->metrics.horiBearingY >> 6);

    UWORD i,j,p,q;

    double gray;

    for(j = ypos, q = 0; j < height + ypos; j++, q++){
        for(i = xpos, p = 0; i < width + xpos; i++, p++){
            gray = bmp.buffer[q * bmp.pitch + p];
            if(gray == 0x00) continue; //treat completely white pixel as "transparent" - i.e. do not overwite with complete what was already painted. 
            gray -= brighten * 0xFF; 
            printf("eh 1     %d | %d \n", i, j);
            Paint_SetPixel(i, j, 0xFF - gray);
            printf("X\n");
        }
        printf("D   %d\n", j);
    }
    printf("D\n");
    return 0;
}

int paint_string(FT_Face face, FT_ULong* charcodes, unsigned int chars_num, UWORD xpos, UWORD ypos, double brighten){
    FT_GlyphSlot slot = face->glyph;
    UWORD pen_pos = xpos;
    int err;

    for(uint i = 0; i < chars_num; i++){
        printf("got u 1\n");
        err = font_glyph_to_slot(face, charcodes[i]);
        if(err){
            return -1;
        }
        printf("got u 2\n");
        err = paint_glyph(slot, pen_pos, ypos, brighten);
        if(err){
                    printf("got u 3\n");

            return -1;
        }
        printf("got u 3\n");
        
        pen_pos += slot->advance.x >> 6; // the ">> 6" is here because of freetype's weird unit standards, i.e. it is a unit conversion
    }
    return 0;
}

//Uses font width to fix the position slightly, meant to be used only by 
//paint_string_centered(). Otherwise it would be reasonable to fix paint_string and get rid of the
//necessity to apply the corrections of font width and height to the starting poiny coordinates. 
//However, loading this kind of data from the font just forces unnecessary loads and could
//slow down (may not) the program.
int paint_string_corrected(FT_Face face, FT_ULong *charcodes, unsigned int chars_num, UWORD xpos, UWORD ypos, double brighten, UWORD font_width){
    FT_GlyphSlot slot = face->glyph;
    UWORD pen_pos = xpos + font_width;
    int err;

    for(uint i = 0; i < chars_num; i++){
        err = font_glyph_to_slot(face, charcodes[i]);
        if(err){
            return -1;
        }
        err = paint_glyph(slot, pen_pos, ypos, brighten);
        if(err){
            return -1;
        }
        pen_pos += slot->advance.x >> 6; // the ">> 6" is here because of freetype's weird unit standards, i.e. it is a unit conversion
    }
    return 0;
}

int paint_string_centered(FT_Face face, FT_ULong* charcodes, unsigned int chars_num, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten){
    FT_GlyphSlot slot = face->glyph;
    //load glyph and read its data
    font_glyph_to_slot(face, 0x0041); //use always the same glyph for width calc
    FT_Bitmap bmp = slot->bitmap;
    UWORD width = bmp.width;
    //estimate text length
    UWORD text_length = (unsigned int) width * chars_num * 1.05;
    UWORD area_length = xpos_end - xpos_start;
    //calculate spacer size
    UWORD spacer = (area_length - text_length) / 2;

    paint_string_corrected(face, charcodes, chars_num, xpos_start + spacer, ypos, brighten, width);
}

FT_ULong digit_to_unicode(unsigned short digit){
    switch (digit) {
    case 0:
        return _0;
    case 1:
        return _1;
    case 2:
        return _2;
    case 3:
        return _3;
    case 4:
        return _4;
    case 5:
        return _5;
    case 6:
        return _6;
    case 7:
        return _7;
    case 8:
        return _8;
    case 9:
        return _9;
    default:
        return _dot;
    }
}

int integer_to_unicode(int num, FT_ULong* arr){
    int digit_store[MAX_INTEGER_LENGTH];

    //write digits into an array in reversed order
    unsigned int digit_num = 0;
    while (num) {
        digit_store[digit_num] = num % 10;
        digit_num++;
        num /= 10; 
    }
    //read the digits, translate them to unicode and create a unicode "string"
    for(int i = digit_num; i > 0; i--){
        FT_ULong unicode =  digit_to_unicode(digit_store[i - 1]);
        arr[digit_num - i] = unicode;
    }
    return digit_num;
}

int after_comma_to_unicode(int num, int precision, FT_ULong* arr){
    for(int i = 0; i < precision; i++){
        arr[precision - i - 1] = digit_to_unicode( (int) (num / pow(10, i)) % 10);
    }
    return precision;
}

int float_to_unicode(double num, int precision, FT_ULong* arr){
    int after_comma;
    int before_comma;

    double powered = num * pow(10, precision);
    int integerized = (int) (powered) / 1;
    //round it up
    if(powered - (double) integerized >= 0.5){
        ++integerized;
    }

    before_comma = integerized / pow(10, precision);
    after_comma = integerized - (before_comma * pow(10, precision));

    int in_front_digits = integer_to_unicode(before_comma, arr);
    arr[in_front_digits] = _dot;
    after_comma_to_unicode(after_comma, precision, arr + in_front_digits + 1);

    return in_front_digits + 1 + precision;
}


int paint_integer(FT_Face face, int num, UWORD xpos, UWORD ypos, double brighten){
    //it might have been slightly unnecessary to refactor that one out from paint_float
    FT_ULong int_string[MAX_INTEGER_LENGTH];
    int digit_num = integer_to_unicode(num, int_string);

    return paint_string(face, int_string, digit_num, xpos, ypos, brighten);
}

int paint_float(FT_Face face, double num, unsigned int precision, UWORD xpos, UWORD ypos, double brighten){
    FT_ULong float_string[MAX_INTEGER_LENGTH * 2 + 1]; //change to use a different constant/macro ... or not. It's not like we will run out of memory because of it.

    int len = float_to_unicode(num, precision, float_string);

    return paint_string(face, float_string, len, xpos, ypos, brighten);
}