#include <flipper/flipper.h>
#include <pio/pio_sam4s16b.h>
#define IO_1 PIO_PA1

int main(int argc, char *argv[]) {
    printf("Loaded my application!");
    gpio_enable(IO_1, 0);
    gpio_write(IO_1, 0);
    return 0;
}
