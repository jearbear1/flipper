#include <flipper.h>

int main(int argc, char *argv[]) {
    // flipper_attach();
    struct _lf_device *fvm = flipper_attach_network("localhost");
    printf("Loaded my application!");
    gpio_enable(IO_1, 0);
    gpio_write(IO_1, 0);
    return 0;
}
