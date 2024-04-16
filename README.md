# cpu_heater
Tool for running low-priority (SCHED_IDLE) threads in a process. Originally written by @gnuthor.

# build
Any G++ toolchain should work.

On MacOS:
```
$ make
g++ -g -Wall -o cpu_heater main.cpp
Build complete
```

# run
Run a single thread forever:
```
$ ./cpu_heater --max_run_time=0s --thread_sleep_time=1us --thread_num_loops=1
```