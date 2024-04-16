#include <pthread.h>
#include <unistd.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

#include "gflags/gflags.h"

int main()
{
     std::cout << "Hello, World!\n";
}

// using namespace std::literals;

// DEFINE_uint64(num_heater_threads,
//               std::thread::hardware_concurrency(),
//               "How many threads to use when simulating hosts removal");
// DEFINE_time_s(max_run_time, 60s, "How long to run for, 0s = forever");
// DEFINE_time_us(thread_sleep_time,
//                1s,
//                "Time to sleep in each thread before next compute loop");
// DEFINE_uint64(thread_num_loops,
//               1000,
//               "Number of loops to run before next sleep");

// namespace {

// class HeaterThread {

//  public:
//   explicit HeaterThread() {}
//   void operator()(const std::stop_token& st, int threadIdx) {
//     cout << "Starting heater thread";
//     setPriority();
//     setCPUAffinity(threadIdx);

//     while (!st.stop_requested()) {
//       for (unsigned int i = 0; i < FLAGS_thread_num_loops; ++i) {
//         true;
//       }
//       // Sleep to reduce CPU impact if requested
//       std::this_thread::sleep_for(FLAGS_thread_sleep_time_us);
//     }
//     cout << "Stopping heater thread";
//   }

//  private:
//   void setPriority() {
//     pthread_t self{pthread_self()};
//     int policy;
//     struct sched_param param;

//     if (pthread_getschedparam(self, &policy, &param) != 0) {
//       XLOG(ERR) << "Error getting thread priority";
//     } else {
//       param.sched_priority = 0;
//       if (pthread_setschedparam(self, SCHED_IDLE, &param) != 0) {
//         XLOG(ERR) << "Error setting thread priority";
//       }
//     }
//   }

//   void setCPUAffinity(int threadIdx) {
//     cpu_set_t cpuset;
//     CPU_ZERO(&cpuset);
//     CPU_SET(threadIdx % sysconf(_SC_NPROCESSORS_ONLN), &cpuset);
//     if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) !=
//         0) {
//       XLOG(ERR) << "Error setting thread affinity";
//     }
//   }
// };

// std::vector<HeaterThread> threadObjects{FLAGS_num_heater_threads};
// std::vector<std::jthread> threads;

// // volatile taken from https://en.cppreference.com/w/c/program/sig_atomic_t
// volatile std::sig_atomic_t gSignalStatus;

// } // namespace

// void signal_handler(int signal) {
//   gSignalStatus = signal;
// }

// int main(int argc, char** argv) {
//   std::locale::global(std::locale("en_US.UTF-8"));
//   CHECK(gflags::ParseCommandLineFlags(&argc, &argv, true))
//       << "Failed parsing flags";

//   int threadIdx = 0;
//   for (auto& threadObject : threadObjects) {
//     threads.emplace_back(
//         std::bind_front(&HeaterThread::operator(), &threadObject), threadIdx++);
//   }

//   std::signal(SIGINT, signal_handler);
//   std::signal(SIGTERM, signal_handler);

//   auto startTime = std::chrono::system_clock::now();

//   do {
//     // Sleep to reduce impact of done checker
//     std::this_thread::sleep_for(1s);

//     printf("Ran for {} seconds",
//         std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime).count());
//   } while (gSignalStatus == 0 &&
//            (FLAGS_max_run_time_s.count() == 0 ||
//             std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime) < FLAGS_max_run_time_s));

//   for (auto& thread : threads) {
//     CHECK(thread.request_stop()) << "Thread did not stop thread";
//   }

//   for (auto& thread : threads) {
//     thread.join();
//   }

//   return 0;
// }