add_library(ThreadPool STATIC
            ThreadPool.cpp)

include(ProcessorCount)
ProcessorCount(HARDWARE_CONCURRENCY)
target_compile_definitions(ThreadPool PUBLIC
                           HARDWARE_CONCURRENCY=${HARDWARE_CONCURRENCY})
