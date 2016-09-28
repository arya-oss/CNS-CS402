#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t start_state = 0xE1u;
    uint8_t lfsr = start_state;
    uint8_t bit;
    unsigned period = 0;

    do {
        bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1;
        lfsr = (lfsr >> 1) | (bit << 15);
        ++period;
        if ( lfsr < 0x10)
            printf("0%x", lfsr);
        else
            printf("%2x", lfsr);
    } while (lfsr != start_state && lfsr != 0x00u);

    return 0;
}
