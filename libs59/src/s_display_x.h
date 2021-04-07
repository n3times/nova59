/**
 * s_display_x.h
 *
 * Used to input and output numbers into/from register X.
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
 *  EDITING.
 *
 ******************************************************************************/

/**
 * Sets display to '0' putting it in edit mode.
 *
 * This is called when 'CLR' is pressed on TI-59.
 */
void s_display_x_edit_start(s_display_x_t *display_x);

/**
 * In edit mode, adds digit 'd' to display if possible.
 *
 * In non edit mode, sets display to 'd', ready for further editing.
 */
void s_display_x_edit_digit(s_display_x_t *display_x, int d);

/**
 * In edit mode, adds '.' to display, if not already present, and sets edit mode
 * to DISPLAY_X_EDIT_MANT.
 *
 * In non edit mode, sets display to '0.', ready for further editing.
 */
void s_display_x_edit_dot(s_display_x_t *display_x);

/**
 * In edit mode, changes the sign of the mantissa or exponent.
 *
 * In non edit mode, does nothing. Instead s_math_chs should be called, followed
 * by s_display_x_set_with_x.
 */
void s_display_x_edit_chs(s_display_x_t *display_x);

/**
 * Whether in edit or non edit mode:
 * - adds exponent ' 00' if it is missing and there is enough space.
 * - if ' 00' is added or exponent is already present, sets 'edit' to
 *   DISPLAY_X_EDIT_EXP.
 * - if not, sets 'edit' to DISPLAY_X_EDIT_MANT.
 *
 * This is called when 'EE' is pressed on TI-59. A separate function (s_mode_ee)
 * should be called to set the EE mode.
 */
void s_display_x_edit_ee(s_display_x_t *display_x);

/**
 * In edit mode, sets 'edit' to DISPLAY_X_EDIT_MANT.
 *
 * In non edit mode, does nothing.
 *
 * This is called when 'INV EE' is pressed on TI-59. A separate function
 * (s_mode_iee) should be called to unset the EE mode.
 */
void s_display_x_edit_iee(s_display_x_t *display_x);


/******************************************************************************
 *
 *  SYNCHRONIZATION DISPLAY X <=> REGISTER X.
 *
 ******************************************************************************/

/**
 * Updates display using the values of X, fix and format. For example if
 * X.mant = 3141500000000, x.exp = 0, fix = 2 and format = FLOAT then display is
 * updated to '3.15'.
 *
 * Display should be in non edit mode before calling this method and will remain
 * in non edit mode after.
 *
 * This function should be called after X has been modified, for example after
 * a math operation.
 */
void s_display_x_update_display(s_display_x_t *display_x, n_t X, int fix,
                                n_format_t format, s_err_t *err_out);

/**
 * Updates X using the value on the  display. For example if display is '3.15',
 * then X is updated with X.mant = 3150000000000 and X.exp = 0.
 *
 * Display should be in non edit mode before calling this method and will remain
 * in non edit mode after.
 *
 * This function should be called after the display has been edited.
 */
void s_display_x_update_x(
    s_display_x_t *display_x, n_t *X, s_err_t *err_out);

#endif  // S_DISPLAY_X_H
