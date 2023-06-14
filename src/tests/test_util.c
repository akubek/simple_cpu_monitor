#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "../common/cpumon_util.h"

struct timespec t1 = {100, 100};
struct timespec t2 = {100, 200};
struct timespec t3 = {200, 100};

int main() {
    bool result;
    result = compare_timespec(t2, t1);
    assert(result == true);
    result = compare_timespec(t2, t3);
    assert(result == false);
    result = compare_timespec(t3, t1);
    assert(result == true);
    return 0;
}