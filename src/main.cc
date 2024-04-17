#include <pthread.h>
#include <unistd.h>

#include <chrono>
#include <csignal>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "gflags/gflags.h"

using namespace std::literals;
using namespace std::chrono;
using namespace std::chrono_literals;

DEFINE_uint64(num_heater_threads,
              std::thread::hardware_concurrency(),
              "How many threads to run");
DEFINE_uint64(thread_num_loops,
              1000,
              "Number of loops to run before next sleep");

namespace {

class HeaterThread {

 public:
  explicit HeaterThread() {}
  void operator()(const std::stop_token& st, int threadIdx) {
    std::cout << "Starting heater thread" << std::endl;
    setPriority();
    setCPUAffinity(threadIdx);

    while (!st.stop_requested()) {
      for (unsigned int i = 0; i < FLAGS_thread_num_loops; ++i) {
        true;
      }
      // Sleep to reduce CPU impact (tweakable)
      std::this_thread::sleep_for(1us);
    }
    std::cout << "Stopping heater thread" << std::endl;
  }

 private:
  void setPriority() {
    pthread_t self{pthread_self()};
    int policy;
    struct sched_param param;

    if (pthread_getschedparam(self, &policy, &param) != 0) {
      std::cerr << "Error getting thread priority" << std::endl;
    } else {
      param.sched_priority = 0;
      if (pthread_setschedparam(self, SCHED_IDLE, &param) != 0) {
        std::cerr << "Error setting thread priority" << std::endl;
      }
    }
  }

  void setCPUAffinity(int threadIdx) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(threadIdx % sysconf(_SC_NPROCESSORS_ONLN), &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) !=
        0) {
      std::cerr << "Error setting thread affinity" << std::endl;
    }
   }
};

std::vector<HeaterThread> threadObjects{FLAGS_num_heater_threads};
std::vector<std::jthread> threads;

// volatile taken from https://en.cppreference.com/w/c/program/sig_atomic_t
volatile std::sig_atomic_t gSignalStatus;

} // namespace

void signal_handler(int signal) {
  gSignalStatus = signal;
}

int main(int argc, char** argv) {
  std::locale::global(std::locale("en_US.UTF-8"));
  if (!gflags::ParseCommandLineFlags(&argc, &argv, true)) {
     std::cerr << "Failed parsing flags" << std::endl;
     exit(1);
  }

  int threadIdx = 0;
  for (auto& threadObject : threadObjects) {
    threads.emplace_back(
        std::bind_front(&HeaterThread::operator(), &threadObject), threadIdx++);
  }

  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  auto startTime = std::chrono::system_clock::now();

  do {
    // Sleep to reduce impact of done checker
    std::this_thread::sleep_for(1s);

    std::cout << "Ran for " << duration_cast<seconds>(system_clock::now() - startTime).count() << " seconds" << std::endl;
  } while (gSignalStatus == 0);

  for (auto& thread : threads) {
    if (!thread.request_stop()) {
     std::cout << "Thread did not stop thread" << std::endl;
     exit(1);
    }
  }

  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << "Done!" << std::endl;

  return 0;
}
