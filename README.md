# SIMPLE CPU MONITOR

**Example project!**\
On Linux reads data from "/proc/stat" and outputs average cpu/core usage as percentage on console.

uses [NCURSES](https://invisible-island.net/ncurses/)

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
