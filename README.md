# cpu_heater
Tool for running low-priority (SCHED_IDLE) threads in a process. Originally written by @gnuthor.

# build
Use [CMake](https://github.com/gflags/gflags/blob/master/INSTALL.md#compiling-the-source-code-with-cmake) to configure the build tree and then `make` to build.

# run
Run a single thread forever:
```
$ ./cpu_heater --max_run_time=0s --thread_sleep_time=1us --thread_num_loops=1
```