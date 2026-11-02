/* firmware.c -- the customer's shipping audio-filter firmware (Case 3).
 *
 * A 16-tap FIR boost filter with an output limiter. It is fast enough on the
 * customer's old chip, but listeners hear crackle on loud sounds: some output
 * samples are wrong. (An engineer tried 64-bit math everywhere; the crackle
 * went away, but the filter fell behind the audio, so that version never
 * shipped.) Your team may rewrite this file (and change the hardware) to meet
 * the requirements in the case brief: every output sample must match the
 * definition in gen_data.py bit for bit, and the block must finish in time.
 */
#include <stdint.h>
#include "firmware.h"

void fir_block(const int16_t *x, int16_t *y, int n, const int16_t *h, int taps)
{
    for (int i = 0; i < n; i++) {
        int32_t acc = 0;
        for (int k = 0; k < taps; k++) {
            if (i - k >= 0)
                acc += x[i - k] * h[k];
        }
        int32_t v = acc >> 15;
        if (v > 32767)
            v = 32767;
        else if (v < -32768)
            v = -32768;
        y[i] = (int16_t)v;
    }
}
