cmake_minimum_required(VERSION 3.21.1)

project(dict
        VERSION 0.0.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Werror
                    -Weverything
                    -Wno-c++98-compat
                    -Wno-c++98-compat-pedantic
                    -Wno-padded
                    -Wno-poison-system-directories
                    -Wno-global-constructors)

if (APPLE)
  set(CMAKE_THREAD_LIBS_INIT "-lpthread")
  set(CMAKE_HAVE_THREADS_LIBRARY 1)
  set(CMAKE_USE_WIN32_THREADS_INIT 0)
  set(CMAKE_USE_PTHREADS_INIT 1)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif ()

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address
                      -fsanitize-address-use-after-scope
                      -fno-omit-frame-pointer
                      -fno-optimize-sibling-calls)
  add_link_options(-fsanitize=address)
endif ()

#set(CMAKE_CXX_CLANG_TIDY
#    clang-tidy;)

file(GLOB_RECURSE ALL_SOURCE_FILES
     *.cpp
     *.h)
add_custom_target(clang-format
                  COMMAND clang-format
                  -i
                  --sort-includes
                  -style=file
                  ${ALL_SOURCE_FILES})

add_subdirectory(dict)
