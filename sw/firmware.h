/* firmware.h -- interface between the acceptance harness and the firmware. */
#ifndef CASE3_FIRMWARE_H
#define CASE3_FIRMWARE_H

#include <stdint.h>

/* Filter n samples of x into y with the taps in h (see gen_data.py for the
 * exact definition the output must match). */
void fir_block(const int16_t *x, int16_t *y, int n, const int16_t *h, int taps);

#endif
