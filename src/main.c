#include "main.h"

int main() {
    thrd_t printer;

    printf("Simple CPU Monitor\n");

    thrd_create(&printer,printer_thrd,NULL);

    thrd_join(printer,NULL);

    return 0;
}