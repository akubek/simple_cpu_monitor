#include "main.h"

int main() {
    thrd_t printer,reader;
    reader_args reader_arg;


    printf("Simple CPU Monitor\n");

    thrd_create(&reader,reader_thrd,(void *)&reader_arg);
    thrd_create(&printer,printer_thrd,NULL);
    

    thrd_join(printer,NULL);
    stop_reader();
    thrd_join(reader,NULL);

    return 0;
}