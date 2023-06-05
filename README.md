# SIMPLE CPU MONITOR

reads data from "/proc/stat"
outputs average cpu/core usage as percentage on console

example build:

```bash
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
