/**
 * s_display_x.h
 *
 * Used to edit and display register X.
 *
 * In display mode, display X  contains a representation of register X, taking
 * into account the current 'fix' and 'format'.
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

typedef enum s_display_x_mode_e {
  DISPLAY_X_MODE_DISPLAY,
  DISPLAY_X_MODE_EDIT_MANT,
  DISPLAY_X_MODE_EDIT_EXP
} s_display_x_mode_t;

typedef struct s_display_x_s {
  char display[N_N2S_MAX_SIZE];
  s_display_x_mode_t mode;
} s_display_x_t;


/******************************************************************************
 *
 *  EDITING.
 *
 ******************************************************************************/

/**
 * Sets/resets display to '0'.
 *
 * This should be called:
 * - when 'CLR' is pressed.
 * - when 'CE' is pressed and display is in edit mode.
 * - when TI-59 is turned on.
 *
 * Transition: * -> edit mode.
 */
void s_display_x_edit_clear(s_display_x_t *display_x);

/**
 * In edit mode, adds digit 'd' to display if possible.
 * In display mode, sets display to 'd'.
 *
 * Transition: * -> edit mode.
 */
void s_display_x_edit_digit(s_display_x_t *display_x, int d);

/**
 * In edit mode, adds '.' to display, if not already present.
 * In display mode, sets display to '0.'.
 *
 * Transition: * -> DISPLAY_X_MODE_EDIT_MANT.
 */
void s_display_x_edit_dot(s_display_x_t *display_x);

/**
 * Changes the sign of the mantissa or exponent.
 *
 * In display mode, 's_math_chs' should be called too, but not
 * 's_display_x_update_display'.
 *
 * Transition: keeps mode.
 */
void s_display_x_edit_chs(s_display_x_t *display_x);

/**
 * Whether in edit or display mode:
 * - adds exponent ' 00' if it is missing and there is enough space.
 * - if ' 00' is added or exponent is already present, sets 'mode' to
 *   DISPLAY_X_MODE_EDIT_EXP. Otherwise, sets 'mode' to
 *   DISPLAY_X_MODE_EDIT_MANT.
 *
 * This should be called when 'EE' is pressed on TI-59. A separate function
 * ('s_mode_ee') should be called to set the EE mode.
 *
 * Transition: * -> edit mode.
 */
void s_display_x_edit_ee(s_display_x_t *display_x);

/**
 * In edit mode, sets 'mode' to DISPLAY_X_MODE_EDIT_MANT.
 *
 * In display mode, does nothing.
 *
 * This should be called when 'INV EE' is pressed on TI-59. A separate function
 * (s_mode_iee) should be called to unset the EE mode.
 *
 * Transition: edit    -> DISPLAY_X_MODE_EDIT_MANT.
 *             display -> abort or noop.
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
 * updated to '3.14'.
 *
 * This function should be called:
 * - soon after 's_display_x_update_x' is called.
 * - after X has been modified, except if X has been cleared
 *   (call s_display_x_edit_clear instead).
 * - after fix or format have been modified.
 *
 * Transition: * -> display.
 */
void s_display_x_update_display(s_display_x_t *display_x, n_t X, int fix,
                                n_format_t format, s_err_t *err_out);

/**
 * Updates X using the value on the display. For example if display is '3.15',
 * then X is updated with X.mant = 3150000000000 and X.exp = 0.
 *
 * Called just after the user has edited the display and just before an
 * operation is evaluated. For example:
 * - in '59 lnx', this function is called when the user presses 'lnx' but
 *   before the lnx is actually evaluated.
 * - in '59 STO 00', this function is called when the user presses 'STO' but
 *   before the 'STO 00' is actually evaluated.
 *
 * Whether this function is called depends on the operation. For example, this
 * function is called in '59 CP' but not in '59 Deg'.
 *
 * Typically the caller of this method will call 's_display_x_update_display'
 * soon after (after evaluating 'lnx' and after the user presses 'STO' in the
 * examples above).
 *
 * Transition: edit -> display.
 *             display -> abort or noop.
 */
void s_display_x_update_x(s_display_x_t *display_x, n_t *X, s_err_t *err_out);

#endif  // S_DISPLAY_X_H
