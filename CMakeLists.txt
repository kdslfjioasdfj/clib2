project(CLib2 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
  set(BASE_CFLAGS
    /W4
    /WX 
    /Wall
    /Za
    /std:c11
  )
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(BASE_CFLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wuninitialized
    -Wvla
    -Wconversion
    -Wsign-conversion
    -Wpointer-arith
    -Wunused
    -Wnull-dereference
    -Wcast-align
    -Wstack-protector
    -Wfloat-equal
    -Wfloat-conversion
    -Werror
  )
endif()

set(SRCS
  # ...
)

add_library(CLib2Static STATIC ${SRCS})
add_library(CLib2Dynamic SHARED ${SRCS})
target_compile_options(CLib2Static PRIVATE ${BASE_CFLAGS})
target_compile_options(CLib2Dynamic PRIVATE ${BASE_CFLAGS})