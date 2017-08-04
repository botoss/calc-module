cmake_minimum_required(VERSION 3.0)
project(calc-module)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads)
find_package(Boost COMPONENTS log log_setup thread REQUIRED)

include_directories(
	.
	${Threads_INCLUDE_DIR}
	SYSTEM
	${RDKAFKA_INCLUDE_DIR}
	${CPPKAFKA_DIR}/include
	${Boost_INCLUDE_DIRS}
)

add_executable(calc-module message.cpp kafka_connector.cpp main.cpp)

target_link_libraries(
	calc-module

	${CPPKAFKA_DIR}/build/lib/libcppkafka.a
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	-lpthread
	-lrdkafka
)

