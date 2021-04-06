/**
 * s_display_x.h
 *
 * Describes how register X appears on the display. Also used for editing
 * register X.
 */

#ifndef S_DISPLAY_X_H
#define S_DISPLAY_X_H

#include "n59.h"
#include "s_err.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

typedef enum s_display_x_edit_e {
  DISPLAY_X_EDIT_NONE,
  DISPLAY_X_EDIT_MANT,
  DISPLAY_X_EDIT_EXP
} s_display_x_edit_t;

typedef struct s_display_x_s {
  char display[N_N2S_MAX_SIZE];
  s_display_x_edit_t edit;
} s_display_x_t;


/******************************************************************************
 *
 *  FUNCTIONS.
 *
 ******************************************************************************/

/**
 * Sets display to '0', and puts it in edit mode.
 *
 * This is called when 'CLR' is pressed on TI-59.
 */
void s_display_x_init(s_display_x_t *display_x);

/**
 * Set display to represent 'X', taking into account 'fix' and 'format'. The
 * display is put in no edit mode..
 *
 * This function should be called whenever X is modified, for example after
 * evaluating a math function.
 */
void s_display_x_set_with_x(s_display_x_t *display_x, n_t X, int fix,
                            n_format_t format, s_err_t *err_out);

/**
 * In edit mode, adds digit 'd' to display if possible.
 *
 * In non edit mode, sets display to 'd', ready for further editing.
 */
void s_display_x_digit(s_display_x_t *display_x, int d);

/**
 * In edit mode, adds '.' to display, if not already present, and sets edit mode
 * to DISPLAY_X_EDIT_MANT.
 *
 * In non edit mode, sets display to '0.', ready for further editing.
 */
void s_display_x_dot(s_display_x_t *display_x);

/**
 * In edit mode, changes the sign of the mantissa or exponent.
 *
 * In non edit mode, does nothing. Instead s_math_chs should be called, followed
 * by s_display_x_set_with_x.
 */
void s_display_x_chs(s_display_x_t *display_x);

/**
 * Whether in edit or non edit mode:
 * - adds exponent ' 00' if it is missing and there is enough space.
 * - if ' 00' is added or exponent is already present, sets 'edit' to
 *   DISPLAY_X_EDIT_EXP.
 * - if there is no room for exponent, does nothing.
 *
 * Note that this function has the ability to put in edit mode a number that has
 * an exponent.
 *
 * This is called when 'EE' is pressed on TI-59. A separate function (s_mode_ee)
 * should be called to set the EE mode.
 */
void s_display_x_ee(s_display_x_t *display_x);

/**
 * In edit mode, sets 'edit' to DISPLAY_X_EDIT_MANT.
 *
 * In non edit mode, does nothing.
 *
 * This is called when 'INV EE' is pressed on TI-59. A separate function
 * (s_mode_iee) should be called to unset the EE mode.
 */
void s_display_x_iee(s_display_x_t *display_x);

#endif  // S_DISPLAY_X_H
