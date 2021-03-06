/**
 * xdisplay.h
 *
 * In charge of displaying and editing register X.
 */

#ifndef XDISPLAY_H
#define XDISPLAY_H

#include "n59.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

/** Indicates whether register X is being displayed or edited. */
typedef enum xdisplay_mode_e {
  XDISPLAY_MODE_REG,             // X is being displayed.
  XDISPLAY_MODE_EDIT_MANT,       // X's mantissa is being edited.
  XDISPLAY_MODE_EDIT_EXP         // X's exponent is being edited.
} xdisplay_mode_t;

/** The display state. */
typedef struct xdisplay_s {
  char display[N_N2S_MAX_SIZE];  // string on the display, such as '1.2'.
  xdisplay_mode_t mode;          // whether X is being edited or displayed.
  bool overflow;                 // whether there is overflow.

  n_t reg_x;                     // register X.
  int fix;                       // fix value.
  bool ee;                       // ee mode.
  bool eng;                      // eng notation.
} xdisplay_t;


/******************************************************************************
 *
 *  INIT & CLEAR.
 *
 ******************************************************************************/

/**
 * Inits display.
 *
 * Called when TI-59 is turned on.
 *
 * Mode: n/a -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_init(xdisplay_t *x);

/**
 * Resets 'x' except for 'fix' and 'eng'.
 *
 * Called on 'CLR'.
 *
 * Mode: * -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_clear(xdisplay_t *x);

/**
 * In edit mode, resets display to '0'.
 *
 * Called on 'CE'.
 *
 * Mode: edit     -> XDISPLAY_MODE_EDIT_MANT.
 *       register -> register.
 */
void xdisplay_clear_entry(xdisplay_t *x);

/******************************************************************************
 *
 *  EDIT.
 *
 ******************************************************************************/

/**
 * In edit mode, adds 'd' to number on display if possible.
 * In register mode, sets display to 'd'.
 *
 * Called on '0'-'9'.
 *
 * Mode: * -> edit.
 */
void xdisplay_digit(xdisplay_t *x, int d);

/**
 * In edit mode, adds '.' to number on display, if not already present.
 * In register mode, sets display to '0.'.
 *
 * Called on '.'.
 *
 * Mode: * -> XDISPLAY_MODE_EDIT_MANT.
 */
void xdisplay_dot(xdisplay_t *x);

/**
 * Changes the sign of the mantissa or exponent.
 *
 * In register mode a separate function ('s_math_chs') should also be called to
 * change the sign of X.
 *
 * Typically when a function modifies X, 'xdisplay_update_reg_x' should be
 * called but not in this case, as we want to show '-0' and not '0', for
 * example.
 *
 * Called on '+/-'.
 *
 * Mode: unchanged.
 */
void xdisplay_chs(xdisplay_t *x);

/******************************************************************************
 *
 *  MODES.
 *
 ******************************************************************************/

/**
 * Sets fix and updates display.
 *
 * Called on 'Fix d'.
 *
 * Mode * -> register.
 */
void xdisplay_fix(xdisplay_t *x, int fix);

/**
 * Sets scientific mode. Additionally, sets display to edit mode and possibly
 * adds exponent.
 *
 * If there is no exponent and there is enough space, adds exponent '00'. If
 * exponent is added or is already present, sets mode to XDISPLAY_MODE_EDIT_EXP.
 * Otherwise, sets mode to XDISPLAY_MODE_EDIT_MANT.
 *
 * Called on 'EE'.
 *
 * Mode: * -> edit.
 */
void xdisplay_ee(xdisplay_t *x);

/**
 * Unsets scientific mode.
 *
 * In edit mode, sets 'mode' to XDISPLAY_MODE_EDIT_MANT.
 *
 * Called on 'INV EE'.
 *
 * Mode: edit     -> XDISPLAY_MODE_EDIT_MANT.
 *       register -> register.
 */
void xdisplay_iee(xdisplay_t *x);

/**
 * Sets engineering notation, possibly updating display.
 *
 * Called on 'Eng'.
 *
 * Mode: * -> register.
 */
void xdisplay_eng(xdisplay_t *x);

/**
 * Unsets engineering notation, possibly updating display.
 *
 * Called on 'INV Eng'.
 *
 * Mode: * -> register.
 */
void xdisplay_ieng(xdisplay_t *x);


/******************************************************************************
 *
 * SYNCHRONIZATION WITH REGISTER X.
 *
 ******************************************************************************/

/**
 * Sets 'reg_x', possibly updating display.
 *
 * Called when register X has been modified.
 *
 * Examples: 'lnx', 'RCL 59'.
 * Non examples:
 * - editing keys: '.', 'EE' or '0'.
 * - keys that keep display in edit mode: 'Deg', 'Lbl' or 'RST'.
 * - keys/operations that don't change X: 'STO 59', 'CP'
 *
 * Note: this function should not be called when calling 'xdisplay_chs'.
 *
 * Mode: * -> register.
 */
void xdisplay_update_reg_x(xdisplay_t *x, n_t X);

/**
 * Resolves edit mode by determining reg_x based on the number on the display.
 * The display is then updated based on the new value of reg_x.
 * For example if display is '12 12' and fix is 9 then
 * reg_x = { 1200000000000, 13 } and display becomes '1.2 13'.
 *
 * This operation can result in 2 errors. If the number on the display does
 * not fit an n_t number ('99 99 or '0.9-99' for example).
 *
 * This function should be called when the display is in edit mode and the user
 * is done editing it.
 *
 * The user is done editing the display when one of several keys is pressed such
 * as 'lnx', 'STO' and 'CP'. On the other hand, keys such as 'Lbl', 'Deg' and
 * '+/-', do not end editing mode.
 *
 * For example for 'lnx':
 *   X = xdisplay_resolve_edit(&x);
 *   s_math_ln(&X, &err);
 *   xdisplay_update_reg_x(&x, X);
 *   if (err) ...;
 *
 * Mode: edit     -> register.
 *       register -> abort.
 */
void xdisplay_resolve_edit(xdisplay_t *x, n_err_t *err);

#endif  // XDISPLAY_H
