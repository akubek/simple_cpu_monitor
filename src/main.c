#include "main.h"

int main() {
    thrd_t analyzer,printer,reader;
    cpustat_queue analyzer_q, reader_logger_q;
    cpuperc_queue printer_q, analyzer_logger_q;
    init_cpustatq(&analyzer_q); init_cpustatq(&reader_logger_q);
    init_cpupercq(&printer_q); init_cpupercq(&analyzer_logger_q);
    reader_args rargs = {&analyzer_q,&reader_logger_q};
    analyzer_args aargs = {&analyzer_q,&printer_q,&analyzer_logger_q};
    printer_args pargs = {&printer_q};


    printf("Simple CPU Monitor\n");

    thrd_create(&reader,reader_thrd,(void *)(&rargs));
    thrd_create(&analyzer,analyzer_thrd,(void *)&aargs);
    thrd_create(&printer,printer_thrd,(void *)&pargs);
    

    thrd_join(printer,NULL);
    stop_reader();
    thrd_join(reader,NULL);
    stop_analyzer();
    thrd_join(analyzer,NULL);

    return 0;
}