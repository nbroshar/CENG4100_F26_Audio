/* main.c -- Case 3 acceptance harness (audio filter). Do not modify.
 *
 * Filters one block of audio and checks:
 *   1. every output sample matches the customer's golden data bit for bit,
 *   2. the block finishes within TARGET_NS (target.h), i.e. the filter keeps
 *      up with the audio arriving in real time.
 * Result through tohost: PASS, or FAIL code 1 (wrong output) or 3 (too slow).
 */
#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "case_util.h"
#include "firmware.h"
#include "data.h"
#include "target.h"

static int16_t output[N_SAMPLES];

int main(void)
{
    print_header("Case 3: audio filter");

    uint32_t t0 = TIMER_COUNT;
    fir_block(input, output, N_SAMPLES, coeffs, N_TAPS);
    uint32_t cycles = TIMER_COUNT - t0;
    uint32_t ns = cycles_to_ns(cycles);

    int wrong = 0, first = -1;
    for (int i = 0; i < N_SAMPLES; i++) {
        if (output[i] != expected[i]) {
            if (first < 0) first = i;
            wrong++;
        }
    }
    printf("Samples: %d, wrong: %d", N_SAMPLES, wrong);
    if (first >= 0)
        printf(" (first at sample %d: got %d, expected %d)", first, output[first], expected[first]);
    printf("\nCycles for the block: %u (%u per sample)\n", cycles, cycles / N_SAMPLES);
    print_us("Block time", ns);
    print_us("Target", TARGET_NS);

    if (wrong) {
        puts("RESULT: FAIL, output differs from the golden data");
        return CASE_WRONG_OUTPUT;
    }
    if (ns > TARGET_NS) {
        puts("RESULT: FAIL, slower than the target");
        return CASE_TOO_SLOW;
    }
    puts("RESULT: PASS");
    return CASE_PASS;
}
