/**
 * xdisplay.h
 *
 * In charge of displaying and editing register X.
 *
 * Holds all the state necessary to display register X including fix, ee and
 * eng.
 */

#ifndef XDISPLAY_H
#define XDISPLAY_H

#include "n59.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

/** Whether register X is being displayed or edited. */
typedef enum xdisplay_mode_e {
  XDISPLAY_MODE_REG,        // register X is being displayed.
  XDISPLAY_MODE_EDIT_MANT,  // X's mantissa is being edited.
  XDISPLAY_MODE_EDIT_EXP    // X's exponent is being edited.
} xdisplay_mode_t;

/**
 * The display state.
 *
 * There are 2 kind of modes, register and edit. In register mode, 'display' is
 * fully determined by x, fix, ee and eng. In edit mode, 'display' is updated by
 * edit key strokes such as digits and '.'.
 */
typedef struct xdisplay_s {
  char display[N_N2S_MAX_SIZE];  // X, possibly being edited.
  xdisplay_mode_t mode;       // how to interpret the display.
  bool blink;                    // whether the display is blinking.

  // Data used to display X in register mode.
  n_t x;                         // register X.
  int fix;                       // fix value.
  bool ee;                       // ee mode.
  bool eng;                      // eng notation.
} xdisplay_t;


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
 * Mode: n/a -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_edit_init(xdisplay_t *d);

/**
 * Resets display to '0' and all the state except for 'fix' and 'eng.
 *
 * This function should be called when 'CLR' is pressed.
 *
 * Mode: * -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_edit_clear(xdisplay_t *d);

/**
 * Sets blink to false. In addition, if mode is edit, resets display to '0'.
 *
 * This function should be called when 'CE' is pressed.
 *
 * Mode: edit     -> XDISPLAY_MODE_EDIT_MANT.
 *       register -> register.
 */
void xdisplay_edit_clear_entry(xdisplay_t *d);

/**
 * In edit mode, adds 'digit' to display if possible.
 * In register mode, sets display to 'digit'.
 *
 * This function should be called when a digit is pressed.
 *
 * Mode: * -> edit.
 */
void xdisplay_edit_digit(xdisplay_t *d, int digit);

/**
 * In edit mode, adds '.' to display, if not already present.
 * In register mode, sets display to '0.'.
 *
 * This function should be called when '.' is pressed.
 *
 * Mode: * -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_edit_dot(xdisplay_t *d);

/**
 * Changes the sign of the mantissa or exponent.
 *
 * This function should be called when '+/-' is pressed. In register mode a
 * separate function ('s_math_chs') should also be called to change the sign of
 * X.
 *
 * Typically when a function modifies X, 'xdisplay_update_x' should be called
 * but not in this case, as we want to show '-0' and not '0', for example.
 *
 * Mode: unchanged.
 */
void xdisplay_edit_chs(xdisplay_t *d);


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
void xdisplay_mode_fix(xdisplay_t *d, int fix);

/**
 * Sets scientific mode, sets display to edit mode and possibly adds exponent.
 *
 * If there is no exponent and there is enough space, adds ' 00'. If ' 00' is
 * added or exponent is already present, sets mode to XDISPLAY_MODE_EDIT_EXP.
 * Otherwise, sets mode to XDISPLAY_MODE_EDIT_MANT.
 *
 * This should be called when 'EE' is pressed.
 *
 * Mode: * -> edit.
 */
void xdisplay_mode_ee(xdisplay_t *d);

/**
 * Unsets scientific mode.
 *
 * In edit mode, sets 'mode' to XDISPLAY_MODE_EDIT_MANT.
 *
 * This should be called when 'INV EE' is pressed.
 *
 * Mode: edit     -> XDISPLAY_MODE_EDIT_MANT.
 *       register -> register.
 */
void xdisplay_mode_iee(xdisplay_t *d);

/**
 * Sets engineering notation, possibly updating display.
 *
 * This should be called when 'Eng' is pressed.
 *
 * Mode: * -> register.
 */
void xdisplay_mode_eng(xdisplay_t *d);

/**
 * Unsets engineering notation, possibly updating display.
 *
 * This should be called when 'INV Eng' is pressed.
 *
 * Mode: * -> register.
 */
void xdisplay_mode_ieng(xdisplay_t *d);


/******************************************************************************
 *
 *  SYNCHRONIZATION DISPLAY X <=> REGISTER X.
 *
 ******************************************************************************/

/**
 * Updates display's state based on X.
 *
 * This function should be called when register X has been modified.
 *
 * Examples: 'lnx', 'RCL 59'.
 * Non examples:
 * - editing keys: '.', 'EE' or '0'.
 * - keys that keep display in edit mode: 'Deg', 'Lbl' or 'RST'.
 * - keys/operations that don't change X: 'STO 59', 'CP'
 *
 * This function should not be called when calling 'xdisplay_edit_chs'.
 *
 * Mode: * -> register.
 */
void xdisplay_update_x(xdisplay_t *d, n_t X);

/**
 * Updates 'blink' flag.
 *
 * This function should be called in case of error.
 *
 * Examples: 'STO STO', '0 lnx'.
 *
 * Mode: unchanged.
 */
void xdisplay_set_blink(xdisplay_t *d);

/**
 * Returns the new value of register X based on the number on the display.
 *
 * This function should be called when the display is in edit mode and the user
 * is done editing it.
 * 
 * The user is done editing the display when one of several keys is pressed such
 * as 'lnx', 'STO' and 'CP'. On the other hand, keys such as 'Lbl', 'Deg' and
 * '+/-', do not end editing mode.
 *
 * Note that this function does not change the state of 'd'. To do so, the
 * caller of this function needs to call 'xdisplay_update_x'. For example for
 * 'lnx':
 *   X = xdisplay_resolve_edit(&d);
 *   s_math_ln(&X, &err);
 *   xdisplay_update_x(&d, X);
 *   if (err) xdisplay_set_blink(&d);
 *
 * Mode: edit     -> edit.
 *       register -> abort.
 */
n_t xdisplay_resolve_edit(xdisplay_t *d);

#endif  // XDISPLAY_H
