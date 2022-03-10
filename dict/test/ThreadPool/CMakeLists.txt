add_executable(ThreadPoolTest
               ThreadPool.cpp)
target_link_libraries(ThreadPoolTest PRIVATE
                      ThreadPool
                      GTest::Main)
