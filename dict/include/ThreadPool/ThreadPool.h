#ifndef THREAD_POOL_THREAD_POOL_H
#define THREAD_POOL_THREAD_POOL_H

#include <array>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <type_traits>

namespace ThreadPool {
class ThreadPool final {
 public:
  ThreadPool();
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool(const ThreadPool &) = delete;
  auto operator=(ThreadPool &&) noexcept -> ThreadPool & = delete;
  auto operator=(const ThreadPool &) -> ThreadPool & = delete;
  ~ThreadPool();

  template <typename FnTy, typename... ArgTys,
            typename ResTy = std::invoke_result_t<FnTy, ArgTys...>>
  auto create_task(FnTy fn, ArgTys... args) -> std::future<ResTy> {
    std::shared_ptr<std::promise<ResTy>> promise =
        std::make_shared<std::promise<ResTy>>();
    std::future<ResTy> future{promise->get_future()};

    std::unique_lock<std::mutex> tasks_lk{tasks_mtx_};
    tasks_.push([promise = std::move(promise), fn = std::move(fn),
                 ... args = std::move(args)]() mutable {
      promise->set_value(std::invoke(fn, args...));
    });
    tasks_lk.unlock();
    task_available_.notify_one();

    return future;
  }

  static constexpr std::size_t workers_cnt{HARDWARE_CONCURRENCY};

 private:
  void worker_loop();

  std::atomic_size_t workers_alive_{workers_cnt};
  std::array<std::thread, workers_cnt> workers_{};
  std::queue<std::function<void()>> tasks_;
  std::mutex tasks_mtx_;
  std::condition_variable task_available_;
  std::atomic<bool> finish_{};
};
} /* namespace ThreadPool */

#endif /* THREAD_POOL_THREAD_POOL_H */
