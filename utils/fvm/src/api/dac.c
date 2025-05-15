#include <flipper/flipper.h>

int dac_configure(void) {
    printf("Configured the dac.\n");
    return lf_success;
}

// Define the actual module:
struct _lf_module _dac_module = {
    .name = "dac",
};
