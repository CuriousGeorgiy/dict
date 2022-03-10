#include "ThreadPool/ThreadPool.h"

namespace ThreadPool {
ThreadPool::ThreadPool() {
  std::generate(workers_.begin(), workers_.end(), [this]() {
    return std::thread(&ThreadPool::worker_loop, this);
  });
}

ThreadPool::~ThreadPool() {
  finish_ = true;

  while (workers_alive_ != 0) {
    task_available_.notify_all();
  }

  for (auto &worker : workers_) {
    worker.join();
  }
}

void ThreadPool::worker_loop() {
  while (true) {
    std::unique_lock<std::mutex> work_available_lock(tasks_mtx_);
    task_available_.wait(work_available_lock);

    if (finish_) {
      --workers_alive_;
      return;
    }

    tasks_.front()();
    tasks_.pop();
  }
}
} /* namespace ThreadPool */
