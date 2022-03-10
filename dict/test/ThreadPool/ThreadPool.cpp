#include "ThreadPool/ThreadPool.h"

#include <gtest/gtest.h>

#include <array>

TEST(ThreadPool, DefaultCtor) {  // NOLINT
  ThreadPool::ThreadPool thread_pool;
}

TEST(ThreadPool, CreateTask) {  // NOLINT
  ThreadPool::ThreadPool thread_pool;

  auto future = thread_pool.create_task([]() { return 0; });
  ASSERT_TRUE(future.valid());
  ASSERT_EQ(future.get(), 0);

  std::array<decltype(future), HARDWARE_CONCURRENCY> futures;
  std::generate(futures.begin(), futures.end(), [&thread_pool]() {
    return thread_pool.create_task([]() { return 0; });
  });


}
