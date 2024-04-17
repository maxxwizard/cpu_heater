# cpu_heater
Tool for running low-priority (SCHED_IDLE) threads in a process. Originally written by @gnuthor.

# build and run
Below steps were tested on `CentOS Stream release 9` + `cmake version 3.26.5` + `gcc (GCC) 11.4.1 20231218 (Red Hat 11.4.1-3)`.

Use [CMake](https://github.com/gflags/gflags/blob/master/INSTALL.md#compiling-the-source-code-with-cmake) to configure the build tree and then `make` to build.

```
$ git clone --recurse https://github.com/maxxwizard/cpu_heater.git

$ cd cpu_heater

$ cmake -DCMAKE_CXX_STANDARD=20 .

$ make

# Run a single thread on each core forever
$ src/cpu_heater --thread_num_loops=1
```
