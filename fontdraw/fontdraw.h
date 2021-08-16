#include "DEV_Config.h"
#include <ft2build.h>
#include FT_FREETYPE_H


/** 
 * The maximum number of digits that can be printed by print_integer().
 * Also, a max number of digits before as well as after the comma/dot in a floating point number 
 */
#define MAX_INTEGER_LENGTH 8 


enum UnicodeChars{
    _0 = 0x0030,
    _1 = 0x0031,
    _2 = 0x0032,
    _3 = 0x0033,
    _4 = 0x0034,
    _5 = 0x0035,
    _6 = 0x0036,
    _7 = 0x0037,
    _8 = 0x0038,
    _9 = 0x0039,
    _degree = 0x00B0,
    _dot = 0x002E,
    _perc = 0x0025,
    _C = 0x0043,
};

/**
 * Initialize a new font face.
 * 
 * @param face - pointer to a face to be initialized
 * @return int - non-zero value implies an error
 */
int init_freetype_font(FT_Face* face);

/**
 * Set desired size of font in pixels
 * 
 * @param face - font face to be configured
 * @param width - desired width in px
 * @param height - desired height in px
 * @return int - non-zero value implies error
 */
int configure_font(FT_Face face, uint width, uint height);

/**
 * Paint a string of text onto a epaper dirver's image cache.
 * 
 * @param face - Font face to be used
 * @param charcodes - Address of an array storing unicode values
 * of characters to be printed.
 * @param chars_num - Size of the array storing unicode characters.
 * @param xpos - Text start position.
 * @param ypos - Text start position.
 * @param brighten - Set to zero to achieve a pitch black font.
 * The maximum possible value of brighten is 1.0 which makes the
 * font perfectly white.
 * @remark The function is not responsible for screen refreshing.
 * and it requires a driver's image to be selected.
 * @remark The required format of Unicode can be checked in `UnicodeChars` enum.
 * @remark In the rendered image of a glyph/font white color is considered transparent.
 * If beneath the text there are already painted pixels they will not be overwitten with completely white pixels. 
 */
int paint_string(FT_Face face, FT_ULong* charcodes, uint chars_num, UWORD xpos, UWORD ypos, double brighten);

/**
 * @brief TODO
 * 
 * @param face 
 * @param charcodes 
 * @param chars_num 
 * @param xpos_start 
 * @param xpos_end 
 * @param ypos 
 * @param brighten 
 * @return int 
 */
int paint_string_centered(FT_Face face, FT_ULong *charcodes, uint chars_num, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten);

/**
 * @param face - See `paint_string()`
 * @param num - The integer to be printed
 * @param xpos - See `paint_string()`
 * @param ypos - See `paint_string()`
 * @param brighten - See `paint_string()`
 * @remark Negative numbers not supported
 */
int paint_integer(FT_Face face, int num, UWORD xpos, UWORD ypos, double brighten);

/**
 * @param face - See `paint_string()` 
 * @param num - The number to be printed
 * @param precision - The number of digits after comma to be displayed
 * @param xpos - See `paint_string()` 
 * @param ypos - See `paint_string()` 
 * @param brighten - See `paint_string()` 
 * @remark Negative numbers not supported
 */
int paint_float(FT_Face face, double num, unsigned int precision, UWORD xpos, UWORD ypos, double brighten);

/**
 * @brief Meant to be used to build more specific "paint" functions that use integers
 * 
 * @param num - number to be converted
 * @param arr - place to store the utf string
 * @return int - lenght of created string
 */
int integer_to_unicode(int num, FT_ULong* arr);

/**
 * @brief Similar to `integer_to_unicode()` but for floats. Requires specifying the number of
 * digits after the comma/dot.
 * 
 * @param num - number to be converted
 * @param precision - number of digits after the dot
 * @param arr - place to store the utf string
 * @return int - length of resulting string
 */
int float_to_unicode(double num, int precision, FT_ULong* arr);