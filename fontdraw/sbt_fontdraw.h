/**
 * @file sbt_fontdraw.h is meant to store specialized paint functions that work well with
 * the input received through CAN from the boat.
 */

#include <ft2build.h>
#include FT_FREETYPE_H
#include "DEV_Config.h"

/**
 * @brief Creates a string of e.g. "45%". The percent value cannot exceed 3 digits.
 */
int paint_percent(FT_Face face, double num, UWORD xpos, UWORD ypos, double brighten);

/**
 * @brief TODO
 */
int paint_percent_centered(FT_Face face, double num, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten);

/**
 * @brief Takes a `double` value, makes an integer out of it and creates a string of 
 * format e.g. "215°C" (an integer).
 */
int paint_temperature(FT_Face face, double temp, UWORD xpos, UWORD ypos, double brighten);

/**
 * @brief TODO
 */
int paint_temperature_centered(FT_Face face, double temp, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten);

/**
 * @brief TODO
 * 
 */
int paint_float_centered(FT_Face face, double num, int precision, UWORD xpos_start, UWORD xpos_end, UWORD ypos, double brighten);

