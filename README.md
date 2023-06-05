# SIMPLE CPU MONITOR

reads data from "/proc/stat"
outputs average cpu/core usage as percentage on console

uses ncurses

example build:

```bash
#from project directory
mkdir build
cd build/
cmake ../src
cmake --build .
```

then run:

```bash
cd ../bin
./cpumon
```
