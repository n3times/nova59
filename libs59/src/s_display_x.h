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

/** Whether register X is being displayed or edited. */
typedef enum s_display_x_mode_e {
  DISPLAY_X_MODE_REG,        // register X is being displayed.
  DISPLAY_X_MODE_EDIT_MANT,  // X's mantissa is being edited.
  DISPLAY_X_MODE_EDIT_EXP    // X's exponent is being edited.
} s_display_x_mode_t;

/**
 * The display state.
 *
 * In register mode, 'display' is fully determined by x, fix, ee and eng.
 * In edit mode, 'display' is updated by editing key strokes such as digits and
 * '.'.
 */
typedef struct s_display_x_s {
  char display[N_N2S_MAX_SIZE];  // X or number being edited.
  s_display_x_mode_t mode;       // how to interpret the display.
  bool blink;                    // whether the display is blinking.
  n_t x;                         // in register mode, number being shown.
  int fix;                       // fix value.
  bool ee;                       // ee mode.
  bool eng;                      // eng notation.
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
 * Resets display to '0', no blinking and in edit mode.
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
 * This function should be called when '+/-' is pressed on TI-59. In register
 * mode a separate function ('s_math_chs') should also be called to change the
 * sign of X.
 *
 * Typically when a function modifies X, 's_display_x_update_x' should be called
 * but not in this case, as we want to show '-0' and not '0', for example.
 *
 * Mode: keeps mode.
 */
void s_display_x_edit_chs(s_display_x_t *display_x);


/******************************************************************************
 *
 *  MODES.
 *
 ******************************************************************************/

/**
 * Updates fix.
 *
 * Updates display.
 *
 * Mode * -> register.
 */
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
 * In edit mode, sets 'mode' to DISPLAY_X_MODE_EDIT_MANT.
 *
 * This should be called when 'INV EE' is pressed on TI-59.
 *
 * Mode: edit     -> DISPLAY_X_MODE_EDIT_MANT.
 *       register -> register.
 */
void s_display_x_mode_iee(s_display_x_t *d);

/**
 * Sets engineering notation, possibly updating display.
 *
 * This should be called when 'Eng' is pressed on TI-59.
 *
 * Mode: * -> register.
 */
void s_display_x_mode_eng(s_display_x_t *d);

/**
 * Unsets engineering notation, possibly updating display.
 *
 * This should be called when 'INV Eng' is pressed on TI-59.
 *
 * Mode: * -> register.
 */
void s_display_x_mode_ieng(s_display_x_t *d);


/******************************************************************************
 *
 *  SYNCHRONIZATION DISPLAY X <=> REGISTER X.
 *
 ******************************************************************************/

/**
 * Updates display using X.
 *
 * This function should be called when register X has been modified by a
 * function.
 *
 * Examples: 'lnx', 'RCL 59'.
 * Non examples:
 * - editing keys: '.', 'EE' or '0'.
 * - keys that keep display in edit mode: 'Deg', 'Lbl' or 'RST'.
 * - keys/operations that don't change X: 'STO 59', 'CP'
 *
 * This function should not be called when calling 's_display_x_clear' or
 * 's_display_x_chs'.
 *
 * Mode: * -> register.
 */
void s_display_x_update_x(s_display_x_t *display_x, n_t X);

void s_display_x_set_blink(s_display_x_t *display_x);

/**
 * Determines and returns the number on the display.
 *
 * In addition, updates 'display', 'X', 'blink' if overflow/underflow, and sets
 * 'mode' to register.
 *
 * This function should be called when, and only when, the display is in edit
 * mode and the user is done editing it.
 * 
 * The user is done editing the display when one of several keys is pressed such
 * as 'lnx', 'STO' and 'CP'. On the other hand, keys such as 'Lbl', 'Deg' and
 * '+/-', do not end editing mode.
 *
 * If this method is called, 's_display_x_update_x' should be called just
 * after the key that ends editing mode is evaluated.
 *
 * Mode: edit     -> edit.
 *       register -> abort.
 */
n_t s_display_x_resolve_edit(s_display_x_t *display_x);

#endif  // S_DISPLAY_X_H
