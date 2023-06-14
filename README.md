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

tested for memory leaks using valgrind,\
note, from ncurses FAQ:\
Perhaps you used a tool such as dmalloc or valgrind to check for memory leaks. It will normally report a lot of memory still in use. That is normal.

The ncurses configure script has an option, --disable-leaks, which you can use to continue the analysis. It tells ncurses to free memory if possible. However, most of the in-use memory is "permanent".
