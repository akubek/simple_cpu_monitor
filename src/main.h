#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <threads.h>
#include <unistd.h>

#include "./threads/analyzer.h"
#include "./threads/printer.h"
#include "./threads/reader.h"
#include "./threads/logger.h"
#include "./threads/watchdog.h"
#include "./dataStructures/cpustat_queue.h"
#include "./dataStructures/cpuperc_queue.h"
#include "./dataStructures/log_queue.h"
