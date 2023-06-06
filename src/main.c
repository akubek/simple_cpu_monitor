#include "main.h"

int main() {
    thrd_t analyzer,printer,reader;
    cpustat_queue analyzer_q, reader_logger_q;
    cpuperc_queue printer_q, analyzer_logger_q;
    init_cpustatq(&analyzer_q); init_cpustatq(&reader_logger_q);
    init_cpupercq(&printer_q); init_cpupercq(&analyzer_logger_q);
    reader_args reader_arg = {&analyzer_q,&reader_logger_q};
    analyzer_args analyzer_arg = {&analyzer_q,&printer_q,&analyzer_logger_q};


    printf("Simple CPU Monitor\n");

    thrd_create(&reader,reader_thrd,(void *)(&reader_arg));
    thrd_create(&analyzer,analyzer_thrd,(void *)&analyzer_arg);
    //thrd_create(&printer,printer_thrd,NULL);
    

    //thrd_join(printer,NULL);
    //stop_reader();
    //thrd_join(reader,NULL);
    //stop_analyzer();
    thrd_join(analyzer,NULL);

    return 0;
}