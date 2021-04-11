/**
 * s_display_x.h
 *
 * Used for editing and displaying register X.
 */

#ifndef S_DISPLAY_X_H
#define S_DISPLAY_X_H

#include "n59.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

/** Describes what the display is showing. */
typedef enum s_display_x_mode_e {
  DISPLAY_X_MODE_REG,        // register X.
  DISPLAY_X_MODE_EDIT_MANT,  // a number whose mantissa is being edited.
  DISPLAY_X_MODE_EDIT_EXP    // a number whose exponent is being edited.
} s_display_x_mode_t;

/** The display and the state necessary for editing and showing it. */
typedef struct s_display_x_s {
  char display[N_N2S_MAX_SIZE];  // X or number being edited.
  bool blink;                    // whether the display is blinking.
  s_display_x_mode_t mode;       // how to interpret the display.

  n_t x;                         // number being shown.
  int fix;                       // current fix value.
  bool ee;                       // current ee mode.
  bool eng;                      // current eng mode.
} s_display_x_t;


/******************************************************************************
 *
 *  EDITING.
 *
 ******************************************************************************/

/**
 * Inits display.
 *
 * This function should be called when TI-59 is turned on.
 *
 * Mode: n/a -> DISPLAY_X_MODE_EDIT_MANT.
 */
void s_display_x_edit_init(s_display_x_t *display_x);

/**
 * Resets display to '0'.
 *
 * This function should be called when 'CLR' is pressed on TI-59.
 *
 * Mode: * -> DISPLAY_X_MODE_EDIT_MANT.
 */
void s_display_x_edit_clear(s_display_x_t *display_x);

/**
 * Sets blink to false. In addition, if mode is edit, resets display to '0'.
 *
 * This function should be called when 'CE' is pressed on TI-59.
 *
 * Mode: edit     -> DISPLAY_X_MODE_EDIT_MANT.
 *       register -> register.
 */
void s_display_x_edit_clear_entry(s_display_x_t *display_x);

/**
 * In edit mode, adds digit 'd' to display if possible.
 * In register mode, sets display to 'd'.
 *
 * This function should be called when a digit is pressed on TI-59.
 *
 * Mode: * -> edit.
 */
void s_display_x_edit_digit(s_display_x_t *display_x, int d);

/**
 * In edit mode, adds '.' to display, if not already present.
 * In register mode, sets display to '0.'.
 *
 * This function should be called when '.' is pressed on TI-59.
 *
 * Mode: * -> DISPLAY_X_MODE_EDIT_MANT.
 */
void s_display_x_edit_dot(s_display_x_t *display_x);

/**
 * Changes the sign of the mantissa or exponent.
 *
 * This function should be called when '+/-' is pressed on TI-59. A separate
 * function ('s_math_chs') should be called to change the sign of X.
 *
 * Do not call 's_display_x_update_from_reg' as we want to show '-0' and not
 * '0', for example.
 *
 * Mode: keeps mode.
 */
void s_display_x_edit_chs(s_display_x_t *display_x);


/******************************************************************************
 *
 *  MODES.
 *
 ******************************************************************************/

/** Updates fix. */
void s_display_x_mode_fix(s_display_x_t *d, int fix);

/**
 * Sets scientific mode, sets display to edit mode and possibly adds exponent.
 *
 * If there is no exponent and there is enough space, adds ' 00'. If ' 00' is
 * added or exponent is already present, sets mode to DISPLAY_X_MODE_EDIT_EXP.
 * Otherwise, sets mode to DISPLAY_X_MODE_EDIT_MANT.
 *
 * This should be called when 'EE' is pressed on TI-59.
 *
 * Mode: * -> edit.
 */
void s_display_x_mode_ee(s_display_x_t *d);

/**
 * Unsets scientific mode.
 *
 * In edit mode, sets 'mode' to DISPLAY_X_MODE_EDIT_MANT. In register mode, does
 * nothing.
 *
 * This should be called when 'INV EE' is pressed on TI-59.
 *
 * Mode: edit     -> DISPLAY_X_MODE_EDIT_MANT.
 *       register -> abort or noop.
 */
void s_display_x_mode_iee(s_display_x_t *d);

/** Sets engineering notation. */
void s_display_x_mode_eng(s_display_x_t *d);

/** Unsets engineering notation. */
void s_display_x_mode_ieng(s_display_x_t *d);


/******************************************************************************
 *
 *  SYNCHRONIZATION DISPLAY X <=> REGISTER X.
 *
 ******************************************************************************/

/**
 * Updates display using the values of X, fix, format and sets the blink flag
 * to true if overflow.
 *
 * This function should be called when X, fix or format are modified, or when
 * the display may start blinking because of overflow, after a key stroke.
 *
 * Examples:
 * - 'lnx': math function, X modified.
 * - 'Eng': possible format change.
 * - 'Fix 0 9.99 EE 99 STO': 'Fix', possible overflow, 'Fix 0', fix modified,
 *   'STO', definitive overflow.
 * - 'RCL 59': possible overflow after 'RCL'. Then X is modified after 'RCL 59'.
 *
 * Non examples:
 * - '.', 'EE' or '0': editing keys
 * - 'Deg', 'Lbl' or 'RST': keep display in edit mode.
 *
 * This function should not be called when calling 's_display_x_clear' or
 * 's_display_x_chs'.
 *
 * Mode: * -> register.
 */
void s_display_x_update_from_reg(s_display_x_t *display_x,
                                 n_t X, int fix, n_format_t format);

// Update display, blink. (?)
void s_display_x_update_x(s_display_x_t *display_x, n_t X);

/**
 * Updates register X using the value on the display and sets the blink flag to
 * true if overflow/underflow.
 *
 * This function should be called when, and only when, the display is in edit
 * mode and the user is done editing it.
 * 
 * The user is done editing the display when one of several keys is pressed such
 * as 'lnx', 'STO' and 'CP'. On the other hand, keys such as 'Lbl', 'Deg' and
 * '+/-', do not end editing mode.
 *
 * If this method is called, 's_display_x_update_from_reg' should be called just
 * after the key that ends editing mode is evaluated.
 *
 * Mode: edit     -> edit.
 *       register -> abort.
 */
void s_display_x_update_reg(s_display_x_t *display_x, n_t *X);

#endif  // S_DISPLAY_X_H
